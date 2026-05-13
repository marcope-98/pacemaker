#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/inca/Session.hpp"

namespace
{
}

TEST(TC009, A) { /* Left empty on purpose because it is an integration test*/ }
TEST(TC009, B) { /* Left empty on purpose because it is an integration test*/ }
TEST(TC009, C) { /* Left empty on purpose because it is an integration test*/ }

TEST(TC009, D)
{
  EXPECT_FALSE(std::is_copy_constructible_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_copy_assignable_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_move_constructible_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_move_assignable_v<pacemaker::inca::Session>);
}