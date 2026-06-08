#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include "robin_hashtable.hpp"
#include "../common/top-it-vector.hpp"
#include "../common/my_siphash.hpp"
#include "../common/my_equal.hpp"
namespace zubarev
{
  // 1. Метаданные (сделаем структуры открытыми или добавим конструкторы)
  struct FileMetaData
  {
    std::string date;
    std::string owner;
  };

  // 2. Блок данных для дедупликации и сжатия Хаффмана
  struct DataBlock
  {
    topit::Vector< uint8_t > compressed_data; // Сжатый поток бит
    topit::Vector< uint8_t > codebook;        // Таблица частот / дерево Хаффмана для распаковки
    std::string content_hash;                 // Хэш исходных данных (например, MD5/SHA или простой хэш)
    size_t original_size;                     // Размер до сжатия (полезно для контроля)
  };

  // 3. Базовый интерфейс для узла ФС
  class FSNode
  {
  public:
    virtual ~FSNode() = default;

    // Чисто виртуальный метод для определения типа узла (вместо тяжелого dynamic_cast)
    virtual bool isDirectory() const noexcept = 0;

    // Общий интерфейс для работы с метаданными
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

    const topit::Vector< std::string >& getSubNodeNames() const noexcept
    {
      return sub_nodes_;
    }
    void addSubNodeName(const std::string& name)
    {
      sub_nodes_.push_back(name);
    }

  private:
    FileMetaData meta;
    RobinHashTable< std::string, std::shared_ptr< FSNode >, SipHash, Equaler< std::string > > children;
  };

}
