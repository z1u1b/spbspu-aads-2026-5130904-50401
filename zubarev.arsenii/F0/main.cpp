// #include "../common/my_vector/top-it-vector.hpp"
#include "commands.hpp"
#include "my_equal.hpp"
#include "my_siphash.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include "file_system.hpp"

int main(int argc, char* argv[])

{

  namespace zub = zubarev;

  if (argc != 2) {
    std::cerr << "Wrong arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input) {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  using Hash = zub::SipHash;
  using Equal = zub::Equaler< std::string >;
  using cmd_t = void (*)(std::istream&, std::ostream&, zub::FileSystem&);
  zub::RobinHashTable< std::string, cmd_t, Hash, Equal > cmds;

  cmds["mkdir"] = zub::cmd_mkdir;
  cmds["rm"] = zub::cmd_rm;
  cmds["touch"] = zub::cmd_touch;
  cmds["write"] = zub::cmd_write;
  cmds["append"] = zub::cmd_append;
  cmds["cd"] = zub::cmd_cd;
  cmds["mv"] = zub::cmd_mv;
  cmds["cp"] = zub::cmd_cp;
  cmds["cat"] = zub::cmd_cat;
  cmds["pwd"] = zub::cmd_pwd;
  cmds["ls"] = zub::cmd_ls;
  cmds["tree"] = zub::cmd_tree;
  cmds["search"] = zub::cmd_search;

  zub::FileSystem file_sys;

  // try {

  //   zub::inputGraphs(input, std::cout, graph);
  // }

  // catch (const std::exception& e) {
  //   std::cerr << e.what() << '\n';
  //   return 1;
  // }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (!cmds.has(cmd)) {
        throw std::out_of_range("unknown command");
      }
      cmds.at(cmd)(std::cin, std::cout, file_sys);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << '\n';
    }
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input";
    return 1;
  }
}
