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

