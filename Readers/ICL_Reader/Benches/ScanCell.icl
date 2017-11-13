/*
*
* -------------+------------+-------------------------+------------------------
*   Version    |  Date      | Author                  | Organization
* -------------+------------+-------------------------+------------------------
*          1.0 | 18.04.2016 | Anton Tsertov           | Testonica Lab
*--------------+------------+-------------------------+------------------------
*          2.0 | 15.05.2016 | Anton Tsertov           | Testonica Lab
*--------------+------------+-------------------------+------------------------
*
*/

Module ScanCell {

	ScanInPort SI;
	ShiftEnPort SE;
	CaptureEnPort CE;
	UpdateEnPort UE;
	SelectPort SEL;
	ResetPort RST;
	TCKPort TCK;
	ScanOutPort SO { 
		Source SMux;
	}
	ScanInterface flex_Scan { 
		Port SI; 
		Port SO; 
		Port SEL; 
	}
	
	ScanInPort maskSI;
	SelectPort maskSEL;
	ScanOutPort maskSO { 
		Source scbMask.SO;
	}
	ScanInterface mask_Scan { 
		Port maskSI; 
		Port maskSO; 
		Port maskSEL; 
	}
	
	LogicSignal ls_maskSEL {
		SEL & maskSEL;
	}
	LogicSignal ls_sRegSEL {
		SEL & scbMask.toSEL;
	}
	
	Instance scbMask Of SCB {
		InputPort SI = maskSI;
		InputPort SEL = ls_maskSEL;
	}
	
	Instance sReg Of SReg {
		InputPort SI = SI;
		InputPort SEL = ls_sRegSEL;
		Parameter Size = 1;
	}
		
	ScanMux SMux SelectedBy MaskSR {
		0: SI;
		1: sReg.SO;
	}
	
}