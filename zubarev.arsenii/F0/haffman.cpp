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

  void treeLetters(BSTree< char, size_t, Comparator< char > > frequency)
  {
    using HuffNode = BSTreeNode< size_t, char >;

    topit::Vector< HuffNode > tree;
    for (auto it = frequency.begin(); it != frequency.end(); ++it) {
      tree.pushBack(HuffNode(std::make_pair(it->second, it->first), nullptr, nullptr, nullptr));
    }

    auto tree_comp = [](const HuffNode* a, const HuffNode* b) { return a->data_.first > b->data_.first; };

    while (tree.getSize() > 1) {
      std::sort(tree.begin(), tree.end(), tree_comp);

      HuffNode* right_child = *(tree.end());
    }
  }
}
