NameSpace;
Module SReg
{
  Parameter Size = 8;
  ScanInterface scan_client
  {
    Port SI;
    Port SO;
  }
  ScanInPort SI;
  ScanOutPort SO { Source SR[0]; }
  DataInPort DI[8 - 1:0];
  DataOutPort DO[8 - 1:0] { Source SR; }
  ScanRegister SR[8 - 1:0]
  {
    ScanInSource SI;
    ResetValue 'b0;
  }
}
