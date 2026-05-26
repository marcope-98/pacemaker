#ifndef PACEMAKER_INCA_DETAIL_DISPATCHWRAPPER_HPP_
#define PACEMAKER_INCA_DETAIL_DISPATCHWRAPPER_HPP_

namespace pacemaker::inca::detail
{
  /** 
   * @brief Helper struct for standardize interface retrieval
   * 
   * Exposes an alias `type` that matches the underlying *_Dispatch interface specified
   * as the template parameter.
   * 
   * @tparam T A COM interface
  */
  template<class T>
  struct DispatchWrapper : public T
  {
    using type = T;
  };
} // namespace pacemaker::inca::detail

#endif