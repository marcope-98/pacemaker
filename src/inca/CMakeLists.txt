
add_library(
  pacemaker_inca
  STATIC 
  src/com/IncaProxy.cpp
  src/com/IncaOnlineExperimentProxy.cpp
  src/com/IncaExperimentViewProxy.cpp
  src/com/ExperimentDeviceProxy.cpp
  src/com/CalibrationScalarDataProxy.cpp
  src/Session.cpp
  src/Experiment.cpp
)

target_precompile_headers(
  pacemaker_inca
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/incacom.hpp
)

target_include_directories(
  pacemaker_inca
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(
  pacemaker_inca
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall -O2>
  $<$<CXX_COMPILER_ID:MSVC>: /W2 /O2 /DINCACOM_TLB="${INCACOM_TLB}">
)

target_compile_features(pacemaker_inca PUBLIC cxx_std_17)

add_library(pacemaker::inca ALIAS pacemaker_inca)