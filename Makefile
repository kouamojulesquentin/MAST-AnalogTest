.RECIPEPREFIX = >

CMAKE_RELEASE_BUILD_DIR = cmake_release
CMAKE_DEBUG_BUILD_DIR   = cmake_debug
CMAKE_ARM_BUILD_DIR     = cmake_arm

#+CMAKE_DEBUG_BUILD_MAKEFILE =$(CMAKE_DEBUG_BUILD_DIR)/Makefile

CMAKE_FLAGS=  -DCMAKE_CXX_COMPILER=g++
LIB_DIR    = Lib

ifeq ($(OS), Windows_NT)
$(info ==> Building for Windows)
USE_OPEN_OCD     = OFF
USE_LIBFTDISPI   = OFF
INTEL_EXPERIMENT = OFF
CMAKE_FLAGS+= -G "MinGW Makefiles"

IF_EXIST = if exist

RM     = del /Q
RM_DIR = rmdir /S /Q
MKDIR  = mkdir
SEP    = "\"
RUN    =

BIN_DIR              = Bin
MAST_UT_EXE_NAME     = Mast_UT.exe
CPP_EXAMPLE_EXE_NAME = MastExample_CPP.exe
SIT_UT_EXE_NAME      = SIT_Reader_UT.exe
SIT_READER_EXE_NAME  = SIT_reader_demo.exe
TESTCASES_EXE_NAME   = TestCasesApp.exe

MAST_UT_EXE_PATH      = $(BIN_DIR)\$(MAST_UT_EXE_NAME)
SIT_UT_EXE_PATH       = $(BIN_DIR)\$(SIT_UT_EXE_NAME)
SIT_READER_EXE_PATH   = $(SIT_READER_EXE_NAME)
SIT_READER_INPUT_FILE = ..\..\SIT_reader\prova.txt
TESTCASES_EXE_PATH    = $(BIN_DIR)\$(TESTCASES_EXE_NAME)
CPP_EXAMPLE_EXE_PATH  = $(BIN_DIR)\$(CPP_EXAMPLE_EXE_NAME)

else
USE_OPEN_OCD     = ON
USE_LIBFTDISPI   = ON
INTEL_EXPERIMENT = ON
MKDIR  = mkdir -p
RM     = rm -f
RM_DIR = rm -rf
SEP    = /
RUN    = ./

BIN_DIR              = Bin
MAST_UT_EXE_NAME     = Mast_UT
TESTCASES_EXE_NAME   = TestCasesApp
CPP_EXAMPLE_EXE_NAME = MastExample_CPP
SIT_READER_EXE_NAME  = SIT_reader_demo
SIT_UT_EXE_NAME      = SIT_Reader_UT

MAST_UT_EXE_PATH      = $(BIN_DIR)/$(MAST_UT_EXE_NAME)
SIT_UT_EXE_PATH       = $(BIN_DIR)/$(SIT_UT_EXE_NAME)
SIT_READER_EXE_PATH   = $(SIT_READER_EXE_NAME)
SIT_READER_INPUT_FILE = ../../SIT_reader/prova.txt
TESTCASES_EXE_PATH    = $(BIN_DIR)/$(TESTCASES_EXE_NAME)
CPP_EXAMPLE_EXE_PATH  = $(BIN_DIR)/$(CPP_EXAMPLE_EXE_NAME)
endif

#+BUILD_UT = ON

CPP_DEFINES  = -DUSE_OPEN_OCD:BOOL=$(USE_OPEN_OCD)
CPP_DEFINES += -DUSE_LIBFTDISPI:BOOL=$(USE_LIBFTDISPI)
CPP_DEFINES += -DINTEL_EXPERIMENT:BOOL=$(INTEL_EXPERIMENT)
#+CPP_DEFINES += -DBUILD_UT:BOOL=$(BUILD_UT)

CMAKE_DEBUG_FLAGS=    -DCMAKE_BUILD_TYPE=Debug   $(CMAKE_FLAGS) $(CPP_DEFINES)
CMAKE_RELEASE_FLAGS=  -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS) $(CPP_DEFINES)



CMAKE_ARM_FLAGS  = -D CMAKE_TOOLCHAIN_FILE=Toolchain-arm.cmake
CMAKE_ARM_FLAGS += -DUSE_OPEN_OCD:BOOL=OFF
CMAKE_ARM_FLAGS += -DUSE_LIBFTDISPI:BOOL=OFF
CMAKE_ARM_FLAGS += -DINTEL_EXPERIMENT:BOOL=OFF

MAKE_FLAGS= -j4

OPENOCD_INSTALL_DIR=./openocd/

all: debug

ifeq ("$(USE_OPEN_OCD)","ON")
debug:   openocd_debug
release: openocd_release
else
debug:   debug_cmake
release: release_cmake
endif

debug_cmake:
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_DEBUG_BUILD_DIR)
> cd       $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_DEBUG_FLAGS) ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
#+> $(info ==> Makefile: Build UT:     $(BUILD_UT))
> cd $(CMAKE_DEBUG_BUILD_DIR) && make $(MAKE_FLAGS)

release_cmake:
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_RELEASE_BUILD_DIR)
> cd $(CMAKE_RELEASE_BUILD_DIR) && cmake  $(CMAKE_RELEASE_FLAGS)  ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
#+> $(info ==> Makefile: Build UT:     $(BUILD_UT))
> cd $(CMAKE_RELEASE_BUILD_DIR) && make  $(MAKE_FLAGS)

run_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(MAST_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(MAST_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Lib UT available ========"
endif

run_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(MAST_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(MAST_UT_EXE_PATH)
else
>  @echo "    ==== No Release Lib UT available ========"
endif

run_sit_ut_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(SIT_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(SIT_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Sit UT available ========"
>  @echo "    ==== $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(SIT_UT_EXE_NAME)
endif

run_sit_ut_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(SIT_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(SIT_UT_EXE_PATH)
else
>  @echo "    ==== No Release Sit UT available ========"
>  @echo "    ==== Expecting: $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(SIT_UT_EXE_NAME) ========"
endif

run_sit_reader_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(SIT_READER_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR) && $(RUN)$(SIT_READER_EXE_NAME)
else
>  @echo "    ==== No Debug parser available ========"
endif

run_sit_reader_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(SIT_READER_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR) && $(RUN)$(SIT_READER_EXE_NAME)
else
>  @echo "    ==== No Release parser available ========"
endif

run_testcases_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(TESTCASES_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(TESTCASES_EXE_PATH)
else
>  @echo "    ==== No Debug testcases available ========"
endif

run_testcases_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(TESTCASES_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(TESTCASES_EXE_PATH)
else
>  @echo "    ==== No Release testcases available ========"
endif

run_cpp_example_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(CPP_EXAMPLE_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(CPP_EXAMPLE_EXE_PATH)
else
>  @echo "    ==== No Debug cpp_example available ========"
endif

run_cpp_example_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(CPP_EXAMPLE_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(CPP_EXAMPLE_EXE_PATH)
else
>  @echo "    ==== No Release cpp_example available ========"
endif

ifeq ("$(USE_OPEN_OCD)","ON")
openocd_debug: debug_cmake $(OPENOCD_INSTALL_DIR)/lib/libopenocd.so $(OPENOCD_INSTALL_DIR)/openocd-ft2232.cfg
> cp -f $(OPENOCD_INSTALL_DIR)/lib/libopenocd.so $(CMAKE_DEBUG_BUILD_DIR)/$(LIB_DIR)/
> cp -f $(OPENOCD_INSTALL_DIR)/openocd-ft2232.cfg $(CMAKE_DEBUG_BUILD_DIR)/$(LIB_DIR)/
> export MAST_CONFIGURATION_PATH="$(PWD)/$(CMAKE_DEBUG_BUILD_DIR)/$(LIB_DIR)/"
endif

ifeq ("$(USE_OPEN_OCD)","ON")
openocd_release: release_cmake $(OPENOCD_INSTALL_DIR)/lib/libopenocd.so $(OPENOCD_INSTALL_DIR)/openocd-ft2232.cfg
> cp -f $(OPENOCD_INSTALL_DIR)/lib/libopenocd.so $(CMAKE_RELEASE_BUILD_DIR)/$(LIB_DIR)/
> cp -f $(OPENOCD_INSTALL_DIR)/openocd-ft2232.cfg $(CMAKE_RELEASE_BUILD_DIR)/$(LIB_DIR)/
> export MAST_CONFIGURATION_PATH="$(PWD)/$(CMAKE_DEBUG_BUILD_DIR)/$(LIB_DIR)/"
endif

run_arm:
>  @echo "    ==== No run defined for ARM build (needs qemu) ========"

arm:
ifeq ("$(wildcard $(CMAKE_ARM_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_ARM_BUILD_DIR)
> cd $(CMAKE_ARM_BUILD_DIR) && cmake  $(CMAKE_ARM_FLAGS)  ..
endif
> cd $(CMAKE_ARM_BUILD_DIR) && make  $(MAKE_FLAGS)

clean:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/Makefile)","")
> cd $(CMAKE_DEBUG_BUILD_DIR) &&  make clean;
endif
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/Makefile)","")
> cd $(CMAKE_RELEASE_BUILD_DIR) &&  make clean;
endif

docs:
ifeq ($(OS), Windows_NT)
> $(IF_EXIST) Doxygen_Doc$(SEP)MastDev$(SEP)html  $(RM_DIR) Doxygen_Doc$(SEP)MastDev$(SEP)html
> $(IF_EXIST) Doxygen_Doc$(SEP)FULL_API$(SEP)html $(RM_DIR) Doxygen_Doc$(SEP)FULL_API$(SEP)html
> $(IF_EXIST) Doxygen_Doc$(SEP)CPP_API$(SEP)html  $(RM_DIR) Doxygen_Doc$(SEP)CPP_API$(SEP)html
> $(IF_EXIST) Doxygen_Doc$(SEP)C_API$(SEP)html    $(RM_DIR) Doxygen_Doc$(SEP)C_API$(SEP)html
else
> $(RM_DIR)   Doxygen_Doc$(SEP)html
endif
> doxygen Doxyfile_MastDev.cfg
> doxygen Doxyfile_FULL_API.cfg
> doxygen Doxyfile_CPP_API.cfg
> doxygen Doxyfile_C_API.cfg

distclean:
ifeq ($(OS), Windows_NT)
> $(IF_EXIST) Mast_UT$(SEP)Generated$(SEP)Runner.cpp       $(RM)     Mast_UT$(SEP)Generated$(SEP)Runner.cpp
> $(IF_EXIST) SIT_reader_UT$(SEP)Generated$(SEP)Runner.cpp $(RM)     SIT_reader_UT$(SEP)Generated$(SEP)Runner.cpp
> $(IF_EXIST) $(CMAKE_RELEASE_BUILD_DIR)                   $(RM_DIR) $(CMAKE_RELEASE_BUILD_DIR)
> $(IF_EXIST) $(CMAKE_DEBUG_BUILD_DIR)                     $(RM_DIR) $(CMAKE_DEBUG_BUILD_DIR)
> $(IF_EXIST) $(CMAKE_ARM_BUILD_DIR)                       $(RM_DIR) $(CMAKE_ARM_BUILD_DIR)
else
> $(RM)     Mast_UT$(SEP)Generated$(SEP)Runner.cpp
> $(RM)     SIT_reader_UT$(SEP)Generated$(SEP)Runner.cpp
> $(RM_DIR) $(CMAKE_RELEASE_BUILD_DIR)
> $(RM_DIR) $(CMAKE_DEBUG_BUILD_DIR)
> $(RM_DIR) $(CMAKE_ARM_BUILD_DIR)
endif

