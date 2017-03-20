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

#ifndef PWF_SHT3X_H
#define PWF_SHT3X_H

#include "Arduino.h"
#include "Wire.h"
#include "PWFusion_SHT3X_STRUCT.h"

// REGISTER DEFINITIONS
#define SING_SHOT_HIGHREP 0x2400      // single shot measurement, high repeatability, no clock stretching
#define SING_SHOT_MEDREP  0x240B      // single shot measurement, medium repeatability, no clock stretching
#define SING_SHOT_LOWHREP 0x2416      // single shot measurement, low repeatability, no clock stretching
#define SHT3X_RESET       0x30A2      // soft reset the SHT3X device
#define SHT3X_HEATER_EN   0x306D      // command to enable heater
#define SHT3X_HEATER_DIS  0x3066      // command to disable heater


class PWFusion_SHT3X
{
public:	
  PWFusion_SHT3X();
  void begin(uint8_t address);
  void sht3x_heat_en(uint8_t en_flag);
  void write_cmd(uint16_t cmd);
  boolean SHT3X_update(var_SHT3X *hum_sns);
  
private:
  void sht3x_reset(void);
  uint8_t crc8(uint8_t *datptr, uint8_t len);
  uint8_t _dev_add;
};
#endif // PWF_SHT3X_H
