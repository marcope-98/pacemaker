#ifndef INCA_DETAIL_COMPROXY_HPP_
#define INCA_DETAIL_COMPROXY_HPP_

#include "inca/detail/unique_com_ptr.hpp"

namespace inca::detail
{
  template<class T>
  class COMProxy
  {
  protected:
    explicit COMProxy(inca::detail::unique_com_ptr<T> subject) noexcept
        : p_subject{std::move(subject)}
    {
    }
    COMProxy(const COMProxy &)            = delete;
    COMProxy &operator=(const COMProxy &) = delete;

    COMProxy(COMProxy &&)            = default;
    COMProxy &operator=(COMProxy &&) = default;

    COMProxy()  = default;
    ~COMProxy() = default;

    inca::detail::unique_com_ptr<T> p_subject{nullptr};
  };
} // namespace inca::detail

#endif // INCA_DETAIL_COMPROXY_HPP_