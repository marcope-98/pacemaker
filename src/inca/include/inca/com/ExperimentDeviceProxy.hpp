#ifndef INCA_COM_EXPERIMENTDEVICEPROXY_HPP_
#define INCA_COM_EXPERIMENTDEVICEPROXY_HPP_

#include "inca/traits/COMProxy.hpp"
#include "inca/unique_com_ptr.hpp"

struct IDispatch;
struct ExperimentDevice_Dispatch;

namespace inca::com
{
  class ExperimentDeviceProxy : private inca::traits::COMProxy<::ExperimentDevice_Dispatch>
  {
  public:
    ExperimentDeviceProxy() = default;
    explicit ExperimentDeviceProxy(::IDispatch *const idispatch);
  };
} // namespace inca::com

#endif // INCA_COM_EXPERIMENTDEVICEPROXY_HPP_