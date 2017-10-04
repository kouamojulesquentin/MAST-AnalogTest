/*
* Copyright Testonica Lab (C) 2016
*
* History:
* -------------+------------+-------------------------+------------------------
*   Version    |  Date      | Author                  | Organization
* -------------+------------+-------------------------+------------------------
*          1.0 | 23.03.2016 | Anton Tsertov           | Testonica Lab
*--------------+------------+-------------------------+------------------------
*
* This file contains code snippets from example E.30 from IEEE Std 1687 Appendix E 
*
*/


Module MyChip {
    Attribute lic = 'h 3bbb8f7e;
    ClockPort CLKA;
    ClockPort CLKB;
    DataInPort P1;
    DataInPort P2;
    Instance PLLA_I1 Of PLLA { InputPort ref = CLKA;}
    Instance PLLB_I1 Of PLLB { InputPort ref = CLKB;}
    Instance COREA_I1 Of CoreA {
        InputPort P = P1;
        InputPort CLK = PLLA_I1.vco;
        InputPort SWIR = Tap1.toSWIR;
    }
    Instance COREA_I2 Of CoreA {
        InputPort P = P2;
        InputPort CLK = PLLB_I1.vco;
    }

    AccessLink Tap1 Of STD_1149_1_2001 {
        BSDLEntity Mychip;
        wir_select {
    	    ScanInterface {
    		    COREA_I1.SP1;
                COREA_I2.SP1;
    		}
            ActiveSignals { toSWIR ;}
        }
        wdr_select { 
    	    ScanInterface {
    		    COREA_I1;
                COREA_I2;
    		}
    // Because CoreA only has one scan port, explicitly listing the ScanInterface (ā€�.SP1ā€¯) is optional
    // Also note that since ā€�toSWIRā€¯ is not listed as an ActiveSignal, so it is INACTIVE here.
        }
    }
}

Module PLLA {
    Attribute lic = 'h b7f034b7;
    ClockPort ref;
    ToClockPort vco { FreqMultiplier 6; }
}
Module PLLB {
    Attribute lic = 'h e8263db4;
    ClockPort ref;
    ToClockPort vco { FreqMultiplier 12; }
}
Module CoreA {
    Attribute lic = 'h 4567ea2;
    ClockPort CLK;
    ScanInPort WSI;
    ScanOutPort WSO {Source WTAP.WSO; }
    ShiftEnPort shiftWR;
    CaptureEnPort captureWR;
    UpdateEnPort updateWR;
    ResetPort WRST;
    TCKPort WRCK;
	SelectPort SEL;
    DataInPort SWIR;
	DataInPort P;
    ScanInterface SP1 {
		Port WSI; 
		Port WSO;
		Port SEL;
        Port shiftWR;
		Port captureWR;
		Port updateWR;
	}
	
	Instance CLKDIV Of CLK_Divider {
		InputPort ref = CLK;
	}
	
	Instance WTAP Of WSP {
		InputPort WSI = WSI;
		InputPort dataPort1 = SR1[0];
		InputPort dataPort2 = M1.WSO;
		InputPort SWIR = SWIR;
	}
	
	Instance M1 Of M {
		InputPort P = P;
		InputPort WSI = WSI;
		InputPort CLK = CLKDIV.divRef;
	}
	
	ScanRegister SRj[1:0] {
		ScanInSource WSI;
		CaptureSource SRj;
		ResetValue 'b0;
	}
	
	ScanMux SM SelectedBy SR1 {
		0: SRj[0];
		1: WSI;
	}
	
	ScanRegister SR1 {
		ScanInSource SM;
		CaptureSource SR1;
		ResetValue 'b0;
	}
}

Module M {
    Attribute lic = 'h 7a599832;
    ClockPort CLK;
    ScanInPort WSI;
    ScanOutPort WSO {Source SR0[0]; }
    ShiftEnPort shiftWR;
    CaptureEnPort captureWR;
    UpdateEnPort updateWR;
    TCKPort WRCK;
	SelectPort SEL;
    ScanInterface SI1 {
		Port WSI; 
		Port WSO;
		Port SEL;
        Port shiftWR;
		Port captureWR;
		Port updateWR;
	}
	
	ScanRegister SRi[1:0] {
		ScanInSource WSI;
		CaptureSource SRi;
		ResetValue 'b0;
	}
	
	ScanMux SM SelectedBy SR0 {
		0: SRi[0];
		1: WSI;
	}
	
	ScanRegister SR0 {
		ScanInSource SM;
		CaptureSource SR0;
		ResetValue 'b0;
	}
}

Module WSP {
    Attribute lic = 'h 1f464a9d;
    ClockPort CLK;
    ScanInPort WSI;
    ScanOutPort WSO {Source sMuxInst; }
    ShiftEnPort shiftWR;
    CaptureEnPort captureWR;
    UpdateEnPort updateWR;
    ResetPort WRST;
    TCKPort WRCK;
    SelectPort SEL;
    DataInPort SWIR;
    DataInPort dataPort1;
    DataInPort dataPort2;
    ScanInterface SI1 {
		Port WSI; 
		Port WSO;
		Port SEL;
        Port shiftWR;
		Port captureWR;
		Port updateWR;
	}
	
	ScanRegister WIR[1:0] {
		ScanInSource WSI;
		CaptureSource WIR;
		ResetValue 'b0;
	}
	
	ScanMux sMuxInst SelectedBy SWIR {
		0: WIR[0];
		1: sMuxData;
	}
	
	ScanMux sMuxData SelectedBy WIR[0] {
		0: dataPort1;
		1: dataPort2;
	}	
}

Module CLK_Divider {
    Attribute lic = 'h 258d6169;
    ClockPort ref;
    ToClockPort divRef { FreqDivider 4; }
}

