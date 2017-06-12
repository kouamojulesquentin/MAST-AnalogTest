#include "Advantest_SmartRDI_Protocol.hpp"
#include "Remote_Loopback_Protocol.hpp"
#include "XmlRpc_Protocol_Server.hpp"

#include "testmethod.hpp"

#include <stdexcept>
#include <iostream>
#include <string>

using mast::Advantest_SmartRDI_Protocol;
using mast::Remote_Loopback_Protocol;
using mast::XmlRpc_Protocol_Server;
using std::string;

//for test method API interfaces
#include "mapi.hpp"
using namespace std;

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class MAST_Advantest_PoC: public testmethod::TestMethod {
public:
	//! Stops the server
	  //!
	  //! @note Destructor cannot only be called when server has been started from another thread!
	  //!
	  virtual ~MAST_Advantest_PoC()
	  {
	    m_server.Stop();
	  }

	  //! Initializes the server BUT does not start it
	  //!
	  MAST_Advantest_PoC()
	    : m_protocol ()
	    , m_server   (8080, &m_protocol)
	  {
		try{
		m_server.LogInfos (true);
	    m_server.LogErrors(true);
	    std::cout << "[MAST_Adv_PoC] Constructor: Server created\n";
		}
		catch(std::exception& exc)  // Catch C++ standard exceptions
		  {
		    std::cerr << "XmlRpc Loopback Server got std::exception: " << exc.what();
		    return;
		  }
		  catch (...)
		  {
		    std::cerr << "XmlRpc Loopback Server got unknown type exeption";
		    return;
		  }

	//    m_server.Start(); // Do not return - use Ctrl-C 'or kill -9 ' to exit
	  }

protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize()
  {
    //Add your initialization code here
    //Note: Test Method API should not be used in this method!
	  std::cout << "[MAST_Adv_PoC] initialize()\n";
	  std::cout << "Int has size " << sizeof(int) << "\n";
	  	  //
  }

  /**
   *This test is invoked per site.
   */
  virtual void run()
  {
    //Add your test code here.
	   std::cout << "[MAST_Adv_PoC] Launching server\n";
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
    return;
  }

  /**
   *This function will be invoked once the specified parameter's value is changed.
   *@param parameterIdentifier
   */
  virtual void postParameterChange(const string& parameterIdentifier)
  {
    //Add your code
    //Note: Test Method API should not be used in this method!
    return;
  }

  /**
   *This function will be invoked once the Select Test Method Dialog is opened.
   */
  virtual const string getComment() const 
  {
    string comment = " Proof of Concept of MAST implementation for V93k";
    return comment;
  }
  // ---------------- Private  Fields
  //
  private:
  Advantest_SmartRDI_Protocol m_protocol;
//  Remote_Loopback_Protocol m_protocol;
  XmlRpc_Protocol_Server     m_server;
};
REGISTER_TESTMETHOD("MAST_Advantest_PoC", MAST_Advantest_PoC);
