#include "inca/Experiment.hpp"
#include <thread>

inca::Experiment::Experiment(
    inca::com::IncaOnlineExperimentProxy exp,
    inca::com::IncaExperimentViewProxy   expview,
    inca::com::ExperimentDeviceProxy     device)
    : m_exp{std::move(exp)},
      m_expview{std::move(expview)},
      m_device{std::move(device)},
      m_calibrations{m_exp, m_device}
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
}