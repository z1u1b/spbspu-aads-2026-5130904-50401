#pragma once
#include <string>
#include <memory>
#include <cstdint>
#include "robin_hashtable.hpp"
#include "../common/top-it-vector.hpp"
// #include "../common/my_siphash.hpp"
// #include "../common/my_equal.hpp"
#include "my_siphash.hpp"
#include "my_equal.hpp"
#include "haffman.cpp"
namespace zubarev
{
  struct FileMetaData
  {
    std::string date;
    std::string owner;
  };

  // 2. Блок данных для дедупликации и сжатия Хаффмана
  struct DataBlock
  {
    // topit::Vector< uint8_t > compressed_data; // Сжатый поток бит
    std::string compressed_data;
    std::shared_ptr< BSTree< char, std::string, Comparator< char > > >
        out_dictionary;       // Таблица частот / дерево Хаффмана для распаковки
    std::string content_hash; // Хэш исходных данных
    size_t original_size;     // Размер до сжатия (полезно для контроля)
  };

  // 3. Базовый интерфейс для узла ФС
  class FSNode
  {
  public:
    virtual ~FSNode() = default;

    virtual bool isDirectory() const noexcept = 0;

    virtual const FileMetaData& getMeta() const noexcept = 0;
    virtual void setMeta(const FileMetaData& meta) = 0;
  };

  // 4. Класс Файла
  class File : public FSNode
  {
  public:
    File(FileMetaData m):
      meta(std::move(m))
    {}

    bool isDirectory() const noexcept override
    {
      return false;
    }

    const FileMetaData& getMeta() const noexcept override
    {
      return meta;
    }
    void setMeta(const FileMetaData& m) override
    {
      meta = m;
    }

    // Геттеры и сеттеры для блоков данных (для Хаффмана и дедупликации)
    const topit::Vector< std::shared_ptr< DataBlock > >& getBlocks() const noexcept
    {
      return blocks;
    }
    void addBlock(std::shared_ptr< DataBlock > block)
    {
      blocks.pushBack(std::move(block));
    }
    void clearBlocks()
    {
      blocks.clear();
    }

  private:
    FileMetaData meta;
    topit::Vector< std::shared_ptr< DataBlock > > blocks;
  };

  // 5. Класс Директории
  class Directory : public FSNode
  {
  public:
    Directory(FileMetaData m):
      meta(std::move(m))
    {}

    bool isDirectory() const noexcept override
    {
      return true;
    }

    const FileMetaData& getMeta() const noexcept override
    {
      return meta;
    }
    void setMeta(const FileMetaData& m) override
    {
      meta = m;
    }

    // const topit::Vector< std::string >& getSubNodeNames() const noexcept
    // {
    //   return sub_nodes_;
    // }
    // void addSubNodeName(const std::string& name)
    // {
    //   sub_nodes_.push_back(name);
    // }

  private:
    FileMetaData meta;
    RobinHashTable< std::string, std::shared_ptr< FSNode >, SipHash, Equaler< std::string > > children_;
  };

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
    std::shared_ptr< FSNode > resolvePath(const std::string& path);
    RobinHashTable< std::string, std::shared_ptr< FSNode >, SipHash, Equaler< std::string > > directories_;
    std::string current_path_;
  };

}
