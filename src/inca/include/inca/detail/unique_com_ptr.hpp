#ifndef INCA_DETAIL_UNIQUE_COM_PTR_HPP_
#define INCA_DETAIL_UNIQUE_COM_PTR_HPP_

#include <memory>

namespace inca::detail
{
  /**
   * @brief Custom deleter for COM objects.
   *
   * Calls `Release()` on the managed pointer when it is non-nullptr, satisfying
   * the COM reference-counting contract without requiring explicit cleanup at
   * every call site.
   *
   * @tparam T A COM interface type that exposes a `Release()` method.
   */
  template<class T>
  struct ComObjDeleter
  {
    /**
     * @brief Release the COM object.
     *
     * If @p ptr is non-nullptr, `ptr->Release()` is called. A nullptr is
     * silentrly ignored, matching the behaviour of `delete nullptr`.
     *
     * @param ptr Raw pointer to the COM object to release. May be nullptr.
     */
    void operator()(T *ptr)
    {
      if (ptr == nullptr) return;
      ptr->Release();
    }
  };

  /**
   * @brief RAII owning pointer for COM objects.
   *
   * An alias for `std::unique_ptr<T, ComObjDeleter<T>>` that automatically
   * calls `T::Release()` when the pointer goes out of scope. Ownership
   * semantics are identical to `std::unique_ptr`: the type is move-only
   * and non-copyable, ensuring that a COM object's reference count is never
   * accidentally incremented by a copy
   *
   * ### Typical usage
   * @code
   * inca::detail::unique_com_ptr<IDispatch> p{rawPtr};
   * // rawPtr->Release() is called when p is destroyed.
   * @endcode
   *
   * @tparam T A COM interface type that exposes a `Release()` method.
   *
   * @see ComObjDeleter
   */
  template<class T>
  using unique_com_ptr = std::unique_ptr<T, ComObjDeleter<T>>;

}; // namespace inca::detail

#endif // INCA_DETAIL_UNIQUE_COM_PTR_HPP_