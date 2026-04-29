#ifndef INCA_EXPERIMENT_HPP_
#define INCA_EXPERIMENT_HPP_

#include <chrono>
#include <string>

#include "inca/CalibrationSet.hpp"

#include "inca/com/ExperimentDeviceProxy.hpp"
#include "inca/com/IncaExperimentViewProxy.hpp"
#include "inca/com/IncaOnlineExperimentProxy.hpp"

namespace inca
{
  class Experiment
  {
  public:
    Experiment(inca::com::IncaOnlineExperimentProxy exp,
               inca::com::IncaExperimentViewProxy   expview,
               inca::com::ExperimentDeviceProxy     device);

    Experiment(const Experiment &)            = delete;
    Experiment &operator=(const Experiment &) = delete;
    Experiment(Experiment &&)                 = default;
    Experiment &operator=(Experiment &&)      = default;

    ~Experiment() = default;

    [[nodiscard]] auto calibrations() noexcept -> inca::CalibrationSet & { return this->m_calibrations; }

    auto start_recording() -> void;
    auto stop_recording(const std::string &filename) -> void;

  private:
    inca::com::IncaOnlineExperimentProxy m_exp;
    inca::com::IncaExperimentViewProxy   m_expview;
    inca::com::ExperimentDeviceProxy     m_device;

    inca::CalibrationSet m_calibrations;

    static constexpr std::chrono::milliseconds k_flush_delay{40};
  };
} // namespace inca

#endif // INCA_EXPERIMENT_HPP_