//===========================================================================
//                           ml505_fft.cpp
//===========================================================================
// Copyright (C) 2017 G-INP/Tima. All rights reserved.
//
// Project : Mast
//
//! @file Tutorial_1.cpp
//!
//! Implements PDL algorithms of tutorial 1
//!
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "CPP_API.hpp"
#include "g3log/g3log.hpp"
#include "kissfft/tools/kiss_fftr.h"

#include <iostream>

using namespace mast;


namespace
{
#define en_on 		0x80 
#define pden_on 	0x40 
#define en_off 		0x00 
#define pden_off 	0x00 

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
  //  End of: LDO
  //---------------------------------------------------------------------------




//! Registers PDL algorithm functions in this file
//!
//! @note Names used from registration must be the same as found in SIT file
bool RegisterAlgorithms ()
{
  // ---------------- Get an handle on PDL algorithm repository
  //
  auto& repo = PDL_AlgorithmsRepository::Instance();

  // ---------------- Do register algorithm(s) with a name
  //
  repo.RegisterAlgorithm("kissfft", kissfft);

  return true;
}
//
//  End of: RegisterAlgorithms
//---------------------------------------------------------------------------


//! Make PDL algorithm functions in this file to be registered
//!
//! @note As a "static" variable, it is initialized once when the corresponding DLL is loaded
//!
bool registrated = RegisterAlgorithms();

} // End of unnamed namespace


//===========================================================================
// End of Tutorial_1.cpp
//===========================================================================
