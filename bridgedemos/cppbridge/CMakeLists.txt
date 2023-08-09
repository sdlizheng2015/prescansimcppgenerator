cmake_minimum_required (VERSION 3.21)

project(SimCppProject VERSION 2023.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(bridge)

add_executable(cppbridge main.cpp)

target_link_libraries(cppbridge 
                      prescan_protos
                      ${Boost_LIBRARIES}
)

target_include_directories(cppbridge PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/bridge
 )
