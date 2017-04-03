//===========================================================================
//                           Example_XmlRpc.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Example_XmlRpc.cpp
//!
//! Implements specific initialization for example based upon Xml-Rpc client-server
//!
//===========================================================================

#include "AppFunctionAndNodePath_CPP.hpp"
#include "AppFunctionAndName_CPP.hpp"
#include "PDL_Adapter_CPP.hpp"
#include "XmlRpc_Protocol_Client.hpp"
#include "RemoteProtocolFactory.hpp"
#include "Examples_Utils.hpp"
#include <iostream>


using std::string;
using std::experimental::string_view;
using std::vector;
using std::unique_ptr;
using std::make_unique;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;
using namespace mast;

void                           Init                      (int argc, char* argv []);
string                         GetSitModelFilePath       (int argc, char* argv []);
vector<AppFunctionAndName>     GetAlgorithmsNames        (int argc, char* argv []);
vector<AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []);


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

    std::cout << "Running Increment for "<< loopCount << " times \n";
    while (loopCount--)
    {
      iWrite(registerPath, initialValue);
      iApply();

      ++initialValue;
    }
    std::cout << "Increment finished\n";
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

    std::cout << "Running Decrement for "<< loopCount << " times \n";

    while (loopCount--)
    {
      iWrite(registerPath, value);
      iApply();

      --value;
    }
    std::cout << "Decrement finished\n";
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

    std::cout << "Running Copy from " << fromPath<<" to "<< toPath<<" for "<< loopCount << " times \n";

    while (loopCount--)
    {
      auto gotValue = iGetRefresh<uint32_t>(fromPath);

      iWrite(toPath, gotValue);
      iApply();
    }
       std::cout << "Copy finished \n";
 
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

    std::cout << "Running Shift from " << fromPath<<" to "<< toPath<<" for "<< loopCount << " times \n";

    while (loopCount--)
    {
      auto gotValue = iGetRefresh<uint32_t>(fromPath);
      gotValue <<= 1u;
      iWrite(toPath, gotValue);
      iApply();
    }
    std::cout << "End Shift\n";
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
  auto sitFile = GetFromArgs(argc, argv, "--sit");

  if (sitFile.empty())
  {
    sitFile = "Example_XmlRpc.sit";
  }

  return sitFile;
}
//
//  End of: GetSitModelFilePath
//---------------------------------------------------------------------------



//! Doe special init for specific example (can do nothing)
//!
void Init (int argc, char* argv[])
{
  auto url = GetFromArgs(argc, argv, "--url");

  auto& factory = RemoteProtocolFactory::Instance();

  auto creator = [url](const string& parameters)
  {
    if (!url.empty())
    {
      return make_unique<XmlRpc_Protocol_Client>(url);
    }

    return make_unique<XmlRpc_Protocol_Client>(parameters);
  };

  factory.RegisterCreator("XmlRpc", creator);
}



//===========================================================================
// End of Example_XmlRpc.cpp
//===========================================================================
