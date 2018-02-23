// IEEE-1687 example 20: IEEE-1500 Wrapper Serial Port with Select WIR register
//
Module WSP1500_SWIR
{
  TCKPort       TCK;
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;

  ScanInPort SI;
  ScanOutPort SO { Source IR_MUX; }

  ScanRegister SWIR
  {
    ScanInSource SI;
    ResetValue   1'b1;
  }

  ScanRegister WBY
  {
    ScanInSource SWIR;
    ResetValue   1'b1;
  }

  Instance WIR Of SReg
  {
    Parameter Size = 2;
    InputPort SI   = SWIR;
  }

  Instance WI1 Of WrappedInstr { InputPort SI = SWIR; }
  Instance WI2 Of WrappedInstr { InputPort SI = SWIR; }
  Instance WI3 Of WrappedInstr { InputPort SI = SWIR; }

  ScanMux DR_MUX SelectedBy WIR[1:0]
  {
    2'b00 : WBY;
    2'b01 : WI1.SO;
    2'b10 : WI2.SO;
    2'b11 : WI3.SO;
  }

  ScanMux IR_MUX SelectedBy SWIR
  {
    1'b0 : DR_MUX;
    1'b1 : WIR[0];
  }
}
