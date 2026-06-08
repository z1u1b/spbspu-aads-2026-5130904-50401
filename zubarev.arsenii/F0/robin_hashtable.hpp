#ifndef ROBIN_HASHTABLE_HPP
#define ROBIN_HASHTABLE_HPP

#include <cstddef>
#include <ut>
#include "robin_node.hpp"
#include "../common/top-it-vector.hpp"
#include "robin_iter.hpp"
#include "robin_citer.hpp"
namespace zubarev
{
  template< class Key, class Value, class Hash, class Equal >
  class RobinHashTable
  {
    friend class RobinIter< Key, Value, Hash, Equal >;
    friend class RobinCIter< Key, Value, Hash, Equal >;

    using Node = RobinNode< Key, Value >;
    using Iter = RobinIter< Key, Value, Hash, Equal >;
    using CIter = RobinCIter< Key, Value, Hash, Equal >;
    using Table = RobinHashTable< Key, Value, Hash, Equal >;

  public:
    RobinHashTable();
    RobinHashTable(size_t cap = 16);
    ~RobinHashTable() = default;
    RobinHashTable(const RobinHashTable& table);
    RobinHashTable(RobinHashTable&& table) noexcept;
    RobinHashTable& operator=(const RobinHashTable& other);
    RobinHashTable& operator=(RobinHashTable&& other) noexcept;

    Value& operator[](const Key& k);
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

    Node* find_el(const Key& k) noexcept
    {
      if (empty()) {
        return nullptr;
      }

      size_t index = hasher_(k) % capacity_;
      int cur_psl = 0;

      for (size_t i = 0; i < capacity_; ++i) {
        const Node& cur_node = slots_[index];
        if (cur_node.psl == -1) {
          return nullptr;
        }

        if (cur_psl > cur_node.psl) {
          return nullptr;
        }
        if (equal_(k, cur_node.k)) {
          return std::addressof(slots_[index]);
        }
        index = (index + 1) % capacity_;
        cur_psl++;
      }
      return nullptr;
    }

    const Node* find_el(const Key& k) const noexcept
    {
      if (empty()) {
        return nullptr;
      }

      size_t index = hasher_(k) % capacity_;
      int cur_psl = 0;

      for (size_t i = 0; i < capacity_; ++i) {
        const Node& cur_node = slots_[index];
        if (cur_node.psl == -1) {
          return nullptr;
        }

        if (cur_psl > cur_node.psl) {
          return nullptr;
        }
        if (equal_(k, cur_node.k)) {
          return std::addressof(slots_[index]);
        }
        index = (index + 1) % capacity_;
        cur_psl++;
      }
      return nullptr;
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable():
    size_(0),
    capacity_(8),
    slots_(topit::Vector< Node >(8)),
    hasher_(),
    equal_()
  {}

  // template < class Key, class Value, class Hash, class Equal >
  // RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(
  //     size_t size, size_t capacity, topit::Vector< Node > slots, Hash hasher, Equal equal):
  //   size_(size),
  //   capacity_(capacity),
  //   slots_(slots),
  //   hasher_(hasher),
  //   equal_(equal)
  // {}
  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(size_t capacity):
    size_(0),
    capacity_(capacity),
    slots_(topit::Vector< Node >(capacity)),
    hasher_(),
    equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(const RobinHashTable& table):
    size_(table.size_),
    capacity_(table.capacity_),
    slots_(),
    hasher_(table.hasher_),
    equal_(table.equal_)
  {
    slots_ = topit::Vector< Node >(table.capacity_);
    for (size_t i = 0; i < table.size_; ++i) {
      slots_[i] = table.slots_[i];
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(RobinHashTable&& table) noexcept:
    size_(table.size_),
    capacity_(table.capacity_),
    slots_(table.slots_),
    hasher_(table.hasher_),
    equal_(table.equal_)
  {
    table.slots_ = topit::Vector< Node >();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >&
  RobinHashTable< Key, Value, Hash, Equal >::operator=(const RobinHashTable& rhs)
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >&
  RobinHashTable< Key, Value, Hash, Equal >::operator=(RobinHashTable&& rhs) noexcept
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    Table cpy(std::move(rhs));
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& RobinHashTable< Key, Value, Hash, Equal >::operator[](const Key& k)
  {
    Node* el = find_el(k);
    if (el) {
      return el->val_;
    } else {
      add(k, Value{});
      return Value{};
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value& RobinHashTable< Key, Value, Hash, Equal >::at(const Key& id)
  {
    Node* el = find_el(id);
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("RobinHashTable: index out of range");
    }
  }
  template< class Key, class Value, class Hash, class Equal >
  const Value& RobinHashTable< Key, Value, Hash, Equal >::at(const Key& id) const
  {
    const Node* el = find_el(id);
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("RobinHashTable: index out of range");
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::swap(Table& rhs) noexcept
  {
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(slots_, rhs.slots_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(equal_, rhs.equal_);
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::begin()
  {
    size_t cap = capacity();
    for (size_t i = 0; i < cap; ++i) {
      if (slots_[i].occupied) {
        return Iter(i, this);
      }
    }
    return end();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter(capacity_, this);
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    size_t cap = capacity();
    for (size_t i = 0; i < cap; ++i) {
      if (slots_[i].occupied) {
        return CIter(i, this);
      }
    }
    return cend();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::cend() const
  {
    return CIter(capacity_, this);
  }

  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::begin() const
  {
    return cbegin();
  }
  template< class Key, class Value, class Hash, class Equal >
  RobinCIter< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::end() const
  {
    return cend();
  }

  template< class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::add(const Key& k, Value v)
  {
    if (load_factor() >= 0.75) {
      rehash(capacity_ * 2);
    }

    size_t index = hasher_(k) % capacity_;
    size_t cur_psl = 0;

    Node node_to_add(k, v, false, 0);

    for (size_t i = 0; i < capacity_; ++i) {
      if (!slots_[index].occupied) {
        slots_[index] = node_to_add;
        size_++;
        return;
      }

      if (equal_(slots_[index].key, node_to_add.key)) {
        slots_[index].val_ = node_to_add.val_;
        return;
      }

      if (node_to_add.psl > slots_[index].psl) {
        std::swap(node_to_add, slots_[index]);
      }

      node_to_add.psl_++;
      index = (index + 1) % capacity_;
    }
    throw std::runtime_error("RobinHashTable overflow: rehash failed or capacity logic error");
  }
  template< class Key, class Value, class Hash, class Equal >
  Value RobinHashTable< Key, Value, Hash, Equal >::drop(const Key& k)
  {
    Node* node_to_del=find_el(k);
    if (node_to_del) {
      size_t cur_id=get_el_idx(node_to_del);
      Value saved_val=node_to_del->val;


      size_t next_id=(cur_id+1)%capacity_;
      while (slots_[index].occupaied && slots_[index].psl!=0) {
        slots_[cur_id]=slots_[next_id];
        slots_[cur_id].psl--;

        cur_id=next_id;
        next_id=(next_id+1)%capacity_;
      }

      slots_[cur_id].occupied=false;
      slots_[cur_id].psl=-1;
      size_--;
      return saved_val;
    }
    throw std::out_of_range("Key not found in drop()");
  }
  template< class Key, class Value, class Hash, class Equal >
  bool RobinHashTable< Key, Value, Hash, Equal >::has(Key k) const
  {

    size_t buc_idx = getBucketIndex(k);

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
      if (equaler_(k, (*it).key_) && (*it).is_val_) {
        return true;
      }
    }

    for (size_t i = 0; i < bucket_capacity_; ++i) {
      size_t idx = bucket_capacity_ * buc_idx + i;
      if (equaler_(data_[idx].key_, k) && data_[idx].is_val_) {

        return true;
      }
    }

    return false;
  }
  template< class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    Table tmp(slots,
              bucket_capacity_,
              new Node[slots * bucket_capacity_],
              new size_t[slots],
              OverflowList(),
              hasher_,
              equaler_);
    for (size_t i = 0; i < bucket_count_; ++i) {
      for (size_t j = 0; j < bucket_capacity_; ++j) {
        size_t idx = bucket_capacity_ * i + j;
        if (data_[idx].is_val_) {
          tmp.add(data_[idx].key_, data_[idx].val_);
        }
      }
    }

    for (auto it = overflow_bucket_.begin(); it != overflow_bucket_.end(); ++it) {
      if ((*it).is_val_) {
        tmp.add((*it).key_, (*it).val_);
      }
    }
    swap(tmp);
  }
}

#endif
