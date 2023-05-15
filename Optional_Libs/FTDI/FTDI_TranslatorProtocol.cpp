//===========================================================================
//                           FTDI_TranslationProtocol.cpp
//===========================================================================
// Copyright (C) 2020 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file FTDI_TranslatorProtocol.cpp
//!
//! Implements class FTDI_TranslatorProtocol, which uses the lifFTDI library
//! to performs transactions on a remote target trought an FTDI chip
//! 
//! Current support: JTAG SDR and SIR
//!
//!------------------------ Pin mapping ----------------------------------
//!
//!                JTAG
//!AD0 - CN2-07 --> TCK
//!AD1 - CN2-10 --> TDI
//!AD2 - CN2-09 --> TDO
//!AD3 - CN2-12 --> TMS
//!AD4 - CN2-14 --> TRST
//===========================================================================

#include "FTDI_TranslatorProtocol.hpp"
#include "Utility.hpp"
#include "RVF.hpp"

#include <experimental/string_view>
#include <sstream>
#include <string.h>

#include "g3log/g3log.hpp"

//Functions for JTAG to bit-banging
#include "function.h"

using namespace mast;
using std::string;
using std::experimental::string_view;
using std::ostringstream;

using std::vector;
using std::initializer_list;

using namespace std::string_literals;
using namespace std::experimental::literals::string_view_literals;

//! Initialization with no parameters
//!
FTDI_TranslatorProtocol::FTDI_TranslatorProtocol ()
{ 
 int retval = 0;
 //Allocate and initialise FTDI context
 jtag = (jtag_context_t*) calloc(1,sizeof(*jtag));
 ftdi = my_ftdi_start(&retval);
 if(retval != 0) 
   {
    LOG(ERROR_LVL)<<"Error in initializing libftdi : my_ftdi_start returned " <<retval;
    THROW_RUNTIME_ERROR("Error in initializing libftdi : my_ftdi_start returned " + retval);
    }
 //Set Default TCK speed
    jtag->buf[0] = 0x00;
    jtag->clock = 0.5 * 100;

}
//
//  End of: FTDI_TranslatorProtocol::FTDI_TranslatorProtocol
//---------------------------------------------------------------------------

//! Clean up FTDI descriptors
//!
FTDI_TranslatorProtocol::~FTDI_TranslatorProtocol ()
{ 
  LOG(DEBUG)<<"Cleaning up FTDI Descriptors";
   my_ftdi_free(ftdi);
   free(jtag);

}
//
//  End of: FTDI_TranslatorProtocol::~FTDI_TranslatorProtocol
//---------------------------------------------------------------------------

//Allows to print a unsigned char
void string_print_char(char *out_string,unsigned char * buf, int nbytes){
    int i;
    int position;
    position=sprintf(out_string,"0x");
    for(i = 0;i< nbytes; i++){
        position+=sprintf(out_string+position,"%02x.",buf[i]);
    }
    printf("\n");
}

BinaryVector FTDI_TranslatorProtocol::TransformationCallback(RVFRequest current_request) 
{
  // This callback is called each time a RVFRequest arrived
  unsigned char * buf_read, *buf_write;
   
  BinaryVector from_SUT;
  
  LOG(DEBUG) << "FTDI TransformationCallback: received a Request for a "<<     current_request.CallbackId()<<" operation of " <<
       current_request.ToSutVector().BitsCount() << "bits ("<<current_request.ToSutVector().BytesCount() <<" bytes)"  ;

   if (current_request.CallbackId()==TRST)
    {THROW_RUNTIME_ERROR("TRST unimplemented yet");
     }

   else if ((current_request.CallbackId()==SIR) || (current_request.CallbackId()==SDR))
     {//THROW_RUNTIME_ERROR("SIR unimplemented yet");
        auto bytesCount = current_request.ToSutVector().BytesCount() ; 

        buf_read = (unsigned char *) calloc(bytesCount,sizeof(unsigned char));
         buf_write = (unsigned char *) calloc(bytesCount,sizeof(unsigned char));
	 

	memcpy(buf_write,(unsigned char *)current_request.ToSutVector().DataLeftAligned(),bytesCount);
	char PROVA[200];
	string_print_char(PROVA,buf_write,bytesCount);
	//string_print_char(PROVA,(unsigned char *)current_request.ToSutVector().DataLeftAligned(),bytesCount);
	
          if (current_request.CallbackId()==SIR)
           {
                LOG(DEBUG) << "FTDI TransformationCallback: Preparing to run my_ftdi_sir";
                my_ftdi_sir(jtag,
                           buf_write, //(unsigned char *)current_request.ToSutVector().DataLeftAligned(),
                           buf_read, 
          		 current_request.ToSutVector().BitsCount(),
                           ftdi);
               LOG(DEBUG) << "FTDI TransformationCallback: my_ftdi_sir finished";
               }
     
          if (current_request.CallbackId()==SDR)
           {
                LOG(DEBUG) << "FTDI TransformationCallback: Preparing to run my_ftdi_sdr";
                my_ftdi_sdr(jtag,
                           buf_write, //(unsigned char *)current_request.ToSutVector().DataLeftAligned(),
                           buf_read, 
		           current_request.ToSutVector().BitsCount(),
                           ftdi);
               LOG(DEBUG) << "FTDI TransformationCallback: my_ftdi_sdr finished";
               }

       string_print_char(PROVA,buf_read,bytesCount);
       LOG(DEBUG) << "FTDI TransformationCallback: from_SUT: " <<PROVA;

     //Convert C buffer to C++ vector to call a constructor

/*    auto bytesCount = (current_request.ToSutVector().BitsCount()%8==0)? 
    			current_request.ToSutVector().BitsCount()/8 : 
			current_request.ToSutVector().BitsCount()/8+1;*/
      vector<uint8_t> C_Data;
      for (auto i=0;i<bytesCount;i++)
       C_Data.push_back(buf_read[i]);
      from_SUT = BinaryVector(C_Data, current_request.ToSutVector().BitsCount());
    free(buf_read);	 
       }
   else if (current_request.CallbackId()==NO_MORE_PENDING)
   {
    //Syncrhonisation
    //Finished, release parent Translator
    RVFRequest request(NO_MORE_PENDING);
    PushRequest(request);
    return from_SUT;
   }
    else
      THROW_RUNTIME_ERROR("FTDI : unknown primitive "+current_request.CallbackId());


  
    LOG(DEBUG) << "FTDI TransformationCallback: returning a vector of size "<<from_SUT.BitsCount();
  //Return Callback result to lower level
  return from_SUT;
}
//
//  End of: FTDI_TranslatorProtocol::TransformationCallback
//---------------------------------------------------------------------------


//===========================================================================
// End of FTDI_TranslatorProtocol.cpp
//===========================================================================
