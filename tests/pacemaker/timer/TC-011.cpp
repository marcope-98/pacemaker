#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/timer/Timer.hpp"

PACEMAKER_FIXTURE_INIT(TC011)

using namespace std::chrono_literals;

namespace
{
  struct MockCOM
  {
    MOCK_METHOD(void, func, ());
  };
} // namespace

TEST_F(TC011, A)
{
  constexpr bool condition = std::is_constructible_v<pacemaker::timer::Timer,
                                                     std::chrono::milliseconds, std::size_t>;
  EXPECT_TRUE(condition);
}

TEST_F(TC011, B) { /* Left empty on purpose because it is an integration test*/ }

TEST_F(TC011, C)
{
  pacemaker::timer::Timer timer{10ms, 10};
  EXPECT_FALSE(timer.is_running());
}

TEST_F(TC011, D)
{
  EXPECT_FALSE(std::is_copy_constructible_v<pacemaker::timer::Timer>);
  EXPECT_FALSE(std::is_copy_assignable_v<pacemaker::timer::Timer>);
  EXPECT_FALSE(std::is_move_constructible_v<pacemaker::timer::Timer>);
  EXPECT_FALSE(std::is_move_assignable_v<pacemaker::timer::Timer>);
}

TEST_F(TC011, E)
{
  EXPECT_THROW(pacemaker::timer::Timer(-10ms, 0), std::invalid_argument);
}

TEST_F(TC011, F)
{
  EXPECT_THROW(pacemaker::timer::Timer(0ms, 0), std::invalid_argument);
}

TEST_F(TC011, G)
{
  bool                    running{false};
  pacemaker::timer::Timer timer{10ms, 0};
  timer.start([&timer, &running]()
              { running = timer.is_running(); });
  EXPECT_TRUE(running);
}

TEST_F(TC011, H)
{
  MockCOM mock;
  EXPECT_CALL(mock, func()).Times(1);
  pacemaker::timer::Timer timer{10ms, 0};
  timer.start([&mock]()
              { mock.func(); });
}

TEST_F(TC011, I)
{
  pacemaker::timer::Timer timer{10ms, 0};
  timer.start();
  EXPECT_DEBUG_DEATH(timer.start(), "Timer is already running");
}

TEST_F(TC011, J)
{
  pacemaker::timer::Timer timer{10ms, 0};
  EXPECT_NO_THROW(timer.start());
}

TEST_F(TC011, K)
{
  pacemaker::timer::Timer timer{10ms, 0};
  EXPECT_DEBUG_DEATH(timer.wait([](std::size_t) {}),
                     "Timer was not started correctly. Did you forget a Timer::start?");
}

TEST_F(TC011, L)
{
  constexpr std::size_t    tasks_to_execute{10};
  std::vector<std::size_t> vec;
  vec.reserve(tasks_to_execute);
  pacemaker::timer::Timer timer{10ms, tasks_to_execute};
  timer.start();
  timer.wait([&vec](std::size_t i)
             { vec.emplace_back(i); });
  std::vector<std::size_t> expected(10);
  std::iota(expected.begin(), expected.end(), 0);
  EXPECT_EQ(vec, expected);
}

TEST_F(TC011, M)
{
  MockCOM mock;
  EXPECT_CALL(mock, func()).Times(1);
  pacemaker::timer::Timer timer{10ms, 0};
  timer.stop([&mock]()
             { mock.func(); });
}

TEST_F(TC011, N)
{
  bool                    running{false};
  pacemaker::timer::Timer timer{10ms, 0};
  timer.start();

  timer.stop([&timer, &running]()
             { running = timer.is_running(); });

  EXPECT_TRUE(running);
  EXPECT_FALSE(timer.is_running());
}

TEST_F(TC011, O)
{
  pacemaker::timer::Timer timer{10ms, 0};
  EXPECT_NO_THROW(timer.stop());
}

TEST_F(TC011, P)
{
  pacemaker::timer::Timer timer{10ms, 1};
  timer.start();
  timer.wait([](std::size_t) {});
  timer.stop();

  EXPECT_NO_THROW(
      {
        timer.start();
        timer.wait([](std::size_t) {});
        timer.stop();
      });
}