#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>
#include <propvarutil.h>
#include <type_traits>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/com/IncaProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

using ::testing::_;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC003)

namespace
{
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
      // clang-format on
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

TEST_F(TC003, A)
{
  MockInca_Dispatch mock;
  mock.Delegate();

  EXPECT_CALL(mock, Release()).Times(4);
  EXPECT_CALL(mock, AddRef()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(2);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaProxy                    proxy{std::move(idispatch)};
  EXPECT_NO_THROW(
      {
        auto exp = proxy->GetOpenedExperiment();
        EXPECT_NE(exp.get(), nullptr);
      });
}

TEST_F(TC003, B)
{
  MockInca_Dispatch mock;
  mock.Delegate();

  EXPECT_CALL(mock, Release()).Times(4);
  EXPECT_CALL(mock, AddRef()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(2);
  EXPECT_CALL(mock, Invoke(_, _, _, _, _, _, _, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaProxy                    proxy{std::move(idispatch)};
  EXPECT_NO_THROW(
      {
        auto expview = proxy->GetOpenedExperimentView();
        EXPECT_NE(expview.get(), nullptr);
      });
}

TEST_F(TC003, C)
{
  EXPECT_TRUE(is_COMProxy_v<pacemaker::inca::com::IncaProxy>);
}