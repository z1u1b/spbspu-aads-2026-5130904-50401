#include <string>
#include "../common/my_tree.hpp"
#include "../common/my_compare.hpp"
#include "../common/top-it-vector.hpp"
namespace zubarev
{
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
}
