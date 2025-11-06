# IJTAG Analog Simulation

This directory contains all the essential components for the **Analog Simulation** part of the **MAST** project using **Cadence/Spectre**.

---

## Overview

This setup enables **mixed-signal co-simulation** between **analog modules (Cadence/Spectre)** and **digital RTL (ModelSim/QuestaSim)** through the **MAST framework**.  
It reproduces a realistic analog–digital test flow for IJTAG-compatible mixed-signal systems.

---

## Note

> This is a **work in progress** — the first release of the analog co-simulation flow.  
> Feedback, suggestions, and contributions are very welcome!

---

## Requirements

To perform this co-simulation, you will need the following tools:

- **Cadence Virtuoso / Spectre** (version 16.0 or later recommended)  
- **Python 3.x**  
- **ModelSim** or **QuestaSim**  
- All standard **MAST project dependencies** (see the main MAST installation guide)

>  Ensure that all required environment variables (for Cadence, ModelSim, and MAST) are correctly configured before launching the simulation.

---

## Installation

1. Follow the installation instructions for **MAST** from the main repository.  
2. Install **Cadence** (for analog simulation).  
3. Install **ModelSim** or **QuestaSim** (for digital RTL simulation).  
4. Verify that both tools are accessible from your terminal.

---

## Quick Start

1. Place your **analog schematic** in the directory:
   ```bash
   Analog_SUT/{name_of_your_CUT}
   ```
   Example:
   ```bash
   Analog_SUT/SALLEN_KEY_FILTER
   ```

2. Place your generated **Ocean script** in the same directory, or run the simulation directly from Cadence.  
   Make sure that:
   - The output signal is named **`V_out`**  
   - Simulation results are stored in the subdirectory `data/`  
   - The file `model_weights.txt` is present in the same directory  

3. Launch the analog co-simulation script:
   ```bash
   ./launch_analog_sim.sh
   ```

---

## What `launch_analog_sim.sh` Does

This script automates the **entire analog–digital co-simulation pipeline**.  
Here’s what happens step by step:

1. **Configuration Step**  
   Runs `configure_cosim.sh` to prepare the simulation environment.

2. **VHDL Compilation**  
   Compiles all VHDL sources from `../RTL/vhdl/` using:
   ```bash
   ./compile_VHDL.sh
   ```

3. **(Optional) Cadence Simulation**  
   If Cadence is properly configured, uncomment the following lines in the script:
   ```bash
   # ocean < Analog_SUT/SALLEN_KEY_FILTER/OceanScript.ocn
   ```
   Replace `SALLEN_KEY_FILTER` with your own CUT name.  
   This will automatically run the transient simulation in Spectre and generate the voltage output file `V_out`.

4. **ADC Conversion**  
   Executes the Python script:
   ```bash
   python3 adc.py
   ```
   This converts the analog output voltage from Cadence (`V_out`) into binary samples (`reg.in`) to be exchanged with the digital domain.

5. **MAST Execution**  
   Runs the MAST session:
   ```bash
   ./Mast -c=Cosim.yml -s=./Examples/SIT/JTAG.sit
   ```
   This step creates exchange files and sends data to the RTL simulation.

6. **File Exchange with RTL**  
   MAST then waits for the RTL simulation to produce the response file:
   ```
   ../RTL/vhdl/data_from_rtl.dat
   ```
   It checks for this file every second (timeout: 120 s).

7. **Completion**  
   Displays:
   ```
   === Co-simulation completed successfully ===
   ```

---

## RTL Co-Simulation (in Another Terminal)

1. at the same time open a **second terminal** and navigate to:
   ```bash
   cd ../RTL/vhdl/
   ```

2. Launch **ModelSim** and load the top-level testbench:
   ```bash
   vsim work.svf_simulation_top
   ```

3. Configure waveform debugging as desired.  
   You can use the provided file:
   ```bash
   Tutorial_1.do
   ```
   which adds the basic IJTAG and exchange register signals to the waveform window.

4. MAST will automatically start, create the exchange files, and wait for ModelSim to send back data.

5. In the ModelSim console, run the simulation:
   ```bash
   run -all
   ```

   This triggers the digital part of the co-simulation.

---

## Summary of the Data Flow

| Step | Tool | Description |
|------|------|--------------|
| 1 | MAST | Sends configuration through `iWrite` commands |
| 2 | Cadence Spectre | Simulates the analog CUT and generates `V_out` |
| 3 | Python (`adc.py`) | Converts the analog waveform to binary codes (`reg.in`) |
| 4 | MAST | Sends binary data to RTL, manages synchronization |
| 5 | ModelSim | Runs the RTL simulation (`svf_simulation_top.vhd`) |
| 6 | MAST | Reads back RTL output (`data_from_rtl.dat`) and completes the flow |

---

## Example Setup

Example CUT: `SALLEN_KEY_FILTER`  
- Directory: `Analog_SUT/SALLEN_KEY_FILTER/`  
- Ocean script: `OceanScript.ocn`  
- Output signal: `V_out`  
- Generated data: `data/V_out.txt`

---

## Contribution

If you encounter any issue, please open an **Issue** or a **Pull Request**.  
All feedback is welcome to improve this analog co-simulation environment.

---

## License

This part of the project is distributed under the  
**GNU Lesser General Public License (LGPL v2.1)** — the same as the main MAST framework.
