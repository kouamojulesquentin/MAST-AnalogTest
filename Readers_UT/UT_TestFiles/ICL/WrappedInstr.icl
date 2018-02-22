Module WrappedInstr
{
  ShiftEnPort   SE;
  CaptureEnPort CE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;

  ScanInPort  SI;
  ScanOutPort SO { Source reg8.SO;}

  ScanInterface scan_client
  {
    Port SI;
    Port SO;
    Port SEL;
  }

  Instance I1 Of Instrument
  {
    InputPort DI = reg8.DO;
  }

  Instance reg8 Of SReg
  {
    Parameter Size = 8;

    InputPort SI   = SI;
    InputPort DI   = I1.DO;
  }
}
