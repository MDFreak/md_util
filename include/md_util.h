#ifndef _MD_UTIL_H_
  #define _MD_UTIL_H_

  #include <Arduino.h>
  #include <Wire.h>
  #include <Time.h>
  //#include <md_crc16.h>
  #include <md_defines.h>
  //--------------------------
  // binary / hex tools
    // --- monitor dump HEX streams
      void     serHEXdump(const uint8_t* pData, size_t length);
    // --- handle binary strings
      char*    getBinStr(char* pstr, uint8_t val, bool _debug = FALSE);
      char*    getBinStr(char* pstr, uint16_t val, bool _debug = FALSE);
    // --- set / clear a single Bit in 16 bit word
      uint16_t setBit(uint16_t inWert, uint16_t inBit, bool inVal = true, bool _debug = FALSE);
      uint16_t clrBit(uint16_t inWert, uint16_t inBit, bool _debug = FALSE);
      uint16_t getBit(uint16_t inWert, uint16_t inBit, bool _debug = FALSE);
    // --- swap bytes
      uint16_t swap_bytes(uint16_t number);
    // --- calculate CRC16  (modbus mode)
      uint16_t md_crc16_update (uint16_t crc, uint8_t a);
      uint16_t md_modbus_crc(uint8_t buf[], int len);
  // --- scan I2C - serial output
    //uint8_t  scanI2C(uint8_t no, uint8_t start, uint8_t _stop, uint8_t sda, uint8_t scl);
    uint8_t  scanI2C(TwoWire *i2c, uint8_t sda, uint8_t scl);
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
  // class touchpin handling of touch pin function (ESP32, ESP8266)
  #if (BOARD ^ XIAO_ESP32C3)
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
      // class md_value
        // stores a value with max 32 bit values
        // types int8, int16, int32, char[4], uint8, uint16, uint32
        // char[4], char*, void*
    #endif
/*
    // C++ program to pass function as a
    // pointer to any function

    #include <iostream>
    using namespace std;

    // Function that add two numbers
    int add(int x, int y)
    {
        return x + y;
    }

    // Function that multiplies two
    // numbers
    int multiply(int x, int y)
    {
        return x * y;
    }

    // Function that takes a pointer
    // to a function
    int invoke(int x, int y,
               int (*func)(int, int))
    {
        return func(x, y);
    }

    // Driver Code
    int main()
    {
        // Pass pointers to add & multiply
        // function as required
        cout << "Addition of 20 and 10 is ";
        cout << invoke(20, 10, &add)
             << '\n';

        cout << "Multiplication of 20"
             << " and 10 is ";
        cout << invoke(20, 10, &multiply)
             << '\n';

        return 0;
    }


  class md_value32
    {
    private:
      uint8_t type = int32_t;
      union
        {
          uint8_t  bytes[4];  // bright, red, green, blue
          uint32_t dword;
        } _value32;

    public:
      // Constructors
        md_LEDPix24();
        md_LEDPix24(uint8_t bright, uint8_t red, uint8_t green, uint8_t blue);
        md_LEDPix24(uint32_t brgb24);
        md_LEDPix24(const uint8_t *pbrgb24);
        virtual ~md_LEDPix24() {}
      // methodes
        virtual bool   fromString(const char *LEDPixel);
        virtual bool   fromString(const String &LEDPixel);
        virtual String toString() const;
        virtual size_t printTo(Print& p) const;
        void     col24(uint32_t col);
        uint32_t col24();
        void     col16(uint32_t col);
        uint32_t col16();
        void     bright(uint8_t col);
        uint8_t  bright();

      //  Overloaded cast operator to allow  md_LEDPix24 objects
       // to be used where a pointer to a four-byte uint8_t array is expected
        operator uint32_t() const;
        bool operator==(const md_LEDPix24& LEDPixel) const;
        //bool operator==(const uint8_t* LEDPixel) const;
       // Overloaded index operator to allow getting and setting individual octets of the LEDPixeless
        uint8_t  operator[](int index) const;
        uint8_t& operator[](int index);
       // Overloaded copy operators to allow initialisation of IPAddress objects from other types
        //md_LEDPix24& operator=(const uint8_t *LEDPixel);
        //md_LEDPix24& operator=(uint32_t LEDPixel);
    };
  */
/* not working !!!
  class md_LEDPix24
  {
  private:
    union
      {
        uint8_t  bytes[4];  // bright, red, green, blue
        uint32_t dword;
      } _LEDPix24;

  public:
    // Constructors
      md_LEDPix24();
      md_LEDPix24(uint8_t bright, uint8_t red, uint8_t green, uint8_t blue);
      md_LEDPix24(uint32_t brgb24);
      md_LEDPix24(const uint8_t *pbrgb24);
      virtual ~md_LEDPix24() {}
    // methodes
      virtual bool   fromString(const char *LEDPixel);
      virtual bool   fromString(const String &LEDPixel);
      virtual String toString() const;
      virtual size_t printTo(Print& p) const;
      void     col24(uint32_t col);
      uint32_t col24();
      void     col16(uint32_t col);
      uint32_t col16();
      void     bright(uint8_t col);
      uint8_t  bright();

    //  Overloaded cast operator to allow  md_LEDPix24 objects
     // to be used where a pointer to a four-byte uint8_t array is expected
      operator uint32_t() const;
      bool operator==(const md_LEDPix24& LEDPixel) const;
      //bool operator==(const uint8_t* LEDPixel) const;
     // Overloaded index operator to allow getting and setting individual octets of the LEDPixeless
      uint8_t  operator[](int index) const;
      uint8_t& operator[](int index);
     // Overloaded copy operators to allow initialisation of IPAddress objects from other types
      //md_LEDPix24& operator=(const uint8_t *LEDPixel);
      //md_LEDPix24& operator=(uint32_t LEDPixel);
  };
 */
#endif