#include "inca/IncaCOMAdapter.hpp"

#include <chrono>
#include <stdexcept>
#include <thread>

#include "utils.hpp"

#import "incacom.tlb" named_guids no_namespace

inca::IncaCOMAdapter::IncaCOMAdapter()
{
  {
    Inca_Dispatch *raw{nullptr};
    HRESULT        hr = CoCreateInstance(CLSID_Inca,
                                         nullptr,
                                         CLSCTX_INPROC_SERVER,
                                         __uuidof(Inca_Dispatch),
                                         reinterpret_cast<void **>(&raw));
    if (FAILED(hr)) throw std::runtime_error("Failed to instantiate INCA COM object. Verify that INCA is installed.");
    this->p_inca = unique_com_ptr<Inca_Dispatch>(raw);
  }
  this->p_exp = query_interface<IncaOnlineExperiment_Dispatch>(this->p_inca->GetOpenedExperiment());
  this->p_exp->StopMeasurement();

  this->p_expview = query_interface<IncaExperimentView_Dispatch>(this->p_inca->GetOpenedExperimentView());

  _variant_t device_list = this->p_exp->GetAllDevices();
  if (device_list.vt != (VT_ARRAY | VT_VARIANT)) throw std::runtime_error("GetAllDevices returned an unexpected type.");

  SAFEARRAY *psa = V_ARRAY(&device_list);
  long       lLower{}, lUpper{};
  SafeArrayGetLBound(psa, 1, &lLower);
  SafeArrayGetUBound(psa, 1, &lUpper);

  if (lLower < 0 || lUpper < lLower) throw std::runtime_error("Device list has invalid bounds or is empty");

  _variant_t device;
  SafeArrayGetElement(psa, &lLower, &device);
  if (device.vt != VT_DISPATCH) throw std::runtime_error("First device in the device list is not of IDispatch* type.");
  query_interface<ExperimentDevice_Dispatch>(V_DISPATCH(&device));
}

inca::IncaCOMAdapter::~IncaCOMAdapter()
{
  if (this->p_inca) this->p_inca->DisconnectFromTool();
}

auto inca::IncaCOMAdapter::add_param(const std::string& name) -> void
{
  if (this->m_calibrations_map.count(name) > 0) return;

  this->m_calibrations_map[name] = this->m_calibrations_vector.size();
  auto calib                     = query_interface<CalibrationScalarData_Dispatch>(
      this->p_exp->GetCalibrationValueInDevice(
          name.c_str(),
          this->p_device.get()));
  this->p_expview->OpenViewForExperimentDataItem(calib.get());
  this->m_calibrations_vector.emplace_back(std::move(calib));
}

auto inca::IncaCOMAdapter::set_param(std::string_view name, double value) -> void
{
  throw std::logic_error("Not implemented");
}

auto inca::IncaCOMAdapter::reset() -> void
{
  throw std::logic_error("Not implemented");
}

auto inca::IncaCOMAdapter::start_recording() -> void
{
  this->p_exp->StartRecording();
}

auto inca::IncaCOMAdapter::stop_recording(std::string_view filename) -> void
{
  (void)(filename);
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(40ms);
  this->p_exp->StopRecordingAndSave();
  this->p_exp->StopMeasurement();
  this->reset();
}