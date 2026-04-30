#ifndef INCA_DETAIL_COMPROXY_HPP_
#define INCA_DETAIL_COMPROXY_HPP_

#include "inca/detail/unique_com_ptr.hpp"

namespace inca::detail
{
  /**
   * @brief Non-copyable CRTP base class for COM dispatch proxy objects.
   *
   * `COMProxy<T>` stores a `unique_com_ptr<T>` and enforces move-only
   * ownership semantics so that concrete proxy classes do not need to repeat
   * this boilerplate.
   *
   * Concrete proxy classes inherit privately from this base and gain access
   * to the protected member `p_subject` through which they invoke the
   * underlying COM methods.
   *
   * ### Typical usage
   * @code
   * class MyProxy : private inca::detail::COMProxy<MyDispatch_Dispatch>
   * {
   * public:
   *   explicit MyProxy(inca::detail::unique_com_ptr<IDispatch> idispatch);
   *   void DoSomething() { this->p_subject->DoSomething(); }
   * };
   * @endcode
   *
   * @tparam T the concrete COM dispatch type managed by this proxy.
   *
   * @see inca::detail::unique_com_ptr
   */
  template<class T>
  class COMProxy
  {
  protected:
    /**
     * @brief Constructs the proxy, taking ownership of @p subject.
     *
     * @param subject Owning pointer to the COM object. Moved into the
     *                internal `p_subject` member; the callers' pointer is
     *                left null after construction.
     */
    explicit COMProxy(inca::detail::unique_com_ptr<T> subject) noexcept
        : p_subject{std::move(subject)}
    {
    }
    /// @brief Move constructor transfers ownership of the underlying COM pointer.
    COMProxy(COMProxy &&) = default;
    /// @brief Move assignment transfers ownership of the underlying COM pointer.
    COMProxy &operator=(COMProxy &&) = default;

    /**
     * @brief Owning pointer to the underlying COM dispatch object.
     *
     * Derived classes use this member to invoke methods on the wrapped COM
     * interface. Its lifetime is tied to the proxy: the COM object is
     * released when `p_subject` is destroyed.
     */
    inca::detail::unique_com_ptr<T> p_subject{nullptr};
  };
} // namespace inca::detail

#endif // INCA_DETAIL_COMPROXY_HPP_