#ifndef INCA_SESSION_HPP_
#define INCA_SESSION_HPP_

#include "inca/Experiment.hpp"
#include "inca/com/IncaProxy.hpp"

namespace inca
{
  /**
   * @brief Top-level session object that owns the connection to a running INCA instance.
   *
   * `Session` is the single entry point for the pacemaker library. It establishes a COM
   * connection to a locally running INCA process, validates that an experiment is open and
   * at least one device is online, and constructs the `Experiment` object through which the
   * measurement run is controlled.
   *
   * ### Typical usage
   * @code
   * CoInitialize(NULL);
   * auto session = inca::Session::connect();
   * auto &exp    = session.experiment();
   *
   * exp.add_param("Engine_TorqueRequest");
   * exp.start_recording();
   * exp.set_param("Engine_TorqueRequest", 120.0);
   * exp.stop_recording("run_001.mf4");
   *
   * CoUninitialize();
   * @endcode
   *
   * ### Lifetime and thread safety
   * The session is non-copyable and non-movable. Exactly one `Session` should exist
   * per INCA automation connection. COM must be initialised on the calling thread
   * before `connect()` is invoked.
   *
   * On destruction, `IncaProxy`'s destructor calls `DisconnectFromTool()` so INCA
   * is cleanly notified of the disconnection.
   *
   * @note Platform: Windows only. INCA must be installed and runnign.
   *
   * @see inca::Experiment
   * @see inca::com::IncaProxy
   */
  class Session
  {
  public:
    /**
     * @brief Factory function that connects to a running INCA instance and returns an initialised `Session`.
     *
     * The function performs the following steps:
     * 1. Calls `CoCreateInstance(CLSID_Inca, CLSCTX_LOCAL_SERVER)` to obtain the root INCA COM object.
     * 2. Retrieves the open experiment and experiment-view objects via `IncaProxy`.
     * 3. Enumerates connected devices; selects the first available device.
     * 4. Constructs and returns a `Session` owning an `IncaProxy` and the fully initialised `Experiment`.
     *
     * @return A fully constructed `Session` ready for use.
     *
     * @throws std::runtime_error if `CoCreateInstance` fails (INCA not installed or not running).
     * @throws std::runtime_error if the open experiment has no devices configured or online.
     *
     * @pre COM must be initialised on the calling thread (e.g. `CoInitializeEx`)
     */
    [[nodiscard]] static auto connect() -> Session;

    /// @brief Copying is deleted; the COM connection must not be duplicated.
    Session(const Session &) = delete;
    /// @brief Copying assignment is deleted; the COM connection must not be duplicated.
    Session &operator=(const Session &) = delete;
    /// @brief Move construction is deleted; the session has a unique identity.
    Session(Session &&) = delete;
    /// @brief Move assignment is deleted; the session has a unique identity.
    Session &operator=(Session &&) = delete;

    /**
     * @brief Destructor; triggers disconnection from INCA via `IncaProxy`.
     *
     * The `IncaProxy` member is destroyed first (in reverse declaration order), which calls
     * `DisconnectFromTool()` on the INCA COM object. The destructor is explicitly defined (rather than
     * defaulted in the header) to ensure this sequencing is correct across translation units.
     */
    ~Session();

    /**
     * @brief Returns a reference to the owned `Experiment`.
     *
     * The reference is valid for the lifetime of the `Session`. Use it to register parameters,
     * control recordings, and update calibration values.
     *
     * @return Non-const reference to the session's `Experiment` object.
     */
    [[nodiscard]] auto experiment() noexcept -> Experiment & { return this->m_experiment; }

  private:
    /**
     * @brief Private constructor used exclusively by `connect()`.
     *
     * @param inca       Initialised proxy for the top-level INCA COM object.
     * @param experiment Fully constructed experiment, ready for use
     */
    Session(inca::com::IncaProxy inca, inca::Experiment experiment);

    inca::com::IncaProxy m_inca;
    inca::Experiment     m_experiment;
  };
} // namespace inca

#endif // INCA_SESSION_HPP_