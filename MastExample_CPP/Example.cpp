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


using std::vector;
using std::string;

using namespace std::string_literals;
using namespace mast;

std::string                               GetSitModelFilePath       (int argc, char* argv []);
std::vector<mast::AppFunctionAndName>     GetAlgorithmsNames        (int argc, char* argv []);
std::vector<mast::AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []);


namespace
{
  //! This algorithm increment "reg_0"
  //!
  void Algo_Increment ()
  {
    iPrefix("SWIR.WIR");

    auto     registerPath = "reg_0";
    auto     loopCount    = 20u;
    uint16_t initialValue = 1u;

    while (loopCount--)
    {
      iWrite(registerPath, initialValue);
      iApply();

      ++initialValue;
    }
  }
  //
  //  End of: Algo_Increment
  //---------------------------------------------------------------------------


  //! This algorithm decrement "reg_1"
  //!
  void Algo_Decrement ()
  {
    iPrefix("SWIR.WIR");

    auto     registerPath = "reg_1";
    auto     loopCount    = 60u;
    uint16_t value        = 0xCAFEu;

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



  //! This algorithm copy value from "reg_0"  to "reg_2"
  //!
  void Algo_Copy ()
  {
    iPrefix("SWIR.WIR");

    auto fromPath  = "reg_0";
    auto toPath    = "reg_2";
    auto loopCount = 20u;

    while (loopCount--)
    {
      auto gotValue = iGetRefresh<uint32_t>(fromPath);

      iWrite(toPath, gotValue);
      iApply();
    }
  }
  //
  //  End of: Algo_Copy
  //---------------------------------------------------------------------------


  //! This algorithm Shifting value from "reg_2"  to "reg_3"
  //!
  void Algo_Shift ()
  {
    iPrefix("SWIR.WIR");

    auto fromPath  = "reg_2";
    auto toPath    = "reg_3";
    auto loopCount = 20u;

    while (loopCount--)
    {
      auto gotValue = iGetRefresh<uint32_t>(fromPath);
      gotValue <<= 1u;
      iWrite(toPath, gotValue);
      iApply();
    }
  }
  //
  //  End of: Algo_Shift
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
    {Algo_Increment, "W_1500", "Inc"},
    {Algo_Decrement, "W_1500", "Dec"},
    {Algo_Copy,      "W_1500", "Copy"},
    {Algo_Shift,     "W_1500", "Shift"},
  };

  return associations;
}
//
//  End of GetAlgorithmsAndNodePaths
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
    {Algo_Copy,      "Copy"},
    {Algo_Shift,     "Shift"},
  };

  return names;
}
//
//  End of: GetAlgorithmsNames
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
