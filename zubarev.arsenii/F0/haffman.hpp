#include <string>
#include "../common/my_tree/my_tree.hpp"
#include "../common/my_tree/my_compare.hpp"
namespace zubarev
{
  class Huffman
  {
  public:
    std::string encode(const std::string&);
    std::string decode(const std::string&);

  private:
    BSTree< char, std::string, Comparator< char > > codes_;
    BSTreeNode< size_t, char >* root_;

    BSTree< char, size_t, Comparator< char > > calculateLetters(const std::string& text);
    BSTreeNode< size_t, char >* treeLetters(BSTree< char, size_t, Comparator< char > > frequency);

    void generateCodes(const BSTreeNode< size_t, char >* node,
                       const std::string& code,
                       BSTree< char, std::string, Comparator< char > >& out_dictionary);
  };

}
