#include "inca/com/ExperimentDeviceProxy.hpp"

#include "incacom.hpp"
#include "utils.hpp"

inca::com::ExperimentDeviceProxy::~ExperimentDeviceProxy() {}

inca::com::ExperimentDeviceProxy::ExperimentDeviceProxy(
    inca::unique_com_ptr<::IDispatch> idispatch)
{
  this->p_subject = query_interface<ExperimentDevice_Dispatch>(std::move(idispatch));
}