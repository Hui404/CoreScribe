/*
  serial.c - Low level functions for sending and recieving bytes via the serial port
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"
#include "usart.h"




uint8_t serial_rx_buffer[RX_RING_BUFFER];
uint8_t serial_rx_buffer_head = 0;
volatile uint8_t serial_rx_buffer_tail = 0;

uint8_t serial_tx_buffer[TX_RING_BUFFER];
uint8_t serial_tx_buffer_head = 0;
volatile uint8_t serial_tx_buffer_tail = 0;



// Returns the number of bytes available in the RX serial buffer.
uint8_t serial_get_rx_buffer_available()
{
  uint8_t rtail = serial_rx_buffer_tail; // Copy to limit multiple calls to volatile
  if (serial_rx_buffer_head >= rtail) { return(RX_BUFFER_SIZE - (serial_rx_buffer_head-rtail)); }
  return((rtail-serial_rx_buffer_head-1));
}


// Returns the number of bytes used in the RX serial buffer.
// NOTE: Deprecated. Not used unless classic status reports are enabled in config.h.
uint8_t serial_get_rx_buffer_count()
{
  uint8_t rtail = serial_rx_buffer_tail; // Copy to limit multiple calls to volatile
  if (serial_rx_buffer_head >= rtail) { return(serial_rx_buffer_head-rtail); }
  return (RX_BUFFER_SIZE - (rtail-serial_rx_buffer_head));
}


// Returns the number of bytes used in the TX serial buffer.
// NOTE: Not used except for debugging and ensuring no TX bottlenecks.
uint8_t serial_get_tx_buffer_count()
{
  uint8_t ttail = serial_tx_buffer_tail; // Copy to limit multiple calls to volatile
  if (serial_tx_buffer_head >= ttail) { return(serial_tx_buffer_head-ttail); }
  return (TX_RING_BUFFER - (ttail-serial_tx_buffer_head));
}


void serial_init()
{
	huart1.Init.BaudRate = 115200; //设置波特率为115200

	huart1.Instance->CR1 |= USART_CR1_TE | USART_CR1_RE; //使能发送和接收
	USART1->CR1 |= USART_CR1_RXNEIE; //使能接收中断

	__HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);//禁用发送中断
}







// Writes one byte to the TX serial buffer. Called by main program.
void serial_write(uint8_t data) {
  // Calculate next head
  uint8_t next_head = serial_tx_buffer_head + 1;

  if (next_head == TX_RING_BUFFER) { next_head = 0; }

  // Wait until there is space in the buffer
  while (next_head == serial_tx_buffer_tail) {
    // TODO: Restructure st_prep_buffer() calls to be executed here during a long print.
    if (sys_rt_exec_state & EXEC_RESET) { return; } // Only check for abort to avoid an endless loop.
  }

  // Store data and advance head
  serial_tx_buffer[serial_tx_buffer_head] = data;
  serial_tx_buffer_head = next_head;

  // Enable Data Register Empty Interrupt to make sure tx-streaming is running
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE); //使能发送寄存器空中断
}





// Fetches the first byte in the serial read buffer. Called by main program.
// 获取串口读取缓冲区中的第一个字节。由主程序调用。
uint8_t serial_read()
{
  uint8_t tail = serial_rx_buffer_tail; // Temporary serial_rx_buffer_tail (to optimize for volatile)
  if (serial_rx_buffer_head == tail)
  {
    return SERIAL_NO_DATA;
  }
  else
  {
    uint8_t data = serial_rx_buffer[tail];

    tail++;
    if (tail == RX_RING_BUFFER) { tail = 0; }
    serial_rx_buffer_tail = tail;

    return data;
  }


}


//手动往缓冲区写入一个字符
uint8_t serial_Inject_char(uint8_t c)
{
	uint8_t next_head = (serial_rx_buffer_head + 1) & (RX_RING_BUFFER - 1);

	if(next_head != serial_rx_buffer_tail)	//如果头指针等于尾指针，视为缓冲区未满
	{
		serial_rx_buffer[serial_rx_buffer_head] = c;	//将字符放入缓冲区
		serial_rx_buffer_head = next_head; //更新头指针

		return 1; //返回1，表示成功

	}

	return 0; //返回0， 表示缓冲区已满

}


// 手动写入完整的G指令
void serial_manual_Write(const char* message)
{
	uint8_t i;

	//Serial_Printf("%s", message);

	for(i = 0; i < strlen(message); i++)
	{

		serial_Inject_char(message[i]);

	}


}





void serial_reset_read_buffer()
{
  serial_rx_buffer_tail = serial_rx_buffer_head;
}



/*--------------------------------------------------*/



void Serial_SendByte(uint8_t Byte)
{
	USART1->DR = Byte;
	while(!(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)));

}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}


}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);

	}


}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;


	}

	return Result;

}


void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i = 0; i < Length; i++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + 0x30);

	}

}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;

}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String );


}










