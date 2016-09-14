.RECIPEPREFIX = >

CMAKE_RELEASE_BUILD_DIR = cmake_release
CMAKE_DEBUG_BUILD_DIR   = cmake_debug
CMAKE_ARM_BUILD_DIR     = cmake_arm

#+CMAKE_DEBUG_BUILD_MAKEFILE =$(CMAKE_DEBUG_BUILD_DIR)/Makefile

CMAKE_FLAGS=  -DCMAKE_CXX_COMPILER=g++

ifeq ($(OS), Windows_NT)
$(info ==> Building for Windows)
USE_OPEN_OCD = OFF
CMAKE_FLAGS+= -G "MinGW Makefiles"

IF_EXIST = if exist

RM     = del /Q
RM_DIR = rmdir /S /Q
MKDIR  = mkdir
SEP    = "\"
RUN    =

BIN_DIR             = Bin
MAST_UT_EXE_NAME    = Mast_UT.exe
SIT_UT_EXE_NAME     = SIT_Reader_UT.exe
SIT_READER_EXE_NAME = SIT_reader_demo.exe
TESTCASES_EXE_NAME  = TestCasesApp.exe

MAST_UT_EXE_PATH      = $(BIN_DIR)\$(MAST_UT_EXE_NAME)
SIT_UT_EXE_PATH       = $(BIN_DIR)\$(SIT_UT_EXE_NAME)
SIT_READER_EXE_PATH   = $(SIT_READER_EXE_NAME)
SIT_READER_INPUT_FILE = ..\..\SIT_reader\prova.txt
TESTCASES_EXE_PATH    = $(BIN_DIR)\$(TESTCASES_EXE_NAME)

else
USE_OPEN_OCD = ON
MKDIR = mkdir -p
RM     = rm -f
RM_DIR = rm -rf
SEP   = /
RUN   = ./

BIN_DIR             = Bin
MAST_UT_EXE_NAME    = Mast_UT
TESTCASES_EXE_NAME  = TestCasesApp
SIT_READER_EXE_NAME = SIT_reader_demo
SIT_UT_EXE_NAME     = SIT_Reader_UT

MAST_UT_EXE_PATH      = $(BIN_DIR)/$(MAST_UT_EXE_NAME)
SIT_UT_EXE_PATH       = $(BIN_DIR)/$(SIT_UT_EXE_NAME)
SIT_READER_EXE_PATH   = $(SIT_READER_EXE_NAME)
SIT_READER_INPUT_FILE = ../../SIT_reader/prova.txt
TESTCASES_EXE_PATH    = $(BIN_DIR)/$(TESTCASES_EXE_NAME)
endif

BUILD_UT = ON

CMAKE_DEBUG_FLAGS=    -DCMAKE_BUILD_TYPE=Debug   $(CMAKE_FLAGS) -DUSE_OPEN_OCD:BOOL=$(USE_OPEN_OCD) -DBUILD_UT:BOOL=$(BUILD_UT)
CMAKE_RELEASE_FLAGS=  -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS) -DUSE_OPEN_OCD:BOOL=$(USE_OPEN_OCD) -DBUILD_UT:BOOL=$(BUILD_UT)


CMAKE_ARM_FLAGS= -D CMAKE_TOOLCHAIN_FILE=Toolchain-arm.cmake -DUSE_OPEN_OCD:BOOL=OFF

MAKE_FLAGS= -j4

all: debug

debug:
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_DEBUG_BUILD_DIR)
> cd       $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_DEBUG_FLAGS) ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
> $(info ==> Makefile: Build UT:     $(BUILD_UT))
> cd $(CMAKE_DEBUG_BUILD_DIR) && make $(MAKE_FLAGS)

release:
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_RELEASE_BUILD_DIR)
> cd $(CMAKE_RELEASE_BUILD_DIR) && cmake  $(CMAKE_RELEASE_FLAGS)  ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
> $(info ==> Makefile: Build UT:     $(BUILD_UT))
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
>  cd $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR) && $(RUN)$(SIT_READER_EXE_PATH)    $(SIT_READER_INPUT_FILE)
else
>  @echo "    ==== No Debug parser available ========"
endif

run_sit_reader_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(SIT_READER_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR) && $(RUN)$(SIT_READER_EXE_PATH)    $(SIT_READER_INPUT_FILE)
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

