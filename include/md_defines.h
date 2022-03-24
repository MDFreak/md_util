#ifndef _MD_DEFINES_H_
  #define _MD_DEFINES_H_
  #include <Arduino.h>
  //#include <stdint.h>

  #ifndef CFG_DEFS
    #define CFG_DEFS
    // **********************************************
    // --- generic constants
      #define NN      -1  // undefined
      #define NU      -1  // not used
      #define NC      -1  // not connected
      #define UNDEF    0
      #define OFF                                                    0  // not active
      #define ON       1  // active

      #define TRUE     1
      #define FALSE    0

      #define MD_UNDEF -1 // default  used for user interface
      #define MD_DEF   0  // default  used for user interface
      #define MD_SEL   1  // selected
      #define MD_RDY   2  // ready
      #define MD_EME   3  // emergency

      #define OBJFREE  0 // modes for task handshake
      #define OBJBUSY  1
      #define OBJDEF   2
      #define OBJUSER  3

      #define SWITCH  TRUE

      #define CR       13 // carrige return
      #define LF       10 // line feed
      #define LN       LF
      #define U3V3     3
      #define U5V      5

      #define NOKEY    0
      #define NO_PIN   255

      #define UTC_TIMEZONE      3600           // +1 hour
      #define UTC_SUMMERTIME    3600           // +1 hour
      #define UTC_WINTERTIME    0

      enum ret_t
        {
          ISOK     = FALSE,       // 0
          ERR_OPEN,               // 2^0
          ERR_GND,                // 2^1
          ERR_VCC,                // 2^2
          ISERR    = 0x010000     // 2^16
        } ;

      // typedef data types
        /*
          #ifndef int8_t
            typedef char int8_t;
          #endif
          #ifndef uint8_t
            typedef unsigned char uint8_t;
          #endif
          #ifndef int16_t
            typedef short int16_t;
          #endif
          #ifndef uint16_t
            typedef unsigned short uint16_t;
          #endif
          #ifndef int32_t
            typedef int int32_t;
          #endif
          #ifndef uint32_t
            typedef unsigned int uint32_t;
          #endif
          #ifndef int64_t
            typedef long int64_t;
          #endif
          #ifndef uint64_t
            typedef unsigned long uint64_t;
          #endif
          */

        typedef enum dattype
          {
            DT_UNDEF   = 0,
            DT_8BIT,             // 8 bit types
            DT_INT8    = DT_8BIT,
            DT_UINT8,
            DT_CHAR,
            DT_16BIT,            // 16 bit types
            DT_INT16   = DT_16BIT,
            DT_UINT16,
            DT_32BIT,            // 32 bit types
            DT_INT32   = DT_32BIT,
            DT_UINT32,
            DT_PCHAR,
            DT_PVOID,
            DT_PSTRING,
            DT_64BIT,            // 64 bit types
            DT_INT64   = DT_64BIT,
            DT_DOUBLE,
            DT_END
          } dattype_t;

      #define DEBUG   TRUE
      #define NODEBUG FALSE

      #define CFG_DEBUG_NONE    0
      #define CFG_DEBUG_STARTUP 1
      #define CFG_DEBUG_ACTIONS 2
      #define CFG_DEBUG_DETAILS 3
    // **********************************************
    // --- system defines
      // serial baudrates
        #define AVR_SER_BAUD  9600ul
        #define ESP_SER_BAUD  115200ul
      // display defines
        #define GEO_128_64    0
        #define GEO_128_32    1
        #define GEO_64_48     2
        #define GEO_64_32     3
        #define GEO_RAWMODE   4
    // **********************************************
    // --- macros
      #define SET(b)      (b = true)
      #define RESET(b)    (b = false)

      #define SOUT(c)     (Serial.print(c))
      #define SOUTHEX(c)  (Serial.print(c, HEX))
      #define SOUTLN(c)   (Serial.println(c))
      #define SOUTHEXLN(c)(Serial.println(c, HEX))

      #define MDFILLARR(a,n) a[0]=n, memcpy( ((char*)a) + sizeof(a[0]), a, sizeof(a)-sizeof(a[0]) );

    // **********************************************
    // --- internal coding of HW defines
      //--------------------------------------------
      // --- MC_: 16 bit coded numbering of controller and periferal boards
        //bin xxxx xxxx xxxx xxxx
        //    ||xx xxxx xxxx xxxx - voltage used
        //    xx|| xxxx xxxx xxxx - HW type
        //    xxHW |||| xxxx xxxx - UCTY controller TYPE (type spezific)
        //    xxxx |||| xxxx xxxx - family/function spec (type spezific)
        //    xx|| || |||||| ++++ individual number (type spezific)
        //    || |||| |||||+ user output (optical, acustic)
        //    || |||| ||||+  user input  (grafical/text, acustic, keys)
        //    || |||| |||+   periferal input  (sensor)
        //    || |||| ||+    periferal output (relay)
        //    || |||| |+     system / interface
        //    || |||| +      reserved
        //    || |||| ---- controller family
        #define HW_DEF      0xFFFFu
      // --- PW voltages        ||xx xxxx xxxx xxxx
        #define MC_PW       0xc000u // mask for voltage
        #define MC_PW_5V    0x8000u
        #define MC_PW_3V3   0x4000u

      // --- HW types        xx|| xxxx xxxx xxxx
        #define MC_HW        0x3000u // mask for HW type
        #define MC_HW_UC     0x2000u // reserved
        #define MC_HW_MO     0x1000u // system internal or interface

      // --- UCTY controller TYPE    xxHW |||| xxxx xxxx
        #define MC_UCTY             MC_HW_UC + 0x0F00u // mask for Controller Type
        #define MC_UCTY_AV          MC_HW_UC + 0x0100u // arduino
        #define MC_UCTY_ESP8266     MC_HW_UC + 0x0200u // ESP8266
        #define MC_UCTY_ESP32       MC_HW_UC + 0x0300u // ESP32
        #define MC_UCTY_STM         MC_HW_UC + 0x0400u // STM32

      // --- MOTY module types       xxHW |||| xxxx xxxx
        #define MC_MOTY              MC_HW_MO + 0x0F00u // mask for family/function spec (type spezific)
        #define MC_MOTY_SYS          MC_HW_MO + 0x0100u // system internal or interface
        #define MC_MOTY_POUT         MC_HW_MO + 0x0200u // periferal output
        #define MC_MOTY_PIN          MC_HW_MO + 0x0300u // periferal input
        #define MC_MOTY_UOUT         MC_HW_MO + 0x0400u // user output
        #define MC_MOTY_UIN          MC_HW_MO + 0x0500u // user input
      // --- SPEC controller boards
        // --- arduino boards   xx1x 0001 xxxx ||||
          #define  _MC_AV_NANO_V3    MC_PW_5V + MC_UCTY_AV + 0x0001
          #define  _MC_AV_UNO_V3     MC_PW_5V + MC_UCTY_AV + 0x0002
          #define  _MC_AV_MEGA_V3    MC_PW_5V + MC_UCTY_AV + 0x0003
        //
        // --- ESP32 boards           xx1x 0001 xxxx ||||
          #define  MC_ESP32S_Node    MC_PW_3V3 + MC_UC_ESP32 + 0x0001 // single core
          #define  MC_ESP32_Node     MC_PW_3V3 + MC_UC_ESP32 + 0x0002
          #define  MC_ESP32_D1_R32   MC_PW_3V3 + MC_UC_ESP32 + 0x0003 // UNO compatible
          #define  MC_ESP32_LORA     MC_PW_3V3 + MC_UC_ESP32 + 0x0004
          #define  MC_ESP32_D1_MINI  MC_PW_3V3 + MC_UC_ESP32 + 0x0005
          #define  MC_ESP32_AZTOUCH  MC_PW_3V3 + MC_UC_ESP32 + 0x0006
      // --- user output parts (MC_MOTY_UOUT)
        // --- TFT displays
          #define  MC_UO_TFT1602_I2C_XA    MC_PW_3V3 + MC_MOTY_UOUT + 0x0001u
          #define  MC_UO_TFT1602_GPIO_RO   MC_PW_3V3 + MC_MOTY_UOUT + 0x0002u + MC_PW_5V  // used by KEYPADSHIELD
          #define  MC_UO_TXPT2046_AZ_SPI   MC_PW_3V3 + MC_MOTY_UOUT + 0x0003u   // used by Arduino-touch-case
          #define  MC_UO_TXPT2046_AZ_UNO   MC_PW_3V3 + MC_MOTY_UOUT + 0x0004u   // used by Arduino-touch-case
        // --- OLED displays
          #define  OLED_DRV_1106              1106
          #define  OLED_DRV_1306              1306

          #define  MC_UO_OLED_066_AZ        MC_PW_3V3 + MC_MOTY_UOUT + 0x0005u // IIC adress 0x3C,0x3D solder switch
            #define  OLED_066_GEO             GEO_64_48
            #define  OLED_066_DRV             OLED_DRV_1306
            #define  OLED_066_MAXCOLS         14  // ??
            #define  OLED_066_MAXROWS         4

          #define  MC_UO_OLED_091_AZ        MC_PW_3V3 + MC_MOTY_UOUT + 0x0006u// IIC adress 0x3C
            #define  OLED_091_GEO             GEO_128_32
            #define  OLED_091_DRV             OLED_DRV_1306
            #define  OLED_091_MAXCOLS         30
            #define  OLED_091_MAXROWS         4

          #define  MC_UO_OLED_096_AZ        MC_PW_3V3 + MC_MOTY_UOUT + 0x0007u
            #define  OLED_096_GEO             GEO_128_64
            #define  OLED_096_DRV             OLED_DRV_1306
            #define  OLED_096_MAXCOLS         30
            #define  OLED_096_MAXROWS         6

          #define  MC_UO_OLED_130_AZ        MC_PW_3V3 + MC_MOD_UOUT + 0x0008u
            #define  OLED_130_GEO             GEO_128_64
            #define  OLED_130_DRV             OLED_DRV_1106
            #define  OLED_130_MAXCOLS         30
            #define  OLED_130_MAXROWS         6
      // --- user input parts (MC_MOTY_UIN)
        // --- keypads
          #define  MC_UI_Keypad_ANA0_RO    MC_PW_3V3 + MC_MOD_UIN + 0x0001u + MC_PW_5V // used by KEYPADSHIELD

        // --- touchpads
          #define  MC_UI_TXPT2046_AZ_SPI   MC_PW_3V3 + MC_MOD_UIN + 0x0002u  // used by Arduino-touch-case
          #define  MC_UI_TXPT2046_AZ_UNO   MC_PW_3V3 + MC_MOD_UIN + 0x0003u  // used by Arduino Uno shield

      // --- periferal outputs (MC_MOTY_POUT)
          #define  AOUT_PAS_BUZZ_3V5V      U_3V5V + 0x0001u  // used by Arduino-touch-case

    // **********************************************
    // --- configuration and HW spezific defines
      // --- I2C devices
          #define  I2C1                     1           // I2C device 1
          #define  I2C2                     2           // I2C device 2
          //#define  I2C_TFT1602_IIC_XA_3V3   U_3V3  + 0
          #define  I2C_OLED_3C              0x3C
          #define  I2C_OLED_3D              0x3D
          #define  I2C_HDC1080_HUM_40       0x40
          #define  I2C_FRAM_50              0x50
          #define  I2C_AT24C32_57           0x57
          #define  I2C_CCS811_AQ_5A         0x5A
          #define  I2C_MLX90614_5B          0x5B
          #define  I2C_Si1145_LQ_60         0x60
          #define  I2C_DS3231_RTC_68        0x68
          #define  I2C_BME280               0x76
          #define  I2C_FRAM_7C              0x7C
          #define  I2C_DEV_NN               0xFF
      // --- music defines
        // --- structures
          typedef struct {
            int8_t   note;     // NOTE_C .. NOTE_B
            int8_t   octa;     // oktave 0 .. 7
            uint64_t beat;     // MB1 = base
          } tone_t;
        // --- music units
          // --- beat units --------------------
            #define MUSIC_BASEBEAT_US       250000ul  // base beat of 1/4 note = 250 ms
            #define MUSIC_RATIO_P100        80          // 80% ON / 20% OFF
            #define MB44  MUSIC_BASEBEAT_US * 4  // 4/4
            #define MB43  MUSIC_BASEBEAT_US * 3  // 3/4
            #define MB42  MUSIC_BASEBEAT_US * 2  // 2/4
            #define MB4   MUSIC_BASEBEAT_US * 1  // 1/4 note = basebeat
            #define MB8   MUSIC_BASEBEAT_US / 2  // 1/8
            #define MB16  MUSIC_BASEBEAT_US / 4  // 1/16
            #define MB32  MUSIC_BASEBEAT_US / 8  // 1/32
          // --- octave units ------------------
            #ifndef MUSIC_BASE_OCTA
              #define MUSIC_BASE_OCTA  5
            #endif // MUSIC_BASE_OCTA
            #define ON3   MUSIC_BASE_OCTA + 3  // octave negative 1
            #define ON2   MUSIC_BASE_OCTA + 2  // octave negative 1
            #define ON1   MUSIC_BASE_OCTA + 1  // octave negative 1
            #define OP0   MUSIC_BASE_OCTA      // (base-) octave positiv 0
            #define OP1   MUSIC_BASE_OCTA - 1  // octave positiv 1
            #define OP2   MUSIC_BASE_OCTA - 2  // octave positiv 1
            #define OP3   MUSIC_BASE_OCTA - 3  // octave positiv 1
            #define OP4   MUSIC_BASE_OCTA - 4  // octave positiv 1
          // --- notes -------------------------
            #define PAUSE   -1
        // --- songs
    	    #define SONG_HAENSCHEN_KLEIN  0
            const uint64_t SONG0_BEAT_US = MUSIC_BASEBEAT_US;
            const tone_t   SONG0_NOTES[] =
                { // Haenschen klein
                  {NOTE_G ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_E ,OP0,MB4 },{ PAUSE ,OP0, MB4 },
                  {NOTE_F ,OP0,MB4 },{NOTE_D ,OP0, MB4 },{NOTE_D ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_C ,OP0,MB4 },{NOTE_D ,OP0, MB4 },{NOTE_E ,OP0,MB4 },{NOTE_F ,OP0, MB4 },
                  {NOTE_G ,OP0,MB4 },{NOTE_G ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_G ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_E ,OP0,MB4 },{ PAUSE ,OP0, MB4 },
                  {NOTE_F ,OP0,MB4 },{NOTE_D ,OP0, MB4 },{NOTE_D ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_C ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{NOTE_G ,OP0, MB4 },
                  {NOTE_C ,OP0,MB42},                    { PAUSE ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_D ,OP0,MB4 },{NOTE_D ,OP0, MB4 },{NOTE_D ,OP0,MB4 },{NOTE_D ,OP0, MB4 },
                  {NOTE_D ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_F ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_E ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_E ,OP0,MB4 },{NOTE_E ,OP0, MB4 },
                  {NOTE_E ,OP0,MB4 },{NOTE_F ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_G ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_E ,OP0,MB4 },{ PAUSE ,OP0, MB4 },
                  {NOTE_F ,OP0,MB4 },{NOTE_D ,OP0, MB4 },{NOTE_D ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_C ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{NOTE_G ,OP0, MB4 },
                  {NOTE_C ,OP0,MB42},                    { PAUSE ,OP0,MB4 },{ PAUSE ,OP0, MB4 }
                };
              const uint16_t SONG0_LEN     = sizeof(SONG0_NOTES)/sizeof(tone_t);    //62;
          #define SONG_ALLE_VOEGLEIN    1
            const uint64_t SONG1_BEAT_US = MUSIC_BASEBEAT_US;
            const tone_t   SONG1_NOTES[] =
                { // Haenschen klein
                  {NOTE_C ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{NOTE_C ,OP1, MB4 },
                  {NOTE_B ,OP0,MB4 },{NOTE_C ,OP1, MB4 },{NOTE_B ,OP0,MB4 },{NOTE_G ,OP0, MB4 },

                  {NOTE_E ,OP0,MB4 },{NOTE_G ,OP0, MB4 },{NOTE_E ,OP0,MB4 },{NOTE_C ,OP0, MB4 },
                  {NOTE_D ,OP0,MB4 },{NOTE_C ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{ PAUSE ,OP0, MB4 },

                  {NOTE_C ,OP0,MB4 },{NOTE_E ,OP0, MB4 },{NOTE_G ,OP0,MB4 },{NOTE_G ,OP0, MB4 },
                  {NOTE_C ,OP0,MB42},                    { PAUSE ,OP0,MB4 },{ PAUSE ,OP0, MB4 }
                };
              const uint16_t SONG1_LEN     = sizeof(SONG1_NOTES)/sizeof(tone_t);    //62;
          //#define SONG_ALLE_ENTCHEN     2
    // **********************************************
  #endif // CFG_DEFS

#endif   // _MD_DEFINES_H_