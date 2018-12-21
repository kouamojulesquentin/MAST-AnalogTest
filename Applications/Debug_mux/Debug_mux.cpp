//===========================================================================
//                           Debug_mux.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Debug_mux.cpp
//!
//! Implements PDL algorithms of tutorial 1
//!
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "CPP_API.hpp"
#include "g3log/g3log.hpp"

#include <iostream>
#include <random>
#include <functional>
#include <chrono>

using namespace mast;
using namespace std;


namespace
{

  void Debug_mux ()
  {
    auto     register_1 = "reg_1";
    auto     register_2 = "reg_2";
    auto     registerSize = 8;
    bool mismatch = false;

    uint16_t curValue_1;
    uint16_t writtenValue_1;
    uint16_t curValue_2;
    uint16_t writtenValue_2;

   auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
   auto word_rand = std::bind(std::uniform_int_distribution<int>(0,1<<registerSize),
                           mt19937(seed));


    std::cout << "\nMaking random iWrites on registers  " <<register_1 <<" and "<< register_1 << "\n";

    writtenValue_1 = word_rand();
    writtenValue_2 = word_rand();
    iWrite(register_1, writtenValue_1);
    iWrite(register_2, writtenValue_2);
    iApply();
    
     curValue_1=iGetRefresh<uint16_t>(register_1);
     curValue_2=iGetRefresh<uint16_t>(register_2);

     if  (writtenValue_1 == curValue_1)      std::cout << register_1 << "  OK\n"; else
        { std::cout << register_1 << " ERROR: wrote "<<writtenValue_1<<" read "<<curValue_1<<"\n";mismatch= true;}
     if  (writtenValue_2 == curValue_2)      std::cout << register_2 << "  OK\n"; else
       {std::cout << register_2 << " ERROR: wrote "<<writtenValue_2<<" read "<<curValue_2<<"\n";;mismatch= true;}
       
     if (mismatch)
      {
       if (curValue_1 == writtenValue_2) std::cout << "\tPossible Stuck at 1 in Mux\n";
       if (curValue_2 == writtenValue_1) std::cout << "\tPossible Stuck at 0 in Mux\n";
      }  

  }

  //
  //  End of: Debug_mux
  //---------------------------------------------------------------------------


  void Check_TDR (int registerSize, bool do_check, int ncycles)
  {
    auto     register_1 = "TDR";
    bool mismatch = false;

    uint32_t curValue_1;
    uint32_t writtenValue_1;
    uint32_t old_writtenValue_1 = 0x0;

//   auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
 //  Fixed seed for reproductibility
   auto seed = 0x1234;
  // uint64_t range = (1<<30)*2;
   
    
//   if (registerSize < 32) 
     //auto range = 1<<registerSize;
     
    auto word_rand = std::bind(std::uniform_int_distribution<unsigned long int>(0,pow(2.0,registerSize)),
                           mt19937(seed));
   

    std::cout << "\nMaking "<< ncycles << " random iWrites on registers  " <<register_1<<endl;

  for (auto count=0;count<ncycles;count++)
   {
    writtenValue_1 = (uint32_t) word_rand();
    iWrite(register_1, writtenValue_1);
    iApply();
    curValue_1=iGet<uint32_t>(register_1);
    std::cout << "Round " << count << " Random value is "<< std::hex<<writtenValue_1<< "\n";
    if (do_check)
     {
     if  (old_writtenValue_1 == curValue_1)      std::cout << register_1 << "  OK\n"; else
        { std::cout << register_1 << " ERROR: wrote "<< std::hex <<old_writtenValue_1<<" read "<< std::hex<<curValue_1<<"\n";mismatch= true;}
       
     if (mismatch)  std::cout << "\tError in Selecting "<< register_1 << " \n";
     }
    old_writtenValue_1 = writtenValue_1;
    } 
  }


  void Check_TDR_string (int registerSize, bool do_check, int ncycles)
  {
    auto     register_1 = "TDR";
    bool mismatch = false;
    

    std::string curValue_1;
    std::string writtenValue_1;
    std::string old_writtenValue_1 = "0b0";

//   auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
 //  Fixed seed for reproductibility
   auto seed = 0x1234;
  // uint64_t range = (1<<30)*2;
   
    auto word_rand = std::bind(std::uniform_int_distribution<unsigned long int>(0,pow(2.0,32)),
                           mt19937(seed));

   auto random_binary_string = [&] (int registerSize)
    {
//    std::cout << "random_binary_string for "<<registerSize<<" bits: ";
    std::string random_bin_string;
    random_bin_string+="0b";
    auto n_words=registerSize/16;
    auto offset_bits=registerSize%16;
//    std::cout << n_words <<" half words and "<<offset_bits<<" offset_bits: ";
    auto cur_bits=0;
    
    while (n_words>=0)
     {
     uint32_t rand_word= (uint32_t) word_rand();                       
      if (n_words>0) cur_bits=16; else cur_bits= offset_bits;
     while (cur_bits-->0)
      {
       (rand_word%2)?random_bin_string+='1':random_bin_string+='0';
      rand_word = rand_word>>2;
      }
      n_words --;
     }
     
//     std::cout << random_bin_string <<"\n";
     return  random_bin_string;
    };   

    std::cout << "\nMaking "<< ncycles << " random iWrites on registers  " <<register_1<<endl;

  for (auto count=0;count<ncycles;count++)
   {
    writtenValue_1 = random_binary_string(registerSize);
    iWrite(register_1, writtenValue_1);
    iApply();
    curValue_1=iGet(register_1,StringType::Binary);
    std::cout << "Round " << count << " Random value is "<< std::hex<<writtenValue_1<< "\n";
    if (do_check)
     {
     if  (old_writtenValue_1 == curValue_1)      std::cout << register_1 << "  OK\n"; else
        { std::cout << register_1 << " ERROR: wrote "<< std::hex <<old_writtenValue_1<<" read "<< std::hex<<curValue_1<<"\n";mismatch= true;}
       
     if (mismatch)  std::cout << "\tError in Selecting "<< register_1 << " \n";
     }
    old_writtenValue_1 = writtenValue_1;
    } 
  }

  void Check_TDR_8  () {Check_TDR (8 ,false,2);}
  void Check_TDR_16 () {Check_TDR (16,false,2);}
  void Check_TDR_24 () {Check_TDR (24,false,2);}
  void Check_TDR_32 () {Check_TDR_string (32,false,2);}
  
void Check_TDR_0 () { Check_TDR_string (0 ,false,2);}
void Check_TDR_1 () { Check_TDR_string (1 ,false,2);}
void Check_TDR_10 () { Check_TDR_string (10 ,false,2);}
void Check_TDR_100 () { Check_TDR_string (100 ,false,2);}
void Check_TDR_101 () { Check_TDR_string (101 ,false,2);}
void Check_TDR_102 () { Check_TDR_string (102 ,false,2);}
void Check_TDR_1028 () { Check_TDR_string (1028 ,false,2);}
void Check_TDR_103 () { Check_TDR_string (103 ,false,2);}
void Check_TDR_104 () { Check_TDR_string (104 ,false,2);}
void Check_TDR_105 () { Check_TDR_string (105 ,false,2);}
void Check_TDR_106 () { Check_TDR_string (106 ,false,2);}
void Check_TDR_107 () { Check_TDR_string (107 ,false,2);}
void Check_TDR_108 () { Check_TDR_string (108 ,false,2);}
void Check_TDR_109 () { Check_TDR_string (109 ,false,2);}
void Check_TDR_1097 () { Check_TDR_string (1097 ,false,2);}
void Check_TDR_1099 () { Check_TDR_string (1099 ,false,2);}
void Check_TDR_11 () { Check_TDR_string (11 ,false,2);}
void Check_TDR_110 () { Check_TDR_string (110 ,false,2);}
void Check_TDR_1109 () { Check_TDR_string (1109 ,false,2);}
void Check_TDR_111 () { Check_TDR_string (111 ,false,2);}
void Check_TDR_112 () { Check_TDR_string (112 ,false,2);}
void Check_TDR_1121 () { Check_TDR_string (1121 ,false,2);}
void Check_TDR_1122 () { Check_TDR_string (1122 ,false,2);}
void Check_TDR_113 () { Check_TDR_string (113 ,false,2);}
void Check_TDR_114 () { Check_TDR_string (114 ,false,2);}
void Check_TDR_115 () { Check_TDR_string (115 ,false,2);}
void Check_TDR_1153 () { Check_TDR_string (1153 ,false,2);}
void Check_TDR_116 () { Check_TDR_string (116 ,false,2);}
void Check_TDR_117 () { Check_TDR_string (117 ,false,2);}
void Check_TDR_118 () { Check_TDR_string (118 ,false,2);}
void Check_TDR_1181 () { Check_TDR_string (1181 ,false,2);}
void Check_TDR_119 () { Check_TDR_string (119 ,false,2);}
void Check_TDR_12 () { Check_TDR_string (12 ,false,2);}
void Check_TDR_120 () { Check_TDR_string (120 ,false,2);}
void Check_TDR_1209 () { Check_TDR_string (1209 ,false,2);}
void Check_TDR_121 () { Check_TDR_string (121 ,false,2);}
void Check_TDR_122 () { Check_TDR_string (122 ,false,2);}
void Check_TDR_1225 () { Check_TDR_string (1225 ,false,2);}
void Check_TDR_123 () { Check_TDR_string (123 ,false,2);}
void Check_TDR_124 () { Check_TDR_string (124 ,false,2);}
void Check_TDR_125 () { Check_TDR_string (125 ,false,2);}
void Check_TDR_126 () { Check_TDR_string (126 ,false,2);}
void Check_TDR_1263 () { Check_TDR_string (1263 ,false,2);}
void Check_TDR_1264 () { Check_TDR_string (1264 ,false,2);}
void Check_TDR_127 () { Check_TDR_string (127 ,false,2);}
void Check_TDR_129 () { Check_TDR_string (129 ,false,2);}
void Check_TDR_1293 () { Check_TDR_string (1293 ,false,2);}
void Check_TDR_1297 () { Check_TDR_string (1297 ,false,2);}
void Check_TDR_13 () { Check_TDR_string (13 ,false,2);}
void Check_TDR_130 () { Check_TDR_string (130 ,false,2);}
void Check_TDR_131 () { Check_TDR_string (131 ,false,2);}
void Check_TDR_1316 () { Check_TDR_string (1316 ,false,2);}
void Check_TDR_132 () { Check_TDR_string (132 ,false,2);}
void Check_TDR_133 () { Check_TDR_string (133 ,false,2);}
void Check_TDR_134 () { Check_TDR_string (134 ,false,2);}
void Check_TDR_136 () { Check_TDR_string (136 ,false,2);}
void Check_TDR_1371 () { Check_TDR_string (1371 ,false,2);}
void Check_TDR_1372 () { Check_TDR_string (1372 ,false,2);}
void Check_TDR_138 () { Check_TDR_string (138 ,false,2);}
void Check_TDR_139 () { Check_TDR_string (139 ,false,2);}
void Check_TDR_14 () { Check_TDR_string (14 ,false,2);}
void Check_TDR_140 () { Check_TDR_string (140 ,false,2);}
void Check_TDR_1400 () { Check_TDR_string (1400 ,false,2);}
void Check_TDR_141 () { Check_TDR_string (141 ,false,2);}
void Check_TDR_1412 () { Check_TDR_string (1412 ,false,2);}
void Check_TDR_142 () { Check_TDR_string (142 ,false,2);}
void Check_TDR_143 () { Check_TDR_string (143 ,false,2);}
void Check_TDR_144 () { Check_TDR_string (144 ,false,2);}
void Check_TDR_145 () { Check_TDR_string (145 ,false,2);}
void Check_TDR_146 () { Check_TDR_string (146 ,false,2);}
void Check_TDR_147 () { Check_TDR_string (147 ,false,2);}
void Check_TDR_1472 () { Check_TDR_string (1472 ,false,2);}
void Check_TDR_1473 () { Check_TDR_string (1473 ,false,2);}
void Check_TDR_148 () { Check_TDR_string (148 ,false,2);}
void Check_TDR_149 () { Check_TDR_string (149 ,false,2);}
void Check_TDR_15 () { Check_TDR_string (15 ,false,2);}
void Check_TDR_150 () { Check_TDR_string (150 ,false,2);}
void Check_TDR_151 () { Check_TDR_string (151 ,false,2);}
void Check_TDR_152 () { Check_TDR_string (152 ,false,2);}
void Check_TDR_154 () { Check_TDR_string (154 ,false,2);}
void Check_TDR_1549 () { Check_TDR_string (1549 ,false,2);}
void Check_TDR_155 () { Check_TDR_string (155 ,false,2);}
void Check_TDR_1550 () { Check_TDR_string (1550 ,false,2);}
void Check_TDR_157 () { Check_TDR_string (157 ,false,2);}
void Check_TDR_161 () { Check_TDR_string (161 ,false,2);}
void Check_TDR_162 () { Check_TDR_string (162 ,false,2);}
void Check_TDR_1626 () { Check_TDR_string (1626 ,false,2);}
void Check_TDR_163 () { Check_TDR_string (163 ,false,2);}
void Check_TDR_164 () { Check_TDR_string (164 ,false,2);}
void Check_TDR_1644 () { Check_TDR_string (1644 ,false,2);}
void Check_TDR_165 () { Check_TDR_string (165 ,false,2);}
void Check_TDR_166 () { Check_TDR_string (166 ,false,2);}
void Check_TDR_1667 () { Check_TDR_string (1667 ,false,2);}
void Check_TDR_1669 () { Check_TDR_string (1669 ,false,2);}
void Check_TDR_168 () { Check_TDR_string (168 ,false,2);}
void Check_TDR_1689 () { Check_TDR_string (1689 ,false,2);}
void Check_TDR_17 () { Check_TDR_string (17 ,false,2);}
void Check_TDR_170 () { Check_TDR_string (170 ,false,2);}
void Check_TDR_171 () { Check_TDR_string (171 ,false,2);}
void Check_TDR_173 () { Check_TDR_string (173 ,false,2);}
void Check_TDR_174 () { Check_TDR_string (174 ,false,2);}
void Check_TDR_175 () { Check_TDR_string (175 ,false,2);}
void Check_TDR_177 () { Check_TDR_string (177 ,false,2);}
void Check_TDR_1785 () { Check_TDR_string (1785 ,false,2);}
void Check_TDR_179 () { Check_TDR_string (179 ,false,2);}
void Check_TDR_18 () { Check_TDR_string (18 ,false,2);}
void Check_TDR_180 () { Check_TDR_string (180 ,false,2);}
void Check_TDR_181 () { Check_TDR_string (181 ,false,2);}
void Check_TDR_182 () { Check_TDR_string (182 ,false,2);}
void Check_TDR_1824 () { Check_TDR_string (1824 ,false,2);}
void Check_TDR_183 () { Check_TDR_string (183 ,false,2);}
void Check_TDR_184 () { Check_TDR_string (184 ,false,2);}
void Check_TDR_185 () { Check_TDR_string (185 ,false,2);}
void Check_TDR_186 () { Check_TDR_string (186 ,false,2);}
void Check_TDR_1862 () { Check_TDR_string (1862 ,false,2);}
void Check_TDR_187 () { Check_TDR_string (187 ,false,2);}
void Check_TDR_1870 () { Check_TDR_string (1870 ,false,2);}
void Check_TDR_188 () { Check_TDR_string (188 ,false,2);}
void Check_TDR_1882 () { Check_TDR_string (1882 ,false,2);}
void Check_TDR_1884 () { Check_TDR_string (1884 ,false,2);}
void Check_TDR_189 () { Check_TDR_string (189 ,false,2);}
void Check_TDR_19 () { Check_TDR_string (19 ,false,2);}
void Check_TDR_190 () { Check_TDR_string (190 ,false,2);}
void Check_TDR_191 () { Check_TDR_string (191 ,false,2);}
void Check_TDR_192 () { Check_TDR_string (192 ,false,2);}
void Check_TDR_198 () { Check_TDR_string (198 ,false,2);}
void Check_TDR_1987 () { Check_TDR_string (1987 ,false,2);}
void Check_TDR_199 () { Check_TDR_string (199 ,false,2);}
void Check_TDR_2 () { Check_TDR_string (2 ,false,2);}
void Check_TDR_20 () { Check_TDR_string (20 ,false,2);}
void Check_TDR_2012 () { Check_TDR_string (2012 ,false,2);}
void Check_TDR_2015 () { Check_TDR_string (2015 ,false,2);}
void Check_TDR_203 () { Check_TDR_string (203 ,false,2);}
void Check_TDR_2035 () { Check_TDR_string (2035 ,false,2);}
void Check_TDR_2049 () { Check_TDR_string (2049 ,false,2);}
void Check_TDR_206 () { Check_TDR_string (206 ,false,2);}
void Check_TDR_207 () { Check_TDR_string (207 ,false,2);}
void Check_TDR_208 () { Check_TDR_string (208 ,false,2);}
void Check_TDR_2099 () { Check_TDR_string (2099 ,false,2);}
void Check_TDR_21 () { Check_TDR_string (21 ,false,2);}
void Check_TDR_211 () { Check_TDR_string (211 ,false,2);}
void Check_TDR_212 () { Check_TDR_string (212 ,false,2);}
void Check_TDR_213 () { Check_TDR_string (213 ,false,2);}
void Check_TDR_214 () { Check_TDR_string (214 ,false,2);}
void Check_TDR_2141 () { Check_TDR_string (2141 ,false,2);}
void Check_TDR_215 () { Check_TDR_string (215 ,false,2);}
void Check_TDR_2155 () { Check_TDR_string (2155 ,false,2);}
void Check_TDR_216 () { Check_TDR_string (216 ,false,2);}
void Check_TDR_218 () { Check_TDR_string (218 ,false,2);}
void Check_TDR_2181 () { Check_TDR_string (2181 ,false,2);}
void Check_TDR_219 () { Check_TDR_string (219 ,false,2);}
void Check_TDR_2195 () { Check_TDR_string (2195 ,false,2);}
void Check_TDR_22 () { Check_TDR_string (22 ,false,2);}
void Check_TDR_2202 () { Check_TDR_string (2202 ,false,2);}
void Check_TDR_221 () { Check_TDR_string (221 ,false,2);}
void Check_TDR_2227 () { Check_TDR_string (2227 ,false,2);}
void Check_TDR_2241 () { Check_TDR_string (2241 ,false,2);}
void Check_TDR_226 () { Check_TDR_string (226 ,false,2);}
void Check_TDR_23 () { Check_TDR_string (23 ,false,2);}
void Check_TDR_2304 () { Check_TDR_string (2304 ,false,2);}
void Check_TDR_2403 () { Check_TDR_string (2403 ,false,2);}
void Check_TDR_2442 () { Check_TDR_string (2442 ,false,2);}
void Check_TDR_25 () { Check_TDR_string (25 ,false,2);}
void Check_TDR_2520 () { Check_TDR_string (2520 ,false,2);}
void Check_TDR_253 () { Check_TDR_string (253 ,false,2);}
void Check_TDR_2532 () { Check_TDR_string (2532 ,false,2);}
void Check_TDR_2540 () { Check_TDR_string (2540 ,false,2);}
void Check_TDR_2548 () { Check_TDR_string (2548 ,false,2);}
void Check_TDR_255 () { Check_TDR_string (255 ,false,2);}
void Check_TDR_2565 () { Check_TDR_string (2565 ,false,2);}
void Check_TDR_2576 () { Check_TDR_string (2576 ,false,2);}
void Check_TDR_26 () { Check_TDR_string (26 ,false,2);}
void Check_TDR_2626 () { Check_TDR_string (2626 ,false,2);}
void Check_TDR_2629 () { Check_TDR_string (2629 ,false,2);}
void Check_TDR_263 () { Check_TDR_string (263 ,false,2);}
void Check_TDR_2632 () { Check_TDR_string (2632 ,false,2);}
void Check_TDR_2646 () { Check_TDR_string (2646 ,false,2);}
void Check_TDR_2671 () { Check_TDR_string (2671 ,false,2);}
void Check_TDR_2679 () { Check_TDR_string (2679 ,false,2);}
void Check_TDR_268 () { Check_TDR_string (268 ,false,2);}
void Check_TDR_2680 () { Check_TDR_string (2680 ,false,2);}
void Check_TDR_2692 () { Check_TDR_string (2692 ,false,2);}
void Check_TDR_27 () { Check_TDR_string (27 ,false,2);}
void Check_TDR_270 () { Check_TDR_string (270 ,false,2);}
void Check_TDR_2701 () { Check_TDR_string (2701 ,false,2);}
void Check_TDR_2707 () { Check_TDR_string (2707 ,false,2);}
void Check_TDR_2710 () { Check_TDR_string (2710 ,false,2);}
void Check_TDR_2725 () { Check_TDR_string (2725 ,false,2);}
void Check_TDR_273 () { Check_TDR_string (273 ,false,2);}
void Check_TDR_276 () { Check_TDR_string (276 ,false,2);}
void Check_TDR_2761 () { Check_TDR_string (2761 ,false,2);}
void Check_TDR_278 () { Check_TDR_string (278 ,false,2);}
void Check_TDR_279 () { Check_TDR_string (279 ,false,2);}
void Check_TDR_28 () { Check_TDR_string (28 ,false,2);}
void Check_TDR_2817 () { Check_TDR_string (2817 ,false,2);}
void Check_TDR_2830 () { Check_TDR_string (2830 ,false,2);}
void Check_TDR_2834 () { Check_TDR_string (2834 ,false,2);}
void Check_TDR_285 () { Check_TDR_string (285 ,false,2);}
void Check_TDR_286 () { Check_TDR_string (286 ,false,2);}
void Check_TDR_2876 () { Check_TDR_string (2876 ,false,2);}
void Check_TDR_29 () { Check_TDR_string (29 ,false,2);}
void Check_TDR_2944 () { Check_TDR_string (2944 ,false,2);}
void Check_TDR_2963 () { Check_TDR_string (2963 ,false,2);}
void Check_TDR_2967 () { Check_TDR_string (2967 ,false,2);}
void Check_TDR_2977 () { Check_TDR_string (2977 ,false,2);}
void Check_TDR_3 () { Check_TDR_string (3 ,false,2);}
void Check_TDR_30 () { Check_TDR_string (30 ,false,2);}
void Check_TDR_301 () { Check_TDR_string (301 ,false,2);}
void Check_TDR_302 () { Check_TDR_string (302 ,false,2);}
void Check_TDR_303 () { Check_TDR_string (303 ,false,2);}
void Check_TDR_3072 () { Check_TDR_string (3072 ,false,2);}
void Check_TDR_308 () { Check_TDR_string (308 ,false,2);}
void Check_TDR_3091 () { Check_TDR_string (3091 ,false,2);}
void Check_TDR_31 () { Check_TDR_string (31 ,false,2);}
void Check_TDR_3152 () { Check_TDR_string (3152 ,false,2);}
void Check_TDR_316 () { Check_TDR_string (316 ,false,2);}
void Check_TDR_3180 () { Check_TDR_string (3180 ,false,2);}
void Check_TDR_3230 () { Check_TDR_string (3230 ,false,2);}
void Check_TDR_3232 () { Check_TDR_string (3232 ,false,2);}
void Check_TDR_3235 () { Check_TDR_string (3235 ,false,2);}
void Check_TDR_324 () { Check_TDR_string (324 ,false,2);}
void Check_TDR_327 () { Check_TDR_string (327 ,false,2);}
void Check_TDR_329 () { Check_TDR_string (329 ,false,2);}
void Check_TDR_33 () { Check_TDR_string (33 ,false,2);}
void Check_TDR_3307 () { Check_TDR_string (3307 ,false,2);}
void Check_TDR_3314 () { Check_TDR_string (3314 ,false,2);}
void Check_TDR_332 () { Check_TDR_string (332 ,false,2);}
void Check_TDR_333 () { Check_TDR_string (333 ,false,2);}
void Check_TDR_3337 () { Check_TDR_string (3337 ,false,2);}
void Check_TDR_334 () { Check_TDR_string (334 ,false,2);}
void Check_TDR_3358 () { Check_TDR_string (3358 ,false,2);}
void Check_TDR_3377 () { Check_TDR_string (3377 ,false,2);}
void Check_TDR_3379 () { Check_TDR_string (3379 ,false,2);}
void Check_TDR_3390 () { Check_TDR_string (3390 ,false,2);}
void Check_TDR_3394 () { Check_TDR_string (3394 ,false,2);}
void Check_TDR_34 () { Check_TDR_string (34 ,false,2);}
void Check_TDR_3404 () { Check_TDR_string (3404 ,false,2);}
void Check_TDR_343 () { Check_TDR_string (343 ,false,2);}
void Check_TDR_3454 () { Check_TDR_string (3454 ,false,2);}
void Check_TDR_3458 () { Check_TDR_string (3458 ,false,2);}
void Check_TDR_3473 () { Check_TDR_string (3473 ,false,2);}
void Check_TDR_348 () { Check_TDR_string (348 ,false,2);}
void Check_TDR_35 () { Check_TDR_string (35 ,false,2);}
void Check_TDR_350 () { Check_TDR_string (350 ,false,2);}
void Check_TDR_351 () { Check_TDR_string (351 ,false,2);}
void Check_TDR_3510 () { Check_TDR_string (3510 ,false,2);}
void Check_TDR_3519 () { Check_TDR_string (3519 ,false,2);}
void Check_TDR_352 () { Check_TDR_string (352 ,false,2);}
void Check_TDR_3543 () { Check_TDR_string (3543 ,false,2);}
void Check_TDR_3567 () { Check_TDR_string (3567 ,false,2);}
void Check_TDR_3585 () { Check_TDR_string (3585 ,false,2);}
void Check_TDR_36 () { Check_TDR_string (36 ,false,2);}
void Check_TDR_3601 () { Check_TDR_string (3601 ,false,2);}
void Check_TDR_361 () { Check_TDR_string (361 ,false,2);}
void Check_TDR_362 () { Check_TDR_string (362 ,false,2);}
void Check_TDR_3663 () { Check_TDR_string (3663 ,false,2);}
void Check_TDR_3693 () { Check_TDR_string (3693 ,false,2);}
void Check_TDR_37 () { Check_TDR_string (37 ,false,2);}
void Check_TDR_370 () { Check_TDR_string (370 ,false,2);}
void Check_TDR_372 () { Check_TDR_string (372 ,false,2);}
void Check_TDR_377 () { Check_TDR_string (377 ,false,2);}
void Check_TDR_3773 () { Check_TDR_string (3773 ,false,2);}
void Check_TDR_3780 () { Check_TDR_string (3780 ,false,2);}
void Check_TDR_3784 () { Check_TDR_string (3784 ,false,2);}
void Check_TDR_38 () { Check_TDR_string (38 ,false,2);}
void Check_TDR_381 () { Check_TDR_string (381 ,false,2);}
void Check_TDR_386 () { Check_TDR_string (386 ,false,2);}
void Check_TDR_3860 () { Check_TDR_string (3860 ,false,2);}
void Check_TDR_389 () { Check_TDR_string (389 ,false,2);}
void Check_TDR_39 () { Check_TDR_string (39 ,false,2);}
void Check_TDR_393 () { Check_TDR_string (393 ,false,2);}
void Check_TDR_3933 () { Check_TDR_string (3933 ,false,2);}
void Check_TDR_396 () { Check_TDR_string (396 ,false,2);}
void Check_TDR_3984 () { Check_TDR_string (3984 ,false,2);}
void Check_TDR_399 () { Check_TDR_string (399 ,false,2);}
void Check_TDR_4 () { Check_TDR_string (4 ,false,2);}
void Check_TDR_40 () { Check_TDR_string (40 ,false,2);}
void Check_TDR_400 () { Check_TDR_string (400 ,false,2);}
void Check_TDR_4034 () { Check_TDR_string (4034 ,false,2);}
void Check_TDR_406 () { Check_TDR_string (406 ,false,2);}
void Check_TDR_4064 () { Check_TDR_string (4064 ,false,2);}
void Check_TDR_408 () { Check_TDR_string (408 ,false,2);}
void Check_TDR_41 () { Check_TDR_string (41 ,false,2);}
void Check_TDR_4125 () { Check_TDR_string (4125 ,false,2);}
void Check_TDR_413 () { Check_TDR_string (413 ,false,2);}
void Check_TDR_4139 () { Check_TDR_string (4139 ,false,2);}
void Check_TDR_417 () { Check_TDR_string (417 ,false,2);}
void Check_TDR_4195 () { Check_TDR_string (4195 ,false,2);}
void Check_TDR_42 () { Check_TDR_string (42 ,false,2);}
void Check_TDR_4202 () { Check_TDR_string (4202 ,false,2);}
void Check_TDR_4210 () { Check_TDR_string (4210 ,false,2);}
void Check_TDR_4215 () { Check_TDR_string (4215 ,false,2);}
void Check_TDR_4259 () { Check_TDR_string (4259 ,false,2);}
void Check_TDR_4282 () { Check_TDR_string (4282 ,false,2);}
void Check_TDR_429 () { Check_TDR_string (429 ,false,2);}
void Check_TDR_43 () { Check_TDR_string (43 ,false,2);}
void Check_TDR_4339 () { Check_TDR_string (4339 ,false,2);}
void Check_TDR_4368 () { Check_TDR_string (4368 ,false,2);}
void Check_TDR_437 () { Check_TDR_string (437 ,false,2);}
void Check_TDR_4377 () { Check_TDR_string (4377 ,false,2);}
void Check_TDR_438 () { Check_TDR_string (438 ,false,2);}
void Check_TDR_439 () { Check_TDR_string (439 ,false,2);}
void Check_TDR_4396 () { Check_TDR_string (4396 ,false,2);}
void Check_TDR_44 () { Check_TDR_string (44 ,false,2);}
void Check_TDR_4439 () { Check_TDR_string (4439 ,false,2);}
void Check_TDR_4474 () { Check_TDR_string (4474 ,false,2);}
void Check_TDR_448 () { Check_TDR_string (448 ,false,2);}
void Check_TDR_45 () { Check_TDR_string (45 ,false,2);}
void Check_TDR_454 () { Check_TDR_string (454 ,false,2);}
void Check_TDR_46 () { Check_TDR_string (46 ,false,2);}
void Check_TDR_47 () { Check_TDR_string (47 ,false,2);}
void Check_TDR_478 () { Check_TDR_string (478 ,false,2);}
void Check_TDR_48 () { Check_TDR_string (48 ,false,2);}
void Check_TDR_480 () { Check_TDR_string (480 ,false,2);}
void Check_TDR_489 () { Check_TDR_string (489 ,false,2);}
void Check_TDR_49 () { Check_TDR_string (49 ,false,2);}
void Check_TDR_5 () { Check_TDR_string (5 ,false,2);}
void Check_TDR_50 () { Check_TDR_string (50 ,false,2);}
void Check_TDR_500 () { Check_TDR_string (500 ,false,2);}
void Check_TDR_501 () { Check_TDR_string (501 ,false,2);}
void Check_TDR_502 () { Check_TDR_string (502 ,false,2);}
void Check_TDR_506 () { Check_TDR_string (506 ,false,2);}
void Check_TDR_51 () { Check_TDR_string (51 ,false,2);}
void Check_TDR_514 () { Check_TDR_string (514 ,false,2);}
void Check_TDR_515 () { Check_TDR_string (515 ,false,2);}
void Check_TDR_519 () { Check_TDR_string (519 ,false,2);}
void Check_TDR_52 () { Check_TDR_string (52 ,false,2);}
void Check_TDR_520 () { Check_TDR_string (520 ,false,2);}
void Check_TDR_521 () { Check_TDR_string (521 ,false,2);}
void Check_TDR_53 () { Check_TDR_string (53 ,false,2);}
void Check_TDR_530 () { Check_TDR_string (530 ,false,2);}
void Check_TDR_538 () { Check_TDR_string (538 ,false,2);}
void Check_TDR_54 () { Check_TDR_string (54 ,false,2);}
void Check_TDR_540 () { Check_TDR_string (540 ,false,2);}
void Check_TDR_55 () { Check_TDR_string (55 ,false,2);}
void Check_TDR_556 () { Check_TDR_string (556 ,false,2);}
void Check_TDR_56 () { Check_TDR_string (56 ,false,2);}
void Check_TDR_560 () { Check_TDR_string (560 ,false,2);}
void Check_TDR_566 () { Check_TDR_string (566 ,false,2);}
void Check_TDR_567 () { Check_TDR_string (567 ,false,2);}
void Check_TDR_569 () { Check_TDR_string (569 ,false,2);}
void Check_TDR_57 () { Check_TDR_string (57 ,false,2);}
void Check_TDR_570 () { Check_TDR_string (570 ,false,2);}
void Check_TDR_58 () { Check_TDR_string (58 ,false,2);}
void Check_TDR_59 () { Check_TDR_string (59 ,false,2);}
void Check_TDR_6 () { Check_TDR_string (6 ,false,2);}
void Check_TDR_60 () { Check_TDR_string (60 ,false,2);}
void Check_TDR_61 () { Check_TDR_string (61 ,false,2);}
void Check_TDR_617 () { Check_TDR_string (617 ,false,2);}
void Check_TDR_62 () { Check_TDR_string (62 ,false,2);}
void Check_TDR_621 () { Check_TDR_string (621 ,false,2);}
void Check_TDR_63 () { Check_TDR_string (63 ,false,2);}
void Check_TDR_631 () { Check_TDR_string (631 ,false,2);}
void Check_TDR_635 () { Check_TDR_string (635 ,false,2);}
void Check_TDR_64 () { Check_TDR_string (64 ,false,2);}
void Check_TDR_645 () { Check_TDR_string (645 ,false,2);}
void Check_TDR_65 () { Check_TDR_string (65 ,false,2);}
void Check_TDR_655 () { Check_TDR_string (655 ,false,2);}
void Check_TDR_66 () { Check_TDR_string (66 ,false,2);}
void Check_TDR_67 () { Check_TDR_string (67 ,false,2);}
void Check_TDR_671 () { Check_TDR_string (671 ,false,2);}
void Check_TDR_68 () { Check_TDR_string (68 ,false,2);}
void Check_TDR_69 () { Check_TDR_string (69 ,false,2);}
void Check_TDR_690 () { Check_TDR_string (690 ,false,2);}
void Check_TDR_697 () { Check_TDR_string (697 ,false,2);}
void Check_TDR_7 () { Check_TDR_string (7 ,false,2);}
void Check_TDR_70 () { Check_TDR_string (70 ,false,2);}
void Check_TDR_701 () { Check_TDR_string (701 ,false,2);}
void Check_TDR_71 () { Check_TDR_string (71 ,false,2);}
void Check_TDR_72 () { Check_TDR_string (72 ,false,2);}
void Check_TDR_728 () { Check_TDR_string (728 ,false,2);}
void Check_TDR_729 () { Check_TDR_string (729 ,false,2);}
void Check_TDR_73 () { Check_TDR_string (73 ,false,2);}
void Check_TDR_74 () { Check_TDR_string (74 ,false,2);}
void Check_TDR_740 () { Check_TDR_string (740 ,false,2);}
void Check_TDR_75 () { Check_TDR_string (75 ,false,2);}
void Check_TDR_751 () { Check_TDR_string (751 ,false,2);}
void Check_TDR_753 () { Check_TDR_string (753 ,false,2);}
void Check_TDR_754 () { Check_TDR_string (754 ,false,2);}
void Check_TDR_76 () { Check_TDR_string (76 ,false,2);}
void Check_TDR_77 () { Check_TDR_string (77 ,false,2);}
void Check_TDR_777 () { Check_TDR_string (777 ,false,2);}
void Check_TDR_78 () { Check_TDR_string (78 ,false,2);}
void Check_TDR_79 () { Check_TDR_string (79 ,false,2);}
void Check_TDR_80 () { Check_TDR_string (80 ,false,2);}
void Check_TDR_800 () { Check_TDR_string (800 ,false,2);}
void Check_TDR_806 () { Check_TDR_string (806 ,false,2);}
void Check_TDR_81 () { Check_TDR_string (81 ,false,2);}
void Check_TDR_815 () { Check_TDR_string (815 ,false,2);}
void Check_TDR_82 () { Check_TDR_string (82 ,false,2);}
void Check_TDR_83 () { Check_TDR_string (83 ,false,2);}
void Check_TDR_84 () { Check_TDR_string (84 ,false,2);}
void Check_TDR_85 () { Check_TDR_string (85 ,false,2);}
void Check_TDR_850 () { Check_TDR_string (850 ,false,2);}
void Check_TDR_86 () { Check_TDR_string (86 ,false,2);}
void Check_TDR_87 () { Check_TDR_string (87 ,false,2);}
void Check_TDR_88 () { Check_TDR_string (88 ,false,2);}
void Check_TDR_884 () { Check_TDR_string (884 ,false,2);}
void Check_TDR_89 () { Check_TDR_string (89 ,false,2);}
void Check_TDR_891 () { Check_TDR_string (891 ,false,2);}
void Check_TDR_896 () { Check_TDR_string (896 ,false,2);}
void Check_TDR_897 () { Check_TDR_string (897 ,false,2);}
void Check_TDR_9 () { Check_TDR_string (9 ,false,2);}
void Check_TDR_90 () { Check_TDR_string (90 ,false,2);}
void Check_TDR_904 () { Check_TDR_string (904 ,false,2);}
void Check_TDR_91 () { Check_TDR_string (91 ,false,2);}
void Check_TDR_910 () { Check_TDR_string (910 ,false,2);}
void Check_TDR_911 () { Check_TDR_string (911 ,false,2);}
void Check_TDR_92 () { Check_TDR_string (92 ,false,2);}
void Check_TDR_93 () { Check_TDR_string (93 ,false,2);}
void Check_TDR_94 () { Check_TDR_string (94 ,false,2);}
void Check_TDR_95 () { Check_TDR_string (95 ,false,2);}
void Check_TDR_96 () { Check_TDR_string (96 ,false,2);}
void Check_TDR_97 () { Check_TDR_string (97 ,false,2);}
void Check_TDR_970 () { Check_TDR_string (970 ,false,2);}
void Check_TDR_971 () { Check_TDR_string (971 ,false,2);}
void Check_TDR_98 () { Check_TDR_string (98 ,false,2);}
void Check_TDR_99 () { Check_TDR_string (99 ,false,2);}


  //
  //  End of: Debug_mux
  //---------------------------------------------------------------------------


//! Registers PDL algorithm functions in this file
//!
//! @note Names used from registration must be the same as found in SIT file
bool RegisterAlgorithms ()
{
  // ---------------- Get an handle on PDL algorithm repository
  //
  auto& repo = PDL_AlgorithmsRepository::Instance();

  // ---------------- Do register algorithm(s) with a name
  //
  repo.RegisterAlgorithm("Debug_mux", Debug_mux);
  repo.RegisterAlgorithm("Check_TDR_8",  Check_TDR_8);
  repo.RegisterAlgorithm("Check_TDR_16", Check_TDR_16);
  repo.RegisterAlgorithm("Check_TDR_24", Check_TDR_24);
  repo.RegisterAlgorithm("Check_TDR_32", Check_TDR_32);

repo.RegisterAlgorithm("Check_TDR_0", Check_TDR_0);
repo.RegisterAlgorithm("Check_TDR_1", Check_TDR_1);
repo.RegisterAlgorithm("Check_TDR_10", Check_TDR_10);
repo.RegisterAlgorithm("Check_TDR_100", Check_TDR_100);
repo.RegisterAlgorithm("Check_TDR_101", Check_TDR_101);
repo.RegisterAlgorithm("Check_TDR_102", Check_TDR_102);
repo.RegisterAlgorithm("Check_TDR_1028", Check_TDR_1028);
repo.RegisterAlgorithm("Check_TDR_103", Check_TDR_103);
repo.RegisterAlgorithm("Check_TDR_104", Check_TDR_104);
repo.RegisterAlgorithm("Check_TDR_105", Check_TDR_105);
repo.RegisterAlgorithm("Check_TDR_106", Check_TDR_106);
repo.RegisterAlgorithm("Check_TDR_107", Check_TDR_107);
repo.RegisterAlgorithm("Check_TDR_108", Check_TDR_108);
repo.RegisterAlgorithm("Check_TDR_109", Check_TDR_109);
repo.RegisterAlgorithm("Check_TDR_1097", Check_TDR_1097);
repo.RegisterAlgorithm("Check_TDR_1099", Check_TDR_1099);
repo.RegisterAlgorithm("Check_TDR_11", Check_TDR_11);
repo.RegisterAlgorithm("Check_TDR_110", Check_TDR_110);
repo.RegisterAlgorithm("Check_TDR_1109", Check_TDR_1109);
repo.RegisterAlgorithm("Check_TDR_111", Check_TDR_111);
repo.RegisterAlgorithm("Check_TDR_112", Check_TDR_112);
repo.RegisterAlgorithm("Check_TDR_1121", Check_TDR_1121);
repo.RegisterAlgorithm("Check_TDR_1122", Check_TDR_1122);
repo.RegisterAlgorithm("Check_TDR_113", Check_TDR_113);
repo.RegisterAlgorithm("Check_TDR_114", Check_TDR_114);
repo.RegisterAlgorithm("Check_TDR_115", Check_TDR_115);
repo.RegisterAlgorithm("Check_TDR_1153", Check_TDR_1153);
repo.RegisterAlgorithm("Check_TDR_116", Check_TDR_116);
repo.RegisterAlgorithm("Check_TDR_117", Check_TDR_117);
repo.RegisterAlgorithm("Check_TDR_118", Check_TDR_118);
repo.RegisterAlgorithm("Check_TDR_1181", Check_TDR_1181);
repo.RegisterAlgorithm("Check_TDR_119", Check_TDR_119);
repo.RegisterAlgorithm("Check_TDR_12", Check_TDR_12);
repo.RegisterAlgorithm("Check_TDR_120", Check_TDR_120);
repo.RegisterAlgorithm("Check_TDR_1209", Check_TDR_1209);
repo.RegisterAlgorithm("Check_TDR_121", Check_TDR_121);
repo.RegisterAlgorithm("Check_TDR_122", Check_TDR_122);
repo.RegisterAlgorithm("Check_TDR_1225", Check_TDR_1225);
repo.RegisterAlgorithm("Check_TDR_123", Check_TDR_123);
repo.RegisterAlgorithm("Check_TDR_124", Check_TDR_124);
repo.RegisterAlgorithm("Check_TDR_125", Check_TDR_125);
repo.RegisterAlgorithm("Check_TDR_126", Check_TDR_126);
repo.RegisterAlgorithm("Check_TDR_1263", Check_TDR_1263);
repo.RegisterAlgorithm("Check_TDR_1264", Check_TDR_1264);
repo.RegisterAlgorithm("Check_TDR_127", Check_TDR_127);
repo.RegisterAlgorithm("Check_TDR_129", Check_TDR_129);
repo.RegisterAlgorithm("Check_TDR_1293", Check_TDR_1293);
repo.RegisterAlgorithm("Check_TDR_1297", Check_TDR_1297);
repo.RegisterAlgorithm("Check_TDR_13", Check_TDR_13);
repo.RegisterAlgorithm("Check_TDR_130", Check_TDR_130);
repo.RegisterAlgorithm("Check_TDR_131", Check_TDR_131);
repo.RegisterAlgorithm("Check_TDR_1316", Check_TDR_1316);
repo.RegisterAlgorithm("Check_TDR_132", Check_TDR_132);
repo.RegisterAlgorithm("Check_TDR_133", Check_TDR_133);
repo.RegisterAlgorithm("Check_TDR_134", Check_TDR_134);
repo.RegisterAlgorithm("Check_TDR_136", Check_TDR_136);
repo.RegisterAlgorithm("Check_TDR_1371", Check_TDR_1371);
repo.RegisterAlgorithm("Check_TDR_1372", Check_TDR_1372);
repo.RegisterAlgorithm("Check_TDR_138", Check_TDR_138);
repo.RegisterAlgorithm("Check_TDR_139", Check_TDR_139);
repo.RegisterAlgorithm("Check_TDR_14", Check_TDR_14);
repo.RegisterAlgorithm("Check_TDR_140", Check_TDR_140);
repo.RegisterAlgorithm("Check_TDR_1400", Check_TDR_1400);
repo.RegisterAlgorithm("Check_TDR_141", Check_TDR_141);
repo.RegisterAlgorithm("Check_TDR_1412", Check_TDR_1412);
repo.RegisterAlgorithm("Check_TDR_142", Check_TDR_142);
repo.RegisterAlgorithm("Check_TDR_143", Check_TDR_143);
repo.RegisterAlgorithm("Check_TDR_144", Check_TDR_144);
repo.RegisterAlgorithm("Check_TDR_145", Check_TDR_145);
repo.RegisterAlgorithm("Check_TDR_146", Check_TDR_146);
repo.RegisterAlgorithm("Check_TDR_147", Check_TDR_147);
repo.RegisterAlgorithm("Check_TDR_1472", Check_TDR_1472);
repo.RegisterAlgorithm("Check_TDR_1473", Check_TDR_1473);
repo.RegisterAlgorithm("Check_TDR_148", Check_TDR_148);
repo.RegisterAlgorithm("Check_TDR_149", Check_TDR_149);
repo.RegisterAlgorithm("Check_TDR_15", Check_TDR_15);
repo.RegisterAlgorithm("Check_TDR_150", Check_TDR_150);
repo.RegisterAlgorithm("Check_TDR_151", Check_TDR_151);
repo.RegisterAlgorithm("Check_TDR_152", Check_TDR_152);
repo.RegisterAlgorithm("Check_TDR_154", Check_TDR_154);
repo.RegisterAlgorithm("Check_TDR_1549", Check_TDR_1549);
repo.RegisterAlgorithm("Check_TDR_155", Check_TDR_155);
repo.RegisterAlgorithm("Check_TDR_1550", Check_TDR_1550);
repo.RegisterAlgorithm("Check_TDR_157", Check_TDR_157);
repo.RegisterAlgorithm("Check_TDR_161", Check_TDR_161);
repo.RegisterAlgorithm("Check_TDR_162", Check_TDR_162);
repo.RegisterAlgorithm("Check_TDR_1626", Check_TDR_1626);
repo.RegisterAlgorithm("Check_TDR_163", Check_TDR_163);
repo.RegisterAlgorithm("Check_TDR_164", Check_TDR_164);
repo.RegisterAlgorithm("Check_TDR_1644", Check_TDR_1644);
repo.RegisterAlgorithm("Check_TDR_165", Check_TDR_165);
repo.RegisterAlgorithm("Check_TDR_166", Check_TDR_166);
repo.RegisterAlgorithm("Check_TDR_1667", Check_TDR_1667);
repo.RegisterAlgorithm("Check_TDR_1669", Check_TDR_1669);
repo.RegisterAlgorithm("Check_TDR_168", Check_TDR_168);
repo.RegisterAlgorithm("Check_TDR_1689", Check_TDR_1689);
repo.RegisterAlgorithm("Check_TDR_17", Check_TDR_17);
repo.RegisterAlgorithm("Check_TDR_170", Check_TDR_170);
repo.RegisterAlgorithm("Check_TDR_171", Check_TDR_171);
repo.RegisterAlgorithm("Check_TDR_173", Check_TDR_173);
repo.RegisterAlgorithm("Check_TDR_174", Check_TDR_174);
repo.RegisterAlgorithm("Check_TDR_175", Check_TDR_175);
repo.RegisterAlgorithm("Check_TDR_177", Check_TDR_177);
repo.RegisterAlgorithm("Check_TDR_1785", Check_TDR_1785);
repo.RegisterAlgorithm("Check_TDR_179", Check_TDR_179);
repo.RegisterAlgorithm("Check_TDR_18", Check_TDR_18);
repo.RegisterAlgorithm("Check_TDR_180", Check_TDR_180);
repo.RegisterAlgorithm("Check_TDR_181", Check_TDR_181);
repo.RegisterAlgorithm("Check_TDR_182", Check_TDR_182);
repo.RegisterAlgorithm("Check_TDR_1824", Check_TDR_1824);
repo.RegisterAlgorithm("Check_TDR_183", Check_TDR_183);
repo.RegisterAlgorithm("Check_TDR_184", Check_TDR_184);
repo.RegisterAlgorithm("Check_TDR_185", Check_TDR_185);
repo.RegisterAlgorithm("Check_TDR_186", Check_TDR_186);
repo.RegisterAlgorithm("Check_TDR_1862", Check_TDR_1862);
repo.RegisterAlgorithm("Check_TDR_187", Check_TDR_187);
repo.RegisterAlgorithm("Check_TDR_1870", Check_TDR_1870);
repo.RegisterAlgorithm("Check_TDR_188", Check_TDR_188);
repo.RegisterAlgorithm("Check_TDR_1882", Check_TDR_1882);
repo.RegisterAlgorithm("Check_TDR_1884", Check_TDR_1884);
repo.RegisterAlgorithm("Check_TDR_189", Check_TDR_189);
repo.RegisterAlgorithm("Check_TDR_19", Check_TDR_19);
repo.RegisterAlgorithm("Check_TDR_190", Check_TDR_190);
repo.RegisterAlgorithm("Check_TDR_191", Check_TDR_191);
repo.RegisterAlgorithm("Check_TDR_192", Check_TDR_192);
repo.RegisterAlgorithm("Check_TDR_198", Check_TDR_198);
repo.RegisterAlgorithm("Check_TDR_1987", Check_TDR_1987);
repo.RegisterAlgorithm("Check_TDR_199", Check_TDR_199);
repo.RegisterAlgorithm("Check_TDR_2", Check_TDR_2);
repo.RegisterAlgorithm("Check_TDR_20", Check_TDR_20);
repo.RegisterAlgorithm("Check_TDR_2012", Check_TDR_2012);
repo.RegisterAlgorithm("Check_TDR_2015", Check_TDR_2015);
repo.RegisterAlgorithm("Check_TDR_203", Check_TDR_203);
repo.RegisterAlgorithm("Check_TDR_2035", Check_TDR_2035);
repo.RegisterAlgorithm("Check_TDR_2049", Check_TDR_2049);
repo.RegisterAlgorithm("Check_TDR_206", Check_TDR_206);
repo.RegisterAlgorithm("Check_TDR_207", Check_TDR_207);
repo.RegisterAlgorithm("Check_TDR_208", Check_TDR_208);
repo.RegisterAlgorithm("Check_TDR_2099", Check_TDR_2099);
repo.RegisterAlgorithm("Check_TDR_21", Check_TDR_21);
repo.RegisterAlgorithm("Check_TDR_211", Check_TDR_211);
repo.RegisterAlgorithm("Check_TDR_212", Check_TDR_212);
repo.RegisterAlgorithm("Check_TDR_213", Check_TDR_213);
repo.RegisterAlgorithm("Check_TDR_214", Check_TDR_214);
repo.RegisterAlgorithm("Check_TDR_2141", Check_TDR_2141);
repo.RegisterAlgorithm("Check_TDR_215", Check_TDR_215);
repo.RegisterAlgorithm("Check_TDR_2155", Check_TDR_2155);
repo.RegisterAlgorithm("Check_TDR_216", Check_TDR_216);
repo.RegisterAlgorithm("Check_TDR_218", Check_TDR_218);
repo.RegisterAlgorithm("Check_TDR_2181", Check_TDR_2181);
repo.RegisterAlgorithm("Check_TDR_219", Check_TDR_219);
repo.RegisterAlgorithm("Check_TDR_2195", Check_TDR_2195);
repo.RegisterAlgorithm("Check_TDR_22", Check_TDR_22);
repo.RegisterAlgorithm("Check_TDR_2202", Check_TDR_2202);
repo.RegisterAlgorithm("Check_TDR_221", Check_TDR_221);
repo.RegisterAlgorithm("Check_TDR_2227", Check_TDR_2227);
repo.RegisterAlgorithm("Check_TDR_2241", Check_TDR_2241);
repo.RegisterAlgorithm("Check_TDR_226", Check_TDR_226);
repo.RegisterAlgorithm("Check_TDR_23", Check_TDR_23);
repo.RegisterAlgorithm("Check_TDR_2304", Check_TDR_2304);
repo.RegisterAlgorithm("Check_TDR_2403", Check_TDR_2403);
repo.RegisterAlgorithm("Check_TDR_2442", Check_TDR_2442);
repo.RegisterAlgorithm("Check_TDR_25", Check_TDR_25);
repo.RegisterAlgorithm("Check_TDR_2520", Check_TDR_2520);
repo.RegisterAlgorithm("Check_TDR_253", Check_TDR_253);
repo.RegisterAlgorithm("Check_TDR_2532", Check_TDR_2532);
repo.RegisterAlgorithm("Check_TDR_2540", Check_TDR_2540);
repo.RegisterAlgorithm("Check_TDR_2548", Check_TDR_2548);
repo.RegisterAlgorithm("Check_TDR_255", Check_TDR_255);
repo.RegisterAlgorithm("Check_TDR_2565", Check_TDR_2565);
repo.RegisterAlgorithm("Check_TDR_2576", Check_TDR_2576);
repo.RegisterAlgorithm("Check_TDR_26", Check_TDR_26);
repo.RegisterAlgorithm("Check_TDR_2626", Check_TDR_2626);
repo.RegisterAlgorithm("Check_TDR_2629", Check_TDR_2629);
repo.RegisterAlgorithm("Check_TDR_263", Check_TDR_263);
repo.RegisterAlgorithm("Check_TDR_2632", Check_TDR_2632);
repo.RegisterAlgorithm("Check_TDR_2646", Check_TDR_2646);
repo.RegisterAlgorithm("Check_TDR_2671", Check_TDR_2671);
repo.RegisterAlgorithm("Check_TDR_2679", Check_TDR_2679);
repo.RegisterAlgorithm("Check_TDR_268", Check_TDR_268);
repo.RegisterAlgorithm("Check_TDR_2680", Check_TDR_2680);
repo.RegisterAlgorithm("Check_TDR_2692", Check_TDR_2692);
repo.RegisterAlgorithm("Check_TDR_27", Check_TDR_27);
repo.RegisterAlgorithm("Check_TDR_270", Check_TDR_270);
repo.RegisterAlgorithm("Check_TDR_2701", Check_TDR_2701);
repo.RegisterAlgorithm("Check_TDR_2707", Check_TDR_2707);
repo.RegisterAlgorithm("Check_TDR_2710", Check_TDR_2710);
repo.RegisterAlgorithm("Check_TDR_2725", Check_TDR_2725);
repo.RegisterAlgorithm("Check_TDR_273", Check_TDR_273);
repo.RegisterAlgorithm("Check_TDR_276", Check_TDR_276);
repo.RegisterAlgorithm("Check_TDR_2761", Check_TDR_2761);
repo.RegisterAlgorithm("Check_TDR_278", Check_TDR_278);
repo.RegisterAlgorithm("Check_TDR_279", Check_TDR_279);
repo.RegisterAlgorithm("Check_TDR_28", Check_TDR_28);
repo.RegisterAlgorithm("Check_TDR_2817", Check_TDR_2817);
repo.RegisterAlgorithm("Check_TDR_2830", Check_TDR_2830);
repo.RegisterAlgorithm("Check_TDR_2834", Check_TDR_2834);
repo.RegisterAlgorithm("Check_TDR_285", Check_TDR_285);
repo.RegisterAlgorithm("Check_TDR_286", Check_TDR_286);
repo.RegisterAlgorithm("Check_TDR_2876", Check_TDR_2876);
repo.RegisterAlgorithm("Check_TDR_29", Check_TDR_29);
repo.RegisterAlgorithm("Check_TDR_2944", Check_TDR_2944);
repo.RegisterAlgorithm("Check_TDR_2963", Check_TDR_2963);
repo.RegisterAlgorithm("Check_TDR_2967", Check_TDR_2967);
repo.RegisterAlgorithm("Check_TDR_2977", Check_TDR_2977);
repo.RegisterAlgorithm("Check_TDR_3", Check_TDR_3);
repo.RegisterAlgorithm("Check_TDR_30", Check_TDR_30);
repo.RegisterAlgorithm("Check_TDR_301", Check_TDR_301);
repo.RegisterAlgorithm("Check_TDR_302", Check_TDR_302);
repo.RegisterAlgorithm("Check_TDR_303", Check_TDR_303);
repo.RegisterAlgorithm("Check_TDR_3072", Check_TDR_3072);
repo.RegisterAlgorithm("Check_TDR_308", Check_TDR_308);
repo.RegisterAlgorithm("Check_TDR_3091", Check_TDR_3091);
repo.RegisterAlgorithm("Check_TDR_31", Check_TDR_31);
repo.RegisterAlgorithm("Check_TDR_3152", Check_TDR_3152);
repo.RegisterAlgorithm("Check_TDR_316", Check_TDR_316);
repo.RegisterAlgorithm("Check_TDR_3180", Check_TDR_3180);
repo.RegisterAlgorithm("Check_TDR_3230", Check_TDR_3230);
repo.RegisterAlgorithm("Check_TDR_3232", Check_TDR_3232);
repo.RegisterAlgorithm("Check_TDR_3235", Check_TDR_3235);
repo.RegisterAlgorithm("Check_TDR_324", Check_TDR_324);
repo.RegisterAlgorithm("Check_TDR_327", Check_TDR_327);
repo.RegisterAlgorithm("Check_TDR_329", Check_TDR_329);
repo.RegisterAlgorithm("Check_TDR_33", Check_TDR_33);
repo.RegisterAlgorithm("Check_TDR_3307", Check_TDR_3307);
repo.RegisterAlgorithm("Check_TDR_3314", Check_TDR_3314);
repo.RegisterAlgorithm("Check_TDR_332", Check_TDR_332);
repo.RegisterAlgorithm("Check_TDR_333", Check_TDR_333);
repo.RegisterAlgorithm("Check_TDR_3337", Check_TDR_3337);
repo.RegisterAlgorithm("Check_TDR_334", Check_TDR_334);
repo.RegisterAlgorithm("Check_TDR_3358", Check_TDR_3358);
repo.RegisterAlgorithm("Check_TDR_3377", Check_TDR_3377);
repo.RegisterAlgorithm("Check_TDR_3379", Check_TDR_3379);
repo.RegisterAlgorithm("Check_TDR_3390", Check_TDR_3390);
repo.RegisterAlgorithm("Check_TDR_3394", Check_TDR_3394);
repo.RegisterAlgorithm("Check_TDR_34", Check_TDR_34);
repo.RegisterAlgorithm("Check_TDR_3404", Check_TDR_3404);
repo.RegisterAlgorithm("Check_TDR_343", Check_TDR_343);
repo.RegisterAlgorithm("Check_TDR_3454", Check_TDR_3454);
repo.RegisterAlgorithm("Check_TDR_3458", Check_TDR_3458);
repo.RegisterAlgorithm("Check_TDR_3473", Check_TDR_3473);
repo.RegisterAlgorithm("Check_TDR_348", Check_TDR_348);
repo.RegisterAlgorithm("Check_TDR_35", Check_TDR_35);
repo.RegisterAlgorithm("Check_TDR_350", Check_TDR_350);
repo.RegisterAlgorithm("Check_TDR_351", Check_TDR_351);
repo.RegisterAlgorithm("Check_TDR_3510", Check_TDR_3510);
repo.RegisterAlgorithm("Check_TDR_3519", Check_TDR_3519);
repo.RegisterAlgorithm("Check_TDR_352", Check_TDR_352);
repo.RegisterAlgorithm("Check_TDR_3543", Check_TDR_3543);
repo.RegisterAlgorithm("Check_TDR_3567", Check_TDR_3567);
repo.RegisterAlgorithm("Check_TDR_3585", Check_TDR_3585);
repo.RegisterAlgorithm("Check_TDR_36", Check_TDR_36);
repo.RegisterAlgorithm("Check_TDR_3601", Check_TDR_3601);
repo.RegisterAlgorithm("Check_TDR_361", Check_TDR_361);
repo.RegisterAlgorithm("Check_TDR_362", Check_TDR_362);
repo.RegisterAlgorithm("Check_TDR_3663", Check_TDR_3663);
repo.RegisterAlgorithm("Check_TDR_3693", Check_TDR_3693);
repo.RegisterAlgorithm("Check_TDR_37", Check_TDR_37);
repo.RegisterAlgorithm("Check_TDR_370", Check_TDR_370);
repo.RegisterAlgorithm("Check_TDR_372", Check_TDR_372);
repo.RegisterAlgorithm("Check_TDR_377", Check_TDR_377);
repo.RegisterAlgorithm("Check_TDR_3773", Check_TDR_3773);
repo.RegisterAlgorithm("Check_TDR_3780", Check_TDR_3780);
repo.RegisterAlgorithm("Check_TDR_3784", Check_TDR_3784);
repo.RegisterAlgorithm("Check_TDR_38", Check_TDR_38);
repo.RegisterAlgorithm("Check_TDR_381", Check_TDR_381);
repo.RegisterAlgorithm("Check_TDR_386", Check_TDR_386);
repo.RegisterAlgorithm("Check_TDR_3860", Check_TDR_3860);
repo.RegisterAlgorithm("Check_TDR_389", Check_TDR_389);
repo.RegisterAlgorithm("Check_TDR_39", Check_TDR_39);
repo.RegisterAlgorithm("Check_TDR_393", Check_TDR_393);
repo.RegisterAlgorithm("Check_TDR_3933", Check_TDR_3933);
repo.RegisterAlgorithm("Check_TDR_396", Check_TDR_396);
repo.RegisterAlgorithm("Check_TDR_3984", Check_TDR_3984);
repo.RegisterAlgorithm("Check_TDR_399", Check_TDR_399);
repo.RegisterAlgorithm("Check_TDR_4", Check_TDR_4);
repo.RegisterAlgorithm("Check_TDR_40", Check_TDR_40);
repo.RegisterAlgorithm("Check_TDR_400", Check_TDR_400);
repo.RegisterAlgorithm("Check_TDR_4034", Check_TDR_4034);
repo.RegisterAlgorithm("Check_TDR_406", Check_TDR_406);
repo.RegisterAlgorithm("Check_TDR_4064", Check_TDR_4064);
repo.RegisterAlgorithm("Check_TDR_408", Check_TDR_408);
repo.RegisterAlgorithm("Check_TDR_41", Check_TDR_41);
repo.RegisterAlgorithm("Check_TDR_4125", Check_TDR_4125);
repo.RegisterAlgorithm("Check_TDR_413", Check_TDR_413);
repo.RegisterAlgorithm("Check_TDR_4139", Check_TDR_4139);
repo.RegisterAlgorithm("Check_TDR_417", Check_TDR_417);
repo.RegisterAlgorithm("Check_TDR_4195", Check_TDR_4195);
repo.RegisterAlgorithm("Check_TDR_42", Check_TDR_42);
repo.RegisterAlgorithm("Check_TDR_4202", Check_TDR_4202);
repo.RegisterAlgorithm("Check_TDR_4210", Check_TDR_4210);
repo.RegisterAlgorithm("Check_TDR_4215", Check_TDR_4215);
repo.RegisterAlgorithm("Check_TDR_4259", Check_TDR_4259);
repo.RegisterAlgorithm("Check_TDR_4282", Check_TDR_4282);
repo.RegisterAlgorithm("Check_TDR_429", Check_TDR_429);
repo.RegisterAlgorithm("Check_TDR_43", Check_TDR_43);
repo.RegisterAlgorithm("Check_TDR_4339", Check_TDR_4339);
repo.RegisterAlgorithm("Check_TDR_4368", Check_TDR_4368);
repo.RegisterAlgorithm("Check_TDR_437", Check_TDR_437);
repo.RegisterAlgorithm("Check_TDR_4377", Check_TDR_4377);
repo.RegisterAlgorithm("Check_TDR_438", Check_TDR_438);
repo.RegisterAlgorithm("Check_TDR_439", Check_TDR_439);
repo.RegisterAlgorithm("Check_TDR_4396", Check_TDR_4396);
repo.RegisterAlgorithm("Check_TDR_44", Check_TDR_44);
repo.RegisterAlgorithm("Check_TDR_4439", Check_TDR_4439);
repo.RegisterAlgorithm("Check_TDR_4474", Check_TDR_4474);
repo.RegisterAlgorithm("Check_TDR_448", Check_TDR_448);
repo.RegisterAlgorithm("Check_TDR_45", Check_TDR_45);
repo.RegisterAlgorithm("Check_TDR_454", Check_TDR_454);
repo.RegisterAlgorithm("Check_TDR_46", Check_TDR_46);
repo.RegisterAlgorithm("Check_TDR_47", Check_TDR_47);
repo.RegisterAlgorithm("Check_TDR_478", Check_TDR_478);
repo.RegisterAlgorithm("Check_TDR_48", Check_TDR_48);
repo.RegisterAlgorithm("Check_TDR_480", Check_TDR_480);
repo.RegisterAlgorithm("Check_TDR_489", Check_TDR_489);
repo.RegisterAlgorithm("Check_TDR_49", Check_TDR_49);
repo.RegisterAlgorithm("Check_TDR_5", Check_TDR_5);
repo.RegisterAlgorithm("Check_TDR_50", Check_TDR_50);
repo.RegisterAlgorithm("Check_TDR_500", Check_TDR_500);
repo.RegisterAlgorithm("Check_TDR_501", Check_TDR_501);
repo.RegisterAlgorithm("Check_TDR_502", Check_TDR_502);
repo.RegisterAlgorithm("Check_TDR_506", Check_TDR_506);
repo.RegisterAlgorithm("Check_TDR_51", Check_TDR_51);
repo.RegisterAlgorithm("Check_TDR_514", Check_TDR_514);
repo.RegisterAlgorithm("Check_TDR_515", Check_TDR_515);
repo.RegisterAlgorithm("Check_TDR_519", Check_TDR_519);
repo.RegisterAlgorithm("Check_TDR_52", Check_TDR_52);
repo.RegisterAlgorithm("Check_TDR_520", Check_TDR_520);
repo.RegisterAlgorithm("Check_TDR_521", Check_TDR_521);
repo.RegisterAlgorithm("Check_TDR_53", Check_TDR_53);
repo.RegisterAlgorithm("Check_TDR_530", Check_TDR_530);
repo.RegisterAlgorithm("Check_TDR_538", Check_TDR_538);
repo.RegisterAlgorithm("Check_TDR_54", Check_TDR_54);
repo.RegisterAlgorithm("Check_TDR_540", Check_TDR_540);
repo.RegisterAlgorithm("Check_TDR_55", Check_TDR_55);
repo.RegisterAlgorithm("Check_TDR_556", Check_TDR_556);
repo.RegisterAlgorithm("Check_TDR_56", Check_TDR_56);
repo.RegisterAlgorithm("Check_TDR_560", Check_TDR_560);
repo.RegisterAlgorithm("Check_TDR_566", Check_TDR_566);
repo.RegisterAlgorithm("Check_TDR_567", Check_TDR_567);
repo.RegisterAlgorithm("Check_TDR_569", Check_TDR_569);
repo.RegisterAlgorithm("Check_TDR_57", Check_TDR_57);
repo.RegisterAlgorithm("Check_TDR_570", Check_TDR_570);
repo.RegisterAlgorithm("Check_TDR_58", Check_TDR_58);
repo.RegisterAlgorithm("Check_TDR_59", Check_TDR_59);
repo.RegisterAlgorithm("Check_TDR_6", Check_TDR_6);
repo.RegisterAlgorithm("Check_TDR_60", Check_TDR_60);
repo.RegisterAlgorithm("Check_TDR_61", Check_TDR_61);
repo.RegisterAlgorithm("Check_TDR_617", Check_TDR_617);
repo.RegisterAlgorithm("Check_TDR_62", Check_TDR_62);
repo.RegisterAlgorithm("Check_TDR_621", Check_TDR_621);
repo.RegisterAlgorithm("Check_TDR_63", Check_TDR_63);
repo.RegisterAlgorithm("Check_TDR_631", Check_TDR_631);
repo.RegisterAlgorithm("Check_TDR_635", Check_TDR_635);
repo.RegisterAlgorithm("Check_TDR_64", Check_TDR_64);
repo.RegisterAlgorithm("Check_TDR_645", Check_TDR_645);
repo.RegisterAlgorithm("Check_TDR_65", Check_TDR_65);
repo.RegisterAlgorithm("Check_TDR_655", Check_TDR_655);
repo.RegisterAlgorithm("Check_TDR_66", Check_TDR_66);
repo.RegisterAlgorithm("Check_TDR_67", Check_TDR_67);
repo.RegisterAlgorithm("Check_TDR_671", Check_TDR_671);
repo.RegisterAlgorithm("Check_TDR_68", Check_TDR_68);
repo.RegisterAlgorithm("Check_TDR_69", Check_TDR_69);
repo.RegisterAlgorithm("Check_TDR_690", Check_TDR_690);
repo.RegisterAlgorithm("Check_TDR_697", Check_TDR_697);
repo.RegisterAlgorithm("Check_TDR_7", Check_TDR_7);
repo.RegisterAlgorithm("Check_TDR_70", Check_TDR_70);
repo.RegisterAlgorithm("Check_TDR_701", Check_TDR_701);
repo.RegisterAlgorithm("Check_TDR_71", Check_TDR_71);
repo.RegisterAlgorithm("Check_TDR_72", Check_TDR_72);
repo.RegisterAlgorithm("Check_TDR_728", Check_TDR_728);
repo.RegisterAlgorithm("Check_TDR_729", Check_TDR_729);
repo.RegisterAlgorithm("Check_TDR_73", Check_TDR_73);
repo.RegisterAlgorithm("Check_TDR_74", Check_TDR_74);
repo.RegisterAlgorithm("Check_TDR_740", Check_TDR_740);
repo.RegisterAlgorithm("Check_TDR_75", Check_TDR_75);
repo.RegisterAlgorithm("Check_TDR_751", Check_TDR_751);
repo.RegisterAlgorithm("Check_TDR_753", Check_TDR_753);
repo.RegisterAlgorithm("Check_TDR_754", Check_TDR_754);
repo.RegisterAlgorithm("Check_TDR_76", Check_TDR_76);
repo.RegisterAlgorithm("Check_TDR_77", Check_TDR_77);
repo.RegisterAlgorithm("Check_TDR_777", Check_TDR_777);
repo.RegisterAlgorithm("Check_TDR_78", Check_TDR_78);
repo.RegisterAlgorithm("Check_TDR_79", Check_TDR_79);
repo.RegisterAlgorithm("Check_TDR_80", Check_TDR_80);
repo.RegisterAlgorithm("Check_TDR_800", Check_TDR_800);
repo.RegisterAlgorithm("Check_TDR_806", Check_TDR_806);
repo.RegisterAlgorithm("Check_TDR_81", Check_TDR_81);
repo.RegisterAlgorithm("Check_TDR_815", Check_TDR_815);
repo.RegisterAlgorithm("Check_TDR_82", Check_TDR_82);
repo.RegisterAlgorithm("Check_TDR_83", Check_TDR_83);
repo.RegisterAlgorithm("Check_TDR_84", Check_TDR_84);
repo.RegisterAlgorithm("Check_TDR_85", Check_TDR_85);
repo.RegisterAlgorithm("Check_TDR_850", Check_TDR_850);
repo.RegisterAlgorithm("Check_TDR_86", Check_TDR_86);
repo.RegisterAlgorithm("Check_TDR_87", Check_TDR_87);
repo.RegisterAlgorithm("Check_TDR_88", Check_TDR_88);
repo.RegisterAlgorithm("Check_TDR_884", Check_TDR_884);
repo.RegisterAlgorithm("Check_TDR_89", Check_TDR_89);
repo.RegisterAlgorithm("Check_TDR_891", Check_TDR_891);
repo.RegisterAlgorithm("Check_TDR_896", Check_TDR_896);
repo.RegisterAlgorithm("Check_TDR_897", Check_TDR_897);
repo.RegisterAlgorithm("Check_TDR_9", Check_TDR_9);
repo.RegisterAlgorithm("Check_TDR_90", Check_TDR_90);
repo.RegisterAlgorithm("Check_TDR_904", Check_TDR_904);
repo.RegisterAlgorithm("Check_TDR_91", Check_TDR_91);
repo.RegisterAlgorithm("Check_TDR_910", Check_TDR_910);
repo.RegisterAlgorithm("Check_TDR_911", Check_TDR_911);
repo.RegisterAlgorithm("Check_TDR_92", Check_TDR_92);
repo.RegisterAlgorithm("Check_TDR_93", Check_TDR_93);
repo.RegisterAlgorithm("Check_TDR_94", Check_TDR_94);
repo.RegisterAlgorithm("Check_TDR_95", Check_TDR_95);
repo.RegisterAlgorithm("Check_TDR_96", Check_TDR_96);
repo.RegisterAlgorithm("Check_TDR_97", Check_TDR_97);
repo.RegisterAlgorithm("Check_TDR_970", Check_TDR_970);
repo.RegisterAlgorithm("Check_TDR_971", Check_TDR_971);
repo.RegisterAlgorithm("Check_TDR_98", Check_TDR_98);
repo.RegisterAlgorithm("Check_TDR_99", Check_TDR_99);

  return true;
}
//
//  End of: RegisterAlgorithms
//---------------------------------------------------------------------------


//! Make PDL algorithm functions in this file to be registered
//!
//! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
//!
bool registrated = RegisterAlgorithms();

} // End of unnamed namespace


//===========================================================================
// End of Debug_mux.cpp
//===========================================================================
