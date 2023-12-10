/*************************************************************************
|* filename:   dict_list.h
 * project:    part of standard utilities
 * author:     Martin DORFNER (MD)
 * maintainer: Martin DORFNER
 * email:      git@martin-dorfner.at
 * Date:       10.12.2023
 *-----------------------------------------------------------------------
 * function:
 *   provide dictionary
 *-----------------------------------------------------------------------
 * external dependancies:
 *   - no external dependancies
 *-----------------------------------------------------------------------
 * dependancies:
 *   - transfer C-type statement with make statement
 *     - '#define USE_DICT_LIST=1'
 *     use VSCode/PlatformIO
 *        -> 'platformio.h'
 *           -> [env:<project_name>]
 *                build_flags = -D USE_DICT_LIST=1
 *   - md_defines.h    (utility intern)
 *   - md_list.h   base class
 *-----------------------------------------------------------------------
 * state of coding:
 *   - initial historic coding
 *   - state is unknown
 *   TODO  testing
 ************************************************************************
 * Version| Date   | Changes                                    | Autor
 *-----------------------------------------------------------------------
 * 0.001  |10.12.23| import from former project                 | MD
*-----------------------------------------------------------------------*/
#ifndef _MD_DICT_LIST_H_
  #define _MD_DICT_LIST_H_

  #ifdef USE_MD_DICT_LIST
      #include <stdlib.h>
      #include <Arduino.h>
      #include <md_defines.h>
      #ifndef USE_MD_LIST // integrate if not defined
          #define USE_MD_LIST
        #endif // USE_MD_LIST
      #include <md_list.h>
      // --- dictionary classes dict_cell, dict_list
        class dict_cell : public md_cell
          {
            private:
              int16_t  _idx = (int16_t) NN;
              char     _name[DICT_MAX_NAME_LEN+1] = "";

            public:
              dict_cell() {}
              ~dict_cell() {}

              void    init   (int16_t idx, const char name[DICT_MAX_NAME_LEN]);
              int16_t getIdx ()             { return _idx; }
              void    getName(char* pname)  { strcpy( pname, _name); }
              void    setIdx (int16_t idx)  { _idx = idx; }
              void    setName(const char name[DICT_MAX_NAME_LEN]);
          };
        class dict_list : public md_list
          {
            private:
              dict_cell* _pFirst;
              dict_cell* _pLast;

            public:
              dict_list();
              ~dict_list();

              void    append( int16_t idx, const char name[DICT_MAX_NAME_LEN]);
              int16_t getIdx (const char name[DICT_MAX_NAME_LEN]);
              void    getName(int16_t idx, char* pname);
              void    setIdx (const char name[DICT_MAX_NAME_LEN], int16_t idx);
              void    setName(int16_t idx, const char name[DICT_MAX_NAME_LEN]);
              dict_cell* getCellPointer( unsigned short index);
              dict_cell* getNextCellPointer( dict_cell* pCell );

            protected:
              dict_cell* _findCell(int16_t idx);
              dict_cell* _findCell(const char name[]);
          };
    #endif // USE_MD_DICT_LIST
#endif // _MD_DICT_LIST_H_