#ifndef MY_EQUAL_HPP
#define MY_EQUAL_HPP
namespace zubarev
{
  template < class T >
  struct Equaler
  {
    bool operator()(T& a, T& b) noexcept const
    {
      return a == b;
    }
  };
}
#endif
