#ifndef MY_ROBIN_NODE_HPP
#define MY_ROBIN_NODE_HPP

namespace zubarev
{
  template < class Key, class Value >
  class NodeRobin
  {
    NodeRobin();
    NodeRobin(Key, Value, bool, int);

    Value val_;
    Key key_;
    bool occupied_ = false;
    int psl_ = -1;
  };

  template < class Key, class Value >
  NodeRobin< Key, Value >::NodeRobin():
    key_(),
    val_(),
    occupied_(false),
    psl_(-1)
  {}

  template < class Key, class Value >
  NodeRobin< Key, Value >::NodeRobin(Key key, Value val, bool occupied, int psl):
    key_(key),
    val_(val),
    occupied_(occupied),
    psl_(psl)
  {}
}

#endif
