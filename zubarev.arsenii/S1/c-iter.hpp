#ifndef CITER_HPP
#define CITER_HPP

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
  class LCIter
  {
    friend class List< T >;

  public:
    const T& operator*() const;
    const T* operator->() const;
    LCIter< T >& operator++();
    LCIter< T > operator++(int);
    bool operator!=(const LCIter&) const;
    bool operator==(const LCIter&) const;

  private:
    const detail::Node< T >* ptr;
    LCIter(const detail::Node< T >* p);
  };

  template < class T >
  LCIter< T >::LCIter(const detail::Node< T >* p):
    ptr(p)
  {}

  template < class T >
  const T& LCIter< T >::operator*() const
  {
    return ptr->val;
  }
  template < class T >

  const T* LCIter< T >::operator->() const
  {
    return std::addressof(ptr->val);
  }

  template < class T >
  LCIter< T >& LCIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }
  template < class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > tmp = this;
    ptr = ptr->next;
    return tmp;
  }

  template < class T >
  bool LCIter< T >::operator!=(const LCIter& iter) const
  {
    return ptr != iter.ptr;
  }

  template < class T >
  bool LCIter< T >::operator==(const LCIter& iter) const
  {
    return ptr == iter.ptr;
  }
}

#endif
