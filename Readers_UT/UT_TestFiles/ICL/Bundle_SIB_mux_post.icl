Module Top
{
  ScanInPort  SI;
  ScanOutPort SO
  {
    Source SIB.SO;
  }

  Instance SIB Of SIB_mux_post
  {
    InputPort SI     = SI;
    InputPort fromSO = SHUNT.SO;
  }

  Instance SHUNT Of Shunt
  {
    InputPort SI = SIB.toSI;
  }
}

Module Shunt
{
  ScanInPort  SI;
  ScanOutPort SO
  {
    Source SI;
  }
}

Module SIB_mux_post
{
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;

  ScanInPort    SI;
  ScanOutPort   SO { Source SIBmux; LaunchEdge Rising; }

  ScanInterface client {
  Port SI; Port CE; Port SE; Port UE;
  Port SEL; Port RST; Port TCK; Port SO;
  }

  ToCaptureEnPort toCE;
  ToShiftEnPort   toSE;
  ToUpdateEnPort  toUE;
  ToSelectPort    toSEL;
  ToResetPort     toRST;
  ToTCKPort       toTCK;

  ScanInPort      fromSO;
  ScanOutPort     toSI { Source SR; LaunchEdge Rising; }

  ScanInterface host {
  Port fromSO; Port toCE; Port toSE; Port toUE;
  Port toSEL; Port toRST; Port toTCK; Port toSI;
  }

  ScanRegister SR
  {
    ScanInSource  SI;
    CaptureSource SR;
    ResetValue    1'b0;
  }

  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SR;
    1'b1 : fromSO;
  }

}

