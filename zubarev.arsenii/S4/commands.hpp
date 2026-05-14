#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dataset.hpp"
#include <iomanip>
#include <iosfwd>
namespace zubarev
{
  void cmd_print(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_complement(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_intersect(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_union(std::istream& in, std::ostream& out, GraphTable& graph);

}

#endif
