//===========================================================================
//                           Example.cpp
//===========================================================================
//! @file Example.cpp
//!
//! Example of PDL like algorithm implementation and association to node
//!
//===========================================================================

#include "CPP_API.hpp"
#include "g3log/g3log.hpp"

#include "kissfft/tools/kiss_fftr.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


using std::vector;
using std::string;
using std::ofstream;

using namespace std::string_literals;
using namespace mast;

std::string                               GetSitModelFilePath       (int argc, char* argv []);
std::vector<mast::AppFunctionAndName>     GetAlgorithmsNames        (int argc, char* argv []);
std::vector<mast::AppFunctionAndNodePath> GetAlgorithmsAndNodePaths (int argc, char* argv []);

#include "psdpng.c"

namespace
{

double
blackman (int i, int nn)
{
 return ( 0.42 - 0.5 * cos (2.0*M_PI*(double)i/(double)(nn-1))
     + 0.08 * cos (4.0*M_PI*(double)i/(double)(nn-1)) );
}

void
windowing (int n, kiss_fft_scalar *data, float scale,  kiss_fft_scalar *out)
{
 int i;
 float tmp;
 for (i = 0; i < n; i ++)
   {
     tmp = ((float)data[i]) * blackman (i, n) / scale;
     out [i] = static_cast<kiss_fft_scalar>(tmp);
   }
return;
}

  //! This algorithm Computes the FFT
  //!
  void compute_FFT ()
  {

    auto     registerPath = "SDM_out";
    auto     n_samples    = 72064u;
    auto     i    = 0u;
    uint16_t curValue;

    kiss_fftr_cfg kissCfg = nullptr;

    kiss_fft_scalar adcValue = 1.0f;

    kiss_fft_scalar adcSamples[n_samples];
    kiss_fft_cpx fftOutput[n_samples];

    kissCfg = kiss_fftr_alloc(n_samples, 0, 0, 0);
    std::cout << "Retrieving " << n_samples << " samples from " <<registerPath << "\n";
    while (i<n_samples)
    {
     curValue=iGetRefresh<uint16_t>(registerPath);
     if (curValue==0)
        adcValue =  static_cast<kiss_fft_scalar>(-1);
     else
        adcValue =  static_cast<kiss_fft_scalar>(curValue);
     adcSamples[i]=adcValue;
     i++;
    }
     std::cout << "Applying Blackman windowing\n";
   windowing (n_samples, adcSamples, 1.0,adcSamples);
     std::cout << "Running FFT\n";
     kiss_fftr(kissCfg, adcSamples, fftOutput);

   ofstream out_real,out_complex;
   out_real.open   ("Output_real.dat");
   out_complex.open("Output_complex.dat");
   i=0;
    while (i<n_samples)
    {
     out_real    << static_cast<float>(fftOutput[i].r) <<"\n";
     out_complex << static_cast<float>(fftOutput[i].i) <<"\n";
    i++;
    }
   out_real.close();
   out_complex.close();

   /*Making image*/
/*   nfft = n_samples;
  fout = fopen("FFT.png","wb");
  make_png(fftOutput);

  fclose(fout);*/

 }
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
    {compute_FFT, "", "compute_FFT"},
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
    {compute_FFT, "compute_FFT"},
  };

  return names;
}
//
//  End of: GetAlgorithmsNames
//---------------------------------------------------------------------------


//! Returns SIT formatted file path to create a SystemModel from.
//!
string GetSitModelFilePath (int argc, char* argv[])
{
  if (argc > 1)
  {
    return string(argv[1]);
  }

  return "Example.sit";
}
//
//  End of: GetSitModelFilePath
//---------------------------------------------------------------------------


//===========================================================================
// End of Example.cpp
//===========================================================================
