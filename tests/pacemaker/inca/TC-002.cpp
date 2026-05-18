#include <type_traits>

#include <gtest/gtest.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

PACEMAKER_FIXTURE_INIT(TC002)

namespace
{
  std::size_t counter{};
  struct MockCOM
  {
    void Release() { counter++; }
  };

  class ConcreteProxy : private pacemaker::inca::detail::COMProxy<MockCOM>
  {
  public:
    ConcreteProxy(pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr)
        : COMProxy{std::move(ptr)} {}
    ConcreteProxy(ConcreteProxy &&)            = default;
    ConcreteProxy &operator=(ConcreteProxy &&) = default;
    ~ConcreteProxy() {}
  };

} // namespace

TEST_F(TC002, A)
{
  EXPECT_FALSE(std::is_copy_constructible_v<ConcreteProxy>);
  EXPECT_TRUE(std::is_move_constructible_v<ConcreteProxy>);
  EXPECT_FALSE(std::is_copy_assignable_v<ConcreteProxy>);
  EXPECT_TRUE(std::is_move_assignable_v<ConcreteProxy>);
}

TEST_F(TC002, B)
{
  counter = 0;
  {
    MockCOM                                          mock;
    pacemaker::inca::detail::unique_com_ptr<MockCOM> mock_unique_ptr{&mock};
    EXPECT_EQ(counter, 0);
    auto concrete_proxy = ConcreteProxy{std::move(mock_unique_ptr)};
    EXPECT_EQ(counter, 0);
  }
  EXPECT_EQ(counter, 1);
}