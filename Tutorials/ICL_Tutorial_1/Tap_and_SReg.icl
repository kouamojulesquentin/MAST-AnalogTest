Module TOP
{
  Instance SReg_I Of SReg
  {
    Parameter Size = 12;
    Attribute PDL  = "Incr";
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

