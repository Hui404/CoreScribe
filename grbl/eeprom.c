#include <AT24C64.h>
#include "settings.h"

unsigned char eeprom_get_char(unsigned int addr)
{
  unsigned char data = 0;
  uint8_t status = AT24C64_ReadOneByte(addr, &data);
  (void)status;
  return data;
}

void eeprom_put_char(unsigned int addr, unsigned char new_value)
{
  cli();
  if (AT24C64_WriteOneByte(addr, new_value) != 0) {
    sei();
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
  } else {
    sei();
    MyDelay_ms(10);
  }
}

void memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size)
{
  unsigned char checksum = 0;
  for (; size > 0; size--) {
    checksum = (checksum << 1) | (checksum >> 7);
    checksum += *source;
    eeprom_put_char(destination++, *(source++));
  }
  eeprom_put_char(destination, checksum);
}

int memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size)
{
  unsigned char data, checksum = 0;
  for (; size > 0; size--) {
    data = eeprom_get_char(source++);
    checksum = (checksum << 1) | (checksum >> 7);
    checksum += data;
    *(destination++) = data;
  }
  return (checksum == eeprom_get_char(source));
}
