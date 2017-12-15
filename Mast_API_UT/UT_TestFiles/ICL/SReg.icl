Module SReg
{
  ScanInPort    SI;
  ScanOutPort   SO          { Source  reg[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister reg[11:0]    { ScanInSource SI; ResetValue 'b0; }
}
