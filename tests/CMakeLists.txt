include(FetchContent)
set(FETCHCONTENT_QUITE FALSE CACHE INTERNAL "")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY          https://github.com/google/googletest.git
  GIT_TAG                 v1.17.x
  GIT_PROGRESS            ON
  GIT_SHALLOW             1
  FIND_PACKAGE_ARGS NAMES GTest
)
set(INSTALL_GTEST OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(googletest)

set(PACEMAKER_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(tests ALL)
add_compile_options(/EHsc /W4 /D_DEBUG /MDd)
add_compile_definitions(_CRTDBG_MAP_ALLOC)

add_subdirectory(pacemaker/fixture)
add_subdirectory(pacemaker/inca)
add_subdirectory(pacemaker/timer)