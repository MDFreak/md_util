 /*
   * MIT License:
   * Copyright (c) 2008 Bjoern Hartmann
   * Permission is hereby granted, free of charge, to any person obtaining a copy
   * of this software and associated documentation files (the "Software"), to deal
   * in the Software without restriction, including without limitation the rights
   * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   * copies of the Software, and to permit persons to whom the Software is
   * furnished to do so, subject to the following conditions:
   *
   * The above copyright notice and this permission notice shall be included in
   * all copies or substantial portions of the Software.
   *
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   * THE SOFTWARE.
   *
   * @MDFreak
   * inbox@martin-dorfner.at
   */

#ifndef _MD_FILTER_H_
  #define _MD_FILTER_H_

  #include <Arduino.h>
  #include <md_defines.h>
  #include <linked_list.hpp>
  #include <list>

  #define VAL_DEBUG CFG_DEBUG_NONE
    //#define VAL_DEBUG CFG_DEBUG_STARTUP
    //#define VAL_DEBUG CFG_DEBUG_ACTIONS
    //#define VAL_DEBUG CFG_DEBUG_DETAILS

  /* class md_val handles measurement values
   * - filtering configurable (default FILT_NU)
   *   - FILT_NU:      filter not used
   *   - FILT_FL_MEAN: floating mean value, possible max/min drop
   *   - FILT_STD_DEV: standard deviation
   */  typedef enum filtertype
    {
      FILT_NU        = 0,  // not used
      FILT_FL_MEAN,        // floating mean value, possible max/min drop
      FILT_STD_DEV         // stanard deviation
    } filter_t;

  template<typename T>
  class md_val
    {
      private:
        T        _val;
        T*       _pvals    = NULL;
        uint8_t* _psort    = NULL;
        uint8_t  _vIdx   = 0;
        uint8_t  _sIdx  = 0;
        uint8_t  _cnt      = 0;
        uint8_t  _numFilts = 0;
        uint8_t  _numDrops = 0;
        filter_t _tfilt    = FILT_NU;

      public:
        //md_val(T val);
        //md_val(T valuint8_t numFilters);
        /*
            md_val() {};

            ~md_val();

            void begin(uint8_t numFilts = 0, uint8_t numDrops = 0, filter_t filtType = FILT_NU);
            T    getVal() { return _val; }
            T    doVal (T newVal);
            void    clear ();
            uint8_t count () { return _cnt; }

          private:
            void    _sort ();
            void    _swapSort (uint8_t idx1, uint8_t idx2);
            void    _calc ();
        */
        md_val() {}

        ~md_val()
            {
              if (_pvals != NULL) { delete _pvals;  }
              if (_psort != NULL) { delete _psort; }
            }

        void begin(uint8_t numFilts = 0, uint8_t numDrops = 0, filter_t filtType = FILT_NU)
            {
              if (numFilts > 1)
                {
                      SOUT(" new _pvals ");
                  _pvals = new T[numFilts];
                  _psort = new uint8_t[_numFilts];
                  for (uint8_t i = 0 ; i < _numFilts ; i++ )
                    {
                      _pvals[i]  = 0;
                      _psort[i] = 0;
                    }
                }

              _numFilts = numFilts;
              if (numFilts > 2 * numDrops)
                {
                  _numDrops = numDrops;
                      SOUT(" drops "); SOUT(_numDrops); SOUT(" ");
                } // else Drops are too many => stay 0
              switch (filtType)
                {
                  case FILT_FL_MEAN: // floating mean value, possible max/min drop
                    _tfilt = filtType;
                    break;
                  case FILT_STD_DEV: // standard deviation TODO -> not implemented
                    // break;
                  default:
                    _tfilt = FILT_NU;
                    break;
                }
              _tfilt    = filtType;
              _cnt      = 0;

            };

        T    getVal()
            {
              return _val;
            }

        T    doVal (T newVal)
            {
              // no filter with return
                //SOUT(" doVal "); SOUT(newVal); SOUT(" ... ");
              if (_numFilts == 0)
                {
                  _val = newVal;
                  return newVal;
                    //SOUTLN(" single ");
                }

              // sorted filter - 0 ~ smallest
              else
                {
                  // startup
                    //SOUT(" multi "); SOUT(_numFilts);
                  if (_cnt == 0)
                    {
                        _vIdx  = 0;
                        _sIdx = 0;
                          //SOUT(" startup ");
                    }
                  // running
                  else
                    {
                      // find next data and sort pointers
                      _vIdx++; if (_vIdx >= _numFilts) { _vIdx = 0; }
                      _sIdx = 0;
                      while ((_psort[_sIdx] != _vIdx ) && (_sIdx < _cnt))
                        { _sIdx++; }
                          //SOUT(" _vIdx "); SOUT(_vIdx); SOUT(" _sIdx "); SOUT(_sIdx);
                    }

                  // write new value
                  _pvals[_vIdx] = newVal;
                      //SOUT(" newVal "); SOUTLN(_pvals[_psort[_sIdx]]);

                  // ajust pointers for initial upload of values
                  if ( _cnt < _numFilts ) // initial upload of values
                    {
                      _psort[_vIdx] = _sIdx = _vIdx;
                      _cnt++;
                          //SOUT(" incomplete ");
                          //SOUT(" _vIdx "); SOUT(_vIdx); SOUT(" _sIdx "); SOUT(_sIdx);
                          //SOUT(" _cnt "); SOUTLN(_cnt);
                    }

                      //SOUTLN(" vor Sort ");
                      //for ( uint8_t i = 0; i < _cnt ; i++ )
                      //  { SOUT(_psort[i]); SOUT(" - "); SOUTLN(_pvals[_psort[i]]); }
                  _sort();
                      //SOUTLN(" nach Sort ");
                      //for ( uint8_t i = 0; i < _cnt ; i++ )
                      //  { SOUT(_psort[i]); SOUT(" - "); SOUTLN(_pvals[_psort[i]]); }

                  _calc();
                }
              return _val;
            }

        void    clear ()
            {
              if (_pvals != NULL)
                {
                  for ( uint8_t i = 0 ; i <_numFilts ; i++ )
                    {
                      _pvals[i] = 0;
                      _psort[i]= 0;
                    }
                  _vIdx = 0;
                  _cnt     = 0;
                }
            }

        uint8_t count ()
            {
              return _cnt;
            }

      private:
        void    _sort ()
            {
              uint8_t state = 0; // 1 = dir up ready, 2 = dir down ready, 3 = ready
              uint8_t idx   = _sIdx;
              for ( uint8_t i = idx + 1 ; i < _cnt ; i++ )
                {
                  if (_pvals[_psort[idx]] > _pvals[_psort[i]])
                    {
                          //SOUT(" sortup or swap idx > i "); SOUT(idx); SOUT("("); SOUT(_pvals[_psort[idx]]);
                          //SOUT(")>"); SOUT(i); SOUT("("); SOUT(_pvals[_psort[i]]); SOUT(")");
                      _swapSort(idx, i);
                          //SOUT(" sortup nach swap idx > i "); SOUT(idx); SOUT("("); SOUT(_pvals[_psort[idx]]);
                          //SOUT(")>"); SOUT(i); SOUT("("); SOUT(_pvals[_psort[i]]); SOUT(")");
                      state |= 2;
                      idx++;
                    }
                  else
                    {
                      break;
                    }
                }
              state |= 1;

              if ( state < 3 )
                {
                  for ( int8_t i = idx - 1 ; i >= 0 ; i-- )
                    {
                      if (_pvals[_psort[idx]] < _pvals[_psort[i]])
                        {
                              //SOUT(" sortup or swap idx < i "); SOUT(idx); SOUT("("); SOUT(_pvals[_psort[idx]]);
                              //SOUT(")<"); SOUT(i); SOUT("("); SOUT(_pvals[_psort[i]]); SOUT(")");
                          _swapSort(idx, i);
                              //SOUT(" sortup nach swap idx < i "); SOUT(idx); SOUT("("); SOUT(_pvals[_psort[idx]]);
                              //SOUT(")<"); SOUT(i); SOUT("("); SOUT(_pvals[_psort[i]]); SOUT(")");
                          //state |= 1;
                          idx--;
                        }
                      else
                        {
                          break;
                        }
                    }
                }
            }

        void    _swapSort (uint8_t idx1, uint8_t idx2)
            {
              uint8_t tmp  = _psort[idx1];
              _psort[idx1] = _psort[idx2];
              _psort[idx2] = tmp;
            }

        void    _calc ()
            {
              if (_cnt < 1) { return; }
              if (_tfilt == FILT_FL_MEAN)
                {
                  T       sum    = 0;
                  uint8_t begSum = 0;
                  uint8_t endSum = _cnt - 1;
                  int8_t  cnt    = _cnt - (_numDrops << 1);
                  if (cnt <= 0) { cnt = _cnt; }
                  else
                    {
                      begSum = _numDrops;
                      endSum = _cnt - _numDrops -1;
                    }
                  for (uint8_t i = 0 ; i < cnt ; i++)
                    {
                      sum += _pvals[_psort[i]];
                    }
                  _val = sum / cnt;
                }
            }
    };

  class md0_val : public md_cell
    {
      private:
        double  _val  = 0.;
        //<T>*    _pval = NULL;
        int8_t  _idx  = NN;
        int8_t  _filt = 0;

      public:
        md0_val() {}
        md0_val(double val, int8_t idx) { _val = val; _idx  = idx; }
        ~md0_val() { /*if (_pval != NULL) delete(_pval);*/ }

        int8_t  index()   { return _idx; }
        int8_t  filtPos() { return _filt;}
        double  value()   { return _val; }
        void    index(int8_t idx)       { _idx = idx; }
        void    filtPos(int8_t filtPos) { _filt = filtPos; }
        void    value(double val)       { _val = val; }
    };

  //
  class filterValue
    {
      protected:
        int8_t   _maxCnt  = 0;
        int8_t   _minIdx  = 0;
        int8_t   _maxIdx  = 0;
        int8_t   _cnt     = 0;
        int8_t   _filtPos = 0;
        double   _filtVal = 0.;
        double   _offset  = 0.;
        double   _gain    = 1.;
        md_list* _pVals   = NULL;


      public:
        filterValue(uint8_t filtAnz, uint8_t killPeek = 0, double offset = 0., double gain = 1.);
        ~filterValue();

        void   init(uint8_t filtAnz, uint8_t killPeek = 0, double offset = 0., double gain = 1.);
        void   clear();
        double value(double inval);
        double value();

      private:
        void   sum();
        void   sortLast();
        void   move(md0_val* pCell, int8_t dest);
        void   inc (md0_val* pIn);
        void   dec (md0_val* pIn);
    };

#endif