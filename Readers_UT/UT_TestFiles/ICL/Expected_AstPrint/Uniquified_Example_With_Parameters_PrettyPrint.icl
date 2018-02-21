NameSpace;
Module my_block
{
  Attribute att1 = "$myStringParam";
  Attribute att2 = $myStringParam;
  Parameter Reset_value = 'b0;
  Parameter my = "_";
  Parameter string = "_";
  Parameter myStringParam = "default";
  Parameter nspace = "default";
  Parameter pname = "myproc";
  Parameter ss = "default";
  Parameter MSB1 = 1;
  Parameter MSB2 = $width - 1;
  Parameter width = 32;
  Parameter CC = 1;
  Parameter CS = $MSB2, 1'b0;
  Parameter A = 3'b0;
  Parameter B = $A, 6'b110;
  Parameter E = "E_", $C, "_", $D, $G, "_E";
  Parameter F = "F_", $D, "_", $C, "_F";
  Parameter G = "G_Logica_G";
  Parameter D = "D_", $C, "_D";
  Parameter C = "C_Test_C";
  LocalParameter LSB = 1;
  LocalParameter MSB = $width - 1;
  LocalParameter Middle = $MSB / 2;
  Alias SR_H = Reg_a[$MSB:$Middle];
  Alias SR_L = Reg_a[$Middle - 1:0];
  ScanRegister Reg_a[$MSB1:0]
  {
    ScanInSource SI;
    CaptureSource $CS;
    ResetValue $Reset_value;
  }
  ScanRegister Reg_b[$width - 1:$LSB]
  {
    ScanInSource Reg_a;
  }
}
Module top
{
  LocalParameter ABC = 4;
  LocalParameter Reset_value = 'hFFF;
  Instance I1 Of UniquifiedModules::my_block__uniquified__1
  {
    Parameter my = "My";
    Parameter string = "String";
    Parameter myStringParam = "MyString";
    Parameter MSB1 = 31;
    Parameter MSB2 = 11;
    Parameter nspace = "mynspace";
    Parameter pname = "myproc";
    Parameter ss = "125 5'b0";
    Parameter CC = (4 * 10) + 11 + 31 + 1;
  }
  Instance I2 Of UniquifiedModules::my_block__uniquified__2
  {
    Parameter Reset_value = 'b0110;
    Parameter my = "MyI2";
    Parameter string = " I2String";
    Parameter myStringParam = "MyI2 I2String";
    Parameter MSB1 = 63;
    Parameter MSB2 = 49;
    Parameter nspace = "i2_nspace";
    Parameter pname = "i2_myproc";
    Parameter ss = "128 5'b0";
    Parameter CC = (4 * 10) + 49 + 63 + 2;
    Parameter C = "Me";
  }
}
NameSpace UniquifiedModules;
Module my_block__uniquified__1
{
  Attribute att1 = "$myStringParam";
  Attribute att2 = "MyString";
  Parameter Reset_value = 'b0;
  Parameter my = "My";
  Parameter string = "String";
  Parameter myStringParam = "MyString";
  Parameter nspace = "mynspace";
  Parameter pname = "myproc";
  Parameter ss = "125 5'b0";
  Parameter MSB1 = 31;
  Parameter MSB2 = 11;
  Parameter width = 32;
  Parameter CC = (4 * 10) + 11 + 31 + 1;
  Parameter CS = 11, 1'b0;
  Parameter A = 3'b0;
  Parameter B = 3'b0, 6'b110;
  Parameter E = "E_C_Test_C_D_C_Test_C_DG_Logica_G_E";
  Parameter F = "F_D_C_Test_C_D_C_Test_C_F";
  Parameter G = "G_Logica_G";
  Parameter D = "D_C_Test_C_D";
  Parameter C = "C_Test_C";
  LocalParameter LSB = 1;
  LocalParameter MSB = 32 - 1;
  LocalParameter Middle = (32 - 1) / 2;
  Alias SR_H = Reg_a[31:15];
  Alias SR_L = Reg_a[14:0];
  ScanRegister Reg_a[31:0]
  {
    ScanInSource SI;
    CaptureSource $CS;
    ResetValue 'b0;
  }
  ScanRegister Reg_b[31:1]
  {
    ScanInSource Reg_a;
  }
}
Module my_block__uniquified__2
{
  Attribute att1 = "$myStringParam";
  Attribute att2 = "MyI2 I2String";
  Parameter Reset_value = 'b0110;
  Parameter my = "MyI2";
  Parameter string = " I2String";
  Parameter myStringParam = "MyI2 I2String";
  Parameter nspace = "i2_nspace";
  Parameter pname = "i2_myproc";
  Parameter ss = "128 5'b0";
  Parameter MSB1 = 63;
  Parameter MSB2 = 49;
  Parameter width = 32;
  Parameter CC = (4 * 10) + 49 + 63 + 2;
  Parameter CS = 49, 1'b0;
  Parameter A = 3'b0;
  Parameter B = 3'b0, 6'b110;
  Parameter E = "E_Me_D_Me_DG_Logica_G_E";
  Parameter F = "F_D_Me_D_Me_F";
  Parameter G = "G_Logica_G";
  Parameter D = "D_Me_D";
  Parameter C = "Me";
  LocalParameter LSB = 1;
  LocalParameter MSB = 32 - 1;
  LocalParameter Middle = (32 - 1) / 2;
  Alias SR_H = Reg_a[31:15];
  Alias SR_L = Reg_a[14:0];
  ScanRegister Reg_a[63:0]
  {
    ScanInSource SI;
    CaptureSource $CS;
    ResetValue 'b0110;
  }
  ScanRegister Reg_b[31:1]
  {
    ScanInSource Reg_a;
  }
}
