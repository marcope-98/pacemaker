#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/inca/com/IncaExperimentViewProxy.hpp"

using ::testing::_;
using ::testing::Return;

namespace
{
  class MockIncaExperimentView_Dispatch : public IDispatch
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
                                                         { *out = reinterpret_cast<void*>(this); return S_OK; });
    }

    template<class CB>
    void Delegate_OpenViewForExperimentDataItem(CB OpenViewForExperimentDataItem)
    {
      constexpr DISPID OpenViewForExperimentDataItem_dispid{0x6002000a};
      ON_CALL(*this, Invoke(OpenViewForExperimentDataItem_dispid, _, _, _, _, _, _, _)).WillByDefault(OpenViewForExperimentDataItem);
    }

    IDispatch *address{nullptr};
  };

  class MockDataItem : public IDispatch
  {
  public:
    MOCK_METHOD(ULONG, AddRef, (), (override));
    MOCK_METHOD(ULONG, Release, (), (override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT, LCID, ITypeInfo **), (override));
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *), (override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (const IID &, LPOLESTR *, UINT, LCID, DISPID *), (override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *), (override));
    MOCK_METHOD(HRESULT, QueryInterface, (const IID &, void **), (override));
  };
} // namespace

TEST(TC005, A)
{
  MockIncaExperimentView_Dispatch mock;
  auto                            OpenViewForExperimentDataItem = [&mock](DISPID, const IID &, LCID, WORD, DISPPARAMS *dispparams, VARIANT *, EXCEPINFO *, UINT *)
  {
    mock.address = (IDispatch *)(dispparams->rgvarg[0].pdispVal);
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_OpenViewForExperimentDataItem(OpenViewForExperimentDataItem);

  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaExperimentViewProxy      expview(std::move(idispatch));

  MockDataItem                                       dataitem;
  pacemaker::inca::detail::unique_com_ptr<IDispatch> dataitem_ptr{&dataitem};

  expview.OpenViewForExperimentDataItem(std::move(dataitem_ptr));

  EXPECT_EQ(mock.address, &dataitem);
}