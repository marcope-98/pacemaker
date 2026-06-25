cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(
  pacemaker 
  DESCRIPTION "COM automation for people with deadlines and a hatred of GUIs"
  LANGUAGES CXX
  VERSION 2.1.0
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CMakeOptions.cmake)

add_subdirectory(src)

if(PACEMAKER_BUILD_REQUIREMENTS)
  add_subdirectory(requirements)
endif()

if(PACEMAKER_BUILD_DOCS)
  add_subdirectory(docs)
endif()

if(PACEMAKER_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PACEMAKER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()