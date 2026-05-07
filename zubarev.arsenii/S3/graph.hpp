#ifndef GRAPH_TABLE_HPP
#define GRAPH_TABLE_HPP

#include "my_equal.hpp"
#include "my_hashtable.hpp"
#include "my_siphash.hpp"
#include <iostream>
#include <string>

#include "../common/top-it-vector.hpp"
namespace zubarev
{
  class GraphTable
  {
  private:
    using innerTable = HashTable< std::string, topit::Vector< size_t >, SipHash, Equaler< std::string > >;
    using vertexTable = HashTable< std::string, innerTable, SipHash, Equaler< std::string > >;

    using Table = HashTable< std::string, vertexTable, SipHash, Equaler< std::string > >;
    Table data_;

  public:
    void graphs(std::ostream&) const;
    void vertexes(const std::string&, std::ostream&) const;
    void outbound(const std::string&, const std::string&, std::ostream&) const;
    void inbound(const std::string&, const std::string&, std::ostream&) const;
    void bind(const std::string&, const std::pair< std::string, std::string >&, size_t);
    void cut(const std::string&, const std::pair< std::string, std::string >&, size_t);
    void create(const std::string&, size_t, const topit::Vector< std::string >&);
    void merge(const std::string&, const std::string&, const std::string&);
    void extract(const std::string&, const std::string&, size_t, const topit::Vector< std::string >&);
  };

  // void GraphTable::graphs()
  // {}

  void GraphTable::graphs(std::ostream& out) const
  {
    for (auto it=data_.begin();it!=data_.end();++it) {
      out<<it->key_;
    }
  }

  void GraphTable::vertexes(const std::string& graph_name, std::ostream& out) const
  {}

  void GraphTable::outbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {}

  void GraphTable::inbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {}

  void GraphTable::bind(const std::string& graph_name, const std::pair< std::string, std::string >& edge, size_t weight)
  {}

  void GraphTable::cut(const std::string& graph_name, const std::pair< std::string, std::string >& edge, size_t weight)
  {}

  void GraphTable::create(const std::string& graph_name, size_t count, const topit::Vector< std::string >& vertices)
  {}

  void GraphTable::merge(const std::string& new_name, const std::string& source1, const std::string& source2)
  {}

  void GraphTable::extract(const std::string& new_name,
                           const std::string& source,
                           size_t count,
                           const topit::Vector< std::string >& vertices)
  {}

}

#endif
