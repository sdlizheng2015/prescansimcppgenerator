cmake_minimum_required (VERSION 3.21)

project(prescan_proto_msgs VERSION 2023.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)
FILE(GLOB protobuf_files 
  "${CMAKE_CURRENT_SOURCE_DIR}/bridge_msgs/basic_msgs/*.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bridge_msgs/chassis_msgs/*.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bridge_msgs/control_msgs/*.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bridge_msgs/localiztion_msgs/*.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bridge_msgs/perception_msgs/*.proto"
  "${CMAKE_CURRENT_SOURCE_DIR}/bridge_msgs/sensor_msgs/*.proto")

SET(PROTO_GENERATE_DIR ${CMAKE_CURRENT_BINARY_DIR})
LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(FILE ${protobuf_files})

  EXECUTE_PROCESS(
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_GENERATE_DIR} ${FILE})
    
ENDFOREACH()