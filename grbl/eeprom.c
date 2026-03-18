// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* \li File:               eeprom.c
* \li Compiler:           IAR EWAAVR 3.10c
* \li Support mail:       avr@atmel.com
*
* \li Supported devices:  All devices with split EEPROM erase/write
*                         capabilities can be used.
*                         The example is written for ATmega48.
*
* \li AppNote:            AVR103 - Using the EEPROM Programming Modes.
*
* \li Description:        Example on how to use the split EEPROM erase/write
*                         capabilities in e.g. ATmega48. All EEPROM
*                         programming modes are tested, i.e. Erase+Write,
*                         Erase-only and Write-only.
*
*                         $Revision: 1.6 $
*                         $Date: Friday, February 11, 2005 07:16:44 UTC $
****************************************************************************/

#include <AT24C64.h>
#include "settings.h"


unsigned char eeprom_get_char( unsigned int addr )
{
	unsigned char *p;
	unsigned char data = 0;
	p = &data;
//	unsigned char data_0;
	//cli(); //关闭总中断，防止打断数据写入
	data = AT24C64_ReadOneByte(addr, p);

	//sei();	//开启总中断
	return data;

}


void eeprom_put_char( unsigned int addr, unsigned char new_value )
{
	cli(); //关闭总中断，防止打断数据写入
	if(AT24C64_WriteOneByte(addr, new_value) != 0)
	{
			//写入失败
			sei();	//开启总中断
			serial_write('A');
			serial_write('T');
			serial_write('2');
			serial_write('4');
			serial_write('C');
			serial_write('0');
			serial_write('2');
			serial_write('E');
			serial_write('E');
			serial_write('R');
			serial_write('O');
			serial_write('M');

	}
	else
	{
		sei();	//开启总中断
		MyDelay_ms(10); //写入成功 延时10ms
	}


}

void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size) {
  unsigned char checksum = 0;
  for(; size > 0; size--) { 
    checksum = (checksum << 1) | (checksum >> 7);
    checksum += *source;
    eeprom_put_char(destination++, *(source++)); 
  }
  eeprom_put_char(destination, checksum);
}

int memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size) {
  unsigned char data, checksum = 0;
  for(; size > 0; size--) { 
    data = eeprom_get_char(source++);
    checksum = (checksum << 1) | (checksum >> 7);
    checksum += data;    
    *(destination++) = data; 
  }
  return(checksum == eeprom_get_char(source));
}

// end of file
