cmake_minimum_required (VERSION 3.21)

project(prescan_types VERSION 2023.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED 11)

add_subdirectory(bridge_base)
add_subdirectory(bridge_msgs)

# find protobuf
find_package(Protobuf REQUIRED)
if (PROTOBUF_FOUND)
    message("protobuf found")
else ()
    message(FATAL_ERROR "Cannot find Protobuf")
endif ()

include_directories(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE proto_headers ./bridge_msgs/*.pb.h)
file(GLOB_RECURSE proto_sources ./bridge_msgs/*.pb.cc)

set(PROTO_SRCS ${proto_sources})
set(PROTO_HDRS ${proto_headers})

add_library(prescan_protos STATIC ${PROTO_SRCS})
target_link_libraries(prescan_protos ${Protobuf_LIBRARIES})
message(${Protobuf_LIBRARIES})

target_include_directories(prescan_protos
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${PROTOBUF_INCLUDE_DIRS})

target_include_directories(prescan_protos PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/bridge_base/
        ${CMAKE_SOURCE_DIR}/bridge_msgs/
)
