#include "inca/IncaCOMAdapter.hpp"

#include "utils.hpp"

#include <stdexcept>

#import "incacom.tlb" named_guids no_namespace

inca::IncaCOMAdapter::IncaCOMAdapter()
{
  {
    Inca_Dispatch *raw{nullptr};
    HRESULT        hr = CoCreateInstance(CLSID_Inca,
                                         nullptr,
                                         CLSCTX_INPROC_SERVER,
                                         __uuidof(Inca_Dispatch),
                                         reinterpret_cast<void **>(&raw));
    if (FAILED(hr)) throw std::runtime_error("Failed to instantiate INCA COM object. Verify that INCA is installed.");
    this->p_inca = unique_com_ptr<Inca_Dispatch>(raw);
  }
  this->p_exp = query_interface<IncaOnlineExperiment_Dispatch>(this->p_inca->GetOpenedExperiment());
  this->p_exp->StopMeasurement();
  this->p_expview = query_interface<IncaExperimentView_Dispatch>(this->p_inca->GetOpenedExperimentView());
}

inca::IncaCOMAdapter::~IncaCOMAdapter()
{
  if(this->p_inca) this->p_inca->DisconnectFromTool();
}

auto inca::IncaCOMAdapter::add_param(std::string_view name) -> void
{
  throw std::logic_error("Not implemented");
}

auto inca::IncaCOMAdapter::set_param(std::string_view name, double value) -> void
{
  throw std::logic_error("Not implemented");
}

auto inca::IncaCOMAdapter::reset() -> void
{
  throw std::logic_error("Not implemented");
}

auto inca::IncaCOMAdapter::start_recording() -> void
{
  throw std::logic_error("Not implemented");
}

auto inca::IncaCOMAdapter::stop_recording(std::string_view filename) -> void
{
  throw std::logic_error("Not implemented");
}