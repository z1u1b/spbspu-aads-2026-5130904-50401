#include "input.hpp"

#include <iostream>

namespace zubarev
{
  void inputDataset(std::istream& in, std::ostream&, DatasetTable& datasets)
  {
    std::string dataset_name;
    while (in >> dataset_name) {
      BSTree< size_t, std::string, Comparator< size_t > > dataset;
      size_t key = 0;
      std::string value;

      while (in >> key >> value) {

        dataset.push(key, value);
        if (in.peek() == '\n') {
          in.get();
          break;
        }
      }

      datasets.insertDataset(dataset_name, dataset);
    }
  }
}
