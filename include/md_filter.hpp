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

  #define VAL_DEBUG CFG_DEBUG_NONE
  //#define VAL_DEBUG CFG_DEBUG_STARTUP
  //#define VAL_DEBUG CFG_DEBUG_ACTIONS
  //#define VAL_DEBUG CFG_DEBUG_DETAILS

  // class filterValue handles measurement values
  //                   inside floating filtering
  class md_val : public md_cell
    {
      private:
        double  _val  = 0.;
        int8_t  _idx  = NN;
        int8_t  _filt = 0;

      public:
        md_val(double val, int8_t idx);
        ~md_val();

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
        md_list* _pVals   = NULL;

      public:
        filterValue(uint8_t filtAnz, uint8_t killPeek = 0);
        ~filterValue();

        void   init(uint8_t filtAnz, uint8_t killPeek = 0);
        void   clear();
        double value(double inval);
        double value();

      private:
        void   sum();
        void   sortLast();
        void   move(md_val* pCell, int8_t dest);
        void   inc (md_val* pIn);
        void   dec (md_val* pIn);
    };

#endif