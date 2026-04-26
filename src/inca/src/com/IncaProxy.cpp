#include "inca/com/IncaProxy.hpp"

#include <stdexcept>

#include "incacom.hpp"

inca::com::IncaProxy::IncaProxy()
{
  Inca_Dispatch *raw{nullptr};
  HRESULT        hr = CoCreateInstance(CLSID_Inca,
                                       nullptr,
                                       CLSCTX_INPROC_SERVER,
                                       __uuidof(Inca_Dispatch),
                                       reinterpret_cast<void **>(&raw));
  if (FAILED(hr))
    throw std::runtime_error("Failed to instantiate INCA COM object. Verify that INCA is installed.");
  this->p_subject = unique_com_ptr<::Inca_Dispatch>(raw);
}

inca::com::IncaProxy::~IncaProxy()
{
  this->p_subject->DisconnectFromTool();
}

auto inca::com::IncaProxy::GetOpenedExperiment() -> ::IDispatch *
{
  return this->p_subject->GetOpenedExperiment();
}

auto inca::com::IncaProxy::GetOpenedExperimentView() -> ::IDispatch *
{
  return this->p_subject->GetOpenedExperimentView();
}