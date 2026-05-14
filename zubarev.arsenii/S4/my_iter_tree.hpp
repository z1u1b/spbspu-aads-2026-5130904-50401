#ifndef MY_EQUAL_HPP
#define MY_EQUAL_HPP

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
    IterHashTable() = default;
    IterHashTable(Node*);

    Node& operator*() const;
    Node* operator->() const noexcept;
    BSTIterator& operator++();
    bool operator!=(const BSTIterator&) const;
    bool operator==(const BSTIterator&) const;
  };

}

#endif
