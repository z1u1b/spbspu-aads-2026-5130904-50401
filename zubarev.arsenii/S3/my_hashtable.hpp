#ifndef MY_HASHTABLE_HPP
#define MY_HASHTABLE_HPP

#include "../common/list.hpp"
#include "my_iter_hashtable.hpp"
#include "my_node_hashtable.hpp"
#include "my_siphash.hpp"
#include <iostream>
namespace zubarev
{
  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  private:
    using Node = NodeHashTable< Key, Value >;
    using OverflowList = List< Node >;
    using OverflowIter = LIter< Node >;
    using Iter = IterHashTable< Key, Value, Hash, Equal >;
    using Table = HashTable< Key, Value, Hash, Equal >;

    size_t bucket_count_;
    size_t bucket_capacity_;

    Node* data_;
    size_t* sizes_;
    OverflowList overlow_bucket_;
    Hash hasher_;
    Equal equaler_;

    size_t getBucketIndex(const Key& k)
    {
      return static_cast< size_t >(hasher_(k)) % bucket_count_;
    }

  public:
    HashTable();
    HashTable(size_t, size_t, Node*, size_t*, OverflowList, Hash, Equal);
    ~HashTable();
    HashTable(const HashTable& table);
    HashTable(HashTable&& table) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
    void swap(Table& rhs) noexcept;

    Iter begin();
    Iter end();

    CIter cbegin() const;
    CIter cend() const;
  };

}

#endif
