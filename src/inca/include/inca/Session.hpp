#ifndef INCA_SESSION_HPP_
#define INCA_SESSION_HPP_

#include "inca/Experiment.hpp"
#include "inca/com/IncaProxy.hpp"

namespace inca
{
  class Session
  {
  public:
    [[nodiscard]] static auto connect() -> Session;

    Session(const Session &)            = delete;
    Session &operator=(const Session &) = delete;
    Session(Session &&)                 = delete;
    Session &operator=(Session &&)      = delete;

    ~Session();
    [[nodiscard]] auto experiment() noexcept -> Experiment & { return this->m_experiment; }

  private:
    Session(inca::com::IncaProxy inca, inca::Experiment experiment);

    inca::com::IncaProxy m_inca;
    inca::Experiment     m_experiment;
  };
} // namespace inca

#endif // INCA_SESSION_HPP_