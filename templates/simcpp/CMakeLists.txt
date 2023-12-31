cmake_minimum_required (VERSION 3.21)

project(SimCppProject VERSION 2023.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


################### add_subdirectory() here ###############
#ADDSUBDIR#
# e.g., add_subdirectory(bridge)
#
###########################################################

################### find_package() here ###################
#FINDPKG#
# if you are using vcpkg, you may not need find_package
# e.g.,
# find_package(boost)
# find_package(protobuf)
# find_package(OpenCV REQUIRED)
#find_package(PCL REQUIRED)
#include_directories(${PCL_INCLUDE_DIRS})
#link_directories(${PCL_LIBRARY_DIRS})
#add_definitions(${PCL_DEFINITIONS})
###########################################################
#ADDPKGS#

# add Eigen3
find_package(Eigen3 REQUIRED NO_MODULE)
add_definitions(${EIGEN3_DEFINITIONS})
include_directories(${EIGEN3_INCLUDE_DIRS})

# Find Prescan package
find_package(Prescan 2023.1 REQUIRED)
include(${Prescan_DIR}/PrescanUtils.cmake)

#file(GLOB_RECURSE allheaders CONFIGURE_DEPENDS "simmodel/*.h*")
file(GLOB_RECURSE allsources CONFIGURE_DEPENDS "simmodel/*.cpp")

add_executable(${PROJECT_NAME} ${allsources})

target_link_libraries(
${PROJECT_NAME} 
${Prescan_LIBS}
################ add additional libs ###################
#e.g., 
#${Protobuf_LIBRARIES}
#proj
#${OpenCV_LIBRARIES}
#${Boost_LIBRARIES}
#${PCL_LIBRARIES}
########################################################
#ADDLIBS#
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/simmodel/
  ${CMAKE_SOURCE_DIR}/simmodel/sensorsdemux/
  ${CMAKE_SOURCE_DIR}/simmodel/simmodel/
  ${CMAKE_SOURCE_DIR}/simmodel/utils/
  ${CMAKE_SOURCE_DIR}/simmodel/worldobjects/
 )

prescan_generate_helper_scripts(TARGET ${PROJECT_NAME} EXPERIMENT_DIR "${CMAKE_SOURCE_DIR}/..")

# Set some debugging properties when using Visual Studio
if(MSVC)
  get_filename_component(Prescan_ROOT_DIR ${Prescan_DIR}\\..\\.. ABSOLUTE)
  message(STATUS "PrescanRootDIR ${Prescan_ROOT_DIR}")
  set_target_properties(${PROJECT_NAME} PROPERTIES
    # Add paths to interface dll's, PRESCAN_BUILD points to path where bin\prescan.ini is stored
    VS_DEBUGGER_ENVIRONMENT "PATH=${Prescan_ROOT_DIR}\\bin;${Plugin_ROOT_DIR}\\bin"
  )

  # Set ${PROJECT_NAME} as startup project in Visual Studio
  set_property(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
  )
endif()
