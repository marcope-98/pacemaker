#ifndef INCA_DETAIL_UTILS_HPP_
#define INCA_DETAIL_UTILS_HPP_

#include <stdexcept>
#include <typeinfo>

#include <comdef.h>

#include "inca/detail/unique_com_ptr.hpp"

namespace inca::detail
{
  template<class T>
  [[nodiscard]] auto query_interface(inca::detail::unique_com_ptr<::IDispatch> src) -> inca::detail::unique_com_ptr<T>
  {
    if (src == nullptr)
      throw std::invalid_argument("Attempt on calling QueryInterface on nullptr");

    T      *out{nullptr};
    HRESULT hr = src->QueryInterface(__uuidof(T), reinterpret_cast<void **>(&out));

    if (FAILED(hr))
      throw std::runtime_error(
          std::string("QueryInterface failed for ") + typeid(T).name());

    return inca::detail::unique_com_ptr<T>(out);
  }
} // namespace inca

#endif // INCA_DETAIL_UTILS_HPP_