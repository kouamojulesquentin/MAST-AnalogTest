# This is a model for defining user options specific to a, local sandbox.
# To specify non default value in a sandbox, copy this file into UserOptions.cmake
# and adjust the values as necessary in the copy (the copy MUST NOT be added to GIT repository).
# This file should be modified only to add new variables driving build process (or to remove deprecated ones)

message(STATUS "Master: Reading user options (from UserOptions.cmake)")

set(BUILD_ATE_SERVER   OFF)
set(USE_XML_RPC        OFF)
set(USE_OPEN_OCD       OFF)
set(USE_LIBFTDISPI     OFF)
set(INTEL_EXPERIMENT   OFF)
set(KISSFFT            OFF)
