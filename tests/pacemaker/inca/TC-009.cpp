#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>
#include <propvarutil.h>

#include <chrono>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/Session.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC009)

namespace
{
  struct MockCOM : public IDispatch
  {
    MOCK_METHOD(ULONG, AddRef, (), (override));
    MOCK_METHOD(ULONG, Release, (), (override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT, LCID, ITypeInfo **), (override));
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *), (override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (const IID &, LPOLESTR *, UINT, LCID, DISPID *), (override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *), (override));
    MOCK_METHOD(HRESULT, QueryInterface, (const IID &, void **), (override));

    virtual ~MockCOM() = default;

    template<class F>
    void Delegate_GetAllDevices(F GetAllDevices)
    {
      constexpr DISPID GetAllDevices_dispid{0x60020088};
      ON_CALL(*this, Invoke(GetAllDevices_dispid, _, _, _, _, _, _, _)).WillByDefault(GetAllDevices);
    }

    template<class F>
    void Delegate_SetImplValue(F SetImplValue)
    {
      constexpr DISPID SetImplValue_dispid{0x60020015};
      ON_CALL(*this, Invoke(SetImplValue_dispid, _, _, _, _, _, _, _)).WillByDefault(SetImplValue);
    }

    void Delegate()
    {
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault([this](const IID &, void **out)
                                                         {*out = reinterpret_cast<void*>(this); return S_OK; });
      // clang-format off
      constexpr DISPID GetOnlineExperiment_dispid{0x6002002f};
      auto             GetOnlineExperiment = [this](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *) {
        InitVariantFromDispatch(this, variant);
        return S_OK;
      };
      ON_CALL(*this, Invoke(GetOnlineExperiment_dispid, _, _, _, _, _, _, _)).WillByDefault(GetOnlineExperiment);
      // clang-format on
      // clang-format off
      constexpr DISPID GetOnlineExperimentView_dispid{0x60020030};
      auto             GetOnlineExperimentView = [this](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *){ 
        InitVariantFromDispatch(this, variant);
        return S_OK; 
      }; 
      ON_CALL(*this, Invoke(GetOnlineExperimentView_dispid, _, _, _, _, _, _, _)).WillByDefault(GetOnlineExperimentView);
      
      constexpr DISPID GetAllDevices_dispid{0x60020088};
      auto             GetAllDevices = [this](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *){
        
        VariantInit(variant);
        SAFEARRAYBOUND Bound;
        Bound.lLbound   = 0;
        Bound.cElements = 1;
        SAFEARRAY *out  = SafeArrayCreate(VT_VARIANT, 1, &Bound);
        
        VARIANT elem;
        VariantInit(&elem);
        elem.pdispVal = this;
        elem.vt       = VT_DISPATCH;
        
        LONG index = 0;
        SafeArrayPutElement(out, &index, (void*)(&elem));
        variant->parray = out;
        variant->vt     = (VT_ARRAY | VT_VARIANT);
        return S_OK;
      };
      ON_CALL(*this, Invoke(GetAllDevices_dispid, _, _, _, _, _, _, _)).WillByDefault(GetAllDevices);

      constexpr DISPID GetCalibrationValueInDevice_dispid{0x60020090};
      auto             GetCalibrationValueInDevice = [this](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *){
        InitVariantFromDispatch(this, variant);
        return S_OK;
      };
      ON_CALL(*this, Invoke(GetCalibrationValueInDevice_dispid, _, _, _, _, _, _, _)).WillByDefault(GetCalibrationValueInDevice);
      // clang-format on
    }
  };
} // namespace

TEST_F(TC009, A) { /* Left empty on purpose because it is an integration test*/ }
TEST_F(TC009, B) { /* Left empty on purpose because it is an integration test*/ }
TEST_F(TC009, C)
{
  MockCOM mock;
  mock.Delegate();
  mock.Delegate_GetAllDevices([](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
                              {                  
    VariantInit(variant);

    SAFEARRAYBOUND Bound;
    Bound.lLbound   = 0;
    Bound.cElements = 0;
    SAFEARRAY *out  = SafeArrayCreate(VT_VARIANT, 1, &Bound);
    
    variant->parray = out;
    variant->vt     = (VT_ARRAY | VT_VARIANT);

    return S_OK; });

  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in Invoke(GetOpenedExperiment_dispid)
   *  - Implicit call in Invoke(GetOpenedExperimentView_dispid)
   * QueryInterface:
   *  - IncaProxy                 ctor
   *  - IncaOnlineExperimentProxy ctor
   *  - IncaExperimentViewProxy   ctor
   * Release:
   *  - IncaProxy                 dtor
   *  - IncaOnlineExperimentProxy dtor
   *  - IncaExperimentView        dtor
   *  - Implicit call in QueryInterface x3
   */
  EXPECT_CALL(mock, AddRef()).Times(3);
  EXPECT_CALL(mock, Release()).Times(6);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(3);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());

  EXPECT_THROW(
      {
        auto session = pacemaker::inca::Session::connect([&mock]()
                                                         { mock.AddRef(); return &mock; });
      },
      std::runtime_error);
}

TEST_F(TC009, D)
{
  EXPECT_FALSE(std::is_copy_constructible_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_copy_assignable_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_move_constructible_v<pacemaker::inca::Session>);
  EXPECT_FALSE(std::is_move_assignable_v<pacemaker::inca::Session>);
}

TEST_F(TC009, E)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in Invoke(GetOpenedExperiment_dispid)
   *  - Implicit call in Invoke(GetOpenedExperimentView_dispid)
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - Implicit call in QueryInterface x4
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(6);
  EXPECT_CALL(mock, Release()).Times(10);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(4);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());

  EXPECT_CALL(mock, Invoke(0x60020021, _, _, _, _, _, _, _)).Times(1);
  {
    auto session = pacemaker::inca::Session::connect([&mock]()
                                                     { mock.AddRef(); return &mock; });
  }
}
TEST_F(TC009, F)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in Invoke(GetOpenedExperiment_dispid)
   *  - Implicit call in Invoke(GetOpenedExperimentView_dispid)
   *  - Implicit call in Invoke(GetCalibrationValueInDevice_dispid)
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetCalibrationValueInDevice (cast of device to _variant_t)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   *  - CalibrationScalarData      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - CalibrationScalarData      dtor
   *  - Implicit call in QueryInterface x5
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   *  - Implicit call in GetCalibrationValueInDevice (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(8);
  EXPECT_CALL(mock, Release()).Times(13);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(5);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());
  EXPECT_CALL(mock, Invoke(0x60020090, _, _, _, _, _, _, _)).Times(1);
  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });
  session.add_param("speed");
  session.add_param("speed");
}

TEST_F(TC009, G)
{
  double  expected{50.0};
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in Invoke(GetOpenedExperiment_dispid)
   *  - Implicit call in Invoke(GetOpenedExperimentView_dispid)
   *  - Implicit call in Invoke(GetCalibrationValueInDevice_dispid)
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetCalibrationValueInDevice (cast of device to _variant_t)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   *  - CalibrationScalarData      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - CalibrationScalarData      dtor
   *  - Implicit call in QueryInterface x5
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   *  - Implicit call in GetCalibrationValueInDevice (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(8);
  EXPECT_CALL(mock, Release()).Times(13);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(5);

  mock.Delegate_SetImplValue([&mock, expected](DISPID, const IID &, LCID, WORD, DISPPARAMS *dispparams, VARIANT *, EXCEPINFO *, UINT *)
                             { EXPECT_EQ(dispparams->rgvarg[0].dblVal, expected); return S_OK; });

  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());
  EXPECT_CALL(mock, Invoke(0x60020015, _, _, _, _, _, _, _)).Times(1);

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });
  session.add_param("torque");
  session.set_param("torque", expected);
}

TEST_F(TC009, H)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in GetOpenedExperiment
   *  - Implicit call in GetOpenedExperimentView
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - Implicit call in QueryInterface x4
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(6);
  EXPECT_CALL(mock, Release()).Times(10);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(4);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });

  std::string param{"torque"};
  EXPECT_NO_THROW(session.set_param(param, 50.0));
}

TEST_F(TC009, I)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in Invoke(GetOpenedExperiment_dispid)
   *  - Implicit call in Invoke(GetOpenedExperimentView_dispid)
   *  - Implicit call in Invoke(GetCalibrationValueInDevice_dispid)                 x2
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetCalibrationValueInDevice (cast of device to _variant_t) x2
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   *  - CalibrationScalarData      ctor x2
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - CalibrationScalarData      dtor x2
   *  - Implicit call in QueryInterface x6
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   *  - Implicit call in GetCalibrationValueInDevice (_variant_t device dtor implicit Release()) x2
   */
  EXPECT_CALL(mock, AddRef()).Times(10);
  EXPECT_CALL(mock, Release()).Times(16);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(6);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());
  EXPECT_CALL(mock, Invoke(0x60020028, _, _, _, _, _, _, _)).Times(2);

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });

  session.add_param("a");
  session.add_param("b");
  session.reset();
}

TEST_F(TC009, J)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in GetOpenedExperiment
   *  - Implicit call in GetOpenedExperimentView
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - Implicit call in QueryInterface x4
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(6);
  EXPECT_CALL(mock, Release()).Times(10);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(4);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });

  auto begin = std::chrono::steady_clock::now();
  session.stop_recording("");
  auto end     = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
  EXPECT_GE(elapsed, 40);
}

TEST_F(TC009, K)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in GetOpenedExperiment
   *  - Implicit call in GetOpenedExperimentView
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - Implicit call in QueryInterface x4
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(6);
  EXPECT_CALL(mock, Release()).Times(10);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(4);

  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());

  InSequence seq;
  EXPECT_CALL(mock, Invoke(0x600200a5, _, _, _, _, _, _, _)).Times(1); // StopRecordingAndSave
  EXPECT_CALL(mock, Invoke(0x60020074, _, _, _, _, _, _, _)).Times(1); // StopMeasurement

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });
  session.stop_recording("");
}

TEST_F(TC009, L)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in GetOpenedExperiment
   *  - Implicit call in GetOpenedExperimentView
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - Implicit call in QueryInterface x4
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(6);
  EXPECT_CALL(mock, Release()).Times(10);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(4);

  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());
  EXPECT_CALL(mock, Invoke(0x600200a1, _, _, _, _, _, _, _)).Times(1);

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });
  session.start_recording();
}

TEST_F(TC009, M)
{
  MockCOM mock;
  mock.Delegate();
  // clang-format off
  struct TC009M_Exception {};
  EXPECT_THROW(
      { 
        auto session = pacemaker::inca::Session::connect(
          [&mock]() -> IDispatch*
          {
            throw TC009M_Exception{};
          }
        ); 
      },
      TC009M_Exception);
  // clang-format on
}

TEST_F(TC009, N)
{
  double  expected{50.0};
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in Invoke(GetOpenedExperiment_dispid)
   *  - Implicit call in Invoke(GetOpenedExperimentView_dispid)
   *  - Implicit call in Invoke(GetCalibrationValueInDevice_dispid)
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetCalibrationValueInDevice (cast of device to _variant_t)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   *  - CalibrationScalarData      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - CalibrationScalarData      dtor
   *  - Implicit call in QueryInterface x5
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   *  - Implicit call in GetCalibrationValueInDevice (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(8);
  EXPECT_CALL(mock, Release()).Times(13);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(5);

  mock.Delegate_SetImplValue([&mock, expected](DISPID, const IID &, LCID, WORD, DISPPARAMS *dispparams, VARIANT *, EXCEPINFO *, UINT *)
                             { EXPECT_EQ(dispparams->rgvarg[0].dblVal, expected); return S_OK; });

  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());
  EXPECT_CALL(mock, Invoke(0x60020015, _, _, _, _, _, _, _)).Times(1);

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });
  session.add_param("torque");
  session.set_param(0, expected);
}

TEST_F(TC009, O)
{
  MockCOM mock;
  mock.Delegate();
  /**
   * AddRef:
   *  - Session factory function (cfr. connect lambda)
   *  - Implicit call in GetOpenedExperiment
   *  - Implicit call in GetOpenedExperimentView
   *  - Implicit call in Invoke(GetAllDevices_dispid)
   *  - Implicit call in GetAllDevices (IDispatch* cast operator on _variant_t)
   *  - Implicit call in GetAllDevices (SafeArrayGetElement implicit AddRef())
   * QueryInterface:
   *  - IncaProxy                  ctor
   *  - IncaOnlineExperimentProxy  ctor
   *  - IncaExperimentViewProxy    ctor
   *  - ExperimentDeviceProxy      ctor
   * Release:
   *  - IncaProxy                  dtor
   *  - IncaOnlineExperimentProxy  dtor
   *  - IncaExperimentView         dtor
   *  - ExperimentDeviceProxy      dtor
   *  - Implicit call in QueryInterface x4
   *  - Implicit call in SAFEARRAY dtor
   *  - Implicit call in GetAllDevices (_variant_t device dtor implicit Release())
   */
  EXPECT_CALL(mock, AddRef()).Times(6);
  EXPECT_CALL(mock, Release()).Times(10);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(4);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(AnyNumber());

  auto session = pacemaker::inca::Session::connect([&mock]()
                                                   { mock.AddRef(); return &mock; });

  EXPECT_NO_THROW(session.set_param(0, 50.0));
}