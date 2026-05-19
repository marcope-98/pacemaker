#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/com/CalibrationScalarDataProxy.hpp"

using ::testing::_;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC007)

namespace
{
  class MockCalibrationScalarDataProxy_Dispatch : public IDispatch
  {
  public:
    MOCK_METHOD(ULONG, AddRef, (), (override));
    MOCK_METHOD(ULONG, Release, (), (override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT, LCID, ITypeInfo **), (override));
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *), (override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (const IID &, LPOLESTR *, UINT, LCID, DISPID *), (override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *), (override));
    MOCK_METHOD(HRESULT, QueryInterface, (const IID &, void **), (override));

    void Delegate()
    {
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault([this](const IID &, void **out)
                                                         { *out = reinterpret_cast<void*>(this); return S_OK; });
    }

    template<class CB>
    void Delegate_SetImplValue(CB SetImplValue)
    {
      constexpr DISPID SetImplValue_dispid{0x60020015};
      ON_CALL(*this, Invoke(SetImplValue_dispid, _, _, _, _, _, _, _)).WillByDefault(SetImplValue);
    }
    double actual{};
  };
} // namespace

TEST_F(TC007, A)
{
  double expected{5.0};

  MockCalibrationScalarDataProxy_Dispatch mock;
  auto                                    SetImplValue = [&mock, expected](DISPID, const IID &, LCID, WORD, DISPPARAMS *dispparams, VARIANT *, EXCEPINFO *, UINT *)
  {
    EXPECT_EQ(dispparams->rgvarg[0].dblVal, expected);
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_SetImplValue(SetImplValue);

  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  constexpr DISPID SetImplValue_dispid{0x60020015};
  EXPECT_CALL(mock, Invoke(SetImplValue_dispid, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::CalibrationScalarDataProxy   calib(std::move(idispatch));

  calib.SetImplValue(expected);
}

TEST_F(TC007, B)
{
  MockCalibrationScalarDataProxy_Dispatch mock;
  mock.Delegate();

  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  constexpr DISPID ResetValueToRP_dispid{0x60020028};
  EXPECT_CALL(mock, Invoke(ResetValueToRP_dispid, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::CalibrationScalarDataProxy   calib(std::move(idispatch));

  calib.ResetValueToRP();
}