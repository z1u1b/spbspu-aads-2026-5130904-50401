// #include "../common/my_vector/top-it-vector.hpp"
#include "commands.hpp"
#include "my_equal.hpp"
#include "my_siphash.hpp"
#include <iostream>
#include <limits>
#include <string>
#include "file_system.hpp"
#include "robin_hashtable.hpp"

int main()

{

  namespace zub = zubarev;

  // if (argc != 2) {
  //   std::cerr << "Wrong arguments\n";
  //   return 1;
  // }

  // std::ifstream input(argv[1]);

  // if (!input) {
  //   std::cerr << "Cannot open file\n";
  //   return 1;
  // }

  using Hash = zub::SipHash;
  using Equal = zub::Equaler< std::string >;
  using cmd_t = void (*)(std::istream&, std::ostream&, zub::FileSystem&);
  zub::RobinHashTable< std::string, cmd_t, Hash, Equal > cmds(128);

  cmds["mkdir"] = zub::cmd_mkdir;
  cmds["rm"] = zub::cmd_rm;
  cmds["touch"] = zub::cmd_touch;
  cmds["write"] = zub::cmd_write;
  cmds["append"] = zub::cmd_append;
  cmds["cd"] = zub::cmd_cd;
  cmds["mv"] = zub::cmd_mv;
  cmds["cp"] = zub::cmd_cp;
  std::cout << (cmds.at("cp") == zub::cmd_cp) << '\n';
  cmds["cat"] = zub::cmd_cat;
  cmds["pwd"] = zub::cmd_pwd;
  cmds["ls"] = zub::cmd_ls;
  cmds["ls"] = zub::cmd_ls;
  std::cout << (cmds.at("ls") == zub::cmd_ls) << '\n';
  cmds["tree"] = zub::cmd_tree;
  cmds["ls"] = zub::cmd_ls;

  std::cout << (cmds.at("tree") == zub::cmd_tree) << '\n';
  cmds["search"] = zub::cmd_search;
  std::cout << "mkdir  = " << reinterpret_cast< const void* >(cmds.at("mkdir")) << '\n';
  std::cout << "rm     = " << reinterpret_cast< const void* >(cmds.at("rm")) << '\n';
  std::cout << "touch  = " << reinterpret_cast< const void* >(cmds.at("touch")) << '\n';
  std::cout << "write  = " << reinterpret_cast< const void* >(cmds.at("write")) << '\n';
  std::cout << "append = " << reinterpret_cast< const void* >(cmds.at("append")) << '\n';
  std::cout << "cd     = " << reinterpret_cast< const void* >(cmds.at("cd")) << '\n';
  std::cout << "mv     = " << reinterpret_cast< const void* >(cmds.at("mv")) << '\n';
  std::cout << "cp     = " << reinterpret_cast< const void* >(cmds.at("cp")) << '\n';
  std::cout << "cat    = " << reinterpret_cast< const void* >(cmds.at("cat")) << '\n';
  std::cout << "pwd    = " << reinterpret_cast< const void* >(cmds.at("pwd")) << '\n';
  std::cout << "ls     = " << reinterpret_cast< const void* >(cmds.at("ls")) << '\n';
  std::cout << "tree   = " << reinterpret_cast< const void* >(cmds.at("tree")) << '\n';
  std::cout << "search = " << reinterpret_cast< const void* >(cmds.at("search")) << '\n';

  zub::FileSystem file_sys;

  // try {

  //   zub::inputGraphs(input, std::cout, graph);
  // }

  // catch (const std::exception& e) {
  //   std::cerr << e.what() << '\n';
  //   return 1;
  // }

  std::string cmd;
  zub::printPrompt(file_sys, std::cout);
  while (std::cin >> cmd) {

    try {
      if (!cmds.has(cmd)) {
        throw std::out_of_range("unknown command");
      }
      // std::cout << "COMMAND = " << cmd << '\n';
      cmds.at(cmd)(std::cin, std::cout, file_sys);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<<INVALID COMMAND: " << e.what() << '\n';
    }
    zub::printPrompt(file_sys, std::cout);
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input";
    return 1;
  }
}
