#include "commands.hpp"

#include <iostream>
#include <iomanip>
#include "utils.hpp"
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
    try {
      file_sys.touch(name);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
    // Логика: file_sys.touch(name);
  }

  void cmd_write(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name, text;
    in >> name >> std::quoted(text);
    try {
      file_sys.write(name, text);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
  }

  void cmd_append(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name, text;
    in >> name >> std::quoted(text);
    try {
      file_sys.append(name, text);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
    // Логика: file_sys.append(name, text);
  }

  void cmd_cd(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    in >> path;
    try {
      file_sys.cd(path);
    } catch (...) {
      out << "<<INVALID COMMAND>";
    }
    // Логика: file_sys.cd(path);
  }

  void cmd_mv(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string from, to;
    in >> from >> to;
    try {
      file_sys.mv(from, to);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
    // Логика: file_sys.mv(from, to);
  }

  void cmd_cp(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string from, to;
    in >> from >> to;
    try {
      file_sys.cp(from, to);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
    // Логика: file_sys.cp(from, to);
  }

  void cmd_cat(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    in >> name;
    auto text_out = file_sys.cat(name);
    out << text_out << '\n';
    // try {
    //   file_sys.cat(name);
    // } catch (...) {
    //   out << "<INVALID COMMAND>";
    // }
    // out << file_sys.cat(name);
  }

  void cmd_pwd(std::istream&, std::ostream& out, FileSystem& file_sys)
  {
    out << file_sys.pwd() << "\n";
  }

  void cmd_ls(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path = "";
    if (in.peek() != '\n' && in.peek() != EOF) {
      in >> path;
    }
    auto names = file_sys.ls(path);
    std::sort(names.begin(), names.end());
    out << detail::formatLsColumns(names);
  }

  void cmd_tree(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path = "";
    if (in.peek() != '\n' && in.peek() != EOF) {
      in >> path;
    }

    auto res = file_sys.tree(path);
    out << std::get< 0 >(res) << "\n";
    out << std::get< 1 >(res) << " directories, ";
    out << std::get< 2 >(res) << " files\n";
  }

  void cmd_search(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    in >> path;
    try {
      file_sys.search(path);
    } catch (...) {
      out << "<INVALID COMMAND>";
    }
  }

  void printPrompt(const FileSystem& fs, std::ostream& out)
  {
    // ANSI коды для цветов
    const std::string RESET = "\033[0m";
    const std::string GREEN = "\033[32m";
    const std::string BLUE = "\033[34m";
    const std::string YELLOW = "\033[33m";

    std::string user = detail::getCurrentUser();
    std::string path = fs.pwd();

    out << GREEN << user << RESET << ":" << BLUE << path << RESET << YELLOW << " $ " << RESET;
  }

}
