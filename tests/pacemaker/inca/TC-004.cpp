#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <comdef.h>

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
      ON_CALL(*this, Release()).WillByDefault(Return(0));
      ON_CALL(*this, QueryInterface(_, _)).WillByDefault([this](const IID &, void **out)
                                                         { *out = reinterpret_cast<void*>(this); return S_OK; });
    }
  };
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

    VARIANT elem0;
    VariantInit(&elem0);
    elem0.pdispVal = &mock;
    elem0.vt       = VT_DISPATCH;

    VARIANT elem1;
    VariantInit(&elem1);
    elem1.pdispVal = &mock;
    elem1.vt       = VT_DISPATCH;

    LONG index = 0;
    SafeArrayPutElement(out, &index, (void *)(&elem0));
    index = 1;
    SafeArrayPutElement(out, &index, (void *)(&elem1));

    variant->parray = out;
    variant->vt     = (VT_ARRAY | VT_VARIANT);
    return S_OK;
  };
  mock.Delegate();
  mock.Delegate_GetAllDevices(GetAllDevices);

  pacemaker::inca::detail::unique_com_ptr<IDispatch>              idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy                 exp(std::move(idispatch));
  std::vector<pacemaker::inca::detail::unique_com_ptr<IDispatch>> devices;

  EXPECT_NO_THROW(devices = exp.GetAllDevices());
  EXPECT_EQ(devices.size(), 2);
  EXPECT_NE(devices[0], nullptr);
  EXPECT_NE(devices[1], nullptr);
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

  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));
  EXPECT_THROW(auto devices = exp.GetAllDevices(), std::runtime_error);
}

TEST_F(TC004, C)
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

  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));

  const std::string param = "unknownParam";
  EXPECT_THROW(
      {
        try
        {
          exp.GetCalibrationValueInDevice(param, nullptr);
        }
        catch (const std::runtime_error &e)
        {
          EXPECT_EQ("GetCalibrationValueInDevice: parameter not found " + param, e.what());
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TC004, D)
{
  MockIncaOnlineExperiment_Dispatch mock;
  mock.Delegate();

  constexpr DISPID StartRecording_dispid{0x600200a1};
  EXPECT_CALL(mock, Invoke(StartRecording_dispid, _, _, _, _, _, _, _));
  constexpr DISPID StopRecordingAndSave_dispid{0x600200a4};
  EXPECT_CALL(mock, Invoke(StopRecordingAndSave_dispid, _, _, _, _, _, _, _));
  constexpr DISPID StopMeasurement_dispid{0x60020074};
  EXPECT_CALL(mock, Invoke(StopMeasurement_dispid, _, _, _, _, _, _, _));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> idispatch{&mock};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(idispatch));

  exp.StartRecording();
  exp.StopRecordingAndSave();
  exp.StopMeasurement();
}
