cmake_minimum_required(VERSION 3.21)
project(vislibrary VERSION 2023.1 LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/../install")


# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# add OpenCV
find_package(OpenCV REQUIRED)

# add Eigen3
find_package(Eigen3 REQUIRED NO_MODULE)
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

file(GLOB_RECURSE vis_headers ./inc/*.h)
file(GLOB_RECURSE vis_sources ./src/*.cpp)
set(VIS_SRCS ${vis_sources})
set(VIS_HDRS ${vis_headers})
include_directories(inc)

# 创建库
add_library(vis_static STATIC ${VIS_SRCS})
set_target_properties(vis_static PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME visualizationtools)
target_link_libraries(vis_static ${Protobuf_LIBRARIES} ${OpenCV_LIBRARIES})

add_library(vis_shared SHARED ${VIS_SRCS})
set_target_properties(vis_shared PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME visualizationtools)
target_link_libraries(vis_shared ${Protobuf_LIBRARIES} ${OpenCV_LIBRARIES})

# 指定库的安装路径
install(TARGETS vis_static ARCHIVE DESTINATION lib)
install(TARGETS vis_shared LIBRARY DESTINATION bin)
install(FILES ${vis_headers} DESTINATION include)


