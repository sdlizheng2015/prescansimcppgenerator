cmake_minimum_required (VERSION 3.21)

project(prescan_types VERSION 2023.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED 11)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

