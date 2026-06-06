#include <iostream>

template < class Key, class T, class Hash, class Equal >
class HashTable
{

  size_t size;
  size_t capacity;
  Node< Key, T >* slots;
  Hash hasher;
  Equal equal;

public:
  HashTable(size_t cap = 16);
  ~HashTable();
  size_t erase(const Key& k);
  size_t insert(const Key& k, const T& val);

private:
  void rehash();
};
