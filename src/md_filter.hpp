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
  //#include <list>

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

  //template<typename T>
  class md_scale
    {
      private:
        float _offsetRaw  = 0;
        float _offsetReal = 0;
        float _gain       = 1;

      public:
        md_scale() {}

        void  init(float offsetRaw = 0, float gain = 1, float offsetReal = 0)
                  { setScale(offsetRaw, gain, offsetReal); }

        void  setScale(float offsetRaw, float gain, float offsetReal = 0)
                  { _offsetRaw = offsetRaw; _gain = gain; _offsetReal = offsetReal; }

        float scale(float val)
                  { return (( val + _offsetRaw) * _gain + _offsetReal); }
    };

  class md_val // : public md_scale
    {
      private:
        float*    _pvals    = NULL;
        uint8_t*  _psort    = NULL;
        uint8_t   _vIdx     = 0;
        uint8_t   _sIdx     = 0;
        uint8_t   _cnt      = 0;
        uint8_t   _numFilts = 0;
        uint8_t   _numDrops = 0;
        filter_t  _tfilt    = FILT_NU;
        float     _val;
        float     _offsetRaw   = 0;
        float     _gain        = 1;
        float     _offsetReal  = 0;

      public:
                md_val() {}
                ~md_val()
                  {
                    if (_pvals != NULL) { delete _pvals;  }
                    if (_psort != NULL) { delete _psort; }
                  }

        void    begin(uint8_t numFilts = 0, uint8_t numDrops = 0, filter_t filtType = FILT_NU)
                  {
                    if (numFilts > 1)
                      {
                            SOUT(" new _pvals ");
                        _pvals = new float[numFilts];
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
                            SOUT(" drops "); SOUT(_numDrops); SOUTLN(" ");
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

                  }

        float   getVal()
                  { return _val; }

        float   doVal (float newVal)
                  {
                    // no filter with return
                      //SOUT(" doVal "); SOUT(newVal); SOUT(" ... ");
                    if (_numFilts == 0)
                      {
                        _val = newVal;
                        return getVal();
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
                    return getVal();
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
        /*void    setScale(T offsetRaw, double gain, T offsetReal)
                  {
                    _offsetRaw = offsetRaw;
                    _gain = gain;
                    _offsetReal = offsetReal;
                  }
          */
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
                         //SOUT("  calc ");
                    if (_tfilt == FILT_FL_MEAN)
                      {
                        uint32_t  sum    = 0;
                        uint8_t   begSum = 0;
                        uint8_t   endSum = _cnt - 1;
                        int8_t    cnt    = _cnt - (_numDrops << 1);
                        if (cnt <= 0) { cnt = _cnt; }
                        else
                          {
                            begSum = _numDrops;
                            endSum = _cnt - _numDrops -1;
                          }
                            //SOUT(begSum); SOUT(" - "); SOUT(endSum);
                        for (uint8_t i = 0 ; i < cnt ; i++)
                          {
                                //SOUT(" ");
                            sum += _pvals[_psort[i]];
                                //SOUT(sum);
                          }
                        _val = (float) (sum / cnt);
                                //SOUT(" _val "); SOUTLN(_val);
                      }
                  }

        /*T*      scale(T* pValue)
                  {
                    *pValue = (T) (  ((double) *pValue + (double) _offsetRaw )
                                   * _gain + (double) _offsetReal );
                    return pValue;
                  }
          */
    };

  #ifdef UNUSED  // class md0_val and filtervalue is replaced by md_val
    // class md0_val
      /*---------------------------------------------------------------------
        A measurement value is embedded in a cell to be used in a managed list
        md0val adds a list index and an index for filtering
      \*---------------------------------------------------------------------*/
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
    // class filterValue
      /*---------------------------------------------------------------------
        filterValue is a container for a measurement value
        - only 'double' values are stored
        - only raw values are stored in a ring buffer (no scaling, no filtering)
        - the count of values to be filtered are stored as a list and sorted

        filtervalue.value() returnes a value that is first filtered and then scaled


       *---------------------------------------------------------------------*/
    /* filtervalue is replaced by md_val
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
            filterValue();
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
      */
    #endif
#endif