#ifndef INCA_TRAITS_COMPROXY_HPP_
#define INCA_TRAITS_COMPROXY_HPP_

#include "inca/unique_com_ptr.hpp"

#include "inca/traits/NonCopyable.hpp"
#include "inca/traits/NonMovable.hpp"

namespace inca::traits
{
  template<class T>
  class COMProxy : private inca::traits::NonCopyable, inca::traits::NonMovable
  {
  protected:
    inca::unique_com_ptr<T> p_subject{nullptr};
  };
} // namespace inca::traits

#endif // INCA_TRAITS_COMPROXY_HPP_