#include "AT24C64.h"



void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, BitValue);
	MyDelay_us(100);
}


void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, BitValue);
	MyDelay_us(100);

}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin);
	MyDelay_us(100);
	return BitValue;

}


//起始条件
void MyI2C_Start(void)
{
	SDA_OUT();
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);

	MyI2C_W_SDA(0);
	MyDelay_us(10);

	MyI2C_W_SCL(0);



}


void MyI2C_Stop(void)
{
	SDA_OUT();
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyDelay_us(10);

	MyI2C_W_SDA(1);



}


void MyI2C_SendByte(uint8_t Byte)
{
	SDA_OUT();
	uint8_t i;

	//Serial_Printf("Byte = %x\n", Byte);

	for (i = 0; i < 8; i++)
	{

		MyI2C_W_SDA(!!(Byte & (0x80 >> i)));
		//Serial_Printf("Byte[%i] = %x\n", i, !!(Byte & (0x80 >> i)));

		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);


	}


}


uint8_t MyI2C_ReceiveByte(void)
{
	MyI2C_W_SDA(1);
	SDA_IN();

	uint8_t i,Byte = 0x00;

	for (i = 0; i < 8; i++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA()){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}

	return Byte;
}



void MyI2C_SendAck(uint8_t AckBit)
{
	SDA_OUT();

	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);

}

uint8_t MyI2C_ReceiveAck(void)
{
	SDA_OUT();
	uint8_t AckBit;
	MyI2C_W_SDA(1);

	SDA_IN();
	MyI2C_W_SCL(1);

	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}

uint8_t AT24C64_WriteOneByte(uint16_t WriteAddr, uint8_t Data)
{
	uint32_t timeout;

	//如果输入地址超出最大地址，返回2
	if(WriteAddr > (AT24C64_MAX_ADDR))
	{
		return 2;
	}


	MyI2C_Start();
	MyI2C_SendByte(AT24C64_Write_Addr); //发送设备地址（写）
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 1;}	//等待应答位
	MyI2C_SendByte((WriteAddr >> 8) & 0x1F); //发送高字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 1;}	//等待应答位
	MyI2C_SendByte(WriteAddr & 0xFF); //发送低字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 1;}	//等待应答位
	MyI2C_SendByte(Data); //发送数据
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 1;}	//等待应答位
	MyI2C_Stop();         //结束


	timeout = 1000;
	while(timeout--)
	{
		 // 尝试发送设备地址
		 MyI2C_Start();
		 MyI2C_SendByte(AT24C64_Write_Addr);
		 if(MyI2C_ReceiveAck() == 0)	//等待应答位
		 {
			 //Ack为0，表示写入完成

			 MyI2C_Stop();
			 return 0;

		 }


		 MyI2C_Stop();  //未收到Ack，继续等待
		 MyDelay_ms(1);

	}

	return 1;  //返回1：超时，写入失败
}


uint8_t AT24C64_ReadOneByte(uint16_t ReadAddr, uint8_t *pData)
{

	//如果输入地址超出最大地址，返回2
	if (pData == NULL) return 1;                 // 指针无效
	if (ReadAddr > AT24C64_MAX_ADDR) return 2;             // 地址超范围

	MyI2C_Start();
	MyI2C_SendByte(AT24C64_Write_Addr); //发送设备地址（写）
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 4;}	//等待应答位
	MyI2C_SendByte((ReadAddr >> 8) & 0x1F); //发送高字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 5;}	//等待应答位
	MyI2C_SendByte(ReadAddr & 0xFF); //发送低字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 6;}	//等待应答位

	MyI2C_Start();
	MyI2C_SendByte(AT24C64_Read_Addr); //发送设备地址（读）
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 7;}	//等待应答位
	*pData = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);      //发送应答位
	MyI2C_Stop();         //结束

	//返回0，表示数据读取成功
	return 0;

}

/*
 * @brief  页写入(无跨页处理)
 *
 *
 * */
uint8_t AT24C64_WritePage(uint16_t WriteAddr, uint8_t *pData, uint16_t Len)
{
	uint16_t i;
	uint32_t timeout;

	//判断页大小，是否跨页
	if(Len > 32) return 6;

	MyI2C_Start();
	MyI2C_SendByte(AT24C64_Write_Addr); //发送设备地址（写）
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 2;}	//等待应答位
	MyI2C_SendByte((WriteAddr >> 8) & 0x1F); //发送高字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 3;}	//等待应答位
	MyI2C_SendByte(WriteAddr & 0xFF); //发送低字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 4;}	//等待应答位

	for(i = 0; i < Len; i++)
	{
		MyI2C_SendByte(pData[i]); //发送数据
		if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 5;}	//等待应答位

	}

	MyI2C_Stop();


	timeout = 1000;
	while(timeout--)
	{
		 // 尝试发送设备地址
		 MyI2C_Start();
		 MyI2C_SendByte(AT24C64_Write_Addr);
		 if(MyI2C_ReceiveAck() == 0)	//等待应答位
		 {
			 //Ack为0，表示写入完成

			 MyI2C_Stop();
			 return 0;

		 }


		 MyI2C_Stop();  //未收到Ack，继续等待
		 MyDelay_ms(1);

	}

	//超时
	return 1;


}

/*
 * @brief  数据连续写入（可跨页处理）
 *
 * */
uint8_t AT24C64_WriteBytes(uint16_t WriteAddr, uint8_t *pData, uint16_t Len)
{
	uint16_t i = 0;
	uint16_t page_remaining;
	uint16_t write_len;
	uint8_t status;
	uint32_t end_addr;

	if (pData == NULL) return 1;
	end_addr = (uint32_t)WriteAddr + (uint32_t)Len;
	if (end_addr > AT24C64_SIZE) return 2;

	while( Len > 0)
	{
		//计算当前写入的页还能存储多少个字节
		page_remaining = AT24C64_PAGE_SIZE - (WriteAddr % AT24C64_PAGE_SIZE);
		write_len = (Len < page_remaining) ? Len : page_remaining;

		status = AT24C64_WritePage(WriteAddr, pData + i, write_len);
		if (status != 0) return status;

		Len -= write_len;
		i += write_len;
		WriteAddr += write_len;

	}

	return 0;

}



/*
 *  @brief  连续读取多个字节
 *
 *
 * */
uint8_t AT24C64_ReadBytes(uint16_t ReadAddr, uint8_t *pData, uint16_t Len)
{
	uint16_t i;
	uint32_t end_addr;

	//如果输入地址超出最大地址，返回2
	if (pData == NULL) return 1;                 // 指针无效
	end_addr = (uint32_t)ReadAddr + (uint32_t)Len;
	if (end_addr > AT24C64_SIZE) return 2;             // 地址超范围

	MyI2C_Start();
	MyI2C_SendByte(AT24C64_Write_Addr); //发送设备地址（写）
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 4;}	//等待应答位
	MyI2C_SendByte((ReadAddr >> 8) & 0x1F); //发送高字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 5;}	//等待应答位
	MyI2C_SendByte(ReadAddr & 0xFF); //发送低字节
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 6;}	//等待应答位

	MyI2C_Start();
	MyI2C_SendByte(AT24C64_Read_Addr);
	if(MyI2C_ReceiveAck() != 0)	{ MyI2C_Stop(); return 7;}	//等待应答位

	for(i = 0; i < Len; i++)
	{
		pData[i] = MyI2C_ReceiveByte();

		MyI2C_SendAck( (i == Len - 1) ? 1 : 0);      //发送应答位
	}

	MyI2C_Stop();

	return 0;

}


/**
 * @brief  检测 AT24C64 是否在线
 * @retval 0 存在，1 不存在
 */
uint8_t AT24C64_Check(void)
{
    MyI2C_Start();
    MyI2C_SendByte(AT24C64_Write_Addr);
    uint8_t ack = MyI2C_ReceiveAck();
    MyI2C_Stop();
    return (ack != 0) ? 1 : 0;
}













