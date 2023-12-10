#ifndef _MD_PROJECTS_H_
  #define _MD_PROJECTS_H_

  /*+++ workspaces and projects +++++++++++++++++++++++++++++
   *
   * workspace
   *   is a VSCode workspace project - maybe only directory project
   *   is to be defined in every project environment [env:...]
   *
   * project
   *   using projects definition is a way the use same sourcecode implementation
   *   for different tasks and use minimum codesize and compiletime
   *   installing a project in 'platformio.h'
   *   + [platformio]
   *       default_envs= <PROJECT_NAME>   ; from this file
   *     [env:PROJECT_NAME]
   *       build_flags = <project number> ; from this file
   *
   * how it works
   *   + usualy all sourcefiles inside the workspace will be compiled and
   *     the used libraries have to be included
   *   + the '-D ...' defines in 'platformio' are transfered by the compiler
   *     statement and exist for all files in project
   *   + because of this it is possible to use '#ifdef' statements in
   *     library files to prevent compilation compilation and integration
   *     of libraries that are not necessary
   *   + define a 'USE_***' statement in 'platformio.h' for
   *     each library or/and device
   *     i.e. 'build_flags = -D USE_I2C
   *                         -D USE_BME280
   *   + an internal library file in project directory 'md_stdlib' is used for
   *     integration of external libraries, that are automatic downloaded
   *
   * directory structure and integration of projects and libraries
   *   + projects   - <workspace 1>         (contains projects)
   *                - <workspace 2>
   *                - md_stdlib
   *                                - <library 1>
   *                                - <library 2>
   *                                - <library ...>
   *                                - <library n>
   *   + in my code system there is an own github repo for every library
   *     in VSCode they can be cloned under directory 'md_stdlib'
   *   + to use a library from an user workspace under directory 'projects'
   *     use entry in 'platformio.h'
   *     [env:PROJECT_NAME]
   *       lib_extra_dir = './../md_stdlib/<library name>
   *
  *///++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // workspace names in visual studio code - 0xww00
    #define ESP32_TEST_MD_STDLIB    0x0100u
    #define MD_TINYHOUSE_BLUETTI    0x0200u

  //  test contained by workspace ESP32_TEST_MD_STDLIB - 0x**pp
    #define PRJ_TEST_LIB_OLED     ESP32_TEST_MD_STDLIB + 1 // 257
    #define PRJ_TEST_LIB_BME280   ESP32_TEST_MD_STDLIB + 2 // 258
    #define PRJ_TEST_LIB_TFT      ESP32_TEST_MD_STDLIB + 3 // 259
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 4
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 5
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
    //#define PRJ_TEST_LIB_         ESP32_TEST_MD_STDLIB + 6
#endif