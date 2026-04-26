#ifndef INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_
#define INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_

#include <string>
#include <vector>


#include "inca/unique_com_ptr.hpp"

struct IncaOnlineExperiment_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaOnlineExperimentProxy
  {
  public:
    IncaOnlineExperimentProxy()  = default;
    ~IncaOnlineExperimentProxy() = default;
    explicit IncaOnlineExperimentProxy(::IDispatch *const idispatch);
    IncaOnlineExperimentProxy(const IncaOnlineExperimentProxy &)            = delete;
    IncaOnlineExperimentProxy(IncaOnlineExperimentProxy &&)                 = delete;
    IncaOnlineExperimentProxy &operator=(const IncaOnlineExperimentProxy &) = delete;
    IncaOnlineExperimentProxy &operator=(IncaOnlineExperimentProxy &&)      = delete;

    auto GetAllDevices() -> std::vector<inca::unique_com_ptr<::IDispatch>>;
    auto StopMeasurement() -> void;
    auto StartRecording() -> void;
    auto StopRecordingAndSave() -> void;
    auto GetCalibrationValueInDevice(const std::string &name, ::IDispatch *const device) -> ::IDispatch *;

  private:
    inca::unique_com_ptr<::IncaOnlineExperiment_Dispatch> p_subject{nullptr};
  };

} // namespace inca::com
#endif // INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_