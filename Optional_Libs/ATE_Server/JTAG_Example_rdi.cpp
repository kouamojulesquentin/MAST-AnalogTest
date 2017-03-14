#include "testmethod.hpp"

//for test method API interfaces
#include "mapi.hpp"
#include "rdi.hpp"
#include "jtag_definitions.h"
using namespace std;

/**
 * Test method class.
 *
 * For each testsuite using this test method, one object of this
 * class is created.
 */
class JTAG_Example_rdi: public testmethod::TestMethod
{
protected:
  /**
   *Initialize the parameter interface to the testflow.
   *This method is called just once after a testsuite is created.
   */
  virtual void initialize()
  {
    //Add your initialization code here
    //Note: Test Method API should not be used in this method!
  }

  /**
   *This test is invoked per site.
   */
  virtual void run()
  {
    RDI_INIT();

    ON_FIRST_INVOCATION_BEGIN();
        CONNECT(); // make sure device is connected if debugging a single test suite
        //rdi.smartVec().readMode(TA::EMAP);
        RDI_BEGIN();
          int ir_data_in  = 0x9;
          int ir_data_out = 0x3F;

//          rdi.port("pNonJtag").func().label("pattern_pNonJtag").execute();

          // write into IR, prepare to read IDCODE data register
          jtag_access_ir(ir_data_in, "ID1");

          // write into IR, prepare to compare the IDCODE data register to the expected value
          jtag_access_ir_expData(0x9, "ID1exp", ir_data_out);

          // read data register 0x0b0 and read register value
          jtag_access_dr(0x0b0, "ID2");

          // read data register 0x0 and read register value
          jtag_access_dr(0x0, "ID3");

          // write into IR and into DR, capture data shifted out of DR
          jtag_access(ir_data_in, ir_data_out, "ID4");

          jtag_synch_reset(32);
        RDI_END();

    ON_FIRST_INVOCATION_END();


    ARRAY_LL data1    = rdi.id("ID1").getReadData();
    ARRAY_LL data2    = rdi.id("ID2").getReadData();
    ARRAY_LL data3    = rdi.id("ID3").getReadData();
    ARRAY_LL data4    = rdi.id("ID4").getReadData();
    int      data1exp = rdi.id("ID1exp").getPassFail();

    cout << "Site: " << CURRENT_SITE_NUMBER() << ": JTAG_Capture Read ID1: "      << data1    << endl;
    cout << "Site: " << CURRENT_SITE_NUMBER() << ": JTAG_Capture Read ID2: "      << data2    << endl;
    cout << "Site: " << CURRENT_SITE_NUMBER() << ": JTAG_Capture Read ID3: "      << data3    << endl;
    cout << "Site: " << CURRENT_SITE_NUMBER() << ": JTAG_Capture Read ID4: "      << data4    << endl;
    cout << "Site: " << CURRENT_SITE_NUMBER() << ": JTAG_Capture Expect ID1exp: " << data1exp << endl;

    cout << "JTAG example test done." << endl << endl;

    DISCONNECT(); // make sure device is disconnected if debugging a single test suite

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
    string comment = " please add your comment for this method.";
    return comment;
  }
};
REGISTER_TESTMETHOD("JTAG_Example_rdi", JTAG_Example_rdi);
