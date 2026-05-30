#ifndef ITER_HPP
#define ITER_HPP
#include <memory>
namespace zubarev

{
  template < class T >
  class List;
  namespace detail
  {
    template < class T >
    struct Node;
  }
  template < class T >
  class LIter
  {
    friend class List< T >;

  public:
    T& operator*();
    T* operator->();
    LIter< T >& operator++();
    LIter< T > operator++(int);

    bool operator!=(const LIter&) const;
    bool operator==(const LIter&) const;

  private:
    detail::Node< T >* ptr;
    LIter(detail::Node< T >* p);
  };
  template < class T >
  LIter< T >::LIter(detail::Node< T >* p):
    ptr(p)
  {}

  template < class T >
  T& LIter< T >::operator*()
  {
    return ptr->val;
  }
  template < class T >
  T* LIter< T >::operator->()
  {
    return std::addressof(ptr->val);
  }

  template < class T >
  LIter< T >& LIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }
  template < class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > tmp = this;
    ptr = ptr->next;
    return tmp;
  }
  template < class T >
  bool LIter< T >::operator!=(const LIter& iter) const
  {
    return ptr != iter.ptr;
  }
  template < class T >
  bool LIter< T >::operator==(const LIter& iter) const
  {
    return ptr == iter.ptr;
  }
}

#endif
