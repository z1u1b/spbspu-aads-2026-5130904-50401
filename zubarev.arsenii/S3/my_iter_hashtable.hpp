#ifndef MY_ITER_HPP
#define MY_ITER_HPP

#include "../common/iter.hpp"
#include "my_node_hashtable.hpp"
#include <iostream>
namespace zubarev
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class IterHashTable
  {

  private:
    using Node = NodeHashTable< Key, Value >;
    using OverflowIter = LIter< Node >;
    using Table = HashTable< Key, Value, Hash, Equal >;

    size_t element_index_;
    size_t bucket_index_;
    OverflowIter overflow_el_;
    const Table* table_;

  public:
    IterHashTable() = default;
    IterHashTable(size_t, size_t, OverflowIter, const Table*);

    Node& operator*();
    IterHashTable& operator++();
    bool operator!=(const IterHashTable&) const;
    bool operator==(const IterHashTable&) const;
  };
  

}

#endif
