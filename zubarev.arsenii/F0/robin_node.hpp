#ifndef MY_ROBIN_NODE_HPP
#define MY_ROBIN_NODE_HPP

namespace zubarev
{
  template< class Key, class Value >
  struct RobinNode
  {
    // template< class, class, class, class >
    // friend class RobinHashTable;

    // public:
    // const Value& getValue() const
    // {
    //   return val_;
    // }
    // const Key& getKey() const
    // {
    //   return key_;
    // }
    RobinNode();
    RobinNode(Key, Value, bool, int);

    // private:
    Key key_;
    Value val_;
    bool occupied_ = false;
    int psl_ = -1;
  };

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode():
    key_(),
    val_(),
    occupied_(false),
    psl_(-1)
  {}

  template< class Key, class Value >
  RobinNode< Key, Value >::RobinNode(Key key, Value val, bool occupied, int psl):
    key_(key),
    val_(val),
    occupied_(occupied),
    psl_(psl)
  {}
}

#endif
