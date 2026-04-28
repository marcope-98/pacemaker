#ifndef INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_
#define INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_

#include "inca/traits/COMProxy.hpp"
#include "inca/unique_com_ptr.hpp"


struct IncaExperimentView_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaExperimentViewProxy : private inca::traits::COMProxy<::IncaExperimentView_Dispatch>
  {
  public:
    explicit IncaExperimentViewProxy(inca::unique_com_ptr<::IDispatch> idispatch);

    IncaExperimentViewProxy(IncaExperimentViewProxy &&)            = default;
    IncaExperimentViewProxy &operator=(IncaExperimentViewProxy &&) = default;

    ~IncaExperimentViewProxy();

    auto OpenViewForExperimentDataItem(inca::unique_com_ptr<::IDispatch> dataitem) -> void;
  };
} // namespace inca::com

#endif // INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_