#ifndef INCA_COM_INCAPROXY_HPP_
#define INCA_COM_INCAPROXY_HPP_

#include "inca/unique_com_ptr.hpp"

struct Inca_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaProxy
  {
  public:
    IncaProxy();
    ~IncaProxy();
    IncaProxy(const IncaProxy &)                = delete;
    IncaProxy(IncaProxy &&) noexcept            = delete;
    IncaProxy &operator=(const IncaProxy &)     = delete;
    IncaProxy &operator=(IncaProxy &&) noexcept = delete;

    auto GetOpenedExperiment() -> ::IDispatch *;
    auto GetOpenedExperimentView() -> ::IDispatch *;

  private:
    inca::unique_com_ptr<::Inca_Dispatch> p_subject{nullptr};
  };
} // namespace inca::com
#endif // INCA_COM_INCAPROXY_HPP_