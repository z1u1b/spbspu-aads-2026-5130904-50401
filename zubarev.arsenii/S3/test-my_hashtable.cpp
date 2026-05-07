#include "my_hashtable.hpp"
#include <iostream>
#include <string>

namespace zubarev
{
  int test()
  {
    using Table = zubarev::HashTable< std::string, int, std::hash< std::string >, std::equal_to< std::string > >;

    Table table;

    std::cout << "=== ADD TEST ===\n";
    table.add("apple", 1);
    table.add("banana", 2);
    table.add("cherry", 3);
    table.add("date", 4);
    table.add("elderberry", 5);

    std::cout << "Elements added\n\n";

    // -----------------------------

    std::cout << "=== HAS TEST ===\n";
    std::cout << "apple: " << table.has("apple") << "\n";
    std::cout << "banana: " << table.has("banana") << "\n";
    std::cout << "grape: " << table.has("grape") << "\n\n";

    // -----------------------------

    std::cout << "=== ITERATOR TEST ===\n";
    for (auto it = table.begin(); it != table.end(); ++it) {
      std::cout << it->key_ << " : " << it->val_ << "\n";
    }
    std::cout << "\n";

    // -----------------------------

    std::cout << "=== CONST ITERATOR TEST ===\n";
    const Table& const_ref = table;
    for (auto it = const_ref.cbegin(); it != const_ref.cend(); ++it) {
      std::cout << it->key_ << " : " << it->val_ << "\n";
    }
    std::cout << "\n";

    // -----------------------------

    std::cout << "=== DROP TEST ===\n";
    try {
      int val = table.drop("banana");
      std::cout << "Dropped banana = " << val << "\n";
    } catch (...) {
      std::cout << "Error dropping banana\n";
    }

    std::cout << "banana exists? " << table.has("banana") << "\n\n";

    // -----------------------------

    std::cout << "=== ITER AFTER DROP ===\n";
    for (auto it = table.begin(); it != table.end(); ++it) {
      std::cout << it->key_ << " : " << it->val_ << "\n";
    }
    std::cout << "\n";

    // -----------------------------

    std::cout << "=== COPY TEST ===\n";
    Table copy_table = table;

    for (auto it = copy_table.begin(); it != copy_table.end(); ++it) {
      std::cout << it->key_ << " : " << it->val_ << "\n";
    }
    std::cout << "\n";

    // -----------------------------

    std::cout << "=== MOVE TEST ===\n";
    Table moved_table = std::move(copy_table);

    std::cout << "Moved table:\n";
    for (auto it = moved_table.begin(); it != moved_table.end(); ++it) {
      std::cout << it->key_ << " : " << it->val_ << "\n";
    }
    std::cout << "\n";

    // -----------------------------

    std::cout << "=== EDGE CASES ===\n";

    try {
      table.drop("not_exist");
    } catch (...) {
      std::cout << "Correctly handled drop of non-existing key\n";
    }

    std::cout << "\n";

    // -----------------------------

    std::cout << "=== LARGE INSERT TEST ===\n";
    for (int i = 0; i < 50; ++i) {
      table.add("key" + std::to_string(i), i);
    }

    std::cout << "Inserted 50 elements\n";

    int count = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
      count++;
    }

    std::cout << "Total elements counted by iterator: " << count << "\n";

    std::cout << "\n=== DONE ===\n";

    return 0;
  }

}
