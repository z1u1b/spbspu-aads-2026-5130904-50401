#ifndef MY_ITER_TREE_HPP
#define MY_ITER_TREE_HPP

#include "my_citer_tree.hpp"
#include "my_node_tree.hpp"
namespace zubarev
{
  template < class Key, class Value, class Compare >
  class HashTable;

  template < class Key, class Value >
  class BSTIterator
  {
    using Node = BSTreeNode< Key, Value >;

  private:
    Node* ptr_;

  public:
    BSTIterator() = default;
    BSTIterator(Node*);

    Node& operator*() const;
    Node* operator->() const noexcept;
    BSTIterator& operator++();
    bool operator!=(const BSTIterator&) const;
    bool operator==(const BSTIterator&) const;
  };
  template < class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(Node* node):
    ptr_(node)
  {}

  template < class Key, class Value >
  BSTreeNode< Key, Value >& BSTIterator< Key, Value >::operator*() const
  {
    return ptr_;
  }

  template < class Key, class Value >
  BSTreeNode< Key, Value >* BSTIterator< Key, Value >::operator->() const noexcept
  {
    return std::addressof(operator*());
  }
  template < class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator++()
  {}
  template < class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator& rhs) const
  {
    return !(*this == rhs);
  }
  template < class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator& rhs) const
  {}

}

#endif
