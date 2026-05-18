#ifndef PACEMAKER_FIXTURE_LEAKTESTFIXTURE_HPP_
#define PACEMAKER_FIXTURE_LEAKTESTFIXTURE_HPP_

#include <gtest/gtest.h>

#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#include <crtdbg.h>
#include <stdlib.h>

#define PACEMAKER_FIXTURE_INIT(name) class name : public pacemaker::fixture::LeakTestFixture {}; 

namespace pacemaker::fixture
{
  class LeakTestFixture : public ::testing::Test
  {
  protected:
    LeakTestFixture()  = default;
    ~LeakTestFixture() = default;

    void SetUp() override
    {
      _CrtMemCheckpoint(&memoryState);
    }

    void TearDown() override
    {
      _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
      _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
      _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
      _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
      _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
      _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
      _CrtMemDumpAllObjectsSince(&this->memoryState);
    }
    _CrtMemState memoryState = {0};
  };
} // namespace pacemaker::fixture

#endif // PACEMAKER_FIXTURE_LEAKTESTFIXTURE_HPP_