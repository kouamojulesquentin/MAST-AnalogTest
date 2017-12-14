//===========================================================================
//                           ICL_Parser_SmokeTest.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ICL_Parser_SmokeTest.cpp
//!
//! Implements main function to be used as a smoke test for ICL parser
//!
//===========================================================================

#include "SystemModel.hpp"
#include "ICL_Reader.hpp"
#include "Session.hpp"
#include "Utility.hpp"
#include "ParserException.hpp"
#include "g3log/g3log.hpp"

#include <iostream>


//! Runs ICL on a file, checking there is no errors
//!
//! @return 0 when successful, -1 in case of error successful
int main (int argc, char* argv[])
{
  mast::Session session(true);
  try
  {
    CHECK_PARAMETER_RANGE(argc, 2, 3, "Expect at least one parameter. Syntax: ICL_SmokeTest icl_file_path [error_message_excerpt])");

    std::string iclFile(argv[1]);
    CHECK_PARAMETER_NOT_EMPTY(iclFile, "ICL file path must be non empty");

    LOG(INFO) << "iclFile: " << iclFile;

    auto systemModel = std::make_shared<mast::SystemModel>();

    ICL::ICL_Reader reader(systemModel);
    reader.ParseOnlyCheckGrammar(true);    // Do not try to check AST nor create a model for it (ICL files are not always complete)

    reader.Parse(iclFile);
    return 0;
  }
  catch(Parsers::ParserException& exc)
  {
    std::string errorMessage(exc.what());

    bool haveMessageExerpt = argc == 3;
    if (haveMessageExerpt)
    {
      std::string errorMessageExcerpt(argv[2]);

      auto errorMessageIsExpected = errorMessage.find(errorMessageExcerpt) != std::string::npos;

      if (errorMessageIsExpected)
      {
        return 0;
      }
      std::cerr << "\n"
                << "Error message: \"" << errorMessage << "\"\n"
                << "=============\n"
                << "Does not contains expected excerpt: \"" << errorMessageExcerpt << "\"\n"
                << "================================== \n";

    }
    else
    {
      std::cerr << "\n"
                << "Error message: \"" << errorMessage << "\"\n"
                << "=============\n";
    }
  }
  catch(std::exception& exc)  // Catch C++ standard exceptions
  {
    std::cerr << exc.what();
  }
  catch (...)
  {
    std::cerr << "Caught unknown exception";
  }

  return -1;
}
//
//  End of: main
//---------------------------------------------------------------------------

//===========================================================================
// End of ICL_Parser_SmokeTest.cpp
//===========================================================================



