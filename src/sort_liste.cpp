/*************************************************************************
|  Filename..............: linked_list.cpp
 * Project...............: part of standard utilities
 * Autor.................: Martin Dorfner (MD)
 * Date .................: 20.11.2020
 *-----------------------------------------------------------------------
 * Function:
 *   provide linked list support with small footprint
 *-----------------------------------------------------------------------
 * Dependecies:
 *   md_defines.h    (utility intern)
 *   linked_list.hpp
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.1.0  |15.04.21| import from former project                 | MD
*-----------------------------------------------------------------------*/

#include "sort_list.hpp"

// --- class sort_list
  uint16_t sort_list::add(void* pCell )
    {
      return ins_idx(pCell);
    }

  uint16_t sort_list::rem(bool first)
    {
      if (first) { return rem_idx(OFIRST); }
      else       { return rem_idx(OMAX); }
    }

  uint16_t sort_list::rem_idx(int16_t  index)
    {

    }

  uint16_t sort_list::ins_idx(void* pCell, int16_t  index)
    {

    }

  comp_t   sort_list::comp(void* pObj1, void* pObj2)
    {

    }

  void     sort_list::swap(void* pObj1, void* pObj2)
    {

    }

  void     sort_list::moveToEnd(void* pObj)
    {

    }

/* EOF */

