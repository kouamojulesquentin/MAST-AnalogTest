NameSpace;
Module Exclusive
{
  ScanInPort SI;
  ScanOutPort SO { Source reg2[0]; }
  Instance WI1 Of UniquifiedModules::WrappedInstr__uniquified__1
  {
    InputPort SI = SI;
  }
  Instance WI2 Of UniquifiedModules::WrappedInstr__uniquified__4
  {
    InputPort SI = SI;
  }
  Instance WI3 Of UniquifiedModules::WrappedInstr__uniquified__7
  {
    InputPort SI = SI;
  }
  ScanMux M4 SelectedBy reg2
  {
    2'b00 : SI;
    2'b01 : WI1.SO;
    2'b10 : WI2.SO;
    2'b11 : WI3.SO;
  }
  ScanRegister reg2[1:0]
  {
    ScanInSource M4;
    CaptureSource 2'b00;
    ResetValue 2'b00;
  }
}
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
    ResetValue 8'b0;
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
Module Instrument__uniquified__2
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
Module Instrument__uniquified__5
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
Module Instrument__uniquified__8
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
Module SReg__uniquified__3
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
    ResetValue 8'b0;
  }
}
Module SReg__uniquified__6
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
    ResetValue 8'b0;
  }
}
Module SReg__uniquified__9
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
    ResetValue 8'b0;
  }
}
Module WrappedInstr__uniquified__1
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__2
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__3
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
Module WrappedInstr__uniquified__4
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__5
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__6
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
Module WrappedInstr__uniquified__7
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__8
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__9
  {
    Parameter Size = 8;
    InputPort SI = SI;
    InputPort DI = I1.DO;
  }
}
