#ifndef PACEMAKER_INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_
#define PACEMAKER_INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_

#include <stdexcept>
#include <string>
#include <vector>

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/DispatchWrapper.hpp"
#include "pacemaker/inca/detail/incacom.hpp"

#include "pacemaker/inca/com/CalibrationScalarDataProxy.hpp"
#include "pacemaker/inca/com/ExperimentDeviceProxy.hpp"

namespace pacemaker::inca::com
{
  /**
   * @brief DispatchWrapper for the `IncaOnlineExperiment_Dispatch` COM interface.
   * 
   * Inherits `::IncaOnlineExperiment_Dispatch` and overload two C++ methods that hide COM SAFEARRAY
   * and BSTR marshalling behind standard C++ return types. Methods that require no adaptation
   * are inherited directly from the base and accessed via `operator->` on the `IncaOnlineExperimentProxy` owning pointer.
   * 
   * Instances are managed exclusively through the `IncaOnlineExperimentProxy` owning smart pointer.
   * 
   * @see IncaOnlineExperimentProxy
   * @see pacemaker::inca::detail::COMProxy
   */
  struct IncaOnlineExperiment_DispatchWrapper : public pacemaker::inca::detail::DispatchWrapper<::IncaOnlineExperiment_Dispatch>
  {
    /**
     * @brief Returns all ECU devices associated with the open experiment.
     * 
     * Calls the base `IncaOnlineExperiment_Dispatch::GetAllDevices()`, unapckas the returned `SAFEARRAY`
     * of `VT_DISPATCH` elements, and constructs an `ExperimentDeviceProxy` for each element by narrowing
     * the individual `IDispatch` pointers.
     * 
     * @return `std::vector<ExperimentDeviceProxy>` containing one proxy per device reported by INCA.
     *         The vector is empty if no devices are configured in the open experiment.
     * 
     * @throws std::runtime_error if the returned variant is not of type `VT_ARRAY | VT_VARIANT`.
     * @throws std::runtime_error if `SafeArrayGetElement` fails for any element.
     * @throws std::runtime_error if any element variant is not `VT_DISPATCH`.
     */
    [[nodiscard]] auto GetAllDevices() -> std::vector<ExperimentDeviceProxy>
    {
      _variant_t device_list = ::IncaOnlineExperiment_Dispatch::GetAllDevices();
      if (device_list.vt != (VT_ARRAY | VT_VARIANT))
        throw std::runtime_error("GetAllDevices returned an unexpected type.");

      SAFEARRAY *psa = V_ARRAY(&device_list);
      long       lLower{}, lUpper{};
      SafeArrayGetLBound(psa, 1, &lLower);
      SafeArrayGetUBound(psa, 1, &lUpper);

      std::vector<ExperimentDeviceProxy> out{};
      out.reserve(static_cast<std::size_t>(lUpper - lLower + 1));
      for (long i{lLower}; i <= lUpper; ++i)
      {
        _variant_t device;
        if (FAILED(SafeArrayGetElement(psa, &i, &device)))
          throw std::runtime_error("GetAllDevices: SafeArrayGetElement failed");

        if (device.vt != VT_DISPATCH)
          throw std::runtime_error("GetAllDevices: device element is not VT_DISPATCH");

        auto idispatch = (IDispatch *)(device);
        out.emplace_back(idispatch);
      }
      return out;
    }

    /**
     * @brief Retrieves a calibration scalar data item by name from a specific device.
     * 
     * Calls the base `IncaOnlineExperiment_Dispatch::GetCalibrationValueInDevice()`, detaches the returned
     * `IDispatch`, and constructs a `CalibrationScalarDataProxy` by narrowing the pointer via `query_interface`.
     * 
     * @param name   Name of the calibration parameter as configured in INCA
     * @param device Non-owning raw pointer to the device on which to look up the parameter. Obtain this via `ExperimentDeviceProxy::get()`.
     * 
     * @return Owning `CalibrationScalarDataProxy` for the named parameter.
     * 
     * @throws std::runtime_error if the parameter is not found on the device (the underlying COM call returns a nullptr).
     */
    [[nodiscard]] auto GetCalibrationValueInDevice(const std::string &name, ::ExperimentDevice_Dispatch *const device) -> CalibrationScalarDataProxy
    {
      ::IDispatch *raw = ::IncaOnlineExperiment_Dispatch::GetCalibrationValueInDevice(name.c_str(), device).Detach();
      if (raw == nullptr)
        throw std::runtime_error("GetCalibrationValueInDevice: parameter not found " + name);
      return CalibrationScalarDataProxy(raw);
    }
  };

  /**
   * @brief Owning smart-pointer proxy for the INCA online experiment COM object.
   * 
   * `IncaOnlineExperimentProxy` is a type alias for `pacemaker::inca::detail::COMProxy<IncaOnlineExperiment_DispatchWrapper>`.
   * 
   * Use `operator->` to access both the C++ helper methods defined on `IncaOnlineExperiment_DisaptchWrapper` and the inherited
   * COM methods
   * @code
   * IncaOnlineExperimentProxy exp{rawIDispatch};
   * auto devices = exp->GetAllDevices(); // overloaded C++ method
   * exp->StartRecording();               // inherited COM method
   * exp->StopRecordingAndSave();         // inherited COM method
   * exp->StopMeasurement();              // inherited COM method
   * @endcode
   * 
   * The proxy is move-only. Copying is deleted via the `unique_ptr` base.
   * 
   * @see IncaOnlineExperiment_DispatchWrapper
   * @see pacemaker::inca::detail::COMProxy
   * @see pacemaker::inca::Session
   */
  using IncaOnlineExperimentProxy = pacemaker::inca::detail::COMProxy<IncaOnlineExperiment_DispatchWrapper>;
} // namespace pacemaker::inca::com
#endif // PACEMAKER_INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_