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

static bool compare(const EdgeKey& a, const EdgeKey& b) { return a.first < b.first; }

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
    topit::Vector< std::string > names;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      names.pushBack(it->key_);
    }
    std::sort(names.begin(), names.end());
    for (auto it = names.begin(); it != names.end(); ++it) {
      out << *it << '\n';
    }
  }

  void GraphTable::vertexes(const std::string& graph_name, std::ostream& out) const
  {
    const auto& graph = data_.at(graph_name);
    topit::Vector< std::string > verts;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      verts.pushBack(it->key_.first);
      verts.pushBack(it->key_.second);
    }
    std::sort(verts.begin(), verts.end());
    auto last = std::unique(verts.begin(), verts.end());
    verts.erase(last, verts.end());

    for (auto it = verts.begin(); it != verts.end(); ++it) {
      out << *it << '\n';
    }
  }

  void GraphTable::outbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {
    const auto& graph = data_.at(graph_name);
    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;
    Equaler< std::string > eq;
    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.first, vertex)) {
        std::pair< std::string, topit::Vector< size_t > > tmp;
        tmp.first = it->key_.second;
        tmp.second = it->val_;
        results.pushBack(tmp);
      }
    }
    std::sort(results.begin(), results.end(), compare);
    for (auto it = results.begin(); it != results.end(); ++it) {
      out << (*it).first;
      for (auto w = (*it).second.begin(); w != (*it).second.end(); ++w) {
        out << " ";
        out << *w;
      }
      out << '\n';
    }
  }

  void GraphTable::inbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {
    // const auto& graph = data_.at(graph_name);
    // graph.at(vertex);
    // topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;
    // for (auto it = graph.begin(); it != graph.end(); ++it) {
    //   for (auto it2 = it->val_.begin(); it2 != it->val_.end(); ++it2) {
    //     if (Equaler< std::string >{}(it2->key_, vertex)) {
    //       std::pair< std::string, topit::Vector< size_t > > tmp;
    //       tmp.first = it->key_;

    //       for (auto vit = it2->val_.cbegin(); vit != it2->val_.cend(); ++vit) {
    //         tmp.second.pushBack(*vit);
    //       }
    //       results.pushBack(tmp);
    //     }
    //   }
    // }
    // std::sort(results.begin(), results.end(), compare);
    // for (auto it = results.begin(); it != results.end(); ++it) {
    //   out << (*it).first << '\n';
    // }
    const auto& graph = data_.at(graph_name);
    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;
    Equaler< std::string > eq;
    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.second, vertex)) {
        std::pair< std::string, topit::Vector< size_t > > tmp;
        tmp.first = it->key_.first;
        tmp.second = it->val_;
        results.pushBack(tmp);
      }
    }
    std::sort(results.begin(), results.end(), compare);
    for (auto it = results.begin(); it != results.end(); ++it) {
      out << (*it).first;
      for (auto w = (*it).second.begin(); w != (*it).second.end(); ++w) {
        out << " ";
        out << *w;
      }
      out << '\n';
    }
  }

  void GraphTable::bind(const std::string& graph_name, const std::pair< std::string, std::string >& edge, size_t weight)
  {
    
  }

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
