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
  };
  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode():

  {}
  template < class Key, class Value >
  BSTreeNode< Key, Value >::BSTreeNode(Key key, Value val):
    data_(std::make_pair(key, val))
  {}

};

#endif
