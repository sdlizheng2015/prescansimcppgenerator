
get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

if(NOT Visualizationtools_FIND_QUIETLY)
  message(STATUS "Found Visualizationtools in ${PACKAGE_PREFIX_DIR}")
endif()

set(VIS_INCLUDE_DIRS  "${PACKAGE_PREFIX_DIR}/install/include")
include_directories(${VIS_INCLUDE_DIRS})

if (MSVC)
  file(GLOB VIS_LIBRARIES "${PACKAGE_PREFIX_DIR}/install/lib/*.lib")
endif (MSVC)

if (UNIX)
  file(GLOB VIS_LIBRARIES "${PACKAGE_PREFIX_DIR}/install/lib/*.a")
endif (UNIX)


