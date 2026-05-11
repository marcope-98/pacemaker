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

add_subdirectory(pacemaker/inca)