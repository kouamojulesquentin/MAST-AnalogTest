Module TOP_mux_remote3
{
  Instance mr Of mux_remote3
  {
    InputPort RS = rc.RS;
    //+ How to connect tap ? Is it implicit ?
  }

  Instance rc Of remote_control
  {
    //+ How to connect tap ? Is it implicit ?
  }

  Attribute ACCESS_LINK_PROTOCOL_NAME = "Loopback";
  AccessLink TAP Of STD_1149_1_2001
  {
    BSDLEntity TOP;
    ijtag_en  // Instruction Name
    {
      ScanInterface { rc.scan_client; }
      ScanInterface { mr.scan_client; }
    }
  }
}
