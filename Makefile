.RECIPEPREFIX = >

#LOCAL_GCC_PATH = /home/michele/local_gcc-4.9.3/bin/
CMAKE_RELEASE_BUILD_DIR       = cmake_release
CMAKE_DEBUG_BUILD_DIR         = cmake_debug
CMAKE_CODE_COVERAGE_BUILD_DIR = cmake_code_coverage
CMAKE_ARM_BUILD_DIR           = cmake_arm
CMAKE_RISCV32_BUILD_DIR       = cmake_riscV32

#+CMAKE_DEBUG_BUILD_MAKEFILE =$(CMAKE_DEBUG_BUILD_DIR)/Makefile

CMAKE_FLAGS = -DCMAKE_CXX_COMPILER=g++
LIB_DIR     = Lib

# ----------------- Defines what differs include platforms
#
ifeq ($(OS), Windows_NT)
$(info ==> Building for Windows)
USE_OPEN_OCD  = OFF
CMAKE_FLAGS  += -G "MinGW Makefiles"
PYTHON = python

MKDIR   = mkdir
RUN     =
EXT     = .exe
BIN_DIR = Bin

# Following trick is to get a single '\' for Windows
SEP    = \\
SEP   := $(strip $(SEP:\\=\))

else # ==> not Windows
USE_OPEN_OCD = ON
PYTHON       = python3

MKDIR   = mkdir -p
RUN     = ./
EXT     =
BIN_DIR = Bin
SEP     = /
endif


# ----------------- Defines exe names and paths
#
MAST_CORE_UT_EXE_NAME := Mast_Core_UT$(EXT)
MAST_API_UT_EXE_NAME  := Mast_API_UT$(EXT)
EXTERNAL_UT_EXE_NAME  := Externals_UT$(EXT)
OPTIONAL_UT_EXE_NAME  := Optionals_UT$(EXT)
TESTCASES_EXE_NAME    := TestCasesApp$(EXT)
CPP_EXAMPLE_EXE_NAME  := MastExample_CPP$(EXT)
READERS_UT_EXE_NAME   := Readers_UT$(EXT)

MAST_CORE_UT_EXE_PATH := $(BIN_DIR)$(SEP)$(MAST_CORE_UT_EXE_NAME)
MAST_API_UT_EXE_PATH  := $(BIN_DIR)$(SEP)$(MAST_API_UT_EXE_NAME)
EXTERNAL_UT_EXE_PATH  := $(BIN_DIR)$(SEP)$(EXTERNAL_UT_EXE_NAME)
OPTIONAL_UT_EXE_PATH  := $(BIN_DIR)$(SEP)$(OPTIONAL_UT_EXE_NAME)
READERS_UT_EXE_PATH   := $(BIN_DIR)$(SEP)$(READERS_UT_EXE_NAME)
TESTCASES_EXE_PATH    := $(BIN_DIR)$(SEP)$(TESTCASES_EXE_NAME)
CPP_EXAMPLE_EXE_PATH  := $(BIN_DIR)$(SEP)$(CPP_EXAMPLE_EXE_NAME)

ifneq ("$(wildcard Makefile.local)","")
include Makefile.local
endif


CPP_DEFINES += -DUSE_OPEN_OCD:BOOL=$(USE_OPEN_OCD)

CMAKE_DEBUG_FLAGS         =  -DCMAKE_BUILD_TYPE=Debug   $(CMAKE_FLAGS) $(CPP_DEFINES)
CMAKE_RELEASE_FLAGS       =  -DCMAKE_BUILD_TYPE=Release $(CMAKE_FLAGS) $(CPP_DEFINES)
CMAKE_CODE_COVERAGE_FLAGS = $(CMAKE_DEBUG_FLAGS) -DCODE_COVERAGE:BOOL=ON

CMAKE_CENTOS_FLAGS =  -DCMAKE_BUILD_TYPE=Debug   $(CMAKE_FLAGS)
CMAKE_CENTOS_FLAGS += -DCMAKE_CXX_COMPILER="/home/michele/local_gcc-4.9.3/bin/g++"
CMAKE_CENTOS_FLAGS += -DCMAKE_C_COMPILER="/home/michele/local_gcc-4.9.3/bin/gcc"

CMAKE_CENTOS_FLAGS += -DUSE_OPEN_OCD:BOOL=OFF

CMAKE_ARM_FLAGS  = -D CMAKE_TOOLCHAIN_FILE=Toolchain-arm.cmake
CMAKE_ARM_FLAGS += -DUSE_OPEN_OCD:BOOL=OFF

CMAKE_RISCV32_FLAGS  = -D CMAKE_TOOLCHAIN_FILE=Toolchain-riscV32.cmake
CMAKE_RISCV32_FLAGS += -DUSE_OPEN_OCD:BOOL=OFF

MAKE_FLAGS= -j4

OPENOCD_INSTALL_DIR=./External_Libs/OpenOCD/

# ----------------- Detect when install must be done
#
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/SIT)","")
  DO_DEBUG_INSTALL = TRUE
else
  DO_DEBUG_INSTALL = FALSE
endif
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/SIT)","")
  DO_RELEASE_INSTALL = TRUE
else
  DO_RELEASE_INSTALL = FALSE
endif

all:     debug
install: install_debug
pack:    pack_debug

ifneq ($(LOCAL_GCC_PATH),)
set_compiler:
> @echo "STATUS: Makefile found a local GCC/G++"
CMAKE_COMPILER_FLAGS  = -DCMAKE_CXX_COMPILER="$(LOCAL_GCC_PATH)g++"
CMAKE_COMPILER_FLAGS += -DCMAKE_C_COMPILER="$(LOCAL_GCC_PATH)gcc"
CMAKE_COMPILER_FLAGS += -DLOCAL_GCC_PATH="$(LOCAL_GCC_PATH)"
else
set_compiler:
> @echo "STATUS: Using system-wide GCC/G++"
> @echo "STATUS: LOCAL_GCC_PATH "$(LOCAL_GCC_PATH)
CMAKE_COMPILER_FLAGS = ""
endif

ifeq ("$(USE_OPEN_OCD)","ON")
debug:   openocd_debug
release: openocd_release
else
debug:   debug_cmake
release: release_cmake
endif

external_libs: xmlrpc-c

xmlrpc-c:
> cd  $(EXTDIR_ROOT_DIR)/$(XMLRPC_ROOT_DIR) && ./configure  --prefix=$(PWD)/$(EXTDIR_ROOT_DIR)/$(EXTDIR_INSTALL_DIR)/$(XMLRPC_ROOT_DIR)
> cd  $(EXTDIR_ROOT_DIR)/$(XMLRPC_ROOT_DIR) && make && make install


debug_cmake: set_compiler
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_DEBUG_BUILD_DIR)
> cd       $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_COMPILER_FLAGS) $(CMAKE_DEBUG_FLAGS) ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
#+> $(info ==> Makefile: Build UT:     $(BUILD_UT))
> cd $(CMAKE_DEBUG_BUILD_DIR) && make $(MAKE_FLAGS)
ifeq ($(DO_DEBUG_INSTALL),TRUE)
> cd $(CMAKE_DEBUG_BUILD_DIR)   && make install
endif

release_cmake: set_compiler
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_RELEASE_BUILD_DIR)
> cd $(CMAKE_RELEASE_BUILD_DIR) && cmake  $(CMAKE_COMPILER_FLAGS)  $(CMAKE_RELEASE_FLAGS)  ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
#+> $(info ==> Makefile: Build UT:     $(BUILD_UT))
> cd $(CMAKE_RELEASE_BUILD_DIR) && make  $(MAKE_FLAGS)
ifeq ($(DO_RELEASE_INSTALL),TRUE)
> cd $(CMAKE_RELEASE_BUILD_DIR)   && make install
endif

code_coverage_build: set_compiler
ifeq ("$(wildcard $(CMAKE_CODE_COVERAGE_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_CODE_COVERAGE_BUILD_DIR)
> cd       $(CMAKE_CODE_COVERAGE_BUILD_DIR) && cmake $(CMAKE_COMPILER_FLAGS) $(CMAKE_CODE_COVERAGE_FLAGS) ..
endif
> cd $(CMAKE_CODE_COVERAGE_BUILD_DIR) && make $(MAKE_FLAGS)
ifeq ("$(wildcard $(CMAKE_CODE_COVERAGE_BUILD_DIR)/$(BIN_DIR)/SIT)","")
> cd $(CMAKE_CODE_COVERAGE_BUILD_DIR)   && make install
endif

code_coverage_run: code_coverage_build
ifeq ("$(wildcard $(CMAKE_CODE_COVERAGE_BUILD_DIR)/$(BIN_DIR)/$(MAST_CORE_UT_EXE_NAME))","")
> $(error     ==== No Mast UT available for Code Coverage ========)
endif
ifeq ("$(wildcard $(CMAKE_CODE_COVERAGE_BUILD_DIR)/$(BIN_DIR)/$(READERS_UT_EXE_NAME))","")
> $(error     ==== No Readers UT available for Code Coverage ========)
endif
> cd $(CMAKE_CODE_COVERAGE_BUILD_DIR) && $(RUN)$(MAST_CORE_UT_EXE_PATH)
> cd $(CMAKE_CODE_COVERAGE_BUILD_DIR) && $(RUN)$(MAST_API_UT_EXE_PATH)
> cd $(CMAKE_CODE_COVERAGE_BUILD_DIR) && $(RUN)$(READERS_UT_EXE_PATH)

CODE_COVERAGE_EXCLUDED      = --gcov-exclude=".*(SIT_reader.UnresolvedPathSelector.hpp).*"
CODE_COVERAGE_FILTERS       = --gcov-filter=".*(Mast_Core|Mast_API_CPP|Mast_API_C|Readers).*"
CODE_COVERAGE_OUTPUT        = -o CodeCoverage/CodeCoverage.html
#+CODE_COVERAGE_SOURCE_DIR    = --root=$(PWD)
CODE_COVERAGE_SOURCE_DIR    = --root=.
CODE_COVERAGE_OBJECT_DIR    = --object-directory="$(CMAKE_CODE_COVERAGE_BUILD_DIR)"
CODE_COVERAGE_OUTPUT_FORMAT = --html --html-details --print-summary --print-tabular
CODE_COVERAGE_PARAMETERS    = --exclude-unreachable-branches $(CODE_COVERAGE_OUTPUT_FORMAT) $(CODE_COVERAGE_FILTERS) $(CODE_COVERAGE_EXCLUDED) $(CODE_COVERAGE_OUTPUT) $(CODE_COVERAGE_OBJECT_DIR) $(CODE_COVERAGE_SOURCE_DIR)

code_coverage_report: code_coverage_run
ifeq ("$(wildcard CodeCoverage)","")
> $(MKDIR) CodeCoverage
endif
> $(info CODE_COVERAGE_PARAMETERS: $(CODE_COVERAGE_PARAMETERS))
> $(PYTHON) gcovr.py $(CODE_COVERAGE_PARAMETERS)
#+> $(PYTHON) gcovr.py --verbose --verbose_debug $(CODE_COVERAGE_PARAMETERS)

code_coverage_install:
> cd $(CMAKE_CODE_COVERAGE_BUILD_DIR) && make install

code_coverage_clean:
> cmake -E remove_directory $(CMAKE_CODE_COVERAGE_BUILD_DIR)
> cmake -E remove_directory CodeCoverage


centos:
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_DEBUG_BUILD_DIR)
> cd       $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_CENTOS_FLAGS) ..
endif
> $(info ==> Makefile: Use Open OCD: $(USE_OPEN_OCD))
#+> $(info ==> Makefile: Build UT:     $(BUILD_UT))
> cd $(CMAKE_DEBUG_BUILD_DIR) && make $(MAKE_FLAGS)

install: install_debug
install_debug:
> cd $(CMAKE_DEBUG_BUILD_DIR)   && make install

install_release:
> cd $(CMAKE_RELEASE_BUILD_DIR) && make install

pack: pack_debug
pack_debug: set_compiler
> echo "Gcc path: "$(LOCAL_GCC_PATH)
> cd $(CMAKE_DEBUG_BUILD_DIR)   && cpack -G TGZ

test: test_debug
test_debug:
> cd $(CMAKE_DEBUG_BUILD_DIR) && ctest -j4 --output-on-failure

test_release:
> cd $(CMAKE_RELEASE_BUILD_DIR) && ctest -j4 --output-on-failure

run_all:         run_all_debug
run_all_debug:   run_debug   run_api_debug   run_readers_ut_debug
run_all_release: run_release run_api_release run_readers_ut_release

run_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(MAST_CORE_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(MAST_CORE_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Mast Core UT available ========"
endif

run_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(MAST_CORE_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(MAST_CORE_UT_EXE_PATH)
else
>  @echo "    ==== No Release Mast Core UT available ========"
endif

run_api_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(MAST_API_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(MAST_API_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Mast API UT available ========"
endif

run_api_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(MAST_API_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(MAST_API_UT_EXE_PATH)
else
>  @echo "    ==== No Release Mast API UT available ========"
endif


run_external_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(EXTERNAL_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(EXTERNAL_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Externals Libs UT available ========"
endif

run_external_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(EXTERNAL_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(EXTERNAL_UT_EXE_PATH)
else
>  @echo "    ==== No Release Optionals Libs UT available ========"
endif


run_optionals_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(OPTIONAL_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(OPTIONAL_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Optionals Libs UT available ========"
endif

run_optionals_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(OPTIONAL_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(OPTIONAL_UT_EXE_PATH)
else
>  @echo "    ==== No Release Optionals Libs UT available ========"
endif

run_readers: run_readers_ut_debug

run_readers_ut_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(READERS_UT_EXE_NAME))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && $(RUN)$(READERS_UT_EXE_PATH)
else
>  @echo "    ==== No Debug Readers UT available ========"
>  @echo "    ==== $(CMAKE_DEBUG_BUILD_DIR)/$(BIN_DIR)/$(READERS_UT_EXE_NAME)
endif

run_readers_ut_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(READERS_UT_EXE_NAME))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && $(RUN)$(READERS_UT_EXE_PATH)
else
>  @echo "    ==== No Release Readers UT available ========"
>  @echo "    ==== Expecting: $(CMAKE_RELEASE_BUILD_DIR)/$(BIN_DIR)/$(READERS_UT_EXE_NAME) ========"
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


riscV32:
ifeq ("$(wildcard $(CMAKE_RISCV32_BUILD_DIR))","")
> $(MKDIR) $(CMAKE_RISCV32_BUILD_DIR)
> cd $(CMAKE_RISCV32_BUILD_DIR) && cmake  $(CMAKE_RISCV32_FLAGS)  ..
endif
> cd $(CMAKE_RISCV32_BUILD_DIR) && make  $(MAKE_FLAGS)

clean:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/Makefile)","")
> cd $(CMAKE_DEBUG_BUILD_DIR) &&  make clean;
endif
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/Makefile)","")
> cd $(CMAKE_RELEASE_BUILD_DIR) &&  make clean;
endif

docs:
> cmake   -E remove_directory Doxygen_Doc/MastDev/html
> cmake   -E remove_directory Doxygen_Doc/FULL_API/html
> cmake   -E remove_directory Doxygen_Doc/CPP_API/html
> cmake   -E remove_directory Doxygen_Doc/C_API/html
> doxygen Doxyfile_MastDev.cfg
> doxygen Doxyfile_FULL_API.cfg
> doxygen Doxyfile_CPP_API.cfg
> doxygen Doxyfile_C_API.cfg

distclean: code_coverage_clean
> cmake -E remove_directory $(CMAKE_DEBUG_BUILD_DIR)
> cmake -E remove_directory $(CMAKE_RELEASE_BUILD_DIR)
> cmake -E remove_directory $(CMAKE_ARM_BUILD_DIR)
> cmake -E remove_directory $(CMAKE_RISCV32_BUILD_DIR)
> cmake -E remove -f Mast_Core_UT/Generated/Runner.cpp
> cmake -E remove -f Mast_API_UT/Generated/Runner.cpp
> cmake -E remove -f Readers_UT/Generated/Runner.cpp
> cmake -E remove -f External_Libs_UT/Generated/Runner.cpp
> cmake -E remove -f Optional_Libs_UT/Generated/Runner.cpp

# ----------------- Displays list of targets
#
# Usage: make -s targets
#
targets:
> cmake -E echo List of targets:
> cmake -E echo all
> cmake -E echo arm
> cmake -E echo centos
> cmake -E echo clean
> cmake -E echo code_coverage_build
> cmake -E echo code_coverage_clean
> cmake -E echo code_coverage_install
> cmake -E echo code_coverage_report
> cmake -E echo code_coverage_run
> cmake -E echo debug
> cmake -E echo distclean
> cmake -E echo docs
> cmake -E echo external_libs
> cmake -E echo install
> cmake -E echo install_debug
> cmake -E echo install_release
> cmake -E echo openocd_debug
> cmake -E echo openocd_release
> cmake -E echo pack
> cmake -E echo pack_debug
> cmake -E echo release
> cmake -E echo riscV32
> cmake -E echo run_all
> cmake -E echo run_all_debug
> cmake -E echo run_api_debug
> cmake -E echo run_debug
> cmake -E echo run_arm
> cmake -E echo run_cpp_example_debug
> cmake -E echo run_cpp_example_release
> cmake -E echo run_all_release
> cmake -E echo run_api_release
> cmake -E echo run_release
> cmake -E echo run_external_debug
> cmake -E echo run_external_release
> cmake -E echo run_optionals_debug
> cmake -E echo run_optionals_release
> cmake -E echo run_readers
> cmake -E echo run_readers_ut_debug
> cmake -E echo run_readers_ut_release
> cmake -E echo run_testcases_release
> cmake -E echo set_compiler
> cmake -E echo test
> cmake -E echo test_debug
> cmake -E echo test_release
