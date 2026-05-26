#ifndef PACEMAKER_INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_
#define PACEMAKER_INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/DispatchWrapper.hpp"
#include "pacemaker/inca/detail/incacom.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

namespace pacemaker::inca::com
{
  /**
   * @brief Dispatch Wrapper for the `CalibrationScalarData_Dispatch` COM interface.
   *
   * Inherits `::CalibrationScalarData_Dispatch` and adds no additional C++
   * methods. The two primary operations, writing the implementation value
   * and resetting to the reference-page value, are inherited from the base
   * dispatch type and accessed via `operator->` on the
   * `CalibrationScalarDataProxy` owning pointer:
   *
   * @code
   * CalibrationScalarDataProxy param{rawIDispatch};
   * param->SetImplValue(120.0);  // write a new value
   * param->ResetValueToRP();     // restore reference-page baseline
   * @endcode
   *
   * Instances are managed exclusively through the `CalibrationScalarDataProxy` owning smart pointer
   *
   * @see CalibrationScalarDataProxy
   * @see pacemaker::inca::detail::COMProxy
   */
  struct CalibrationScalarData_DispatchWrapper : public pacemaker::inca::detail::DispatchWrapper<::CalibrationScalarData_Dispatch>
  {
  };

  /**
   * @brief Owning smart-pointer proxy for an INCA calibration scalar data COM object.
   *
   * `CalibrationScalarDataProxy` is a type alias for
   * `pacemaker::inca::detail::COMProxy<CalibrationScalarData_DispatchWrapper>`.
   *
   * | Method | Description |
   * |---|---|
   * | `SetImplValue(double)` | Writes a new value to the parameter's implementation page. |
   * | `ResetValueToRP()` | Restores the parameter to its reference-page baseline. |
   *
   * The proxy is move-only. Copying is deleted via the `unique_ptr` base.
   *
   * @see CalibrationScalarData_DispatchWrapper
   * @see pacemaker::inca::detail::COMProxy
   * @see IncaOnlineExperiment_DispatchWrapper::GetCalibrationValueInDevice()
   * @see pacemaker::inca::Session::set_param()
   * @see pacemaker::inca::Session::reset()
   */
  using CalibrationScalarDataProxy = pacemaker::inca::detail::COMProxy<CalibrationScalarData_DispatchWrapper>;
} // namespace pacemaker::inca::com

#endif // PACEMAKER_INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_