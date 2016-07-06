.RECIPEPREFIX = >

CMAKE_RELEASE_BUILD_DIR = cmake_release
CMAKE_DEBUG_BUILD_DIR = cmake_debug


CMAKE_DEBUG_BUILD_MAKEFILE =$(CMAKE_DEBUG_BUILD_DIR)/Makefile

#CMAKE_FLAGS=  -DCMAKE_CXX_COMPILER=g++-4.9 -DCMAKE_C_COMPILER=gcc-4.9
CMAKE_FLAGS=  -DCMAKE_CXX_COMPILER=g++-4.9
CMAKE_DEBUG_FLAGS=  -DCMAKE_BUILD_TYPE=Debug $(CMAKE_FLAGS)
CMAKE_RELEASE_FLAGS=  -DCMAKE_BUILD_TYPE=Debug $(CMAKE_FLAGS)


all: debug

debug:
ifeq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR))","")
> mkdir -p $(CMAKE_DEBUG_BUILD_DIR)
> cd $(CMAKE_DEBUG_BUILD_DIR) && cmake $(CMAKE_DEBUG_FLAGS) ..
endif
> cd $(CMAKE_DEBUG_BUILD_DIR) && make 

release:
ifeq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR))","")
> mkdir -p $(CMAKE_RELEASE_BUILD_DIR)
> cd $(CMAKE_RELEASE_BUILD_DIR) && cmake  $(CMAKE_RELEASE_FLAGS)  ..
endif
> cd $(CMAKE_RELEASE_BUILD_DIR) && make 

clean:
ifneq ("$(wildcard $(CMAKE_DEBUG_BUILD_DIR)/Makefile)","")
> cd $(CMAKE_DEBUG_BUILD_DIR) &&  make clean;
endif
ifneq ("$(wildcard $(CMAKE_RELEASE_BUILD_DIR)/Makefile)","")
> cd $(CMAKE_RELEASE_BUILD_DIR) &&  make clean;
endif

distclean:
> rm -rf $(CMAKE_RELEASE_BUILD_DIR) $(CMAKE_DEBUG_BUILD_DIR)
