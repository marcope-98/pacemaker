#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>
#include <propvarutil.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/com/IncaOnlineExperimentProxy.hpp"

using ::testing::_;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC004)

namespace
{
  class MockIncaOnlineExperiment_Dispatch : public IDispatch
  {
  public:
    MOCK_METHOD(ULONG, AddRef, (), (override));
    MOCK_METHOD(ULONG, Release, (), (override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT, LCID, ITypeInfo **), (override));
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *), (override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (const IID &, LPOLESTR *, UINT, LCID, DISPID *), (override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *), (override));
    MOCK_METHOD(HRESULT, QueryInterface, (const IID &, void **), (override));

    virtual ~MockIncaOnlineExperiment_Dispatch() = default;

    template<class F>
    void Delegate_GetAllDevices(F GetAllDevices)
    {
      constexpr DISPID GetAllDevices_dispid{0x60020088};
      ON_CALL(*this, Invoke(GetAllDevices_dispid, _, _, _, _, _, _, _)).WillByDefault(GetAllDevices);
    }

    template<class F>
    void Delegate_GetCalibrationValueInDevice(F GetCalibrationValueInDevice)
    {
      constexpr DISPID GetCalibrationValueInDevice_dispid{0x60020090};
      ON_CALL(*this, Invoke(GetCalibrationValueInDevice_dispid, _, _, _, _, _, _, _)).WillByDefault(GetCalibrationValueInDevice);
    }

    void Delegate()
    {
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault([this](const IID &, void **out)
                                                         { *out = reinterpret_cast<void*>(this); return S_OK; });
    }
  };

  template<typename T>
  struct is_COMProxy : std::false_type
  {
  };

  template<typename T>
  struct is_COMProxy<pacemaker::inca::detail::COMProxy<T>> : std::true_type
  {
  };

  template<typename T>
  constexpr bool is_COMProxy_v = is_COMProxy<T>::value;
} // namespace

TEST_F(TC004, A)
{
  MockIncaOnlineExperiment_Dispatch mock;
  auto                              GetAllDevices = [&mock](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);

    SAFEARRAYBOUND Bound;
    Bound.lLbound   = 0;
    Bound.cElements = 2;
    SAFEARRAY *out  = SafeArrayCreate(VT_VARIANT, 1, &Bound);

    VARIANT elem;
    VariantInit(&elem);
    elem.pdispVal = &mock;
    elem.vt       = VT_DISPATCH;

    LONG index;
    index = 0;
    SafeArrayPutElement(out, &index, (void *)(&elem)); // Calls AddRef
    index = 1;
    SafeArrayPutElement(out, &index, (void *)(&elem)); // Calls AddRef

    variant->parray = out;
    variant->vt     = (VT_ARRAY | VT_VARIANT);
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_GetAllDevices(GetAllDevices);

  /**
   * AddRef call summary:
   * 2 * SafeArrayPutElement
   * 2 * SafeArrayGetElement
   * 2 * _variant_t::operator IDispatch*
   * 1 * Implicit call in QueryInterface
   * 1 * explicit call to AddRef (see below code)
   *
   * https://learn.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-safearrayputelement
   * If the data element is a VT_DISPATCH or VT_UNKNOWN, AddRef is called to increment the object's reference count.
   *
   * https://learn.microsoft.com/en-us/cpp/cpp/variant-t-extractors?view=msvc-170
   * operator IDispatch*( ) Extracts a dispinterface pointer from an encapsulated VARIANT. AddRef is called on the resulting pointer, so it is up to you to call Release to free it.
   *
   * https://learn.microsoft.com/en-us/windows/win32/api/oleauto/nf-oleauto-safearraygetelement
   * If the data element is a string, object, or variant, the function copies the element in the correct way
   *
   * Release call summary:
   * 1 * Destructor of IncaOnlineExperimentProxy
   * 1 * Implicitly called inside QueryInterface (destructor of unique_com_ptr input argument)
   * 2 * Destructor of devices
   * 2 * Destructor of _variant_t in IncaOnlineExperimentProxy::GetAllDevices
   * 2 * Destructor of SAFEARRAY  in IncaOnlineExperimentProxy::GetAllDevices (to be precise the destructor of _variant_t containing the SAFEARRAY)
   */
  EXPECT_CALL(mock, AddRef()).Times(7);                       // One per device
  EXPECT_CALL(mock, Release()).Times(10);                     // IncaOnlineExperimentProxy destructor + QueryInterface + 6 AddRefs
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(3);           // IncaOnlineExperimentProxy constructor
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1); // GetAllDevices

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));

  EXPECT_NO_THROW(
      {
        auto devices = exp->GetAllDevices();
        EXPECT_EQ(devices.size(), 2);
        EXPECT_NE(devices[0], nullptr);
        EXPECT_NE(devices[1], nullptr);
      });
}

TEST_F(TC004, B)
{
  MockIncaOnlineExperiment_Dispatch mock;
  auto                              GetAllDevices = [&mock](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);
    variant->lVal = 42L;
    variant->vt   = VT_I4;
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_GetAllDevices(GetAllDevices);

  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));
  EXPECT_THROW(auto devices = exp->GetAllDevices(), std::runtime_error);
}

TEST_F(TC004, C)
{
  MockIncaOnlineExperiment_Dispatch mock;
  auto                              GetCalibrationValueInDevice = [&mock](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    InitVariantFromDispatch(&mock, variant);
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_GetCalibrationValueInDevice(GetCalibrationValueInDevice);

  EXPECT_CALL(mock, AddRef()).Times(2);
  EXPECT_CALL(mock, Release()).Times(4);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(2);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));

  const std::string param = "unknownParam";
  auto              out   = exp->GetCalibrationValueInDevice(param, nullptr);
  EXPECT_EQ(typeid(out).name(), typeid(pacemaker::inca::com::CalibrationScalarDataProxy{}).name());
}

TEST_F(TC004, D)
{
  MockIncaOnlineExperiment_Dispatch mock;
  auto                              GetCalibrationValueInDevice = [&mock](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);
    variant->pdispVal = nullptr;
    variant->vt       = VT_DISPATCH;
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_GetCalibrationValueInDevice(GetCalibrationValueInDevice);

  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));

  const std::string param = "unknownParam";
  EXPECT_THROW(
      {
        try
        {
          [[maybe_unused]] auto out = exp->GetCalibrationValueInDevice(param, nullptr);
        }
        catch (const std::runtime_error &e)
        {
          EXPECT_EQ("GetCalibrationValueInDevice: parameter not found " + param, e.what());
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TC004, E)
{
  EXPECT_TRUE(is_COMProxy_v<pacemaker::inca::com::IncaOnlineExperimentProxy>);
}