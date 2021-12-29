/*************************************************************************
|  Filename..............: linked_list.hpp
 * Project...............: part of standard utilities
 * Autor.................: Martin Dorfner (MD)
 * Date .................: 20.11.2020
 *-----------------------------------------------------------------------
 * Function:
 *   Basic class for handling linked lists with small footprint
 *-----------------------------------------------------------------------
 * Dependecies:
 *   md_defines.h    (utility intern)
 * Implementation
 *   linked_list.cpp implementation
 *-----------------------------------------------------------------------
 * Description:
 *   - only void-pointers are connected with attributes
 *   - no extra memory allocation is done
 *   - payload is a simple void pointer
 *     typeof of payload may be set as dattype_t defined md_defines.h
 * Implementation:
 *   - simple list of objects, organised as fifo or as stack
 *   - fifo: first in = first out
 *     implementation: append after last object, remove first object
 *     push: add(void*),   pop: void*= rem() [def = OFIRST]
 *   - stack:  last in = first out
 *     implementation: append after last object, remove last object
 *     push: add(void*),   pop: void*= rem(OLAST) [def = OFIRST]
 * goal is small footprint with high performance
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.1.0  |20.11.20| import from former project                 | MD
 * 0.1.1  |16.04.21| import from former project                 | MD
*-----------------------------------------------------------------------*/

#ifndef _LINKED_LIST_HPP_
  #define _LINKED_LIST_HPP_

  #include <stdlib.h>
  //#include <Arduino.h>
  #include <md_defines.h>

  #define DICT_MAX_NAME_LEN 14
  //#define MD_CELL_TYPE_INT8  0
  //#define MD_CELL_TYPE_INT16 0

  #define LL_DEBUG CFG_DEBUG_NONE
  //#define LL_DEBUG CFG_DEBUG_STARTUP
  //#define LL_DEBUG CFG_DEBUG_ACTIONS
  //#define LL_DEBUG CFG_DEBUG_DETAILS

  enum OPOS_t
    {
      OFIRST = FALSE,      // function call
      OMAX   = 0x7FFF, // = int16 max
      OLAST  =  OMAX
    };

  #define MD_DEBUG          TRUE
  #define DICT_DEBUG        TRUE

  // --- base classes md_cell, md_list
    class md_cell           /* Abstrakte Basisklasse fuer Listenelemente */
      {
        private:
          void*   _pNext  = NULL;   // Pointer auf naechstes Listenelement
          void*   _pPriv  = NULL;
          uint8_t _idx    = 0;
          void*   _obj    = NULL;

        public:
          md_cell();
          ~md_cell();

          void*   pNext (void);
          void*   pPriv (void);
          void    pNext (void* pNext);
          void    pPriv (void* pPriv);
          uint8_t index (void);
          void    index (uint8_t idx);
          void*   getobj(void);
          void    setobj(void* obj);

        private:
          void    init();
      };

    //
    class md_list
      {
        private:
          md_cell* _pFirst = NULL;
          md_cell* _pLast  = NULL;
          uint16_t _count  = 0;
          uint8_t  _mode   = OBJDEF;

        public:
          md_list()  { _pFirst = _pLast = NULL; _count = 0; } // Konstruktor
          ~md_list() {}

          uint16_t count  ();
          uint8_t  mode   ();
          void     setmode(uint8_t newmode = OBJUSER);
          void*    pFirst ();
          void*    pLast  ();
          void     pFirst (void*  newpFirst);
          void     pLast  (void*  newpLast);
          void*    pIndex (uint8_t idx);
          ret_t    add    (void*  pCell);
          ret_t    rem    (OPOS_t first = OFIRST);

        private:
          void     doIdx ();
      };


  /* ===========================================
     reduced library from Stefan Kremser github.com/spacehuhn
    */

  #include <type_traits>
  #include <cstddef>
  #include <functional>

    template<class T>
    struct smal_node
      {
          T             data;
          smal_node<T>* next = NULL;
      };

    template<typename T>
    class smal_list
      {
        public:
          smal_list();
          smal_list(int(*compare)(T & a, T & b));
          virtual ~smal_list();

          virtual void setCompare(int (* compare)(T& a, T& b));

          virtual int size();
          virtual bool isSorted();
          virtual bool isEmpty();

          virtual void add(int index, T obj);
          virtual void add(T obj);
          virtual void insert(T obj);

          virtual void replace(int index, T obj);
          virtual void swap(int x, int y);

          virtual void remove(int index);
          virtual void removeFirst();
          virtual void removeLast();

          virtual bool has(T obj);
          virtual int count(T obj);

          virtual T shift();
          virtual T pop();
          virtual T get(int index);
          virtual T getFirst();
          virtual T getLast();

          virtual void moveToEnd();

          virtual int search(T obj);
          virtual int searchNext(T obj);
          virtual int binSearch(T obj);

          virtual void sort();
          virtual void clear();

        protected:
          int (* compare)(T& a, T& b) = NULL;

          int listSize            = 0;
          smal_node<T>* listBegin = NULL;
          smal_node<T>* listEnd   = NULL;

          // Helps get() method by saving last position
          smal_node<T>* lastNodeGot = NULL;
          int  lastIndexGot       = -1;
          bool isCached           = false;

          bool sorted = true;

          virtual smal_node<T>* getNode(int index);
          virtual int binSearch(T obj, int lowerEnd, int upperEnd);
      };




    #endif
