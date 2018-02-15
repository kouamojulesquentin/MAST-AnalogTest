Module WrappedInstr
{
  // Aliases and enums are moved (and adapted) from Instrument.icl because there is no parent node constructed for Instrument
  //
  Alias enable    = reg8.reg[7]    { RefEnum YesNo; }
  Alias mode[3:0] = reg8.reg[6:5], reg8.reg[3:2] { RefEnum   Modes; }
  Alias data[2:0] = reg8.reg[4],   reg8.reg[1:0];
  Alias okay      = reg8.reg[0]    { RefEnum PassFail; }
  Alias done      = reg8.reg[1]    { RefEnum YesNo;    }

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


  ScanInPort    SI;
  ScanOutPort   SO { Source reg8.SO;}
  ScanInterface scan_client { Port SI; Port SO; }

  Instance I1   Of Instrument { InputPort DI = reg8.DO; }
  Instance reg8 Of SReg       { InputPort SI = SI;      }
}


