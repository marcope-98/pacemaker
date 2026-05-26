#ifndef PACEMAKER_INCA_DETAIL_UTILS_HPP_
#define PACEMAKER_INCA_DETAIL_UTILS_HPP_

#include <stdexcept>
#include <typeinfo>

#include <comdef.h>

#include "pacemaker/inca/detail/unique_com_ptr.hpp"

namespace pacemaker::inca::detail
{
  /**
   * @brief Narrows a generic `IDispatch` pointer to a typed DispatchWrapper
   *
   * Calls `QueryInterface` on the supplied `IDispatch` using `__uuidof(T::type)`, where
   * `T::type` is the raw COM dispatch type that the DispatchWrapper `T` inherits from.
   * On success the function returns an owning `unique_com_ptr<T>`; on failure it throws
   * a standard exception so that COM errors surface immediately with context, rather that
   * propagating as a null pointer that crashes later.
   *
   * This function is called exclusively from `COMProxy<T>`'s owning ctor and is not part of the public API.
   *
   * @tparam T A DispatchWrapper struct that publicly inherits a COM dispatch type and
   *           declares `using type = <raw dispatch type>`. `__uuidof(T::type)` must be a valid expression.
   *
   * @param src Owning pointer to the source `IDispatch` object. Ownership is consumed unconditionally:
   *            the pointer is released either because `QueryInterface` succeeded and the new typed pointer
   *            takes over, or because `src` goes out of scope on the exception path.
   *
   * @return Owning `unique_com_ptr<T>` pointing to the same underlying COM object narrowed to the `T` interface.
   *
   * @throws std::invalid_argument if @p src is null
   * @throws std::runtime_error    if `QueryInterface` returns a failing `HRESULT`; the message includes the demangled name of `T`.
   *
   * @see pacemaker::inca::detail::COMProxy
   * @see pacemaker::inca::detail::unique_com_ptr
   */
  template<class T>
  [[nodiscard]] auto query_interface(pacemaker::inca::detail::unique_com_ptr<::IDispatch> src) -> pacemaker::inca::detail::unique_com_ptr<T>
  {
    if (src == nullptr)
      throw std::invalid_argument("Attempt on calling QueryInterface on nullptr");

    T      *out{nullptr};
    HRESULT hr = src->QueryInterface(__uuidof(T::type), reinterpret_cast<void **>(&out));

    if (FAILED(hr))
      throw std::runtime_error(
          std::string("QueryInterface failed for ") + typeid(T).name());

    return pacemaker::inca::detail::unique_com_ptr<T>(out);
  }
} // namespace pacemaker::inca::detail

#endif // PACEMAKER_INCA_DETAIL_UTILS_HPP_