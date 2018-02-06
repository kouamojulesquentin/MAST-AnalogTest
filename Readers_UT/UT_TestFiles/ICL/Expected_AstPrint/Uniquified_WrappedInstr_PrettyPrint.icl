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
  Instance I1 Of UniquifiedModules::Instrument__uniquified__1
  {
    InputPort DI = reg8.DO;
  }
  Instance reg8 Of UniquifiedModules::SReg__uniquified__2
  {
    InputPort SI = SI;
  }
}
NameSpace UniquifiedModules;
Module Instrument__uniquified__1
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
Module SReg__uniquified__2
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
