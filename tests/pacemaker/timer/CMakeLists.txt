add_executable(pacemaker.timer.tests)
target_sources(
  pacemaker.timer.tests
  PRIVATE
    TC-011.cpp
)

target_link_libraries(
  pacemaker.timer.tests
  PRIVATE 
    pacemaker::timer
    pacemaker::fixture
    GTest::gtest GTest::gtest_main
    GTest::gmock GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(pacemaker.timer.tests)

add_dependencies(tests pacemaker.timer.tests)