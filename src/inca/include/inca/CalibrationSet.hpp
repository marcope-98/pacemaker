#ifndef INCA_CALIBRATIONSET_HPP_
#define INCA_CALIBRATIONSET_HPP_

#include <string>
#include <unordered_map>

#include "inca/com/CalibrationScalarDataProxy.hpp"
#include "inca/com/ExperimentDeviceProxy.hpp"
#include "inca/com/IncaOnlineExperimentProxy.hpp"

namespace inca
{
  class CalibrationSet
  {
  public:
    CalibrationSet(inca::com::IncaOnlineExperimentProxy &exp,
                   inca::com::ExperimentDeviceProxy     &device) noexcept;

    auto add_param(const std::string &name) -> void;
    auto set_param(const std::string &name, double value) -> void;
    auto reset() -> void;

  private:
    inca::com::IncaOnlineExperimentProxy                                  &m_exp;
    inca::com::ExperimentDeviceProxy                                      &m_device;
    std::unordered_map<std::string, inca::com::CalibrationScalarDataProxy> m_values;
  };
} // namespace inca

#endif // INCA_CALIBRATIONSET_HPP_