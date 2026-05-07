#include "pacemaker/inca/Experiment.hpp"
#include <thread>

pacemaker::inca::Experiment::Experiment(
    pacemaker::inca::com::IncaOnlineExperimentProxy exp,
    pacemaker::inca::com::IncaExperimentViewProxy   expview,
    pacemaker::inca::com::ExperimentDeviceProxy     device)
    : m_exp{std::move(exp)},
      m_expview{std::move(expview)},
      m_device{std::move(device)}
{
}

auto pacemaker::inca::Experiment::start_recording() -> void
{
  this->m_exp.StartRecording();
}

auto pacemaker::inca::Experiment::stop_recording(const std::string &filename) -> void
{
  (void)(filename);
  std::this_thread::sleep_for(this->k_flush_delay);
  this->m_exp.StopRecordingAndSave();
  this->m_exp.StopMeasurement();
  this->reset();
}

auto pacemaker::inca::Experiment::add_param(const std::string &name) -> void
{
  if (this->m_map.count(name) > 0) return;
  pacemaker::inca::detail::unique_com_ptr<::IDispatch> raw = this->m_exp.GetCalibrationValueInDevice(name, this->m_device.get());
  this->m_map[name]                             = this->m_values.size();
  this->m_values.emplace_back(std::move(raw));
}

auto pacemaker::inca::Experiment::set_param(const std::string &name, double value) -> void
{
  if (this->m_map.find(name) == this->m_map.end())
    throw std::out_of_range("Experiment::set_param: unknown parameter '" + name + "'. Call add_param() before set_param().");
  this->m_values[this->m_map[name]].SetImplValue(value);
}

auto pacemaker::inca::Experiment::reset() -> void
{
  for (auto &proxy : this->m_values)
    proxy.ResetValueToRP();
  this->m_values.clear();
  this->m_map.clear();
}