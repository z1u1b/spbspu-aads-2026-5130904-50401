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
    void mkdir(const std::string& name_dir);
    void rm(const std::string& name);
    void touch(const std::string& name_file);
    void write(const std::string& name, const std::string& text);
    void cat(const std::string& name);
    void append(const std::string& name, const std::string& text);
    void cd(const std::string& path);
    void pwd();
    void ls();
    void tree(const std::string& path);
    void search(const std::string& name);
    void mv(const std::string&, const std::string&);
    void cp(const std::string&, const std::string&);

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
