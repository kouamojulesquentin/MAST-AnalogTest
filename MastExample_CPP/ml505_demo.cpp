//===========================================================================
//                           ml505_demo.cpp
//===========================================================================
//! @file ml505_demo.cpp
//!
//! PDL wrapper executing the FFT for the ML505 demo. ADC and LCD are updated
//! via the JTAG protocol.
//!
//===========================================================================

#include "AppFunctionAndNodePath_CPP.hpp"
#include "AppFunctionAndName_CPP.hpp"
#include "PDL_Adapter_CPP.hpp"
//+#include "g3log/g3log.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "kissfft/tools/kiss_fftr.h"


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
  void kissfft ()
  {

    auto     lcdRegisterPath = "reg_lcd";
		auto     adcRegisterPath = "reg_adc";
    auto     loopCount    = 5u;
    auto     i    = 0u;
    uint16_t initialValue = 1u;
    uint16_t adcSampledValues = 32u;
		uint16_t lcdCalculatedAmplitudes;

		kiss_fftr_cfg kissCfg = nullptr;

		kiss_fft_scalar adcLeftChannelValue = 1.0f;
		kiss_fft_scalar adcRightChannelValue = 1.0f;

		kiss_fft_scalar adcLeftChannelSamples[255];
		kiss_fft_scalar adcRightChannelSamples[255];

		kiss_fft_cpx fftOutputLeftChannel[255];
		kiss_fft_cpx fftOutputRightChannel[255];


		kissCfg = kiss_fftr_alloc(254, 0, 0, 0);

		//CHECK_VALUE_NOT_NULL(kissCfg, "[KissFFT] KissFFT configuration returns null.");

		for(int i=0; i< 255; i++) {
				adcLeftChannelSamples[i] = 0.0f;
				adcRightChannelSamples[i] = 0.0f;
			}

    std::cout << "Running " << loopCount << " KissFFT on registers " <<lcdRegisterPath << "; " << adcRegisterPath << "\n";
		std::cout << "We suppose ADC has transmitted the following 16-bit word: " << (int) adcSampledValues << std::endl;
    while (i++<loopCount)
    {
			//iGetRefresh(adcRegisterPath, adcSampledValues);

			adcRightChannelValue =  static_cast<kiss_fft_scalar>(adcSampledValues&0x00FF);

			adcLeftChannelValue =  static_cast<kiss_fft_scalar>((adcSampledValues>>8)&0x00FF);

			for(int i=0; i< 254; i++) {
				adcLeftChannelSamples[i] = adcLeftChannelSamples[i+1];
				adcRightChannelSamples[i] = adcRightChannelSamples[i+1];
			}

			adcLeftChannelSamples[254] = adcLeftChannelValue;
			adcRightChannelSamples[254] = adcRightChannelValue;

			kiss_fftr(kissCfg, adcLeftChannelSamples, fftOutputLeftChannel);
			kiss_fftr(kissCfg, adcRightChannelSamples, fftOutputRightChannel);
			
			lcdCalculatedAmplitudes = static_cast<uint16_t>(fftOutputLeftChannel[0].r);
			lcdCalculatedAmplitudes<<=8;
			lcdCalculatedAmplitudes += static_cast<uint16_t>(fftOutputRightChannel[0].r);
			
      iWrite(lcdRegisterPath, lcdCalculatedAmplitudes);
 //     iGet(registerPath,curValue);
     std::cout << "\n Cycle "<< i << ": Sampled " << (int)adcLeftChannelSamples[254] << ":" << (int)adcRightChannelSamples[254];
     std::cout << "\n       "<< i << ": Estimated amplitude " << (int)fftOutputLeftChannel[0].r << ":" << (int)fftOutputRightChannel[0].r << std::endl;
      iApply();

      ++initialValue;
    }
     std::cout << "\n" ;

		free(kissCfg);
  }
  //
  //  End of: Algo_Increment
  //---------------------------------------------------------------------------
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
    {kissfft, "", "kissfft"},
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
    {kissfft, "kissfft"},
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

  return "ml505_demo.sit";
}
//
//  End of: GetSitModelFilePath
//---------------------------------------------------------------------------


//===========================================================================
// End of ml505_demo.cpp
//===========================================================================
