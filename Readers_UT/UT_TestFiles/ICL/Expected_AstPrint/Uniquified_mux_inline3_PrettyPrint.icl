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
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }
  ScanRegister SR[$Size - 1:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
Module WrappedInstr
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
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
Module mux_inline3
{
  ScanInPort SI;
  ScanOutPort SO { Source reg3.SO; }
  Instance WI1 Of UniquifiedModules::WrappedInstr__uniquified__1
  {
    InputPort SI = SI;
  }
  Instance WI2 Of UniquifiedModules::WrappedInstr__uniquified__4
  {
    InputPort SI = mux1;
  }
  Instance WI3 Of UniquifiedModules::WrappedInstr__uniquified__7
  {
    InputPort SI = mux2;
  }
  Instance reg3 Of UniquifiedModules::SReg__uniquified__10
  {
    Parameter Size = 3;
    InputPort SI = mux3;
    InputPort DI = 'b0;
  }
  ScanMux mux1 SelectedBy reg3.DO[2]
  {
    1'b0 : SI;
    1'b1 : WI1.SO;
  }
  ScanMux mux2 SelectedBy reg3.DO[1]
  {
    1'b0 : mux1;
    1'b1 : WI2.SO;
  }
  ScanMux mux3 SelectedBy reg3.DO[0]
  {
    1'b0 : mux2;
    1'b1 : WI3.SO;
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
Module SReg__uniquified__10
{
  Parameter Size = 3;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[2:0];
  DataOutPort DO[2:0] { Source SR; }
  ScanRegister SR[2:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
Module SReg__uniquified__3
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
Module SReg__uniquified__6
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
Module SReg__uniquified__9
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
Module WrappedInstr__uniquified__1
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
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
