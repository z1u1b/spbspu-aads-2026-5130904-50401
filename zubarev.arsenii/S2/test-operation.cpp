#include "func-math.hpp"
#include "func-stack-queue.hpp"
#include "queue.hpp"
#include "stack.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(operation_test)

BOOST_AUTO_TEST_CASE(BitShiftToRightTest)
{

  std::string inp = "( 10 + 5 ) ## 3\n100 ## ( 2 * 3 )\n( 2 ## 3 ) * ( 4 ## 5 )";
  std::istringstream input(inp);

  std::ostringstream output;

  zubarev::run(input, output);

  BOOST_CHECK(output.str() == "153 1006 1035\n");
}

BOOST_AUTO_TEST_SUITE_END()
