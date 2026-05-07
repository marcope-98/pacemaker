#include "pacemaker/inca/com/CalibrationScalarDataProxy.hpp"

#include "pacemaker/inca/detail/incacom.hpp"
#include "pacemaker/inca/detail/utils.hpp"

pacemaker::inca::com::CalibrationScalarDataProxy::~CalibrationScalarDataProxy() {}

pacemaker::inca::com::CalibrationScalarDataProxy::CalibrationScalarDataProxy(
    pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{pacemaker::inca::detail::query_interface<::CalibrationScalarData_Dispatch>(std::move(idispatch))}
{
}

auto pacemaker::inca::com::CalibrationScalarDataProxy::SetImplValue(double value) -> void
{
  this->p_subject->SetImplValue(value);
}

auto pacemaker::inca::com::CalibrationScalarDataProxy::ResetValueToRP() -> void
{
  this->p_subject->ResetValueToRP();
}