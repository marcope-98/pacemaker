#ifndef INCA_INCACOMADAPTER_HPP_
#define INCA_INCACOMADAPTER_HPP_

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>


#include "inca/unique_com_ptr.hpp"

struct Inca_Dispatch;
struct IncaOnlineExperiment_Dispatch;
struct IncaExperimentView_Dispatch;
struct ExperimentDevice_Dispatch;
struct CalibrationScalarData_Dispatch;

namespace inca
{
  class IncaCOMAdapter
  {
  public:
    IncaCOMAdapter();
    ~IncaCOMAdapter();
    IncaCOMAdapter(const IncaCOMAdapter &)            = delete;
    IncaCOMAdapter(IncaCOMAdapter &&)                 = delete;
    IncaCOMAdapter &operator=(const IncaCOMAdapter &) = delete;
    IncaCOMAdapter &operator=(IncaCOMAdapter &&)      = delete;

    auto set_param(std::string_view name, double value) -> void;
    auto add_param(const std::string& name) -> void;
    auto reset() -> void;
    auto start_recording() -> void;
    auto stop_recording(std::string_view filename) -> void;

  private:
    unique_com_ptr<Inca_Dispatch>                 p_inca;
    unique_com_ptr<IncaOnlineExperiment_Dispatch> p_exp;
    unique_com_ptr<IncaExperimentView_Dispatch>   p_expview;
    unique_com_ptr<ExperimentDevice_Dispatch>     p_device;

    std::unordered_map<std::string, std::size_t>               m_calibrations_map;
    std::vector<unique_com_ptr<CalibrationScalarData_Dispatch>> m_calibrations_vector;
  };
} // namespace inca

#endif // INCA_INCACOMADAPTER_HPP_