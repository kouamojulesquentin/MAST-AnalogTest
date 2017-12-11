Module TOP
{
  Instance sreg Of SReg
  {
    Attribute PDL = "Incr";
  }

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
  ScanInPort    SI;
  ScanOutPort   SO          { Source  SR[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }
}
