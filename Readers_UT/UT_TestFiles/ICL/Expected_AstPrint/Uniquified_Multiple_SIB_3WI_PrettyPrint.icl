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
Module Multiple_SIB_3WI
{
  ScanInPort SI;
  ScanOutPort SO { Source SIB3.SO; }
  Instance SIB1 Of UniquifiedModules::SIB_mux_pre__uniquified__1
  {
    InputPort SI = SI;
    InputPort fromSO = WI1.SO;
  }
  Instance SIB2 Of UniquifiedModules::SIB_mux_pre__uniquified__2
  {
    InputPort SI = SIB1.SO;
    InputPort fromSO = WI2.SO;
  }
  Instance SIB3 Of UniquifiedModules::SIB_mux_pre__uniquified__3
  {
    InputPort SI = SIB2.SO;
    InputPort fromSO = WI3.SO;
  }
  Instance WI1 Of UniquifiedModules::WrappedInstr__uniquified__4
  {
    InputPort SI = SIB1.toSI;
  }
  Instance WI2 Of UniquifiedModules::WrappedInstr__uniquified__7
  {
    InputPort SI = SIB2.toSI;
  }
  Instance WI3 Of UniquifiedModules::WrappedInstr__uniquified__10
  {
    InputPort SI = SIB3.toSI;
  }
}
Module SIB_mux_pre
{
  ScanInterface client
  {
    Port SI;
    Port SO;
  }
  ScanInterface host
  {
    Port fromSO;
    Port toSI;
  }
  ScanInPort SI;
  ScanInPort fromSO;
  ScanOutPort SO { Source SR; }
  ScanOutPort toSI { Source SI; }
  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SI;
    1'b1 : fromSO;
  }
  ScanRegister SR
  {
    ScanInSource SIBmux;
    ResetValue 1'b0;
  }
}
Module SReg
{
  Parameter Size = 8;
  LocalParameter MSB = $Size - 1;
  LocalParameter Middle = $MSB / 2;
  Alias SR_H = SR[$MSB:$Middle];
  Alias SR_L = SR[$Middle - 1:0];
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
NameSpace UniquifiedModules;
Module Instrument__uniquified__11
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
Module SIB_mux_pre__uniquified__1
{
  ScanInterface client
  {
    Port SI;
    Port SO;
  }
  ScanInterface host
  {
    Port fromSO;
    Port toSI;
  }
  ScanInPort SI;
  ScanInPort fromSO;
  ScanOutPort SO { Source SR; }
  ScanOutPort toSI { Source SI; }
  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SI;
    1'b1 : fromSO;
  }
  ScanRegister SR
  {
    ScanInSource SIBmux;
    ResetValue 1'b0;
  }
}
Module SIB_mux_pre__uniquified__2
{
  ScanInterface client
  {
    Port SI;
    Port SO;
  }
  ScanInterface host
  {
    Port fromSO;
    Port toSI;
  }
  ScanInPort SI;
  ScanInPort fromSO;
  ScanOutPort SO { Source SR; }
  ScanOutPort toSI { Source SI; }
  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SI;
    1'b1 : fromSO;
  }
  ScanRegister SR
  {
    ScanInSource SIBmux;
    ResetValue 1'b0;
  }
}
Module SIB_mux_pre__uniquified__3
{
  ScanInterface client
  {
    Port SI;
    Port SO;
  }
  ScanInterface host
  {
    Port fromSO;
    Port toSI;
  }
  ScanInPort SI;
  ScanInPort fromSO;
  ScanOutPort SO { Source SR; }
  ScanOutPort toSI { Source SI; }
  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SI;
    1'b1 : fromSO;
  }
  ScanRegister SR
  {
    ScanInSource SIBmux;
    ResetValue 1'b0;
  }
}
Module SReg__uniquified__12
{
  Parameter Size = 8;
  LocalParameter MSB = 8 - 1;
  LocalParameter Middle = (8 - 1) / 2;
  Alias SR_H = SR[7:3];
  Alias SR_L = SR[2:0];
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
    ResetValue 'b0;
  }
}
Module SReg__uniquified__6
{
  Parameter Size = 8;
  LocalParameter MSB = 8 - 1;
  LocalParameter Middle = (8 - 1) / 2;
  Alias SR_H = SR[7:3];
  Alias SR_L = SR[2:0];
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
    ResetValue 'b0;
  }
}
Module SReg__uniquified__9
{
  Parameter Size = 8;
  LocalParameter MSB = 8 - 1;
  LocalParameter Middle = (8 - 1) / 2;
  Alias SR_H = SR[7:3];
  Alias SR_L = SR[2:0];
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
    ResetValue 'b0;
  }
}
Module WrappedInstr__uniquified__10
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__11
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__12
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
