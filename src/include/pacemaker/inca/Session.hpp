#ifndef PACEMAKER_INCA_SESSION_HPP_
#define PACEMAKER_INCA_SESSION_HPP_

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include "pacemaker/inca/pacemaker.inca_export.h"

struct ::IDispatch;

namespace pacemaker::inca
{
  /**
   * @brief Top-level session object that owns the connection to a running INCA instance.
   *
   * `Session` is the single entry point for the pacemaker library. It establishes a COM connection to a locally
   * running INCA process, validates that an experiment is open and at least one device is online and exposes the
   * complete measurement workflow, parameter management, recording lifecycle, and teardown, through a single, opaque
   * object.
   *
   * ### Typical usage
   * @code
   * CoInitialize(NULL);
   * auto session = pacemaker::inca::Session::connect();
   *
   * session.add_param("Engine_TorqueRequest");
   * session.start_recording();
   * session.set_param("Engine_TorqueRequest", 120.0);
   * session.stop_recording("run_001.mf4");
   *
   * // Session destructor calls DisconnectFromTool() automatically
   * CoUninitialize();
   * @endcode
   *
   * ### Lifetime and thread safety
   * The session is non-copyable and non-movable. Exactly one `Session` should exist per INCA automation connection.
   * COM must be initialised on the calling thread before `connect()` is invoked.
   *
   * On destruction, `Session`'s destructors calls `DisconnectFromTool()` on the INCA COMM object, ensuring INCA is cleanly
   * notified of the disconnection.
   *
   * @note Platform: Windows only. INCA must be installed and running.
   */
  class Session
  {
  public:
    /**
     * @brief Factory function that connects to a running INCA instance and returns an initialised `Session`.
     *
     * The function allows to specify a custom way to initialize Session and its underlying structures.
     *
     * @param factory Function returning the argument to initialize the underlying IncaProxy member variable
     *
     * @return A fully constructed `Session` ready for use.
     *
     * @throws std::runtime_error if `CoCreateInstance` fails (INCA not installed or not running).
     * @throws std::runtime_error if the open experiment has no devices configured or online.
     *
     * @pre COM must be initialised on the calling thread (e.g.\ `CoInitialize` oir `CoInitializeEx`)
     */
    [[nodiscard]] static auto PACEMAKER_INCA_EXPORT connect(const std::function<IDispatch *(void)> &factory) -> Session;

    /**
     * @brief Factory function that connects to a running INCA instance and returns an initialised `Session`.
     *
     * The function performs the following steps:
     * 1. Calls `CoCreateInstance(CLSID_Inca, CLSCTX_LOCAL_SERVER)` to obtain the root INCA COM object.
     * 2. Retrieves the open experiment and experiment-view objects via `IncaProxy`.
     * 3. Enumerates connected devices; selects the first available device.
     * 4. Constructs and returns a `Session` owning an `IncaProxy` and the fully initialised `Experiment`.
     *
     * @return A fully constructed `Session` ready for use.
     *
     * @throws std::runtime_error if `CoCreateInstance` fails (INCA not installed or not running).
     * @throws std::runtime_error if the open experiment has no devices configured or online.
     *
     * @pre COM must be initialised on the calling thread (e.g.\ `CoInitialize` oir `CoInitializeEx`)
     */
    [[nodiscard]] static auto PACEMAKER_INCA_EXPORT connect() -> Session;

    /// @brief Copying is deleted; the COM connection must not be duplicated.
    Session(const Session &) = delete;
    /// @brief Move construction is deleted; the session has a unique identity.
    Session(Session &&) = delete;
    /// @brief Copying assignment is deleted; the COM connection must not be duplicated.
    Session &operator=(const Session &) = delete;
    /// @brief Move assignment is deleted; the session has a unique identity.
    Session &operator=(Session &&) = delete;

    /**
     * @brief Destructor; triggers disconnection from INCA via `IncaProxy`.
     *
     * `DisconnectFromTool()` is called on the `Incaproxy` object, so INCA is notified of the disconnection.
     * Then the Proxies are destroyed in reverse order.
     */
    PACEMAKER_INCA_EXPORT ~Session();

    /**
     * @brief Registers a calibration parameter for use during the experiment.
     *
     * Looks up the named calibration scalar on the primary device via
     * `IncaOnlineExperiment_DispatchWrapper::GetCalibrationValueInDevice()` and stores the resulting
     * `CalibrationScalarDataProxy` in the internal parameter table. The parameter can then be updated during
     * the run via `set_param()`.
     *
     * Calling `add_param()` a second time with the same @p naem is a no-op; the parameter is not re-fetched from INCA.
     *
     * @param name Name of the calibration parameter as it appears in INCA.
     *
     * @throws std::runtime_error if the parameter cannot be found in the primary device.
     */
    auto PACEMAKER_INCA_EXPORT add_param(const std::string &name) -> void;

    /**
     * @brief Sets a registered calibration parameter to the given value.
     *
     * Looks up @p name in the internal parameter table and calls `SetImplValue(value)` via `operator->` on the
     * stored `CalibrationScalarDataProxy`. The change takes effect immediately in the running INCA measurement session.
     *
     * @param name  Name of the calibration parameter previously registered via `add_param()`.
     * @param value New value to write to the parameter's implementation page.
     *
     * @note returns silently if @p name was not registered via `add_param()`.
     */
    auto PACEMAKER_INCA_EXPORT set_param(const std::string &name, double value) -> void;

    /**
     * @brief Sets a registered calibration parameter to the given value.
     *
     * Looks up @p idx in the internal vector container and calls `SetImplValue(value)` via `operator->` on the
     * stored `CalibrationScalarDataProxy`. The change takes effect immediately in the running INCA measurement session.
     * 
     * @param idx   Index of the calibration parameter previously registered via the idx-th call to `add_param()`.
     * @param value New value to write to the parameter's implementation page.
     * 
     * @note returns silently if @p idx is out of range.
     */
    auto PACEMAKER_INCA_EXPORT set_param(const std::size_t &idx, double value) -> void;

    /**
     * @brief Resets all registered parameters to their reference-page values.
     *
     * Iterates over all stored `CalibrationScalarDataProxy` objects and calls `ResetValueToRP()` via `operator->`
     * on each. After this call the session is in the same state as immediately after `connect(...)`, no parameters are
     * registered and INCA's calibration values are restored to their reference-page baseline.
     *
     * This method is called automatically by `stop_recording()` at the end of each run, but may also be called explicitly if
     * an early reset is needed.
     */
    auto PACEMAKER_INCA_EXPORT reset() -> void;

    /**
     * @brief Starts a new measurement recording in INCA.
     *
     * Delegates to `StartRecording()` on the internal `IncaOnlineExperimentProxy` via `operator->`. The recording captures
     * all online measurement signals configured in the open INCA experiment from this point forward.
     *
     * Register all calibration parameters via `add_param()` before calling `start_recording()`.
     */
    auto PACEMAKER_INCA_EXPORT start_recording() -> void;

    /**
     * @brief Stops the recording, saves the measurement data, and resets all registered parameters.
     *
     * The method performs the following steps in order:
     * 1. Sleeps for `k_flush_delay` (40 ms) to allow INCA to flush any buffered measurement samples.
     * 2. Stops the INCA recording and saves the recording
     * 3. Stop measurement
     * 4. Resets all registered calibration parameters to their reference-page values
     *
     * @param filename Output recording filename or filepath passed to INCA.
     */
    auto PACEMAKER_INCA_EXPORT stop_recording(const std::filesystem::path &filename) -> void;

  private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;

    explicit Session(const std::function<IDispatch *(void)> &factory);

    /**
     * @brief Minimum delay before stopping a recording to allow INCA to flush buffered measurement data.
     *
     * Set to 40 ms, which is sufficient for INCA's internal sample-buffer write cycle under normal operating
     * conditions. Applied at the start of `stop_recording()` via `std::this_thread::sleep_for`.
     */
    static constexpr std::chrono::milliseconds k_flush_delay{40};
  };
} // namespace pacemaker::inca
#endif // PACEMAKER_INCA_SESSION_HPP_