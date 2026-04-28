#ifndef INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_
#define INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_

#include "inca/detail/COMProxy.hpp"
#include "inca/detail/unique_com_ptr.hpp"

struct CalibrationScalarData_Dispatch;
struct IDispatch;

namespace inca::com
{
  class CalibrationScalarDataProxy : private inca::detail::COMProxy<::CalibrationScalarData_Dispatch>
  {
  public:
    explicit CalibrationScalarDataProxy(inca::detail::unique_com_ptr<::IDispatch> idispatch);

    CalibrationScalarDataProxy(CalibrationScalarDataProxy &&)            = default;
    CalibrationScalarDataProxy &operator=(CalibrationScalarDataProxy &&) = default;
    
    ~CalibrationScalarDataProxy();

    auto SetImplValue(double value) -> void;
    auto ResetValueToRP() -> void;
  };
} // namespace inca::com

#endif // INCA_COM_CALIBRATIONSCALARDATAPROXY_HPP_