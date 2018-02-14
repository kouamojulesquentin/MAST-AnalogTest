Module Daisy_3WI {
  CaptureEnPort CE; ShiftEnPort SE;
  UpdateEnPort UE; SelectPort SEL; ResetPort RST;
  TCKPort TCK;

  ScanInPort SI;
  ScanOutPort SO { Source WI3.SO; }

  Instance WI1 Of WrappedInstr { InputPort SI = SI; }
  Instance WI2 Of WrappedInstr { InputPort SI = WI1.SO; }
  Instance WI3 Of WrappedInstr { InputPort SI = WI2.SO; }
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
  Parameter      Size   = 8;
  LocalParameter MSB    = $Size - 1;
  LocalParameter Middle = $MSB / 2;

  Alias SR_H = SR[$MSB:$Middle];  // Theses aliases are just there to check unification process related to aliases
  Alias SR_L = SR[$Middle - 1:0]; // SR must be at least 2 bits wide !

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

