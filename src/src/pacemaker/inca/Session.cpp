#include "pacemaker/inca/Session.hpp"

#include <stdexcept>

// clang-format off
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>
// clang-format on

#include "pacemaker/inca/com/ExperimentDeviceProxy.hpp"
#include "pacemaker/inca/com/IncaExperimentViewProxy.hpp"
#include "pacemaker/inca/com/IncaOnlineExperimentProxy.hpp"
#include "pacemaker/inca/com/IncaProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

#include "pacemaker/inca/detail/incacom.hpp"

namespace
{
  [[nodiscard]] auto create_inca_idispatch() -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>
  {
    ::IDispatch *raw{nullptr};
    HRESULT      hr = CoCreateInstance(CLSID_Inca,
                                       nullptr,
                                       CLSCTX_LOCAL_SERVER,
                                       IID_IDispatch,
                                       reinterpret_cast<void **>(&raw));
    if (FAILED(hr))
      throw std::runtime_error("CoCreateInstance failed: Verify that INCA is installed.");

    return pacemaker::inca::detail::unique_com_ptr<::IDispatch>{raw};
  }
} // namespace

auto pacemaker::inca::Session::connect() -> Session
{
  pacemaker::inca::com::IncaProxy                 inca{create_inca_idispatch()};
  pacemaker::inca::com::IncaOnlineExperimentProxy exp{inca.GetOpenedExperiment()};
  pacemaker::inca::com::IncaExperimentViewProxy   expview{inca.GetOpenedExperimentView()};

  auto devices = exp.GetAllDevices();
  if (devices.empty())
    throw std::runtime_error(
        "The open INCA experiment has no devices. "
        "Verify that a device is configured and online.");
  pacemaker::inca::com::ExperimentDeviceProxy device{std::move(devices[0])};

  pacemaker::inca::Experiment experiment{
      std::move(exp),
      std::move(expview),
      std::move(device)};

  return Session{std::move(inca), std::move(experiment)};
}

pacemaker::inca::Session::Session(pacemaker::inca::com::IncaProxy inca, pacemaker::inca::Experiment experiment)
    : m_inca{std::move(inca)},
      m_experiment{std::move(experiment)}
{
}

pacemaker::inca::Session::~Session() {}