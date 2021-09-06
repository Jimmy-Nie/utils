#-----------------------------------------------------------
# Get the source code and header
#-----------------------------------------------------------
PROJECT(utils)

#-----------------------------------------------------------
# Set the toolchain flag base the toolchain type
#-----------------------------------------------------------
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CMAKE_CXX_FLAGS "-g -O0")
else ()
    SET(CMAKE_CXX_FLAGS "-s -O3") #-Wall show all the warning
endif ()

#-----------------------------------------------------------
# Get the source file
#-----------------------------------------------------------
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(SRC_DIR ${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(
        ${SRC_DIR}
        ${SRC_DIR}/spdlog/inc
)

FILE(GLOB_RECURSE OPTIONAL_SRC "${SRC_DIR}/optional/*.h*")
FILE(GLOB_RECURSE PUBSUB_SRC "${SRC_DIR}/pubsub/*.h*")
FILE(GLOB_RECURSE THREAD_SRC "${SRC_DIR}/thread/*.h*")

FILE(GLOB_RECURSE SPDLOG_SOURCE "${SRC_DIR}/spdlog/inc/*.h*"    "${SRC_DIR}/spdlog/src/*.c*")

FILE(GLOB_RECURSE DIR_SRC "${SRC_DIR}/directory/*.h*"           "${SRC_DIR}/directory/*.c*")
if (WIN32)
    FILE(GLOB_RECURSE RM_SRC "${SRC_DIR}/*_unix.cpp")
    LIST(REMOVE_ITEM DIR_SRC ${RM_SRC})
else ()
    FILE(GLOB_RECURSE RM_SRC "${SRC_DIR}/*_win32.cpp")
    list(REMOVE_ITEM DIR_SRC ${RM_SRC})
endif ()

FILE(GLOB_RECURSE EXC_SRC   "${SRC_DIR}/exception/*.h*"         "${SRC_DIR}/exception/*.c*")
FILE(GLOB_RECURSE IO_SRC    "${SRC_DIR}/iodata/*.h*"            "${SRC_DIR}/iodata/*.c*")
FILE(GLOB_RECURSE SYS_SRC   "${SRC_DIR}/system/*.h*"            "${SRC_DIR}/system/*.c*")
FILE(GLOB_RECURSE TEXT_SRC  "${SRC_DIR}/text/*.h*"              "${SRC_DIR}/text/*.c*")
FILE(GLOB_RECURSE TIMER_SRC "${SRC_DIR}/timer/*.h*"             "${SRC_DIR}/timer/*.c*")

#-----------------------------------------------------------
# Generate executable file
#-----------------------------------------------------------
ADD_DEFINITIONS(-DSPDLOG_COMPILED_LIB)  #for compile spdlog
ADD_LIBRARY(Log         ${SPDLOG_SOURCE})
ADD_LIBRARY(Exception   ${EXC_SRC})
ADD_LIBRARY(Dir         ${DIR_SRC})
ADD_LIBRARY(IOdata      ${IO_SRC})
ADD_LIBRARY(System      ${SYS_SRC})
ADD_LIBRARY(Text        ${TEXT_SRC})
ADD_LIBRARY(Timer       ${TIMER_SRC})

TARGET_LINK_LIBRARIES(Exception -ldl)
TARGET_LINK_LIBRARIES(Timer Exception)
TARGET_LINK_LIBRARIES(Text Exception)
TARGET_LINK_LIBRARIES(Dir Exception Text -lpthread)
TARGET_LINK_LIBRARIES(System Exception -lpthread)
