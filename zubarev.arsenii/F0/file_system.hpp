#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>
#include <memory>

#include "robin_hashtable.hpp"
#include "fs_nodes.hpp"
#include "blockkey.hpp"

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
    std::tuple< std::string, size_t, size_t > tree(const std::string& path) const;
    topit::Vector< std::string > search(const std::string& name) const;

    struct StateInfo
    {
      std::string name;
      size_t size_kb;
      std::string date;
    };
    // Работа с постоянным хранилищем сессий и внешними файлами
    bool save(const std::string& name_node);
    bool load(const std::string& name_node);
    std::vector< StateInfo > states(const std::string& path = ".") const;
    bool import_file(const std::string& real_path, const std::string& virtual_name);
    bool export_file(const std::string& virtual_name, const std::string& real_path);

    // bool save_state();

    // bool archive(const std::string& name_dir);

    // bool start_state();
    // bool start_state(const std::string& file_name, bool force = false);

    // Управление кэшированием (LRU)
    void cache_size();
    void cache_on();
    void cache_off();
    void cache_stats();

    // Поисковый движок
    void ssearch();

    // void setStatesDirectory(const std::string& path) {
    //     states_directory_ = path;
    //     // Создать директорию, если не существует
    //     std::filesystem::create_directories(states_directory_);
    // }

    // std::string getStatesDirectory() const {
    //     return states_directory_;
    // }

  private:
    bool isDescendant(const std::shared_ptr< Directory >& root, const std::shared_ptr< FSNode >& candidate) const;
    std::shared_ptr< FSNode > cloneFile(const std::shared_ptr< File >& old_file);
    std::shared_ptr< FSNode > cloneDirectory(const std::shared_ptr< Directory >& old_directory);

    struct FindResult
    {
      std::shared_ptr< Directory > parent_dir_ = nullptr;
      std::shared_ptr< FSNode > target_ = nullptr;
      std::string target_name_ = "";
    };

    std::shared_ptr< FSNode > navigateTo(const std::string& path) const;

    void treeImpl(std::shared_ptr< Directory >, const std::string&, std::string&, size_t&, size_t&) const;

    std::shared_ptr< Directory > root_;
    std::shared_ptr< Directory > curr_dir_;
    // std::string current_path_str_;

    RobinHashTable< BlockKey, std::shared_ptr< DataBlock >, BlockKeyHash, BlockKeyEqual > data_block_;
    RobinHashTable< std::string, std::shared_ptr< FSNode >, SipHash, Equaler< std::string > > session_storage_;
    // std::string states_directory_ = "./states/";
  };

}

#endif
