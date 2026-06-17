#include "commands.hpp"

#include <iostream>
#include <iomanip>
#include "utils.hpp"
#include <sstream>
namespace zubarev
{

  void cmd_mkdir(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.mkdir(name);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_rm(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.rm(name);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_touch(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.touch(name);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_write(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name, text;
    if (!(in >> name >> std::quoted(text))) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.write(name, text);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_append(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name, text;
    if (!(in >> name >> std::quoted(text))) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.append(name, text);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_cd(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    if (!(in >> path)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.cd(path);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_mv(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string from, to;
    if (!(in >> from >> to)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.mv(from, to);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_cp(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string from, to;
    if (!(in >> from >> to)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.cp(from, to);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_cat(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      auto text_out = file_sys.cat(name);
      out << text_out << '\n';
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_pwd(std::istream&, std::ostream& out, FileSystem& file_sys)
  {
    try {
      out << file_sys.pwd() << "\n";
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_ls(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path = "";
    if (in.peek() != '\n' && in.peek() != EOF) {
      in >> path;
    }
    try {
      auto names = file_sys.ls(path);
      std::sort(names.begin(), names.end());
      out << detail::formatLsColumns(names);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_tree(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path = "";
    if (in.peek() != '\n' && in.peek() != EOF) {
      in >> path;
    }
    try {
      auto res = file_sys.tree(path);
      out << std::get< 0 >(res) << "\n";
      out << std::get< 1 >(res) << " directories, ";
      out << std::get< 2 >(res) << " files\n";
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_search(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    if (!(in >> path)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      file_sys.search(path);
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_save(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path;
    if (!(in >> path)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      if (file_sys.save(path)) {
        out << "<SAVED>\n";
      } else {
        out << "<INVALID COMMAND>\n";
      }
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_load(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try {
      if (file_sys.load(name)) {
        out << "<LOADED>\n";
      } else {
        out << "<INVALID COMMAND>\n";
      }
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_states(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string path = "";
    if (in.peek() != '\n' && in.peek() != EOF) {
      in >> path;
    }
    try {
      auto states_list = file_sys.states(path);
      if (states_list.empty()) {
        out << "<NO STATES FOUND>\n";
      } else {
        for (const auto& info : states_list) {
          out << info.name << " [" << info.size_kb << "KB, " << info.date << "]\n";
        }
      }
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_import(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string real_path, virtual_name;
    // Требуется ровно два аргумента
    if (!(in >> real_path >> virtual_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try {
      if (file_sys.import_file(real_path, virtual_name)) {
        out << "<IMPORTED: " << real_path << " -> " << virtual_name << ">\n";
      }
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_export(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string virtual_name, real_path;
    if (!(in >> virtual_name >> real_path)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try {
      if (file_sys.export_file(virtual_name, real_path)) {
        out << "<EXPORTED: " << virtual_name << " -> " << real_path << ">\n";
      }
    } catch (const std::exception& e) {
      out << e.what() << '\n';
    }
  }

  void cmd_save_state(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string line;
    std::getline(in, line);

    std::istringstream iss(line);

    std::string name = "";
    bool rewrite = false;

    if (!(iss >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    iss >> std::boolalpha >> rewrite;

    try {
      file_sys.save_state(name, rewrite);
      out << "<STATE SAVED COMPLETE>" << '\n';

    } catch (const std::exception& e) {
      out << e.what() << '\n';
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

  void cmd_start_state(std::istream& in, std::ostream& out, FileSystem& file_sys)
  {
    std::string line;
    std::getline(in, line);
    std::istringstream iss(line);

    std::string name;
    if (!(iss >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!file_sys.isEmpty()) {
      out << "WARNING: Current file system is not empty.\n";
      out << "Loading a new state will affect your current files.\n";
      out << "Choose an option:\n";
      out << "  1. Save current state to a new file, then load\n";
      out << "  2. Discard current state and load new\n";
      out << "  3. Cancel loading\n";
      out << "Enter choice (1/2/3): ";
      out.flush();

      std::string choice_line;
      std::getline(in, choice_line);

      std::istringstream choice_stream(choice_line);
      int choice = 0;
      choice_stream >> choice;
      if (choice == 1) {
        out << "Enter filename to save current state (e.g., backup.state): ";
        out.flush();

        std::string backup_name;
        std::getline(in, backup_name);
        std::istringstream name_stream(backup_name);
        name_stream >> backup_name;

        if (backup_name.empty()) {
          out << "<INVALID COMMAND>: empty backup filename\n";
          return;
        }

        try {
          file_sys.save_state(backup_name, false);
          out << "<STATE SAVED: " << backup_name << ">\n";
        } catch (const std::exception& e) {
          out << "Failed to save backup: " << e.what() << "\n";
          out << "Loading cancelled.\n";
          return;
        }
      } else if (choice == 2) {
        out << "Discarding current state...\n";

      } else if (choice == 3) {
        out << "<LOADING CANCELLED>\n";
        return;
      } else {
        out << "<INVALID COMMAND>: invalid choice\n";
        return;
      }
    }

    try {
      file_sys.start_state(name);
      out << "<STATE START COMPLETE>" << '\n';

    } catch (const std::exception& e) {
      out << e.what() << '\n';
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

  void cmd_help(std::istream&, std::ostream& out, FileSystem&)
  {
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string YELLOW = "\033[33m";
    const std::string GREEN = "\033[32m";
    const std::string CYAN = "\033[36m";
    const std::string MAGENTA = "\033[35m";

    out << "\n";
    out << BOLD << YELLOW << "╔══════════════════════════════════════════════════════════════════╗\n";
    out << "║                     FILE SYSTEM HELP                             ║\n";
    out << "╚══════════════════════════════════════════════════════════════════╝" << RESET << "\n\n";

    // === КАТЕГОРИЯ 1: ФАЙЛЫ И ДИРЕКТОРИИ ===
    out << BOLD << GREEN << "  [FILES AND DIRECTORIES]" << RESET << "\n";
    out << CYAN << "  mkdir <dir-name>" << RESET << "           Create a new directory\n";
    out << CYAN << "  rmdir <dir-name>" << RESET << "           Remove an empty directory\n";
    out << CYAN << "  rm <file-name>" << RESET << "             Remove a file\n";
    out << CYAN << "  touch <file-name>" << RESET << "          Create an empty file\n";
    out << CYAN << "  write <file> \"<text>\"" << RESET << "      Write text to a file (replace content)\n";
    out << CYAN << "  cat <file-name>" << RESET << "            Display file content\n";
    out << CYAN << "  append <file> \"<text>\"" << RESET << "     Append text to the end of a file\n";
    out << CYAN << "  mv <from> <to>" << RESET << "             Move or rename a file/directory\n";
    out << CYAN << "  cp <from> <to>" << RESET << "             Copy a file/directory\n";
    out << "\n";

    // === КАТЕГОРИЯ 2: НАВИГАЦИЯ ===
    out << BOLD << GREEN << "  [NAVIGATION]" << RESET << "\n";
    out << CYAN << "  cd <path>" << RESET << "                  Change current directory\n";
    out << CYAN << "  pwd" << RESET << "                        Print current working directory\n";
    out << CYAN << "  ls [path]" << RESET << "                  List directory contents\n";
    out << CYAN << "  tree [path]" << RESET << "                Display directory tree structure\n";
    out << "\n";

    // === КАТЕГОРИЯ 3: ПОИСК ===
    out << BOLD << GREEN << "  [SEARCH]" << RESET << "\n";
    out << CYAN << "  search <name>" << RESET << "              Search files/dirs by exact name\n";
    out << CYAN << "  ssearch <type> <text>" << RESET << "      Smart search (name/content/ext)\n";
    out << "\n";

    // === КАТЕГОРИЯ 4: СЕССИИ И СОСТОЯНИЯ ===
    out << BOLD << GREEN << "  [SESSIONS AND STATES]" << RESET << "\n";
    out << CYAN << "  save <name>" << RESET << "                Save object to in-memory session storage\n";
    out << CYAN << "  load <name>" << RESET << "                Load object from session storage\n";
    out << CYAN << "  states [path]" << RESET << "              List available .state files on disk\n";
    out << CYAN << "  save-state <file> [true]" << RESET << "   Save current FS to disk (.state file)\n";
    out << CYAN << "  start-state <file>" << RESET << "         Load FS state from disk (replaces current)\n";
    out << "\n";

    // === КАТЕГОРИЯ 5: ИМПОРТ/ЭКСПОРТ ===
    out << BOLD << GREEN << "  [IMPORT / EXPORT]" << RESET << "\n";
    out << CYAN << "  import <real-path> <virtual-name>" << RESET << "   Import file from real disk to VFS\n";
    out << CYAN << "  export <virtual-name> <real-path>" << RESET << "   Export file from VFS to real disk\n";
    out << CYAN << "  archive <dir-name>" << RESET << "               Create compressed archive (TODO)\n";
    out << "\n";

    // === КАТЕГОРИЯ 6: КЭШИРОВАНИЕ ===
    out << BOLD << GREEN << "  [CACHING (LRU)]" << RESET << "\n";
    out << CYAN << "  cache_size <size>" << RESET << "         Set cache size (e.g., 512MB)\n";
    out << CYAN << "  cache_on" << RESET << "                  Enable caching system\n";
    out << CYAN << "  cache_off" << RESET << "                 Disable caching and flush data\n";
    out << CYAN << "  cache_stats" << RESET << "               Show cache usage statistics\n";
    out << "\n";

    // === КАТЕГОРИЯ 7: СИСТЕМНЫЕ ===
    out << BOLD << GREEN << "  [SYSTEM]" << RESET << "\n";
    out << CYAN << "  help" << RESET << "                      Show this help message\n";
    out << CYAN << "  exit" << RESET << "                      Exit the file system\n";
    out << "\n";

    out << BOLD << MAGENTA << "  NOTES:" << RESET << "\n";
    out << "  • Use quotes for text with spaces: write file.txt \"Hello World\"\n";
    out << "  • Paths support ~ (root), . (current), .. (parent)\n";
    out << "  • Data is compressed with Huffman algorithm and deduplicated via SipHash+XXHash\n";
    out << "\n";
  }
  void cmd_exit(std::istream&, std::ostream& out, FileSystem&)
  {
    out << "\n";
    out << "\033[1;32m";
    out << "╔══════════════════════════════════════════════════════════════════╗\n";
    out << "║                  GOODBYE! SEE YOU SOON!                          ║\n";
    out << "╚══════════════════════════════════════════════════════════════════╝\n";
    out << "\033[0m";
    out << "\n";

    std::exit(0);
  }
}
