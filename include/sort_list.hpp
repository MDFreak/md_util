/*************************************************************************
|  Filename..............: sort_list.hpp
 * Project...............: part of standard utilities
 * Autor.................: Martin Dorfner (MD)
 * Date .................: 15.04.2021
 *-----------------------------------------------------------------------
 * Function:
 *   provide linked list support with small footprint
 *-----------------------------------------------------------------------
 * Dependecies:
 *   md_defines.h    (utility intern)
 *   linkde_list:h
 *   sort_list.cpp implementation
 *-----------------------------------------------------------------------
 * Description:
 *   Primitives for sorting linked lists
 *   - only void-pointers are connected with attributes
 *   - no extra memory allocation is done
 *   - some cell types are handled from derived classes like sorted list
 *     => type has to be defined and implemented
 *   - derivatives with specified types have to be introduced as well
 *   goal is small footprint with high performance
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.1.0  |20.11.20| import from former project                 | MD
*-----------------------------------------------------------------------*/

#ifndef _SORT_LIST_HPP_
  #define _SORT_LIST_HPP_

  #include <stdlib.h>
  #include <Arduino.h>
  #include <md_defines.h>
  #include <linked_list.hpp>

  enum comp_t
    {
      C_UNDEF   = 0,
      OBJ1_GT_2 = 1,
      OBJ1_EQ_2 = 2,
      OBJ1_LS_2 = 3
    };

  #define SORTMODE_NONE 0
  enum sortMode_t
    {
      NOSORT  = 0,
      IDX_UP  = 0,
      IDX_DN  = 1,
      VAL_UP,
      VAL_DN,
      SIZE_UP,
      SIZE_DN,
      ASC_UP,
      ASC_DN
    };

#define MD_DEBUG          TRUE
  #define DICT_DEBUG        TRUE

  //
    class sort_cell: public md_cell          /* Abstrakte Basisklasse fuer Listenelemente */
      {
        private:
          int16_t _idx       = NN;
          int16_t _sortidx   = NN;

        public:
          sort_cell() { }
          sort_cell(uint8_t celltype)  { }
          ~sort_cell() {}

          int16_t  getIdx()            { return _idx; }
          void     setIdx(int16_t idx) { _idx = idx; }
          int16_t  sortIdx()               { return _sortidx; }
          void     setSortIdx(int16_t idx) { _sortidx = idx; }
      };

  //
    /* class sort_list
     *
     * description additional to md_list
     * members are type sort_cell with additional properties:
     * -  idx     : index mirrors pointer enrowing, always increasing from last object
     *              pointer enrowing is tight, idx can hae gaps due to deleted objects
     * -  sortidx : used for sorting and iterating
     *              initially it runs like idx, but there are no gaps
     *              initially sorting is by creation time
     *              when changed by sorting, idx and pointers stay constant
     *
     */
    class sort_list: public md_list
      {
        private:
          sort_cell* _pFirst   = NULL;
          sort_cell* _pLast    = NULL;
          sortMode_t _sortmode = NOSORT;

        public:
          sort_list()  {} // Konstruktor
          sort_list(sortMode_t sortmode)  { _sortmode = sortmode; } // Konstruktor
          ~sort_list() {}

          // overlay base members
          uint16_t add    (void* pCell );
          uint16_t rem    (bool first = OFIRST);
          // members using index
          uint16_t rem_idx(int16_t  index = OFIRST );
          uint16_t ins_idx(void* pCell, int16_t  index = OMAX );

          // members or sorting
          comp_t   comp(void* pObj1, void* pObj2);
          virtual  comp_t compare(void* pObj1, void* pObj2);

        protected:
          void swap(void* pObj1, void* pObj2);
          void moveToEnd(void* pObj);
          void sortLast(int16_t idx);
      };
  /*
    class stack_list: public md_list
      {
        public:
          stack_list () {}
          ~stack_list() {}

          uint16_t push(void* pCell) { return add(pCell); }
          void*    pop ()            { return pLast(); remove(FALSE); }
      };
  */
#endif
