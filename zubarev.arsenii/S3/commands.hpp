#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"
#include <iomanip>
#include <iosfwd>
// #include "notebook.hpp"
namespace zubarev
{
  // using zub = zubarev;
  // using Hash = zub::SipHash;
  // using Equal = zub::Equaler< std::string >;
  // using Graph = zub::HashTable<
  //     std::string,
  //     zub::HashTable< std::string, zub::HashTable< std::string, topit::Vector< size_t >, Hash, Equal >, Hash, Equal
  //     >, Hash, Equal >;
  void cmd_graphs(std::istream&, std::ostream& out, GraphTable& graph);
  void cmd_vertexes(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_outbound(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_inbound(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_bind(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_cut(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_create(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_merge(std::istream& in, std::ostream& out, GraphTable& graph);
  void cmd_extract(std::istream& in, std::ostream& out, GraphTable& graph);
}

#endif
