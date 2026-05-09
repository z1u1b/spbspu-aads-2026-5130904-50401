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
    // private:
    //   using innerTable = HashTable< std::string, topit::Vector< size_t >, SipHash, Equaler< std::string > >;
    //   using vertexTable = HashTable< std::string, innerTable, SipHash, Equaler< std::string > >;

    //   using Table = HashTable< std::string, vertexTable, SipHash, Equaler< std::string > >;
    //   Table data_;

  private:
    using EdgeKey = std::pair< std::string, std::string >;
    using Weights = topit::Vector< size_t >;

    using Table = HashTable< std::string,
                             HashTable< EdgeKey, Weights, SipHash, Equaler< std::pair< std::string, std::string > > >,
                             SipHash,
                             Equaler< std::string > >;
    Table data_;

    static bool compare(const EdgeKey& a, const EdgeKey& b)
    {
      return a.first < b.first;
    }

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

}

#endif
