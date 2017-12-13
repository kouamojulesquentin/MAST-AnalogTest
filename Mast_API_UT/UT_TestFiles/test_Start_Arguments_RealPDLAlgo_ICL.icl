Module TOP
{
  Instance SReg_I Of SReg
  {
    Attribute PDL = "Incr";
  }

  Attribute ACCESS_LINK_PROTOCOL_NAME = "Loopback";
  AccessLink TAP Of STD_1149_1_2001
  {
    BSDLEntity TOP;
    ijtag_en  // Instruction Name
    {
      ScanInterface { SReg_I.scan_client; }
    }
  }
}

Module SReg
{
  ScanInPort    SI;
  ScanOutPort   SO          { Source  reg[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister reg[11:0]      { ScanInSource SI; ResetValue 'hABC; }
}
