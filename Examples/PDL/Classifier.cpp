//===========================================================================
//  File        : classifier.cpp
//  Project     : MAST Analog/Digital Co-Simulation
//  Description : Acquires simulated ADC register data, reconstructs voltages,
//                and performs classification using a simple ML model.
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

namespace mast
{

//===========================================================================
// Utility functions
//===========================================================================

// Load model weights and bias from text file
static bool LoadModel(const std::string& modelPath,
                      std::vector<double>& weights,
                      double& bias,
                      std::string& errMsg)
{
    std::ifstream ifs(modelPath);
    if (!ifs.is_open())
    {
        errMsg = "Cannot open model file: " + modelPath;
        return false;
    }

    std::string line;
    std::getline(ifs, line);
    std::istringstream iss(line);
    double val;
    std::vector<double> vals;

    while (iss >> val)
        vals.push_back(val);

    if (vals.size() < 2)
    {
        errMsg = "Invalid model file format.";
        return false;
    }

    bias = vals.back();
    vals.pop_back();
    weights = vals;
    return true;
}

// Compute dot product
static double DotProduct(const std::vector<double>& a,
                         const std::vector<double>& b)
{
    double s = 0.0;
    size_t n = std::min(a.size(), b.size());
    for (size_t i = 0; i < n; ++i)
        s += a[i] * b[i];
    return s;
}

// Logistic sigmoid
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

//===========================================================================
// Main algorithm
//===========================================================================
void Classifier()
{
    constexpr unsigned int loopCount = 5u;  // number of ADC samples
    constexpr double Vref = 0.000188435;     // same as adc.py output
    constexpr int bits = 16;                 // ADC resolution
    constexpr double LSB = Vref / ((1ULL << bits) - 1);
    constexpr double threshold = 0.5;

    const std::string regName = "reg";                // ADC register
    const std::string modelFile = "model_weights.txt";

    std::cout << "\n[Classifier] Starting analog classification pipeline...\n";
    std::cout << "[ADC] Using Vref = " << Vref << " V ; bits = " << bits
              << " ; LSB = " << LSB << " V\n";

    // Step 1: Acquire ADC-like register data
    std::vector<double> analogSamples;
    uint16_t curValue = 0;
    uint16_t writeValue = 1;

    iNote(iNoteType::Comment, "Starting ADC register sampling...");

    for (unsigned int i = 0; i < loopCount; ++i)
    {
        // Simulate ADC register write and read
        iWrite(regName, writeValue);
        iApply();
        curValue = iGet<uint16_t>(regName);

        // Convert register value to analog voltage
        double voltage = static_cast<double>(curValue) * LSB;
        analogSamples.push_back(voltage);

        // Print debug info
        std::cout << "Cycle " << i + 1 << ": "
                  << "reg=" << std::dec << curValue
                  << " (" << std::hex << curValue << "h)"
                  << " → V=" << std::dec << voltage << " V\n";

        // Optionally increment or modulate the input value
        writeValue = (writeValue + 17) % 65535;

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    std::cout << "[ADC] Acquired " << analogSamples.size()
              << " samples from simulated register.\n";

    // Step 2: Load ML model
    std::vector<double> weights;
    double bias = 0.0;
    std::string err;

    if (!LoadModel(modelFile, weights, bias, err))
    {
        std::cerr << "[Error] " << err << std::endl;
        return;
    }

    std::cout << "[Model] Loaded " << weights.size() << " weights + bias.\n";

    // Step 3: Adjust size
    if (analogSamples.size() > weights.size())
        analogSamples.resize(weights.size());
    else if (analogSamples.size() < weights.size())
        analogSamples.resize(weights.size(), 0.0);

    // Step 4: Classification
    double score = DotProduct(weights, analogSamples) + bias;
    double prob = Sigmoid(score);
    bool isGo = (prob >= threshold);

    // Step 5: Output result
    std::cout << "\n[Result] Probability = " << prob
              << " → Decision = " << (isGo ? "Go" : "NoGo") << "\n";
    std::cout << "[Classifier] Finished successfully.\n";
}

//===========================================================================
// Register in MAST
//===========================================================================
bool RegisterAlgorithms()
{
    auto& repo = PDL_AlgorithmsRepository::Instance();
    repo.RegisterAlgorithm("Classifier", Classifier);
    return true;
}

bool registered = RegisterAlgorithms();

} // namespace mast