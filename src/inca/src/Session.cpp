#include "inca/Session.hpp"

#include <stdexcept>

// clang-format off
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>
// clang-format on

#include "inca/com/ExperimentDeviceProxy.hpp"
#include "inca/com/IncaExperimentViewProxy.hpp"
#include "inca/com/IncaOnlineExperimentProxy.hpp"
#include "inca/com/IncaProxy.hpp"
#include "inca/detail/unique_com_ptr.hpp"


#include "detail/incacom.hpp"

namespace
{
  [[nodiscard]] auto create_inca_idispatch() -> inca::detail::unique_com_ptr<::IDispatch>
  {
    ::IDispatch *raw{nullptr};
    HRESULT      hr = CoCreateInstance(CLSID_Inca,
                                       nullptr,
                                       CLSCTX_LOCAL_SERVER,
                                       IID_IDispatch,
                                       reinterpret_cast<void **>(&raw));
    if (FAILED(hr))
      throw std::runtime_error("CoCreateInstance failed: Verify that INCA is installed.");

    return inca::detail::unique_com_ptr<::IDispatch>{raw};
  }
} // namespace

auto inca::Session::connect() -> Session
{
  inca::com::IncaProxy                 inca{create_inca_idispatch()};
  inca::com::IncaOnlineExperimentProxy exp{inca.GetOpenedExperiment()};
  inca::com::IncaExperimentViewProxy   expview{inca.GetOpenedExperimentView()};

  auto devices = exp.GetAllDevices();
  if (devices.empty())
    throw std::runtime_error(
        "The open INCA experiment has no devices. "
        "Verify that a device is configured and online.");
  inca::com::ExperimentDeviceProxy device{std::move(devices[0])};

  return Session{std::move(inca)};
}

inca::Session::Session(inca::com::IncaProxy inca)
    : m_inca{std::move(inca)}
{
}

inca::Session::~Session() {}