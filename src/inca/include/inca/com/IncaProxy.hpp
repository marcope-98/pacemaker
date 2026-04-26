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
    ~IncaProxy();

    auto GetOpenedExperiment() -> ::IDispatch *;
    auto GetOpenedExperimentView() -> ::IDispatch *;
  };
} // namespace inca::com
#endif // INCA_COM_INCAPROXY_HPP_