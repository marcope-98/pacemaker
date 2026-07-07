find_package(INCA REQUIRED)

add_library(pacemaker.inca.internal INTERFACE)
add_library(pacemaker::inca::internal ALIAS pacemaker.inca.internal)

target_sources(
  pacemaker.inca.internal
  INTERFACE
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/src"
    FILES
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/CalibrationScalarDataProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/ExperimentDeviceProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/IncaExperimentViewProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/IncaOnlineExperimentProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/IncaProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/utils.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/COMProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/DispatchWrapper.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/unique_com_ptr.hpp
)

target_precompile_headers(
  pacemaker.inca.internal
  INTERFACE
  ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
)

target_compile_definitions(pacemaker.inca.internal INTERFACE ETAS_INCACOM_TLB="${ETAS_INCACOM_TLB}")
target_compile_features(pacemaker.inca.internal INTERFACE cxx_std_17)

set_target_properties(
  pacemaker.inca.internal 
  PROPERTIES 
    VERIFY_INTERFACE_HEADER_SETS ON
    VERIFY_PRIVATE_HEADER_SETS   ON
)

# ============================================

add_library(pacemaker.inca)
add_library(pacemaker::inca ALIAS pacemaker.inca)

include(GenerateExportHeader)
generate_export_header(pacemaker.inca)

target_sources(
  pacemaker.inca
  PUBLIC
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/include"
    FILES
      ${PACEMAKER_SOURCE_DIR}/include/pacemaker/inca/Session.hpp
  PUBLIC
    FILE_SET  generated_headers
    TYPE      HEADERS
    BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/../.."
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/pacemaker.inca_export.h
  PRIVATE
    FILE_SET  private_headers
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/src"
    FILES 
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/CalibrationScalarDataProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/ExperimentDeviceProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/IncaExperimentViewProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/IncaOnlineExperimentProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/com/IncaProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/utils.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/COMProxy.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/DispatchWrapper.hpp
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/unique_com_ptr.hpp
  PRIVATE
    ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/Session.cpp
)

target_precompile_headers(
  pacemaker.inca
  PRIVATE
  ${PACEMAKER_SOURCE_DIR}/src/pacemaker/inca/detail/incacom.hpp
)

target_compile_definitions(pacemaker.inca PRIVATE ETAS_INCACOM_TLB="${ETAS_INCACOM_TLB}")
target_compile_features(pacemaker.inca PUBLIC cxx_std_17)

set_target_properties(
  pacemaker.inca 
  PROPERTIES 
    VERIFY_INTERFACE_HEADER_SETS ON
    VERIFY_PRIVATE_HEADER_SETS   ON
    EXPORT_NAME                  inca
)

include(GNUInstallDirs)
install(
  TARGETS pacemaker.inca
  EXPORT  pacemaker.inca
  DESTINATION ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
  FILE_SET HEADERS           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET generated_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT      pacemaker.inca
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pacemaker"
  NAMESPACE   pacemaker:: 
  FILE        pacemaker.inca-targets.cmake
)