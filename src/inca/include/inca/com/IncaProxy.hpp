#ifndef INCA_COM_INCAPROXY_HPP_
#define INCA_COM_INCAPROXY_HPP_

#include "inca/unique_com_ptr.hpp"
#include "inca/traits/COMProxy.hpp"

struct Inca_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaProxy : private inca::traits::COMProxy<::Inca_Dispatch>
  {
  public:
    IncaProxy();
    IncaProxy(inca::unique_com_ptr<::IDispatch> idispatch);
    ~IncaProxy();

    auto GetOpenedExperiment() -> inca::unique_com_ptr<::IDispatch>;
    auto GetOpenedExperimentView() -> inca::unique_com_ptr<::IDispatch>;
  };
} // namespace inca::com
#endif // INCA_COM_INCAPROXY_HPP_