#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
adc.py — simple ADC emulator for MAST co-simulation
--------------------------------------------------


Author:  Jules KOUAMO, 2025-10-22
"""

from __future__ import annotations
import re
import os
import sys
import argparse
from typing import List, Optional

# -------------------------
# Default configuration
# -------------------------
DEFAULT_INPUT = "Analog_SUT/SALLEN_KEY_FILTER/data/V_out.txt"
DEFAULT_OUTPUT = "../RTL/vhdl/Simulation_exchange_files/tutorial_1/reg.in"
DEFAULT_BITS = 32
DEFAULT_VREF = None
DEFAULT_GAIN = 1.0

# -------------------------
# Utilities
# -------------------------
UNIT_MULTIPLIERS = {
    'p': 1e-12, 'n': 1e-9, 'u': 1e-6, 'm': 1e-3,
    'k': 1e3, 'M': 1e6, 'G': 1e9, 'T': 1e12, '': 1.0
}

FLOAT_TOKEN_RE = re.compile(r"^([+-]?[0-9]*\.?[0-9]+)([pnumkMGT]?)$")


def parse_value(token: str) -> Optional[float]:
    """Parse a numeric token possibly having a SI suffix (p, n, u, m, k, M, G, T)."""
    if token is None:
        return None
    token = token.strip()
    if token == "":
        return None

    m = FLOAT_TOKEN_RE.match(token)
    if not m:
        cleaned = re.sub(r"[^\d\.\+\-eE]", "", token)
        try:
            return float(cleaned)
        except Exception:
            return None

    mantissa = float(m.group(1))
    suffix = m.group(2)
    mul = UNIT_MULTIPLIERS.get(suffix, 1.0)
    return mantissa * mul


def read_voltage_column(path: str) -> List[float]:
    """Read input text file and return list of voltage values (last token per non-empty line)."""
    if not os.path.isfile(path):
        raise FileNotFoundError(f"Input file not found: {path}")

    voltages: List[float] = []
    with open(path, "r", encoding="utf-8") as fh:
        for raw in fh:
            line = raw.strip()
            if not line:
                continue
            if line.lower().startswith("time"):
                continue
            parts = line.split()
            if len(parts) < 2:
                continue
            v = parse_value(parts[-1])
            if v is None:
                v = parse_value(parts[1] if len(parts) > 1 else "")
            if v is None:
                continue
            voltages.append(v)
    return voltages


def quantize_voltage(v: float, vref: float, bits: int, gain: float = 1.0) -> int:
    """Quantize a single voltage into an integer code (unipolar 0..(2^bits-1))."""
    v_scaled = gain * v
    v_clamped = max(0.0, min(vref, v_scaled))
    max_code = (1 << bits) - 1
    code = int(round((v_clamped / vref) * max_code))
    if code < 0:
        code = 0
    if code > max_code:
        code = max_code
   
    return code


# -------------------------
# Main routine
# -------------------------
def main(argv=None):
    parser = argparse.ArgumentParser(description="ADC emulator for MAST (Spectre->reg.in).")
    parser.add_argument("-i", "--input", default=DEFAULT_INPUT, help="Input file (time value pairs).")
    parser.add_argument("-o", "--output", default=DEFAULT_OUTPUT, help="Output reg.in file (binary codes).")
    parser.add_argument("--bits", type=int, default=DEFAULT_BITS, help="ADC resolution (bits).")
    parser.add_argument("--vref", type=float, default=DEFAULT_VREF,
                        help="Reference voltage. If omitted, auto-scaled to max(abs(samples))*1.05.")
    parser.add_argument("--gain", type=float, default=DEFAULT_GAIN, help="Optional pre-gain applied to samples.")
    args = parser.parse_args(argv)

    try:
        voltages = read_voltage_column(args.input)
    except FileNotFoundError as e:
        print(f"[ERROR] {e}", file=sys.stderr)
        sys.exit(2)

    if len(voltages) == 0:
        print("[ERROR] No voltages found in input file.", file=sys.stderr)
        sys.exit(3)

    vref = args.vref
    if vref is None:
        vmax = max(abs(x) for x in voltages)
        vref = 1.0 if vmax == 0 else vmax * 1.05

    bits = args.bits
    if bits <= 0 or bits > 64:
        print("[ERROR] bits must be between 1 and 64", file=sys.stderr)
        sys.exit(4)

    max_code = (1 << bits) - 1
    lsb = vref / max_code


    outdir = os.path.dirname(args.output)
    if outdir:
        os.makedirs(outdir, exist_ok=True)

    
    bin_lines = []
    neg_positions = []

    for i, v in enumerate(voltages):
        is_negative = v < 0
        if is_negative:
            neg_positions.append(i)
            v = abs(v)
        code = quantize_voltage(v, vref, bits, args.gain)
        binstr = format(code, f"0{bits}b")
        bin_lines.append(binstr)

   
    with open(args.output, "w", encoding="utf-8") as fout_bin:
        for i, line in enumerate(bin_lines):
            sign_bit = "1" if i in neg_positions else "0"
            fout_bin.write(sign_bit + line + "\n")

    print("[OK] Conversion finished.")


if __name__ == "__main__":
    main()
