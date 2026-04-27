#ifndef INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_
#define INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_

#include "inca/unique_com_ptr.hpp"
#include "inca/traits/COMProxy.hpp"

struct IncaExperimentView_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaExperimentViewProxy : private inca::traits::COMProxy<::IncaExperimentView_Dispatch>
  {
  public:
    IncaExperimentViewProxy() = default;
    explicit IncaExperimentViewProxy(inca::unique_com_ptr<::IDispatch> idispatch);
    ~IncaExperimentViewProxy();

    auto OpenViewForExperimentDataItem(inca::unique_com_ptr<::IDispatch> dataitem) -> void;
  };
} // namespace inca::com

#endif // INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_