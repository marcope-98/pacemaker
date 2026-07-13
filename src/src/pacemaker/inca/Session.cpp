#include "pacemaker/inca/Session.hpp"

#include <thread>
#include <unordered_map>
#include <vector>

#include "pacemaker/inca/com/CalibrationScalarDataProxy.hpp"
#include "pacemaker/inca/com/ExperimentDeviceProxy.hpp"
#include "pacemaker/inca/com/IncaExperimentViewProxy.hpp"
#include "pacemaker/inca/com/IncaOnlineExperimentProxy.hpp"
#include "pacemaker/inca/com/IncaProxy.hpp"

namespace
{
  [[nodiscard]] auto create_inca_idispatch() -> IDispatch *
  {
    ::IDispatch *raw{nullptr};
    HRESULT      hr = CoCreateInstance(CLSID_Inca,
                                       nullptr,
                                       CLSCTX_INPROC_SERVER,
                                       IID_IDispatch,
                                       reinterpret_cast<void **>(&raw));
    if (FAILED(hr))
      throw std::runtime_error("CoCreateInstance failed: Verify that INCA is installed.");

    return raw;
  }
} // namespace

namespace pacemaker::inca
{
  struct Session::Impl
  {
    pacemaker::inca::com::IncaProxy                 inca;
    pacemaker::inca::com::IncaOnlineExperimentProxy exp;
    pacemaker::inca::com::IncaExperimentViewProxy   expview;
    pacemaker::inca::com::ExperimentDeviceProxy     device;

    std::unordered_map<std::string, std::size_t>                  map;
    std::vector<pacemaker::inca::com::CalibrationScalarDataProxy> vector;

    Impl(const std::function<IDispatch *(void)> &factory)
        : inca{factory()},
          exp{this->inca->GetOpenedExperiment()},
          expview{this->inca->GetOpenedExperimentView()}
    {
      auto devices = this->exp->GetAllDevices();
      if (devices.empty())
        throw std::runtime_error("The open INCA experiment has no devices. "
                                 "Verify that a device is configured and online.");
      this->device = std::move(devices[0]);
    }

    ~Impl()
    {
      if (this->inca)
        this->inca->DisconnectFromTool();
    }
  };

  pacemaker::inca::Session::Session(const std::function<IDispatch *(void)> &factory) : pimpl{std::make_unique<Impl>(factory)} {}

  pacemaker::inca::Session::~Session() = default;

  auto pacemaker::inca::Session::connect(const std::function<IDispatch *(void)> &factory) -> Session { return Session{factory}; }

  auto pacemaker::inca::Session::connect() -> Session { return connect(create_inca_idispatch); }

  auto pacemaker::inca::Session::add_param(const std::string &name) -> void
  {
    if (this->pimpl->map.count(name) > 0) return;
    this->pimpl->map[name] = this->pimpl->vector.size();
    this->pimpl->vector.emplace_back(this->pimpl->exp->GetCalibrationValueInDevice(name, this->pimpl->device.get()));
  }

  auto pacemaker::inca::Session::set_param(const std::string &name, double value) -> void
  {
    auto it = this->pimpl->map.find(name);
    if (it == this->pimpl->map.end()) return;
    this->pimpl->vector[it->second]->SetImplValue(value);
  }
  
  auto pacemaker::inca::Session::set_param(const std::size_t &idx, double value) -> void
  {
    this->pimpl->vector.at(idx)->SetImplValue(value);
  }

  auto pacemaker::inca::Session::reset() -> void
  {
    for (auto &proxy : this->pimpl->vector)
      proxy->ResetValueToRP();
  }

  auto pacemaker::inca::Session::start_recording() -> void { this->pimpl->exp->StartRecording(); }

  auto pacemaker::inca::Session::stop_recording(const std::filesystem::path &filename) -> void
  {
    std::this_thread::sleep_for(this->k_flush_delay);
    this->pimpl->exp->StopRecordingAndSaveAs(filename.c_str()); // HACK: fails if filename is invalid and saves the recording in the default Measure folder
    this->pimpl->exp->StopMeasurement();
    this->reset();
  }

} // namespace pacemaker::inca