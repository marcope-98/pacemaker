#include "inca/com/CalibrationScalarDataProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

inca::com::CalibrationScalarDataProxy::~CalibrationScalarDataProxy() {}

inca::com::CalibrationScalarDataProxy::CalibrationScalarDataProxy(
    inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{inca::detail::query_interface<::CalibrationScalarData_Dispatch>(std::move(idispatch))}
{
}

auto inca::com::CalibrationScalarDataProxy::SetImplValue(double value) -> void
{
  this->p_subject->SetImplValue(value);
}

auto inca::com::CalibrationScalarDataProxy::ResetValueToRP() -> void
{
  this->p_subject->ResetValueToRP();
}