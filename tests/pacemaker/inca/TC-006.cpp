#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/com/ExperimentDeviceProxy.hpp"

using ::testing::_;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC006)

namespace
{
  class MockExperimentDevice_Dispatch : public IDispatch
  {
  public:
    MOCK_METHOD(ULONG, AddRef, (), (override));
    MOCK_METHOD(ULONG, Release, (), (override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT, LCID, ITypeInfo **), (override));
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *), (override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (const IID &, LPOLESTR *, UINT, LCID, DISPID *), (override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *), (override));
    MOCK_METHOD(HRESULT, QueryInterface, (const IID &, void **), (override));

    virtual ~MockExperimentDevice_Dispatch() = default;

    void Delegate()
    {
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault([this](const IID &, void **out)
                                                         { *out = reinterpret_cast<void*>(this); return S_OK; });
    }

    template<class CB>
    void Delegate_GetName(CB GetName)
    {
      constexpr DISPID GetName_dispid{0x60020009};
      ON_CALL(*this, Invoke(GetName_dispid, _, _, _, _, _, _, _)).WillByDefault(GetName);
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

TEST_F(TC006, A)
{
  MockExperimentDevice_Dispatch mock;
  auto                          GetName = [&mock](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);
    variant->bstrVal = SysAllocString(L"ECU1");
    variant->vt      = VT_BSTR;
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_GetName(GetName);

  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::ExperimentDeviceProxy        device(std::move(idispatch));

  std::wstring name = device->GetName();
  EXPECT_EQ(name, std::wstring(L"ECU1"));
}

TEST_F(TC006, B)
{
  EXPECT_TRUE(is_COMProxy_v<pacemaker::inca::com::ExperimentDeviceProxy>);
}