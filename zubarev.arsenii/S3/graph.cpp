#include "graph.hpp"
namespace zubarev
{
  void GraphTable::graphs(std::ostream& out) const
  {
    topit::Vector< std::string > names;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      names.pushBack(it->key_);
    }
    zubarev::sort(names.begin(), names.end(), [](const std::string& a, const std::string& b) { return a < b; });
    for (auto it = names.begin(); it != names.end(); ++it) {
      out << *it << '\n';
    }
  }

  void GraphTable::vertexes(const std::string& graph_name, std::ostream& out) const
  {

    if (!data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }

    const auto& graph = data_.at(graph_name);
    topit::Vector< std::string > verts;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      verts.pushBack(it->key_.first);
      verts.pushBack(it->key_.second);
    }
    zubarev::sort(verts.begin(), verts.end(), [](const std::string& a, const std::string& b) { return a < b; });
    auto last =
        zubarev::unique(verts.begin(), verts.end(), [](const std::string& a, const std::string& b) { return a == b; });
    // verts.erase(last, verts.end());
    while (last != verts.end()) {
      verts.popBack();
    }

    for (auto it = verts.begin(); it != verts.end(); ++it) {
      out << *it << '\n';
    }
  }

  void GraphTable::outbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {
    if (!data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    Equaler< std::string > eq;

    const auto& graph = data_.at(graph_name);
    bool find = false;
    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.first, vertex)) {
        find = true;
        break;
      }
    }

    if (!find) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.first, vertex)) {
        std::pair< std::string, topit::Vector< size_t > > tmp;
        tmp.first = it->key_.second;
        tmp.second = it->val_;
        results.pushBack(tmp);
      }
    }
    zubarev::sort(results.begin(), results.end(), compare);
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
    if (!data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    Equaler< std::string > eq;

    const auto& graph = data_.at(graph_name);
    bool find = false;
    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.second, vertex)) {
        find = true;
        break;
      }
    }

    if (!find) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }

    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.second, vertex)) {
        std::pair< std::string, topit::Vector< size_t > > tmp;
        tmp.first = it->key_.first;
        tmp.second = it->val_;
        results.pushBack(tmp);
      }
    }
    zubarev::sort(results.begin(), results.end(), compare);
    for (auto it = results.begin(); it != results.end(); ++it) {
      out << (*it).first;
      for (auto w = (*it).second.begin(); w != (*it).second.end(); ++w) {
        out << " ";
        out << *w;
      }
      out << '\n';
    }
  }

  void GraphTable::bind(const std::string& graph_name,
                        const std::pair< std::string, std::string >& edge,
                        size_t weight,
                        std::ostream& out)
  {
    if (!data_.has(graph_name)) {
      create(graph_name, 0, {}, out);
    } else {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    auto& graph = data_.at(graph_name);
    graph[edge].pushBack(weight);
  }

  void GraphTable::cut(const std::string& graph_name,
                       const std::pair< std::string, std::string >& edge,
                       size_t weight,
                       std::ostream& out)
  {
    if (data_.has(graph_name)) {

      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    auto& graph = data_.at(graph_name);
    if (!graph.has(edge)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    auto& weights = graph.at(edge);
    bool find = false;

    for (size_t i = 0; i < weights.getSize(); ++i) {
      if (weights[i] == weight) {
        weights.erase(i);
        find = true;
        return;
      }
    }
    if (!find) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    // graph[edge].erase(weight);
  }

  bool GraphTable::create(const std::string& graph_name,
                          size_t count,
                          const topit::Vector< std::string >& vertices,
                          std::ostream& out)
  {
    if (data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return false;
    }
    if (count != vertices.getSize()) {
      out << "<INVALID COMMAND>" << "\n";
      return false;
    }

    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;
    EdgeTable new_table;

    data_.add(graph_name, new_table);
    return true;
  }

  void GraphTable::merge(const std::string& new_name,
                         const std::string& source1,
                         const std::string& source2,
                         std::ostream& out)
  {

    if (!(data_.has(source1) && data_.has(source2))) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (!create(new_name, 0, {}, out)) {
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
                           const topit::Vector< std::string >& vertices,
                           std::ostream& out)
  {

    if (!data_.has(source)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (count != vertices.getSize()) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (!create(new_name, count, vertices, out)) {
      return;
    }

    Equaler< std::string > eq_;
    const auto& graph = data_.at(source);
    for (size_t i = 0; i < count; ++i) {
      bool found = false;
      for (auto it = graph.begin(); it != graph.end(); ++it) {
        if (eq_(it->key_.first, vertices[i]) || eq_(it->key_.second, vertices[i])) {
          found = true;
          break;
        }
      }
      if (!found) {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }
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
