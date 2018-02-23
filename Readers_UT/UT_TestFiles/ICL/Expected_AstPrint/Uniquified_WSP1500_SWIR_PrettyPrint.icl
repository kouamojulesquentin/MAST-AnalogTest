NameSpace;
Module Instrument
{
  Alias enable = DI[7]
  {
    RefEnum YesNo;
  }
  Alias mode[3:0] = DI[6:5], DI[3:2]
  {
    RefEnum Modes;
  }
  Alias data[2:0] = DI[4], DI[1:0];
  Alias okay = DO[0]
  {
    RefEnum PassFail;
  }
  Alias done = DO[1]
  {
    RefEnum YesNo;
  }
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module SReg
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }
  ScanRegister SR[$Size - 1:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 2'b0;
  }
}
Module WSP1500_SWIR
{
  ScanInPort SI;
  ScanOutPort SO { Source IR_MUX; }
  Instance WIR Of UniquifiedModules::SReg__uniquified__1
  {
    Parameter Size = 2;
    InputPort SI = SWIR;
  }
  Instance WI1 Of UniquifiedModules::WrappedInstr__uniquified__2
  {
    InputPort SI = SWIR;
  }
  Instance WI2 Of UniquifiedModules::WrappedInstr__uniquified__5
  {
    InputPort SI = SWIR;
  }
  Instance WI3 Of UniquifiedModules::WrappedInstr__uniquified__8
  {
    InputPort SI = SWIR;
  }
  ScanMux DR_MUX SelectedBy WIR[1:0]
  {
    2'b00 : WBY;
    2'b01 : WI1.SO;
    2'b10 : WI2.SO;
    2'b11 : WI3.SO;
  }
  ScanMux IR_MUX SelectedBy SWIR
  {
    1'b0 : DR_MUX;
    1'b1 : WIR[0];
  }
  ScanRegister SWIR
  {
    ScanInSource SI;
    ResetValue 1'b1;
  }
  ScanRegister WBY
  {
    ScanInSource SWIR;
    ResetValue 1'b1;
  }
}
Module WrappedInstr
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of ::Instrument
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of ::SReg
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
NameSpace UniquifiedModules;
Module Instrument__uniquified__3
{
  Alias enable = DI[7]
  {
    RefEnum YesNo;
  }
  Alias mode[3:0] = DI[6:5], DI[3:2]
  {
    RefEnum Modes;
  }
  Alias data[2:0] = DI[4], DI[1:0];
  Alias okay = DO[0]
  {
    RefEnum PassFail;
  }
  Alias done = DO[1]
  {
    RefEnum YesNo;
  }
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module Instrument__uniquified__6
{
  Alias enable = DI[7]
  {
    RefEnum YesNo;
  }
  Alias mode[3:0] = DI[6:5], DI[3:2]
  {
    RefEnum Modes;
  }
  Alias data[2:0] = DI[4], DI[1:0];
  Alias okay = DO[0]
  {
    RefEnum PassFail;
  }
  Alias done = DO[1]
  {
    RefEnum YesNo;
  }
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module Instrument__uniquified__9
{
  Alias enable = DI[7]
  {
    RefEnum YesNo;
  }
  Alias mode[3:0] = DI[6:5], DI[3:2]
  {
    RefEnum Modes;
  }
  Alias data[2:0] = DI[4], DI[1:0];
  Alias okay = DO[0]
  {
    RefEnum PassFail;
  }
  Alias done = DO[1]
  {
    RefEnum YesNo;
  }
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module SReg__uniquified__1
{
  Parameter Size = 2;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[1:0];
  DataOutPort DO[1:0] { Source SR; }
  ScanRegister SR[1:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 2'b0;
  }
}
Module SReg__uniquified__10
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 2'b0;
  }
}
Module SReg__uniquified__4
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 2'b0;
  }
}
Module SReg__uniquified__7
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 2'b0;
  }
}
Module WrappedInstr__uniquified__2
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__3
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__4
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
Module WrappedInstr__uniquified__5
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__6
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__7
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
Module WrappedInstr__uniquified__8
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__9
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__10
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
