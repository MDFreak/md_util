/*************************************************************************
|  Filename..............: ip_list.cpp
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
 *   ip_list.hpp
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.1.0  |20.11.20| import from former project                 | MD
*-----------------------------------------------------------------------*/

#include <ip_list.hpp>
// --- class ip_cell
  ip_cell::ip_cell() : md_cell()
    {
                #if (MDIP_DEBUG > CFG_DEBUG_NONE)
                  SOUT(millis()); SOUT(" ip_cell new "); SOUTHEXLN((u_long) this);
                #endif
    }
  ip_cell::~ip_cell()
    {
                #if (MDIP_DEBUG > CFG_DEBUG_NONE)
                  SOUT(millis()); SOUT(" ip_cell del me "); SOUTHEXLN((u_long) this);
                #endif
    }
  void ip_cell::init(uint32_t locIP, uint32_t gwIP, uint32_t snIP, const char* ssid, const char* pw)
    {
      if (strlen(ssid) > LOGINTXT_MAX_LEN)
        {
          SOUT("ssid '"); SOUT(ssid); SOUT("' zu lang("); SOUT(LOGINTXT_MAX_LEN); SOUTLN(")");
          return;
        }
      if (strlen(pw) > LOGINTXT_MAX_LEN)
        {
          SOUT("pw '"); SOUT(pw); SOUT("' zu lang ("); SOUT(LOGINTXT_MAX_LEN); SOUTLN(")");
          return;
        }
      _locIP = locIP;
      _gwIP  = gwIP;
      _snIP  = snIP;
      strncpy(_ssid, ssid, LOGINTXT_MAX_LEN);
      strncpy(_pw, pw, LOGINTXT_MAX_LEN);
      _ssid[LOGINTXT_MAX_LEN]=0;
      _pw[LOGINTXT_MAX_LEN]=0;
                #if (MDIP_DEBUG > CFG_DEBUG_NONE)
                  SOUT(millis()); SOUT(" ip_cell init _locIP "); SOUTHEX(_locIP); SOUT("  _ssid "); SOUTLN(_ssid);
                #endif
    }
  void ip_cell::getSSID(char* ssid, uint8_t maxlen)
    {
      if (maxlen >= strlen(_ssid))
          { strcpy(ssid, _ssid); }
        else if (maxlen >= 3)
          {
            sprintf(ssid, "err");
            SOUT("len _ssid '"); SOUT(_ssid); SOUT("' to long ("); SOUT(maxlen); SOUTLN(")");
          }
    }
  void ip_cell::getPW(char* pw, uint8_t maxlen)
    {
      if (maxlen >= strlen(_pw))
          { strcpy(pw, _pw); }
        else if (maxlen >= 3)
          {
            sprintf(pw, "err");
            SOUT("len _pw '"); SOUT(_pw); SOUT("' to long ("); SOUT(maxlen); SOUTLN(")");
          }
    }

// --- class ip_list
  ip_list::ip_list() : md_list()
    {
                #if (MDIP_DEBUG > CFG_DEBUG_NONE)
                  SOUT(millis()); SOUT(" ip_list new");  SOUTHEXLN((u_long) this);
                #endif
    }
  ip_list::~ip_list()
    {
      ip_cell* pip = NULL;
      void*    ptmp = NULL;
      uint16_t cnt  = this->count();
      #if (MDIP_DEBUG > CFG_DEBUG_NONE)
          SOUT(millis()); SOUT(" ip_list delete me");  SOUTHEX((u_long) this);
      	  SOUT(" count "); SOUTLN(this->count());
        #endif
      while (cnt > 0)
        {
          ptmp = this->pFirst();
          pip = (ip_cell*) ptmp;
          rem(OFIRST);
          if (cnt > this->count())
            {
              #if (MDIP_DEBUG > CFG_DEBUG_NONE)
                  SOUT(" delete first "); SOUTHEXLN((u_long) pip);
                #endif
              delete( (ip_cell*) pip ) ;
              cnt--;
            }
        }
                    //SOUT(millis()); SOUT(" ip_list deleted ");  SOUTHEXLN((u_long) this);
    }
  void ip_list::append(uint32_t locIP, uint32_t gwIP, uint32_t snIP, const char* ssid, const char* pw)
    {
      ip_cell* neu = new ip_cell();
      this->add((void*) neu);
      neu->init(locIP, gwIP, snIP, ssid, pw);
      char stmp[LOGINTXT_MAX_LEN + 1];
      neu->getSSID(stmp, LOGINTXT_MAX_LEN);
      #if (MDIP_DEBUG > CFG_DEBUG_NONE)
            SOUT(millis());
            SOUT("  append ip_cell "); SOUTHEX((u_long) neu);
            SOUT("  locIP "); SOUTHEX(neu->locIP()); SOUT("  ssid "); SOUTLN(stmp);
        #endif
    }
  ip_cell* ip_list::find(const char* ssid)
    {
      void*    ptmp = this->pFirst();
      ip_cell* pcell = (ip_cell*) ptmp;
      char myname[LOGINTXT_MAX_LEN + 1] = "";
      #if (MDIP_DEBUG > CFG_DEBUG_NONE)
            SOUT(" ip_list::find search for ssid "); SOUT(ssid); //SOUT(" ptmp "); SOUTHEXLN((u_long) ptmp);
        #endif
      while ( ptmp != NULL )
        {
          pcell = (ip_cell*) ptmp;
          pcell->getSSID(myname, LOGINTXT_MAX_LEN);
                          //SOUT(" ptmp "); SOUTHEX((u_long) ptmp); SOUT(" cell ssid "); SOUT(myname);
          if (strcmp(myname, ssid) == 0)
            {
                          //SOUT(" ssid found "); SOUTLN(myname);
              break;
            }
          else
            {
                          //SOUTLN(" ssid wrong -> continue");
            }
          ptmp = pcell->pNext();
        }
                          //SOUT(" exit find pCell "); SOUTHEX((u_long) ptmp); SOUT(" cell ssid "); SOUTLN(myname);
      return (ip_cell*) ptmp;
    }
/* EOF */

