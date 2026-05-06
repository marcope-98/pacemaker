#ifndef PACEMAKER_INCA_COM_EXPERIMENTDEVICEPROXY_HPP_
#define PACEMAKER_INCA_COM_EXPERIMENTDEVICEPROXY_HPP_

#include <string>

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

struct IDispatch;
struct ExperimentDevice_Dispatch;

namespace pacemaker::inca::com
{
  /**
   * @brief Proxy for an INCA experiment device COM object.
   *
   * `ExperimentDeviceProxy` wraps the `ExperimentDevice_Dispatch` COM
   * interface, which represents a single ECU device configured in the open
   * INCA experiment. It allows the caller to identify the device by name and
   * to obtain a non-owning raw pointer that other INCA COM methods accept as
   * a device argument.
   *
   * Instances are normally obtained by iterating the vector returned by
   * `IncaOnlineExperimentProxy::GetAllDevices()` and constructing a proxy
   * from each element.
   *
   * The class is move-only.
   *
   * @see pacemaker::inca::com::IncaOnlineExperimentProxy::GetAllDevices()
   * @see pacemaker::inca::com::IncaOnlineExperimentProxy::GetCalibrationValueInDevice()
   * @see pacemaker::inca::detail::COMProxy
   */
  class ExperimentDeviceProxy : private pacemaker::inca::detail::COMProxy<::ExperimentDevice_Dispatch>
  {
  public:
    /**
     * @brief Constrcuts the proxy by narrowing a generic `IDispatch` to `ExperimentDevice_Dispatch`
     *
     * @param idispatch Owning pointer to a generic `IDispatch` representing
     *                  the device object. Ownership is transferred to the proxy.
     *
     * @throws std::invalid_argument if @p idispatch is null
     * @throws std::runtime_error    if `QueryInterface` fails to obtain `ExperimentDevice_Dispatch`.
     */
    explicit ExperimentDeviceProxy(pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch);
    /// @brief Move contructor transfers ownership of the COM object.
    ExperimentDeviceProxy(ExperimentDeviceProxy &&) = default;
    /// @brief Move assignment transfers ownership of the COM object.
    ExperimentDeviceProxy &operator=(ExperimentDeviceProxy &&) = default;
    /// @brief Destructor; releases the underlying COM object.
    ~ExperimentDeviceProxy();

    /**
     * @brief Returns a non-owning raw pointer to the underlying COM device object.
     *
     * The returned pointer is valid for as long as the proxy is alive. It is
     * intended to be passed directly to COM methods that accept an
     * `ExperimentDevice_Dispatch*` argument, such as
     * `IncaOnlineExperimentProxy::GetCalibrationValueInDevice()`.
     *
     * @return Non-owning pointer to the `ExperimentDevice_Dispatch` object.
     * Never null after successful construction.
     */
    auto get() const noexcept -> ::ExperimentDevice_Dispatch * { return this->p_subject.get(); }

    /**
     * @brief Returns the name of the device as configured in INCA.
     *
     * Retrieves the device name via `ExperimentDevice_Dispatch::GetName()`,
     * which returns a `BSTR`. The result is converted to a `std::wstring`
     * before being returned to the caller.
     *
     * @return Wide string containing the device name (e.g. `L"XCP:1"`)
     */
    [[nodiscard]] auto GetName() -> std::wstring;
  };
} // namespace pacemaker::inca::com

#endif // PACEMAKER_INCA_COM_EXPERIMENTDEVICEPROXY_HPP_