SIT files documentation  {#SIT_Files}
=======================

[TOC]

[API]:   Acronyms.html "Application Programming Interface"
[ATE]:   Acronyms.html "Automated Test Equipment"
[ATPG]:  Acronyms.html "Automated Test Pattern Generation"
[HSDL]:  Acronyms.html "Hierarchical Scan Description Language"
[ICL]:   Acronyms.html "Instrument Connectivity Language"
[JTAG]:  Acronyms.html "Joint Test Association Group"
[MAST]:  Acronyms.html "MAnager for SoC Test"
[MIBs]:  Acronyms.html "Multiple Segment Insertion Bits"
[PDL]:   Acronyms.html "Procedural Description Language"
[SIB]:   Acronyms.html "Segment Insertion Bit"
[SIT]:   Acronyms.html "Simplified [ICL] Tree Reader"
[SJTAG]: Acronyms.html "System JTAG (working group)"
[SM]:    Acronyms.html "System Model"
[SPI]:   Acronyms.html "Serial Peripheral Interface"
[STIL]:  Acronyms.html "Standard Test Interface Language (For Digital Test Vectors)"
[SUT]:   Acronyms.html "System Under Test"
[SVF]:   Acronyms.html "Serial Vector Format"
[TAP]:   Acronyms.html "Test Access Point"
[TDR]:   Acronyms.html "Test Data Register"

# Simplified [ICL] Tree Reader

This module is based on flex/bison and provides an interface to generate a System Model from a simplified language, called Simplified [ICL] Tree ([SIT]).<br>
The Syntax is extremely simple:

## LEAF NODES

Leaf nodes are nodes that cannot have children because they represent the basic 1687 topology unit : the register. <br>
Syntax is:

    REGISTER  <node_name> <Size> Hold_value* Bypass : "<bypass_value>"
<br>
<table>
  <caption id="Register"></caption>
  <tr><th width = 100>Node Type   </th><th>   Node Information</th></tr>
  <tr>
    <td>REGISTER</td>
    <td>
      <Size> Hold_value* Bypass: "<bypass_value>"<br>
      <table>
        <tr><td><Size>:         </td><td>  number of bits composing the register in decimal notation            </td></tr>
        <tr><td>Hold_value:     </td><td>  if provided, sets the "hold" property of the register to true  (i.e. the bypass value corresponds to the last value it has been assigned)      </td></tr>
        <tr><td><Bypass_value>: </td><td>  the bypass sequence, written following MAST's BinaryVector convention</td></tr>
      </table>
    </td>
  </tr>
</table>

Another leaf node is 'INSTANCE_OF' that represents the instanciation of a module at the point of insertion (as for any other child nodes). <br>
Syntax is:

      INSTANCE_OF SIT      <sit_file>
    | INSTANCE_OF FACTORY  <factory_name>


<br>
<table>
  <tr><th width = 100>Node Type   </th><th>   Node Information</th></tr>
  <tr>
    <td>INSTANCE_OF</td>
    <td>
      SIT     <sit_file>   <br>
      FACTORY <factory_name>
      <table>
        <tr><td><sit_file></td><td>  name or path of a SIT file            </td></tr>
        <tr><td><factory_name></td><td> identify a model factory registered by a plugin</td></tr>
      </table>
    </td>
  </tr>
</table>


## Parent NODES

Parent nodes represent hierarchical information: they can be either 1687 elements, such as [ICL] instances, or abstract constructs to help representing hierarchy.

Each node follows the same syntax:

<[PDL]_association>* :  [PDL] <function_name_list><br>
<node_type> <node_name> <node information>* <[PDL]_association>* { <children_list>+ }

<table>
  <tr><td><node_type>:  </td><td>  it can be either a base node (REGISTER,CHAIN,LINKER,ACCESS_INTERFACE) or a composite node identifying a
                                   specific configuration (ex: [SIB], 1500_WRAPPER, AI_1149_1, [TAP]_1149_1, etc...).</td></tr>
  <tr><td><node_name>  </td><td>  standard identifier. Can be shared by different nodes (the Builder will assign an unique identifier)</td></tr>
<tr><td><PDL_association>  </td><td> used to indicate that the function identified by "function_name_list" (comma-separated identifiers) is to be associated with the current Parent Node (see main documentation for details).<br>
It is the equivalent of the [PDL] <code>iProcsForModule</code> command.</td></tr>
<tr><td><children_list>+  </td><td>non-empty list of nodes in the lower hierarchical level</td></tr>
<tr><td><node information> </td><td>a variable field containing information specific to a given node_type, as explained in the following table. NB: underlined strings depict language token</td></tr>
</table>

### NODE INFORMATION DEPENDING ON NODE TYPE
<table>
  <caption id="Parent_node"></caption>
  <tr><th width = 100>Node Type   </th><th>   Node Information</th></tr>
  <tr>
    <td>CHAIN</td>
    <td>NOT_IN_PATH*: if provided, the name of the chain is not used when composing a path (used by [PDL] [API] to identify registers)</td>
  </tr>
  <tr>
    <td>MIB</td>
    <td>
      Macro to generate a n-bit register controlling a n- derivation linker.<br>
      <position> <active> <reverse>* <max_derivations> <path_selector><br>

      <table>
        <tr><td><position>         </td><td>  can be either PRE (the control register precedes the Linker) or POST (the control register follows the Linker)                     </td></tr>
        <tr><td><active>           </td><td>  can be either HIGH ( active-high logic selection) or LOW (active-low logic selection)                                              </td></tr>
        <tr><td><reverse>         </td><td>  if set to REVERSE, bit mapping in the control register is reversed (i.e. leftmost bit selects rightmost derivation)                </td></tr>
        <tr><td><max_derivations>  </td><td>  the maximum number of derivations accepted by the MIB, as a decimal number. NB: a MIB can have fewer derivations that its maximum  </td></tr>
        <tr><td><path_selector>    </td><td>  a string selecting the decoding strategy. See "Linker" for this lsit and description of supported selectors                        </td></tr>
      </table>
    </td>
  </tr>
  <tr>
    <td>[SIB]</td>
    <td>
      Macro to generate a 1-bit register controlling a 1- derivation linker, as defined in the standard document.<br>
      <position> <active><br>
      <table>
        <tr><td><position> </td><td>  can be either PRE (the control register precedes the Linker) or POST (the control register follows the Linker)</td></tr>
        <tr><td><active>   </td><td>  can be either HIGH ( active-high logic selection) or LOW (active-low logic selection)                         </td></tr>
      </table>
    </td>
  </tr>
  <tr>
    <td>WRAPPER_1500</td>
    <td>
      IEEE-1500 compliant wrapper  derivations. <br>
      Selector is set as Binary <br>
      <table>
        <tr><td><max_derivations>  </td><td> Maximum number of wrapper registers</td></tr>
      </table>
    </td>
  </tr>
  <tr>
    <td>LINKER</td>
    <td>
      <path_selector> <ctrl_node> <max_derivations>
      <table>
        <tr><td>  <path_selector>    </td><td>  a string selecting the decoding strategy of the Linker</td></tr>
        <tr><td>  <max_derivations>  </td><td>  maximum number of allowed paths (i.e. number of children)</td></tr>
        <tr><td>  <ctrl_reg>         </td><td>  string denoting the "node_name" of the register controlling the linker</td></tr>
      </table>
      <br>
      NB: the control register can be declared anywhere in the [SIT] file, not necessarily before the Linker.
      <table>
        <tr>
          <td>Binary</td>
          <td>
            Derivation "n" is selected by putting the binary value "n" in the controlling register. <br>
            When set to 0, no path is selected. Selection register size is <code>ceiling(log2(path_count+1))</code>
          </td>
        </tr>
        <tr>
          <td>Binary_noidle</td>
          <td>
            Derivation "n" is selected by putting the binary value "n-1" in the controlling register. <br>
            When put to 0, path 1 is selected. Selection register size is <code>ceiling(log2(path_coun</code>t))
          </td>
        </tr>
        <tr>
          <td>One_Hot</td>
          <td>
            Derivation "n" is selected by putting the n-th register of the controlling path to 1. <br>
            When set to 0, no path is selected. <br>
            Only one path can be opened at a given time.<br>
            <br>
            Selection register size is "path_count"
          </td>
        </tr>
        <tr>
          <td>One_Hot_noidle</td>
          <td>
            Like One_Hot, but a path is always be selected (i.e. there is no "no path selected" coding)
          </td>
        </tr>
        <tr>
          <td>N_Hot</td>
          <td>
            Derivation "n" is selected by putting the n-th register of the controlling path to 1.<br>
            When set to 0, no path is selected. Several paths can be opened at a given time.<br>
            <br>
            Selection register size is "path_count"
          </td>
        </tr>
        <tr>
          <td>N_Hot_noidle</td>
          <td>
            Like N_Hot, but at least one path must always be selected (i.e. there is no "no path selected" coding)<br>
            <br>
            Table_Based Not supported yet
          </td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td>ACCES_INTERFACE</td>
    <td>
      <protocol> <AI_identifier>* <AI_table><br>
      Creates an Access Interface node implementing the provided protocol<br>
      <table>
        <tr><td> <protocol>        </td><td>  the protocol used by the underlying AccessInterface. Can be JTAG_Loopback, JTAG_SVF_simulation, JTAG_SVF_Emulation or SPI_FTDI</td></tr>
        <tr><td> <AI_identifier>  </td><td>  an optional string that might be required by the protocol. <br>
                                               For instance, SPI_FTDI might specify an usbDeviceID as quoted-string bit/hex value.</td> </tr>
        <tr><td> <AI_TABLE>        </td><td>  the addresses used by the protocol provided as a comma-separated list between square brackets.<br>
                                               Parenthesis can be used for visual help but have no semantic value. <br>
                                               For instance:<br>
                                               [ ( "0x41" , "0x42" ,"0x0" ), ( "0x1" , "0x2" ,"0x3" )] == [ "0x41" , "0x42" ,"0x0" , "0x1" , "0x2" ,"0x3" ] == [ ( "0x41" , "0x42" ),"0x0" , ( "0x1" , "0x2" ,("0x3" ]</td></tr>
      </table>
      <br>
      NB: openOCD protocol supposes a fully-formed [TAP] in the System Under Test, so it can be instantiated only inside the [TAP] macro<br>
    </td>
  </tr>
  <tr>
    <td>[JTAG]_[TAP]</td>
    <td>
      <protocol> <optional_AI_identifier>* <IR_size>  <optional_IR_coding>* <max_DR_chains><br>
      Creates an IEEE-1149.1 compliant Test Access Port composed by a AI_1149_1 node having a "IR_size" IR register controlling <br>
      a DR_MUX for "chain count" endpoints, with a 1-bit bypass register already appended.<br>
      When no coding is provided, BYPASS is selected by FFFF (all ones), and chains by a binary selection starting at 0x01.<br>

      <table>
        <tr><td>  <protocol>               </td><td>  the protocol used by the underlying AccessInterface. <br>Can be Loopback, SVF_simulation or SVF_openOCD</td></tr>
        <tr><td>  <optional_AI_identifier> </td><td>  an optional string that might be required by the protocol. <br>For instance, SVF_openOCD requires a Design Name to be provided</td></tr>
        <tr><td>  <IR_size>                </td><td>  decimal number indicating the size in bits of the IR register</td></tr>
        <tr><td>  <max_DR_chains>          </td><td>  decimal number indicating the maximum number of chains</td></tr>
        <tr><td>  <optional_IR_coding>     </td><td>  the coding  used by the IR, starting from the Bypass Register, provided as a comma-separated list  between square brackets.
                                                       Parenthesis can be used for visual help but have no semantic value.<br>
                                                       For instance, [  "0xFF" ,  "0x42" ,"0x00"] == [  "0xFF" , ( "0x42" ,"0x00")] ==[  ("0xFF" ,  "0x42" ,("0x00"]</td></tr>
      </table>
    </td>
  </tr>
</table>



## Hierarchy
The children of a given node are listed between two accolades following it.<br>
The order in the list is respected when appending children.<br>
For instance:

    CHAIN test_chain
    {
      REGISTER test_register_1 12 Bypass: "0b1001:0110:1100"
      REGISTER test_register_2 12 Bypass: "0b1001:0110:1100"
    }


Register nodes test_register_1 and test_register_2 are respectively the first and second child of chain node test_chain.<br>
Due to Lex/Yacc limitations, SIT_Reader does not support empty hierarchies. At least one node must be provided.<br>
<br>
NB: Trying to provide children to a Register node will cause a syntax error.<br>

## USAGE
To use the SIT_reader, the <code>SIT_Reader.hpp</code> header must be included.<br>
A reader object can be instantiated by giving it a <code>std::shared_ptr<mast::SystemModel></code>  to work on as parameter, for instance:

    SIT::SIT_Reader driver(sm);

Parsing can then be launched using the "parse" method, with either a filename or an istream as input:<br>

    auto result = driver.parse(stream);

If parsing is correctly executed, the return value is __true__ and <code>driver.parsed_sut</code> points to the root of the generated tree.<br>
If an error occurs, the return value is __false__ and <code>driver.parsed_sut == nullptr</code>.<br>
<br>
NB: [SIT] parsing is not supposed to raise any exception or cause execution to stop, but the SystemModel build process raises exceptions as usual.





