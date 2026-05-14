#ifndef MY_TREE_HPP
#define MY_TREE_HPP

#include "my_citer_tree.hpp"
#include "my_iter_tree.hpp"
#include "my_node_tree.hpp"
#include <cstddef>

namespace zubarev
{

  template < class Key, class Value, class Compare >
  class BSTree
  {
    friend class BSTIterator< Key, Value, Compare >;
    friend class BSTConstIterator< Key, Value, Compare >;

  private:
    BSTreeNode< Key, Value > fake_root_;

  public:
    BSTree();
    BSTree(BSTreeNode< Key, Value >);
    ~BSTree();
    BSTree(const BSTree& table);
    BSTree(BSTree&& table) noexcept;
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    Value& operator[](Key k) noexcept;
    const Value& operator[](Key id) const noexcept;
    Value& at(Key id);
    const Value& at(Key id) const;

    //...
    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);

    //...

    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    //...

    size_t height(const_iterator it);
    size_t height();
    //...
  };

}

#endif
