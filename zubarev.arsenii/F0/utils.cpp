#include "utils.hpp"
#include <chrono>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>

// #include <iostream>
#include <string>
#include <cstdlib>
namespace zubarev
{
  namespace detail
  {
    std::string getCurrentDateTime()
    {

      auto now = std::chrono::system_clock::now();

      std::time_t now_time = std::chrono::system_clock::to_time_t(now);

      std::tm* now_tm = std::localtime(&now_time);

      std::stringstream ss;
      ss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S"); // Формат: ГГГГ-ММ-ДД ЧЧ:ММ:СС

      return ss.str();
    }

    std::string getCurrentUser()
    {
      char* user = nullptr;

#if defined(_WIN32) || defined(_WIN64)
      // Для Windows используем USERNAME
      user = std::getenv("USERNAME");
#else
      // Для Linux, macOS и других POSIX-систем используем USER
      user = std::getenv("USER");
#endif

      if (user != nullptr) {
        return std::string(user);
      }

      // Значение по умолчанию, если переменная окружения не найдена
      return "root";
    }

    Queue< std::string > resolvePath(const std::string& path)
    {
      Queue< std::string > dirs;
      std::string cur_str = "";
      for (auto it = path.begin(); it != path.end(); ++it) {
        if (*it != '/') {
          cur_str += *it;
        } else {
          dirs.push(cur_str);
          cur_str = "";
        }
      }
      dirs.push(cur_str);
      cur_str = "";
      return dirs;
    }

  }
}
