cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(inca VERSION 0.0.1 LANGUAGES C CXX)

add_library(
  ${PROJECT_NAME}
  STATIC 
  src/com/IncaProxy.cpp
  src/com/IncaOnlineExperimentProxy.cpp
  src/com/IncaExperimentViewProxy.cpp
  src/com/ExperimentDeviceProxy.cpp
  src/com/CalibrationScalarDataProxy.cpp
  src/Session.cpp
  src/Experiment.cpp
  src/CalibrationSet.cpp
)

target_precompile_headers(
  ${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/incacom.hpp
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
  $<$<CXX_COMPILER_ID:MSVC>: /W2 /O2 /DINCACOM_TLB="${INCACOM_TLB}">
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})