//===========================================================================
//                           Advantest_TrivialPoc.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Advantest_TrivialPoc.cpp
//!
//! Implements RDI test method for a trivial proof of concept to run Mast on
//! an ADVANTEST V93000 ATE
//!
//===========================================================================

#include "Advantest_SmartRDI_Protocol.hpp"
#include "XmlRpc_Protocol_Server.hpp"

#include "testmethod.hpp"

#include <stdexcept>
#include <iostream>
#include <string>

using mast::Advantest_SmartRDI_Protocol;
using mast::XmlRpc_Protocol_Server;
using std::string;

//! Test method class
//!
//! @note Each test suite using this test method, one instance of this class is created
//!
class Advantest_TrivialPoc: public testmethod::TestMethod
{
  // ---------------- Public  Methods
  //
  public:
  //! Stops the server
  //!
  //! @note Destructor cannot only be called when server has been started from another thread!
  //!
  virtual ~Advantest_TrivialPoc()
  {
    m_server.Stop();
  }

  //! Initializes the server BUT does not start it
  //!
  Advantest_TrivialPoc()
    : m_protocol ()
    , m_server   (8080, &m_protocol)
  {
    m_server.LogInfos (true);
    m_server.LogErrors(true);
    m_server.Start(); // Do not return - use Ctrl-C 'or kill -9 ' to exit
  }

  // ---------------- Protected Methods
  //
  protected:

  //! Initializes the parameter interface to the testflow.
  //!
  //! @note It is called just once after a testsuite is created.
  //! @note Test Method API should not be used in this method!
  //!
  virtual void initialize()
  {
  }


  //! Runs the test associated with this test method class
  //!
  virtual void run()
  {
    try
    {
      m_server.Start(); // Do not return - use Ctrl-C 'or kill -9 ' to exit
    }
    catch(std::exception& exc)  // Catch C++ standard exceptions
    {
      std::cerr << "Mast Server for Advantest got std::exception: " << exc.what();
    }
    catch (...)
    {
      std::cerr << "Mast Server for Advantest got unknown type exeption";
    }
  }
  //
  //  End of run
  //---------------------------------------------------------------------------


  //! ???
  //!
  //! @param parameterIdentifier  Identify changed parameter
  //!
  //! @note It is called once the specified parameter's value is changed.
  //! @note Test Method API should not be used in this method!
  //!
  virtual void postParameterChange(const string& parameterIdentifier)
  {
  }

  //! Returns short description of the test
  //!
  //! @note It is called once the Select Test Method Dialog is opened
  //!
  virtual string getComment() const
  {
    return "Trivial (very simple) proof of concept to run Mast on ADVANTEST ATE";
  }

  // ---------------- Private  Fields
  //
  private:
  Advantest_SmartRDI_Protocol m_protocol;
  XmlRpc_Protocol_Server      m_server;
};
//
//  End of Advantest_TrivialPoc class declaration
//---------------------------------------------------------------------------

REGISTER_TESTMETHOD("Advantest_TrivialPoc", Advantest_TrivialPoc);
