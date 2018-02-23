Module Daisy_3WI {
  CaptureEnPort CE; ShiftEnPort SE;
  UpdateEnPort UE; SelectPort SEL; ResetPort RST;
  TCKPort TCK;

  ScanInPort SI;
  ScanOutPort SO { Source WI3.SO; }

  Attribute PDL = "Algo_top_1";
  Attribute PDL = "Algo_top_2";

  Instance WI1 Of WrappedInstr { InputPort SI = SI;     Attribute PDL = "Algo_WI1";}
  Instance WI2 Of WrappedInstr { InputPort SI = WI1.SO; Attribute PDL = "Algo_WI2";}
  Instance WI3 Of WrappedInstr { InputPort SI = WI2.SO; Attribute PDL = "Algo_WI3";}
}

Module WrappedInstr {
  ScanInPort    SI;
  ScanOutPort   SO { Source reg8.SO;}
  ScanInterface scan_client { Port SI; Port SO; }

  Attribute PDL = "Algo_WI";

  Instance I1 Of Instrument
  {
    Attribute PDL = "Algo_WI_I1";
    InputPort DI  = reg8.DO;
  }

  Instance reg8 Of SReg
  {
    Parameter Size = 8;
    Attribute PDL = "Algo_reg8";
    InputPort SI   = SI;
    InputPort DI   = I1.DO;
  }

}

Module SReg
{
  Parameter Size = 8;

  Attribute PDL = "Algo_SReg";

  DataInPort  DI[$Size - 1:0];
  DataOutPort DO[$Size - 1:0] { Source SR; }

  ScanInPort    SI;
  ScanOutPort   SO          { Source  SR[0];}
  ScanInterface scan_client { Port SI; Port SO; }
  ScanRegister SR[7:0]      { ScanInSource SI; ResetValue 'b0; CaptureSource DI; }
}

Module Instrument
{
  Alias enable    = DI[7]            { RefEnum YesNo; }
  Alias mode[3:0] = DI[6:5], DI[3:2] { RefEnum Modes; }
  Alias data[2:0] = DI[4],   DI[1:0];
  Alias okay      = DO[0]            { RefEnum PassFail; }
  Alias done      = DO[1]            { RefEnum YesNo; }

  Attribute PDL = "Algo_Instrument_1";
  Attribute PDL = "Algo_Instrument_2";

  Enum PassFail
  {
    Pass = 1'b1;
    Fail = 1'b0;
  }

  Enum YesNo
  {
    Yes = 1'b1;
    No  = 1'b0;
  }

  Enum Modes
  {
    red   = 4'b0011;
    blue  = 4'b1000;
    green = 4'b0100;
  }
  DataInPort DI[7:0];
  DataOutPort DO[7:0];
}

