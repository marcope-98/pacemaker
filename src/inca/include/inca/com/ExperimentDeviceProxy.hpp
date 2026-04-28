#ifndef INCA_COM_EXPERIMENTDEVICEPROXY_HPP_
#define INCA_COM_EXPERIMENTDEVICEPROXY_HPP_

#include <string>

#include "inca/detail/COMProxy.hpp"
#include "inca/detail/unique_com_ptr.hpp"

struct IDispatch;
struct ExperimentDevice_Dispatch;

namespace inca::com
{
  class ExperimentDeviceProxy : private inca::detail::COMProxy<::ExperimentDevice_Dispatch>
  {
  public:
    explicit ExperimentDeviceProxy(inca::detail::unique_com_ptr<::IDispatch> idispatch);

    ExperimentDeviceProxy(ExperimentDeviceProxy &&)            = default;
    ExperimentDeviceProxy &operator=(ExperimentDeviceProxy &&) = default;
    ~ExperimentDeviceProxy();

    [[nodiscard]] auto GetName() -> std::wstring;
  };
} // namespace inca::com

#endif // INCA_COM_EXPERIMENTDEVICEPROXY_HPP_