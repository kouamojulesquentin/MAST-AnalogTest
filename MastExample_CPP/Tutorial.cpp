//===========================================================================
//                           Example.cpp
//===========================================================================
//! @file Example.cpp
//!
//! Example of PDL like algorithm implementation and association to node
//!
//===========================================================================

#include "AppFunctionAndNodePath_CPP.hpp"
#include "AppFunctionAndName_CPP.hpp"
#include "PDL_Adapter_CPP.hpp"
//+#include "g3log/g3log.hpp"

#include <string>
#include <vector>
#include <iostream>


using std::vector;
using std::string;

using namespace std::string_literals;
using namespace mast;

std::string                               GetSitModelFilePath       (int argc, char* argv []);
std::vector<mast::AppFunctionAndName>     GetAlgorithmsNames        (int argc, char* argv []);
std::vector<mast::AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []);


namespace
{
  //! This algorithm increment "reg" 60 times
  //!
  void Algo_Increment ()
  {

    auto     registerPath = "reg";
    auto     loopCount    = 5u;
    auto     i    = 0u;
    uint16_t initialValue = 1u;
    uint16_t curValue;

    std::cout << "Running " << loopCount << " iWrites on register " <<registerPath << "\n";
    while (i++<loopCount)
    {
      iWrite(registerPath, initialValue);
 //     iGet(registerPath,curValue);
      curValue=iGet<uint16_t>(registerPath);
     std::cout << "\n Cycle "<< i << ": Wrote " << initialValue ;
     std::cout << "\n       "<< i << ": Read " << curValue ;
      iApply();

      ++initialValue;
    }
     std::cout << "\n" ;
  }
  //
  //  End of: Algo_Increment
  //---------------------------------------------------------------------------


  //! This algorithm decrement "reg_1" 60 times
  //!
  void Algo_Decrement ()
  {
    auto     registerPath = "reg";
    auto     loopCount    = 20u;
    uint16_t value = 60u;

    while (loopCount--)
    {
      iWrite(registerPath, value);
      iApply();

      --value;
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
    {Algo_Increment, "", "Inc"},
    {Algo_Decrement, "", "Dec"},
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
