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

#ifdef USE_MD_LISTS
    #include "md_list.h"

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
      void    md_cell::index(uint8_t idx)
                       { _idx = idx; }
      uint8_t md_cell::index()
                       { return _idx; }
      void*   md_cell::getobj(void)
                       { return _obj; }
      void    md_cell::setobj(void* obj)
                       { _obj = obj; }
    // --- class md_list
      uint16_t md_list::count  ()
                       { return _count; }
      uint8_t  md_list::mode   ()
                       { return _mode; }
      void     md_list::setmode(uint8_t newmode)
                       { if (newmode == OBJUSER) { _mode = OBJUSER; } else { _mode = OBJDEF; } }
      void*    md_list::pFirst ()
                       { return (void*) _pFirst; }
      void*    md_list::pLast  ()
                       { return (void*) _pLast; }
      void     md_list::pFirst (void*  newpFirst)
                       { _pFirst = (md_cell*) newpFirst; }
      void     md_list::pLast  (void*  newpLast)
                       { _pLast = (md_cell*) newpLast; }
      ret_t    md_list::add    (void* pCell)   /* ein Listenelement am Ende anhaengen */
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
      ret_t    md_list::rem    (OPOS_t first)
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
              doIdx();
            }
          else
            {
                    #if (IP_DEBUG > CFG_DEBUG_NONE)
                      SOUTLN(" ERR list is empty ");
                    #endif
            }
          return ISOK;
        }
      void*    md_list::pIndex (uint8_t idx)
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
      void     md_list::doIdx  ()
        { // renew idx to members
          uint8_t  myIdx = 0;
          md_cell* ptmp = (md_cell*) _pFirst;
          while (ptmp != NULL)
            {
              ptmp->index(myIdx);
              myIdx++;
              ptmp = (md_cell*) ptmp->pNext();
            }
        }
    // --- class md_cell1           /* Abstrakte Basisklasse fuer Listenelemente */
    // --- class md_list1
  #endif // USE_MD_LISTS
#ifdef TODO_TEMPLATE_TYPE
    // --- class smal_list
      // --- constructor / destructor
        template<typename T>
        smal_list<T>::smal_list()
                      {}

        template<typename T>
        smal_list<T>::smal_list(int(*compare)(T & a, T & b))
                      { setCompare(compare); }

        template<typename T>
        smal_list<T>::~smal_list()
                      { clear(); }

      // --- methodes
        template<typename T>
        void smal_list<T>::setCompare(int (* compare)(T& a, T& b))
          {
            this->compare = compare;
            sort();
          }
        template<typename T>
        smal_node<T>* smal_list<T>::getNode(int index)
          {
            if ((index < 0) || (index >= listSize)) return NULL;

            smal_node<T>* hNode = listBegin;
            int c                    = 0;

            if (isCached && (index >= lastIndexGot))
              {
                c     = lastIndexGot;
                hNode = lastNodeGot;
              }

            while (hNode && c < index)
              {
                hNode = hNode->next;
                c++;
              }

            if (hNode)
              {
                isCached     = true;
                lastIndexGot = c;
                lastNodeGot  = hNode;
              }

            return hNode;
          }

        template<typename T>
        int  smal_list<T>::size()
                          { return listSize; }
        template<typename T>
        bool smal_list<T>::isSorted()
                         { return sorted; }
        template<typename T>
        bool smal_list<T>::isEmpty()
                         { return listSize == 0; }
        template<typename T>
        void smal_list<T>::add(T obj)
          {
            // create new node
            smal_node<T>* newNode = new smal_node<T>();

            newNode->data = obj;

            if (!listBegin) listBegin = newNode;
            if (listEnd)
              {
                listEnd->next = newNode;
                listEnd       = newNode;
              }
              else
              {
                listEnd = newNode;
              }

            listSize++;
            sorted = false;
          }

        template<typename T>
        void smal_list<T>::add(int index, T obj)
          {
            if ((index < 0) || (index >= listSize)) { return; }

            smal_node<T>* newNode = new smal_node<T>();
            newNode->data = obj;

            if (index == 0)
              {
                listBegin = newNode;
              }
              else
              {
                smal_list<T>* nodePrev = getNode(index - 1);
                newNode->next  = nodePrev->next;
                nodePrev->next = newNode;
              }

            listSize++;
            sorted = false;
          }

        template<typename T>
        void smal_list<T>::insert(T obj)
          {
            if (!compare)
              {
                add(obj);
                return;
              }
            if (!sorted) sort();
        	    // create new node
            smal_node<T>* newNode = new smal_node<T>();
            newNode->data = obj;

            if (listSize == 0)
              {
                // add at start (first node)
                listBegin = newNode;

                listEnd = newNode;
              }
              else
              {
                if (compare(obj, listEnd->data) >= 0)
                  {
                    // add at end
                    listEnd->next = newNode;
                    listEnd       = newNode;
                  }
                else if (compare(obj, listBegin->data) < 0)
                  {
                    // add at start
                    newNode->next = listBegin;
                    listBegin     = newNode;
                  }
                  else
                  {
                    // insertion sort
                    smal_list<T>* h = listBegin;
                    smal_list<T>* p = NULL;
                    bool found           = false;

                    // here a sequential search, because otherwise the previous node couldn't be accessed
                    while (h && !found)
                      {
                        if (compare(obj, h->data) < 0)
                          {
                            found = true;
                          }
                          else
                          {
                            p = h;
                            h = h->next;
                          }
                      }
                    newNode->next = h;

                    if (p) p->next = newNode;
                  }
              }
            listSize++;
          }
        template<typename T>
        void smal_list<T>::replace(int index, T obj)
          {
            if ((index >= 0) && (index < listSize))
              {
                getNode(index)->data = obj;
              }
          }
        template<typename T>
        void smal_list<T>::swap(int x, int y)
          {
            // only continue when the index numbers are unequal and at least 0
            if ((x != y) && (x >= 0) && (y >= 0))
              {
                if (x > y)
                  { // the first index should be smaller than the second. If not, swap them!
                    int h = x;
                    x = y;
                    y = h;
                  }

                // When data is small, copy it
                if (sizeof(T) < 24)
                  {
                    smal_node<T>* nodeA = getNode(x);
                    smal_node<T>* nodeB = getNode(y);
                    T h                 = nodeA->data;
                    nodeA->data = nodeB->data;
                    nodeB->data = h;
                  }
                else // otherwise change the pointers
                  {
                    // Example: a -> b -> c -> ... -> g -> h -> i
                    //          we want to swap b with h
                    smal_node<T>* nodeA = getNode(x - 1);                      // x.prev
                    smal_node<T>* nodeB = getNode(x);                          // x
                    smal_node<T>* nodeC = getNode(x + 1);                      // x.next
                    smal_node<T>* nodeG = y - 1 == x ? nodeB : getNode(y - 1); // y.prev
                    smal_node<T>* nodeH = getNode(y);                          // y
                    smal_node<T>* nodeI = getNode(y + 1);                      // y.next

                    // a -> h -> i      b -> c -> ... -> g -> h -> i
                    if (nodeA) nodeA->next = nodeH;
                    else listBegin = nodeH;

                    // a -> h -> c -> ... -> g -> h -> i    b -> i
                    if (nodeH != nodeC) // when nodes between b and h exist
                        nodeH->next = nodeC;
                    else nodeH->next = nodeB;

                    // a -> h -> i      b -> i
                    nodeB->next = nodeI;

                    if (!nodeI) listEnd = nodeB;

                    // a -> h -> c -> ... -> g -> b -> i
                    if (nodeG != nodeB) // when more than 1 nodes between b and h exist
                        nodeG->next = nodeB;
                  }
              }
          }

        template<typename T>
        void smal_list<T>::remove(int index)
          {
            if ((index < 0) || (index >= listSize)) { return; }

            smal_node<T>* nodePrev     = getNode(index - 1);
            smal_node<T>* nodeToDelete = getNode(index);

            if (index == 0)
              {
                listBegin = nodeToDelete->next;
              }
              else
              {
                nodePrev->next = nodeToDelete->next;
                if (!nodePrev->next) listEnd = nodePrev;
              }

            delete nodeToDelete;
            isCached = false;

            listSize--;
          }

        template<typename T>
        void smal_list<T>::removeFirst()
                           { remove(0); }
        template<typename T>
        void smal_list<T>::removeLast()
                           { remove(listSize - 1); }
        template<typename T>
        bool smal_list<T>::has(T obj)
                           { return binSearch(obj) >= 0; }
        template<typename T>
        int  smal_list<T>::count(T obj)
          {
            if (compare == NULL) { return -1; }

            int c = 0;

            for (int i = 0; i < listSize; i++)
              {
                if (compare(obj, getNode(i)->data) == 0) c++;
              }
            return c;
          }
        template<typename T>
        T    smal_list<T>::get(int index)
          {
            smal_node<T>* h = getNode(index);

            if (h) return h->data;
            return T();
          }
        template<typename T>
        T    smal_list<T>::getFirst()
                        { return get(0); }
        template<typename T>
        T    smal_list<T>::getLast()
                        { return get(listSize - 1); }
        template<typename T>
        void smal_list<T>::moveToEnd()
          {
            smal_node<T>* h = listBegin;

            if (!h) return;

            listBegin     = listBegin->next;
            listEnd->next = h;

            h->next = NULL;
            listEnd = h;
            lastNodeGot  = NULL;
            lastIndexGot = -1;
            isCached     = false;
            sorted = false;
          }
        template<typename T>
        int  smal_list<T>::search(T obj)
          {
            if (compare == NULL) { return -1; }

            int i = 0;

            smal_node<T>* hNode = getNode(i);
            bool found               = compare(obj, hNode->data) == 0;

            while (!found && i < listSize)
              {
                i++;
                hNode = getNode(i);
                found = compare(obj, hNode->data) == 0;
              }

            return found ? i : -1;
          }
        template<typename T>
        int  smal_list<T>::searchNext(T obj)
          {
            if (compare == NULL) { return -1; }

            int i = lastIndexGot;

            smal_node<T>* hNode = lastNodeGot;
            bool found               = compare(obj, hNode->data) == 0;

            while (!found && i < listSize)
              {
                i++;
                hNode = getNode(i);
                found = compare(obj, hNode->data) == 0;
              }

            return found ? i : -1;
          }
        template<typename T>
        int  smal_list<T>::binSearch(T obj, int lowerEnd, int upperEnd)
          {
            if (!compare || !sorted) { return search(obj); }

            if (!listBegin) { return -1; }

            int res;
            int mid = (lowerEnd + upperEnd) / 2;

            smal_node<T>* hNode = getNode(0);
            int hIndex               = 0;

            while (lowerEnd <= upperEnd)
              {
                hNode  = lastNodeGot;
                hIndex = lastIndexGot;
                res    = compare(obj, getNode(mid)->data);

                if (res == 0)
                  {
                    return mid;
                  }
                  else if (res < 0)
                  {
                    // when going left, set cached node back to previous cached node
                    lastNodeGot  = hNode;
                    lastIndexGot = hIndex;
                    isCached     = true;

                    upperEnd = mid - 1;
                    mid      = (lowerEnd + upperEnd) / 2;
                  }
                  else if (res > 0)
                  {
                    lowerEnd = mid + 1;
                    mid      = (lowerEnd + upperEnd) / 2;
                  }
              }

            return -1;
          }
        template<typename T>
        int  smal_list<T>::binSearch(T obj)
                          { return binSearch(obj, 0, listSize - 1); }
        template<typename T>
        T    smal_list<T>::pop()
          {
            T data = getLast();

            removeLast();
            return data;
          }
        template<typename T>
        T    smal_list<T>::shift()
          {
            T data = getFirst();

            removeFirst();
            return data;
          }
        template<typename T>
        void smal_list<T>::clear()
          {
            while (listSize > 0) removeFirst();

            listSize  = 0;
            listBegin = NULL;
            listEnd   = NULL;
            lastNodeGot  = NULL;
            lastIndexGot = -1;
            isCached     = false;
            sorted = true;
          }
        template<typename T>
        void smal_list<T>::sort()
          {
            if (compare == NULL) return;

            // selection sort (less swaps than insertion sort)

            int indexH;                 // index of node i
            int indexMin;               // index of next minimum node

            smal_node<T>* nodeMin; // next minimum node
            smal_node<T>* nodeH;   // helper node at index j

            for (int i = 0; i < listSize - 1; i++)
              {
                nodeMin  = getNode(i);
                indexH   = i;
                indexMin = i;

                for (int j = i + 1; j < listSize; j++)
                  {
                    nodeH = getNode(j);

                    if (compare(nodeMin->data, nodeH->data) > 0)
                      {
                        nodeMin  = nodeH;
                        indexMin = j;
                      }
                  }
                swap(indexH, indexMin);
              }

            this->sorted = true;
          }
  #endif // TODO_TEMPLATE_TYPE
/* EOF */

