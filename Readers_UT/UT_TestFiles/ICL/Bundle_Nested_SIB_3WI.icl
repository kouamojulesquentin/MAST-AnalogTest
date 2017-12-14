Module Nested_SIB_3WI
{
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;
  ScanInPort    SI;
  ScanOutPort   SO { Source SIB1.SO; }

  Instance SIB1 Of SIB_mux_pre { InputPort SI     = SI;
                                 InputPort fromSO = SIB2.SO;   }
  Instance SIB2 Of SIB_mux_pre { InputPort SI     = WI1.SO;
                                 InputPort fromSO = SIB3.SO;   }
  Instance SIB3 Of SIB_mux_pre { InputPort SI     = WI2.SO;
                                 InputPort fromSO = WI3.SO;    }
  Instance WI1 Of WrappedInstr { InputPort SI     = SIB1.toSI; }
  Instance WI2 Of WrappedInstr { InputPort SI     = SIB2.toSI; }
  Instance WI3 Of WrappedInstr { InputPort SI     = SIB3.toSI; }
}

Module WrappedInstr {
  ScanInPort    SI;
  ScanOutPort   SO { Source reg8.SO;}
  ScanInterface scan_client { Port SI; Port SO; }

  Instance I1   Of Instrument { InputPort DI = reg8.DO; }
  Instance reg8 Of SReg       { InputPort SI = SI;      }
}

Module SReg
{
  ScanInPort    SI;
  ScanOutPort   SO          { Source  SR[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }
}

Module Instrument
{
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}

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
