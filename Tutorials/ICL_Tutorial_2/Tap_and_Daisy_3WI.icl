Module Tap_and_Daisy_3WI
{
  Attribute ACCESS_LINK_PROTOCOL_NAME = "Loopback";
  AccessLink TAP Of STD_1149_1_2001
  {
    BSDLEntity TOP;
    ijtag_en  // Instruction Name
    {
      ScanInterface { Daisy.scan_client; }
    }
  }

  CaptureEnPort CE; ShiftEnPort SE;
  UpdateEnPort UE; SelectPort SEL; ResetPort RST;
  TCKPort TCK;

  ScanInPort SI;
  ScanOutPort SO { Source WI3.SO; }

  Attribute PDL = "Test_Modes_1";
  Attribute PDL = "Test_Modes_2";
  Attribute PDL = "Test_Data";
  Instance Daisy Of Daisy_3WI {  }
}

