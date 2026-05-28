#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <type_traits>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/detail/DispatchWrapper.hpp"

PACEMAKER_FIXTURE_INIT(TC010)

namespace
{
  struct XDispatch
  {
    MOCK_METHOD(void, func, ());
  };

  struct MockDispatchWrapper : public pacemaker::inca::detail::DispatchWrapper<XDispatch>
  {
  };
} // namespace

TEST_F(TC010, A)
{
  pacemaker::inca::detail::DispatchWrapper<XDispatch> dispatch_wrapper{};
  EXPECT_CALL(dispatch_wrapper, func()).Times(1);
  dispatch_wrapper.func();
}

TEST_F(TC010, B)
{
  bool condition = std::is_same_v<pacemaker::inca::detail::DispatchWrapper<XDispatch>::type, XDispatch>;
  EXPECT_TRUE(condition);
}

TEST_F(TC010, C)
{
  bool condition = std::is_same_v<MockDispatchWrapper::type, XDispatch>;
  EXPECT_TRUE(condition);
}
