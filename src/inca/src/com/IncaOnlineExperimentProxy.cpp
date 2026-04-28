#include "inca/com/IncaOnlineExperimentProxy.hpp"

#include <stdexcept>

#include "detail/incacom.hpp"
#include "detail/utils.hpp"

inca::com::IncaOnlineExperimentProxy::~IncaOnlineExperimentProxy() {}

inca::com::IncaOnlineExperimentProxy::IncaOnlineExperimentProxy(
    inca::detail::unique_com_ptr<::IDispatch> idispatch)
    : COMProxy{inca::detail::query_interface<::IncaOnlineExperiment_Dispatch>(std::move(idispatch))}
{
}

auto inca::com::IncaOnlineExperimentProxy::StopMeasurement() -> void
{
  this->p_subject->StopMeasurement();
}

auto inca::com::IncaOnlineExperimentProxy::GetAllDevices() -> std::vector<inca::detail::unique_com_ptr<::IDispatch>>
{
  _variant_t device_list = this->p_subject->GetAllDevices();
  if (device_list.vt != (VT_ARRAY | VT_VARIANT))
    throw std::runtime_error("GetAllDevices returned an unexpected type.");

  SAFEARRAY *psa = V_ARRAY(&device_list);
  long       lLower{}, lUpper{};
  SafeArrayGetLBound(psa, 1, &lLower);
  SafeArrayGetUBound(psa, 1, &lUpper);

  std::vector<inca::detail::unique_com_ptr<::IDispatch>> out{};
  out.reserve(static_cast<std::size_t>(lUpper - lLower + 1));
  for (long i{lLower}; i <= lUpper; ++i)
  {
    _variant_t device;
    if (FAILED(SafeArrayGetElement(psa, &i, &device)))
      throw std::runtime_error("GetAllDevices: SafeArrayGetElement failed");

    if (device.vt != VT_DISPATCH)
      throw std::runtime_error("GetAllDevices: device element is not VT_DISPATCH");

    ::IDispatch *idispatch = V_DISPATCH(&device);
    idispatch->AddRef();
    out.emplace_back(idispatch);
  }
  return out;
}

auto inca::com::IncaOnlineExperimentProxy::StartRecording() -> void
{
  this->p_subject->StartRecording();
}

auto inca::com::IncaOnlineExperimentProxy::StopRecordingAndSave() -> void
{
  this->p_subject->StopRecordingAndSave();
}

auto inca::com::IncaOnlineExperimentProxy::GetCalibrationValueInDevice(
    const std::string &name,
    ::IDispatch *const device) -> inca::detail::unique_com_ptr<::IDispatch>
{
  ::IDispatch *raw = this->p_subject->GetCalibrationValueInDevice(name.c_str(), device).Detach();
  if (raw == nullptr)
    throw std::runtime_error("GetCalibrationValueInDevice: parameter not found " + name);
  return inca::detail::unique_com_ptr<::IDispatch>(raw);
}