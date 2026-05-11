add_executable(pacemaker.inca.tests)
target_sources(
  pacemaker.inca.tests
  PRIVATE
    TC-001.cpp
    TC-002.cpp
)

target_link_libraries(
  pacemaker.inca.tests
  PRIVATE 
    pacemaker::inca 
    GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(pacemaker.inca.tests)