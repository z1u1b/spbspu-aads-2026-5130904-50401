#include "commands.hpp"
#include <iostream>

namespace zubarev
{
  void cmd_mkdir(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    in >> name;
    try {
      file_sys.mkdir(name);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
    // Логика: file_sys.mkdir(name);
  }

  void cmd_rm(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    in >> name;
    try {
      file_sys.rm(name);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
    // Логика: file_sys.rm(name);
  }

  void cmd_touch(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    in >> name;
    // Логика: file_sys.touch(name);
  }

  void cmd_write(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name, text;
    in >> name >> text;
  }

  void cmd_append(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name, text;
    in >> name >> text;
    // Логика: file_sys.append(name, text);
  }

  void cmd_cd(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    in >> path;
    // Логика: file_sys.cd(path);
  }

  void cmd_mv(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string from, to;
    in >> from >> to;
    // Логика: file_sys.mv(from, to);
  }

  void cmd_cp(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string from, to;
    in >> from >> to;
    // Логика: file_sys.cp(from, to);
  }

  void cmd_cat(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    in >> name;
    // out << file_sys.cat(name);
  }

  void cmd_pwd(std::istream&, std::ostream& out, FileSystem& file_sys)
  {
    out << file_sys.pwd() << "\n";
  }

  void cmd_ls(std::istream&, std::ostream& out, FileSystem& file_sys)
  {}

  void cmd_tree(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {}

  void cmd_search(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {}
}
