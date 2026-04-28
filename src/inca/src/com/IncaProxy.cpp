#include "inca/com/IncaProxy.hpp"

#include <stdexcept>
#include <iostream>

#include "incacom.hpp"
#include "utils.hpp"

inca::com::IncaProxy::IncaProxy(inca::unique_com_ptr<::IDispatch> idispatch)
{
  this->p_subject = query_interface<::Inca_Dispatch>(std::move(idispatch));
}

inca::com::IncaProxy::~IncaProxy()
{
  this->p_subject->DisconnectFromTool();
}

auto inca::com::IncaProxy::GetOpenedExperiment() -> inca::unique_com_ptr<::IDispatch>
{
  return inca::unique_com_ptr<::IDispatch>(this->p_subject->GetOpenedExperiment().Detach());
}

auto inca::com::IncaProxy::GetOpenedExperimentView() -> inca::unique_com_ptr<::IDispatch>
{
  return inca::unique_com_ptr<::IDispatch>(this->p_subject->GetOpenedExperimentView().Detach());
}