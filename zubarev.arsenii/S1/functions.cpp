#include "functions.hpp"
#include "list.hpp"
#include <iostream>
#include <limits>
#include <string>

namespace zubarev
{
  List< Data > input(std::istream& in, bool& error)
  {
    List< Data > list;
    auto itList = list.before_begin();
    Data value;

    error = false;

    while (in >> value.name) {
      List< size_t > nums;
      auto itNum = nums.before_begin();

      while (in.peek() != '\n' && !in.eof()) {
        size_t num;
        if (in >> num) {
          nums.insert_after(itNum, num);
          ++itNum;
        } else {
          in.clear();

          if (in.peek() == '\n' || in.eof()) {
            break;
          }

          error = true;
          return List< Data >{};
        }
      }

      if (in.peek() == '\n') {
        in.get();
      }

      value.numbers = nums;
      list.insert_after(itList, value);
      ++itList;
    }

    if (!in.eof() && in.fail()) {
      error = true;
      return List< Data >{};
    }

    return list;
  }

  size_t output_names(List< Data >* list)
  {
    if (!list) {
      std::cerr << "output_names: list pointer is nullptr" << '\n';
      return 1;
    }
    if (!list->empty()) {
      auto it = list->begin();

      std::cout << it->name;
      ++it;

      while (it != list->end()) {
        std::cout << " " << it->name;
        ++it;
      }

      std::cout << '\n';
    }

    return 0;
  }
  size_t max_sequences(List< Data >* list)
  {
    if (!list) {
      std::cerr << "max_sequences: list pointer is nullptr";
      return 0;
    }
    size_t maxNum = 0;
    LIter< Data > itList = list->begin();
    while (itList != list->end()) {
      const List< size_t >& nums = (*itList).numbers;
      LCIter< size_t > itNums = nums.begin();
      size_t count = 0;
      while (itNums != nums.end()) {
        count++;
        ++itNums;
      }
      maxNum = std::max(maxNum, count);
      ++itList;
    }
    return maxNum;
  }
  size_t output_sequences(List< Data >* list)
  {
    if (!list) {
      std::cerr << "output_sequences: list pointer is nullptr";
      return 1;
    }

    for (size_t i = 0; i < max_sequences(list); ++i) {

      LIter< Data > itList = list->begin();
      bool found_first = false;

      while (itList != list->end() && !found_first) {
        LIter< size_t > itNums = (*itList).numbers.begin();
        bool flag = true;

        for (size_t j = 0; j < i; ++j) {
          if (itNums != (*itList).numbers.end()) {
            ++itNums;
          } else {
            flag = false;
            break;
          }
        }

        if (flag && itNums != (*itList).numbers.end()) {
          std::cout << *itNums; // ✅ первый элемент без пробела
          found_first = true;
        }
        ++itList;
      }

      while (itList != list->end()) {
        LIter< size_t > itNums = (*itList).numbers.begin();
        bool flag = true;

        for (size_t j = 0; j < i; ++j) {
          if (itNums != (*itList).numbers.end()) {
            ++itNums;
          } else {
            flag = false;
            break;
          }
        }

        if (flag && itNums != (*itList).numbers.end()) {
          std::cout << " " << *itNums;
        }
        ++itList;
      }

      std::cout << '\n';
    }
    return 0;
  }

  size_t output_sums(List< Data >* list)
  {
    if (!list) {
      std::cerr << "output_sums: list pointer is nullptr" << '\n';
      return 1;
    }

    const size_t maxSeq = max_sequences(list);

    if (maxSeq == 0) {
      std::cout << "0\n";
      return 0;
    }

    List< size_t > sums;
    LIter< size_t > itSum = sums.before_begin();
    bool overflow = false;

    for (size_t i = 0; i < maxSeq; ++i) {
      size_t sum = 0;
      LIter< Data > itList = list->begin();

      while (itList != list->end()) {
        LIter< size_t > itNums = (*itList).numbers.begin();
        for (size_t j = 0; j < i && itNums != (*itList).numbers.end(); ++j) {
          ++itNums;
        }
        if (itNums != (*itList).numbers.end()) {
          size_t value = (*itNums);
          if (sum > std::numeric_limits< size_t >::max() - value) {
            overflow = true;
            break;
          }
          sum += value;
        }
        ++itList;
      }

      if (overflow) {
        break;
      }

      sums.insert_after(itSum, sum);
      ++itSum;
    }

    if (overflow) {
      std::cerr << "output_sums: sum overflow" << '\n';
      return 1;
    }

    if (!sums.empty()) {
      auto it = sums.begin();
      std::cout << *it;
      ++it;
      while (it != sums.end()) {
        std::cout << " " << *it;
        ++it;
      }
      std::cout << '\n';
    }

    return 0;
  }

}
