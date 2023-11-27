#include <Arduino.h>
#include <wire.h>
#include <md_util.h>
#include <dict_list.hpp>

#ifndef DEBUG_MODE
    #define DEBUG_MODE CFG_DEBUG_STARTUP
  #endif // DEBUG_MODE
//--------------------------
// binary / hex tools
  // --- monitor dump HEX streams
  void serHEXdump(const uint8_t* pData, size_t length, uint8_t use_LF)
    {
      if (use_LF > FALSE)  Serial.println();
      for (int i=1; i<=length; i++)
        {
          Serial.printf("%02x ", pData[i-1]);
          if(i % 2 == 0) { Serial.print(" "); }
          if(i % 16 == 0){ Serial.println();  }
        }
      if (use_LF > FALSE)  Serial.println();
      else Serial.print("    ");
    }
  // --- handle binary strings
  char* getBinStr(char* pstr, uint8_t val, bool _debug)
    {
      if (_debug)
        {
          SOUT("getBinStr8 in '"); SOUT(pstr); SOUT("' val "); SOUTHEXLN(val);
        }
      sprintf(pstr, "%u %u %u %u %u %u %u %u",
                    (val & 0x80) >> 7, (val & 0x40) >> 6,
                    (val & 0x20) >> 5, (val & 0x10) >> 4,
                    (val & 0x08) >> 3, (val & 0x04) >> 2,
                    (val & 0x02) >> 1,  val & 0x01 );
      pstr[15] = 0;
      if (_debug)
        {
          SOUT("getBinStr8 out '"); SOUT(pstr); SOUTLN("'");
        }
      return pstr;
    }
  char* getBinStr(char* pstr, uint16_t val, bool _debug)
    {
      if (_debug)
        {
          SOUT("getBinStr16 in '"); SOUT(pstr); SOUT("' val "); SOUTHEXLN(val);
        }
      sprintf(pstr, "%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",
                    (val & 0x8000) >> 15, (val & 0x4000) >> 14,
                    (val & 0x2000) >> 13, (val & 0x1000) >> 12,
                    (val & 0x0800) >> 11, (val & 0x0400) >> 10,
                    (val & 0x0200) >>  9, (val & 0x0100) >>  8,
                    (val & 0x0080) >>  7, (val & 0x0040) >>  6,
                    (val & 0x0020) >>  5, (val & 0x0010) >>  4,
                    (val & 0x0008) >>  3, (val & 0x0004) >>  2,
                    (val & 0x0002) >>  1,  val & 0x0001 );
      pstr[31] = 0;
      if (_debug)
        {
          SOUT("getBinStr16 out '"); SOUT(pstr); SOUTLN("'");
        }
      return pstr;
    }
  // --- set / clear a single Bit in 16 bit word
  uint16_t clrBit(const uint16_t inWert, const uint16_t inBit, bool _debug)
    {
      uint16_t ret = inWert & (0xffff ^ inBit);
      if (_debug)
        {
          SOUT(" clrBit: inWert= "); SOUTHEX(inWert);
          SOUT("  inBit="); SOUT(inBit); SOUT(" ret= "); SOUTHEXLN(ret);
        }
      return ret;
    }
  uint16_t getBit(const uint16_t inWert, const uint16_t inBit, bool _debug)
    {
      uint16_t ret = inWert & inBit;
      if (_debug)
        {
          SOUT(" getBit: inWert= "); SOUTHEX(inWert);
          SOUT("  inBit="); SOUT(inBit); SOUT(" ret= "); SOUTHEXLN(ret);
        }
      return ret;
    }
  uint16_t setBit(const uint16_t inWert, const uint16_t inBit, const bool inVal, bool _debug)
    {
      if (inVal == 0)
        {
          return clrBit(inWert, inBit, _debug);
        }
      uint16_t ret = (inWert | inBit);
      if (_debug)
        {
          SOUT(" setBit: inWert= "); SOUTHEX(inWert);
          SOUT("  inBit="); SOUT(inBit); SOUT(" ret= "); SOUTHEXLN(ret);
        }
      return ret;
    }
  // --- swap bytes
  uint16_t swap_bytes(uint16_t number)
    {
      return (number << 8) | (number >> 8);
    }
  // --- calculate CRC16  (modbus mode)
  #ifdef USE_CRC16_MODBUS
      uint16_t md_crc16_update (uint16_t crc, uint8_t a)
        {
          int i;
          crc ^= a;
          for (i = 0; i < 8; ++i)
            {
                if (crc & 1)
              crc = (crc >> 1) ^ 0xA001;
                else
              crc = (crc >> 1);
            }
          return crc;
        }
      uint16_t md_modbus_crc(uint8_t buf[], int len)
        {
          unsigned int crc = 0xFFFF;
          for (unsigned int i = 0; i < len; i++)
           {
            crc = md_crc16_update(crc, buf[i]);
           }
           return crc;
        }
    #endif
  #ifdef UNUSED  // other CRC16
      uint16_t md_crc_xmodem_update  (uint16_t crc, uint8_t data)
        {
            int i;

            crc = crc ^ ((uint16_t)data << 8);
            for (i=0; i<8; i++)
            {
                if (crc & 0x8000)
                    crc = (crc << 1) ^ 0x1021;
                else
                    crc <<= 1;
            }

            return crc;
        }
      uint16_t _md_crc_ccitt_update  (uint16_t crc, uint8_t data)
        {
            data ^= lo8 (crc);
            data ^= data << 4;

            return ((((uint16_t)data << 8) | hi8 (crc)) ^ (uint8_t)(data >> 4)
                    ^ ((uint16_t)data << 3));
        }
      uint8_t  _md_crc_ibutton_update(uint8_t  crc, uint8_t data)
        {
          uint8_t i;

          crc = crc ^ data;
          for (i = 0; i < 8; i++)
            {
                if (crc & 0x01)
                    crc = (crc >> 1) ^ 0x8C;
                else
                    crc >>= 1;
            }
          return crc;
        }
    #endif // UNUSED
// --- scan I2C - serial output
      //uint8_t scanI2C(uint8_t no, uint8_t _start, uint8_t _stop, uint8_t sda, uint8_t scl)
  uint8_t scanI2C(TwoWire *i2c, uint8_t sda, uint8_t scl)
    {
      //dict_list devlist;

      uint8_t i = 0;
      uint8_t ec;
              SOUTLN("Scanning I2C ");
      if (i2c != NULL)
        {
          //TwoWire I2C = TwoWire(no-1);
              //SOUTLN(" I2C mounted");
          //I2C.setPins((int) sda, (int) scl);
              //SOUTLN(" I2C pins set");
          //printfI2C.begin(sda,scl,400000);
          i2c->begin((int) sda, (int) scl, (uint32_t) 80000); // 400000
              STXT(" I2C begin ok");
          #if (CFG_DEBUG_STARTUP > CFG_DEBUG_NONE)
              STXT(" Scanning I2C Addresses 1-127");
            #endif
          //uint8_t cnt=0;
          for(i = 1; i < 127 ; i++)
            {
              i2c->beginTransmission((int)i);
              ec=i2c->endTransmission(true);
              if(ec==0)
                {
                  #if (CFG_DEBUG_STARTUP > CFG_DEBUG_NONE)
                      // SHEXVAL(" device at address ", i);
                      switch (i)
                        {

                          case I2C_OLED_3C:         SHEXVAL(" OLED ", i);         break;
                          case I2C_OLED_3D:         SHEXVAL(" OLED ", i);         break;
                          case I2C_HDC1080_HUM_40:  SHEXVAL(" HDC1080 ", i);      break;
                          case I2C_INA3221_41:      SHEXVAL(" INA3221 ", i);      break;
                          case I2C_INA3221_42:      SHEXVAL(" INA3221 ", i);      break;
                          case I2C_INA3221_43:      SHEXVAL(" INA3221 ", i);      break;
                          case I2C_ADS1115_48:      SHEXVAL(" ADS1115 ", i);      break;
                          case I2C_ADS1115_49:      SHEXVAL(" ADS1115 ", i);      break;
                          case I2C_ADS1115_4A:      SHEXVAL(" ADS1115 ", i);      break;
                          case I2C_ADS1115_4B:      SHEXVAL(" ADS1115 ", i);      break;
                          case I2C_FRAM_50:         SHEXVAL(" FRAM ", i);         break;
                          case I2C_AT24C32_57:      SHEXVAL(" AT24C32 ", i);      break;
                          case I2C_CCS811_AQ_5A:    SHEXVAL(" CCS811_AQ ", i);    break;
                          case I2C_MLX90614_5B:     SHEXVAL(" MLX90614 ", i);     break;
                          case I2C_SI1145_LQ_60:    SHEXVAL(" SI1145_LQ ", i);    break;
                          case I2C_DS3231_RTC_68:   SHEXVAL(" DS3231_RTC ", i);   break;
                          case I2C_BME280_76:       SHEXVAL(" BME280 ", i);       break;
                          case I2C_BME680_77:       SHEXVAL(" BME680 ", i);       break;
                          case I2C_OLED_78:         SHEXVAL(" OLED_78 ", i);      break;
                          case I2C_OLED_7A:         SHEXVAL(" OLED_7A ", i);      break;
                          case I2C_FRAM_7C:         SHEXVAL(" FRAM old addr ", i);break;
                          default:                  SHEXVAL("unknown device ", i); break;
                        }
                    #endif
                }
            }
          SOUTLN("device scan loop finshed");
          i2c->end();
        }
      else
        {
          SOUTLN("I2C device ID=0 is not allowed");
        }
      return i;
    }
// --- class msTimer
  msTimer::msTimer()
    {
      startT(0);
    }
  msTimer::msTimer(const uint64_t inTOut)
    {
      startT(inTOut);
    }
  bool msTimer::TOut()
    {
      if ((millis() - _tstart) > _tout)
        return true;
      else
        return false;
    }
  void msTimer::startT()
    {
      _tstart = millis();
    }
  void msTimer::startT(const uint64_t inTOut)
    {
      _tstart = millis();
      _tout   = inTOut;
    }
  uint64_t msTimer::getTact()
    {
      return millis() - _tstart ;
    }
  uint64_t msTimer::getTout()
    {
      return millis() - _tout ;
    }
//
// class touchpin TODO test
  #if (USE_TOUCHPIN > OFF)
      void touchPin::init(bool isSwitch, uint8_t holdCnt, uint8_t limit)
        {
          _toggle = isSwitch;
          _mark   = TRUE;
          _hold   = (int8_t) holdCnt % 2;
          _limit  = limit;
          _value  = 0;
          _state  = OFF;
          _cnt    = 0;
             SOUT(" touchpin init pin "); SOUT(_pin); SOUT(" SW "); SOUT(_toggle);
        }
      void touchPin::reset()
        {
          _cnt = 0;
        }

      void touchPin::setHold(uint8_t holdCnt)
        {
          _hold = holdCnt;
        }
      void touchPin::setLimit(uint8_t limit)
        {
          _limit = limit;
        }
      uint8_t touchPin::state()
        {
          return _state;
        }
      uint8_t touchPin::value()
        {
          return _value;
        }
      uint8_t touchPin::pin()
        {
          return _pin;
        }
      void touchPin::read()
        {
                  SOUT(" touchpin "); SOUT(_pin); SOUT(" read value ");
          _value   = touchRead(_pin);
                  SOUT(_value);
          int8_t state = DOWN;
          if (_value > _limit) { state = UP; }
          _cnt += state;
                  SOUT(" state "); SOUT(state); SOUT(" _cnt "); SOUT(_cnt);
                  SOUT(" mark "); SOUT(_mark); SOUT(" toggle "); SOUTHEXLN(_toggle);

          if ( !_toggle && (_cnt > _hold) )
            {
              _state = OFF;
              _cnt   = _hold;
            }
          else if ( !_toggle && (_cnt < -_hold) )
            {
              _state = ON;
              _cnt   = -_hold;
            }
          else if ( _toggle && (_cnt < -_hold) )
            {
              if (_mark)
                {
                  _state  = !_state;
                  _toggle = -_toggle;
                  _mark   = FALSE;
                }
              _cnt    = -_hold;
            }
          else if ( _toggle && (_cnt > _hold) )
            {
              _mark = TRUE;
              _cnt  = _hold;
            }
                  //SOUT(" _state "); SOUT(_state); SOUT(" _cnt "); SOUT(_cnt);
                  //SOUT(" mark "); SOUT(_mark); SOUT(" toggle "); SOUTHEXLN(_toggle);
        }
    #endif // USE_TOUCHPIN
