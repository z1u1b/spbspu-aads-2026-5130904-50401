#ifndef LIST_HPP
#define LIST_HPP
#include "c-iter.hpp"
#include "iter.hpp"
#include <utility>

namespace zubarev
{
  namespace detail
  {
    template < class T >
    struct Node
    {
      T val;
      Node* next;

      Node(const T& v, Node* n = nullptr):
        val(v),
        next(n) {};
    };

  }
  template < class T >
  class List
  {



  public:
    List();
    ~List() noexcept;
    List(const List& list);
    List(List&& list) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    LIter< T > begin();
    LIter< T > before_begin();
    LIter< T > end();
    LIter< T > back();

    LCIter< T > begin() const;
    LCIter< T > before_begin() const;
    LCIter< T > end() const;
    LCIter< T > back() const;

    LCIter< T > cbegin() const;
    LCIter< T > cbefore_begin() const;
    LCIter< T > cend() const;
    LCIter< T > cback() const;

    void clear();
    bool empty() const;
    void pop_front();
    void push_front(const T&);
    void push_back(const T&);
    LIter< T > insert_after(LIter< T >, const T&);
    void erase_after(LIter< T >);

  private:
    detail::Node< T >* head_;
    detail::Node< T >* tail_;
    detail::Node< T >* ctFake()
    {
      detail::Node< T >* el = new detail::Node< T >{T(), nullptr};
      return el;
    }
    void rmFake()
    {
      delete head_;
    }
  };

  template < class T >
  List< T >::List():
    head_(ctFake()),
    tail_(head_)
  {
    head_->next = nullptr;
  }

  template < class T >
  List< T >::~List() noexcept
  {
    clear();
    if (head_) {
      rmFake();
    }
  }

  template < class T >
  List< T >::List(const List< T >& other):
    head_(ctFake()),
    tail_(head_)
  {
    if (other.head_ == nullptr) {
      head_ = nullptr;
      return;
    }
    List< T > tempList;

    auto it = tempList.before_begin();

    detail::Node< T >* curOld = other.head_->next;

    while (curOld != nullptr) {
      it = tempList.insert_after(it, curOld->val);
      curOld = curOld->next;
    }


    std::swap(head_, tempList.head_);
    std::swap(tail_, tempList.tail_);

  }

  template < class T >
  List< T >::List(List< T >&& other) noexcept:
    head_(std::exchange(other.head_, nullptr)),
    tail_(std::exchange(other.tail_, nullptr))
  {}
  template < class T >
  List< T >& List< T >::operator=(const List& other)
  {

    if (this != &other) {
      List< T > temp(other);
      std::swap(head_, temp.head_);
      std::swap(tail_, temp.tail_);
    }
    return *this;
  }

  template < class T >
  List< T >& List< T >::operator=(List&& other) noexcept
  {
    if (this == &other) {
      return *this;
    }
    clear();
    if (head_) {
      rmFake();
    }
head_ = std::exchange(other.head_, nullptr);
tail_ = std::exchange(other.tail_, nullptr);


    return *this;
  }
  template < class T >
  LIter< T > List< T >::before_begin()
  {
    if (!head_) {
      return end();
    }
    return LIter< T >(head_);
  }
  template < class T >
  LIter< T > List< T >::begin()
  {
    if (!head_) {
      return end();
    }
    return LIter< T >(head_->next);
  }

  template < class T >
  LIter< T > List< T >::end()
  {
    return LIter< T >(nullptr);
  }
  template < class T >
  LIter< T > List< T >::back()
  {
    if (empty()) {
  return end();
}
    return LIter< T >(tail_);
  }

  template < class T >
  LCIter< T > List< T >::before_begin() const
  {
        if (!head_) {
  return end();
}
    return LCIter< T >(head_);
  }
  template < class T >
  LCIter< T > List< T >::begin() const
  {
    if (!head_) {
  return end();
}
    return LCIter< T >(head_->next);
  }

  template < class T >
  LCIter< T > List< T >::end() const
  {
    return LCIter< T >(nullptr);
  }
  template < class T >
  LCIter< T > List< T >::back() const
  {
if (empty()) {
  return end();
}
    return LCIter< T >(tail_);
  }

  template < class T >
  LCIter< T > List< T >::cbefore_begin() const
  {
    return before_begin();
  }
  template < class T >
  LCIter< T > List< T >::cbegin() const
  {
    return begin();
  }

  template < class T >
  LCIter< T > List< T >::cend() const
  {
    return end();
  }
  template < class T >
  LCIter< T > List< T >::cback() const
  {
    return back();
  }

  template < class T >
  void List< T >::clear()
  {
    if (!head_) {
      return;
    }
    detail::Node< T >* cur = begin().ptr;
    while (cur != end().ptr) {
      detail::Node< T >* curNext = cur->next;
      delete cur;
      cur = curNext;
    }

    head_->next = nullptr;
    tail_ = head_;
  }

  template < class T >
  bool List< T >::empty() const
  {
    if (!head_) {
      return true;
    }
    return head_->next == nullptr;
  }

  template < class T >
  void List< T >::push_front(const T& val)
  {
    if (!head_) {
      head_ = ctFake();
      tail_ = head_;
    }
    detail::Node< T >* newNode = new detail::Node< T >(val, head_->next);
    head_->next = newNode;
    if (tail_ == head_) {
      tail_ = newNode;
    }
  }

template < class T >
void List< T >::push_back(const T& val)
{
  if (!head_) {
    head_ = ctFake();
    tail_ = head_;
  }

  detail::Node< T >* newNode = new detail::Node< T >(val, nullptr);

  tail_->next = newNode;
  tail_ = newNode;
}

  template < class T >
  void List< T >::pop_front()
  {
    if (empty() || !head_) {
      return;
    }
    detail::Node< T >* toDel = head_->next;
    head_->next = toDel->next;
    if (tail_ == toDel) {
      tail_ = head_;
    }
    delete toDel;
  }

  template < class T >
  LIter< T > List< T >::insert_after(LIter< T > it, const T& val)
  {
    if (!it.ptr) {
      return end();
    }
    detail::Node< T >* itNext = it.ptr->next;
    detail::Node<T>* newNode = new detail::Node<T>(val, itNext);
it.ptr->next = newNode;

if (tail_ == it.ptr)
{
  tail_ = newNode;
}
    return LIter< T >(it.ptr->next);
  }

  template < class T >
  void List< T >::erase_after(LIter< T > it)
  {
    if (!it.ptr || !it.ptr->next) {
      return;
    }
    detail::Node< T >* itNext = it.ptr->next;
    it.ptr->next = itNext->next;
        if (tail_ == itNext)
{
  tail_ = it.ptr;
}
    delete itNext;

  }
}
#endif
