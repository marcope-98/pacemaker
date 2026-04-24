#include "inca/IncaCOMAdapter.hpp"

#include <stdexcept>

#import "incacom.tlb" named_guids no_namespace

inca::IncaCOMAdapter::IncaCOMAdapter()
{
}

inca::IncaCOMAdapter::~IncaCOMAdapter()
{
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