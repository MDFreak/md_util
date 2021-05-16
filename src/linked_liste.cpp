/*************************************************************************
|  Filename..............: linked_list.cpp
 * Project...............: part of standard utilities
 * Autor.................: Martin Dorfner (MD)
 * Date .................: 20.11.2020
 *-----------------------------------------------------------------------
 * Function:
 *   Basic class for handling linked lists with small footprint
 *-----------------------------------------------------------------------
 * Dependecies:
 *   md_defines.h    (utility intern)
 *   linked_list.hpp
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.1.0  |20.11.20| import from former project                 | MD
*-----------------------------------------------------------------------*/

#include "linked_list.hpp"

// --- class md_cell           /* Abstrakte Basisklasse fuer Listenelemente */
  md_cell::md_cell()
             {
               init();
                      //SOUTLN(" md cell ");
             }

  md_cell::~md_cell()
             {}

  void*   md_cell::pNext()
                   { return (void*) _pNext; }

  void*   md_cell::pPriv()
                   { return (void*) _pPriv; }

  void    md_cell::pNext(void* pNext)
                   { _pNext = pNext; }

  void    md_cell::pPriv(void* pPriv)
                   { _pPriv = pPriv; }

  void    md_cell::init()
                   { _pNext = _pPriv  = NULL; }
  uint8_t md_cell::index()
                   { return _idx; }

// --- class md_list
  ret_t md_list::add(void* pCell)   /* ein Listenelement am Ende anhaengen */
    {
                #if (LL_DEBUG > CFG_DEBUG_NONE)
                  SOUT("   md_list before add: count ");
                  SOUT(_count);SOUT(" pFirst "); SOUTHEX((u_long) _pFirst); SOUT(" plast "); SOUTHEXLN((u_long) _pLast);
                #endif
      if (_pLast == NULL)            /* wenn noch kein Listenelement eingetragen */
          {
            _pFirst = _pLast = (md_cell*) pCell;
            _pLast->index(_count);
            _count++;
          }
        else
          {
            _pLast->pNext(pCell);
            md_cell* ptmp = (md_cell*) pCell;
            ptmp->pPriv((void*) _pLast);
            ptmp->pNext(NULL);
            _pLast = ptmp;
            _pLast->index(_count);
            _count++;
          }
                #if (LL_DEBUG > CFG_DEBUG_NONE)
                    SOUT("   md_list after add: count ");
                    SOUT(_count);SOUT(" pFirst "); SOUTHEX((u_long) _pFirst); SOUT(" plast "); SOUTHEXLN((u_long) _pLast);
                #endif
      return ISOK;
    }

  ret_t md_list::rem(OPOS_t first)
    {
      void*    ptmp  = NULL;
      md_cell* pcell = NULL;
                #if (IP_DEBUG > CFG_DEBUG_NONE)
                  SOUT(millis()); SOUT(" md_list remove ");
                #endif
      if ( _count > 0 )
        {
          if ( first == OFIRST )
            { // remove 1.cell
              ptmp  = _pFirst->pNext();
              if ( ptmp != 0 ) // more cells exist
                {
                  pcell = (md_cell*) ptmp;
                  pcell->pPriv(NULL);
                        #if (IP_DEBUG > CFG_DEBUG_NONE)
                          SOUT(" first "); SOUTHEXLN((u_long) _pFirst);
                        #endif
                  _pFirst = pcell;
                }
              else // last cell left
                {
                  _pFirst = _pLast = NULL;
                  _count = 0;
                }
              if (_count > 0) _count--;
            }
          else // first = LAST
            { // remove last cell
              ptmp  = _pLast->pPriv();
              if (ptmp != NULL)
                {
                  pcell = (md_cell*) ptmp;
                  pcell->pNext(NULL);
                        #if (IP_DEBUG > CFG_DEBUG_NONE)
                          SOUT(" last "); SOUTHEXLN((u_long) _pLast);
                        #endif
                  _pLast = pcell;
                  if (_count > 0) _count--;
                }
            }
        }
      else
        {
                #if (IP_DEBUG > CFG_DEBUG_NONE)
                  SOUTLN(" ERR list is empty ");
                #endif
        }

      return ISOK;
    }

  void* md_list::pIndex(uint8_t idx)
    {
      md_cell* ptmp = (md_cell*) _pFirst;
              //SOUT(" pIndex pFirst "); SOUTHEX((uint32_t) ptmp);
      while (ptmp != NULL)
        {
              //SOUT(" ptmp->idx "); SOUT(ptmp->index());
          if (ptmp->index() == idx)
            {
              break;
            }
          ptmp = (md_cell*) ptmp->pNext();
              //SOUT(" pNext "); SOUTHEX((uint32_t) ptmp);
        }
              //SOUTLN();
      return (void*) ptmp;
    }
/* EOF */

