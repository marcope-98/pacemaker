#include "pacemaker/inca/com/IncaProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

pacemaker::inca::com::IncaProxy::IncaProxy(pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{pacemaker::inca::detail::query_interface<::Inca_Dispatch>(std::move(idispatch))}

{
}

pacemaker::inca::com::IncaProxy::~IncaProxy()
{
  if (this->p_subject) this->p_subject->DisconnectFromTool();
}

auto pacemaker::inca::com::IncaProxy::GetOpenedExperiment() -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>
{
  return pacemaker::inca::detail::unique_com_ptr<::IDispatch>(
      this->p_subject->GetOpenedExperiment().Detach());
}

auto pacemaker::inca::com::IncaProxy::GetOpenedExperimentView() -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>
{
  return pacemaker::inca::detail::unique_com_ptr<::IDispatch>(
      this->p_subject->GetOpenedExperimentView().Detach());
}