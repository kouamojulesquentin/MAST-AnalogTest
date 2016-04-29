# Runner.mak
# Builds a CxxTest runner

$(info )
$(info ====== Runner.mak ======)
$(info )

#+$(info RootPath: $(RootPath))
ifeq ($(RootPath), )
    RootPath     = ./
endif

UT_RootPath	 = $(RootPath)

Suites = \
           $(UT_RootPath)/Utility/UT_*.hpp      \
           $(UT_RootPath)/SystemModel/UT_*.hpp  \


ProjectFile  = $(RootPath)/Lib_UT.cbp

#+Generator    = ..\..\CxxTest\bin\cxxtestgen.py
Generator    = $(RootPath)\..\CxxTest\bin\cxxtestgen.py
RunnerFile   = $(UT_RootPath)/Generated/Runner.cpp
TemplateFile = $(UT_RootPath)/Runner.tpl
TestListener = ParenPrinter


# cxxtestgen needs Python
python="python.exe"

#+ifeq ($(wildcard $(python)),)
#+    $(warning Python is not defined)
#+else
#+    $(warning Using python: $(python))
#+endif

#To debug variables

#+test = $(wildcard $(MAKEFILE_LIST))
#+$(info Test: $(test))
#+$(info )

#+$(info Variables:)
#+$(info - CURDIR:         $(CURDIR))
#+$(info - MAKEFILE_LIST:  $(MAKEFILE_LIST))
$(info - RootPath:       $(RootPath))
$(info - Python:         $(python))
#+$(info - Generator:      $(Generator))
#+$(info - Runner path:    $(RunnerFile))
$(info )

# DO NOT FORGET to define CXXTEST_HAVE_EH and CXXTEST_HAVE_STD to compile UT Files
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

$(RunnerFile): $(Suites) $(ProjectFile) $(TemplateFile) $(MAKEFILE_LIST)
#+	$(python) "$(Generator)" --runner=ParenPrinter --have-eh --have-std --fog-parse --root  -o $(RunnerFile) $(Suites)
	$(python) "$(Generator)" --error-printer --have-eh --have-std --fog-parse --root  -o $(RunnerFile) --template $(TemplateFile) $(Suites)
