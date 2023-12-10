/*************************************************************************
|* filename:   md_ip_list.h
 * project:    part of standard utilities
 * author:     Martin DORFNER (MD)
 * maintainer: Martin DORFNER
 * email:      git@martin-dorfner.at
 * Date:       10.12.2023
 *-----------------------------------------------------------------------
 * Function:
 *   Base class for handling linked lists with small footprint
 *-----------------------------------------------------------------------
 * external dependancies:
 *   - no external dependancies
 *-----------------------------------------------------------------------
 * internal dependancies:
 *   - transfer C-type statement with make statement
 *     - '#define USE_MD_LIST=1'
 *     use VSCode/PlatformIO
 *        -> 'platformio.h'
 *           -> [env:<project_name>]
 *                build_flags = -D USE_MD_LIST=1
 *   - md_defines.h  (utility intern)
 *-----------------------------------------------------------------------
 * state of coding:
 *   - initial historic coding
 *   - was well working before this system change
 *   TODO  testing
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
 * Version| Date   | Changes                                    | Author
 *-----------------------------------------------------------------------
 * 0.001  |10.12.23| import from former project                 | MD
*-----------------------------------------------------------------------*/

#ifndef _MD_LIST_H_
  #define _MD_LIST_H_

  #ifdef USE_MD_LIST
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
          OMAX   = 0x7FFFu, // = int16 max
          OLAST  = OMAX,
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

        class md_list
          {
            protected:
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
    #endif // USE_MD_LIST
  #ifdef TODO_TEMPLATE_TYPE
          // --- base classes md_cell1, md_list1
            template<typename T>
            class md_cell1           /* Abstrakte Basisklasse fuer Listenelemente */
              {
                private:
                  T    _pNext  = NULL;   // Pointer auf naechstes Listenelement
                  T    _pPriv  = NULL;
                  uint8_t _idx = 0;
                  T    _obj    = NULL;

                public:
                  md_cell1(T p) { _obj = p; init(); }
                  ~md_cell1()   { }

                  T       pNext (void)
                                { return (void*) _pNext; }
                  T       pPriv (void)
                                { return (void*) _pPriv; }
                  void    pNext (T pNext)
                                { _pNext = pNext; }
                  void    pPriv (T pPriv)
                                { _pPriv = pPriv; }
                  uint8_t index (void)
                                { return _idx; }
                  void    index (uint8_t idx)
                                { _idx = idx; }
                  T       getobj(void)
                                { return _obj; }
                  void    setobj(T obj)
                                { _obj = obj; }

                private:
                  void    init()
                                { _pNext = _pPriv  = NULL; }
                                //{ _pNext = NULL; }
              };

            template<typename T>
            class md_list1
              {
                protected:
                  T        _pFirst = NULL;
                  T        _pLast  = NULL;
                  uint16_t _count  = 0;
                  uint8_t  _mode   = OBJDEF;

                public:
                  md_list1(T p)  { _pFirst = _pLast = NULL; _count = 0; } // Konstruktor
                  ~md_list1() {}

                  uint8_t  count  ()
                                { return _count; }
                  uint8_t  mode   ()
                                { return _mode; }
                  void     setmode(uint8_t newmode = OBJUSER)
                                { if (newmode == OBJUSER) { _mode = OBJUSER; } else { _mode = OBJDEF; } }
                  T        pFirst ()
                                { return _pFirst; }
                  T        pLast  ()
                                { return _pLast; }
                  void     pFirst (T newpFirst)
                                { _pFirst = newpFirst; }
                  void     pLast  (T newpLast)
                                { _pLast = newpLast; }
                  T        pIndex (uint8_t idx)
                                {
                                  T ptmp = _pFirst;
                                          //SOUT(" pIndex pFirst "); SOUTHEX((uint32_t) ptmp);
                                  while (ptmp != NULL)
                                    { //SOUT(" ptmp->idx "); SOUT(ptmp->index());
                                      if (ptmp->index() == idx)
                                        {
                                          break;
                                        }
                                      ptmp = ptmp->pNext();
                                          //SOUT(" pNext "); SOUTHEX((uint32_t) ptmp);
                                    }
                                          //SOUTLN();
                                  return ptmp;
                                }
                  ret_t    add    (T pCell)
                                {
                                            #if (LL_DEBUG > CFG_DEBUG_NONE)
                                                SOUT("   md_list1 before add: count ");
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
                                                SOUT("   md_list1 after add: count ");
                                                SOUT(_count);SOUT(" pFirst "); SOUTHEX((u_long) _pFirst); SOUT(" plast "); SOUTHEXLN((u_long) _pLast);
                                            #endif
                                  return ISOK;
                                }
                  ret_t    rem    (T inCell, uint8_t idx = 255)
                                {
                                  ret_t ret = ISERR;
                                  T ptmp  = NULL;
                                  T pcell = pFirst;
                                            #if (IP_DEBUG > CFG_DEBUG_NONE)
                                              SOUT(millis()); SOUT(" md_list1 remove ");
                                            #endif
                                  while (pcell != NULL)
                                    {
                                      if ( (inCell == pcell) || (idx == pcell->index()) )
                                        {
                                          if (pcell == pFirst)
                                              {
                                                pFirst = pcell->pNext;
                                              }
                                            else
                                              {
                                                ptmp = pcell->pPriv();
                                                ptmp->pNext(pcell->pNext);
                                              }
                                          if (pcell == pLast)
                                              {
                                                pLast = pcell->pPriv;
                                              }
                                            else
                                              {
                                                ptmp = pcell->pNext();
                                                ptmp->pPriv(pcell->pPriv);
                                              }
                                          if (_count > 0) _count--;
                                          ret = ISOK;
                                        }
                                    }
                                  doIdx();
                                  return ret;
                                }
                  ret_t    rem (OPOS_t first = OFIRST)
                                {
                                  if      (first == OFIRST) { return rem(_pFirst, 0); }
                                  else if (first == OLAST)  { return rem(_pLast,  0); }
                                }
                  ret_t    rem (uint8_t idx)
                                {
                                  return rem(NULL, idx);
                                }
                private:
                  void     doIdx ()
                                { // renew idx to members
                                  uint8_t myIdx = 0;
                                  T       ptmp  = _pFirst;
                                  while (ptmp != NULL)
                                    {
                                      ptmp->index(myIdx);
                                      myIdx++;
                                      ptmp = ptmp->pNext();
                                    }
                                }
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

                  virtual int  size();
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
    #endif // TODO_TEMPLATE_TYPE
#endif // _MD_LIST_H_
