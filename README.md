# md_util
 Basics for use of md_libraries 
 
 Every implementation need to include:
 
 * md_defines.h     - hold lot of basic defines, no code
 * md_projects.h    - defines project selection - coordinate with 'platformio:ini' 
 * md_util.h        - hold several helpful functions
    -> binary manipulation
      void       serHexdump(..)
      char*      getBinStr(..)
      uint16_t   setBit(..)
      uint16_t   clrBit(..)
      uint16_t   getBit(..)
      uint16_t   swap_bytes(..)

   -> calculate CRC16 (modbus mode)
      uint16_t   md_crc16_update(..)
      uint16_t   md_modbus_crc(..)

   -> Scanning I2C  - only if 'USE_I2C' is defined in 'platformio.ini'
      uint8_t  scanI2C(..)

   -> a very smal footprint timer object to be used run control
      class msTimer   - (state: perfect/in use) 
        void     startT(..)
        bool     TOut()
        uint64_t getTact()
        uint64_t getTout()
 
 Additional standard tools often used

 * smal footprint linked list object with 2 derivatives (state: ok/in use)
   -> md_list.h     - base object (state: ok/in use) 
   -> md_dict_list  - derived+ dictionary type object (state: unknown)
   -> md_ip_list    - derived+ wifi ip integration data (state: ok/in use)
 
 * filter object    - "least mean value" filtering (state: not complete testet)
   -> md_filter.h   - TODO
 

 
   
