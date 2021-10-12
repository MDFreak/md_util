#ifndef _MD_UTIL_H_
  #define _MD_UTIL_H_

  #include <Arduino.h>
  #include <Time.h>
  //#include <TimeLib.h>
  #include <md_defines.h>
//#ifdef TMP
  uint16_t setBit(uint16_t inWert, uint16_t inBit, bool inVal = true, bool _debug = FALSE);
  uint16_t clrBit(uint16_t inWert, uint16_t inBit, bool _debug = FALSE);
  bool     getBit(uint16_t inWert, uint16_t inBit, bool _debug = FALSE);
  char*    getBinStr(char* pstr, uint8_t val, bool _debug = FALSE);
  char*    getBinStr(char* pstr, uint16_t val, bool _debug = FALSE);

  uint8_t  scanI2C(uint8_t no, uint8_t start, uint8_t _stop, uint8_t sda, uint8_t scl);
  class msTimer
    {
      private:
        uint64_t _tout;
        uint64_t _tstart;

      public:
        msTimer();
        msTimer(const uint64_t inTOut);

        bool      TOut();
        void      startT();
        void      startT(const uint64_t inTOut);
        uint64_t  getTact();
        uint64_t  getTout();
    };

  //
  // class touchpin handling of touch pin function (ESP32, ESP8266)
  class touchPin
    {
      private:
        #define UP    1
        #define DOWN -1

        int8_t  _pin    = NN;
        int8_t  _hold   = 1;
        int8_t  _cnt    = 0;
        int8_t  _value  = 0;
        int8_t  _state  = 0;    //  OFF < (0) > ON
        int8_t  _limit  = 35;    //  OFF < (0) > ON
        int8_t  _toggle = FALSE;
        int8_t  _mark   = FALSE;

      public:
        touchPin(uint8_t IOpin) { _pin = IOpin; }
        ~touchPin() {}

        void    init(bool isSwitch = FALSE, uint8_t holdCnt=5, uint8_t limit=35);
        void    reset();
        void    setHold(uint8_t holdCnt);
        void    setLimit(uint8_t limit);
        void    read();
        uint8_t state();
        uint8_t value();
        uint8_t pin();
    };
  //#endif // TMP
#endif