/**
 * @file PWFusion_SHT3X.h
 * @brief Playing With Fusion SEN-37002 (SHT3X) Arduino library
 * 
 * @copyright Copyright © 2017 Playing With Fusion, Inc.
 * 
 * @mainpage Playing With Fusion SHT3X Library
 * 
 * Playing With Fusion, Inc. invests time and resources developing open-source
 * code. Please support Playing With Fusion and continued open-source 
 * development by buying products from Playing With Fusion!
 * 
 * @section compat_products Compatible Products
 * 
 * Designed for use with with Playing With Fusion SHT31-DIS-B and SHT31-DIS-F
 * Humidity and Temperature Sensor Breakouts: SEN-37002
 *
 *   SEN-37002 (universal applications)
 *   ---> http://www.playingwithfusion.com/productview.php?pdid=81 B, no filter
 *   ---> F, with filter
 *   Note: This will aslo work with other SHT3X sensors (SHT35, for instance)
 * 
 * Tested Platforms:
 * 
 * - Arduino Uno R3
 * - R3aktor M0 Logger 
 * 
 * @section min_ex Minimal Example
 * 
 * Here is a minimal example you can use to get running using a [R3aktor M0 logger](https://www.playingwithfusion.com/productview.php?pdid=157&catid=1018):
 * ```cpp
 * #include "PWFusion_SHT3X.h"
 *
 * // Static variables
 * static PWFusion_SHT3X sht3 = {};
 *
 * void setup() {
 *   // Setup Serial
 *   Serial.begin(115200);
 *   while (!Serial) {;}
 *   Serial.println("Initialized serial");
 *
 *   // Setup SHT3X sensor (Playing With Fusion)
 *   if (!sht3.begin(SHT3X_ADDR_PRIMARY))
 *   {
 *     Serial.println("Failed to initialize temp/humidity sensor");
 *     while (1) {;}
 *   }
 *
 *   Serial.println("Initialization success");
 * }
 *
 * void loop() {
 *   // Measure sensor
 *   PWFusion_SHT3X_Data d = sht3.measure();
 *
 *   if (!d.wasError())
 *   {
 *     Serial.print("Temp (F): ");
 *     Serial.print(d.getTempF());
 *     Serial.print(" Temp (C): ");
 *     Serial.print(d.getTempC());
 *     Serial.print(" Humidity (%): ");
 *     Serial.println(d.getRelativeHumidity() * 100);
 *   }
 *   else
 *   {
 *     Serial.println("Error trying to get data");
 *   }
 *
 *   delay(1000);
 * }
 * ```
 * 
 * @section rev_his Revision History
 * 
 * | Author       | Date    | Comments                   |
 * | ---          | ---     | ---                        |
 * | J. Steinlage | 2017Feb | Original Version           |
 * | J. Simeone   | 2025Jun | Upgrade Library - Add Docs |
 * 
 * @section license License
 * 
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
 * 
 * > Thanks to jothepro and his [awesome doxygen theme](https://github.com/jothepro/doxygen-awesome-css)
 */

#ifndef PWF_SHT3X_H
#define PWF_SHT3X_H

#include "Arduino.h"
#include "Wire.h"

// REGISTER DEFINITIONS
#define SING_SHOT_HIGHREP 0x2400      ///< single shot measurement, high repeatability, no clock stretching
#define SING_SHOT_MEDREP  0x240B      ///< single shot measurement, medium repeatability, no clock stretching
#define SING_SHOT_LOWHREP 0x2416      ///< single shot measurement, low repeatability, no clock stretching
#define SHT3X_RESET       0x30A2      ///< soft reset the SHT3X device
#define SHT3X_HEATER_EN   0x306D      ///< command to enable heater
#define SHT3X_HEATER_DIS  0x3066      ///< command to disable heater

#define SHT3X_ADDR_PRIMARY   (0x44)   ///< Address to use for device if ADDR pin is tied to GND (solder jumper soldered)
#define SHT3X_ADDR_SECONDARY (0x45)   ///< Address to use for device if ADDR pin is tied to VDD

/**
 * @brief Holds data from a transaction with the SHT3X sensor. Stores raw data,
 * user uses get* functions to retrieve formatted data.
 * 
 */
class PWFusion_SHT3X_Data
{
  public:
    /**
     * @brief Construct a new pwfusion sht3x data object
     * 
     * @param rawTemp See rawTemp
     * @param rawHumidity See rawHumidity
     * @param error See error
     */
    PWFusion_SHT3X_Data(uint16_t rawTemp, uint16_t rawHumidity, bool error);

    uint16_t rawTemp; ///< Raw 16-bit temperature value from SHT3X register. 0 if error
    uint16_t rawHumidity; ///< Raw 16-bit humidity value from SHT3X register. 0 if error
    bool error; ///< True if an error occurred during a transaction

    /**
     * @brief Convert raw register into formatted degrees C
     * 
     * @return float Temperature in degrees C (Centigrade)
     */
    float getTempC();

    /**
     * @brief Convert raw register into formatted degrees F
     * 
     * @return float Temperature in degrees F (Fahrenheit)
     */
    float getTempF();

    /**
     * @brief Convert raw humidity register into relative humidity
     * 
     * @return float Relative humidity, from [0.0, 1.0]
     */
    float getRelativeHumidity();

    /**
     * @brief If an error occurred during a transaction with the SHT3X device
     * 
     * @return true An error occurred. Measured data invalid
     * @return false No errors, measured data valid
     */
    bool wasError();
  private:
};

/**
 * @brief SHT3X device driver for Arduino.
 * 
 */
class PWFusion_SHT3X
{
public:	
  /**
   * @brief Construct a new pwfusion sht3x object
   * 
   * @param pWire Desired Wire interface. Typically left to default (`&Wire`)
   * in order to use hardware I2C
   */
  PWFusion_SHT3X(TwoWire *pWire = &Wire);

  /**
   * @brief Initialize device driver
   * 
   * @param address Address device is at. Either SHT3X_ADDR_PRIMARY or SHT3X_ADDR_SECONDARY
   * depending on connection of the ADDR pin, and status of solder jumper. See macros for more info
   * @return true Device started up OK
   * @return false Error trying to initialize device. Check wiring
   */
  bool begin(uint8_t address);
  /**
   * @brief Soft reset the SHT3X device
   * 
   */
  void reset(void);
  /**
   * @brief Enable internal heater. See datasheet for more information
   * 
   */
  void enableHeater();
  /**
   * @brief Disable internal heater. See datasheet for more information
   * 
   */
  void disableHeater();
  /**
   * @brief Perform a one-shot measurement on the device and return the results
   * 
   * @return PWFusion_SHT3X_Data Measurement data. Ensure to check "error" member
   * in case there was an error trying to get data.
   */
  PWFusion_SHT3X_Data measure();
  /**
   * @brief Write an I2C command to the device. See datasheet for more information
   * 
   * @param cmd 16-bit command to send.
   */
  void writeCmd(uint16_t cmd);
private:
  uint8_t crc8(uint8_t *datptr, uint8_t len);
  TwoWire *_pWire;
  uint8_t _devAddr;
};
#endif // PWF_SHT3X_H
