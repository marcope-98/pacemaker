#include "inca/CalibrationSet.hpp"

#include <stdexcept>

inca::CalibrationSet::CalibrationSet(
    inca::com::IncaOnlineExperimentProxy &exp,
    inca::com::ExperimentDeviceProxy     &device) noexcept
    : m_exp{exp},
      m_device{device}
{
}

auto inca::CalibrationSet::add_param(const std::string &name) -> void
{
  if (this->m_values.count(name)) return;
  inca::detail::unique_com_ptr<::IDispatch> raw = this->m_exp.GetCalibrationValueInDevice(name, this->m_device.get());
  this->m_values.emplace(name, std::move(raw));
}

auto inca::CalibrationSet::set_param(const std::string &name, double value) -> void
{
  auto it = this->m_values.find(name);
  if (it == this->m_values.end())
    throw std::out_of_range("CalirbationSet::set_param: unknown parameter '" + name + "'. Call add_param() before set_param().");
  it->second.SetImplValue(value);
}

auto inca::CalibrationSet::reset() -> void
{
  for (auto &[name, proxy] : this->m_values)
    proxy.ResetValueToRP();
}
