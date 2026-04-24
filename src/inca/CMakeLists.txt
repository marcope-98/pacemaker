cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(inca VERSION 0.0.1 LANGUAGES C CXX)

add_library(
  ${PROJECT_NAME}
  STATIC 
  src/IncaCOMAdapter.cpp
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall -O2>
  $<$<CXX_COMPILER_ID:MSVC>: /W2 /O2>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})