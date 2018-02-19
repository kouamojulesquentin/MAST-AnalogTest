Module BAD_Nested_SIB_3WI
{
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;
  ScanInPort    SI;
  ScanOutPort   SO { Source SIB1.SO;}

  Instance SIB1 Of SIB_mux_post { InputPort SI     = SI;
                                  InputPort fromSO = SIB2.SO; }
  Instance SIB2 Of SIB_mux_post { InputPort SI     = WI1.SO;
                                  InputPort fromSO = SIB3.SO; }
  Instance SIB3 Of SIB_mux_post { InputPort SI     = WI2.SO;
                                  InputPort fromSO = WI3.SO;  }

  Instance WI1 Of WrappedInstr { InputPort SI = SIB1.toSI; }
  Instance WI2 Of WrappedInstr { InputPort SI = SIB2.toSI; }
  Instance WI3 Of WrappedInstr { InputPort SI = SIB3.toSI; }
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

  DataInPort  DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }

  ScanInPort    SI;
  ScanOutPort   SO          { Source  SR[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; CaptureSource DI;}
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

Module SIB_mux_post
{
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;

  ScanInPort    SI;
  ScanOutPort   SO { Source SIBmux; LaunchEdge Rising; }

  ScanInterface client {
  Port SI; Port CE; Port SE; Port UE;
  Port SEL; Port RST; Port TCK; Port SO;
  }

  ToCaptureEnPort toCE;
  ToShiftEnPort   toSE;
  ToUpdateEnPort  toUE;
  ToSelectPort    toSEL;
  ToResetPort     toRST;
  ToTCKPort       toTCK;

  ScanInPort      fromSO;
  ScanOutPort     toSI { Source SR; LaunchEdge Rising; }

  ScanInterface host {
  Port fromSO; Port toCE; Port toSE; Port toUE;
  Port toSEL; Port toRST; Port toTCK; Port toSI;
  }

  ScanRegister SR
  {
    ScanInSource  SI;
    CaptureSource SR;
    ResetValue    1'b0;
  }

  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SR;
    1'b1 : fromSO;
  }
}
