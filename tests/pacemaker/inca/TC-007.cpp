#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/com/CalibrationScalarDataProxy.hpp"

PACEMAKER_FIXTURE_INIT(TC007)

namespace
{
  template<typename T>
  struct is_COMProxy : std::false_type
  {
  };

  template<typename T>
  struct is_COMProxy<pacemaker::inca::detail::COMProxy<T>> : std::true_type
  {
  };

  template<typename T>
  constexpr bool is_COMProxy_v = is_COMProxy<T>::value;
} // namespace

TEST_F(TC007, A)
{
  EXPECT_TRUE(is_COMProxy_v<pacemaker::inca::com::CalibrationScalarDataProxy>);
}