//===========================================================================
//                           Example.cpp
//===========================================================================
//! @file Example.cpp
//!
//! Example of PDL like algorithm implementation and association to node
//!
//===========================================================================

#include "CPP_API.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

using std::vector;
using std::string;

using namespace std::string_literals;
using namespace mast;

std::string                               GetSitModelFilePath       (int argc, char* argv []);
std::vector<mast::AppFunctionAndName>     GetAlgorithmsNames        (int argc, char* argv []);
std::vector<mast::AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []);


namespace
{
  //! This algorithm increment "reg"
  //!
  void Algo_Increment ()
  {

    auto     registerPath = "regHI";
    auto     loopCount    = 5u;
    auto     i    = 0u;
    uint16_t initialValue = 1u;
    uint16_t curValue;

   curValue=iGetRefresh<uint16_t>(registerPath);
    std::cout << "nINCREMENTRunning " << loopCount << " iWrites on register " <<registerPath << "\n";
    while (i++<loopCount)
    {
      iWrite(registerPath, initialValue);
      iApply();
      curValue=iGet<uint16_t>(registerPath);
     std::cout << "\nINCREMENT Cycle "<< i << ": Wrote " << initialValue ;
     std::cout << "\nINCREMENT       "<< i << ": Read " <<std::hex<< curValue <<"\n" ;

      ++initialValue;
    }
     std::cout << "\n" ;
  }
  //
  //  End of: Algo_Increment
  //---------------------------------------------------------------------------


  //! This algorithm decrement "regLO"
  //!
  void Algo_Decrement ()
  {
    auto     registerPath = "regLO";
    auto     loopCount    = 5u;
    uint16_t initialValue = 60u;
    uint16_t curValue;
    auto     i    = 0u;

    curValue=iGetRefresh<uint16_t>(registerPath);
    std::cout << "nDECREMENT Running " << loopCount << " iWrites on register " <<registerPath << "\n";
    while (i++<loopCount)
    {
      iWrite(registerPath, initialValue);
      curValue=iGet<uint16_t>(registerPath);
      std::cout << "\nDECREMENT Cycle "<< i << ": Wrote " << initialValue ;
      std::cout << "\nDECREMENT       "<< i << ": Read " << curValue ;
      iApply();

      --initialValue;
    }
  }
  //
  //  End of: Algo_Increment
  //---------------------------------------------------------------------------
} // End of unnamed namespace



//! Returns descriptions for applications to run on Example board
//!
//! @note Adapt this function as necessary
//!       It must nevertheless be compliant with SystemModel (possibly with 'SIT' file)
vector<AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int /* argc */, char* /* argv */[])
{
  vector<AppFunctionAndNodePath> associations
  {
    {Algo_Increment, "", "Incr"},
    {Algo_Decrement, "", "Decr"},
  };

  return associations;
}
//
//  End of: test::Example_CreateApplicationsDescriptor
//---------------------------------------------------------------------------


//! Returns names of provided applications.
//!
//! @param argc   Number of arguments provided to main function
//! @param argv   Arguments provided to main function
//!
//! @note This is intended to be match names associated with node in SIT files
//!
vector<mast::AppFunctionAndName> GetAlgorithmsNames (int /* argc */, char* /* argv */[])
{
  vector<AppFunctionAndName> names
  {
    {Algo_Increment, "Incr"},
    {Algo_Decrement, "Decr"},
  };

  return names;
}
//
//  End of: GetSitModelFilePath
//---------------------------------------------------------------------------


//! Returns SIT formatted file path to create a SystemModel from.
//!
string GetSitModelFilePath (int argc, char* argv[])
{
  if (argc > 1)
  {
    return string(argv[1]);
  }

  return "Example.sit";
}
//
//  End of: GetSitModelFilePath
//---------------------------------------------------------------------------


//===========================================================================
// End of Example.cpp
//===========================================================================
