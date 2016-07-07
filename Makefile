.RECIPEPREFIX = >

CMAKE_RELEASE_BUILD_DIR = cmake_release
CMAKE_DEBUG_BUILD_DIR = cmake_debug
CMAKE_ARM_BUILD_DIR = cmake_arm

CMAKE_DEBUG_BUILD_MAKEFILE =$(CMAKE_DEBUG_BUILD_DIR)/Makefile

CMAKE_FLAGS=  -DCMAKE_CXX_COMPILER=g++

CMAKE_DEBUG_FLAGS=  -DCMAKE_BUILD_TYPE=Debug $(CMAKE_FLAGS)
CMAKE_RELEASE_FLAGS=  -DCMAKE_BUILD_TYPE=Debug $(CMAKE_FLAGS)

CMAKE_ARM_FLAGS=  -D CMAKE_TOOLCHAIN_FILE=Toolchain-arm.cmake

MAKE_FLAGS= -j4

RUNNER=Lib_UT_Runner

all: debug

debug:
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> mkdir -p $(CMAKE_DEBUG_BUILD_DIR)
> cd $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_DEBUG_FLAGS) ..
endif
> cd $(CMAKE_DEBUG_BUILD_DIR) && make $(MAKE_FLAGS)

release:
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR))","")
> mkdir -p $(CMAKE_RELEASE_BUILD_DIR)
> cd $(CMAKE_RELEASE_BUILD_DIR) && cmake  $(CMAKE_RELEASE_FLAGS)  ..
endif
> cd $(CMAKE_RELEASE_BUILD_DIR) && make  $(MAKE_FLAGS)

run_debug:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/$(RUNNER))","")
>  cd $(CMAKE_DEBUG_BUILD_DIR) && ./$(RUNNER) ;
else
>  @echo "		====No Debug build available ========"
endif

run_release:
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/$(RUNNER))","")
>  cd $(CMAKE_RELEASE_BUILD_DIR) && ./$(RUNNER) ;
else
>  @echo "		====No Release build available ========"
endif

run_arm:
>  @echo "		====No run defined for ARM build (needs qemu) ========"

arm:
ifeq ("$(wildcard $(CMAKE_ARM_BUILD_DIR))","")
> mkdir -p $(CMAKE_ARM_BUILD_DIR)
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
> rm -rf $(CMAKE_RELEASE_BUILD_DIR) $(CMAKE_DEBUG_BUILD_DIR) $(CMAKE_ARM_BUILD_DIR)
