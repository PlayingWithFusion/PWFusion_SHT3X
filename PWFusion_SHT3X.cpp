/***************************************************************************
* File Name: SEN-37002_SHT31D_HumTemp.ino
* Processor/Platform: Arduino Uno R3 (tested)
* Development Environment: Arduino 1.6.5
*
* Designed for use with with Playing With Fusion SHT31-DIS-B and SHT31-DIS-F
* Humidity and Temperature Sensor Breakouts: SEN-37002
*
*   SEN-37002 (universal applications)
*   ---> http://www.playingwithfusion.com/productview.php?pdid=81 B, no filter
*   ---> F, with filter
*   Note: This will aslo work with other SHT3X sensors (SHT35, for instance)
*
* Copyright © 2017 Playing With Fusion, Inc.
* SOFTWARE LICENSE AGREEMENT: This code is released under the MIT License.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
* **************************************************************************
* REVISION HISTORY:
* Author    Date    Comments
* J. Steinlage  2017Feb   Original version
* 
* Playing With Fusion, Inc. invests time and resources developing open-source
* code. Please support Playing With Fusion and continued open-source 
* development by buying products from Playing With Fusion!
*
* **************************************************************************/
#include "PWFusion_SHT3X.h"

PWFusion_SHT3X::PWFusion_SHT3X()
{
}

// set address, start i2c, soft reset chip
void PWFusion_SHT3X::begin(uint8_t address)
{
  _dev_add = address;
  Wire.begin();
  sht3x_reset();
}

// reset command. Issue if something is wrong, resets all configs to default
// this serves the same function as the nRESET pin
void PWFusion_SHT3X::sht3x_reset(void)
{
  write_cmd(SHT3X_RESET);             // issue reset command
  delay(2);                           // max reset time is 1ms per datasheet
}

void PWFusion_SHT3X::sht3x_heat_en(uint8_t en_flag)
{
  if(1 == en_flag){write_cmd(SHT3X_HEATER_EN);}
  else{write_cmd(SHT3X_HEATER_DIS);}
}

// Update temperature and humidity readings from sensor
boolean PWFusion_SHT3X::SHT3X_update(var_SHT3X *hum_sns)
{
  // send single shot update command to sensor, then wait for sensor to complete the measurement
  write_cmd(SING_SHOT_HIGHREP);
  delay(15);  
  
  // read 6 bytes of data after sending read header. wait 15ms after commanding single shot measurement
  uint8_t data_arr[6];
  uint16_t Temp, Hum;
  Wire.requestFrom(_dev_add, (uint8_t)6);  // read 6 bytes of data (must return 6 bytes)
  if(6 != Wire.available()) return false; 
  
  for(uint8_t i=0; i<6; i++)
  {
    data_arr[i] = Wire.read();
  }// read data bytes into array
  Temp = (data_arr[0] << 8) | data_arr[1];
  if(data_arr[2] != crc8(data_arr, 2)) return false;

  Hum = (data_arr[3] << 8) | data_arr[4];
  if(data_arr[5] != crc8(data_arr+3, 2)) return false;

  hum_sns->temp_raw = Temp;
  hum_sns->relhum_raw = Hum;
  return true;
           
}

// write command - used by several functions to tell the sensor what to do
// (set meas mode, turn on heater, etc)
void PWFusion_SHT3X::write_cmd(uint16_t cmd)
{
  Wire.beginTransmission(_dev_add);
  Wire.write(cmd>>8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission();
}

// CRC algorithm. Poly 0x31, final XOR 0x00. 
// test data 0xBEEF should give 0x92
uint8_t PWFusion_SHT3X::crc8(uint8_t *datptr, uint8_t len)
{
  uint8_t POLY(0x31);
  uint8_t crc(0xFF);
  for(int j = len; j; j--)
  {
    crc ^= *datptr++;
    for(int i = 8; i; i--)
    {
      if(crc & 0x80)
      {
        crc = (crc << 1) ^ POLY;
      }
      else
      {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}