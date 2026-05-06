#ifndef PACEMAKER_INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_
#define PACEMAKER_INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_

#include <string>
#include <vector>

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

struct IncaOnlineExperiment_Dispatch;
struct IDispatch;
struct ExperimentDevice_Dispatch;

namespace pacemaker::inca::com
{
  /**
   * @brief Proxy for the INCA online experiment COM object.
   *
   * `IncaOnlineExperimentProxy` wraps the `IncaOnlineExperiment_Dispatch` COM
   * interface, which controls the active measurement session in INCA. It
   * provides the following capabilities:
   *
   * - Enumerating connected ECU devices.
   * - Starting and stopping measurement recordings.
   * - Retrieving calibration scalar data objects by name from a specific device.
   *
   * The class is move-only. All SAFEARRAY and BSTR marshalling is handled
   * internally so that callers work exclusively with standard C++ types.
   *
   * @see pacemaker::inca::com::ExperimentDeviceProxy
   * @see pacemaker::inca::com::CalibrationScalarDataProxy
   * @see pacemaker::inca::detail::COMProxy
   */
  class IncaOnlineExperimentProxy : private pacemaker::inca::detail::COMProxy<::IncaOnlineExperiment_Dispatch>
  {
  public:
    /**
     * @brief Constructs the proxy by narrowing a generic `IDispatch` to `IncaOnlineExperiment_Dispatch`.
     *
     * @param idispatch Owning pointer to a generic `IDispatch` representing
     *                  the online experiment object. Ownership is transferred
     *                  to the proxy.
     *
     * @throws std::invalid_argument if @p idispatch is null.
     * @throws std::runtime_error    if `QueryInterface` fails to obtain `IncaOnlineExperiment_Dispatch`.
     */
    explicit IncaOnlineExperimentProxy(pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch);
    /// @brief Move constructor transfers ownership of the COM object.
    IncaOnlineExperimentProxy(IncaOnlineExperimentProxy &&) = default;
    /// @brief Move assignment transfers ownership of the COM object.
    IncaOnlineExperimentProxy &operator=(IncaOnlineExperimentProxy &&) = default;
    /// @brief Destructor; releases the underlying COM object.
    ~IncaOnlineExperimentProxy();

    /**
     * @brief Returns all ECU devices associated with the open experiment.
     *
     * Calls `GetAllDevices()` on the underlying COM object, unpacks the
     * returned `SAFEARRAY` of `VT_DISPATCH` elements, and returns each
     * element as an owning `IDispatch` pointer. Each pointer can subsequently
     * be used to construct an `ExperimentDeviceProxy`.
     *
     * @return Vector of owning `IDispatch` pointers, one per device. The
     *         vector si empty if no devices are configured.
     *
     * @throws std::runtime_error if the returned SAFEARRAY is not of type
     *         `VT_ARRAY | VT_VARIANT`, if `SafeArrayGetElement` fails, or
     *         if any element is not of type `VT_DISPATCH`.
     */
    [[nodiscard]] auto GetAllDevices() -> std::vector<pacemaker::inca::detail::unique_com_ptr<::IDispatch>>;

    /**
     * @brief Stops the active measurement session.
     *
     * Delegates directly to `IncaOnlineExperiment_Dispatch::StopMeasurement()`.
     * Call this after `StopRecordingAndSave()` to fully stop the measurement.
     */
    auto StopMeasurement() -> void;

    /**
     * @brief Starts a new measurement recording.
     *
     * Delegates directly to `IncaOnlineExperiment_Dispatch::StartRecording()`.
     * The recording captures all online measurement signals configured in the
     * open INCA experiment.
     */
    auto StartRecording() -> void;

    /**
     * @brief Stops the current recording and saves the collected data.
     *
     * Delegates directly to `IncaOnlineExperiment_Dispatch::StopRecordingAndSave()`. The data is
     * persisted by INCA to the location configured in the experiment. Call `StopMeasurement()`
     * afterwards to fully stop the session.
     *
     * @note A short flush delay should be observed before calling this method to ensure all
     *       buffered measurement samples have been written. `pacemaker::inca::Experiment::stop_recording()`
     *       handles this automatically.
     */
    auto StopRecordingAndSave() -> void;

    /**
     * @brief Retrieves a calibration scalar data item by name from a specific device.
     *
     * Looks up the named calibration parameter on the given device and returns an owning `IDispatch`
     * pointer that can be used to construct a `CalibrationScalarDataProxy`.
     *
     * @param name   Name of the calibration parameter as configured in INCA.
     * @param device Non-owning raw pointer to the device from which the parameter is retrieved.
     *               Obtain this via `ExperimentDeviceProxy::get()`.
     *
     * @return Owning `IDispatch` pointer to the calibration scalar data object.
     *
     * @throws std::runtime_error if the parameter is not found on the device
     *         (the underlying COM call returns a nullptr).
     */
    auto GetCalibrationValueInDevice(const std::string &name, ::ExperimentDevice_Dispatch *const device) -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>;
  };

} // namespace pacemaker::inca::com
#endif // PACEMAKER_INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_