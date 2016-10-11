Library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

package tools is

constant clk_ratio : integer := 250; --sysclk is 100MHz

function HEX2STDLOGIC (hex_value : in std_logic_vector (3 downto 0)) return std_logic_vector;
function ASCII2STDLOGIC (char : in character) return std_logic_vector;
function HEX2STRING (hex_value : in std_logic_vector (3 downto 0)) return character;


--Middle value for analog sticks
constant stick_centre : integer := 128;

end tools;

package body tools is

function ASCII2STDLOGIC (char : in character) return std_logic_vector is
begin
 case char is
  when  NUL =>  return "00000000"; --     NUL    (Null char.)
  when	SOH =>  return "00000001"; --      SOH    (Start of Header)
  when	STX =>  return "00000010"; --      STX    (Start of Text)
  when	ETX =>  return "00000011"; --      ETX    (End of Text)
  when	EOT =>  return "00000100"; --     EOT    (End of Transmission)
  when	ENQ =>  return "00000101"; --      ENQ    (Enquiry)
  when	ACK =>  return "00000110"; --      ACK    (Acknowledgment)
  when	BEL =>  return "00000111"; --      BEL    (Bell)
  when	BS  =>  return "00001000"; --       BS    (Backspace)
--  when	HT  =>  return "00001001"; --       HT    (Horizontal Tab)
  when	HT  =>  return "11111111"; --       HT as a black square
  when	LF  =>  return "00001010"; --       LF    (Line Feed)
  when	VT  =>  return "00001011"; --       VT    (Vertical Tab)
  when	FF  =>  return "00001100"; --       FF    (Form Feed)
  when	CR  =>  return "00001101"; --       CR    (Carriage Return)
  when	SO  =>  return "00001110"; --       SO    (Shift Out)
  when	SI  =>  return "00001111"; --       SI    (Shift In)
  when	DLE =>  return "00010000"; --      DLE    (Data Link Escape)
  when	DC1 =>  return "00010001"; --      DC1 (XON) (Device Control 1)
  when	DC2 =>  return "00010010"; --      DC2       (Device Control 2)
  when	DC3 =>  return "00010011"; --      DC3 (XOFF)(Device Control 3)
  when	DC4 =>  return "00010100"; --      DC4       (Device Control 4)
  when	NAK =>  return "00010101"; --     NAK    (Negative Acknowledgement)
  when	SYN =>  return "00010110"; --     SYN    (Synchronous Idle)
  when	ETB =>  return "00010111"; --     ETB    (End of Trans. Block)
  when	CAN =>  return "00011000"; --      CAN    (Cancel)
  when	EM  =>  return "00011001"; --       EM    (End of Medium)
  when	SUB =>  return "00011010"; --      SUB    (Substitute)
  when	ESC =>  return "00011011"; --      ESC    (Escape)
  when	FSP  =>  return "00011100"; --FS    (File Separator)
  when	GSP  =>  return "00011101"; --GS    (Group Separator)
  when	RSP  =>  return "00011110"; --RS    (Request to Send)(Record Separator)
  when	USP  =>  return "00011111"; --US    (Unit Separator)
  when	' ' =>  return "00100000"; --SP    (Space)
  when	'!' =>  return "00100001"; -- !    (exclamation mark)
  when	'"' =>  return "00100010"; -- "0    (double quote)
  when	'#' =>  return "00100011"; -- #    (number sign)
  when	'$' =>  return "00100100"; -- $    (dollar sign)
  when	'%' =>  return "00100101"; -- %    (percent)
  when	'&' =>  return "00100110"; -- &    (ampersand)
  when	'''=>  return "00100111"; -- '    (single quote)
  when	'(' =>  return "00101000"; -- (    (left/opening parenthesis)
  when	')' =>  return "00101001"; -- )    (right/closing parenthesis)
  when	'*' =>  return "00101010"; -- *    (asterisk)
  when	'+' =>  return "00101011"; -- +    (plus)
  when	',' =>  return "00101100"; -- ,    (comma)
  when	'-' =>  return "00101101"; -- -    (minus or dash)
  when	'.' =>  return "00101110"; -- .    (dot)
  when	'/' =>  return "00101111"; -- /    (forward slash)
  when	'0' =>  return "00110000"; -- 0
  when	'1' =>  return "00110001"; -- 1
  when	'2' =>  return "00110010"; -- 2
  when	'3' =>  return "00110011"; -- 3
  when	'4' =>  return "00110100"; -- 4
  when	'5' =>  return "00110101"; -- 5
  when	'6' =>  return "00110110"; -- 6
  when	'7' =>  return "00110111"; -- 7
  when	'8' =>  return "00111000"; -- 8
  when	'9' =>  return "00111001"; -- 9
  when	':' =>  return "00111010"; -- :    (colon)
  when	';' =>  return "00111011"; -- ;    (semi-colon)
  when	'<' =>  return "00111100"; -- <    (less than)
  when	'=' =>  return "00111101"; -- =    (equal sign)
  when	'>' =>  return "00111110"; -- >    (greater than)
  when	'?' =>  return "00111111"; -- ?    (question mark)
  when	'@' =>  return "01000000"; -- @    (AT symbol)
  when	'A' =>  return "01000001"; -- A
  when	'B' =>  return "01000010"; -- B
  when	'C' =>  return "01000011"; -- C
  when	'D' =>  return "01000100"; -- D
  when	'E' =>  return "01000101"; -- E
  when	'F' =>  return "01000110"; -- F
  when	'G' =>  return "01000111"; -- G
  when	'H' =>  return "01001000"; -- H
  when	'I' =>  return "01001001"; -- I
  when	'J' =>  return "01001010"; -- J
  when	'K' =>  return "01001011"; -- K
  when	'L' =>  return "01001100"; -- L
  when	'M' =>  return "01001101"; -- M
  when	'N' =>  return "01001110"; -- N
  when	'O' =>  return "01001111"; -- O
  when	'P' =>  return "01010000"; -- P
  when	'Q' =>  return "01010001"; -- Q
  when	'R' =>  return "01010010"; -- R
  when	'S' =>  return "01010011"; -- S
  when	'T' =>  return "01010100"; -- T
  when	'U' =>  return "01010101"; -- U
  when	'V' =>  return "01010110"; -- V
  when	'W' =>  return "01010111"; -- W
  when	'X' =>  return "01011000"; -- X
  when	'Y' =>  return "01011001"; -- Y
  when	'Z' =>  return "01011010"; -- Z
  when	'[' =>  return "01011011"; -- [    (left/opening bracket)
  when	'\' =>  return "01011100"; -- \    (back slash)
  when	']' =>  return "01011101"; -- ]    (right/closing bracket)
  when	'^' =>  return "01011110"; -- ^    (caret/cirumflex)
  when	'_' =>  return "01011111"; -- _    (underscore)
  when	'`' =>  return "01100000"; -- `
  when	'a' =>  return "01100001"; -- a
  when	'b' =>  return "01100010"; -- b
  when	'c' =>  return "01100011"; -- c
  when	'd' =>  return "01100100"; -- d
  when	'e' =>  return "01100101"; -- e
  when	'f' =>  return "01100110"; -- f
  when	'g' =>  return "01100111"; -- g
  when	'h' =>  return "01101000"; -- h
  when	'i' =>  return "01101001"; -- i
  when	'j' =>  return "01101010"; -- j
  when	'k' =>  return "01101011"; -- k
  when	'l' =>  return "01101100"; -- l
  when	'm' =>  return "01101101"; -- m
  when	'n' =>  return "01101110"; -- n
  when	'o' =>  return "01101111"; -- o
  when	'p' =>  return "01110000"; -- p
  when	'q' =>  return "01110001"; -- q
  when	'r' =>  return "01110010"; -- r
  when	's' =>  return "01110011"; -- s
  when	't' =>  return "01110100"; -- t
  when	'u' =>  return "01110101"; -- u
  when	'v' =>  return "01110110"; -- v
  when	'w' =>  return "01110111"; -- w
  when	'x' =>  return "01111000"; -- x
  when	'y' =>  return "01111001"; -- y
  when	'z' =>  return "01111010"; -- z
  when	'{' =>  return "01111011"; -- {    (left/opening brace)
  when	'|' =>  return "01111100"; -- |    (vertical bar)
  when	'}' =>  return "01111101"; -- }    (right/closing brace)
  when	'~' =>  return "01111110"; -- ~    (tilde)
  when	DEL =>  return "01111111"; -- DEL    (delete)
  when	others =>  return "11111111"; 
end case;
end; 

function HEX2STDLOGIC (hex_value : in std_logic_vector (3 downto 0)) return std_logic_vector is
begin
case hex_value is
  when	"0000" =>  return "00110000"; -- 0
  when	"0001" =>  return "00110001"; -- 1
  when	"0010" =>  return "00110010"; -- 2
  when	"0011" =>  return "00110011"; -- 3
  when	"0100" =>  return "00110100"; -- 4
  when	"0101" =>  return "00110101"; -- 5
  when	"0110" =>  return "00110110"; -- 6
  when	"0111" =>  return "00110111"; -- 7
  when	"1000" =>  return "00111000"; -- 8
  when	"1001" =>  return "00111001"; -- 9
  when	"1010" =>  return "01000001"; -- A
  when	"1011" =>  return "01000010"; -- B
  when	"1100" =>  return "01000011"; -- C
  when	"1101" =>  return "01000100"; -- D
  when	"1110" =>  return "01000101"; -- E
  when	"1111" =>  return "01000110"; -- F
  when others => return "01000000"; -- @    (AT symbol)
end case;

end;

function HEX2STRING (hex_value : in std_logic_vector (3 downto 0)) return character is
begin
case hex_value is
  when	"0000" =>  return '0'; -- 0
  when	"0001" =>  return '1'; -- 1
  when	"0010" =>  return '2'; -- 2
  when	"0011" =>  return '3'; -- 3
  when	"0100" =>  return '4'; -- 4
  when	"0101" =>  return '5'; -- 5
  when	"0110" =>  return '6'; -- 6
  when	"0111" =>  return '7'; -- 7
  when	"1000" =>  return '8'; -- 8
  when	"1001" =>  return '9'; -- 9
  when	"1010" =>  return 'A'; -- A
  when	"1011" =>  return 'B'; -- B
  when	"1100" =>  return 'C'; -- C
  when	"1101" =>  return 'D'; -- D
  when	"1110" =>  return 'E'; -- E
  when	"1111" =>  return 'F'; -- F
  when others => return '@'; -- @    (AT symbol)
end case;

end;


end;
