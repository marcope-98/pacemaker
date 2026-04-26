#include "inca/com/ExperimentDeviceProxy.hpp"

#include "incacom.hpp"
#include "utils.hpp"

inca::com::ExperimentDeviceProxy::ExperimentDeviceProxy(
    ::IDispatch *const idispatch)
{
  this->p_subject = query_interface<ExperimentDevice_Dispatch>(idispatch);
}