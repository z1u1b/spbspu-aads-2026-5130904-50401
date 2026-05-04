#ifndef MY_NODE_HPP
#define MY_NODE_HPP

namespace zubarev
{
  template < class Key, class Value >
  struct NodeHashTable
  {
    NodeHashTable();
    NodeHashTable(Key, Value, bool);

    Key key_;
    Value value_;
    bool is_val_;
  };

}

#endif
