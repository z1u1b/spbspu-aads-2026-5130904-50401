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

  void cmd_save(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    if (!(in >> path)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (file_sys.save(path)) {
      out << "<SAVED>\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmd_load(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (file_sys.load(name)) {
      out << "<LOADED>\n";
    } else {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmd_states(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path = "";
    // Читаем путь, если он есть (аналогично твоему cmd_tree)
    if (in.peek() != '\n' && in.peek() != EOF) {
      in >> path;
    }

    auto states_list = file_sys.states(path);

    if (states_list.empty()) {
      out << "<NO STATES FOUND>\n";
    } else {
      for (const auto& info : states_list) {
        out << info.name << " [" << info.size_kb << "KB, " << info.date << "]\n";
      }
    }
  }

  // void cmd_archive(std::istream& in, std::ostream& out, FileSystem& file_sys)
  // {
  //   std::string path;
  //   if (!(in >> path)) {
  //     out << "<INVALID COMMAND>\n";
  //     return;
  //   }

  //   if (file_sys.archive(path)) {
  //     out << "<ARCHIVED: " << path << ".zip>\n";
  //   } else {
  //     out << "<INVALID COMMAND>\n";
  //   }
  // }

  // void cmd_import(std::istream& in, std::ostream& out, FileSystem& file_sys)
  // {
  //   std::string real_path, virtual_name;
  //   // Требуется ровно два аргумента
  //   if (!(in >> real_path >> virtual_name)) {
  //     out << "<INVALID COMMAND>\n";
  //     return;
  //   }

  //   // Примечание: метод в FileSystem лучше назвать import_file,
  //   // чтобы не конфликтовать с ключевым словом export/import в некоторых компиляторах
  //   if (file_sys.import_file(real_path, virtual_name)) {
  //     out << "<IMPORTED: " << real_path << " -> " << virtual_name << ">\n";
  //   } else {
  //     out << "<INVALID COMMAND>\n";
  //   }
  // }

  // void cmd_export(std::istream& in, std::ostream& out, FileSystem& file_sys)
  // {
  //   std::string virtual_name, real_path;
  //   // Требуется ровно два аргумента
  //   if (!(in >> virtual_name >> real_path)) {
  //     out << "<INVALID COMMAND>\n";
  //     return;
  //   }

  //   // Примечание: метод в FileSystem лучше назвать export_file
  //   if (file_sys.export_file(virtual_name, real_path)) {
  //     out << "<EXPORTED: " << virtual_name << " -> " << real_path << ">\n";
  //   } else {
  //     out << "<INVALID COMMAND>\n";
  //   }
  // }

  // void cmd_start_state(std::istream& in, std::ostream& out, FileSystem& file_sys)
  // {
  //   std::string filename;
  //   if (!(in >> filename)) {
  //     out << "<INVALID COMMAND>\n";
  //     return;
  //   }

  //   bool force = false;
  //   std::string force_str;
  //   // Проверяем, передан ли второй аргумент (true/false)
  //   if (in >> force_str) {
  //     force = (force_str == "true");
  //   }

  //   if (file_sys.start_state(filename, force)) {
  //     out << "<STATE LOADED: " << filename << ">\n";
  //   } else {
  //     out << "<INVALID COMMAND>\n";
  //   }
  // }

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
