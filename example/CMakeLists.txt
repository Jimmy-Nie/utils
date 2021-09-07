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

SET(BIN_NAME timer)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl Log Timer)

SET(BIN_NAME thread)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC -lpthread -ldl Log)

SET(BIN_NAME directory)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log Text Dir)

SET(BIN_NAME system)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log System)

SET(BIN_NAME pubsub)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log -lpthread)

SET(BIN_NAME optional)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log -lpthread)

SET(BIN_NAME log)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log -lpthread)

SET(BIN_NAME memory_pool)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log)

SET(BIN_NAME text)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log Text)

SET(BIN_NAME test)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log Text)

SET(BIN_NAME iodata)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log IOdata)

SET(BIN_NAME enum)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log)

SET(BIN_NAME json)
ADD_EXECUTABLE(${BIN_NAME} ${BIN_NAME}/test.cpp)
TARGET_LINK_LIBRARIES(${BIN_NAME}  PUBLIC Log)