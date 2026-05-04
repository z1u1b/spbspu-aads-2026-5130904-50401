#ifndef MY_BASKET_HPP
#define MY_BASKET_HPP

#include "../common/iter.hpp"
#include <iostream>
namespace zubarev
{
  template < class Key, class Value >
  class IterHashTable
  {
    IterHashTable();
    IterHashTable(size_t, size_t, LIter);

    size_t index_;
    size_t bucket_index_;
    LIter <
  };

}

#endif
