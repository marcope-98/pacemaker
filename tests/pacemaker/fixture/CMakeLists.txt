add_library(pacemaker.fixture INTERFACE)
add_library(pacemaker::fixture ALIAS pacemaker.fixture)
  
target_sources(
  pacemaker.fixture
  INTERFACE
    FILE_SET  HEADERS
    TYPE      HEADERS
    BASE_DIRS ${PACEMAKER_TEST_DIR}
    FILES
      ${PACEMAKER_TEST_DIR}/pacemaker/fixture/LeakTestFixture.hpp
)

target_compile_features(pacemaker.fixture INTERFACE cxx_std_17)

target_link_libraries(
  pacemaker.fixture 
  INTERFACE
  GTest::gtest
)

