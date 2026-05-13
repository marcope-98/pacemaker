#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>

#include <comdef.h>

#include "pacemaker/inca/Experiment.hpp"

// clang-format off
struct IncaOnlineExperiment_Dispatch : public IDispatch { };
struct IncaExperimentView_Dispatch : public IDispatch { };
struct ExperimentDevice_Dispatch : public IDispatch { };
// clang-format on

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

namespace
{
  class MockProxy_Dispatch : public IDispatch
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
    void Delegate_GetCalibrationValueInDevice(CB GetCalibrationValueInDevice)
    {
      constexpr DISPID GetCalibrationValueInDevice_dispid{0x60020090};
      ON_CALL(*this, Invoke(GetCalibrationValueInDevice_dispid, _, _, _, _, _, _, _)).WillByDefault(GetCalibrationValueInDevice);
    }

    template<class CB>
    void Delegate_SetImplValue(CB SetImplValue)
    {
      constexpr DISPID SetImplValue_dispid{0x60020015};
      ON_CALL(*this, Invoke(SetImplValue_dispid, _, _, _, _, _, _, _)).WillByDefault(SetImplValue);
    }
  };
} // namespace

TEST(TC008, A)
{
  MockProxy_Dispatch mock_exp, mock_expview, mock_device;

  auto GetCalibrationValueInDevice = [&mock_exp](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);
    variant->pdispVal = &mock_exp;
    variant->vt       = VT_DISPATCH;
    return S_OK;
  };
  mock_exp.Delegate();
  mock_exp.Delegate_GetCalibrationValueInDevice(GetCalibrationValueInDevice);
  mock_expview.Delegate();
  mock_device.Delegate();

  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp_idispatch{&mock_exp};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(exp_idispatch));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview_idispatch{&mock_expview};
  pacemaker::inca::com::IncaExperimentViewProxy      expview{std::move(expview_idispatch)};

  pacemaker::inca::detail::unique_com_ptr<IDispatch> device_idispatch{&mock_device};
  pacemaker::inca::com::ExperimentDeviceProxy        device{std::move(device_idispatch)};

  pacemaker::inca::Experiment experiment(std::move(exp), std::move(expview), std::move(device));

  constexpr DISPID GetCalibrationValueInDevice_dispid{0x60020090};
  EXPECT_CALL(mock_exp, Invoke(GetCalibrationValueInDevice_dispid, _, _, _, _, _, _, _)).Times(1);

  experiment.add_param("speed");
  experiment.add_param("speed");
}

TEST(TC008, B)
{
  MockProxy_Dispatch mock_exp, mock_expview, mock_device;
  mock_exp.Delegate();
  mock_expview.Delegate();
  mock_device.Delegate();

  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp_idispatch{&mock_exp};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(exp_idispatch));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview_idispatch{&mock_expview};
  pacemaker::inca::com::IncaExperimentViewProxy      expview{std::move(expview_idispatch)};

  pacemaker::inca::detail::unique_com_ptr<IDispatch> device_idispatch{&mock_device};
  pacemaker::inca::com::ExperimentDeviceProxy        device{std::move(device_idispatch)};

  pacemaker::inca::Experiment experiment(std::move(exp), std::move(expview), std::move(device));
  std::string                 param{"torque"};
  EXPECT_THROW(
      {
        try
        {
          experiment.set_param(param, 50.0);
        }
        catch (const std::out_of_range &e)
        {
          EXPECT_EQ("Experiment::set_param: unknown parameter '" + param + "'. Call add_param() before set_param().", e.what());
          throw;
        }
      },
      std::out_of_range);
}

TEST(TC008, C)
{
  MockProxy_Dispatch mock_exp, mock_expview, mock_device, mock_calib;
  double             expected{50.0};
  auto               GetCalibrationValueInDevice = [&mock_exp, &mock_calib](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);
    variant->pdispVal = &mock_calib;
    variant->vt       = VT_DISPATCH;
    return S_OK;
  };
  mock_exp.Delegate();
  mock_exp.Delegate_GetCalibrationValueInDevice(GetCalibrationValueInDevice);
  mock_expview.Delegate();
  mock_device.Delegate();
  auto SetImplValue = [&mock_calib, expected](DISPID, const IID &, LCID, WORD, DISPPARAMS *dispparams, VARIANT *, EXCEPINFO *, UINT *)
  {
    EXPECT_EQ(dispparams->rgvarg[0].dblVal, expected);
    return S_OK;
  };
  mock_calib.Delegate();
  mock_calib.Delegate_SetImplValue(SetImplValue);

  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp_idispatch{&mock_exp};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(exp_idispatch));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview_idispatch{&mock_expview};
  pacemaker::inca::com::IncaExperimentViewProxy      expview{std::move(expview_idispatch)};

  pacemaker::inca::detail::unique_com_ptr<IDispatch> device_idispatch{&mock_device};
  pacemaker::inca::com::ExperimentDeviceProxy        device{std::move(device_idispatch)};

  pacemaker::inca::Experiment experiment(std::move(exp), std::move(expview), std::move(device));

  constexpr DISPID SetImplValue_dispid{0x60020015};
  EXPECT_CALL(mock_calib, Invoke(SetImplValue_dispid, _, _, _, _, _, _, _)).Times(1);

  experiment.add_param("torque");
  experiment.set_param("torque", expected);
}

TEST(TC008, D)
{
  MockProxy_Dispatch mock_exp, mock_expview, mock_device, mock_calib;
  auto               GetCalibrationValueInDevice = [&mock_exp, &mock_calib](DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *variant, EXCEPINFO *, UINT *)
  {
    VariantInit(variant);
    variant->pdispVal = &mock_calib;
    variant->vt       = VT_DISPATCH;
    return S_OK;
  };
  mock_exp.Delegate();
  mock_exp.Delegate_GetCalibrationValueInDevice(GetCalibrationValueInDevice);
  mock_expview.Delegate();
  mock_device.Delegate();
  mock_calib.Delegate();

  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp_idispatch{&mock_exp};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(exp_idispatch));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview_idispatch{&mock_expview};
  pacemaker::inca::com::IncaExperimentViewProxy      expview{std::move(expview_idispatch)};

  pacemaker::inca::detail::unique_com_ptr<IDispatch> device_idispatch{&mock_device};
  pacemaker::inca::com::ExperimentDeviceProxy        device{std::move(device_idispatch)};

  pacemaker::inca::Experiment experiment(std::move(exp), std::move(expview), std::move(device));

  constexpr DISPID ResetValueToRP_dispid{0x60020028};
  EXPECT_CALL(mock_calib, Invoke(ResetValueToRP_dispid, _, _, _, _, _, _, _)).Times(2);

  experiment.add_param("a");
  experiment.add_param("b");
  experiment.reset();
  EXPECT_THROW(experiment.set_param("a", 0.0), std::out_of_range);
}

TEST(TC008, E)
{
  MockProxy_Dispatch mock_exp, mock_expview, mock_device, mock_calib;
  mock_exp.Delegate();
  mock_expview.Delegate();
  mock_device.Delegate();
  mock_calib.Delegate();

  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp_idispatch{&mock_exp};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(exp_idispatch));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview_idispatch{&mock_expview};
  pacemaker::inca::com::IncaExperimentViewProxy      expview{std::move(expview_idispatch)};

  pacemaker::inca::detail::unique_com_ptr<IDispatch> device_idispatch{&mock_device};
  pacemaker::inca::com::ExperimentDeviceProxy        device{std::move(device_idispatch)};

  pacemaker::inca::Experiment experiment(std::move(exp), std::move(expview), std::move(device));

  InSequence seq;

  constexpr DISPID StopRecordingAndSave_dispid{0x600200a4};
  constexpr DISPID StopMeasurement_dispid{0x60020074};
  EXPECT_CALL(mock_exp, Invoke(StopRecordingAndSave_dispid, _, _, _, _, _, _, _)).Times(1);
  EXPECT_CALL(mock_exp, Invoke(StopMeasurement_dispid, _, _, _, _, _, _, _)).Times(1);

  auto begin = std::chrono::steady_clock::now();
  experiment.stop_recording("");
  auto end     = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
  EXPECT_GE(elapsed, 40);
}

TEST(TC008, F)
{
  MockProxy_Dispatch mock_exp, mock_expview, mock_device, mock_calib;
  mock_exp.Delegate();
  mock_expview.Delegate();
  mock_device.Delegate();
  mock_calib.Delegate();

  pacemaker::inca::detail::unique_com_ptr<IDispatch> exp_idispatch{&mock_exp};
  pacemaker::inca::com::IncaOnlineExperimentProxy    exp(std::move(exp_idispatch));

  pacemaker::inca::detail::unique_com_ptr<IDispatch> expview_idispatch{&mock_expview};
  pacemaker::inca::com::IncaExperimentViewProxy      expview{std::move(expview_idispatch)};

  pacemaker::inca::detail::unique_com_ptr<IDispatch> device_idispatch{&mock_device};
  pacemaker::inca::com::ExperimentDeviceProxy        device{std::move(device_idispatch)};

  pacemaker::inca::Experiment experiment(std::move(exp), std::move(expview), std::move(device));

  constexpr DISPID StartRecording_dispid{0x600200a1};
  EXPECT_CALL(mock_exp, Invoke(StartRecording_dispid, _, _, _, _, _, _, _)).Times(1);
  experiment.start_recording();
}