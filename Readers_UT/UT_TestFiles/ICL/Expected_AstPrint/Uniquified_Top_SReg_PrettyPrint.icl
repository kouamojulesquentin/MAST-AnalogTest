NameSpace;
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
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
Module TOP
{
  Attribute ACCESS_LINK_PROTOCOL_NAME = "Loopback";
  AccessLink TAP Of STD_1149_1_2001
  {
    BSDLEntity TOP;
    ijtag_en
    {
      ScanInterface { sreg.scan_client; }
    }
  }
  Instance sreg Of UniquifiedModules::SReg__uniquified__1
  {
    Attribute PDL = "Incr";
  }
}
NameSpace UniquifiedModules;
Module SReg__uniquified__1
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
