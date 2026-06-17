// #include "../common/my_vector/top-it-vector.hpp"
#include "commands.hpp"
#include "my_equal.hpp"
#include "my_siphash.hpp"
#include <iostream>
#include <limits>
#include <string>
#include "file_system.hpp"
#include "robin_hashtable.hpp"
#include "utils.hpp"

int main()

{

  namespace zub = zubarev;

  zubarev::detail::printBannerColored();

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
  cmds["cat"] = zub::cmd_cat;
  cmds["pwd"] = zub::cmd_pwd;
  cmds["ls"] = zub::cmd_ls;
  cmds["ls"] = zub::cmd_ls;
  cmds["tree"] = zub::cmd_tree;
  cmds["ls"] = zub::cmd_ls;

  cmds["search"] = zub::cmd_search;
  cmds["save"] = zub::cmd_save;
  cmds["load"] = zub::cmd_load;
  cmds["states"] = zub::cmd_states;
  cmds["import"] = zub::cmd_import;
  cmds["export"] = zub::cmd_export;
  cmds["save-state"] = zub::cmd_save_state;
  cmds["start-state"] = zub::cmd_start_state;
  cmds["archive"] = zub::cmd_archive;
  // cmds["extract"] = zub::cmd_extract;
  cmds["help"] = zub::cmd_help;
  cmds["exit"] = zub::cmd_exit;
  zub::FileSystem file_sys;

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
  zub::cmd_exit(std::cin, std::cout, file_sys);
}
