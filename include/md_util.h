#ifndef _MD_UTIL_H_
  #define _MD_UTIL_H_

  #include <Arduino.h>
  #include <md_defines.h>

  uint16_t setBit(uint16_t inWert, uint16_t inBit, bool inVal = true, bool _debug = FALSE);
  uint16_t clrBit(uint16_t inWert, uint16_t inBit, bool _debug = FALSE);
  bool     getBit(uint16_t inWert, uint16_t inBit, bool _debug = FALSE);
  char*    getBinStr(char* pstr, uint8_t val, bool _debug = FALSE);
  char*    getBinStr(char* pstr, uint16_t val, bool _debug = FALSE);

  uint8_t  scanI2C(uint8_t no, uint8_t start, uint8_t _stop, uint8_t sda, uint8_t scl);
  class msTimer
    {
      private:
        unsigned long _tout;
        unsigned long _tstart;

      public:
        msTimer();
        msTimer(const unsigned long inTOut);

        bool TOut();
        void startT();
        void startT(const unsigned long inTOut);
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
  //
  // class filterValue handles measurement values
  //                   inside floating filtering
  //
  class filterValue
    {
      protected:
        int8_t     _maxFilt   = 1;
        uint8_t    _filt      = 0;
        uint8_t    _pos       = 0;
        uint32_t*  _pVal      = NULL;  // *pVal[0] contains the filtered value
        //uint32_t   _min       = 0;
        //uint32_t   _max       = 0;

      public:
        filterValue(uint8_t filtAnz);
        ~filterValue();
        double    calcVal(double   _val);
        float     calcVal(float    _val) { return ((float)    calcVal((double) _val)); }
        uint8_t   calcVal(uint8_t  _val) { return ((uint8_t)  calcVal((double) _val)); }
        uint16_t  calcVal(uint16_t _val) { return ((uint16_t) calcVal((double) _val)); }
        uint32_t  calcVal(uint32_t _val) { return ((uint32_t) calcVal((double) _val)); }
        uint64_t  calcVal(uint64_t _val) { return ((uint64_t) calcVal((double) _val)); }
        void      clear();
    };
#endif