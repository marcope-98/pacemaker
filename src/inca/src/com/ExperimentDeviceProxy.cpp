#include "inca/com/ExperimentDeviceProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

inca::com::ExperimentDeviceProxy::~ExperimentDeviceProxy() {}

inca::com::ExperimentDeviceProxy::ExperimentDeviceProxy(
    inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{inca::detail::query_interface<::ExperimentDevice_Dispatch>(std::move(idispatch))}
{
}

auto inca::com::ExperimentDeviceProxy::GetName() -> std::wstring
{
  _bstr_t bs = this->p_subject->GetName();
  return std::wstring(bs, SysStringLen(bs));
}