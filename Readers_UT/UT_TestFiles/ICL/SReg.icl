Module SReg
{
  Parameter      Size   = 8;
  LocalParameter MSB    = $Size - 1;
  LocalParameter Middle = $MSB / 2;

  Alias SR_H = SR[$MSB:$Middle];  // Theses aliases are just there to check unification process related to aliases
  Alias SR_L = SR[$Middle - 1:0]; // SR must be at least 2 bits wide !

  ScanInPort  SI;
  ScanOutPort SO  { Source SR[0];}

  DataInPort  DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }

  ScanInterface scan_client { Port SI; Port SO; }

  ScanRegister SR[$Size - 1:0] { ScanInSource SI; ResetValue 'b0; }
}
