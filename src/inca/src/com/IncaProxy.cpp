#include "inca/com/IncaProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

inca::com::IncaProxy::IncaProxy(inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{inca::detail::query_interface<::Inca_Dispatch>(std::move(idispatch))}

{
}

inca::com::IncaProxy::~IncaProxy()
{
  if (this->p_subject) this->p_subject->DisconnectFromTool();
}

auto inca::com::IncaProxy::GetOpenedExperiment() -> inca::detail::unique_com_ptr<::IDispatch>
{
  return inca::detail::unique_com_ptr<::IDispatch>(
      this->p_subject->GetOpenedExperiment().Detach());
}

auto inca::com::IncaProxy::GetOpenedExperimentView() -> inca::detail::unique_com_ptr<::IDispatch>
{
  return inca::detail::unique_com_ptr<::IDispatch>(
      this->p_subject->GetOpenedExperimentView().Detach());
}