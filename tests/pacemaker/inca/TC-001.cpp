#include <type_traits>

#include <gtest/gtest.h>

#include "pacemaker/inca/detail/unique_com_ptr.hpp"

namespace
{
  std::size_t counter{};
  struct MockCOM
  {
    void Release() { counter++; }
  };
} // namespace

TEST(TC001, A)
{
  counter = 0;
  {
    MockCOM mock;
    pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr{&mock};
    EXPECT_EQ(counter, 0);
  }
  EXPECT_EQ(counter, 1);
}

TEST(TC001, B)
{
  counter = 0;
  {
    pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr{};
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(counter, 0);
  }
  EXPECT_EQ(counter, 0);
}

TEST(TC001, C)
{
  counter = 0;
  {
    MockCOM mock;
    pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr_moved_from{&mock};
    EXPECT_NE(ptr_moved_from.get(), nullptr);
    {
      pacemaker::inca::detail::unique_com_ptr<MockCOM> ptr_moved_to{std::move(ptr_moved_from)};
      EXPECT_EQ(ptr_moved_from.get(), nullptr);
      EXPECT_NE(ptr_moved_to.get(), nullptr);
      EXPECT_EQ(counter, 0);
    }
    EXPECT_EQ(counter, 1);
  }
  EXPECT_EQ(counter, 1);
}

TEST(TC001, D)
{
  EXPECT_FALSE(std::is_copy_constructible_v<pacemaker::inca::detail::unique_com_ptr<MockCOM>>);
  EXPECT_FALSE(std::is_copy_assignable_v<pacemaker::inca::detail::unique_com_ptr<MockCOM>>);
}