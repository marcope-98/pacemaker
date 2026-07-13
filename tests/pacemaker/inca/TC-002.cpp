#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

#include "pacemaker/fixture/LeakTestFixture.hpp"

#include "pacemaker/inca/detail/COMProxy.hpp"
#include "pacemaker/inca/detail/unique_com_ptr.hpp"

using ::testing::_;
using ::testing::Return;

PACEMAKER_FIXTURE_INIT(TC002)

namespace
{
  struct MockCOM : public IDispatch
  {
    using type = IDispatch;

    MOCK_METHOD(ULONG, AddRef, (), (override));
    MOCK_METHOD(ULONG, Release, (), (override));
    MOCK_METHOD(HRESULT, GetTypeInfo, (UINT, LCID, ITypeInfo **), (override));
    MOCK_METHOD(HRESULT, GetTypeInfoCount, (UINT *), (override));
    MOCK_METHOD(HRESULT, GetIDsOfNames, (const IID &, LPOLESTR *, UINT, LCID, DISPID *), (override));
    MOCK_METHOD(HRESULT, Invoke, (DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *), (override));
    MOCK_METHOD(HRESULT, QueryInterface, (const IID &, void **), (override));

    MOCK_METHOD(void, func, ());

    virtual ~MockCOM() = default;

    template<class F>
    void Delegate_QueryInterface(const F &QueryInterface)
    {
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault(QueryInterface);
    }
  };

  using ConcreteProxy = pacemaker::inca::detail::COMProxy<MockCOM>;
} // namespace

TEST_F(TC002, A)
{
  EXPECT_FALSE(std::is_copy_constructible_v<ConcreteProxy>);
  EXPECT_TRUE(std::is_move_constructible_v<ConcreteProxy>);
  EXPECT_FALSE(std::is_copy_assignable_v<ConcreteProxy>);
  EXPECT_TRUE(std::is_move_assignable_v<ConcreteProxy>);
}

TEST_F(TC002, B)
{
  MockCOM mock;
  mock.Delegate_QueryInterface([&mock](const IID &, void **out)
                               {*out = reinterpret_cast<void*>(&mock); return S_OK; });
  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);

  EXPECT_CALL(mock, func()).Times(1);

  mock.AddRef();
  ConcreteProxy proxy{&mock};
  EXPECT_NO_THROW(proxy->func());
}

TEST_F(TC002, C)
{
  ConcreteProxy proxy{};
  EXPECT_EQ(proxy.get(), nullptr);
}

TEST_F(TC002, D)
{
  MockCOM mock;
  mock.Delegate_QueryInterface([&mock](const IID &, void **out)
                               {*out = reinterpret_cast<void*>(&mock); return S_OK; });
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);

  mock.AddRef();
  IDispatch *idispatch = &mock;
  EXPECT_NO_THROW(ConcreteProxy proxy{idispatch});
}

TEST_F(TC002, E)
{
  MockCOM mock;
  mock.Delegate_QueryInterface([&mock](const IID &, void **out)
                               {*out = reinterpret_cast<void*>(&mock); return S_OK; });
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);
  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  EXPECT_NO_THROW(ConcreteProxy proxy{std::move(idispatch)});
}

TEST_F(TC002, F)
{
  MockCOM mock;
  mock.Delegate_QueryInterface([&mock](const IID &, void **out)
                               {*out = reinterpret_cast<void*>(&mock); return S_OK; });
  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(2);

  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  ConcreteProxy                                      proxy{std::move(idispatch)};
}

TEST_F(TC002, G)
{
  MockCOM mock;
  mock.Delegate_QueryInterface([&mock](const IID &, void **out)
                               {*out = reinterpret_cast<void*>(&mock); return E_NOINTERFACE; });
  EXPECT_CALL(mock, AddRef()).Times(1);
  EXPECT_CALL(mock, Release()).Times(1);
  EXPECT_CALL(mock, QueryInterface(_, _)).Times(1);

  mock.AddRef();
  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  EXPECT_THROW(ConcreteProxy proxy{std::move(idispatch)}, std::runtime_error);
}

TEST_F(TC002, H)
{
  EXPECT_THROW(ConcreteProxy proxy{nullptr}, std::invalid_argument);
}