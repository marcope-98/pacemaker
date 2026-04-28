#ifndef INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_
#define INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_

#include <string>
#include <vector>

#include "inca/traits/COMProxy.hpp"
#include "inca/unique_com_ptr.hpp"

struct IncaOnlineExperiment_Dispatch;
struct IDispatch;

namespace inca::com
{
  class IncaOnlineExperimentProxy : private inca::traits::COMProxy<::IncaOnlineExperiment_Dispatch>
  {
  public:
    explicit IncaOnlineExperimentProxy(inca::unique_com_ptr<::IDispatch> idispatch);

    IncaOnlineExperimentProxy(IncaOnlineExperimentProxy &&)            = default;
    IncaOnlineExperimentProxy &operator=(IncaOnlineExperimentProxy &&) = default;

    ~IncaOnlineExperimentProxy();

    auto GetAllDevices() -> std::vector<inca::unique_com_ptr<::IDispatch>>;
    auto StopMeasurement() -> void;
    auto StartRecording() -> void;
    auto StopRecordingAndSave() -> void;
    auto GetCalibrationValueInDevice(const std::string &name, ::IDispatch *const device) -> ::IDispatch *;
  };

} // namespace inca::com
#endif // INCA_COM_INCAONLINEEXPERIMENTPROXY_HPP_