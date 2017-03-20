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
* **************************************************************************
* APPLICATION SPECIFIC NOTES (READ THIS!!!):
* - This code configures an Arduino to interface with an HDC1080 from Texas
*   Instruments.
*    - Configure Arduino (I2C, UART, etc)
*    - Perform initial verification and configuration of the SHT3X-DIS-X
*    - The SHT3X is an I2C-only device
*    - read status/info from sensor
*    - Write formatted information to serial port
* - I2C specific note: This example uses the I2C interface via the I2C lib, not
*   the 'Wire' lib included with the Arduino IDE.
* 
* Circuit:
*    Arduino Uno   Arduino Mega  -->  SEN-37002: SHT31-D Breakout
*    SDA:    SDA        SDA      -->  SDA
*    SCL:    SCL        SCL      -->  SCL
*    GND:    GND        ''       -->  GND
*    5V:     5V         ''       -->  VDD 
*    pulled high via 10k res     -->  ADD (low = 0x44, high = 0x45 address)
*                                -->  ALT (not used in example, altert output) 
*                                -->  RST (not used in example, pull low to reset IC)
*    (note: VDD should match voltage of IO, and can be between 3.3 and 5V)
**************************************************************************/
// The SHT3X sensors communicate via I2C. 
#include "Wire.h"
// include Playing With Fusion SHT31 libraries
#include <PWFusion_SHT3X.h>
#include <PWFusion_SHT3X_STRUCT.h>

// declare sensor object (see PWFusion_SHT3X.h file for definitions)
//#deinfe SHT3X_ADD 0x44 // set address to 0x44 by pulling ADD pin low
#define SHT3X_ADD 0x45 // standard I2C address on PWFusion SEN-37002 boards
PWFusion_SHT3X humsns0 = PWFusion_SHT3X();

// declare sensor structure
struct var_SHT3X hum_snsr_0;

void setup()
{
  var_SHT3X *humsns_ptr;
  humsns_ptr = &hum_snsr_0;
  
  Serial.begin(115200);
  Serial.println("Playing With Fusion: SHT31-DIS-X Humidity Sensor, SEN-37002");
  
  // setup humidity sensor
  humsns0.begin(SHT3X_ADD);
}

void loop()
{
  float temp;
  var_SHT3X *humsns_ptr;
  humsns_ptr = &hum_snsr_0;
  
  delay(1000);

  // update sensor
  //humsns0.SHT3X_update(humsns_ptr);
  
  if( humsns0.SHT3X_update(humsns_ptr))
  {
    Serial.println("SHT3X CH 0... Temp (C), RelHum (%)");
    Serial.print("               ");
    temp = (float)humsns_ptr->temp_raw;
    Serial.print(temp * 175.0 / 65536.0 - 45.0);
    Serial.print("      ");
    temp = (float)humsns_ptr->relhum_raw;
    Serial.println(temp * 100.0 / 65536.0);
    // code to turn on heater if humidity approaching condensing
    // might make sense to debounce this for a while
    // if(temp > 98.0){humsns0.sht3x_heat_en((uint8_t) 1);}
  }
  else{Serial.println("Error Reading Data");}
  
}


