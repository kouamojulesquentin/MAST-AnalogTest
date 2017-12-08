Module top
{
  Attribute PDL = "Topless";
  ScanInPort    SI;
  ScanOutPort   SO { Source SR_2[0];}
  ScanRegister  SR_2[7:0]
  {
    ScanInSource  inst_1.bso;
    ResetValue    8'b0001_0010;
  }
  ScanRegister  SR_1[4:0]
  {
    ScanInSource  inst_2.midout;
    ResetValue    5'b1_0001;
  }
  Instance inst_1 Of bottom
  {
    Attribute PDL = "Bool";
    InputPort bsi = SR_1[0];
  }
  Instance inst_2 Of middle
  {
    Attribute PDL = "Picus";
    Attribute PDL = "Logica";
    InputPort midin = SI;
  }
}

Module bottom
{
  Attribute PDL = "Bat";
  Attribute PDL = "Boot";

  ScanInPort    bsi;
  ScanOutPort   bso { Source reg_2[0];}
  ScanRegister  reg_1[7:0]
  {
    ScanInSource  bsi;
    ResetValue    8'b0010_0001;
  }
  ScanRegister  reg_2[6:0]
  {
    ScanInSource  reg_1[0];
    ResetValue    7'b010_0010;
  }
}

Module middle
{
  Attribute PDL = "Mast";
  Attribute PDL = "Picus";

  ScanInPort    midin;
  ScanOutPort   midout { Source mreg_2[0];}
  Instance inst Of bottom
  {
    InputPort bsi = mreg_1[0];
  }
  ScanRegister  mreg_1[7:0]
  {
    ScanInSource  midin;
    ResetValue    8'b0010_0001;
  }
  ScanRegister  mreg_2[6:0]
  {
    ScanInSource  inst.bso;
    ResetValue    7'b010_0010;
  }
}
