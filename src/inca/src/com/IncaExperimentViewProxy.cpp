#include "inca/com/IncaExperimentViewProxy.hpp"

#include "incacom.hpp"
#include "utils.hpp"

inca::com::IncaExperimentViewProxy::IncaExperimentViewProxy(
    ::IDispatch *const idispatch)
{
  this->p_subject = query_interface<::IncaExperimentView_Dispatch>(idispatch);
}

auto inca::com::IncaExperimentViewProxy::OpenViewForExperimentDataItem(
    ::IDispatch *const dataitem) -> void
{
  this->p_subject->OpenViewForExperimentDataItem(dataitem);
}