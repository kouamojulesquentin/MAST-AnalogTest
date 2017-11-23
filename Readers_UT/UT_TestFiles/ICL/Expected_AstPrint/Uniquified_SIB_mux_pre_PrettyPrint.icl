NameSpace;
Module SIB_mux_pre
{
  ScanInterface client
  {
    Port SI;
    Port CE;
    Port SE;
    Port UE;
    Port SEL;
    Port RST;
    Port TCK;
    Port SO;
  }
  ScanInterface host
  {
    Port fromSO;
    Port toCE;
    Port toSE;
    Port toUE;
    Port toSEL;
    Port toRST;
    Port toTCK;
    Port toSI;
  }
  ScanInPort SI;
  ScanInPort fromSO;
  ScanOutPort SO { Source SR; }
  ScanOutPort toSI { Source SI; }
  ScanMux SIBmux SelectedBy SR
  {
    1'b0 : SI;
    1'b1 : fromSO;
  }
  ScanRegister SR
  {
    ScanInSource SIBmux;
    ResetValue 1'b0;
  }
}
