#ifndef INCA_INCACOMADAPTER_HPP_
#define INCA_INCACOMADAPTER_HPP_

#include <string_view>

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

    auto add_param(std::string_view name) -> void;
    auto set_param(std::string_view name, double value) -> void;
    auto reset() -> void;
    auto start_recording() -> void;
    auto stop_recording(std::string_view filename) -> void;
  };
} // namespace inca

#endif // INCA_INCACOMADAPTER_HPP_