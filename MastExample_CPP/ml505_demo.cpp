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
    auto     loopCount    = 2048u;
		auto		 fft_nb_bins	=	255;
    auto     i    = 0u;
    uint16_t initialValue = 1u;
    uint16_t adcSampledValues = 32u;		// 0x20 is a dummy value.
		uint16_t lcdCalculatedAmplitudes;

		kiss_fftr_cfg kissCfg = nullptr;

		kiss_fft_scalar adcLeftChannelValue = 1.0f;
		kiss_fft_scalar adcRightChannelValue = 1.0f;

		kiss_fft_scalar adcLeftChannelSamples[fft_nb_bins];
		kiss_fft_scalar adcRightChannelSamples[fft_nb_bins];

		kiss_fft_cpx fftOutputLeftChannel[fft_nb_bins];
		kiss_fft_cpx fftOutputRightChannel[fft_nb_bins];

		kiss_fft_scalar fftOutputAmplitudesLeft, fftOutputAmplitudesRight;

		kiss_fft_scalar fftOutputMaxAmplitudesLeft, fftOutputMaxAmplitudesRight;


		kissCfg = kiss_fftr_alloc(fft_nb_bins-1, 0, 0, 0);

		//CHECK_VALUE_NOT_NULL(kissCfg, "[KissFFT] KissFFT configuration returns null.");

		for(int k=0; k < fft_nb_bins; k++) {
				adcLeftChannelSamples[k] = 0.0f;
				adcRightChannelSamples[k] = 0.0f;
			}

    std::cout << "Running " << loopCount << " KissFFT on registers " <<lcdRegisterPath << "; " << adcRegisterPath << "\n";

    while (i++<loopCount)
    {
			iGetRefresh(adcRegisterPath, adcSampledValues);

			adcRightChannelValue =  static_cast<kiss_fft_scalar>(adcSampledValues&0x00FF);

			adcLeftChannelValue =  static_cast<kiss_fft_scalar>((adcSampledValues>>8)&0x00FF);

			for(int k=0; k< fft_nb_bins-1; k++) {
				adcLeftChannelSamples[k] = adcLeftChannelSamples[k+1];
				adcRightChannelSamples[k] = adcRightChannelSamples[k+1];
			}

			adcLeftChannelSamples[fft_nb_bins-1] = adcLeftChannelValue;
			adcRightChannelSamples[fft_nb_bins-1] = adcRightChannelValue;

			kiss_fftr(kissCfg, adcLeftChannelSamples, fftOutputLeftChannel);
			kiss_fftr(kissCfg, adcRightChannelSamples, fftOutputRightChannel);

			fftOutputMaxAmplitudesLeft = 0.0f;
			fftOutputMaxAmplitudesRight = 0.0f;

			for(int k=0; k<(fft_nb_bins/2); k++) { // Frequency domain runs from 0 to NB_BINS/2 because we use only real components.

				// Estimating the amplitude of each bin
				fftOutputAmplitudesLeft
						= static_cast<kiss_fft_scalar>(
											sqrt((fftOutputLeftChannel[k].r/fft_nb_bins*fftOutputLeftChannel[k].r/fft_nb_bins)
											+(fftOutputLeftChannel[k].i/fft_nb_bins*fftOutputLeftChannel[k].i/fft_nb_bins))
											);

				// The VU-meter only displays the bin which amplitude is maximum at this time step
				if(fftOutputAmplitudesLeft > fftOutputMaxAmplitudesLeft)
					fftOutputMaxAmplitudesLeft = fftOutputAmplitudesLeft;

				// Repeating above operations for the right audio channel.
				fftOutputAmplitudesRight
						= static_cast<kiss_fft_scalar>(
											sqrt((fftOutputRightChannel[k].r/fft_nb_bins*fftOutputRightChannel[k].r/fft_nb_bins)
											+(fftOutputRightChannel[k].i/fft_nb_bins*fftOutputRightChannel[k].i/fft_nb_bins))
											);

				if(fftOutputAmplitudesRight > fftOutputMaxAmplitudesRight)
					fftOutputMaxAmplitudesRight = fftOutputAmplitudesRight;
			}

			
			// 16-bit output word for both audio channels:
			// MSB contains the left channel amplitude, LSB the right one.
			lcdCalculatedAmplitudes = static_cast<uint16_t>(fftOutputMaxAmplitudesLeft);
			lcdCalculatedAmplitudes<<=8;
			lcdCalculatedAmplitudes += static_cast<uint16_t>(fftOutputMaxAmplitudesRight);
			
      iWrite(lcdRegisterPath, lcdCalculatedAmplitudes);

     std::cout << "\n Cycle "<< i << ": Sampled " << (int)adcLeftChannelSamples[fft_nb_bins-1]
						<< ":" << (int)adcRightChannelSamples[fft_nb_bins-1];

     std::cout << "\n       "<< i << ": Estimated amplitude " << (int)fftOutputMaxAmplitudesLeft
						<< ":" << (int)fftOutputMaxAmplitudesRight << std::endl;

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
