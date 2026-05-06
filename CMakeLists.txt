cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(
  pacemaker 
  DESCRIPTION "COM automation for people with deadlines and a hatred of GUIs"
  LANGUAGES CXX
  VERSION 0.0.1
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)

add_subdirectory(src/inca)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall -O2>
  $<$<CXX_COMPILER_ID:MSVC>: /W2 /O2>
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} PUBLIC pacemaker::inca)

if(${PACEMAKER_BUILD_REQUIREMENTS})
  add_subdirectory(requirements)
endif()

if(${PACEMAKER_BUILD_DOC})
  add_subdirectory(doc)
endif()