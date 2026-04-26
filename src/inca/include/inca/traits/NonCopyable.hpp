#ifndef INCA_TRAITS_NONCOPYABLE_HPP_
#define INCA_TRAITS_NONCOPYABLE_HPP_

namespace inca::traits
{
  struct NonCopyable
  {
    NonCopyable()                               = default;
    NonCopyable(const NonCopyable &)            = delete;
    NonCopyable(NonCopyable &&)                 = default;
    NonCopyable &operator=(const NonCopyable &) = delete;
    NonCopyable &operator=(NonCopyable &&)      = default;
    ~NonCopyable()                              = default;
  };
} // namespace inca::traits

#endif // INCA_TRAITS_NONCOPYABLE_HPP_