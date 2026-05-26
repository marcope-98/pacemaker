#ifndef PACEMAKER_INCA_DETAIL_COMPROXY_HPP_
#define PACEMAKER_INCA_DETAIL_COMPROXY_HPP_

#include "pacemaker/inca/detail/incacom.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"
#include "pacemaker/inca/detail/utils.hpp"

namespace pacemaker::inca::detail
{
  /**
   * @brief Owning smart-pointer base class for COM dispatch proxy objects.
   * 
   * `COMProxy<T>` publicly inherits `unique_com_ptr<T>`, making every 
   * instantiated proxy type an owning smart pointer. Proxy types are
   * declared as type aliases:
   * @code
   * using FooProxy = pacemaker::inca::detail::COMProxy<FOO_DispatchWrapper>;
   * @endcode
   * 
   * `T` must be a "DispatchWrapper" struct, i.e. a `struct` that:
   * - publicly inherits the corresponding raw COM dispatch type, and
   * - declares `using type = ::RawDispatchType`.
   * 
   * The `type` typedef is consumed by `query_interface<T>`, which uses
   * `__uuidof(T::type)` to call `QueryInterface` with the correct interface 
   * identifier.
   * 
   * ### Constructors
   * Three ctors are provided:
   * - **Default**: leaves the pointer null; required so proxy type aliases
   *   can be default-constructed before being move-assigned (e.g.\ when stored
   *   as class members initialised in a constrcutor body).
   * - **Raw `IDispatch*`**: convenience overload that wraps the pointer in a 
   *   `unique_com_ptr<IDispatch>` and delegates to the owning ctor.
   * - **`unique_com_ptr<IDispatch>`**: takes ownership of the supplied
   *   generic pointer and narrows it to `T` via `query_interface<T>`.
   * 
   * Copy construction and copy assignment are implicitly deleted by the 
   * `unique_ptr` base, preventing accidental duplication of COM reference
   * counts. Move construction and move assignment are defaulted.
   * 
   * ### Typical usage
   * @code
   * // Obtain a raw IDispatch from CoCreateInstance or a COM method, then:
   * FooProxy proxy{rawIDispatch};     // narrows via QueryInterface
   * proxy->SomeCOMMethod();           // operator-> reaches Foo_DispatchWrapper
   * ::Foo_Dispatch *raw = proxy.get() // non-owning raw pointer
   * @endcode
   * 
   * @tparam T A DispatchWrapper struct that publicly inherits a COM dispatch
   *           type and declares `using type = <raw dispatch type>`.
   * 
   * @see pacemaker::inca::detail::unique_com_ptr
   * @see pacemaker::inca::detail::query_interface
   */
  template<class T>
  class COMProxy : public pacemaker::inca::detail::unique_com_ptr<T>
  {
  public:
    /**
     * @brief Default ctor; leaves the internal pointer null.
     * 
     * A null proxy compares equal to `nullptr` via the inherited
     * `unique_ptr::operator bool()`. It must be populated by move-assignment
     * before any COM methods are invoked through it.
     */
    COMProxy() = default;
    
    /**
     * @brief Constrcuts the proxy from a raw `IDispatch*`, taking ownership.
     * 
     * Wraps @p subject in a `unique_com_ptr<IDispatch>` and delegates to the 
     * owning ctor, which calls `query_interface<T>` to narrow the
     * pointer to `T`.
     * 
     * @param subject Raw `IDispatch` pointer obtained from a COM call.
     *                Ownership is transferred to the proxy; the caller must
     *                not call `Release()` on it afterwards.
     * 
     * @throws std::invalid_argument if @p subject is null.
     * @throws std::runtime_error    if `QueryInterface for `T::type` fails.
     */
    explicit COMProxy(::IDispatch *subject) : COMProxy{unique_com_ptr<::IDispatch>{subject}} {}

    /**
     * @brief Constructs the proxy by narrowing an owning `IDispatch` pointer to `T`.
     * 
     * Calls `query_interface<T>(std::move(subject))` which invokes 
     * `QueryInterface` with `__uuidof(T::type)`. The resulting 
     * `unique_com_ptr<T>` is stored in the `unique_ptr` base.
     * 
     * @param subject Owning pointer to a generic `IDispatch` COM object.
     *                Ownership is transferred; the caller's handle is left
     *                null after construction.
     * 
     * @throws std::invalid_argument if @p subject is null.
     * @throws std::runtime_error    if `QueryInterface` for `T::type` fails.
     */
    explicit COMProxy(unique_com_ptr<::IDispatch> subject)
        : unique_com_ptr<T>{
              query_interface<T>(
                  std::move(subject))} {}

    /// @brief Move ctor; transfers ownership of the COM pointer.
    COMProxy(COMProxy &&) = default;
    /// @brief Move assignment; transfers ownership of the COM pointer.
    COMProxy &operator=(COMProxy &&) = default;
  };
} // namespace pacemaker::inca::detail

#endif // PACEMAKER_INCA_DETAIL_COMPROXY_HPP_