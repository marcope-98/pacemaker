#ifndef INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_
#define INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_

#include "inca/unique_com_ptr.hpp"

struct IncaExperimentView_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaExperimentViewProxy
  {
  public:
    IncaExperimentViewProxy() = default;
    explicit IncaExperimentViewProxy(::IDispatch *const idispatch);
    ~IncaExperimentViewProxy()                                          = default;
    IncaExperimentViewProxy(const IncaExperimentViewProxy &)            = delete;
    IncaExperimentViewProxy(IncaExperimentViewProxy &&)                 = delete;
    IncaExperimentViewProxy &operator=(const IncaExperimentViewProxy &) = delete;
    IncaExperimentViewProxy &operator=(IncaExperimentViewProxy &&)      = delete;

    auto OpenViewForExperimentDataItem(::IDispatch *const dataitem) -> void;

  private:
    inca::unique_com_ptr<::IncaExperimentView_Dispatch> p_subject{nullptr};
  };
} // namespace inca::com

#endif // INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_