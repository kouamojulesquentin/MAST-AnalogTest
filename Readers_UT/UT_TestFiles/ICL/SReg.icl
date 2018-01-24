Module SReg
{
  Parameter MSB = 7;

  ScanInPort  SI;
  ScanOutPort SO      { Source SR[0];}

  DataInPort  DI[$MSB:0];
  DataOutPort DO[$MSB:0] { Source SR; }

  ScanInterface scan_client { Port SI; Port SO; }

  ScanRegister SR[$MSB:0]      { ScanInSource SI; ResetValue 'b0; }
}
