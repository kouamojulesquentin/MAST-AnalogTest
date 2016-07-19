.RECIPEPREFIX = >

CMAKE_RELEASE_BUILD_DIR = cmake_release
CMAKE_DEBUG_BUILD_DIR   = cmake_debug
CMAKE_ARM_BUILD_DIR     = cmake_arm

#+CMAKE_DEBUG_BUILD_MAKEFILE =$(CMAKE_DEBUG_BUILD_DIR)/Makefile

CMAKE_FLAGS=  -DCMAKE_CXX_COMPILER=g++

ifeq ($(OS), Windows_NT)
$(info ==> Building for Windows)
CMAKE_FLAGS+= -G "MinGW Makefiles"
RM    = rmdir /S /Q
MKDIR = mkdir
RUN   =

BIN_DIR           = Bin
LIB_UT_EXE_NAME   = Lib_UT.exe
PARSER_EXE_NAME   = SIT_parser_demo.exe
LIB_UT_EXE_PATH   = $(BIN_DIR)\$(LIB_UT_EXE_NAME)
PARSER_EXE_PATH   = $(PARSER_EXE_NAME)
PARSER_INPUT_FILE = ..\..\SIT_parser\prova.txt
else
MKDIR = mkdir -p
RM    = rm -rf
RUN   = ./

BIN_DIR           = Bin
LIB_UT_EXE_NAME   = Lib_UT
PARSER_EXE_NAME   = SIT_parser_demo
LIB_UT_EXE_PATH   = $(BIN_DIR)/$(LIB_UT_EXE_NAME)
PARSER_EXE_PATH   = $(PARSER_EXE_NAME)
PARSER_INPUT_FILE = ../../SIT_parser/prova.txt
endif

CMAKE_DEBUG_FLAGS=    -DCMAKE_BUILD_TYPE=Debug   $(CMAKE_FLAGS)
CMAKE_RELEASE_FLAGS=  -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS)


CMAKE_ARM_FLAGS= -D CMAKE_TOOLCHAIN_FILE=Toolchain-arm.cmake

MAKE_FLAGS= -j4

all: debug

debug:
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_DEBUG_BUILD_DIR)
> cd       $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_DEBUG_FLAGS) ..
endif
> cd $(CMAKE_DEBUG_BUILD_DIR) && make $(MAKE_FLAGS)

release:
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_RELEASE_BUILD_DIR)
> cd $(CMAKE_RELEASE_BUILD_DIR) && cmake  $(CMAKE_RELEASE_FLAGS)  ..
endif
> cd $(CMAKE_RELEASE_BUILD_DIR) && make  $(MAKE_FLAGS)

run_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(LIB_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(LIB_UT_EXE_PATH)
else
>  @echo "    ====No Debug Lib UT available ========"
endif

run_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(LIB_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(LIB_UT_EXE_PATH)
else
>  @echo "    ====No Release Lib UT available ========"
endif

run_parser_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(PARSER_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR) && $(RUN)$(PARSER_EXE_PATH)    $(PARSER_INPUT_FILE)
else
>  @echo "    ====No Debug parser available ========"
endif

run_parser_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(PARSER_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR) && $(RUN)$(PARSER_EXE_PATH)    $(PARSER_INPUT_FILE)
else
>  @echo "    ====No Release parser available ========"
endif

run_arm:
>  @echo "		====No run defined for ARM build (needs qemu) ========"

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
> $(RM) $(CMAKE_RELEASE_BUILD_DIR) $(CMAKE_DEBUG_BUILD_DIR) $(CMAKE_ARM_BUILD_DIR)
