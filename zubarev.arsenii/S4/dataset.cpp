#include "dataset.hpp"
namespace zubarev
{
  void DatasetTable::insertDataset(const std::string& name,
                                   const BSTree< size_t, std::string, Comparator< size_t > >& dataset)
  {
    datasetsTable.push(name, dataset);
  }
  void DatasetTable::print(const std::string&, std::ostream&) const
  {}
  void DatasetTable::complement(const std::string&, const std::string&, const std::string&, std::ostream&)
  {}
  void DatasetTable::intersect(const std::string&, const std::string&, const std::string&, std::ostream&)
  {}
  void DatasetTable::union_fun(const std::string&, const std::string&, const std::string&, std::ostream&)
  {}
}
