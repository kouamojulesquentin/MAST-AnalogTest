//===========================================================================
//                           ml505_demo.cpp
//===========================================================================
// Copyright (C) 2016 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file ml505_demo.cpp
//!
//! Implements functions for demos using MAST ML505 prototype board
//!
//===========================================================================

#include "Startup.hpp"
#include "AppFunctionAndNodePath_CPP.hpp"
#include "PDL_Adapter_CPP.hpp"
//+#include "g3log/g3log.hpp"
#include "ml505_demo.hpp"

#include <string>
#include <iostream>
#include "kissfft/tools/kiss_fftr.h"

using std::vector;
using std::string;
using namespace mast;


//! Returns descriptions for applications running with the ML505 board
//!
//! @note Adapt this function as necessary
//!       It must nevertheless be compliant with SystemModel (possibly with 'SIT' file)
vector<AppFunctionAndNodePath> test::ml505_CreateApplicationsDescriptor ()
{
  auto app = []()
  {
    auto     lcdRegisterPath = "reg_lcd";
		auto     adcRegisterPath = "reg_adc";
    auto     loopCount    = 2048u;
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
			iGetRefresh(adcRegisterPath, adcSampledValues);

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
			
			lcdCalculatedAmplitudes = static_cast<uint16_t>(fftOutputLeftChannel[0].r/255);
			lcdCalculatedAmplitudes<<=8;
			lcdCalculatedAmplitudes += static_cast<uint16_t>(fftOutputRightChannel[0].r/255);
			
      iWrite(lcdRegisterPath, lcdCalculatedAmplitudes);
 //     iGet(registerPath,curValue);
     std::cout << "\n Cycle "<< i << ": Sampled " << (int)adcLeftChannelSamples[254] << ":" << (int)adcRightChannelSamples[254];
     std::cout << "\n       "<< i << ": Estimated amplitude " << (int)fftOutputLeftChannel[0].r/255 << ":" << (int)fftOutputRightChannel[0].r/255 << std::endl;
      iApply();

      ++initialValue;
    }
     std::cout << "\n" ;

		free(kissCfg);
  };

  vector<AppFunctionAndNodePath> associations;

  associations.emplace_back(app, ".", "ML505");

  return associations;
}
//
//  End of: test::ml505_CreateApplicationsDescriptor
//---------------------------------------------------------------------------


//===========================================================================
// End of ml505_demo.cpp
//===========================================================================
