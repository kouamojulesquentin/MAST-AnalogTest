Module des_unit {
  ScanInPort     WSI;
  ScanOutPort    WSO { Source SELECTWIR; }
  ShiftEnPort    SHIFTWR;
  CaptureEnPort  CAPTUREWR;
  UpdateEnPort   UPDATEWR;
  ResetPort      WRSTN { ActivePolarity 0; }
  TCKPort        WRCK;

  Attribute PDL = "PDL_routine";

  ScanInterface scan_client { Port WSI; Port WSO; }

  ScanRegister SELECTWIR { ScanInSource mux4; }

  ScanRegister mask_all { ScanInSource WSI; }
  ScanRegister shift_cnt[15:0] { ScanInSource mask_all; }
  ScanRegister shift_cnt_shift[15:0] { ScanInSource mux1; }
  ScanRegister seed_dest { ScanInSource shift_cnt_shift[0]; }
  ScanRegister cap_ena_shift { ScanInSource seed_dest; }
  ScanRegister cap_ena { ScanInSource cap_ena_shift; }
  ScanRegister pwr_ena[2:0] { ScanInSource cap_ena; }
  ScanRegister shadow[101:0] { ScanInSource mux2; }
  ScanRegister care_prpg[100:0] { ScanInSource shadow[0]; }
  ScanRegister x_prpg[100:0] { ScanInSource care_prpg[0]; }
  ScanRegister care_phase_shifter[100:0] { ScanInSource x_prpg[0]; }
  ScanRegister first_shift { ScanInSource care_phase_shifter[0]; }
  ScanRegister xtol_x_phase_shifter[9:0] { ScanInSource first_shift; }
  ScanRegister xtol_phase_shifter[9:0] { ScanInSource xtol_x_phase_shifter[0]; }
  ScanRegister xtol_ena { ScanInSource xtol_phase_shifter[0]; }
  ScanRegister MISR[31:0] { ScanInSource mux3; }
  ScanRegister WBY { ScanInSource WSI; }
  ScanRegister PATTERN_COUNTER[15:0] { ScanInSource WSI; }
  ScanRegister OCC[11:0] { ScanInSource WSI; }
  ScanRegister CDR[15:0] { ScanInSource WSI; }
  ScanRegister WIR[3:0] { ScanInSource WSI; }
  ScanRegister WDR[119:0] { ScanInSource WSI; }

  Alias CODEC_DRC[497:0] = mask_all, shift_cnt[15:0], shift_cnt_shift[15:0], seed_dest, cap_ena_shift, cap_ena, pwr_ena[2:0], shadow[101:0], care_prpg[100:0], x_prpg[100:0], care_phase_shifter[100:0], first_shift, xtol_x_phase_shifter[9:0], xtol_phase_shifter[9:0], xtol_ena, MISR[31:0] { AccessTogether; }

  ScanMux mux1 SelectedBy WIR[3:0] {
    4'b1000  : shift_cnt[0];
    ~4'b1000 : WSI;
  }

  ScanMux mux2 SelectedBy WIR[3:0] {
    4'b1000  : pwr_ena[0];
    ~4'b1000 : cap_ena_shift;
  }

  ScanMux mux3 SelectedBy WIR[3:0] {
    4'b1000  : xtol_ena;
    ~4'b1000 : WSI;
  }

  ScanMux mux0 SelectedBy WIR[3:0] {
    4'b1000  : MISR[0];
    4'b1010  : PATTERN_COUNTER[0];
    4'b0100  : OCC[0];
    4'b0010  : WDR[0];
    4'b1100  : MISR[0];
    4'b0001  : CDR[0];
    4'b0000  : WBY;
  }

  ScanMux mux4 SelectedBy SELECTWIR {
    1'b0 : mux0;
    1'b1 : WIR[0];
  }
}
