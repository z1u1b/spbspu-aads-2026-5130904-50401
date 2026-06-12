#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>
#include <memory>

#include "robin_hashtable.hpp"

#include "my_siphash.hpp"
#include "my_equal.hpp"
#include "fs_nodes.hpp"
#include "utils.hpp"

namespace zubarev
{

  class FileSystem
  {
  public:
    FileSystem();
    ~FileSystem() = default;
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;
    // Основные операции файловой системы
    // Модифицирующие методы (возвращают bool для обработки ошибок в контроллере)
    bool mkdir(const std::string& name_dir);
    bool rmdir(const std::string& name_dir);
    bool rm(const std::string& name);
    bool touch(const std::string& name_file);
    bool write(const std::string& name, const std::string& text);
    bool append(const std::string& name, const std::string& text);
    bool cd(const std::string& path);
    bool mv(const std::string& from, const std::string& to);
    bool cp(const std::string& from, const std::string& to);

    // Методы чтения (const!)
    std::string cat(const std::string& name) const;
    std::string pwd() const;
    topit::Vector< std::string > ls(const std::string& path) const;
    std::string tree(const std::string& path) const;
    topit::Vector< std::string > search(const std::string& name) const;

    // Работа с постоянным хранилищем сессий и внешними файлами
    void save();
    void load();
    void archive();
    void states();
    void import();
    void export_s();
    void start_state();

    // Управление кэшированием (LRU)
    void cache_size();
    void cache_on();
    void cache_off();
    void cache_stats();

    // Поисковый движок
    void ssearch();

  private:
    std::shared_ptr< FSNode > cloneFile(const std::shared_ptr< File >& old_file);
    std::shared_ptr< FSNode > cloneDirectory(const std::shared_ptr< Directory >& old_directory);

    struct FindResult
    {
      std::shared_ptr< Directory > parent_dir_ = nullptr;
      std::shared_ptr< FSNode > target_ = nullptr;
      std::string target_name_ = "";
    };

    FindResult navigateTo(const std::string& path) const
    {
      FindResult result;
      Queue< std::string > dirs = detail::resolvePath(path);
      if (dirs.empty()) {
        return result;
      }
      std::shared_ptr< Directory > current = curr_dir_;
      if (path[0] == '/') {
        current = root_;
      }
      while (!dirs.empty()) {
        auto next_name = dirs.top();
        dirs.drop();

        auto it = current->children_.find(next_name);

        if (dirs.empty()) {
          result.parent_dir_ = current;
          result.target_name_ = next_name;
          if (it != current->children_.end()) {
            {
              result.target_ = it->getValue();
            }
            return result;
          }

          if (it == current->children_.end() || !it->getValue()->isDirectory()) {
            return result;
          }
          current = std::static_pointer_cast< Directory >(it->getValue());
        }
      }
      return result;
    }
    std::shared_ptr< Directory > root_;
    std::shared_ptr< Directory > curr_dir_;
    std::string current_path_str_;

    RobinHashTable< uint64_t, std::shared_ptr< DataBlock >, SipHash, Equaler< uint64_t > > data_block_;
  };

}

#endif
