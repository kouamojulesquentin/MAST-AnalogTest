//===========================================================================
//  File        : Classifier.cpp
//  Project     : MAST-AnalogTest
//  Description : Analog-data-based classifier 
//===========================================================================
//  Author      : Jules Quentin KOUAMO
//  Institution : TIMA Laboratory
//  Date        : 2025-10-21
//===========================================================================

#include "PDL_AlgorithmsRepository.hpp"
#include "CPP_API.hpp"
#include "g3log/g3log.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdint>
#include <complex>  

namespace mast
{

//===========================================================================
// Utility functions
//===========================================================================

// Convert binary string to unsigned integer 
static unsigned long long BinaryToCode(const std::string& binStr)
{
    std::string s = binStr;
    s.erase(std::remove_if(s.begin(), s.end(),
                           [](char c) { return c != '0' && c != '1'; }),
            s.end());
    if (s.empty())
        return 0ULL;

    try
    {
        return std::stoull(s, nullptr, 2);
    }
    catch (...)
    {
        return 0ULL;
    }
}

// Convert ADC code to voltage 
static double CodeToVoltage(unsigned long long code, double LSB)
{
    return static_cast<double>(code) * LSB;
}




// read sample values from "reg"
void Data_Acquisition ()
{
  auto     registerPath = "reg";
  auto     loopCount    = 1667u; // Number of samples to acquire
  uint32_t initialValue = 1u;

 std::cout << "\n[ADC] Starting acquisition on register \n";
    std::vector<double> DigitalSamples;
  for (int cycle = 0 ; cycle < loopCount ; ++cycle)
  {
    iWrite(registerPath, initialValue);
    auto curValue = iGet<uint32_t>(registerPath);
    DigitalSamples.push_back(static_cast<double>(curValue));
   iApply();
    ++initialValue;
  }
}




//---------------------------------- Model functions ----------------------------------

//--------------------------------------
// Logistic sigmoid activation
//--------------------------------------
static double Sigmoid(double x)
{
    if (x >= 0.0)
    {
        double z = std::exp(-x);
        return 1.0 / (1.0 + z);
    }
    else
    {
        double z = std::exp(x);
        return z / (1.0 + z);
    }
}

//--------------------------------------
// ReLU activation
//--------------------------------------
static double ReLU(double x)
{
    return std::max(0.0, x);
}

//--------------------------------------
// Dot Product
//--------------------------------------
static double DotProduct(const std::vector<double>& a,
                         const std::vector<double>& b)
{
    double s = 0.0;
    size_t n = std::min(a.size(), b.size());
    for (size_t i = 0; i < n; ++i)
        s += a[i] * b[i];
    return s;
}

//--------------------------------------
//  Model structure
//--------------------------------------
struct Model
{
    int n_input = 0;
    int n_hidden = 0;
    int n_output = 0;

    std::vector<std::vector<double>> W1; 
    std::vector<double> b1;              
    std::vector<std::vector<double>> W2; 
    std::vector<double> b2;              
};

//--------------------------------------
// Load full  model
//--------------------------------------
static bool LoadModel(const std::string& modelPath, Model& model, std::string& errMsg)
{
    std::ifstream ifs(modelPath);
    if (!ifs.is_open())
    {
        errMsg = "Cannot open model file: " + modelPath;
        return false;
    }

    
    ifs >> model.n_input >> model.n_hidden >> model.n_output;
    if (!ifs || model.n_input <= 0 || model.n_hidden <= 0 || model.n_output <= 0)
    {
        errMsg = "Invalid model header in " + modelPath;
        return false;
    }

    
    model.W1.assign(model.n_hidden, std::vector<double>(model.n_input, 0.0));
    for (int i = 0; i < model.n_hidden; ++i)
        for (int j = 0; j < model.n_input; ++j)
            ifs >> model.W1[i][j];

    
    model.b1.assign(model.n_hidden, 0.0);
    for (int i = 0; i < model.n_hidden; ++i)
        ifs >> model.b1[i];

   
    model.W2.assign(model.n_output, std::vector<double>(model.n_hidden, 0.0));
    for (int i = 0; i < model.n_output; ++i)
        for (int j = 0; j < model.n_hidden; ++j)
            ifs >> model.W2[i][j];

    
    model.b2.assign(model.n_output, 0.0);
    for (int i = 0; i < model.n_output; ++i)
        ifs >> model.b2[i];

    return true;
}

//--------------------------------------
//  Inference
//--------------------------------------
static std::vector<double> Forward(const Model& model,
                                      const std::vector<double>& input)
{
    // Hidden layer computation
    std::vector<double> hidden(model.n_hidden, 0.0);
    for (int i = 0; i < model.n_hidden; ++i)
    {
        hidden[i] = DotProduct(model.W1[i], input) + model.b1[i];
        hidden[i] = ReLU(hidden[i]);
    }

    // Output layer computation
    std::vector<double> output(model.n_output, 0.0);
    for (int i = 0; i < model.n_output; ++i)
    {
        output[i] = DotProduct(model.W2[i], hidden) + model.b2[i];
        output[i] = Sigmoid(output[i]);
    }

    return output;
}


// ------------------------------------- End Model functions -------------------------------------




/===========================================================================
// Main algorithm implementation
//===========================================================================
void Classifier()
{
    const std::string regInFile = "../RTL/vhdl/data_from_rtl.dat";
    const std::string modelFile = "Analog_SUT/SALLEN_KEY_FILTER/model_weights.txt";

    const double Vref = 1.317204;  // same as adc.py output
    const int bits = 32;
    const double LSB = Vref / ((1ULL << bits) - 1);
    const double threshold = 0.5;

    std::cout << "\n[Classifier] Starting analog classification pipeline...\n";

    // Read binary data and reconstruct analog samples
    std::ifstream fin(regInFile);
    if (!fin.is_open())
    {
        std::cerr << "[Error] Cannot open " << regInFile << "\n";
        return;
    }

    std::vector<double> analogSamples;
    std::getline(fin, line); // Skip header line
    std::string line;

    while (std::getline(fin, line))
    {
        if (line.empty()) continue;
        char sign_bit = line.front();
        std::string mag_bits = line.substr(1);
        unsigned long long code = BinaryToCode(mag_bits);
        double voltage = CodeToVoltage(code, LSB);
        if (sign_bit == '1') voltage = -voltage;
        analogSamples.push_back(voltage);
    }
    fin.close();

    if (analogSamples.empty())
    {
        std::cerr << "[Error] No valid samples found in reg.in.\n";
        return;
    }

    // FFT computation
    std::vector<std::complex<double>> fourierTransform;
    std::vector<double> AnalogSamplesSpectrum;

    size_t N = analogSamples.size();
    size_t N_fft = N / 2 + 1;
    fourierTransform.resize(N_fft);

    for (size_t k = 0; k < N_fft; ++k)
    {
        std::complex<double> sum(0.0, 0.0);
        for (size_t n = 0; n < N; ++n)
        {
            double angle = -2.0 * M_PI * k * n / N;
            sum += analogSamples[n] * std::exp(std::complex<double>(0.0, angle));
        }
        fourierTransform[k] = sum;
        AnalogSamplesSpectrum.push_back(std::abs(sum));
    }



    // Load  model
    Model model;
    std::string err;
    if (!LoadModel(modelFile, model, err))
    {
        std::cerr << "[Error] " << err << std::endl;
        return;
    }

    std::cout << "[Model] Loaded\n";

  
    std::vector<double> input = AnalogSamplesSpectrum;
    if (input.size() > (size_t)model.n_input)
        input.resize(model.n_input);
    else if (input.size() < (size_t)model.n_input)
        input.resize(model.n_input, 0.0);

    std::vector<double> output = Forward(model, input);
    double prob = output[0];
    bool isGo = (prob >= threshold);

    std::cout << "[Result] Probability = " << prob
              << " → Decision = " << (isGo ? "Go" : "NoGo") << "\n";
    std::cout << "[Classifier] Finished successfully.\n";
}




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
  repo.RegisterAlgorithm("Classifier", Classifier);
  repo.RegisterAlgorithm("Data_Acquisition", Data_Acquisition);
    

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



} // namespace mast