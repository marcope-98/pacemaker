#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/com/IncaProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

using ::testing::_;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC003)

namespace
{
  std::size_t counter{};

  class MockInca_Dispatch : public IDispatch
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
      ON_CALL(*this, Release()).WillByDefault(Return(0));
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault([this](const IID &, void **out)
                                                         {*out = reinterpret_cast<void*>(this); return S_OK; });
      // clang-format off
      constexpr DISPID DisconnectFromTool_dispid{0x60020021};
      auto             DisconnectFromTool = [](){counter++; return S_OK; };
      ON_CALL(*this, Invoke(DisconnectFromTool_dispid, _, _, _, _, _, _, _)).WillByDefault(DisconnectFromTool);
      // clang-format on
      // clang-format off
      constexpr DISPID GetOnlineExperiment_dispid{0x6002002f};
      auto             GetOnlineExperiment = [this](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *) {
        VariantInit(variant);
        variant->pdispVal = this;
        variant->vt = VT_DISPATCH;
        return S_OK;
      };
      ON_CALL(*this, Invoke(GetOnlineExperiment_dispid, _, _, _, _, _, _, _)).WillByDefault(GetOnlineExperiment);
      // clang-format on
      // clang-format off
      constexpr DISPID GetOnlineExperimentView_dispid{0x60020030};
      auto             GetOnlineExperimentView = [this](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *){ 
        VariantInit(variant);
        variant->pdispVal = this;
        variant->vt = VT_DISPATCH;
        return S_OK; 
      }; 
      ON_CALL(*this, Invoke(GetOnlineExperimentView_dispid, _, _, _, _, _, _, _)).WillByDefault(GetOnlineExperimentView);
      // clang-format on
    }
  };
} // namespace

TEST_F(TC003, A)
{
  EXPECT_NO_THROW(
      {
        MockInca_Dispatch mock;
        mock.Delegate();
        pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
        pacemaker::inca::com::IncaProxy(std::move(idispatch));
      });
}

TEST_F(TC003, B)
{
  counter = 0;
  {
    MockInca_Dispatch mock;
    mock.Delegate();
    pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
    pacemaker::inca::com::IncaProxy                    inca_proxy{std::move(idispatch)};

    EXPECT_EQ(counter, 0);
  }
  EXPECT_EQ(counter, 1);
}

TEST_F(TC003, C)
{
  MockInca_Dispatch mock;
  mock.Delegate();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch(&mock);
  pacemaker::inca::com::IncaProxy                    inca_proxy(std::move(idispatch));
  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp;
  
  EXPECT_NO_THROW(
      {
        exp = inca_proxy.GetOpenedExperiment();
      });
  EXPECT_NE(exp.get(), nullptr);
}

TEST_F(TC003, D)
{
  MockInca_Dispatch mock;
  mock.Delegate();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch(&mock);
  pacemaker::inca::com::IncaProxy                    inca_proxy(std::move(idispatch));
  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview;

  EXPECT_NO_THROW(
      {
        expview = inca_proxy.GetOpenedExperimentView();
      });
  EXPECT_NE(expview.get(), nullptr);
}