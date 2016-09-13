//===========================================================================
//                           Options.hpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Options.hpp
//!
//! Declares Options class
//!
//===========================================================================


#ifndef OPTIONS_H__C5053EC6_17E5_428B_6C9D_9E0AD8AE9F4__INCLUDED_
  #define OPTIONS_H__C5053EC6_17E5_428B_6C9D_9E0AD8AE9F4__INCLUDED_

#include <string>
#include <experimental/string_view>

namespace mast
{
//! Defines options for test application
//!
//!
class Options
{
  // ---------------- Public Enum
  //
  public:

  //! Defines which kind of AccessInterfaceProtocol to use
  //!
  enum class Protocol
  {
    NotSpecified,
    LoopBack,       //!< Data from SUT are exactly the same as data to SUT
    SVF_Emulation,  //!< Data to SUT are written to file and data from SUT readen from file (SVF formatted )
    SVF_Simulation, //!< Data to SUT are written to file that is read from simulator and data from SUT readen from file that is written by simulator (SVF formatted)
    I2C_Emulation,  //!< Data to SUT are written to file and data from SUT readen from file (Textual I2C commands)
    OpenOCD,        //!< Protocol is based upon OpenOCD
		SPI,
    Generic,        //!< Table based protocol is used (need specific options)
  };

  enum class Testcase
  {
    NotSpecified,
    SIT_File,
    Wrapper_1500,
  };

  // ---------------- Public  Methods
  //
  public:
  virtual ~Options() = default;
  Options()  = default;

  using string_view = std::experimental::string_view;

  std::string ToDebugString (string_view header = "", string_view linePrefix = "") const;

  //! Parses main function arguments
  //!
  static Options ParseArguments (int argc, char* argv []);

  // ---------------- Protected Methods
  //
  protected:

  static std::string ReplaceFilePathExtension (std::string filePath, const std::string& replacement);
  static Protocol    ParseProtocol(const std::string& protocolOption);
  static Testcase    ParseTestcase(const std::string& testcaseOption);

  // ---------------- Private  Methods
  //
  private:

  // ---------------- Public Fields
  //
  public:
  bool        printGraph    = false;
  std::string graphFilePath = "Testcase.gml"; //!< This is default file name when not requested explicitly by user
  uint32_t    loopCount     = 5u;
  Testcase    testcase      = Testcase::NotSpecified;
  std::string testcaseOptions;
  Protocol    protocol      = Protocol::NotSpecified;
  std::string protocolOptions;
};
//
//  End of Options class declaration
//---------------------------------------------------------------------------
} // End of namespace mast




#endif  // not defined OPTIONS_H__C5053EC6_17E5_428B_6C9D_9E0AD8AE9F4__INCLUDED_

//===========================================================================
// End of Options.hpp
//===========================================================================
