/*
 * File:   led_board_driver.c
 * Author: jakub
 *
 * Created on 6. kv?tna 2024, 15:27
 */

#include "../temp_sensor_driver.h"

#if DEVICE_TYPE == DEVICE_TYPE_TEMP_SENSOR

__bit ds18b20_start()
{
  DS18B20_SetLow();      // send reset pulse to the DS18B20 sensor
  DS18B20_SetDigitalOutput();  // configure DS18B20_PIN pin as output
  __delay_us(1100);      // wait 500 us
 
  DS18B20_SetDigitalInput();  // configure DS18B20_PIN pin as input
  __delay_us(70);      // wait 100 us to read the DS18B20 sensor response
 
  if (!DS18B20_GetValue())
  {
    __delay_us(400);    // wait 400 us
    return 1;           // DS18B20 sensor is present
  }
 
  return 0;   // connection error
}
 
void ds18b20_write_bit(uint8_t value)
{
  DS18B20_SetLow();
  DS18B20_SetDigitalOutput();  // configure DS18B20_PIN pin as output
  __delay_us(2);        // wait 2 us
 
  DS18B20_LAT = (__bit)value;
  __delay_us(100);       // wait 80 us
 
  DS18B20_SetDigitalInput();  // configure DS18B20_PIN pin as input
  __delay_us(2);        // wait 2 us
}
 
void ds18b20_write_byte(uint8_t value)
{
  for(uint8_t i = 0; i < 8; i++)
    ds18b20_write_bit(value >> i);
}
 
__bit ds18b20_read_bit(void)
{
  static __bit value;
 
  DS18B20_SetLow();
  DS18B20_SetDigitalOutput();  // configure DS18B20_PIN pin as output
  __delay_us(6);
 
  DS18B20_SetDigitalInput();  // configure DS18B20_PIN pin as input
  __delay_us(9);        // wait 5 us
 
  value = DS18B20_GetValue();  // read and store DS18B20 state 
  
  return value;
}
 
uint8_t ds18b20_read_byte(void)
{
  uint8_t value = 0;
 
  for(uint8_t i = 0; i < 8; i++)
  {
      value |= ds18b20_read_bit() << i;
    __delay_us(120);      // wait 100 us
  }
 
  return value;
}
 
__bit ds18b20_read(uint8_t *raw_temp_value)
{
  if (!ds18b20_start())   // send start pulse
    return 0;             // return 0 if error
 
  ds18b20_write_byte(0xCC);   // send skip ROM command
  ds18b20_write_byte(0x44);   // send start conversion command
 
//  while(ds18b20_read_byte() == 0);  // wait for conversion complete
  __delay_us(1000);
 
  if (!ds18b20_start())  // send start pulse
    return 0;            // return 0 if error
 
  ds18b20_write_byte(0xCC);  // send skip ROM command
  ds18b20_write_byte(0xBE);  // send read command
  
  // read temperature LSB byte and store it on raw_temp_value LSB byte
  *(raw_temp_value)  = ds18b20_read_byte();
  // read temperature MSB byte and store it on raw_temp_value MSB byte
  *(raw_temp_value+1) = ds18b20_read_byte();
 
  return 1;   // OK --> return 1
}

#endif /* DEVICE_TYPE == DEVICE_TYPE_TEMP_SENSOR */