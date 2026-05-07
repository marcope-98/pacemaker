#include "pacemaker/inca/com/IncaOnlineExperimentProxy.hpp"

#include <stdexcept>

#include "pacemaker/inca/detail/incacom.hpp"
#include "pacemaker/inca/detail/utils.hpp"

pacemaker::inca::com::IncaOnlineExperimentProxy::~IncaOnlineExperimentProxy() {}

pacemaker::inca::com::IncaOnlineExperimentProxy::IncaOnlineExperimentProxy(
    pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{pacemaker::inca::detail::query_interface<::IncaOnlineExperiment_Dispatch>(std::move(idispatch))}
{
}

auto pacemaker::inca::com::IncaOnlineExperimentProxy::StopMeasurement() -> void
{
  this->p_subject->StopMeasurement();
}

auto pacemaker::inca::com::IncaOnlineExperimentProxy::GetAllDevices() -> std::vector<pacemaker::inca::detail::unique_com_ptr<::IDispatch>>
{
  _variant_t device_list = this->p_subject->GetAllDevices();
  if (device_list.vt != (VT_ARRAY | VT_VARIANT))
    throw std::runtime_error("GetAllDevices returned an unexpected type.");

  SAFEARRAY *psa = V_ARRAY(&device_list);
  long       lLower{}, lUpper{};
  SafeArrayGetLBound(psa, 1, &lLower);
  SafeArrayGetUBound(psa, 1, &lUpper);

  std::vector<pacemaker::inca::detail::unique_com_ptr<::IDispatch>> out{};
  out.reserve(static_cast<std::size_t>(lUpper - lLower + 1));
  for (long i{lLower}; i <= lUpper; ++i)
  {
    _variant_t device;
    if (FAILED(SafeArrayGetElement(psa, &i, &device)))
      throw std::runtime_error("GetAllDevices: SafeArrayGetElement failed");

    if (device.vt != VT_DISPATCH)
      throw std::runtime_error("GetAllDevices: device element is not VT_DISPATCH");

#if 1
    auto idispatch = (IDispatch *)(device);
#else
    ::IDispatch *idispatch = V_DISPATCH(&device);
    idispatch->AddRef();
#endif
    out.emplace_back(std::move(idispatch));
  }
  return out;
}

auto pacemaker::inca::com::IncaOnlineExperimentProxy::StartRecording() -> void
{
  this->p_subject->StartRecording();
}

auto pacemaker::inca::com::IncaOnlineExperimentProxy::StopRecordingAndSave() -> void
{
  this->p_subject->StopRecordingAndSave();
}

auto pacemaker::inca::com::IncaOnlineExperimentProxy::GetCalibrationValueInDevice(
    const std::string                 &name,
    ::ExperimentDevice_Dispatch *const device) -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>
{
  ::IDispatch *raw = this->p_subject->GetCalibrationValueInDevice(name.c_str(), device).Detach();
  if (raw == nullptr)
    throw std::runtime_error("GetCalibrationValueInDevice: parameter not found " + name);
  return pacemaker::inca::detail::unique_com_ptr<::IDispatch>(raw);
}