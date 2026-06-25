# ====================== winflexbison FetchContent ======================
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  include(FetchContent)
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
    -Wall --no-lines $<$<CONFIG:Debug>:--debug>
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