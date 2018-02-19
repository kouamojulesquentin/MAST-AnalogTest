NameSpace;
Module SReg
{
  Parameter Size = 8;
  LocalParameter MSB = 8 - 1;
  LocalParameter Middle = (8 - 1) / 2;
  Alias SR_H = SR[7:3];
  Alias SR_L = SR[2:0];
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[7:0];
  DataOutPort DO[7:0] { Source SR; }
  ScanRegister SR[7:0]
  {
    ScanInSource SI;
    CaptureSource DI;
    ResetValue 'b0;
  }
}
