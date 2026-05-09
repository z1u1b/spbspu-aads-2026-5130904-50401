#include "graph.hpp"
namespace zubarev
{
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
    auto& graph = data_.at(graph_name);
    graph[edge].pushBack(weight);
  }

  void GraphTable::cut(const std::string& graph_name, const std::pair< std::string, std::string >& edge, size_t weight)
  {
    auto& graph = data_.at(graph_name);
    auto& weights = graph[edge];

    for (size_t i = 0; i < weights.getSize(); ++i) {
      if (weights[i] == weight) {
        weights.erase(i);
        return;
      }
    }
    graph[edge].erase(weight);
  }

  bool GraphTable::create(const std::string& graph_name, size_t count, const topit::Vector< std::string >& vertices)
  {
    if (data_.has(graph_name)) {
      std::cout << "<INVALID COMMAND>" << "\n";
      return false;
    }
    if (count != vertices.getSize()) {
      std::cout << "<INVALID COMMAND>" << "\n";
      return false;
    }

    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;
    EdgeTable new_table;

    data_.add(graph_name, new_table);
    return true;
  }

  void GraphTable::merge(const std::string& new_name, const std::string& source1, const std::string& source2)
  {

    if (!(data_.has(source1) && data_.has(source2))) {
      std::cout << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (!create(new_name, 0, {})) {
      return;
    }
    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;

    EdgeTable new_table = data_.at(new_name);
    const EdgeTable& source1_table = data_[source1];
    const EdgeTable& source2_table = data_[source2];

    for (auto it = source1_table.begin(); it != source1_table.end(); ++it) {
      Weights weights(it->val_);
      for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
        data_.at(new_name)[it->key_].pushBack(*vit);
      }
    }

    for (auto it = source2_table.begin(); it != source2_table.end(); ++it) {
      Weights weights(it->val_);
      for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
        data_.at(new_name)[it->key_].pushBack(*vit);
      }
    }
  }

  void GraphTable::extract(const std::string& new_name,
                           const std::string& source,
                           size_t count,
                           const topit::Vector< std::string >& vertices)
  {

    if (!data_.has(source)) {
      std::cout << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (count != vertices.getSize()) {
      std::cout << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (!create(new_name, count, vertices)) {
      return;
    }
    Equaler< std::string > eq_;
    const auto& graph = data_.at(source);
    for (size_t i = 0; i < count; ++i) {
      for (auto it = graph.begin(); it != graph.end(); ++it) {
        if (eq_(it->key_.first, vertices[i])) {
          Weights weights(it->val_);
          for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
            data_.at(new_name)[it->key_].pushBack(*vit);
          }
        }
      }
    }
  }

}
