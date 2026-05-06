cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(
  pacemaker 
  DESCRIPTION "COM automation for people with deadlines and a hatred of GUIs"
  LANGUAGES CXX
  VERSION 0.0.1
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src)

if(${PACEMAKER_BUILD_REQUIREMENTS})
  add_subdirectory(requirements)
endif()

if(${PACEMAKER_BUILD_DOC})
  add_subdirectory(doc)
endif()