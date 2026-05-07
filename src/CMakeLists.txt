
add_library(
  pacemaker_inca
  STATIC
  src/pacemaker/inca/com/IncaProxy.cpp
  src/pacemaker/inca/com/IncaOnlineExperimentProxy.cpp
  src/pacemaker/inca/com/IncaExperimentViewProxy.cpp
  src/pacemaker/inca/com/ExperimentDeviceProxy.cpp
  src/pacemaker/inca/com/CalibrationScalarDataProxy.cpp
  src/pacemaker/inca/Session.cpp
  src/pacemaker/inca/Experiment.cpp
)

target_precompile_headers(
  pacemaker_inca
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
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