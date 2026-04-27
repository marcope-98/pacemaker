#ifndef INCA_UTILS_HPP_
#define INCA_UTILS_HPP_

#include <stdexcept>

#include <comdef.h>

#include "inca/unique_com_ptr.hpp"

namespace inca
{
  template<class T>
  auto query_interface(inca::unique_com_ptr<::IDispatch> src) -> unique_com_ptr<T>
  {
    if (src == nullptr)
      throw std::invalid_argument("Attempt on calling QueryInterface on nullptr");

    T      *out{nullptr};
    HRESULT hr = src->QueryInterface(__uuidof(T), reinterpret_cast<void **>(&out));

    if (FAILED(hr))
      throw std::runtime_error("QueryInterface failed.");

    return unique_com_ptr<T>(out);
  }
} // namespace inca

#endif // INCA_UTILS_HPP_