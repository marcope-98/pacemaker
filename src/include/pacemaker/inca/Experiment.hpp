#ifndef PACEMAKER_INCA_EXPERIMENT_HPP_
#define PACEMAKER_INCA_EXPERIMENT_HPP_

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

#include "pacemaker/inca/com/CalibrationScalarDataProxy.hpp"
#include "pacemaker/inca/com/ExperimentDeviceProxy.hpp"
#include "pacemaker/inca/com/IncaExperimentViewProxy.hpp"
#include "pacemaker/inca/com/IncaOnlineExperimentProxy.hpp"

namespace pacemaker::inca
{
  /**
   * @brief High-level manager for a single INCA measurement experiment run.
   *
   * `Experiment` composes the lower-level COM proxy objects into a coherent
   * workflow for controlling a measurement session:
   *
   * 1. **Register parameters** - call `add_param()` for each calibration
   *    scalar that the automated run needs to manipulate.
   * 2. **Start recording** - call `start_recording()` to begin data capture.
   * 3. **Modify parameters** - call `set_param()` as many times as required
   *    during the run.
   * 4. **Stop recording** - call `stop_recording()` to flush buffered data,
   *    save the recording, stop the measurement, and reset all parameters to
   *    their reference-page values.
   *
   * `Experiment` objects are not constructed directly; they are created and
   * returned by `Session::connect()`.
   *
   * The class is move-constrcutible but not copyable, reflecting the unique
   * ownership of the underlying COM resources.
   *
   * @see pacemaker::inca::Session::connect()
   * @see pacemaker::inca::com::IncaOnlineExperimentProxy
   * @see pacemaker::inca::com::CalibrationScalarDataProxy
   */
  class Experiment
  {
  public:
    /**
     * @brief Constructs an `Experiment` from its constituent COM proxies.
     *
     * All three proxies are moved into the `Experiment`, transferring
     * ownership. This constructor is intended to be called exclusively by
     * `Session::connect()`.
     *
     * @param exp     Proxy for the INCA online experiment COM object.
     * @param expview Proxy for the INCA experiment-view COM object.
     * @param device  Proxy for the primary ECU device to use during the run.
     */
    Experiment(pacemaker::inca::com::IncaOnlineExperimentProxy exp,
               pacemaker::inca::com::IncaExperimentViewProxy   expview,
               pacemaker::inca::com::ExperimentDeviceProxy     device);

    /// @brief Move constructor transfers ownership of all COM proxies.
    Experiment(Experiment &&) = default;
    /// @brief Move assignment transfers ownership of all COM proxies.
    Experiment &operator=(Experiment &&) = default;
    /// @brief Default Destructor; COM proxies are released via their own destructors.
    ~Experiment() = default;

    /**
     * @brief Starts a new measurement recording in INCA.
     *
     * Delegates to `IncaOnlineExperimentProxy::StartRecording()`. The
     * recording captures all online measurement signals configured in the
     * open INCA experiment from this point forward.
     *
     * Call `add_param()` for any calibration parameters the run needs to
     * control before calling `start_recording()`.
     */
    auto start_recording() -> void;

    /**
     * @brief Stops the recording, saves the data, and resets all parameters.
     *
     * The method performs the following steps in order:
     * 1. Sleeps for `k_flush_delay` (40 ms) to allow INCA to flush any
     *    buffered measurement samples.
     * 2. Calls `IncaOnlineExperimentProxy::StopRecordingAndSave()`.
     * 3. Calls `IncaOnlineExperimentProxy::StopMeasurement()`.
     * 4. Calls `reset()` to restore all registered calibration parameters to
     *    their reference-page values and clear the internal parameter table.
     *
     * @param filename Output recording filename/filepath.
     */
    auto stop_recording(const std::string &filename) -> void;

    /**
     * @brief Registers a calibration parameter for use during the experiment.
     *
     * Looks up the named calibration scalar on the primary device and stores
     * the resulting `CalibrationScalarDataProxy` internally. The parameter
     * can then be updated during the run via `set_param()`.
     *
     * Calling `add_param()` a second time with the same @p name is a no-op;
     * the parameter is not re-fetched from INCA.
     *
     * @param name Name fo the calibration parameter as it appears in INCA
     *             (e.g. `"Engine_TorqueRequest"`).
     *
     * @throws std::runtime_error if the parameter cannot be found on the
     *         device (propagated from
     *         `IncaOnlineExperimentProxy::GetCalibrationValueInDevice()`).
     */
    auto add_param(const std::string &name) -> void;

    /**
     * @brief Sets a registered calibration parameter to the given value.
     *
     * The parameter must have been previously registered via `add_param()`.
     * Delegates to `CalibrationScalarDataProxy::SetImplValue()`.
     *
     * @param name  Name of the calibration parameter previously registered with `add_param()`.
     * @param value New value to write to the parameter's implementation page.
     *
     * @throws std::out_of_range if @p name was not registered via `add_param()`.
     *         The exception message includes the parameter name.
     */
    auto set_param(const std::string &name, double value) -> void;

    /**
     * @brief Resets all registered parameters to their reference-page values
     *        and clears the internal parameter table.
     *
     * Iterates over all stored `CalibrationScalarDataProxy` objects and calls
     * `ResetValueToRP()` on each, then clears both the name-to-index map and
     * the proxy vector. After this call the experiment is in the same state as
     * immediately after construction.
     *
     * This method is called automatically by `stop_recording()` at the end of
     * each run, but may also be called explicitly if an early reset is required.
     */
    auto reset() -> void;

  private:
    pacemaker::inca::com::IncaOnlineExperimentProxy m_exp;
    pacemaker::inca::com::IncaExperimentViewProxy   m_expview;
    pacemaker::inca::com::ExperimentDeviceProxy     m_device;

    std::unordered_map<std::string, std::size_t>       m_map;
    std::vector<pacemaker::inca::com::CalibrationScalarDataProxy> m_values;

    /**
     * @brief Delay applied before stopping a recording to allow INCA to flush buffered measuremetn data.
     *
     * Set to 40ms, which is sufficient for INCA's internal sample-buffer
     * write cycle under normal operating conditions.
     */
    static constexpr std::chrono::milliseconds k_flush_delay{40};
  };
} // namespace inca

#endif // PACEMAKER_INCA_EXPERIMENT_HPP_