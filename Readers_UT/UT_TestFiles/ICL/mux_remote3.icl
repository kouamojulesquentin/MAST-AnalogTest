Module mux_remote3
{
  CaptureEnPort CE;
  ShiftEnPort   SE;
  UpdateEnPort  UE;
  SelectPort    SEL;
  ResetPort     RST;
  TCKPort       TCK;

  DataInPort    RS[2:0];
  ScanInPort    SI;
  ScanOutPort   SO { Source mux3; }

  ScanInterface scan_client { Port SI; Port SO; }

  Instance WI1 Of WrappedInstr { InputPort SI = SI; }
  Instance WI2 Of WrappedInstr { InputPort SI = mux1; }
  Instance WI3 Of WrappedInstr { InputPort SI = mux2; }

  Instance reg3 Of SReg
  {
    Parameter MSB = 2;
    InputPort SI  = mux3;
    InputPort DI  = 'b0;
  }

  ScanMux mux1 SelectedBy RS[2]
  {
    1'b0 : SI;
    1'b1 : WI1.SO;
  }

  ScanMux mux2 SelectedBy RS[1]
  {
    1'b0 : mux1;
    1'b1 : WI2.SO;
  }

  ScanMux mux3 SelectedBy RS[0]
  {
    1'b0 : mux2;
    1'b1 : WI3.SO;
  }
}
