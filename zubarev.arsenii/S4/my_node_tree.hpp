#ifndef MY_NODE_TREE_HPP
#define MY_NODE_TREE_HPP

namespace zubarev
{

  template < class Key, class Value >
  struct BSTreeNode
  {
    using Node = BSTreeNode< Key, Value >;

    BSTreeNode();
    BSTreeNode(Key, Value);

    std::pair< Key, Value > data_;
    BSTreeNode< Key, Value >* left_;
    BSTreeNode< Key, Value >* right_;
    BSTreeNode< Key, Value >* parent_;

    void swap(BSTreeNode& rhs) noexcept;
  };
  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode():

  {}
  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode(Key key, Value val):
    data_(std::make_pair(key, val))
  {}
  template < class Key, class Value >
  void BSTreeNode< Key, Value >::swap(BSTreeNode& rhs) noexcept
  {
    std::swap(data_, rhs.data_);
    std::swap(left_, rhs.left_);
    std::swap(right_, rhs.right_);
    std::swap(parent_, rhs.parent_);
  }
};

#endif
