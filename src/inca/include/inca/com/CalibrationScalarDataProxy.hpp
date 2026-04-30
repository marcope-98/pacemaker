#ifndef INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_
#define INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_

#include "inca/detail/COMProxy.hpp"
#include "inca/detail/unique_com_ptr.hpp"

struct CalibrationScalarData_Dispatch;
struct IDispatch;

namespace inca::com
{
  /**
   * @brief Proxy for an INCA calibration scalar data COM object.
   *
   * `CalibrationScalarDataProxy` wraps the `CalibrationScalarData_Dispatch`
   * COM interface, which represents a single scalar calibration parameter
   * (implementation value) within an INCA experiment device.
   *
   * It provides two operations:
   * - Setting the implementation value to an arbitrary `double` during an
   *   experiment run.
   * - Resetting the value back to its reference-page (RP) baseline when the
   *   experiment ends.
   *
   * Instances are normally created by `inca::Experiment::add_param()` and
   * stored in its internal parameter table, Direct construction requires a
   * valid `IDispatch` pointer obtained from
   * `IncaOnlineExperimentProxy::GetCalibrationValueInDevice()`
   *
   * The class is move-only.
   *
   * @see inca::com::IncaOnlineExperimentProxy::GetCalibrationValueInDevice()
   * @see inca::Experiment::add_param()
   * @see inca::Experiment::set_param()
   * @see inca::detail::COMProxy
   */
  class CalibrationScalarDataProxy : private inca::detail::COMProxy<::CalibrationScalarData_Dispatch>
  {
  public:
    /**
     * @brief Constructs the proxy by narrowing a generic `IDispatch` to `CalibrationScalarData_Dispatch`.
     *
     * @param idispatch Owning pointer to a generic `IDispatch` representing
     *                  the calibration scalar data object. Ownership is
     *                  transferred to the proxy.
     *
     * @throws std::invalid_argument if @p idispatch is null.
     * @throws std::runtime_error    if `QueryInterface` fails to obtain `CalibrationScalarData_Dispatch`
     */
    explicit CalibrationScalarDataProxy(inca::detail::unique_com_ptr<::IDispatch> idispatch);
    /// @brief Move constructor transfers ownership of the COM object.
    CalibrationScalarDataProxy(CalibrationScalarDataProxy &&) = default;
    /// @brief Move assignment transfers ownership of the COM object.
    CalibrationScalarDataProxy &operator=(CalibrationScalarDataProxy &&) = default;
    /// @brief Destructor; releases the underlying COM object.
    ~CalibrationScalarDataProxy();

    /**
     * @brief Sets the implementation (working-page) value of the calibration parameter.
     *
     * Delegates to `CalibrationScalarData_Dispatch::SetImplValue()`. The
     * change takes effect immediately in the running INCA measurement session.
     *
     * @param value The new value to write to the calibration parameter.
     */
    auto SetImplValue(double value) -> void;

    /**
     * @brief Resets the calibration parameter to its reference-page value.
     *
     * Delegates to `CalibrationScalarData_Dispatch::ResetValueToRP()`. This
     * restores the parameter to the baseline value stored on the reference
     * page, undoing any changes applied via `SetImplValue()`.
     *
     * This method is called by `inca::Experiment::reset()` on every registered
     * parameter at the end of an experiment run to leave INCA in a clean state.
     */
    auto ResetValueToRP() -> void;
  };
} // namespace inca::com

#endif // INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_