Module TOP
{
  Instance sreg Of SReg
  {
    Attribute PDL = "Incr";
  }

  Attribute ACCESS_LINK_PROTOCOL_NAME = "Loopback";
  AccessLink TAP Of STD_1149_1_2001
  {
    BSDLEntity TOP;
    ijtag_en  // Instruction Name
    {
      ScanInterface { sreg.scan_client; }
    }
  }
}

Module SReg
{
  Parameter      Size   = 8;
  LocalParameter MSB    = $Size - 1;
  LocalParameter Middle = $MSB / 2;

  Alias SR_H = SR[$MSB:$Middle];  // Theses aliases are just there to check unification process related to aliases
  Alias SR_L = SR[$Middle - 1:0]; // SR must be at least 2 bits wide !

  DataInPort  DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }

  ScanInPort    SI;
  ScanOutPort   SO          { Source  SR[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; CaptureSource DI;}
}
