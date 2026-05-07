#ifndef PACEMAKER_INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_
#define PACEMAKER_INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

struct IncaExperimentView_Dispatch;
struct IDispatch;

namespace pacemaker::inca::com
{
  /**
   * @brief Proxy for the INCA experiment-view COM object.
   *
   * `IncaExperimentViewProxy` wraps the `IncaExperimentView_Dispatch` COM
   * interface, which controls the graphical measurement view displayed in
   * the INCA workbench during an active experiment.
   *
   * The primary use case is opening a view panel for a specific calibration
   * data item so that its value can be observed in real time from the INCA
   * GUI while an automated experiment run is in progress.
   *
   * The class is move-only.
   *
   * @see pacemaker::inca::com::IncaOnlineExperimentProxy::GetCalibrationValueInDevice()
   * @see pacemaker::inca::detail::COMProxy
   */
  class IncaExperimentViewProxy : private pacemaker::inca::detail::COMProxy<::IncaExperimentView_Dispatch>
  {
  public:
    /**
     * @brief Contructs the proxy by narrowing a generic `IDispatch` to `IncaExperimentView_Dispatch`
     *
     * @param idispatch Owning pointer to a generic `IDispatch` representing
     *                  the experiment-view object. Ownership is transferred
     *                  to the proxy.
     *
     * @throws std::invalid_argument if @p idispatch is null.
     * @throws std::runtime_error    if `QueryInterface` fails to obtain `IncaExperimentView_Dispatch`
     */
    explicit IncaExperimentViewProxy(pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch);
    /// @brief Move constructor transfers ownership of the COM object.
    IncaExperimentViewProxy(IncaExperimentViewProxy &&) = default;
    /// @brief Move assignment transfers ownership of the COM object.
    IncaExperimentViewProxy &operator=(IncaExperimentViewProxy &&) = default;
    /// @brief Destructor; releases the underlying COM object.
    ~IncaExperimentViewProxy();

    /**
     * @brief Opens an INCA measurement view for the given calibration data item.
     *
     * Forwards the raw `IDispatch*` held by @p dataitem to the underlying COM
     * method `OpenViewForExperimentDataItem`. The view is displayed inside the
     * INCA workbench UI for the duration of the experiment.
     *
     * Ownership of @p dataitem is consumed by this call; the pointer is moved
     * in but only the raw address is forwarded to COM. INCA does not take
     * permanent ownership of the pointer.
     *
     * @param dataitem Owning `IDispatch` pointer to the calibration data item
     *                 whose view should be opened. Obtain this from
     *                 IncaOnlineExperimentProxy::GetCalibrationValueInDevice()
     */
    auto OpenViewForExperimentDataItem(pacemaker::inca::detail::unique_com_ptr<::IDispatch> dataitem) -> void;
  };
} // namespace pacemaker::inca::com

#endif // PACEMAKER_INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_