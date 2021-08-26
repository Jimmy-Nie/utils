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
SET(SRC_DIR ${PROJECT_SOURCE_DIR}/../src)
INCLUDE_DIRECTORIES(
   ${SRC_DIR}
)

FILE(GLOB_RECURSE SHARE_SOURCE "${SRC_DIR}/signals/*.h*" "${SRC_DIR}/share/*.h*")

SET(BIN_NAME thread_pool)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.c*" "${SRC_DIR}/${BIN_NAME}/*.h*")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} thread_pool/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)

SET(BIN_NAME thread)
FILE(GLOB_RECURSE SRC_SOURCE "${SRC_DIR}/${BIN_NAME}/*.c*" "${SRC_DIR}/${BIN_NAME}/*.h*" "${SHARE_SOURCE}")
ADD_EXECUTABLE(${BIN_NAME} ${SRC_SOURCE} thread/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl)