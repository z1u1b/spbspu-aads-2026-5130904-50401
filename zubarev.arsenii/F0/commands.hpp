#ifndef COMMANDS_FS_HPP
#define COMMANDS_FS_HPP

#include "file_system.hpp"
#include <iosfwd>
namespace zubarev
{
  void cmd_mkdir(std::istream&, std::ostream& out, FileSystem& file_sys);
  void cmd_rm(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_touch(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_write(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_append(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_cd(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_mv(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_cp(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_cat(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_pwd(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_ls(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_tree(std::istream& in, std::ostream& out, FileSystem& file_sys);
  void cmd_search(std::istream& in, std::ostream& out, FileSystem& file_sys);
}

#endif
