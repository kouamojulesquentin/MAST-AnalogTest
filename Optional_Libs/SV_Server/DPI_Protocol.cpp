//===========================================================================
//                           DPI_Protocol.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file DPI_Protocol.cpp
//!
//! Implements class DPI_Protocol
//!
//===========================================================================

#include "DPI_Protocol.hpp"
#include "DPI_interface.hpp"

#include <iostream>
#include <algorithm>    // std::remove_if

using std::string;
using std::vector;
using std::pair;

using std::make_pair;
using namespace mast;

bool is_space(unsigned char const c ){ return std::isspace(c);};

//! Destructor does nothing
DPI_Protocol::~DPI_Protocol ()
{
}


//! Constructor does nothing
DPI_Protocol::DPI_Protocol ()
{
  SupportedCallbacks.insert ( std::pair<std::string,int>("TRST",TRST) );
  SupportedCallbacks.insert ( std::pair<std::string,int>("SIR",SIR) );
  SupportedCallbacks.insert ( std::pair<std::string,int>("SDR",SDR) );  
  
  std::cout << "DPI_Protocol::DPI_Protocol: Registered "<<SupportedCallbacks.size()<<" Callbacks:\n";
  std::map<std::string,int>::iterator it;
  it = SupportedCallbacks.begin();
  while (it!=SupportedCallbacks.end())
    {
    std::cout <<"     " << it->first << " -> " << it->second <<"\n";  
    it++;
    }
 }


int DPI_Protocol::findCallbackId(std::string commandname)
 {
  std::map<std::string,int>::iterator it;
  it = SupportedCallbacks.find(commandname);
  std::cout << "DPI_Protocol::findCallbackId: found "<<it->first<< "=> "<<it->second << "\n";
//  if (it==SupportedCallbacks.end()) return -1;
//  else
    return it->second;
 }

//! Returns toSutScanVector untouched
//!
//! @param commandName  Command name (SIR, SDR, RST...)
//! @param bitsCount    Number of valid bits in scan vector
//! @param scanVector   Binary data to send to SUT (default is right aligned)
//!
//! @return data scanned out from SUT
 Remote_Protocol::DoCallbackReturn_t
  DPI_Protocol::DoCallback(const std::string& commandName, uint32_t bitsCount, const std::vector<uint8_t>& toSutScanVector)
/*pair<uint32_t, vector<uint8_t> >
DPI_Protocol::DoAction (const string&           commandName ,
                                    uint32_t               bitsCount,
                                    const vector<uint8_t>& toSutScanVector)*/
{
  uint8_t bytesCount = toSutScanVector.size();

  vector<uint8_t> fromSutScanVector;
  fromSutScanVector.resize(toSutScanVector.size());
  
  string local_commandName = commandName;
  local_commandName.erase(std::remove_if(local_commandName.begin(), local_commandName.end(), is_space), local_commandName.end());

  set_DPI_bitsCount(bitsCount);
  set_DPI_data_to_SUT( const_cast<unsigned char*>(&toSutScanVector[0]));
  set_DPI_data_from_SUT( const_cast<unsigned char*>(&fromSutScanVector[0]));


  std::cout << "DPI_Protocol::DoCallback : Registered "<<SupportedCallbacks.size()<<" Callbacks:\n";
  std::cout << "DPI_Protocol::DoCallback : Looking for  *"<<local_commandName<<"* \n";


  int Callback_to_execute = findCallbackId(local_commandName);
  std::cout << "DPI_Protocol::DoCallback : findCallbackId result is  *"<<Callback_to_execute<<"* \n";

  if (Callback_to_execute==SIR)  do_SIR(bitsCount);
  if (Callback_to_execute==SDR)  do_SDR();
  if (Callback_to_execute==TRST) return make_pair(bitsCount, toSutScanVector);
  if (Callback_to_execute==-1)   throw std::runtime_error("DPI_Protocol::DoCallback: Unknown callback "+local_commandName);

/*
 For some unknown reason, this switch does not work
 switch(findCallbackId(local_commandName))
  {
   TRST:return make_pair(bitsCount, toSutScanVector);
       break;        
   SIR : 
       do_SIR();
       break;
   SDR :
       do_SDR(); 
       break;
   default : 

       throw std::runtime_error("DPI_Protocol::DoCallback: Unknown callback "+local_commandName);
       
  }
  */
  
  return make_pair(bitsCount, toSutScanVector);  //False loopback
  //return make_pair(bitsCount, fromSutScanVector);
}


//! Does nothing
void DPI_Protocol::DoReset (bool /* doSynchronousReset */)
{
}



//===========================================================================
// End of DPI_Protocol.cpp
//===========================================================================
