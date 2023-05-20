Module SReg
{
  Parameter Size = 8;
  ScanInPort    SI;
  ScanOutPort   SO          { Source  reg[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister reg[$Size - 1:0]    { ScanInSource SI; ResetValue 'hABC; }
}
