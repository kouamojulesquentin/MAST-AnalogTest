NameSpace;
Module SReg
{
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
Module TOP
{
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
