#ifndef INCA_DETAIL_UNIQUE_COM_PTR_HPP_
#define INCA_DETAIL_UNIQUE_COM_PTR_HPP_

#include <memory>

namespace inca::detail
{
  template<class T>
  struct ComObjDeleter
  {
    void operator()(T *ptr)
    {
      if (ptr == nullptr) return;
      ptr->Release();
    }
  };

  template<class T>
  using unique_com_ptr = std::unique_ptr<T, ComObjDeleter<T>>;

}; // namespace inca

#endif // INCA_DETAIL_UNIQUE_COM_PTR_HPP_