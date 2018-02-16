Module Single_SIB_3WI {
  ScanInPort SI;
  ScanOutPort SO { Source SIB1.SO; }

  Alias WI_MSB = WI3.reg8.SR[7], WI2.reg8.SR[7], WI1.reg8.SR[7];
  Alias Bundle = WI3.reg8.SR, WI2.reg8.SR, WI1.reg8.SR;

  Instance SIB1 Of SIB_mux_pre { InputPort SI = SI;
                                 InputPort fromSO = WI3.SO; }
  Instance WI1 Of WrappedInstr { InputPort SI = SIB1.toSI; }
  Instance WI2 Of WrappedInstr { InputPort SI = WI1.SO; }
  Instance WI3 Of WrappedInstr { InputPort SI = WI2.SO; }
}

Module WrappedInstr {
  ScanInPort    SI;
  ScanOutPort   SO { Source reg8.SO;}
  ScanInterface scan_client { Port SI; Port SO; }

  Instance I1   Of Instrument { InputPort DI = reg8.DO; }
  Instance reg8 Of SReg       { InputPort SI = SI; InputPort DI = I1.DO; Parameter Size = 8; }
}

Module SReg
{
  Parameter      Size   = 8;
  LocalParameter MSB    = $Size - 1;
  LocalParameter Middle = $MSB / 2;

  Alias SR_H = SR[$MSB:$Middle];  // Theses aliases are just there to check unification process related to aliases
  Alias SR_L = SR[$Middle - 1:0]; // SR must be at least 2 bits wide !

  ScanInPort  SI;
  ScanOutPort SO { Source SR[0];}

  DataInPort  DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }

  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }
}

Module Instrument
{
  Alias enable    = DI[7]            { RefEnum YesNo; }
  Alias mode[3:0] = DI[6:5], DI[3:2] { RefEnum Modes; }
  Alias data[2:0] = DI[4],   DI[1:0];
  Alias okay      = DO[0]            { RefEnum PassFail; }
  Alias done      = DO[1]            { RefEnum YesNo; }

  Enum PassFail
  {
    Pass = 1'b1;
    Fail = 1'b0;
  }

  Enum YesNo
  {
    Yes = 1'b1;
    No  = 1'b0;
  }

  Enum Modes
  {
    red   = 4'b0011;
    blue  = 4'b1000;
    green = 4'b0100;
  }

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
