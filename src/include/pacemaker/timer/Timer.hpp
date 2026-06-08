#ifndef PACEMAKER_TIMER_TIMER_HPP_
#define PACEMAKER_TIMER_TIMER_HPP_
#include <chrono>
#include <functional>
#include <memory>

namespace pacemaker::timer
{
  /**
   * @brief Fixed-rate, fixed-count periodic timer backed by a Windows high-resolution waitable timer.
   *
   * `Timer` wraps the Win32 `CreateWaitableTimerExW` / `WaitForSingleObject` API with
   * `CREATE_WAITABLE_TIMER_HIGH_RESOLUTION` to achieve sub-millisecond scheduling accuracy on supported
   * hardware. It is designed for deterministic, real-time control loops where a callback must execute
   * at a precise, fixed rate for a known number of steps.
   *
   * ### Execution model
   * The timer operates as a three-phase object:
   *
   * 1. **`start(StartFcn)`**: arms the waitable timer and invokes the optional start callback on the calling thread.
   * 2. **`wait(TimerFcn)`**: blocks the calling thread, elevates it to the MMCSS "Pro Audio" scheduling class,
   *    then loops `tasks_to_execute` timer. Each iteration waits for the next timer tick and invokes `TimerFcn` with
   *    the current zero-based iteration index.
   * 3. **`stop(StopFcn)`**: invokes the optional stop callback and marks the timer as no longer running. Called
   *    automatically by the destructor if the timer is still running at that point.
   *
   * ### Typical usage
   * @code
   * using namespace std::chrono_literls;
   * pacemaker::timer::Timer timer{10ms, 500}; // 10ms period, 500 ticks
   *
   * timer.start([]{ std::cout << "Recording started\n"; });
   * timer.wait([](std::size_t i){
   *  writeDataPoint(i); // called every 10ms, 500 times
   * });
   * timer.stop([]{ std::cout << "Recording stopped\n"; });
   * @endcode
   *
   * ### Threading
   * `start()`, `wait()`, and `stop()` must be called from the same thread in the order shown above.
   * `Timer` is non-copyable and non-movable.
   *
   * ### First-tick delay
   * The waitable timer is armed with an initial due time of -400ms (expressed as a 100-ns relative interval:
   * -4 000 000 x 100 ns) so that the first tick fires approximately 40ms after `start()` is called, giving
   * downstram systems time to initialise before data capture begins.
   *
   * @note Platform: Windows only. Requires `CREATE_WAITABLE_TIMER_HIGH_RESOLUTION`, which is available on Windows10
   *       version 1803 and later
   *
   * @see pacemaker::timer::AvSetMmThreadOptions
   */
  class Timer
  {
  public:
    /**
     * @brief Constructs a timer with the given period and iteration count.
     *
     * Creates the underlying Win32 high-resolution waitable timer via `CreateWaitableTimerExW` with
     * `CREATE_WAITABLE_TIMER_HIGH_RESOLUTION`. The timer is created in a stopped, unarmed state; call
     * `start()` to arm it.
     *
     * @param period           Tick interval, Must be a positive duration; the value is converted to milliseconds
     *                         and passed as the `lPeriod` argument to `SetWaitableTimer`.
     * @param tasks_to_execute Total number of timer ticks (loop iterations) that `wait()` will block for before returning.
     *
     * @throws std::runtime_error if `CreateWaitableTimerExW` returns NULL (e.g.\ insufficient privileges or the
     *         high-resolution timer is unavailable on the current platform).
     */
    Timer(const std::chrono::milliseconds &period, const std::size_t &tasks_to_execute);
    /// @brief Copying is deleted; The Win32 timer handle must not be duplicated.
    Timer(const Timer &) = delete;
    /// @brief Move construction is deleted; the timer has a unique identity.
    Timer(Timer &&) = delete;
    /// @brief Copying assignment is deleted; The Win32 timer handle must not be duplicated.
    Timer &operator=(const Timer &) = delete;
    /// @brief Move assignment is deleted; the timer has a unique identity.
    Timer &operator=(Timer &&) = delete;

    /**
     * @brief Destructor; stops the timer if running and releases the Win32 handle.
     *
     * If the timer is still running when the destructor executes `stop()` is called with the default (no-op)
     * stop callback. The Win32 waitable timer is then cancelled via `CancelWaitableTimer` and its handle is
     * closed via `CloseHandle`
     */
    ~Timer();

    /**
     * @brief Arms the timer and invokes the start callback.
     *
     * Arms the Win32 waitable timer with an initial due time of -400ms (~40ms from now, expressed as a relative 100-ns
     * interval) and the configured period, then calls @p StartFcn on the calling thread.
     *
     * The timer must **not** already be running when this method is called.
     *
     * @param StartFcn Optional callback invoked once on the calling thread immediately after the timer is armeds.
     *                 Defaults to a no-op lambda. Use it to perform any initialisation tht must happen at the moment
     *                 recording begins (e.g.\ opening a file, logging a timestamp).
     *
     * @throws std::runtime_error if the timer is already running.
     */
    auto start(const std::function<void(void)> &StartFcn = []() {}) -> void;

    /**
     * @brief Invokes the stop callback and marks the timer as no longer running.
     *
     * Calls @p StopFcn on the calling thread and then clears the internal running flag. This method does **not**
     * cancel the Win32 waitable timer handle; handle cleanup is performed by the destructor.
     *
     * `stop()` is called automatically by the destructor if the timer is still running at destruction time.
     *
     * @param StopFcn Optional callback invoked once on the calling thread before the running flag is cleared. Defaults to a no-op lambda.
     *                Use it to perform any teardown that must happen at the moment recording ends (e.g.\ flushing a buffer, logging a timestamp).
     */
    auto stop(const std::function<void(void)> &StopFcn = []() {}) -> void;

    /**
     * @brief Blocks the calling thread and drives the timer callback to `tasks_to_execute` iterations.
     *
     * Elevates the calling thread to the Windows Multimedia Class Scheduling Service (MMCSS) "Pro Audio" task
     * at `AVRT_PRIORITY_HIGH` for the duration of the loop, then iterates `tasks_to_execute` times. Each iteration:
     * 1. Calls `WaitForSingleObject(handle, INFINITE)` to block until the next timer tick fires.
     * 2. Invokes @p TimerFcn with the current zero-based iteration index.
     *
     * MMCSS elevation is automatically reverted when `wait()` returns (via the RAII destructor of `AvSetMmThreadOptions`).
     *
     * `wait()` returns only after all `tasks_to_execute` ticks have been processed. Call `stop()` after `wait()` returns to
     * peform teardown.
     *
     * @param TimerFcn Callback invoked once per timer tick. Receives the zero-based iteration index (`0 ... tasks_to_execute - 1`)
     *                 as its sole argument. The callback runs on the calling thread synchronously within the loop body; it must
     *                 complete before the next `WaitForSingleObject` call.
     *
     * @throws std::runtime_error if `start()` was not called before `wait()`
     *
     * @note If @p TimerFcn takes longer than one tick period to execute, subsequent ticks will fire late because the timer
     *       is re-armed relative to the previous expiry by the OS, not relative to when `WaitForSingleObject` returns.
     */
    auto wait(const std::function<void(std::size_t)> &TimerFcn) -> void;

    /// @brief Getter to check whether or not the timer is running
    [[nodiscard]] auto is_running() const -> bool;

  private:
    /// @brief Wrapper to OS-dependent function that waits for next timer expiration
    auto wait_for_single_object() -> void;
    /// @brief Sets OS-dependent thread priority elevation options
    auto set_thread_opts() -> void;
    /// @brief Reverts OS-dependent thread priority elevation options
    auto revert_thread_opts() -> void;

    struct Impl;
    /// @brief Owning pointer to the pimpl; heap-allocated by the constructor.
    std::unique_ptr<Impl> pimpl;

    /**
     * @brief Total number of timer ticks that `wait()` will process before returning.
     *
     * Set once at construction and never modified. Passed as the upper bound of the loop in `wait()`.
     */
    std::size_t m_tasksToExecute{};
  };
} // namespace pacemaker::timer
#endif // PACEMAKER_TIMER_TIMER_HPP_