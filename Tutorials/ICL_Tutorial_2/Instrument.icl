Module Instrument
{
  Alias enable    = DI[7]            { RefEnum YesNo; }
  Alias mode[3:0] = DI[6:5], DI[3:2] { RefEnum Modes; }
  Alias data[2:0] = DI[4],   DI[1:0];
  Alias okay      = DO[0]            { RefEnum PassFail; }
  Alias done      = DO[1]            { RefEnum YesNo; }

  Attribute PDL = "Test_Modes";

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

