# ====================== winflexbison FetchContent ======================
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  include(FetchContent)
  set(CMAKE_TLS_VERIFY 0)
  FetchContent_Declare(
    winflexbison
    URL https://github.com/lexxmark/winflexbison/releases/download/v2.5.25/win_flex_bison-2.5.25.zip
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
  )
  FetchContent_MakeAvailable(winflexbison)
  set(BISON_ROOT "${winflexbison_SOURCE_DIR}")
  set(FLEX_ROOT  "${winflexbison_SOURCE_DIR}")
endif()

# ====================== BISON =====================
find_package(BISON REQUIRED)
bison_target(
  pacemaker.io.bison-csv
  ${PACEMAKER_SOURCE_DIR}/src/pacemaker/io/csv.y 
  ${CMAKE_CURRENT_BINARY_DIR}/bison-csv.cpp
  DEFINES_FILE
    ${CMAKE_CURRENT_BINARY_DIR}/bison-csv.hpp
  OPTIONS
    -Wall --no-lines -Wcounterexamples $<$<CONFIG:Debug>:--debug>
)

# ====================== FLEX ======================
find_package(FLEX REQUIRED)
if(NOT TARGET FLEX::fl)
  add_library(FLEX::fl INTERFACE IMPORTED)
  set_target_properties(
    FLEX::fl
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${FLEX_INCLUDE_DIRS}"
  )
endif()

flex_target(
  pacemaker.io.flex-csv
  ${PACEMAKER_SOURCE_DIR}/src/pacemaker/io/csv.l
  ${CMAKE_CURRENT_BINARY_DIR}/flex-csv.cpp
  DEFINES_FILE
    ${CMAKE_CURRENT_BINARY_DIR}/flex-csv.hpp
)
add_flex_bison_dependency(pacemaker.io.flex-csv pacemaker.io.bison-csv)

# ====================== pacemaker.io ======================
add_library(pacemaker.io)
add_library(pacemaker::io ALIAS pacemaker.io)

include(GenerateExportHeader)
generate_export_header(pacemaker.io)

target_sources(
  pacemaker.io
  PUBLIC
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/include"
    FILES
      ${PACEMAKER_SOURCE_DIR}/include/pacemaker/io/CSV.hpp
  PUBLIC
    FILE_SET generated_headers
    TYPE     HEADERS
    BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/../.."
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pacemaker.io_export.h
  PRIVATE
    FILE_SET  private_headers
    TYPE      HEADERS
    BASE_DIRS "${PACEMAKER_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}"
    FILES
      ${PACEMAKER_SOURCE_DIR}/src/pacemaker/io/CSVLexer.hpp
      ${CMAKE_CURRENT_BINARY_DIR}/bison-csv.hpp
      ${CMAKE_CURRENT_BINARY_DIR}/flex-csv.hpp
  PRIVATE
    ${PACEMAKER_SOURCE_DIR}/src/pacemaker/io/CSV.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/bison-csv.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/flex-csv.cpp
)

target_link_libraries(pacemaker.io PRIVATE FLEX::fl)
target_compile_definitions(pacemaker.io PRIVATE YY_NO_UNISTD_H)
target_compile_features(pacemaker.io PRIVATE cxx_std_17)

set_target_properties(
  pacemaker.io 
  PROPERTIES 
    VERIFY_INTERFACE_HEADER_SETS ON
    VERIFY_PRIVATE_HEADER_SETS   ON
    EXPORT_NAME                  io
)

include(GNUInstallDirs)
install(
  TARGETS pacemaker.io
  EXPORT  pacemaker.io
  DESTINATION ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
  FILE_SET HEADERS           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET generated_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT      pacemaker.io
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/pacemaker"
  NAMESPACE   pacemaker:: 
  FILE        pacemaker.io-targets.cmake
)