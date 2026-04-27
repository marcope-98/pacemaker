#include "inca/com/IncaExperimentViewProxy.hpp"

#include "incacom.hpp"
#include "utils.hpp"

inca::com::IncaExperimentViewProxy::~IncaExperimentViewProxy() {}

inca::com::IncaExperimentViewProxy::IncaExperimentViewProxy(
    inca::unique_com_ptr<::IDispatch> idispatch)
{
  this->p_subject = query_interface<::IncaExperimentView_Dispatch>(std::move(idispatch));
}

auto inca::com::IncaExperimentViewProxy::OpenViewForExperimentDataItem(
    inca::unique_com_ptr<::IDispatch> dataitem) -> void
{
  this->p_subject->OpenViewForExperimentDataItem(dataitem.get());
}