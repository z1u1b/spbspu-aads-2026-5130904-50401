#include "func-stack-queue.hpp"
#include <fstream>
#include <sstream>

#include <istream>

int main()
{
  std::string inp = "( 10 + 5 ) ## 3\n100 ## ( 2 * 3 )\n( 2 ## 3 ) * ( 4 ## 5 )";
  std::istringstream input(inp);

  try {
    zubarev::run(input, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
