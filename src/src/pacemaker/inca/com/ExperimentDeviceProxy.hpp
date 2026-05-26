#ifndef PACEMAKER_INCA_COM_EXPERIMENTDEVICEPROXY_HPP_
#define PACEMAKER_INCA_COM_EXPERIMENTDEVICEPROXY_HPP_

#include <string>

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/DispatchWrapper.hpp"
#include "pacemaker/inca/detail/incacom.hpp"

namespace pacemaker::inca::com
{
  /**
   * @brief DispatchWrapper for the `ExperimentDevice_Dispatch` COM interface
   * 
   * Inherits `::ExperimentDevice_Dispatch` and overloads the C++ method `GetName()`,
   * which converts the COM `BSTR` return value to a `std::wstring`. Other methods
   * inherited from the base dispatch type are accessible via `operator->` on the 
   * `ExperimentDeviceProxy` owning pointer.
   * 
   * Instances are managed exclusively through the `ExperimentDeviceProxy` owning smart pointer.
   * 
   * @see ExperimentDeviceProxy
   * @see pacemaker::inca::detail::COMProxy
   */
  struct ExperimentDevice_DispatchWrapper : public pacemaker::inca::detail::DispatchWrapper<::ExperimentDevice_Dispatch>
  {
    /**
     * @brief Returns the name of the device as a `std::wstring`
     * 
     * Calls the base `ExperimentDevice_Dispatch::GetName()`, which returns a `_bstr_t`.
     * The BSTR is converted to `std::wstring` using `SysStringLen` so that the caller works 
     * entirely with standard C++ string types.
     * 
     * @return Wide string containing the device name as configured in INCA.
     */
    [[nodiscard]] auto GetName() -> std::wstring
    {
      _bstr_t bs = ::ExperimentDevice_Dispatch::GetName();
      return std::wstring(bs, SysStringLen(bs));
    }
  };

  /**
   * @brief Owning smart-pointer proxy for an INCA experiment device object.
   * 
   * `ExperimentDeviceProxy` is a type alias for `pacemaker::inca::detail::COMProxy<ExperimentDevice_DispatchWrapper>`.
   * 
   * Use `operator->` to call `GetName()` or any other inherited COM method, and
   * `get()` (inherited from `unique_ptr`) to obtain the non-owning raw `ExperimentDevice_DispatchWrapper*` required
   * by `IncaOnlineExperiment_DispatcWrapper::GetCalibrationValueInDevice()`:
   * @code
   * ExperimentDeviceProxy device{rawIDispatch};
   * std::wstring name = device->GetName();
   * // Pass the raw pointer to another COM call:
   * auto param = exp->GetCalibrationValueInDevice("Foo", device.get());
   * @endcode
   * 
   * The proxy is move-only. Copying is deleted via the `unique_ptr` base.
   * 
   * @see ExperimentDevice_DispatchWrapper
   * @see pacemaker::inca::detail::COMProxy
   * @see IncaOnlineExperiment_DispatchWrapper::GetAllDevice()
   * @see IncaOnlineExperiment_DispatchWrapper::GetCalibrationValueInDevice()
   */
  using ExperimentDeviceProxy = pacemaker::inca::detail::COMProxy<ExperimentDevice_DispatchWrapper>;
} // namespace pacemaker::inca::com

#endif // PACEMAKER_INCA_COM_EXPERIMENTDEVICEPROXY_HPP_