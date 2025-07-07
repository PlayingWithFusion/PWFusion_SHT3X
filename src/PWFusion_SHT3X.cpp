/**
 * @file PWFusion_SHT3X.cpp
 * 
 * @brief See header for more details
 * 
 * @copyright Copyright (c) 2025 Playing With Fusion
 * 
 */
#include "PWFusion_SHT3X.h"

PWFusion_SHT3X::PWFusion_SHT3X(TwoWire *pWire) { _pWire = pWire; }

// set address, start i2c, soft reset chip
bool PWFusion_SHT3X::begin(uint8_t address) {
  if (!_pWire) {
    return false;
  }

  _devAddr = address;
  _pWire->begin();
  reset();

  return true;
}

// reset command. Issue if something is wrong, resets all configs to default
// this serves the same function as the nRESET pin
void PWFusion_SHT3X::reset(void) {
  writeCmd(SHT3X_RESET); // issue reset command
  delay(2);              // max reset time is 1ms per datasheet
}

void PWFusion_SHT3X::enableHeater() { writeCmd(SHT3X_HEATER_EN); }

void PWFusion_SHT3X::disableHeater() { writeCmd(SHT3X_HEATER_DIS); }

// Update temperature and humidity readings from sensor
PWFusion_SHT3X_Data PWFusion_SHT3X::measure() {
  PWFusion_SHT3X_Data data = PWFusion_SHT3X_Data(0.0, 0.0, false);

  // send single shot update command to sensor, then wait for sensor to complete
  // the measurement
  writeCmd(SING_SHOT_HIGHREP);
  delay(15);

  // read 6 bytes of data after sending read header. wait 15ms after commanding
  // single shot measurement
  uint8_t data_arr[6];
  uint16_t Temp, Hum;
  _pWire->requestFrom(_devAddr,
                      (uint8_t)6); // read 6 bytes of data (must return 6 bytes)
  if (6 != _pWire->available())
  {
    data.error = true;
    return data;
  }

  for (uint8_t i = 0; i < 6; i++) {
    data_arr[i] = _pWire->read();
  } // read data bytes into array
  Temp = (data_arr[0] << 8) | data_arr[1];
  if (data_arr[2] != crc8(data_arr, 2))
  {
    data.error = true;
    return data;
  }

  Hum = (data_arr[3] << 8) | data_arr[4];
  if (data_arr[5] != crc8(data_arr + 3, 2))
  {
    data.error = true;
    return data;
  }

  data.rawTemp = Temp;
  data.rawHumidity = Hum;
  return data;
}

// write command - used by several functions to tell the sensor what to do
// (set meas mode, turn on heater, etc)
void PWFusion_SHT3X::writeCmd(uint16_t cmd) {
  _pWire->beginTransmission(_devAddr);
  _pWire->write(cmd >> 8);
  _pWire->write(cmd & 0xFF);
  _pWire->endTransmission();
}

// CRC algorithm. Poly 0x31, final XOR 0x00.
// test data 0xBEEF should give 0x92
uint8_t PWFusion_SHT3X::crc8(uint8_t *datptr, uint8_t len) {
  uint8_t POLY(0x31);
  uint8_t crc(0xFF);
  for (int j = len; j; j--) {
    crc ^= *datptr++;
    for (int i = 8; i; i--) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ POLY;
      } else {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}

PWFusion_SHT3X_Data::PWFusion_SHT3X_Data(uint16_t rawTemp, uint16_t rawHumidity,
                                         bool error)
    : rawTemp{rawTemp}, rawHumidity{rawHumidity}, error{error} {}

float PWFusion_SHT3X_Data::getTempC() {
  // See SHT3X-DIS datasheet, section 4.13
  return -45.0f + ((175.0f * rawTemp) / 65535.0f);
}

float PWFusion_SHT3X_Data::getTempF() {
  // See SHT3X-DIS datasheet, section 4.13
  return -49.0f + ((315.0f * rawTemp) / 65535.0f);
}

float PWFusion_SHT3X_Data::getRelativeHumidity() {
  return rawHumidity / 65535.0f;
}

bool PWFusion_SHT3X_Data::wasError() {
  return error;
}