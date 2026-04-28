#ifndef INCA_SESSION_HPP_
#define INCA_SESSION_HPP_

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

  private:
    explicit Session(inca::com::IncaProxy inca);

    inca::com::IncaProxy m_inca;
  };
} // namespace inca

#endif // INCA_SESSION_HPP_