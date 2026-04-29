#include "inca/Experiment.hpp"
#include <thread>

inca::Experiment::Experiment(
    inca::com::IncaOnlineExperimentProxy exp,
    inca::com::IncaExperimentViewProxy   expview,
    inca::com::ExperimentDeviceProxy     device)
    : m_exp{std::move(exp)},
      m_expview{std::move(expview)},
      m_device{std::move(device)}
{
}

auto inca::Experiment::start_recording() -> void
{
  this->m_exp.StartRecording();
}

auto inca::Experiment::stop_recording(const std::string &filename) -> void
{
  (void)(filename);
  std::this_thread::sleep_for(this->k_flush_delay);
  this->m_exp.StopRecordingAndSave();
  this->m_exp.StopMeasurement();
  this->reset();
}

auto inca::Experiment::add_param(const std::string &name) -> void
{
  if (this->m_map.count(name) > 0) return;
  inca::detail::unique_com_ptr<::IDispatch> raw = this->m_exp.GetCalibrationValueInDevice(name, this->m_device.get());
  this->m_map[name]                             = this->m_values.size();
  this->m_values.emplace_back(std::move(raw));
}

auto inca::Experiment::set_param(const std::string &name, double value) -> void
{
  if (this->m_map.find(name) == this->m_map.end())
    throw std::out_of_range("Experiment::set_param: unknown parameter '" + name + "'. Call add_param() before set_param().");
  this->m_values[this->m_map[name]].SetImplValue(value);
}

auto inca::Experiment::reset() -> void
{
  for (auto &proxy : this->m_values)
    proxy.ResetValueToRP();
  this->m_values.clear();
  this->m_map.clear();
}