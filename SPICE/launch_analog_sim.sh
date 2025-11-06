#!/bin/bash

# Co-simulation script 

set -e  

echo "=== Starting co-simulation ==="


echo "Configuring co-simulation..."
./configure_cosim.sh
if [ $? -ne 0 ]; then
    echo "Error during configuration"
    exit 1
fi


echo "Compiling VHDL..."
cd ../RTL/vhdl
./compile_VHDL.sh
cd ../../SPICE/

if [ $? -ne 0 ]; then
    echo "Error during VHDL compilation"
    exit 1
fi

###  Uncomment the following lines if you have all the requirements to run your cadence simulation, replace the current circuit (SALLEN_KEY_FILTER) with your own. 
# echo "Running Ocean..."
# ocean < Analog_SUT/SALLEN_KEY_FILTER/OceanScript.ocn
# if [ $? -ne 0 ]; then
#     echo "Error running Ocean"
#     exit 1
# fi


echo "Running ADC script"
python3 adc.py
if [ $? -ne 0 ]; then
    echo "Error running ADC script"
    exit 1
fi


echo "Running Mast"
./Mast -c=Cosim.yml -s=./Examples/SIT/JTAG.sit
if [ $? -ne 0 ]; then
    echo "Error running Mast "
    exit 1
fi


if [ -f Emulation.log ]; then
    cp Emulation.log ../RTL/vhdl/data_to_rtl.svf
  
else
    exit 1
fi


echo "Waiting for RTL simulation to produce data_from_rtl.dat..."
TIMEOUT=120 
ELAPSED=0

while [ ! -s ../RTL/vhdl/data_from_rtl.dat ]; do
    if [ $ELAPSED -ge $TIMEOUT ]; then
        echo " Timeout: data_from_rtl.data not produced after ${TIMEOUT}s"
        exit 1
    fi
    echo "Waiting datas from RTL simulation ..."
    sleep 1
    ELAPSED=$((ELAPSED + 1))
done

echo "MAST"
./Mast -c=Cosim.yml -s=./Examples/SIT/JTAG2.sit
if [ $? -ne 0 ]; then
    echo "Error running Mast"
    exit 1
fi

echo "=== Co-simulation completed successfully ==="