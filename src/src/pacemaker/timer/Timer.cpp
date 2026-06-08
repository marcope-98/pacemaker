#include "pacemaker/timer/Timer.hpp"

#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pacemaker/timer/AvSetMmThreadOptions.hpp"

namespace pacemaker::timer
{
  struct Timer::Impl
  {
    HANDLE handle;
    LONG   period;
    bool   running{false};

    AvSetMmThreadOptions opts;

    explicit Impl(const std::chrono::milliseconds &period)
        : period{static_cast<LONG>(period.count())}
    {
      if (this->period <= 0) throw std::invalid_argument("period must be stricly positive");
      this->handle = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
      if (this->handle == NULL) throw std::runtime_error("Could not instantiate WaitableTimer.");
    }

    ~Impl()
    {
      if (this->handle)
      {
        CancelWaitableTimer(this->handle);
        CloseHandle(this->handle);
      }
    }
  };
} // namespace pacemaker::timer

pacemaker::timer::Timer::Timer(const std::chrono::milliseconds &period,
                               const std::size_t               &tasks_to_execute)
    : pimpl{std::make_unique<Impl>(period)},
      m_tasksToExecute{tasks_to_execute} {}

pacemaker::timer::Timer::~Timer()
{
  if (this->pimpl->running)
    this->stop();
}

auto pacemaker::timer::Timer::start(const std::function<void(void)> &StartFcn) -> void
{
  if (this->pimpl->running)
    throw std::runtime_error("Timer is already running");

  this->pimpl->running = true;

  LARGE_INTEGER due{};
  due.QuadPart = -4000000LL; // 40ms
  SetWaitableTimer(this->pimpl->handle, &due, this->pimpl->period, NULL, NULL, FALSE);

  StartFcn();
}

auto pacemaker::timer::Timer::stop(const std::function<void(void)> &StopFcn) -> void
{
  StopFcn();
  this->pimpl->running = false;
}

auto pacemaker::timer::Timer::wait(const std::function<void(std::size_t)> &TimerFcn) -> void
{
  if (!this->pimpl->running)
    throw std::runtime_error("Timer was not started correctly. Did you forget a timer.start()?");

  this->pimpl->opts.av_set();
  for (std::size_t i{}; i < this->m_tasksToExecute; ++i)
  {
    WaitForSingleObject(this->pimpl->handle, INFINITE);
    TimerFcn(i);
  }
  this->pimpl->opts.av_revert();
}
auto pacemaker::timer::Timer::set_thread_opts() -> void { this->pimpl->opts.av_set(); }

auto pacemaker::timer::Timer::revert_thread_opts() -> void { this->pimpl->opts.av_revert(); }

auto pacemaker::timer::Timer::wait_for_single_object() -> void { WaitForSingleObject(this->pimpl->handle, INFINITE); }

auto pacemaker::timer::Timer::is_running() const -> bool { return this->pimpl->running; }
