/*************************************************************************
|* filename:   md_util.h
 * project:    part of standard utilities
 * author:     Martin DORFNER (MD)
 * maintainer: Martin DORFNER
 * email:      git@martin-dorfner.at
 * Date:       10.12.2023
 *-----------------------------------------------------------------------
 * Function:
 *   provide smal tools without creating a complete library
 *-----------------------------------------------------------------------
 * external dependancies:
 *   TODO
 *-----------------------------------------------------------------------
 * internal dependancies:
 *   - transfer C-type statement with make statement
 *     TODO '#define USE_MD_LIST=1'
 *     use VSCode/PlatformIO
 *        -> 'platformio.h'
 *           -> [env:<project_name>]
 *                TODO build_flags = -D USE_MD_LIST=1
 *   - md_defines.h  (utility intern)
 *-----------------------------------------------------------------------
 * state of coding:
 *   - initial historic coding
 *   - was well working before this system change
 *   TODO  testing
 *-----------------------------------------------------------------------
 * Description:
 *   TODO
 * Implementation:
 *   TODO
 * goal is small footprint with high performance
 ************************************************************************
 * Version| Date   | Changes                                    | Author
 *-----------------------------------------------------------------------
 * 0.001  |10.12.23| import from former project                 | MD
*-----------------------------------------------------------------------*/

#ifndef _MD_UTIL_H_
  #define _MD_UTIL_H_

  #include <Arduino.h>
  #include <Wire.h>
  #ifdef USE_MD_DICT_LIST
      #include <md_dict_list.h>
    #endif // USE_MD_DICT_LIST
  #ifdef USE_MD_IP_LIST
      #include <md_ip_list.h>
    #endif // USE_MD_IP_LIST
  #ifdef USE_MD_LIST
      #include <md_list.h>
    #endif // USE_MD_LIST

  #include <Time.h>
  //#include <md_crc16.h>

  #include <md_defines.h>
  //--------------------------
  // binary / hex tools
    // --- monitor dump HEX streams
      void     serHEXdump(const uint8_t* pData, size_t length, uint8_t use_LF = TRUE);
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
  // --- md_timer
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
  // --- scan I2C - serial output
    //uint8_t  scanI2C(uint8_t no, uint8_t start, uint8_t _stop, uint8_t sda, uint8_t scl);
    #ifdef USE_I2C
        uint8_t  scanI2C(TwoWire *i2c, uint8_t sda, uint8_t scl);
      #endif // USE_I2C
  // class touchpin handling of touch pin function (ESP32, ESP8266)
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
#endif // _MD_UTIL_H_