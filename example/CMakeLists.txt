#-----------------------------------------------------------
# Get the source code and header
#-----------------------------------------------------------
PROJECT(Example)

#-----------------------------------------------------------
# Set the toolchain flag base the toolchain type
#-----------------------------------------------------------
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CMAKE_CXX_FLAGS "-g -O0")
else ()
    SET(CMAKE_CXX_FLAGS "-s -O3") #-Wall show all the warning
endif ()

#-----------------------------------------------------------
# Generate executable file
#-----------------------------------------------------------
ADD_DEFINITIONS(-DSPDLOG_COMPILED_LIB)  #for compile spdlog

SET(SRC_DIR ${PROJECT_SOURCE_DIR}/../src)
INCLUDE_DIRECTORIES(
        ${SRC_DIR}
        ${SRC_DIR}/spdlog/inc
)

FILE(GLOB_RECURSE SHARE_SOURCE "${SRC_DIR}/signals/*.h*" "${SRC_DIR}/share/*.h*" "${SRC_DIR}/share/*.c*")
FILE(GLOB_RECURSE SPDLOG_SOURCE "${SRC_DIR}/spdlog/inc/*.h*" "${SRC_DIR}/spdlog/src/*.c*")

SET(BIN_NAME thread_pool)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.c*" "${SRC_DIR}/${BIN_NAME}/*.h*")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)

SET(BIN_NAME thread)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.c*" "${SRC_DIR}/${BIN_NAME}/*.h*")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} ${SHARE_SOURCE} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)

SET(BIN_NAME spdlog)
ADD_EXECUTABLE(${BIN_NAME} ${SPDLOG_SOURCE} log/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)

SET(BIN_NAME pubsub)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.c*" "${SRC_DIR}/${BIN_NAME}/*.h*")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} ${SPDLOG_SOURCE} ${SHARE_SOURCE} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)

SET(BIN_NAME memory_pool)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.tcc" "${SRC_DIR}/${BIN_NAME}/*.h*")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)

SET(BIN_NAME timer)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.c*" "${SRC_DIR}/${BIN_NAME}/*.h*")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} ${SPDLOG_SOURCE} ${SHARE_SOURCE} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)