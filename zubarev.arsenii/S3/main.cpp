#include "../common/top-it-vector.hpp"
#include "commands.hpp"
#include "graph.hpp"
#include "my_equal.hpp"
#include "my_hashtable.hpp"
#include "my_siphash.hpp"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

// using zub=zub;
int main()
{
  namespace zub = zubarev;
  using Hash = zub::SipHash;
  using Equal = zub::Equaler< std::string >;
  // using Graph = zub::HashTable<
  //     std::string,
  //     zub::HashTable< std::string, zub::HashTable< std::string, topit::Vector< size_t >, Hash, Equal >, Hash, Equal
  //     >, Hash, Equal >;
  using cmd_t = void (*)(std::istream&, std::ostream&, zub::GraphTable&);

  // using Table = zub::HashTable< std::pair<>, std::pair<>, std::hash< std::string >, std::equal_to< std::string >
  // >

  // std::unordered_map< std::string, cmd_t > cmds;

  zub::HashTable< std::string, cmd_t, Hash, Equal > cmds;

  cmds["graphs"] = zub::cmd_graphs;
  cmds["vertexes"] = zub::cmd_vertexes;
  cmds["outbound"] = zub::cmd_outbound;
  cmds["inbound"] = zub::cmd_inbound;
  cmds["bind"] = zub::cmd_bind;
  cmds["cut"] = zub::cmd_cut;
  cmds["create"] = zub::cmd_create;
  cmds["merge"] = zub::cmd_merge;
  cmds["extract"] = zub::cmd_extract;

  std::string cmd;
  // zub::NoteBook notebook;
  zub::GraphTable graph;
  while (std::cin >> cmd) {
    try {
      if (!cmds.has(cmd)) {
        throw std::out_of_range("unknown command");
      }
      cmds.at(cmd)(std::cin, std::cout, graph);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << '\n';
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input";
    return 1;
  }
}
