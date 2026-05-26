#include <memory>
#include <type_traits>


#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/detail/unique_com_ptr.hpp"

PACEMAKER_FIXTURE_INIT(TC001)

namespace
{
  struct MockCOM
  {
    MOCK_METHOD(void, Release, ());
  };

  template<typename T>
  struct is_unique_ptr : std::false_type
  {
  };

  template<typename T, typename Deleter>
  struct is_unique_ptr<std::unique_ptr<T, Deleter>> : std::true_type
  {
  };

  template<typename T>
  constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

} // namespace

TEST_F(TC001, A)
{
  {
    MockCOM mock;
    EXPECT_CALL(mock, Release()).Times(1);
    pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr{&mock};
  }
}

TEST_F(TC001, B)
{
  pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr{};
  EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(TC001, C)
{
  {
    MockCOM mock;
    EXPECT_CALL(mock, Release()).Times(1);

    pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr_moved_from{&mock};
    EXPECT_NE(ptr_moved_from.get(), nullptr);
    {
      pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr_moved_to{std::move(ptr_moved_from)};
      EXPECT_EQ(ptr_moved_from.get(), nullptr);
      EXPECT_NE(ptr_moved_to.get(), nullptr);
    }
  }
}

TEST_F(TC001, D)
{
  EXPECT_FALSE(std::is_copy_constructible_v<pacemaker::inca::detail::unique_com_ptr<MockCOM>>);
  EXPECT_FALSE(std::is_copy_assignable_v<pacemaker::inca::detail::unique_com_ptr<MockCOM>>);
}

TEST_F(TC001, E)
{
  MockCOM mock;
  EXPECT_CALL(mock, Release()).Times(1);
  pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr{&mock};
  EXPECT_EQ(ptr.get(), &mock);
}

TEST_F(TC001, F)
{
  EXPECT_TRUE(is_unique_ptr_v<pacemaker::inca::detail::unique_com_ptr<MockCOM>>);
}