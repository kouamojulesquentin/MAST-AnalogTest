Module Tap_and_Daisy_3WI
{
  Attribute ACCESS_LINK_PROTOCOL_NAME = "SVF_Emulation";
  AccessLink TAP Of STD_1149_1_2001
  {
    BSDLEntity TOP;
    ijtag_en  // Instruction Name
    {
      ScanInterface { des_instance.scan_client; }
    }
  }

  CaptureEnPort CE; ShiftEnPort SE;
  UpdateEnPort UE; SelectPort SEL; ResetPort RST;
  TCKPort TCK;

  ScanInPort SI;
  ScanOutPort SO { Source WI3.SO; }

  Instance des_instance Of des_unit {  }
}
