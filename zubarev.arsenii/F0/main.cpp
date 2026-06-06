#include <iostream>
#include <variant>
class FSNode
{
  virtual ~FSNode() = default;
};
struct Slot
{
  std::string path;
  std::unique_ptr< FSNode > val;
  uint8_t psl;
  bool occupied;
};

struct DataBlock
{
  std::vector< uint8_t > compressed_data;
  std::vector< uint8_t > codebook;
  std::string content_hash;
  size_t original_size;
};
struct FileMetaData
{
  std::string date;
  std::string owner;
};
class File : public FSNode
{
  FileMetaData meta;
  std::vector< std::shared_ptr< DataBlock > > blocks;
};

class Directory : FSNode
{
  FileMetaData meta;
  std::vector< std::shared_ptr< FSNode > > files;
};

int main()
{
  std::vector< Slot > table;
}
