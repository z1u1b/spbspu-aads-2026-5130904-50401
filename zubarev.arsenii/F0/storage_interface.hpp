#include "filesystem.hpp"
namespace zubarev
{
  // Абстрактный слой доступа
  class IFileSystemStorage
  {
  public:
    virtual ~IFileSystemStorage() = default;
    virtual std::shared_ptr< FSNode > find(const std::string& path) = 0;
    virtual void insert(const std::string& path, std::shared_ptr< FSNode > node) = 0;
    virtual void remove(const std::string& path) = 0;
  };

  class TreeStorage : IFileSystemStorage
  {
  public:
  }
}
