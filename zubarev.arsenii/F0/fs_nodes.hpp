#ifndef FS_NODES_HPP
#define FS_NODES_HPP

#include <string>
#include <memory>

#include "robin_hashtable.hpp"
#include <top-it-vector.hpp>
#include <my_siphash.hpp>
#include "blockkey.hpp"
#include <my_equal.hpp>
#include "haffman.hpp"
namespace zubarev
{
  struct FileMetaData
  {
    std::string date;
    std::string owner;
  };

  struct DataBlock
  {
    topit::Vector< uint8_t > compressed_data;
    std::shared_ptr< BSTree< char, std::string, Comparator< char > > > out_dictionary;
    BlockKey content_hash;
    size_t original_size;
    size_t total_bits_count;
  };

  class Directory;
  class FSNode
  {
  public:
    virtual ~FSNode() = default;

    virtual bool isDirectory() const noexcept = 0;

    virtual const FileMetaData& getMeta() const noexcept = 0;
    virtual void setMeta(const FileMetaData& meta) = 0;

    void
    setParent(std::weak_ptr< Directory > p)
    {
      parent_ = p;
    }
    std::shared_ptr< Directory >
    getParent() const
    {
      return parent_.lock();
    }

    void
    setName(const std::string& n)
    {
      name_ = n;
    }
    std::string
    getName() const
    {
      return name_;
    }

  private:
    std::weak_ptr< Directory > parent_;
    std::string name_;
  };

  class File : public FSNode
  {
  public:
    File(FileMetaData m):
      meta_(std::move(m))
    {}

    bool
    isDirectory() const noexcept override
    {
      return false;
    }

    const FileMetaData&
    getMeta() const noexcept override
    {
      return meta_;
    }
    void
    setMeta(const FileMetaData& m) override
    {
      meta_ = m;
    }

    const topit::Vector< std::shared_ptr< DataBlock > >&
    getBlocks() const noexcept
    {
      return blocks_;
    }
    void
    addBlock(std::shared_ptr< DataBlock > block)
    {
      blocks_.pushBack(std::move(block));
    }
    void
    clearBlocks()
    {
      blocks_.clear();
    }

  private:
    FileMetaData meta_;
    topit::Vector< std::shared_ptr< DataBlock > > blocks_;
  };

  class Directory : public FSNode, public std::enable_shared_from_this< Directory >
  {
    friend class FileSystem;

  public:
    Directory(FileMetaData m):
      meta(std::move(m))
    {}

    bool
    isDirectory() const noexcept override
    {
      return true;
    }

    const FileMetaData&
    getMeta() const noexcept override
    {
      return meta;
    }
    void
    setMeta(const FileMetaData& m) override
    {
      meta = m;
    }

    void
    addChild(const std::string& name, std::shared_ptr< FSNode > node)
    {
      node->setName(name);
      node->setParent(shared_from_this());
      children_.add(name, node);
    }
    void
    removeChild(const std::string& name)
    {
      children_.drop(name);
    }

  private:
    FileMetaData meta;
    RobinHashTable< std::string, std::shared_ptr< FSNode >, SipHash, Equaler< std::string > > children_;
  };
}

#endif
