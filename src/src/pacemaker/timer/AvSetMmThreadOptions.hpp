#ifndef PACEMAKER_TIMER_AVSETMMTHREADOPTIONS_HPP_
#define PACEMAKER_TIMER_AVSETMMTHREADOPTIONS_HPP_

#include <avrt.h>

namespace pacemaker::timer
{
  /**
   * @brief RAII guard for Windows Multimedia Scheduler Service (MMCSS) thread elevation.
   *
   * On construction, registers the calling thread with the MMCSS `"Pro Audio"` task and sets its
   * priority to `AVRT_PRIORITY_HIGH` via `AvSetMmThreadCharacteristics` and `AvSetMmThreadPriority`.
   * On destruction, reverts the thread's scheduling characteristics via `AvRevertMmThreadCharacteristics`.
   *
   * MMCSS elevation reduces the likelihood that the Windows scheduler preempts the timer loop thread in favour
   * of lower-priority background work, which in turn reduces jitter in the period at which `Timer::wait()`'s
   * callback is invoked.
   *
   * This struct is constructed as a local variable at the top of `Timer::wait()` so that MMCSS eleveation
   * covers the entire loop and is automatically reverted when `wait()` returns, regardless of whether it returns
   * normally or via an exception.
   *
   * @note This type is an internal implementation detail of `Timer`. User code should not construct it directly.
   *
   * @see pacemaker::timer::Timer::wait()
   */
  struct AvSetMmThreadOptions
  {
    /**
     * @brief Elevates the calling thread to the MMCSS "Pro Audio" task.
     *
     * Calls `AvSetMmThreadCharacteristics` to register the thread via MMCSS, then calls `AvSetMmThreadPriority`
     * to raise its scheduling priority within the task.
     *
     * @note if `AvSetMmThreadCharacteristics` fails it returns NULL; no exception is thrown in the current implementation.
     */
    auto av_set() -> void
    {
      DWORD taskIndex{0};
      this->hTask = AvSetMmThreadCharacteristics(TEXT("Pro Audio"), &taskIndex);
      AvSetMmThreadPriority(this->hTask, AVRT_PRIORITY_HIGH);
    }

    /**
     * @brief Reverts the calling thread's MMCSS scheduling characteristics.
     *
     * Calls `AvRevertMmThreadCharacteristics` to de-register the thread from the MMCSS "Pro Audio" task,
     * restoring its normal scheduling behaviour.
     */
    auto av_revert() -> void { AvRevertMmThreadCharacteristics(this->hTask); }

  private:
    /// @brief Handle returned by `AvSetMmThreadCharacteristics`; used to revert the thread on destruction.
    HANDLE hTask;
  };
} // namespace pacemaker::timer

#endif // PACEMAKER_TIMER_AVSETMMTHREADOPTIONS_HPP_