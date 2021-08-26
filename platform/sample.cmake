#-----------------------------------------------------------
# this is required
#-----------------------------------------------------------
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CXX_STANDARD 11)

#-----------------------------------------------------------
# specify the cross compiler
# Change the directory as your environment
#-----------------------------------------------------------
SET(PLATFORM SAMPLE)

SET(TOOLCHAIN_DIR [toolchain_dir])      #your toolchain directory
SET(CMAKE_C_COMPILER  ${TOOLCHAIN_DIR}/bin/xxx-gcc)     #your gcc file path
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/xxx-g++)    #your g++ file path

#-----------------------------------------------------------
# where is the target environment (Only in toolchain directory)
#-----------------------------------------------------------
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

