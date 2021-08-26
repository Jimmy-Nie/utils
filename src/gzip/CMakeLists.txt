set(target "gzip")

file(GLOB_RECURSE c_srcs "./*.c")
file(GLOB_RECURSE all_hdrs "./*.h")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("${target} Debug -g -O0 -rdynamic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -pthread -ldl")# -g show more info for debug
else ()
    message("${target} Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -pthread -ldl")#-Wall show all the warning
endif ()

add_library(${target} ${c_srcs} ${all_hdrs})