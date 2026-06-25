add_executable(pacemaker.io.tests)
target_sources(
  pacemaker.io.tests
  PRIVATE
    TC-012.cpp
)

target_link_libraries(
  pacemaker.io.tests
  PRIVATE 
    pacemaker::io
    pacemaker::fixture
    GTest::gtest GTest::gtest_main
    GTest::gmock GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(pacemaker.io.tests)

add_dependencies(tests pacemaker.io.tests)