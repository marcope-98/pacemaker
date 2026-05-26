find_package(INCA REQUIRED)

add_library(pacemaker.inca)
add_library(pacemaker::inca ALIAS pacemaker.inca)

target_sources(
  pacemaker.inca
  PUBLIC
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/include"
    FILES
      ${PACEMAKER_SOURCE_DIR}/include/pacemaker/inca/Session.hpp
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
    EXPORT_NAME                  inca
)

include(GNUInstallDirs)
install(
  TARGETS pacemaker.inca
  EXPORT  pacemaker.inca
  DESTINATION ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT      pacemaker.inca
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pacemaker"
  NAMESPACE   pacemaker:: 
  FILE        pacemaker.inca-targets.cmake
)