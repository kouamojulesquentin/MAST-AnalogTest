@section MAST ""
MAST project provides simplified 'C' or 'C++' API to use MAST with PDL like syntax.

@note Simple API implies it does not have the full-fledged capabilities of @ref MAST_Core.

__C++ based algorithm example__

      @code {.cpp}
      void Shift ()
      {
        iPrefix("SWIR.WIR");

        auto fromPath  = "reg_2";
        auto toPath    = "reg_3";
        auto loopCount = 20u;

        while (loopCount--)
        {
          auto gotValue = iGetRefresh<uint32_t>(fromPath);
          gotValue <<= 1u;
          iWrite(toPath, gotValue);
          iApply();
        }
      }
      @endcode

