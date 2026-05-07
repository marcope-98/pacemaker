
add_library(pacemaker_inca)
add_library(pacemaker::inca ALIAS pacemaker_inca)

target_sources(
  pacemaker_inca
  PUBLIC
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
    FILES
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/Experiment.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/Session.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/com/CalibrationScalarDataProxy.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/com/ExperimentDeviceProxy.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/com/IncaExperimentViewProxy.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/com/IncaOnlineExperimentProxy.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/com/IncaProxy.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/detail/COMProxy.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/include/pacemaker/inca/detail/unique_com_ptr.hpp
  PRIVATE
    FILE_SET  private_headers
    TYPE      HEADERS
    BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src"
    FILES 
      ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/detail/utils.hpp
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/Experiment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/Session.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/com/CalibrationScalarDataProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/com/ExperimentDeviceProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/com/IncaExperimentViewProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/com/IncaOnlineExperimentProxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/com/IncaProxy.cpp
)

target_precompile_headers(
  pacemaker_inca
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
)

target_compile_options(
  pacemaker_inca
  PRIVATE
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: -Wall -O2>
  $<$<CXX_COMPILER_ID:MSVC>: /W2 /O2 /DINCACOM_TLB="${INCACOM_TLB}">
)

target_compile_features(pacemaker_inca PUBLIC cxx_std_17)

set_target_properties(
  pacemaker_inca 
  PROPERTIES 
    VERIFY_INTERFACE_HEADER_SETS ON
)