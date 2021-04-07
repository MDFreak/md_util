#include <md_util.h>
#include <wire.h>
#include <dict_list.hpp>

// TwoWire I2Cone = TwoWire(0);
// TwoWire I2Ctwo = TwoWire(1);
#define MD_UTIL_DEBUG  CFG_DEBUG_STARTUP

//--------------------------
// Setzen / Loeschen eines Bit in einer 16-Bit Flags-Wort
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

//--------------------------
// scan I2C - serial output
uint8_t scanI2C(uint8_t no, uint8_t _start, uint8_t _stop, uint8_t sda, uint8_t scl)
  {
    dict_list devlist;

    uint8_t i = 0;
    if (no > 0)
      {
        TwoWire I2C = TwoWire(no-1);
        I2C.begin(sda,scl,400000);
        #if (MD_UTIL_DEBUG > CFG_DEBUG_NONE)
            SOUTLN();
            SOUT("Scanning I2C Addresses Channel "); SOUTLN(no);
          #endif
        //uint8_t cnt=0;
        for(i = _start; i < _stop ; i++)
          {
            I2C.beginTransmission(i);
            uint8_t ec=I2C.endTransmission(true);
            if(ec==0)
              {
                #if (MD_UTIL_DEBUG > CFG_DEBUG_NONE)
                    SOUT(" device at address 0x");
                    if (i<16) Serial.print('0');
                    SOUTHEX(i); SOUT(" - ");
                    switch (i)
                      {
                        case I2C_OLED:     SOUTLN("OLED"); break;
                        case I2C_FRAM:     SOUTLN("FRAM"); break;
                        case I2C_BME280:   SOUTLN("BME280"); break;
                        case I2C_FRAM_ALT: SOUTLN("FRAM old addr"); break;
                        //case I2C_???: SOUTLN("???"); break;
                        //case I2C_???: SOUTLN("???"); break;
                        default:           SOUTLN("unknown device"); break;
                      }
                  #endif
              }
          }
        I2C.~TwoWire();
        Serial.println();
      }
    else
      {
        SOUTLN("I2C device ID=0 is not allowed");
      }
    return i;
  }

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

// class msTimer
  msTimer::msTimer()
    {
      startT(0);
    }

  msTimer::msTimer(const unsigned long inTOut)
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

  void msTimer::startT(const unsigned long inTOut)
    {
      _tstart = millis();
      _tout   = inTOut;
    }

//
// class touchpin
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