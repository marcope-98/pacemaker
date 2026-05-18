add_executable(pacemaker.inca.tests)
target_sources(
  pacemaker.inca.tests
  PRIVATE
    TC-001.cpp
    TC-002.cpp
    TC-003.cpp
    TC-004.cpp
    TC-005.cpp
    TC-006.cpp
    TC-007.cpp
    TC-008.cpp
    TC-009.cpp
)

target_link_libraries(
  pacemaker.inca.tests
  PRIVATE 
    pacemaker::inca
    pacemaker::fixture
    GTest::gtest GTest::gtest_main
    GTest::gmock GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(pacemaker.inca.tests)

add_dependencies(tests pacemaker.inca.tests)