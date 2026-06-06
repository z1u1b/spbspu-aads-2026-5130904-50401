
#include <cstddef>
#include "robin_node.hpp"
#include "../common/top-it-vector.hpp"
#include "robin_iter.hpp"
#include "robin_citer.hpp"
namespace zubarev
{
  template < class Key, class Value, class Hash, class Equal >
  class RobinHashTable
  {
    friend class IterHashTable< Key, Value, Hash, Equal >;
    friend class CIterHashTable< Key, Value, Hash, Equal >;

    using Node = RobinNode< Key, Value >;
    using Iter = IterRobinHashTable< Key, Value, Hash, Equal >;
    using CIter = CIterRobinHashTable< Key, Value, Hash, Equal >;
    using Table = RobinHashTable< Key, Value, Hash, Equal >;

  public:
    RobinHashTable(size_t cap = 16);
    ~RobinHashTable();
    RobinHashTable(const RobinHashTable& table);
    RobinHashTable(RobinHashTable&& table) noexcept;
    RobinHashTable& operator=(const RobinHashTable& other);
    RobinHashTable& operator=(RobinHashTable&& other) noexcept;

    Value& operator[](const Key& k) noexcept;
    const Value& operator[](const Key& id) const noexcept;
    Value& at(const Key& id);
    const Value& at(const Key& id) const;

    void swap(Table& rhs) noexcept;

    void add(const Key& k, Value v);
    Value drop(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t slots);

    Iter begin();
    Iter end();

    CIter cbegin() const;
    CIter cend() const;

    CIter begin() const;
    CIter end() const;

    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    double load_factor() const;

  private:
    size_t size_;
    size_t capacity_;
    topit::Vector< Node > slots_;
    Hash hasher_;
    Equal equal_;
  };

}
