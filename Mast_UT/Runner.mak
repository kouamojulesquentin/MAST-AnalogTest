# Runner.mak
# Builds a CxxTest runner

$(info )
$(info ====== Runner.mak ======)
$(info )

.RECIPEPREFIX = >

#+$(info RootPath: $(RootPath))
ifeq ($(RootPath), )
  RootPath   = .
endif

UT_RootPath  = $(RootPath)

Suites = \
      $(UT_RootPath)/Utility/UT_Utility.hpp                              \
      $(UT_RootPath)/Utility/UT_BinaryVector.hpp                         \
      $(UT_RootPath)/Utility/UT_SVFVector.hpp                            \
      $(UT_RootPath)/Utility/UT_MastConfiguration.hpp                    \
      $(UT_RootPath)/SystemModel/UT_SystemModelNode.hpp                  \
      $(UT_RootPath)/SystemModel/UT_Register.hpp                         \
      $(UT_RootPath)/SystemModel/UT_ParentNode.hpp                       \
      $(UT_RootPath)/SystemModel/UT_NodePathResolver.hpp                 \
      $(UT_RootPath)/SystemModel/UT_DefaultTableBasedPathSelector.hpp    \
      $(UT_RootPath)/SystemModel/UT_DefaultBinaryPathSelector.hpp        \
      $(UT_RootPath)/SystemModel/UT_DefaultOneHotPathSelector.hpp        \
      $(UT_RootPath)/SystemModel/UT_DefaultNHotPathSelector.hpp          \
      $(UT_RootPath)/SystemModel/UT_SystemModel.hpp                      \
      $(UT_RootPath)/SystemModel/UT_SystemModelBuilder.hpp               \
      $(UT_RootPath)/SystemModel/UT_NamesChecker.hpp                     \
      $(UT_RootPath)/SystemModel/UT_SystemModelChecker.hpp               \
      $(UT_RootPath)/SystemModel/UT_PrettyPrinter.hpp                    \
      $(UT_RootPath)/SystemModel/UT_GmlPrinter.hpp                       \
      $(UT_RootPath)/UT_Helpers/UT_TestModelBuilder.hpp                  \
      $(UT_RootPath)/SystemModel/UT_PropagatePendingVisitor.hpp          \
      $(UT_RootPath)/SystemModel/UT_ConfigureVisitor.hpp                 \
      $(UT_RootPath)/SystemModel/UT_FromSutUpdater.hpp                   \
      $(UT_RootPath)/SystemModel/UT_ToSutVisitor.hpp                     \
      $(UT_RootPath)/SystemModel/UT_DataCycle.hpp                        \
      $(UT_RootPath)/SystemModel/UT_GenericAccessInterfaceProtocol.hpp   \
      $(UT_RootPath)/SystemModel/UT_SVF_SimulationProtocol.hpp           \
      $(UT_RootPath)/SystemModel/UT_I2C_EmulationProtocol.hpp            \
      $(UT_RootPath)/SystemModel/UT_STIL_EmulationProtocol.hpp           \
      $(UT_RootPath)/SystemModel/UT_Remote_Protocol_Proxy.hpp            \
      $(UT_RootPath)/SystemModel/UT_AccessInterfaceProtocolFactory.hpp   \
      $(UT_RootPath)/SystemModel/UT_ConfigureAlgorithmFactory.hpp        \
      $(UT_RootPath)/SystemModel/UT_PathSelectorFactory.hpp              \
      $(UT_RootPath)/SystemModel/UT_SystemModelManager.hpp               \
      $(UT_RootPath)/API_CPP/UT_PDL_Adapter_CPP.hpp                      \
      $(UT_RootPath)/API_C/UT_PDL_Adapter_C.hpp                          \
      $(UT_RootPath)/Yaml/UT_YamlFile.hpp

ProjectFile  = $(RootPath)/Mast_UT.pro

#+Generator   = ..\..\CxxTest\bin\cxxtestgen.py
Generator    = $(RootPath)/../cxxtest/bin/cxxtestgen.py
RunnerFile   = $(UT_RootPath)/Generated/Runner.cpp
TemplateFile = $(UT_RootPath)/Runner.tpl
TestListener = ParenPrinter

$(info Variable OS is: '$(OS)')

#+ifeq ($(OS), Windows_NT)
#+ $(info Python3: '$(PYTHON3)')
#+endif

# cxxtestgen needs Python
ifeq ($(OS), Windows_NT)
 $(info Running on Windows)
 python=$(PYTHON3)
 ifeq ($(wildcard $(python)),)
   $(info PYTHON3 environment variable is not defined ==> Will try with Python3)
   python=$(Python3)
 endif
 ifeq ($(wildcard $(python)),)
  $(warning Python is not defined)
 else
  $(info Using python:  $(python))
 endif
 DEL_FILE   = del
 RunnerFile = $(UT_RootPath)\Generated\Runner.cpp
else
 $(info Running on: Linux)
 python=python3
 DEL_FILE    = rm
 RunnerFile  = $(UT_RootPath)/Generated/Runner.cpp
endif




#To debug variables

#+test = $(wildcard $(MAKEFILE_LIST))
#+$(info Test: $(test))
#+$(info )

#+$(info Variables:)
#+$(info - CURDIR:     $(CURDIR))
#+$(info - MAKEFILE_LIST:  $(MAKEFILE_LIST))
$(info - RootPath:    $(RootPath))
$(info - Python:      $(python))
#+$(info - Generator:    $(Generator))
$(info - Runner path: $(RunnerFile))
$(info )

# DO NOT FORGET to define CXXTEST_HAVE_EH and CXXTEST_HAVE_STD to compile UT Files
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

$(RunnerFile): $(Suites) $(ProjectFile) $(TemplateFile)
> $(python) "$(Generator)" --error-printer --have-eh --have-std --fog-parse --root -o $(RunnerFile) --template $(TemplateFile) $(Suites)

clean:
> $(DEL_FILE) $(RunnerFile)

$(info ====== End of Runner.mak ======)
$(info )
