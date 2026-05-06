#include "pacemaker/inca/com/IncaExperimentViewProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

pacemaker::inca::com::IncaExperimentViewProxy::~IncaExperimentViewProxy() {}

pacemaker::inca::com::IncaExperimentViewProxy::IncaExperimentViewProxy(
    pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{pacemaker::inca::detail::query_interface<::IncaExperimentView_Dispatch>(std::move(idispatch))}
{
}

auto pacemaker::inca::com::IncaExperimentViewProxy::OpenViewForExperimentDataItem(
    pacemaker::inca::detail::unique_com_ptr<::IDispatch> dataitem) -> void
{
  this->p_subject->OpenViewForExperimentDataItem(dataitem.get());
}