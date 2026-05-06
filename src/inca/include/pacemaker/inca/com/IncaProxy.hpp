#ifndef PACEMAKER_INCA_COM_INCAPROXY_HPP_
#define PACEMAKER_INCA_COM_INCAPROXY_HPP_

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

struct Inca_Dispatch;
struct IDispatch;

namespace pacemaker::inca::com
{
  /**
   * @brief Proxy for the top-level INCA COM automation object.
   *
   * `IncaProxy` wraps the `Inca_Dispatch` COM interface, which is the root
   * entry point of the INCA autoamation API. It provides access to the
   * currently open experiment and experiment-view objects.
   *
   * On destruction, `DisconnectFromTool()` is called on the underlying COM
   * object so that INCA is notified that the client has disconnected. This
   * ensures INCA is not left in a stale connected state after the session ends.
   *
   * The class is move-only; copying is deleted to prevent accidental
   * duplication of the COM connection.
   *
   * @note A running INCA instance must be present when the object is
   *       constructed. Obtain the required `IDispatch` pointer via
   *       `CoCreateIntance(CLSID_Inca, ...)` before constructing.
   *
   * @see pacemaker::inca::Session::connect()
   * @see pacemaker::inca::detail::COMProxy
   */
  class IncaProxy : private pacemaker::inca::detail::COMProxy<::Inca_Dispatch>
  {
  public:
    /**
     * @brief Constructs the proxy by narrowing a generic `IDispatch` to `Inca_Dispatch`.
     *
     * Calls `QueryInterface` internally via `pacemaker::inca::detail::query_interface`.
     *
     * @param idispatch Owning pointer to a generic `IDispatch` obtained from
     *                  `CoCreateInstance`. Ownership is transferred to the proxy.
     *
     * @throws std::invalid_argument if @p idispatch is nullptr.
     * @throws std::runtime_error    if `QueryInterface` fails to obtain `Inca_Dispatch`.
     */
    IncaProxy(pacemaker::inca::detail::unique_com_ptr<::IDispatch> idispatch);
    /// @brief Move constructor transfers ownership of the COM connection.
    IncaProxy(IncaProxy &&) = default;
    /// @brief Move assignment transfers ownership of the COM connection.
    IncaProxy &operator=(IncaProxy &&) = default;
    /**
     * @brief Destructor; calls `DisconnectFromTool()` if the COM pointer is valid.
     *
     * Notifies INCA that automation client is disconnecting. This method
     * is safe to call even if the proxy was move-constructed from, in which
     * case the internal pointer is nullptr and no COM call is made.
     */
    ~IncaProxy();

    /**
     * @brief Returns the COM object representing the currently open experiment.
     *
     * The returned pointer wraps an `IDispatch` that can be narrowed to `IncaOnlineExperiment_Dispatch`
     * by constructing an `IncaOnlineExperimentProxy`.
     *
     * @return Owning `IDispatch` pointer for the open experiment.
     *
     * @throws std::runtime_error if the underlying COM call fails or returns a nullptr.
     */
    [[nodiscard]] auto GetOpenedExperiment() -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>;

    /**
     * @brief Returns the COM object representing the currently open experiment view.
     *
     * The returned pointer wraps an `IDispatch` that can be narrowed to `IncaExperimentView_Dispatch`
     * by constructing an `IncaExperimentViewProxy`.
     *
     * @return Owning `IDispatch` pointer to the open experiment view.
     *
     * @throws std::runtime_error if the underlying COM call fails or returns a nullptr.
     */
    [[nodiscard]] auto GetOpenedExperimentView() -> pacemaker::inca::detail::unique_com_ptr<::IDispatch>;
  };
} // namespace pacemaker::inca::com
#endif // PACEMAKER_INCA_COM_INCAPROXY_HPP_