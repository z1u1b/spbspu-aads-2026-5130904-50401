// #include "../common/my_vector/top-it-vector.hpp"
#include "commands.hpp"
// #include "my_equal.hpp"
// #include "my_siphash.hpp"
#include <iostream>
#include <limits>
// #include <string>
// #include <functional>
#include "file_system.hpp"
// #include "robin_hashtable.hpp"
#include "utils.hpp"

int main()

{

  zubarev::detail::printBannerColored();
  zubarev::FileSystem file_sys;

  zubarev::process_stream(std::cin, std::cout, file_sys);

  std::cin.clear();
  std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input";
    return 1;
  }

  zubarev::cmd_exit(std::cin, std::cout, file_sys);
}
