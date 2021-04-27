#include <md_util.h>
#include <wire.h>
#include <md_filter.hpp>

// class md_val
md_val::md_val(double val, int8_t idx)
  {
            #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                SOUT("   md_val new "); SOUTHEX((ulong) this);
                SOUT(" idx ");  SOUT(idx); SOUT(" val "); SOUTLN((int32_t) val);
            #endif
    _val = val;
    _idx = idx;
  }

md_val::~md_val()
  {
            #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                SOUT(" md_val del "); SOUTHEXLN((ulong) this);
            #endif
  }

//
// class filterValue
  /*---------------------------------------------------------------------
    Berechnen eines Messwerts aus dem Analogkanal-Inhalt

      Die Funktion berechnet aus dem Rohwert den skalierten Messwert

      - Beim 1. Aufruf mit Mittelzahl > 1 wird eine Integration neu gestartet
      - Fuer jeden skalierten Analogwert wird ein Ringpuffer mitgefuehrt, aus
        dem ein gefilterter Wert mit der Funktion akt_filt_Messwert()
        berechnet werden kann


      Mittelzahl > 1 bedeutet:
        Bei jedem Aufruf wird der Messwert berechnet und aufaddiert,
        bis die Anzahl der Mittelungen erreicht ist.
        -> das zugehoerige Flag MessAna_Fertig wird gesetzt.

      Rueckgabe INT-Ergebnis
  \*---------------------------------------------------------------------*/

filterValue::filterValue(uint8_t filtAnz, uint8_t killPeek)
  {
    _pVals = new md_list();
    init(filtAnz, killPeek);
  }

filterValue::~filterValue()
  {
    clear();
    delete _pVals;
    _pVals = NULL;
  }

void filterValue::init(uint8_t filtAnz, uint8_t killPeek)
  {
    // check parameters killPeek <= 2
      if (filtAnz < 1)  { filtAnz  = 1; }
      if (killPeek < 0) { killPeek = 0; }
      if (killPeek >= 2)
        {
          killPeek = 2;
          if (filtAnz < 6) { killPeek--; }
        }
      if (filtAnz < 4)  { killPeek = 0; }

    clear();
    _maxCnt  = filtAnz;
    _minIdx  = killPeek;
    _maxIdx  = _maxCnt - killPeek - 1;
    if (_maxIdx < _minIdx) { _maxIdx =_minIdx; }
    _filtVal = 0.;
    _cnt = _filtPos = 0;
  }

void   filterValue::clear()
  {
    void* ptmp = NULL;
    if (_pVals != NULL)
      {
        ptmp = _pVals->pFirst();
                #if (VAL_DEBUG > CFG_DEBUG_STARTUP)
                  SOUT(millis()); SOUT(" md_Val clerr ");  SOUTHEX((u_long) this);
              	  SOUT(" cnt "); SOUTLN(_cnt);
                #endif
      }

    while ( (_cnt > 0) && (ptmp != NULL) )
      {
        _pVals->rem();
        delete (md_val*) ptmp;
                          #if (VAL_DEBUG > CFG_DEBUG_STARTUP)
                            SOUT(" delete "); SOUTHEX((u_long) ptmp);
                          #endif
        if (_cnt > 1)
          {
            ptmp = _pVals->pFirst();
            _cnt--;
          }
        else
          {
            ptmp = NULL;
            _cnt = 0;
          }
      }
    _filtPos = 0;
    _maxIdx  = 0;
    _filtVal  = 0;
                          #if (VAL_DEBUG > CFG_DEBUG_STARTUP)
                            SOUTLN(" delete finished ");
                          #endif
  }

double filterValue::value(double inval)
  {
    //
            #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                SOUT(" value ... anz "); SOUT(_cnt); SOUT(" / ");  SOUT(_maxCnt); SOUT(" val= "); SOUT(inval);
                SOUT(" sum "); SOUT((int32_t) _filtVal); SOUT(" _pos "); SOUTLN(_filtPos);
            #endif
    md_val* ptmp = NULL;
    int8_t  idx  = _cnt;
    // new index
    if (_cnt < _maxCnt)
      {
        ptmp = new md_val(inval, idx);
        ptmp->filtPos(_filtPos);
        _pVals->add((void*) ptmp);
        _cnt++;
      }
    else
      {
        ptmp = (md_val*) _pVals->pFirst();
        while ( (ptmp != NULL) && (_filtPos != ptmp->filtPos()) )
          {
            ptmp = (md_val*) ptmp->pNext();
          }
        if (ptmp != NULL)
          {
            ptmp->value(inval);
            move(ptmp, _cnt-1);
          }
      }

    sortLast();
    _filtPos++;
    if (_filtPos >= _maxCnt) { _filtPos = 0; }
    sum();

              #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                  SOUT("       add val idx "); SOUT(idx); SOUT(" cnt "); SOUT(_cnt); SOUT(" / "); SOUT(_maxCnt);
                  SOUT(" val= "); SOUT((int32_t) inval); SOUT(" sum "); SOUT(_filtVal); SOUT(" _pos "); SOUTLN(_filtPos);
              #endif
    return _filtVal;
  }

double filterValue::value()
  {
    return _filtVal;
  }

void   filterValue::sum()
  {
    // check sum area
    int8_t idx1 = _minIdx;
    int8_t idxL = _maxIdx;
    if (_cnt < _maxCnt)
      {
        idx1 = 0;
        idxL = _cnt - 1;
      }

    _filtVal = 0;
              #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                  SOUT(" sum idx1 - idxL "); SOUT(idx1); SOUT(" - "); SOUT(idxL);
              #endif
    md_val* ptmp = (md_val*) _pVals->pFirst();
    int8_t  tmp  = ptmp->index();
    while (tmp <= idxL)
      {
        if (tmp >= idx1)
          {
            _filtVal += ptmp->value();
              #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                    SOUT(" val "); SOUT(ptmp->value());
                    SOUT(" sum "); SOUT(_filtVal);
              #endif
          }
                  //SOUTLN(); SOUTLN(); SOUT("EX ptmp "); SOUTHEX((ulong) ptmp);
                  //SOUT(" pNext "); SOUTHEX((ulong) (ptmp->pNext()));
        ptmp = (md_val*) ptmp->pNext();
        tmp++;
                  //SOUT(" new ptmp "); SOUTHEXLN((ulong) ptmp); SOUTLN();
      }
    tmp = idxL - idx1 + 1;
    _filtVal = _filtVal / tmp;

              #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                  SOUT(" sum filtVal "); SOUTLN(_filtVal);
              #endif
  }

void   filterValue::sortLast()
  {
    md_val* pH   = (md_val*) _pVals->pLast();
    md_val* pL   = pH;
    double  valH = 0.;
    int8_t  idx  = -1;

    if (pH == NULL)
      { return; }
    valH = pH->value();
              #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                  SOUT("    sortLast new val "); SOUT( valH ); SOUT(" pLast "); SOUTHEXLN((ulong) pH);
              #endif

    while ( (pL != NULL) && (idx < 0) )
      {
        pL = (md_val*) pL->pPriv();
        if (pL != NULL)
          {
                      #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                          SOUT(" pPriv "); SOUTHEX((ulong) pL);
                          SOUT(" val "); SOUT( pL->value() );
                          SOUT(" idx "); SOUT( pL->index() );
                      #endif
            if (valH >= (pL->value()))
                {
                  idx = pL->index() + 1;
                      #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                          SOUT(" ok idx "); SOUT( pL->index() );
                      #endif
                }
              else
                {
                  //pL = (md_val*) pL->pPriv();
                      #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                          SOUT(" pPriv "); SOUTHEX((ulong) pL);
                      #endif
                }
          }
      }

    if ( idx < 0 )
      { idx = 0; }
                      #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                          SOUT(" destIdx "); SOUTLN(idx);
                      #endif
    move(pH, idx);
  }

void   filterValue::move(md_val* pCell, int8_t dest)
  {
    //
            /*
              md_val* myp = (md_val*) _pVals->pFirst();
              SOUTLN(" listdump before move ");
              for ( uint8_t i=0 ; i<_cnt ; i++ )
                {
                  SOUTHEX((ulong) myp); SOUT(" "); SOUT(myp->index()); SOUT(" ");
                  SOUTHEX((ulong) myp->pPriv()); SOUT(" "); SOUTHEX((ulong) myp->pNext()); SOUT(" ");
                  SOUT(myp->filtPos()); SOUT(" "); SOUTLN(myp->value());
                  myp = (md_val*) myp->pNext();
                }
            */

    int8_t idx = pCell->index();
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT("     move pCell "); SOUTHEX((ulong) pCell);
                      SOUT(" srcIdx "); SOUT(idx);
                      SOUT(" destIdx "); SOUTLN(dest);
                  #endif
    while (idx > dest)
      {
        dec(pCell);
        idx = pCell->index();
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" move dec idx "); SOUTLN(idx);
                  #endif
      }
    while (idx < dest)
      {
        inc(pCell);
        idx = pCell->index();
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" move inc idx "); SOUTLN(idx);
                  #endif
      }
            /*
              myp = (md_val*) _pVals->pFirst();
              SOUTLN(" listdump after move ");
              for ( uint8_t i=0 ; i<_cnt ; i++ )
                {
                  SOUTHEX((ulong) myp); SOUT(" "); SOUT(myp->index()); SOUT(" ");
                  SOUTHEX((ulong) myp->pPriv()); SOUT(" "); SOUTHEX((ulong) myp->pNext()); SOUT(" ");
                  SOUT(myp->filtPos()); SOUT(" "); SOUTLN(myp->value());
                  myp = (md_val*) myp->pNext();
                }
            */
  }

void  filterValue::dec(md_val* pIn)
    {
      md_val* pC1 = pIn;
      md_val* pC2 = (md_val*) pC1->pPriv();

      //upper cell
      md_val* ptmp = (md_val*) pC1->pNext();
      if (ptmp != NULL)
          {
            //
                    #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                        SOUT(" dec upper alt pCell "); SOUTHEX((ulong) ptmp);
                        SOUT(" pNext "); SOUTHEX((ulong) ptmp->pPriv());
                    #endif
            ptmp->pPriv((void*) pC2);
                    #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                        SOUT(" neu "); SOUTHEXLN((ulong) ptmp->pPriv());
                      #endif
          }
        else
          {
            _pVals->pLast((void*) pC2);
            pC2->pNext(NULL);
          }
      pC2->pNext(ptmp);
      //lower next cell
      ptmp = (md_val*) pC2->pPriv();
      if (ptmp != NULL)
        {
          //
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" dec lower next alt "); SOUTHEX((ulong) ptmp->pNext());
                    #endif
          ptmp->pNext((void*) pC1);
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" neu "); SOUTHEXLN((ulong) ptmp->pNext());
                    #endif
        }
      else
        {
          _pVals->pFirst((void*) pC1);
          pC1->pPriv(NULL);
        }
      pC1->pPriv(ptmp);
      //lower cell
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" dec lower alt pPriv "); SOUTHEX((ulong) pC2->pPriv());
                      SOUT(" idx "); SOUT(pC2->index());
                      SOUT(" pNext "); SOUTHEX((ulong) pC2->pNext());
                    #endif
      pC2->pPriv((void*) pC1);
      pC2->index(pC2->index()+1);
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" neu pPriv "); SOUTHEX((ulong) pC2->pPriv());
                      SOUT(" idx "); SOUT(pC2->index());
                      SOUT(" pNext "); SOUTHEXLN((ulong) pC2->pNext());
                  #endif
      //base cell
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" dec base alt pPriv "); SOUTHEX((ulong) pC1->pPriv());
                      SOUT(" idx "); SOUT(pC1->index());
                      SOUT(" pNext "); SOUTHEX((ulong) pC1->pNext());
                  #endif

      pC1->pNext((void*) pC2);
      pC1->index(pC1->index()-1);
                  #if (VAL_DEBUG > CFG_DEBUG_ACTIONS)
                      SOUT(" neu pPriv "); SOUTHEX((ulong) pC1->pPriv());
                      SOUT(" idx "); SOUT(pC1->index());
                      SOUT(" pNext "); SOUTHEXLN((ulong) pC1->pNext());
                  #endif
    }

void  filterValue::inc(md_val* pIn)
    {
      md_val* pC1 = pIn;
      md_val* pC2 = (md_val*) pC1->pNext();

      //lower cell
        md_val* ptmp = (md_val*) pC1->pPriv();
        if (ptmp != NULL)
            {
              ptmp->pNext((void*) pC2);
            }
          else
            {
              _pVals->pFirst((void*) pC2);
              pC2->pPriv(NULL);
            }
        pC2->pPriv(ptmp);
      //upper next cell
        ptmp = (md_val*) pC2->pNext();
        if (ptmp != NULL)
            {
              ptmp->pPriv((void*) pC1);
            }
          else
            {
              _pVals->pLast((void*) pC1);
              pC1->pNext(NULL);
            }
        pC1->pNext(ptmp);
      //upper cell
        pC2->pNext((void*) pC1);
        pC2->index(pC2->index()-1);
      //base cell
        pC1->pPriv((void*) pC2);
        pC1->index(pC1->index()+1);
    }