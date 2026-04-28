#include "inca/com/IncaExperimentViewProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

inca::com::IncaExperimentViewProxy::~IncaExperimentViewProxy() {}

inca::com::IncaExperimentViewProxy::IncaExperimentViewProxy(
    inca::detail::unique_com_ptr<::IDispatch> idispatch)
{
  this->p_subject = inca::detail::query_interface<::IncaExperimentView_Dispatch>(std::move(idispatch));
}

auto inca::com::IncaExperimentViewProxy::OpenViewForExperimentDataItem(
    inca::detail::unique_com_ptr<::IDispatch> dataitem) -> void
{
  this->p_subject->OpenViewForExperimentDataItem(dataitem.get());
}