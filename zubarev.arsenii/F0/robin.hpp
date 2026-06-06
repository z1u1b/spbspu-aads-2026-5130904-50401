
#include <cstddef>
#include "robin_node.hpp"
namespace zubarev
{
  template < class Key, class T, class Hash, class Equal >
  class RobinHashTable
  {

    size_t size;
    size_t capacity;
    Robin< Key, T >* slots;
    Hash hasher;
    Equal equal;

  public:
    RobinHashTable(size_t cap = 16);
    ~RobinHashTable();
    size_t erase(const Key& k);
    size_t insert(const Key& k, const T& val);

  private:
    void rehash();
  };

}
template < class Key, class T, class Hash, class Equal >
class HashTable
{

  size_t size;
  size_t capacity;
  Rob< Key, T >* slots;
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
