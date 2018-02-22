Module SReg
{
  Parameter Size = 8;

  ShiftEnPort   SE;
  CaptureEnPort CE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;

  ScanInPort  SI;
  ScanOutPort SO { Source SR[0]; }

  DataInPort  DI[$Size-1:0];
  DataOutPort DO[$Size-1:0] { Source SR; }

  ScanInterface scan_client { Port SI; Port SO; Port SEL; }

  ScanRegister SR[$Size-1:0]
  {
    ScanInSource  SI;
    CaptureSource DI;
    ResetValue    $Size'b0;
  }
}
