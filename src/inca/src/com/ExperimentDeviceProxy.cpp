#include "inca/com/ExperimentDeviceProxy.hpp"

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

inca::com::ExperimentDeviceProxy::~ExperimentDeviceProxy() {}

inca::com::ExperimentDeviceProxy::ExperimentDeviceProxy(
    inca::detail::unique_com_ptr<::IDispatch> idispatch)
{
  this->p_subject = inca::detail::query_interface<ExperimentDevice_Dispatch>(std::move(idispatch));
}