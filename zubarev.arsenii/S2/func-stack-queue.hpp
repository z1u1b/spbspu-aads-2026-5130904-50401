#ifndef FUNC_HPP
#define FUNC_HPP

#include <iosfwd>
#include "stack.hpp"
#include "queue.hpp"

namespace zubarev
{
  namespace detail
  {
    std::string readLine(std::istream& in);
    Queue< std::string > fromStrToQueue(const std::string& str);
    bool isOperation(const std::string& str);
    size_t getPriority(const std::string& oper);

    Queue< std::string > fromInfixToPostfix(Queue< std::string >& infixQ);
  }

}
#endif
