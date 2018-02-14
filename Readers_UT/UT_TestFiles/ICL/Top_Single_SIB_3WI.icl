Module Single_SIB_3WI
{
  Alias WI_MSB = WI3.reg8.SR[7], WI2.reg8.SR[7], WI1.reg8.SR[7];
  Alias Bundle = WI3.reg8.SR,    WI2.reg8.SR,    WI1.reg8.SR;

  ScanInPort SI;
  ScanOutPort SO { Source SIB1.SO; }

  Instance SIB1 Of SIB_mux_pre { InputPort SI = SI;
                                 InputPort fromSO = WI3.SO; }
  Instance WI1 Of WrappedInstr { InputPort SI = SIB1.toSI; }
  Instance WI2 Of WrappedInstr { InputPort SI = WI1.SO; }
  Instance WI3 Of WrappedInstr { InputPort SI = WI2.SO; }
}

