## --- Library dependencies ---
#add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/mqtt/c ${CMAKE_SOURCE_DIR}/third_party/mqtt/)

set (THREADS_PREFER_PTHREAD_FLAG ON)

SET(paho-cpp-srcs
  async_client.cpp
  client.cpp
  connect_options.cpp
  create_options.cpp    
  disconnect_options.cpp
  iclient_persistence.cpp
  message.cpp
  properties.cpp
  response_options.cpp
  ssl_options.cpp
  string_collection.cpp
  subscribe_options.cpp
  token.cpp
  topic.cpp
  will_options.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../c)

## create the static library
add_library(paho-mqttpp3 STATIC ${paho-cpp-srcs})

## add dependencies to the shared library
set(LIBS_SYSTEM c stdc++)
target_link_libraries(paho-mqttpp3
    PRIVATE ${LIBS_SYSTEM}
    PUBLIC  paho-mqtt3a paho-mqtt3c)