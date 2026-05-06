#include "pacemaker/inca/com/ExperimentDeviceProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

pacemaker::inca::com::ExperimentDeviceProxy::~ExperimentDeviceProxy() {}

pacemaker::inca::com::ExperimentDeviceProxy::ExperimentDeviceProxy(
    pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{pacemaker::inca::detail::query_interface<::ExperimentDevice_Dispatch>(std::move(idispatch))}
{
}

auto pacemaker::inca::com::ExperimentDeviceProxy::GetName() -> std::wstring
{
  _bstr_t bs = this->p_subject->GetName();
  return std::wstring(bs, SysStringLen(bs));
}