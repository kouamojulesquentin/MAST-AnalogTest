cmake_minimum_required(VERSION 3.5)
project(kissfft)

include_directories(.)
include_directories(./tools)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(LIBKISSFFT_INCLUDE_DIRS
    "${PARENT_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/tools"
    PARENT_SCOPE)

set(KISS_FFT_LIBRARIES "kiss_fft" PARENT_SCOPE)

set(KissFFT_Files kiss_fft.c
                  tools/kfc.c
                  tools/kiss_fftnd.c
                  tools/kiss_fftndr.c
                  tools/kiss_fftr.c)

add_library           (kiss_fft SHARED     ${KissFFT_Files})
set_target_properties (kiss_fft PROPERTIES COMPILE_FLAGS "-Wall" )
