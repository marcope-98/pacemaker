set(PACEMAKER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src/pacemaker/inca)
add_subdirectory(src/pacemaker/io)
add_subdirectory(src/pacemaker/timer)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/pacemaker-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pacemaker-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pacemaker-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pacemaker
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/pacemaker-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/pacemaker-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pacemaker"
)