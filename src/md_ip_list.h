/*************************************************************************
|* filename:   md_ip_list.h
 * project:    part of standard utilities
 * author:     Martin DORFNER (MD)
 * maintainer: Martin DORFNER
 * email:      git@martin-dorfner.at
 * Date:       10.12.2023
 *-----------------------------------------------------------------------
 * function:
 *   provide linked list for ip WiFi connections
 *-----------------------------------------------------------------------
 * external dependancies:
 *   - no external dependancies
 *-----------------------------------------------------------------------
 * internal dependancies:
 *   - transfer C-type statement with make statement
 *     - '#define USE_MD_IP_LIST=1'
 *     use VSCode/PlatformIO
 *        -> 'platformio.h'
 *           -> [env:<project_name>]
 *                build_flags = -D USE_MD_IP_LIST=1
 *   - md_defines.h  (utility intern)
 *   - md_list.h     base class
 *-----------------------------------------------------------------------
 * state of coding:
 *   - initial historic coding
 *   - was well working before this system change
 *   TODO  testing
 *-----------------------------------------------------------------------
 * description
 *   md_ip_cell derived from md_cell
 *     - is a simple data object used with class md_list
 *     - contains all data for a WiFi connection
 *   md_ip_list derived from md_list
 *     - is a standard md_list with 2 additional methodes
 *       - an enhenced 'append' function
 *       - a search function for 'ssid'
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.001  |10.12.23| import from former project                 | MD
*-----------------------------------------------------------------------*/

#ifndef _MD_IP_LIST_H_
  #define _MD_IP_LIST_H_

  #ifdef USE_MD_IP_LIST
      #ifndef USE_MD_LIST
          #define USE_MD_LIST
        #endif // USE_MD_LIST
      #include <md_list.h>
      #ifndef LOGINTXT_MAX_LEN
          #define LOGINTXT_MAX_LEN 30
        #endif
      #define MDIP_DEBUG CFG_DEBUG_NONE
      //#define MDIP_DEBUG CFG_DEBUG_STARTUP
      //#define MDIP_DEBUG CFG_DEBUG_ACTIONS
      //#define MDIP_DEBUG CFG_DEBUG_DETAILS
      class md_ip_cell : public md_cell
        {
          protected:
            uint32_t _locIP    = 0;
            uint32_t _gwIP     = 0;
            uint32_t _snIP     = 0;
            char     _ssid[LOGINTXT_MAX_LEN + 1] = "";
            char     _pw  [LOGINTXT_MAX_LEN + 1] = "";

          public:
            md_ip_cell() ;
            ~md_ip_cell() ;

            void init(uint32_t locIP, uint32_t gwIP, uint32_t snIP, const char* ssid, const char* pw);
            uint32_t locIP() { return(_locIP); }
            uint32_t gwIP()  { return(_gwIP);  }
            uint32_t snIP()  { return(_snIP);  }
            void getSSID(char* ssid, const uint8_t maxlen);
            void getPW(char* pw, const uint8_t maxlen);
        };
      //
      class md_ip_list : public md_list
        {
          private:
            md_ip_cell* _pFirst;
            md_ip_cell* _pLast;

          public:
            md_ip_list();
            ~md_ip_list();

            void append(uint32_t locIP, uint32_t gwIP, uint32_t snIP, const char* ssid, const char* pw);
            md_ip_cell* find(const char* ssid);
        };
    #endif // USE_MD_IP_LIST
#endif // _MD_IP_LIST_H_



