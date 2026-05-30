

#include <iostream>
#include <stdexcept>

#include "data.hpp"
#include "functions.hpp"
#include "iter.hpp"
#include "list.hpp"

int main()
{
  bool error = false;

  zubarev::List< zubarev::Data > list = zubarev::input(std::cin, error);

  if (error) {
    return 1;
  }

  error = zubarev::output_names(list) || zubarev::output_sequences(list);

  if (error) {
    return 1;
  }

  try {
    const zubarev::List< size_t > sums = zubarev::calculate_sums(list);

    if (sums.empty()) {
      std::cout << "0\n";
    } else {
      zubarev::print_sums(sums);
    }
  } catch (const std::overflow_error&) {
    std::cerr << "sum overflow\n";
    return 1;
  }

  return 0;
}
