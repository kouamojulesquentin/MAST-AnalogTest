package fonctions is
    function log2(arg : integer) return integer;
end package;


package body fonctions is
    function log2(arg : integer) return integer is
      variable i : integer;
   begin
      i := 0;  
      while (2**i < arg) and i < 31 loop
         i := i + 1;
      end loop;
      return i;
    end function;
end package body;