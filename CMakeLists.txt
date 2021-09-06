cmake_minimum_required(VERSION 3.5)

PROJECT(Utils)

#-----------------------------------------------------------
# choose the compile toolchain
#-----------------------------------------------------------
if(NOT PLATFORM)
    SET(PLATFORM "PC")
endif()
MESSAGE(STATUS "Platform: ${PLATFORM}")
MESSAGE(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")

# ----------------------------------------------------------------------------
# set the output file directory
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PLATFORM}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PLATFORM}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PLATFORM}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ----------------------------------------------------------------------------
#
# ----------------------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(example)