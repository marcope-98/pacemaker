#ifndef INCA_TRAITS_COMPROXY_HPP_
#define INCA_TRAITS_COMPROXY_HPP_

#include "inca/unique_com_ptr.hpp"

namespace inca::traits
{
  template<class T>
  class COMProxy
  {
  protected:
    explicit COMProxy(inca::unique_com_ptr<T> subject) noexcept
        : p_subject{std::move(subject)}
    {
    }
    COMProxy(const COMProxy &)            = delete;
    COMProxy &operator=(const COMProxy &) = delete;

    COMProxy(COMProxy &&)            = default;
    COMProxy &operator=(COMProxy &&) = default;

    COMProxy()  = default;
    ~COMProxy() = default;

    inca::unique_com_ptr<T> p_subject{nullptr};
  };
} // namespace inca::traits

#endif // INCA_TRAITS_COMPROXY_HPP_