# Runner.mak
# Builds a CxxTest runner

$(info )
$(info ====== Runner.mak ======)
$(info )

#+$(info RootPath: $(RootPath))
ifeq ($(RootPath), )
    RootPath     = .
endif

UT_RootPath	 = $(RootPath)

Suites = \
           $(UT_RootPath)/Utility/UT_BinaryVector.hpp                  \
           $(UT_RootPath)/Utility/UT_SVFVector.hpp                     \
           $(UT_RootPath)/SystemModel/UT_Register.hpp                  \
           $(UT_RootPath)/SystemModel/UT_DefaultBinaryPathSelector.hpp \
           $(UT_RootPath)/SystemModel/UT_SystemModel.hpp               \
           $(UT_RootPath)/SystemModel/UT_PrettyPrinterVisitor.hpp      \
           $(UT_RootPath)/SystemModel/UT_GmlPrinterVisitor.hpp


ProjectFile  = $(RootPath)/Lib_UT.cbp

#+Generator    = ..\..\CxxTest\bin\cxxtestgen.py
Generator    = $(RootPath)/../cxxtest/bin/cxxtestgen.py
RunnerFile   = $(UT_RootPath)/Generated/Runner.cpp
TemplateFile = $(UT_RootPath)/Runner.tpl
TestListener = ParenPrinter


# cxxtestgen needs Python
ifeq ($(OS), Windows_NT)
  python=$(Python3)
else
  python=python3
endif


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
	$(python) "$(Generator)" --error-printer --have-eh --have-std --fog-parse --root  -o $(RunnerFile) --template $(TemplateFile) $(Suites)
