Module SReg
{
  Parameter   Size   = 8;
  ScanInPort  SI;
  ScanOutPort SO  { Source SR[0];}

  DataInPort  DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }

  ScanInterface scan_client { Port SI; Port SO; }

  ScanRegister SR[$Size - 1:0]
  {
    ScanInSource  SI;
    CaptureSource DI;
    ResetValue    'b0;
  }

}
