#ifndef INCA_COM_INCAPROXY_HPP_
#define INCA_COM_INCAPROXY_HPP_

#include "inca/detail/COMProxy.hpp"
#include "inca/detail/unique_com_ptr.hpp"

struct Inca_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaProxy : private inca::detail::COMProxy<::Inca_Dispatch>
  {
  public:
    IncaProxy(inca::detail::unique_com_ptr<::IDispatch> idispatch);

    IncaProxy(const IncaProxy &)            = delete;
    IncaProxy &operator=(const IncaProxy &) = delete;
    IncaProxy(IncaProxy &&)                 = delete;
    IncaProxy &operator=(IncaProxy &&)      = delete;

    ~IncaProxy();

    [[nodiscard]] auto GetOpenedExperiment() -> inca::detail::unique_com_ptr<::IDispatch>;
    [[nodiscard]] auto GetOpenedExperimentView() -> inca::detail::unique_com_ptr<::IDispatch>;
  };
} // namespace inca::com
#endif // INCA_COM_INCAPROXY_HPP_