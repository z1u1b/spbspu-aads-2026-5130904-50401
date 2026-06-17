#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "../common/my_vector/top-it-vector.hpp"
#include "../common/my_queue/queue.hpp"
namespace zubarev
{
  namespace detail
  {
    std::string getCurrentDateTime();
    std::string getCurrentUser();
    Queue< std::string > resolvePath(const std::string& path);
    std::string formatLsColumns(const topit::Vector< std::string >& names, size_t terminalWidth = 80);
    void printBannerColored();
  }
}

#endif
