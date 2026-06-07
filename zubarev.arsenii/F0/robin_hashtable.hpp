#ifndef ROBIN_HASHTABLE_HPP
#define ROBIN_HASHTABLE_HPP

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
    friend class RobinIter< Key, Value, Hash, Equal >;
    friend class RobinCIter< Key, Value, Hash, Equal >;

    using Node = RobinNode< Key, Value >;
    using Iter = RobinIter< Key, Value, Hash, Equal >;
    using CIter = RobinCIter< Key, Value, Hash, Equal >;
    using Table = RobinHashTable< Key, Value, Hash, Equal >;

  public:
    RobinHashTable();
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

  template < class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable():
    size_(8),
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
  template < class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(size_t capacity):
    size_(capacity),
    capacity_(capacity),
    slots_(topit::Vector< Node >(capacity)),
    hasher_(),
    equal_()
  {}

  template < class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::~RobinHashTable()
  {
    delete[] slots_;
  }
  template < class Key, class Value, class Hash, class Equal >
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
  template < class Key, class Value, class Hash, class Equal >
  RobinHashTable< Key, Value, Hash, Equal >::RobinHashTable(RobinHashTable&& table) noexcept:
    size_(table.size_),
    capacity_(table.capacity_),
    slots_(table.slots_),
    hasher_(table.hasher_),
    equal_(table.equal_)
  {
    table.slots_ = topit::Vector< Node >();
  }
  template < class Key, class Value, class Hash, class Equal >
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

  template < class Key, class Value, class Hash, class Equal >
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

  template < class Key, class Value, class Hash, class Equal >
  Value& RobinHashTable< Key, Value, Hash, Equal >::operator[](const Key& k) noexcept
  {
    Node* el = find_el(k);
    if (el) {
      return el->val_;
    } else {
      add(k, Value{});
      return find_el(k)->val_;
    }
  }
  template < class Key, class Value, class Hash, class Equal >
  const Value& RobinHashTable< Key, Value, Hash, Equal >::operator[](Key id) const noexcept
  {
    return at(id);
  }

  template < class Key, class Value, class Hash, class Equal >
  Value& RobinHashTable< Key, Value, Hash, Equal >::at(Key id)
  {
    Node* el = find_el(id);
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("RobinHashTable: index out of range");
    }
  }
  template < class Key, class Value, class Hash, class Equal >
  const Value& RobinHashTable< Key, Value, Hash, Equal >::at(Key id) const
  {
    const Node* el = find_el(id);
    if (el) {
      return el->val_;
    } else {
      throw std::out_of_range("RobinHashTable: index out of range");
    }
  }

  template < class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::swap(Table& rhs) noexcept
  {
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(slots_, rhs.slots_);
    std::swap(hasher_, rhs.hasher_);
    std::swap(equal_, rhs.equal_);
  }
  template < class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::begin()
  {
    Iter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val_) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return end();
    }

    return it;
  }
  template < class Key, class Value, class Hash, class Equal >
  IterHashTable< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::end()
  {
    return Iter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    CIter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val_) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return cend();
    }

    return it;
  }
  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::cend() const
  {
    return CIter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::begin() const
  {
    CIter it(0, 0, overflow_bucket_.begin(), this);

    while (it.bucket_index_ < bucket_count_ && it.element_index_ < bucket_capacity_ &&
           !data_[it.bucket_index_ * bucket_capacity_ + it.element_index_].is_val_) {
      ++it;
      if (it.is_in_overflow())
        break;
    }

    if (it.is_in_overflow() && it.overflow_el_ == overflow_bucket_.end()) {
      return cend();
    }

    return it;
  }
  template < class Key, class Value, class Hash, class Equal >
  CIterHashTable< Key, Value, Hash, Equal > RobinHashTable< Key, Value, Hash, Equal >::end() const
  {
    return CIter(0, bucket_count_, overflow_bucket_.end(), this);
  }

  template < class Key, class Value, class Hash, class Equal >
  void RobinHashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
  {
    Table tmp(*this);
    size_t buc_idx = getBucketIndex(k);
    size_t is_over = true;
    for (size_t i = 0; i < bucket_capacity_; ++i) {
      if (!tmp.data_[bucket_capacity_ * buc_idx + i].is_val_) {
        tmp.data_[bucket_capacity_ * buc_idx + i] = Node(k, v, true);
        tmp.sizes_[buc_idx]++;
        is_over = false;
        break;
      }
    }
    if (is_over) {
      for (auto it = tmp.overflow_bucket_.begin(); it != tmp.overflow_bucket_.end(); ++it) {
        if ((*it).is_val_ && equaler_((*it).key_, k)) {
          (*it).val_ = v;
          is_over = false;
          break;
        }
      }
    }
    if (is_over) {
      tmp.overflow_bucket_.push_back(Node(k, v, true));
    }
    swap(tmp);
  }
  template < class Key, class Value, class Hash, class Equal >
  Value RobinHashTable< Key, Value, Hash, Equal >::drop(Key k)
  {
    Table tmp(*this);
    Value val;
    size_t buc_idx = getBucketIndex(k);

    auto prev = overflow_bucket_.before_begin();

    for (auto it = tmp.overflow_bucket_.begin(); it != tmp.overflow_bucket_.end(); ++it, ++prev) {
      if (equaler_(k, (*it).key_) && (*it).is_val_) {
        Value val = (*it).val_;

        tmp.overflow_bucket_.erase_after(prev);
        swap(tmp);
        return val;
      }
    }

    for (size_t i = 0; i < tmp.bucket_capacity_; ++i) {
      size_t idx = tmp.bucket_capacity_ * buc_idx + i;
      if (equaler_(tmp.data_[idx].key_, k) && tmp.data_[idx].is_val_) {

        tmp.data_[idx].is_val_ = false;
        tmp.sizes_[buc_idx]--;
        val = tmp.data_[bucket_capacity_ * buc_idx + i].val_;
        swap(tmp);
        return val;
      }
    }
    throw std::out_of_range("Key not found in drop()");
  }
  template < class Key, class Value, class Hash, class Equal >
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
  template < class Key, class Value, class Hash, class Equal >
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
