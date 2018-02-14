NameSpace;
Module Instrument
{
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
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
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    ResetValue 'b0;
  }
}
Module Single_SIB_3WI
{
  ScanInPort SI;
  ScanOutPort SO { Source SIB1.SO; }
  Instance SIB1 Of UniquifiedModules::SIB_mux_pre__uniquified__1
  {
    InputPort SI = SI;
    InputPort fromSO = WI3.SO;
  }
  Instance WI1 Of UniquifiedModules::WrappedInstr__uniquified__2
  {
    InputPort SI = SIB1.toSI;
  }
  Instance WI2 Of UniquifiedModules::WrappedInstr__uniquified__5
  {
    InputPort SI = WI1.SO;
  }
  Instance WI3 Of UniquifiedModules::WrappedInstr__uniquified__8
  {
    InputPort SI = WI2.SO;
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
Module Instrument__uniquified__3
{
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module Instrument__uniquified__6
{
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}
Module Instrument__uniquified__9
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
Module SReg__uniquified__10
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
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    ResetValue 'b0;
  }
}
Module SReg__uniquified__4
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
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    ResetValue 'b0;
  }
}
Module SReg__uniquified__7
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
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    ResetValue 'b0;
  }
}
Module WrappedInstr__uniquified__2
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__3
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__4
  {
    InputPort SI = SI;
  }
}
Module WrappedInstr__uniquified__5
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__6
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__7
  {
    InputPort SI = SI;
  }
}
Module WrappedInstr__uniquified__8
{
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source reg8.SO; }
  Instance I1 Of UniquifiedModules::Instrument__uniquified__9
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__10
  {
    InputPort SI = SI;
  }
}
