Module Daisy_3WI
{
  CaptureEnPort CE; ShiftEnPort SE;
  UpdateEnPort UE; SelectPort SEL; ResetPort RST;
  TCKPort TCK;

  ScanInPort SI;
  ScanOutPort SO { Source WI3.SO; }

  ScanInterface scan_client { Port SI; Port SO; }

  Instance WI1 Of WrappedInstr { InputPort SI = SI; }
  Instance WI2 Of WrappedInstr { InputPort SI = WI1.SO; }
  Instance WI3 Of WrappedInstr { InputPort SI = WI2.SO; }
}

