Module SReg
{
  ScanInPort  SI;
  ScanOutPort SO      { Source SR[0];}

  DataInPort  DI[7:0];
  DataOutPort DO[7:0] { Source SR; }

  ScanInterface scan_client { Port SI; Port SO; }

  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; }
}
