#include "file_system.hpp"
namespace zubarev
{
  bool FileSystem::mkdir(const std::string& name_dir)
  {
    // Логика: если успешно создано - return true, иначе false
    return false;
  }

  bool FileSystem::rm(const std::string& name)
  {
    return false;
  }

  bool FileSystem::touch(const std::string& name_file)
  {
    return false;
  }

  bool FileSystem::write(const std::string& name, const std::string& text)
  {
    return false;
  }

  bool FileSystem::append(const std::string& name, const std::string& text)
  {
    return false;
  }

  bool FileSystem::cd(const std::string& path)
  {
    return false;
  }

  bool FileSystem::mv(const std::string& from, const std::string& to)
  {
    return false;
  }

  bool FileSystem::cp(const std::string& from, const std::string& to)
  {
    return false;
  }

  std::string FileSystem::cat(const std::string& name) const
  {
    return "";
  }

  std::string FileSystem::pwd() const
  {
    return "/";
  }

  topit::Vector< std::string > FileSystem::ls() const
  {
    return {};
  }

  std::string FileSystem::tree(const std::string& path) const
  {
    return "";
  }

  topit::Vector< std::string > FileSystem::search(const std::string& name) const
  {
    return {};
  }
}
