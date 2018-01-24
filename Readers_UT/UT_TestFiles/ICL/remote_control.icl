Module remote_control
{
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;

  ScanInPort    SI;
  ScanOutPort   SO { Source reg3.SO; }

  ScanInterface scan_client { Port SI; Port SO; }

  DataOutPort RS[2:0]
  {
    Source reg3.DO[2:0];
  }

  Instance reg3 Of SReg
  {
    Parameter MSB = 2;
    InputPort SI = SI;
  }
}
