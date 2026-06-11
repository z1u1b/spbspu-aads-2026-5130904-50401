#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include "../common/my_stack/stack.hpp"
namespace zubarev
{
  namespace detail
  {
    std::string getCurrentDateTime();
    std::string getCurrentUser();
    Stack< std::string > resolvePath(const std::string& path);

  }
}

#endif
