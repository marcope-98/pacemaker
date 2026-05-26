#ifndef PACEMAKER_INCA_COM_INCAPROXY_HPP_
#define PACEMAKER_INCA_COM_INCAPROXY_HPP_

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/DispatchWrapper.hpp"
#include "pacemaker/inca/detail/incacom.hpp"

#include "pacemaker/inca/com/IncaExperimentViewProxy.hpp"
#include "pacemaker/inca/com/IncaOnlineExperimentProxy.hpp"

namespace pacemaker::inca::com
{
  /**
   * @brief DispatchWrapper for the top-level `Inca_Dispatch` COM interface.
   * 
   * Inherits `::Inca_Dispatch` and overloads two C++ methods that return fully typed proxy objects instead
   * of raw `IDispatch` pointers.
   * 
   * Instances of this type are never constructed directly; they are managed exclusively through the
   * `IncaProxy` owning smart pointer.
   * 
   * @see IncaProxy
   * @see pacemaker::inca::detail::COMProxy
   */
  struct Inca_DispatchWrapper : public pacemaker::inca::detail::DispatchWrapper<::Inca_Dispatch>
  {
    /**
     * @brief Returns the COM object representing the currently open experiment.
     * 
     * Calls the base `Inca_Dispatch::GetOpenedExperiment()`, detaches the returned `IDispatch`, and 
     * constructs an `IncaOnlineExperimentProxy` by narrowing the pointer via `query_interface`.
     * 
     * @return Owning `IncaOnlineExperimentProxy` for the open experiment.
     * 
     * @throws std::runtime_error if the base COM call fails or the resulting pointer cannot be narrowed to `IncaOnlineExperiment_Dispatch`.
     */
    [[nodiscard]] auto GetOpenedExperiment() -> IncaOnlineExperimentProxy { return IncaOnlineExperimentProxy{::Inca_Dispatch::GetOpenedExperiment().Detach()}; }

    /**
     * @brief Returns the COM object representing the currently open experiment view.
     * 
     * Calls the base `Inca_Dispatch::GetOpenedExperimentView()`, detaches the returned `IDispatch`, and constructs an 
     * `IncaExperimentViewProxy` by narrowing the pointer via `query_interface`.
     * 
     * @return Owning `IncaExperimentViewProxy` for the open experimnet view.
     * 
     * @throws std::runtime_error if the base COM call fails or the resulting pointer cannot be narrowed to `IncaExperimentView_Dispatch`.
     */
    [[nodiscard]] auto GetOpenedExperimentView() -> IncaExperimentViewProxy { return IncaExperimentViewProxy{::Inca_Dispatch::GetOpenedExperimentView().Detach()}; }
  };

  /**
   * @brief Owning smart-pointer proxy for the top-level INCA COM object.
   * 
   * `IncaProxy` is a type alias for `pacemaker::inca::detail::COMProxy<Inca_DispatchWrapper>`. It is the first
   * object constructed when establishing an INCA automation session and is the sole owner of the root `Inca_Dispatch` COM
   * interface pointer.
   * 
   * Use `operator->` to access the methods of `Inca_DispatchWrapper`:
   * @code
   * IncaProxy proxy{rawIDispatch};
   * auto exp     = proxy->GetOpenedExperiment();
   * auto expview = proxy->GetOpenedExperimentView();
   * @endcode 
   * 
   * The proxy is move-only. Copying is deleted via the `unique_ptr` base.
   * 
   * @note `DisconnectFromTool()` is **not** called in the proxy's destructor. It is responsibility of `Session` destructor to call
   *       `proxy->DisconnectFromTool()` before the proxy is destroyed.
   * 
   * @see Inca_DispatchWrapper
   * @see pacemaker::inca::detail::COMProxy
   * @see pacemaker::inca::Session
   */
  using IncaProxy = pacemaker::inca::detail::COMProxy<Inca_DispatchWrapper>;
} // namespace pacemaker::inca::com
#endif // PACEMAKER_INCA_COM_INCAPROXY_HPP_