Module top
{
  LocalParameter ABC = 4;
  LocalParameter Reset_value = 'b0;
  Instance I1 Of my_block
  {
    Parameter my            = "My";
    Parameter string        = "String";
    Parameter myStringParam = "", $my,$string;  // empty string is to cope with failure to parse parameters correctly!
    Parameter MSB1          = 31; // A defined as A[31:0] in my_block
    Parameter MSB2          = 11; // D defined as D[11:0]
    Parameter nspace        = "mynspace";
    Parameter pname         = "myproc";
    Parameter ss            = "125 5'b0";
    Parameter CC            = ($ABC * 10) + $MSB2 + $MSB1 + 1;
  }

  Instance I2 Of my_block
  {
    Parameter my            = "MyI2";
    Parameter string        = " I2String";
    Parameter myStringParam = "", $my,$string;  // Empty string is to cope with failure to parse parameters correctly!
    Parameter MSB1          = 63; // A defined as A[63:0] in my_block
    Parameter MSB2          = 49; // D defined as D[49:0]
    Parameter nspace        = "i2_nspace";
    Parameter pname         = "i2_myproc";
    Parameter ss            = "128 5'b0";
    Parameter CC            = ($ABC * 10) + $MSB2 + $MSB1 + 2;
    Parameter C             = "Me";
  }
}

Module my_block
{
  Parameter my            = "_";
  Parameter string        = "_";
  Parameter myStringParam = "default";
  Parameter nspace        = "default";
  Parameter pname         = "myproc";
  Parameter ss            = "default";
  Parameter MSB1          = 1;
  Parameter MSB2          = $width-1;
  Parameter width         = 32;
  Parameter CC            = 1;
  Parameter CS            = $MSB2, 1'b0;

  // A parameter may be used to define the value of another parameter as in the following example:
  Parameter A = 3'b0;
  Parameter B = $A, 6'b110;    // B becomes 9'b000000110
  Parameter E = "E_", $C, "_", $D, $G, "_E";
  Parameter F = "F_", $D, "_", $C, "_F";
  Parameter G = "G_", "Logica", "_G";
  Parameter D = "D_", $C, "_D";
  Parameter C = "C_", "Test", "_C";

  LocalParameter LSB = 1;

  Attribute att1 = "$myStringParam";  // This is a literal string (not a Parameter_Ref)
  Attribute att2 = "", $myStringParam;  // Empty string is to cope with failure to parse string attributes correctly!

  ScanRegister Reg_a[$MSB1:0]
  {
    CaptureSource $CS;
    ScanInSource SI;
    ResetValue $Reset_value;
  }

  ScanRegister Reg_b[$width-1:$LSB]
  {
    ScanInSource Reg_a;
  }

  DataRegister DReg_1[$MSB2:$LSB]
  {
    AddressValue 8'd0;
  }

}

