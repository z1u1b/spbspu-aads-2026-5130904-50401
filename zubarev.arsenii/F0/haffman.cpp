#include "haffman.hpp"
#include "../common/my_vector/top-it-vector.hpp"

namespace zubarev
{
  void destroy(BSTreeNode< size_t, char >* node)
  {
    if (!node) {
      return;
    }

    destroy(node->left_);
    destroy(node->right_);

    node->left_ = node->right_ = node->parent_ = nullptr;
    delete node;
  }
  Huffman::Huffman():
    root_(nullptr)
  {}
  Huffman::~Huffman()
  {
    destroy(root_);
  }
  std::string Huffman::encode(const std::string& text)
  {
    BSTree< char, size_t, Comparator< char > > freq_let = calculateLetters(text);
    root_ = treeLetters(freq_let);

    generateCodes(root_, "", codes_);

    std::string out_code = "";
    for (auto it = text.begin(); it != text.end(); ++it) {
      out_code += codes_[*it];
    }
    return out_code;
  }
  std::string Huffman::decode(const std::string& code)
  {
    auto current = root_;
    std::string out_str = "";
    for (auto it = code.begin(); it != code.end(); ++it) {
      if (*it == '0') {
        current = current->left_;
      } else {
        current = current->right_;
      }
      if (current->data_.second != '\0') {
        out_str += current->data_.second;
        current = root_;
      }
    }
    return out_str;
  }
  BSTree< char, size_t, Comparator< char > > calculateLetters(const std::string& text)
  {
    BSTree< char, size_t, Comparator< char > > letters;

    for (auto it = text.begin(); it != text.end(); ++it) {

      letters[*it]++;
    }
    return letters;
  }

  BSTreeNode< size_t, char >* treeLetters(BSTree< char, size_t, Comparator< char > > frequency)
  {
    using HuffNode = BSTreeNode< size_t, char >;

    topit::Vector< HuffNode* > tree;
    for (auto it = frequency.begin(); it != frequency.end(); ++it) {
      tree.pushBack(new HuffNode(it->second, it->first));
    }

    auto tree_comp = [](const HuffNode* a, const HuffNode* b) { return a->data_.first > b->data_.first; };

    while (tree.getSize() > 1) {
      std::sort(tree.begin(), tree.end(), tree_comp);

      HuffNode* left_child = tree.back();
      tree.popBack();

      HuffNode* right_child = tree.back();
      tree.popBack();

      size_t sum_weight = left_child->data_.first + right_child->data_.first;
      HuffNode* parent_node = new HuffNode(sum_weight, '\0');
      parent_node->left_ = left_child;
      parent_node->right_ = right_child;
      left_child->parent_ = parent_node;
      right_child->parent_ = parent_node;

      tree.pushBack(parent_node);
    }
    return tree.front();
  }

  void generateCodes(const BSTreeNode< size_t, char >* node,
                     const std::string& code,
                     BSTree< char, std::string, Comparator< char > >& out_dictionary)
  {
    if (!node) {
      return;
    }
    if (node->data_.second != '\0') {
      out_dictionary[node->data_.second] = code;
    }

    generateCodes(node->left_, code + "0", out_dictionary);

    generateCodes(node->right_, code + "1", out_dictionary);
  }
}
