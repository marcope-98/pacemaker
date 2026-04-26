#ifndef INCA_TRAITS_NONMOVABLE_HPP_
#define INCA_TRAITS_NONMOVABLE_HPP_

namespace inca::traits
{
  struct NonMovable
  {
    NonMovable()                              = default;
    NonMovable(const NonMovable &)            = default;
    NonMovable(NonMovable &&)                 = delete;
    NonMovable &operator=(const NonMovable &) = default;
    NonMovable &operator=(NonMovable &&)      = delete;
    ~NonMovable()                             = default;
  };
} // namespace inca::traits

#endif // INCA_TRAITS_NONMOVABLE_HPP_