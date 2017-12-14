
Module SIB_mux_pre {
  ScanInPort     SI;
  ScanOutPort    SO { Source SR; }
  ScanInterface client {
    Port SI;
    Port SO;
  }
  ScanInPort       fromSO;
  ScanOutPort      toSI { Source SI; }
  ScanInterface host {
    Port fromSO;
    Port toSI;
  }
  ScanRegister SR {
    ScanInSource SIBmux; CaptureSource SR; ResetValue 1'b0;
  }
  ScanMux SIBmux SelectedBy SR {
    1'b0 : SI;
    1'b1 : fromSO;
  }
}
