#include "functions.hpp"
#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include "list.hpp"


zubarev::List< Data > zubarev::input(std::istream& in, bool& error)
{
  List< Data > list;
  auto itList = list.before_begin();
  error = false;

  std::string line;

  while (std::getline(in, line)) {
    std::istringstream iss(line);
    Data value;

    if (!(iss >> value.name)) {
      continue;
    }

    List< size_t > nums;
    auto itNum = nums.before_begin();
    size_t num;
    while (iss >> num) {
      itNum = nums.insert_after(itNum, num);
    }

    if (!iss.eof()) {
      error = true;
      return List< Data >{};
    }
    value.numbers = nums;
    itList = list.insert_after(itList, value);
  }

  if (!in.eof() && in.fail()) {
    error = true;
    return List< Data >{};
  }
  return list;
}

size_t zubarev::output_names(const List< Data >& list)
{
  if (list.empty()) {
    return 0;
  }

  auto it = list.begin();

  std::cout << it->name;
  ++it;

  while (it != list.end()) {
    std::cout << " " << it->name;
    ++it;
  }

  std::cout << '\n';

  return 0;
}
size_t zubarev::max_sequences(const List< Data >& list)
{
  size_t maxNum = 0;
  auto itList = list.begin();
  while (itList != list.end()) {
    const List< size_t >& nums = itList->numbers;
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
size_t zubarev::output_sequences(const List< Data >& list)
{
  if (list.empty()) {
    return 0;
  }

  for (size_t i = 0; i < max_sequences(list); ++i) {
    LCIter< Data > itList = list.begin();
    bool found_first = false;

    while (itList != list.end() && !found_first) {
      LCIter< size_t > itNums = itList->numbers.begin();
      bool flag = true;

      for (size_t j = 0; j < i; ++j) {
        if (itNums != itList->numbers.end()) {
          ++itNums;
        } else {
          flag = false;
          break;
        }
      }

      if (flag && itNums != itList->numbers.end()) {
        std::cout << *itNums;
        found_first = true;
      }
      ++itList;
    }

    while (itList != list.end()) {
      LCIter< size_t > itNums = itList->numbers.begin();
      bool flag = true;

      for (size_t j = 0; j < i; ++j) {
        if (itNums != itList->numbers.end()) {
          ++itNums;
        } else {
          flag = false;
          break;
        }
      }
      if (flag && itNums != itList->numbers.end()) {
        std::cout << " " << *itNums;
      }
      ++itList;
    }
    std::cout << '\n';
  }
  return 0;
}

zubarev::List< size_t > zubarev::calculate_sums(const List< Data >& list)
{
  const size_t maxSeq = max_sequences(list);
  List< size_t > sums;
  auto itSum = sums.before_begin();

  for (size_t i = 0; i < maxSeq; ++i) {
    size_t sum = 0;
    auto itList = list.begin();

    while (itList != list.end()) {
      auto itNums = itList->numbers.begin();

      for (size_t j = 0; j < i && itNums != itList->numbers.end(); ++j) {
        ++itNums;
      }
      if (itNums != itList->numbers.end()) {
        size_t value = *itNums;
        if (sum > std::numeric_limits< size_t >::max() - value) {
          throw std::overflow_error("sum overflow");
        }
        sum += value;
      }

      ++itList;
    }

    itSum = sums.insert_after(itSum, sum);
  }
  return sums;
}

void zubarev::print_sums(const List< size_t >& sums)
{
  if (sums.empty()) {
    return;
  }

  auto it = sums.begin();
  std::cout << *it;
  ++it;

  while (it != sums.end()) {
    std::cout << " " << *it;
    ++it;
  }
  std::cout << '\n';
}

