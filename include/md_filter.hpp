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