add_library(pacemaker.timer)
add_library(pacemaker::timer ALIAS pacemaker.timer)

include(GenerateExportHeader)
generate_export_header(pacemaker.timer)

target_sources(
  pacemaker.timer
  PUBLIC
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/include"
    FILES
      ${PACEMAKER_SOURCE_DIR}/include/pacemaker/timer/Timer.hpp
  PUBLIC
    FILE_SET generated_headers
    TYPE     HEADERS
    BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/../.."
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/pacemaker.timer_export.h
  PRIVATE
    FILE_SET  private_headers
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/src"
    FILES 
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/timer/AvSetMmThreadOptions.hpp
  PRIVATE
    ${PACEMAKER_SOURCE_DIR}/src/pacemaker/timer/Timer.cpp
)

target_compile_features(pacemaker.timer PUBLIC cxx_std_17)
target_link_libraries(pacemaker.timer PRIVATE avrt)

set_target_properties(
  pacemaker.timer
  PROPERTIES 
    VERIFY_INTERFACE_HEADER_SETS ON
    VERIFY_PRIVATE_HEADER_SETS   ON
    EXPORT_NAME                  timer
)

include(GNUInstallDirs)
install(
  TARGETS pacemaker.timer
  EXPORT  pacemaker.timer
  DESTINATION ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
  FILE_SET HEADERS           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET generated_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT      pacemaker.timer
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pacemaker"
  NAMESPACE   pacemaker:: 
  FILE        pacemaker.timer-targets.cmake
)