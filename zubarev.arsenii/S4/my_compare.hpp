#ifndef MY_EQUAL_HPP
#define MY_EQUAL_HPP
namespace zubarev
{
  template < class T >
  struct Comparator
  {
    bool operator()(const T& a, const T& b)
    {
      return a < b;
    }
  };
}
#endif
