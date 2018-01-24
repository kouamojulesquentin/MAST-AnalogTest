NameSpace;
Module Instrument
{
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
  Parameter MSB = 7;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[$MSB:0];
  DataOutPort DO[$MSB:0] { Source SR; }
  ScanRegister SR[$MSB:0]
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
    InputPort SI = SI;
  }
}
NameSpace UniquifiedModules;
Module Instrument__uniquified__11
{
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module Instrument__uniquified__5
{
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module Instrument__uniquified__8
{
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
  Parameter MSB = 7;
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
  Parameter MSB = 7;
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
  Parameter MSB = 7;
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
    InputPort SI = SI;
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
    InputPort SI = SI;
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
    InputPort SI = SI;
  }
}
