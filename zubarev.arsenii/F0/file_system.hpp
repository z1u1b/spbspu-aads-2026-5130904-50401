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
    void mkdir();
    void rm();
    void touch();
    void write();
    void cat();
    void append();
    void cd();
    void pwd();
    void ls();
    void tree();
    void search();
    void mv();
    void cp();

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
