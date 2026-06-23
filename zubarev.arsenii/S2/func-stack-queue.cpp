#include "func-stack-queue.hpp"

#include <iostream>
#include <string>

#include "queue.hpp"
#include "stack.hpp"

std::string zubarev::detail::readLine(std::istream& in)
{
  std::string line;
  std::getline(in, line);
  return line;
}

zubarev::Queue< std::string > zubarev::detail::fromStrToQueue(const std::string& str)
{
  Queue< std::string > q;
  std::string container = "";
  for (auto it = str.begin(); it != str.end(); ++it) {
    char ch = *it;
    if (std::isspace(static_cast< unsigned char >(ch))) {
      continue;
    }
    if (std::isdigit(ch)) {
      container += (ch);
    } else {
      if (!container.empty()) {
        q.push(container);
        container = "";
      }
      if (ch == '#') {
        container = "";
        if (it + 1 != str.end() && *(it + 1) == '#') {
          q.push("##");
          ++it;
        } else {
          it++;
        }
      } else {
        q.push(std::string(1, ch));
      }
    }
  }
  if (!container.empty()) {
    q.push(container);
  }
  return q;
}
bool zubarev::detail::isOperation(const std::string& str)
{
  return str == "+" || str == "-" || str == "*" || str == "/" || str == "%" || str == "##";
}
size_t zubarev::detail::getPriority(const std::string& oper)
{

  if (oper == "##") {
    return 3;
  }

  if (oper == "*" || oper == "/" || oper == "%") {
    return 2;
  }

  if (oper == "+" || oper == "-") {
    return 1;
  }
  return 0;
}

zubarev::Queue< std::string > zubarev::detail::fromInfixToPostfix(Queue< std::string >& infixQ)
{
  Stack< std::string > stack;
  Queue< std::string > postfixQ;

  while (!infixQ.empty()) {

    std::string el = infixQ.top();
    infixQ.drop();

    if (el == "(") {
      stack.push(el);
    } else if (el == ")") {

      while (!stack.empty() && stack.top() != "(") {
        postfixQ.push(stack.top());
        stack.drop();
      }

      if (stack.empty()) {
        throw std::runtime_error("Unbalanced parentheses");
      }

      stack.drop();
    } else if (isOperation(el)) {

      while (!stack.empty() && getPriority(stack.top()) >= getPriority(el)) {
        postfixQ.push(stack.top());
        stack.drop();
      }
      stack.push(el);
    } else {
      postfixQ.push(el);
    }
  }

  while (!stack.empty()) {
    if (stack.top() == "(") {
      throw std::runtime_error("Unbalanced parentheses");
    }
    postfixQ.push(stack.top());
    stack.drop();
  }

  return postfixQ;
}
