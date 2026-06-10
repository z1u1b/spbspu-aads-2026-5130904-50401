#include "file_system.hpp"
#include <stdexcept>
#include "utils.hpp"
namespace zubarev
{
  bool FileSystem::mkdir(const std::string& name_dir)
  {
    if (curr_dir_->children_.has(name_dir)) {
      throw std::runtime_error("mkdir: " + name_dir + ": File exists");
    }
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_dir = std::make_shared< Directory >(new_data);
    curr_dir_->children_.add(name_dir, new_dir);
    return true;
  }

  bool FileSystem::rmdir(const std::string& name_dir)
  {
    if (curr_dir_->children_.has(name_dir)) {
      throw std::runtime_error("rmdir: " + name_dir + ": No such file or directory");
    }
    curr_dir_->children_.drop(name_dir);
    return true;
  }
  bool FileSystem::rm(const std::string& name)
  {
    if (curr_dir_->children_.has(name)) {
      throw std::runtime_error("rm: " + name + ": No such file or directory");
    } else if (curr_dir_->children_[name]->isDirectory()) {
      throw std::runtime_error("rm: " + name + ": is a directory");
    }
    curr_dir_->children_.drop(name);
    return true;
  }

  bool FileSystem::touch(const std::string& name_file)
  {
    if (curr_dir_->children_.has(name_file)) {
      throw std::runtime_error("mkdir: " + name_file + ": File exists");
    }
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_file = std::make_shared< File >(new_data);
    curr_dir_->children_.add(name_file, new_file);
    return true;
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
