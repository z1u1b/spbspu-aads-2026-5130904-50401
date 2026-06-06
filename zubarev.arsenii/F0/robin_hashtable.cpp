#include "robin_hashtable.hpp"

template < class Key, class T, class Hash, class Equal >
HashTable< Key, T, Hash, Equal >::HashTable(size_t cap = 16):
  capacity(cap),
  size(0)
{
  slots = new Node< Key, T >[capacity];
}
template < class Key, class T, class Hash, class Equal >
HashTable< Key, T, Hash, Equal >::~HashTable()
{
  delete[] slots;
}

template < class Key, class T, class Hash, class Equal >
size_t HashTable< Key, T, Hash, Equal >::erase(const Key& k)
{

  return 0;
}
template < class Key, class T, class Hash, class Equal >
size_t HashTable< Key, T, Hash, Equal >::insert(const Key& k, const T& val)
{
  size_t idx = hasher(k) % capacity;
  if (!slots[idx]) {
    slots[idx] = val;
  }

  return size;
}
