/*************************************************************************
|  Filename..............: ip_list.hpp
 * Project...............: part of standard utilities
 * Autor.................: Martin Dorfner (MD)
 * Date .................: 20.11.2020
 *-----------------------------------------------------------------------
 * Function:
 *   provide a list containing network login data
 *-----------------------------------------------------------------------
 * Dependecies:
 *   md_defines.h    (utility intern)
 *   linked_list.hpp (utility intern)
 *   ip_list.cpp     implementation
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.1.0  |20.11.20| import from former project                 | MD
*-----------------------------------------------------------------------*/

/* Header Digital Input */

#ifndef _IPLIST_HPP_
  #define _IPLIST_HPP_

  #include <linked_list.hpp>

  #define LOGINTXT_MAX_LEN 30
  #define MDIP_DEBUG CFG_DEBUG_NONE
  //#define MDIP_DEBUG CFG_DEBUG_STARTUP
  //#define MDIP_DEBUG CFG_DEBUG_ACTIONS
  //#define MDIP_DEBUG CFG_DEBUG_DETAILS

  class ip_cell : public md_cell
    {
      protected:
        uint32_t _locIP    = 0;
        uint32_t _gwIP     = 0;
        uint32_t _snIP     = 0;
        char     _ssid[LOGINTXT_MAX_LEN + 1] = "";
        char     _pw  [LOGINTXT_MAX_LEN + 1] = "";

      public:
        ip_cell() ;
        ~ip_cell() ;

        void init(uint32_t locIP, uint32_t gwIP, uint32_t snIP, const char* ssid, const char* pw);
        uint32_t locIP() { return(_locIP); }
        uint32_t gwIP()  { return(_gwIP);  }
        uint32_t snIP()  { return(_snIP);  }
        void getSSID(char* ssid, const uint8_t maxlen);
        void getPW(char* pw, const uint8_t maxlen);
    };

  //
  class ip_list : public md_list
    {
      private:
        ip_cell* _pFirst;
        ip_cell* _pLast;

      public:
        ip_list();
        ~ip_list();

        void append(uint32_t locIP, uint32_t gwIP, uint32_t snIP, const char* ssid, const char* pw);
        ip_cell* find(const char* ssid);
    };
#endif



