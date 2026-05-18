#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/Session.hpp"

PACEMAKER_FIXTURE_INIT(TC009)

namespace
{
}

TEST_F(TC009, A) { /* Left empty on purpose because it is an integration test*/ }
TEST_F(TC009, B) { /* Left empty on purpose because it is an integration test*/ }
TEST_F(TC009, C) { /* Left empty on purpose because it is an integration test*/ }

TEST_F(TC009, D)
{
  EXPECT_FALSE(std::is_copy_constructible_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_copy_assignable_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_move_constructible_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_move_assignable_v<pacemaker::inca::Session>);
}