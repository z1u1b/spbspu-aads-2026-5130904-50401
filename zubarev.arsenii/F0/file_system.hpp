#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>
#include <memory>

#include "robin_hashtable.hpp"

#include "my_siphash.hpp"
#include "my_equal.hpp"
#include "fs_nodes.hpp"

namespace zubarev
{

  class FileSystem
  {
  public:
    // Основные операции файловой системы
    // Модифицирующие методы (возвращают bool для обработки ошибок в контроллере)
    bool mkdir(const std::string& name_dir);
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
    topit::Vector< std::string > ls() const;
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
    // std::shared_ptr< FSNode > resolvePath(const std::string& path);

    std::shared_ptr< Directory > root_;
    std::shared_ptr< Directory > curr_dir_;
    std::string current_path_str_;

    RobinHashTable< std::string, std::shared_ptr< DataBlock >, SipHash, Equaler< std::string > > data_block_;
  };

}

#endif
