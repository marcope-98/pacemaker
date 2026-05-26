#ifndef PACEMAKER_INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_
#define PACEMAKER_INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/DispatchWrapper.hpp"
#include "pacemaker/inca/detail/incacom.hpp"

namespace pacemaker::inca::com
{
  /**
   * @brief DispatchWrapper for the `IncaExperimentView_Dispatch` COM interface.
   * 
   * Inherits `::IncaExperimentView_Disaptch` and overloads no C++ methods. All methods of the 
   * underlying COM interface are accessible via `operator->` on the `IncaExperimentViewProxy` ownign pointer.
   * 
   * Instances are managed exclusively through the `IncaExperimentViewProxy` owning smart pointer.
   * 
   * @see IncaExperimentViewProxy
   * @see pacemaker::inca::detail::COMProxy
   */
  struct IncaExperimentView_DispatchWrapper : public pacemaker::inca::detail::DispatchWrapper<::IncaExperimentView_Dispatch>
  {
  };

  /**
   * @brief Owning smart-pointer proxy for the INCA experiment-view COM object.
   * 
   * `IncaExperimentViewProxy` is a type alias for `pacemaker::inca::detail::COMProxy<IncaExperimentView_DispatchWrapper>`.
   * 
   * All methods of `::IncaExperimentView_Dispatch` are accessible via `operator->`:
   * @code
   * IncaExperimentViewProxy view{rawIDispatch};
   * view->OpenViewForExperimentDataItem(dataItemPtr.get());
   * @endcode
   * 
   * The proxy is move-only. Copying is deleted via the `unique_ptr` base.
   * 
   * @see IncaExperimentView_DispatchWrapper
   * @see pacemaker::inca::detail::COMProxy
   * @see pacemaker::inca::Session
   */
  using IncaExperimentViewProxy = pacemaker::inca::detail::COMProxy<IncaExperimentView_DispatchWrapper>;
} // namespace pacemaker::inca::com

#endif // PACEMAKER_INCA_COM_INCAEXPERIMENTVIEWPROXY_HPP_