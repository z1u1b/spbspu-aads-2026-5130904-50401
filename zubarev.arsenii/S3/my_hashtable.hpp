#ifndef MY_HASHTABLE_HPP
#define MY_HASHTABLE_HPP

template < class Key, class Value, class Hash, class Equal >
class HashTable
{
private:

public:
  void add(Key k, Value v);
  Value drop(Key k);
  bool has(Key k);
  void rehash(size_t slots);
};

#endif
