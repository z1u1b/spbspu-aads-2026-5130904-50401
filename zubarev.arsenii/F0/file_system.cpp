#include "file_system.hpp"
#include <stdexcept>
#include "utils.hpp"
#include "../common/my_queue/queue.hpp"

#include <dirent.h>
#include <sys/stat.h>
#include <ctime>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
// #include <sstream>

namespace zubarev
{
  bool FileSystem::isEmpty() const
  {
    return root_->children_.empty();
  }
  std::shared_ptr< FSNode > FileSystem::navigateTo(const std::string& path) const
  {
    if (path.empty()) {
      return curr_dir_;
    }

    Queue< std::string > dirs = detail::resolvePath(path);
    if (dirs.empty()) {
      return curr_dir_;
    }

    std::shared_ptr< Directory > current = curr_dir_;

    if (path[0] == '/' || dirs.top() == "~") {
      current = root_;
      if (dirs.top() == "~") {
        dirs.drop();
      }
    }

    while (!dirs.empty()) {
      std::string next_name = dirs.top();
      dirs.drop();

      if (next_name == "." || next_name.empty()) {
        continue;
      }
      if (next_name == "..") {
        if (current->getParent()) {
          current = current->getParent();
        }
        continue;
      }
      if (!current->children_.has(next_name)) {
        return nullptr;
      }
      auto it = current->children_.find(next_name);

      if (it == current->children_.end()) {
        return nullptr;
      }

      if (dirs.empty()) {
        return it->val_;
      }
      if (!it->val_->isDirectory()) {
        return nullptr;
      }
      current = std::static_pointer_cast< Directory >(it->val_);
    }

    return current;
  }
  FileSystem::FileSystem()
  {
    FileMetaData root_data;
    root_data.date = detail::getCurrentDateTime();
    root_data.owner = detail::getCurrentUser();
    Directory root_dir(root_data);
    root_ = std::make_shared< Directory >(root_dir);
    curr_dir_ = root_;
  }
  bool FileSystem::mkdir(const std::string& name_dir)
  {
    if (curr_dir_->children_.has(name_dir)) {
      throw std::runtime_error("mkdir: " + name_dir + ": File exists");
    }
    if (name_dir.empty()) {
      throw std::runtime_error("mkdir: empty file name");
    }

    if (name_dir == "." || name_dir == "..") {
      throw std::runtime_error("mkdir: invalid directory name");
    }

    if (name_dir.find('/') != std::string::npos) {
      throw std::runtime_error("mkdir: invalid character '/'");
    }
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_dir = std::make_shared< Directory >(new_data);
    curr_dir_->addChild(name_dir, new_dir);
    return true;
  }

  bool FileSystem::rmdir(const std::string& name_dir)
  {
    if (name_dir.empty()) {
      throw std::runtime_error("rmdir: empty file name");
    }

    if (name_dir == "." || name_dir == "..") {
      throw std::runtime_error("rmdir: invalid directory name");
    }

    if (!curr_dir_->children_.has(name_dir)) {
      throw std::runtime_error("rmdir: " + name_dir + ": No such file or directory");
    }

    auto node = curr_dir_->children_.at(name_dir);
    if (!node->isDirectory()) {
      throw std::runtime_error("rmdir: " + name_dir + ": Not a directory");
    }

    auto dir = std::static_pointer_cast< Directory >(node);
    if (!dir->children_.empty()) {
      throw std::runtime_error("rmdir: " + name_dir + ": Directory not empty");
    }
    curr_dir_->removeChild(name_dir);
    return true;
  }
  bool FileSystem::rm(const std::string& name)
  {
    if (name.empty()) {
      throw std::runtime_error("rm: empty file name");
    }

    if (name == "." || name == "..") {
      throw std::runtime_error("rm: " + name + ": is a directory");
    }

    if (!curr_dir_->children_.has(name)) {
      throw std::runtime_error("rm: " + name + ": No such file or directory");
    }

    auto node = curr_dir_->children_.at(name);
    if (node->isDirectory()) {
      throw std::runtime_error("rm: " + name + ": is a directory");
    }

    curr_dir_->removeChild(name);
    return true;
  }

  bool FileSystem::touch(const std::string& name_file)
  {
    if (name_file.empty()) {
      throw std::runtime_error("touch: empty file name");
    }

    if (name_file == "." || name_file == "..") {
      throw std::runtime_error("touch: invalid file name");
    }

    if (name_file.find('/') != std::string::npos) {
      throw std::runtime_error("touch: invalid character '/'");
    }
    if (curr_dir_->children_.has(name_file)) {
      throw std::runtime_error("touch: " + name_file + ": File exists");
    }
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_file = std::make_shared< File >(new_data);
    curr_dir_->addChild(name_file, new_file);
    return true;
  }

  bool FileSystem::write(const std::string& name_file, const std::string& text)
  {
    if (name_file.empty()) {
      throw std::runtime_error("write: empty file name");
    }

    if (name_file == "." || name_file == "..") {
      throw std::runtime_error("write: invalid file name");
    }

    if (name_file.find('/') != std::string::npos) {
      throw std::runtime_error("write: invalid path in file name");
    }
    if (!curr_dir_->children_.has(name_file)) {
      touch(name_file);
    }

    auto node = curr_dir_->children_.find(name_file);

    if (node->val_->isDirectory()) {
      throw std::runtime_error("write: " + name_file + ": is a directory");
    }
    auto file_ptr = std::static_pointer_cast< File >(curr_dir_->children_[name_file]);
    file_ptr->clearBlocks();

    const size_t BLOCK_SIZE = 4096;
    size_t total_size = text.size();
    size_t offset = 0;
    while (offset < total_size) {
      size_t cur_size = std::min(BLOCK_SIZE, total_size - offset);
      std::string cur_str = text.substr(offset, cur_size);
      offset += cur_size;

      SipHash sip_hasher;
      XXHash xx_hasher;
      uint64_t sip_hash = sip_hasher(cur_str);
      uint64_t xx_hash = xx_hasher(cur_str);
      BlockKey key{sip_hash, xx_hash};
      // uint64_t cur_hash = sip_hasher(cur_str);
      if (data_block_.has(key)) {
        file_ptr->addBlock(data_block_[key]);
      } else {
        Huffman huffman;
        std::string bit_string = huffman.encode(cur_str);
        auto block = std::make_shared< DataBlock >();
        block->content_hash = key;
        block->original_size = cur_str.size();
        block->total_bits_count = bit_string.size();
        block->compressed_data = huffman.compress(bit_string);
        block->out_dictionary = std::make_shared< BSTree< char, std::string, Comparator< char > > >(huffman.getCodes());

        data_block_.add(key, block);
        file_ptr->addBlock(block);
      }
    }

    return true;
  }

  bool FileSystem::append(const std::string& name_file, const std::string& text)
  {
    if (!curr_dir_->children_.has(name_file)) {
      throw std::runtime_error("append: " + name_file + ": No such file");
    } else if (curr_dir_->children_[name_file]->isDirectory()) {
      throw std::runtime_error("append: " + name_file + ": is a directory");
    } else if (text == "") {
      throw std::runtime_error("append: " + name_file + ": Without next");
    }

    auto file_ptr = std::static_pointer_cast< File >(curr_dir_->children_[name_file]);

    const size_t BLOCK_SIZE = 4096;
    size_t total_size = text.size();
    size_t offset = 0;
    while (offset < total_size) {
      size_t cur_size = std::min(BLOCK_SIZE, total_size - offset);
      std::string cur_str = text.substr(offset, cur_size);
      offset += cur_size;

      SipHash sip_hasher;
      XXHash xx_hasher;
      uint64_t sip_hash = sip_hasher(cur_str);
      uint64_t xx_hash = xx_hasher(cur_str);
      BlockKey key{sip_hash, xx_hash};
      // uint64_t cur_hash = sip_hasher(cur_str);
      if (data_block_.has(key)) {
        file_ptr->addBlock(data_block_[key]);
      } else {
        Huffman huffman;
        std::string bit_string = huffman.encode(cur_str);
        auto block = std::make_shared< DataBlock >();
        block->content_hash = key;
        block->original_size = cur_str.size();
        block->total_bits_count = bit_string.size();
        block->compressed_data = huffman.compress(bit_string);
        block->out_dictionary = std::make_shared< BSTree< char, std::string, Comparator< char > > >(huffman.getCodes());

        data_block_.add(key, block);
        file_ptr->addBlock(block);
      }
    }
    return true;
  }

  bool FileSystem::cd(const std::string& path)
  {
    if (path.empty()) {
      return true;
    }
    Queue< std::string > dirs = detail::resolvePath(path);
    std::string current_path_str_ = pwd();
    if (dirs.top() == "~") {
      curr_dir_ = root_;
      while (!dirs.empty()) {
        dirs.drop();
      }
      return true;
    }
    while (!dirs.empty()) {

      std::string next_dir = dirs.top();
      if (next_dir == "..") {
        if (current_path_str_ == "~") {
          dirs.drop();
          continue;
        }

        // FindResult find_curr_dir = navigateTo(current_path_str_);
        curr_dir_ = curr_dir_->getParent();

      } else if (curr_dir_->children_.has(next_dir) && curr_dir_->children_.at(next_dir)->isDirectory()) {
        curr_dir_ = std::static_pointer_cast< Directory >(curr_dir_->children_[next_dir]);
      } else {
        throw std::runtime_error("cd: " + next_dir + ": No such directory");
      }
      dirs.drop();
    }
    return true;
  }

  bool FileSystem::isDescendant(const std::shared_ptr< Directory >& root,
                                const std::shared_ptr< FSNode >& candidate) const
  {
    if (!root) {
      return false;
    }

    auto current = candidate;

    while (current) {
      if (current == root) {
        return true;
      }
      current = current->getParent();
    }
    return false;
  }

  bool FileSystem::mv(const std::string& from, const std::string& to)
  {
    if (from.empty() || to.empty()) {
      throw std::runtime_error("mv: empty path");
    }

    if (from == "~" || to == "~") {
      throw std::runtime_error("mv: cannot move root directory");
    }

    if (from == "." || from == ".." || to == "." || to == "..") {
      throw std::runtime_error("mv: invalid path");
    }
    std::shared_ptr< FSNode > src_from = navigateTo(from);
    std::shared_ptr< FSNode > src_to = navigateTo(to);

    if (!src_from || !src_from->getParent()) {
      throw std::runtime_error("mv: : No such directory");
    }
    if (src_to && !src_to->isDirectory()) {
      throw std::runtime_error("mv: destination already exists");
    }

    if (src_from->isDirectory() && isDescendant(std::static_pointer_cast< Directory >(src_from), src_to)) {
      throw std::runtime_error("mv: cannot move directory into itself");
    }
    if (src_from == src_to) {
      throw std::runtime_error("mv: Source and destination are the same");
    }

    std::shared_ptr< Directory > target_dir = nullptr;
    std::string target_name = "";

    if (src_to && src_to->isDirectory()) {
      target_dir = std::static_pointer_cast< Directory >(src_to);
      target_name = src_from->getName();
      if (target_dir->children_.has(target_name)) {
        throw std::runtime_error("mv: destination already exists");
      }
    } else {
      size_t last_slash = to.rfind('/');
      if (last_slash == std::string::npos) {
        target_dir = curr_dir_;
        target_name = to;

      } else {
        std::string parent_path = to.substr(0, last_slash);
        if (parent_path.empty()) {
          parent_path = '/';
        }

        std::shared_ptr< FSNode > parent_node = navigateTo(parent_path);
        if (!parent_node || !parent_node->isDirectory()) {
          throw std::runtime_error("cp: cannot create regular file '" + to + "': No such file or directory");
        }
        target_dir = std::static_pointer_cast< Directory >(parent_node);
        target_name = to.substr(last_slash + 1);
      }
      if (src_to && !src_to->isDirectory()) {
        target_dir->removeChild(src_to->getName());
      } else if (target_dir->children_.has(target_name)) {
        throw std::runtime_error("mv: destination already exists");
      }
    }
    src_from->getParent()->removeChild(src_from->getName());
    target_dir->addChild(target_name, src_from);

    return true;
  }

  std::shared_ptr< FSNode > FileSystem::cloneFile(const std::shared_ptr< File >& old_file)
  {
    auto new_file = std::make_shared< File >(old_file->getMeta());
    for (auto it = old_file->getBlocks().begin(); it != old_file->getBlocks().end(); ++it) {
      new_file->addBlock(*it);
    }
    return new_file;
  }
  std::shared_ptr< FSNode > FileSystem::cloneDirectory(const std::shared_ptr< Directory >& old_dir)
  {
    auto new_dir = std::make_shared< Directory >(old_dir->getMeta());

    for (auto it = old_dir->children_.begin(); it != old_dir->children_.end(); ++it) {
      std::string child_name = it->key_;
      std::shared_ptr< FSNode > child_node = it->val_;

      if (child_node->isDirectory()) {
        auto sub_dir = std::static_pointer_cast< Directory >(child_node);
        new_dir->addChild(child_name, cloneDirectory(sub_dir));

      } else {
        auto sub_file = std::static_pointer_cast< File >(child_node);
        new_dir->addChild(child_name, cloneFile(sub_file));
      }
    }
    return new_dir;
  }

  bool FileSystem::cp(const std::string& from, const std::string& to)
  {
    if (from.empty() || to.empty()) {
      throw std::runtime_error("cp: empty path");
    }
    if (from == "~" || to == "~") {
      throw std::runtime_error("cp: cannot copy root directory");
    }
    if (from == "." || from == ".." || to == "." || to == "..") {
      throw std::runtime_error("cp: invalid path");
    }

    std::shared_ptr< FSNode > src_from = navigateTo(from);
    std::shared_ptr< FSNode > src_to = navigateTo(to);

    if (!src_from || !src_from->getParent()) {
      throw std::runtime_error("cp: : No such directory");
    }

    if (src_from == src_to) {
      throw std::runtime_error("cp: : identical files");
    }
    if (src_from->isDirectory() && src_to && !src_to->isDirectory()) {
      throw std::runtime_error("cp: cannot overwrite non-directory with directory");
    }
    std::shared_ptr< FSNode > clone_node = nullptr;
    if (src_from->isDirectory()) {
      clone_node = cloneDirectory(std::static_pointer_cast< Directory >(src_from));
    } else {
      clone_node = cloneFile(std::static_pointer_cast< File >(src_from));
    }
    std::shared_ptr< Directory > target_dir = nullptr;
    std::string target_name = "";

    if (src_to && src_to->isDirectory()) {
      target_dir = std::static_pointer_cast< Directory >(src_to);
      target_name = src_from->getName();

    } else {
      size_t last_slash = to.rfind('/');
      if (last_slash == std::string::npos) {
        target_dir = curr_dir_;
        target_name = to;
      } else {
        std::string parent_path = to.substr(0, last_slash);
        if (parent_path.empty()) {
          parent_path = '/';
        }

        std::shared_ptr< FSNode > parent_node = navigateTo(parent_path);
        if (!parent_node || !parent_node->isDirectory()) {
          throw std::runtime_error("cp: cannot create regular file '" + to + "': No such file or directory");
        }
        target_dir = std::static_pointer_cast< Directory >(parent_node);
        target_name = to.substr(last_slash + 1);
      }
      if (src_to && !src_to->isDirectory()) {
        target_dir->removeChild(src_to->getName());
      }
    }
    target_dir->addChild(target_name, clone_node);
    return true;
  }

  std::string FileSystem::cat(const std::string& name) const
  {
    std::shared_ptr< FSNode > src = navigateTo(name);
    if (!src) {
      throw std::runtime_error("cat: " + name + ": No such file or directory");
    } else if (src->isDirectory()) {
      throw std::runtime_error("cat: " + name + ": Is a directory");
    }
    std::shared_ptr< File > file = std::static_pointer_cast< File >(src);
    std::string output_str = "";
    const auto& blocks = file->getBlocks();
    Huffman huffman;
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
      if (!(*it)) {
        throw std::runtime_error("cat: corrupted block");
      }

      if (!(*it)->out_dictionary) {
        throw std::runtime_error("cat: corrupted dictionary");
      }

      size_t max_bits = (*it)->compressed_data.getSize() * 8;
      if ((*it)->total_bits_count > max_bits) {
        throw std::runtime_error("cat: corrupted block");
      }
      huffman.buildTreeFromDictionary(*(*it)->out_dictionary);
      std::string bit_string = huffman.decompress((*it)->compressed_data, (*it)->total_bits_count);
      output_str += huffman.decode(bit_string);
    }
    return output_str;
  }

  std::string FileSystem::pwd() const
  {
    if (curr_dir_ == root_) {
      return "~";
    }
    topit::Vector< std::string > path_parts;
    std::shared_ptr< Directory > current = curr_dir_;
    while (current && current != root_) {
      path_parts.pushBack(current->getName());
      current = current->getParent();
    }
    std::reverse(path_parts.begin(), path_parts.end());
    std::string res = "~";
    for (auto it = path_parts.begin(); it != path_parts.end(); ++it) {
      res += '/' + *it;
    }
    return res;
  }

  topit::Vector< std::string > FileSystem::ls(const std::string& path) const
  {
    topit::Vector< std::string > result;
    std::shared_ptr< Directory > target_dir = nullptr;

    if (path.empty()) {
      target_dir = curr_dir_;
    } else {
      std::shared_ptr< FSNode > search_node = navigateTo(path);
      if (!search_node) {
        return result;
      }
      if (search_node->isDirectory()) {
        target_dir = std::static_pointer_cast< Directory >(search_node);
      } else {
        result.pushBack(search_node->getName());
        return result;
      }
    }
    if (target_dir) {
      for (auto it = target_dir->children_.begin(); it != target_dir->children_.end(); ++it) {
        result.pushBack(it->key_);
      }
    }
    return result;
  }

  void FileSystem::treeImpl(std::shared_ptr< Directory > dir,
                            const std::string& prefix,
                            std::string& tree_str,
                            size_t& count_dir,
                            size_t& count_files) const
  {
    const std::string TRAIT = "──";
    const std::string COMPOUND = "├";
    const std::string END_COMPOUND = "└";

    std::vector< std::pair< std::string, std::shared_ptr< FSNode > > > sorted_children;
    for (auto it = dir->children_.begin(); it != dir->children_.end(); ++it) {
      sorted_children.push_back({it->key_, it->val_});
    }

    std::sort(
        sorted_children.begin(), sorted_children.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

    for (auto it = dir->children_.begin(); it != dir->children_.end(); ++it) {
      for (size_t i = 0; i < sorted_children.size(); ++i) {
        const auto& child = sorted_children[i];
        bool isLast = (i == sorted_children.size() - 1);

        std::string childPrefix = prefix;
        if (isLast) {
          childPrefix += "    ";
        } else {
          childPrefix += "│   ";
        }
        tree_str += prefix;
        tree_str += (isLast ? END_COMPOUND : COMPOUND);
        tree_str += TRAIT;
        tree_str += " ";
        tree_str += it->key_;
        tree_str += '\n';

        if (child.second->isDirectory()) {
          ++count_dir;
          auto child_dir = std::static_pointer_cast< Directory >(child.second);
          treeImpl(child_dir, childPrefix, tree_str, count_dir, count_files);
        } else {
          count_files += 1;
        }
      }
    }
  }
  std::tuple< std::string, size_t, size_t > FileSystem::tree(const std::string& path) const
  {
    size_t count_dir = 0;
    size_t count_files = 0;
    std::string res_str = "";
    res_str += ".\n";

    std::shared_ptr< Directory > target_dir = nullptr;
    if (path.empty()) {
      target_dir = curr_dir_;

    } else {
      std::shared_ptr< FSNode > found_target = navigateTo(path);
      if (!found_target) {
        throw std::runtime_error("tree: " + path + ": No such file or directory");
      }
      if (found_target->isDirectory()) {
        throw std::runtime_error("tree: " + path + " [error opening dir]");
      }
      target_dir = std::static_pointer_cast< Directory >(found_target);
    }

    treeImpl(target_dir, "", res_str, count_dir, count_files);

    return {res_str, count_dir, count_files};
  }

  topit::Vector< std::string > FileSystem::search(const std::string& name) const
  {
    if (name.empty()) {
      return {};
    }
    return {};
  }

  // Работа с постоянным хранилищем сессий и внешними файлами
  bool FileSystem::save(const std::string& path)
  {
    std::shared_ptr< FSNode > found_node = navigateTo(path);
    if (!found_node) {
      throw std::runtime_error("save: " + path + " [error opening dir]");
    }

    const std::string& save_name = found_node->getName();
    if (session_storage_.find(save_name) != session_storage_.end()) {
      throw std::runtime_error("save: " + path + " is already exists");
    }

    if (found_node->isDirectory()) {
      session_storage_.add(save_name, cloneDirectory(std::static_pointer_cast< Directory >(found_node)));
    } else {
      session_storage_.add(save_name, cloneFile(std::static_pointer_cast< File >(found_node)));
    }

    return true;
  }
  bool FileSystem::load(const std::string& path)
  {
    std::shared_ptr< FSNode > found_node = navigateTo(path);
    if (!found_node) {
      throw std::runtime_error("load: " + path + " [error opening dir]");
    }

    const std::string& save_name = found_node->getName();
    auto save_node = session_storage_.find(save_name);
    if (save_node == session_storage_.end()) {
      throw std::runtime_error("load: " + path + " is already exists");
    }

    std::shared_ptr< FSNode > new_node = save_node->val_;
    if (new_node->isDirectory()) {
      curr_dir_->addChild(save_name, cloneDirectory(std::static_pointer_cast< Directory >(new_node)));
    } else {
      curr_dir_->addChild(save_name, cloneFile(std::static_pointer_cast< File >(new_node)));
    }

    return true;
  }

  std::vector< FileSystem::StateInfo > FileSystem::states(const std::string& path) const
  {
    std::vector< StateInfo > result;

    DIR* dir = opendir(path.c_str());
    if (!dir) {
      std::cout << "here" << '\n';
      return result;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
      std::string filename = entry->d_name;

      if (filename == "." || filename == "..") {
        continue;
      }

      if (filename.size() < 6) {
        continue;
      }

      std::string extension = filename.substr(filename.size() - 6);
      if (extension != ".state") {
        continue;
      }

      std::string full_path = path;
      if (!full_path.empty() && full_path.back() != '/') {
        full_path += '/';
      }
      full_path += filename;

      struct stat file_stat;
      if (stat(full_path.c_str(), &file_stat) == 0) {
        if (S_ISREG(file_stat.st_mode)) {
          StateInfo info;
          info.name = filename;
          info.size_kb = file_stat.st_size / 1024;

          struct tm* tm_info = localtime(&file_stat.st_mtime);
          char time_buf[20];
          strftime(time_buf, sizeof(time_buf), "%Y-%m-%d", tm_info);
          info.date = time_buf;

          result.push_back(info);
        }
      }
    }
    closedir(dir);
    return result;
  }

  bool FileSystem::import_file(const std::string& real_path, const std::string& virtual_name)
  {
    std::ifstream input(real_path, std::ios::binary);

    if (!input) {
      throw std::runtime_error("import: " + real_path + " [error opening file]");
    }
    std::string new_file_name = "";
    if (virtual_name.empty()) {
      size_t pos = real_path.find_last_of("/\\");
      if (pos != std::string::npos) {
        new_file_name = real_path.substr(pos + 1);
      } else {
        new_file_name = real_path;
      }
    } else {
      new_file_name = virtual_name;
    }
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_file = std::make_shared< File >(new_data);
    std::ostringstream buffer;
    buffer << input.rdbuf();

    append(new_file_name, buffer.str());

    curr_dir_->addChild(new_file_name, new_file);
    return true;
  }
  bool FileSystem::export_file(const std::string& virtual_name, const std::string& real_path)
  {

    if (virtual_name.empty() || !navigateTo(virtual_name)) {
      throw std::runtime_error("export: " + virtual_name + " [error opening file]");
    }

    std::ofstream output_file(real_path);
    if (!output_file) {
      throw std::runtime_error("export: " + real_path + " [error creating file on disk]");
    }
    output_file << cat(virtual_name);
    output_file.close();
    return true;
  }

  void FileSystem::traverse_dir(std::ostream& out, std::shared_ptr< Directory >* dir)
  {
    for (auto it = (*dir)->children_.begin(); it != (*dir)->children_.end(); ++it) {
      std::string name = it->key_;
      std::shared_ptr< FSNode > node = it->val_;

      if (node->isDirectory()) {
        out << "mkdir " << name << '\n';
        out << "cd " << name << '\n';
        auto sub_dir = std::static_pointer_cast< Directory >(node);
        auto old_curr = curr_dir_;
        curr_dir_ = sub_dir;
        traverse_dir(out, &sub_dir);
        curr_dir_ = old_curr;
        out << "cd .." << '\n';
      } else {
        out << "touch " << name << '\n';

        std::string content = this->cat(name);
        out << "write " << name << " " << content.size() << "\n";
        out << content;
        out << "\nend_write\n";
      }
    }
  }

  bool FileSystem::save_state(const std::string& state_name, bool rewrite)
  {
    const std::string dir_path = "zubarev.arsenii/F0/states"; // или "/zubarev..."
    system(("mkdir -p " + dir_path).c_str());
    if (root_->children_.empty()) {
      throw std::runtime_error("save-state: file system is empty, nothing to save.");
    }
    const std::string state_path = "zubarev.arsenii/F0/states/" + state_name;
    size_t dot_pos = state_name.rfind('.');
    if (dot_pos == std::string::npos || state_name.substr(dot_pos) != ".state") {
      throw std::runtime_error("save-state: " + state_name + " invalid extension");
    }

    std::ifstream input(state_path, std::ios::binary);
    if (input && !rewrite) {
      throw std::runtime_error("save-state: " + state_name + " already exists. Use force=true to overwrite.");
    }
    input.close();

    std::ofstream output_file(state_path);
    if (!output_file) {
      throw std::runtime_error("save-state: " + state_path + " [error creating file on disk]");
    }
    auto old_curr_dir = curr_dir_;
    curr_dir_ = root_;
    traverse_dir(output_file, &root_);
    curr_dir_ = old_curr_dir;
    output_file.close();
    return true;
  }

  bool FileSystem::start_state(const std::string& state_name)
  {
    const std::string state_path = "zubarev.arsenii/F0/states/" + state_name;

    std::ifstream input(state_path, std::ios::binary);

    if (!input) {
      throw std::runtime_error("start-state: " + state_path + " [error opening file]");
    }
    FileMetaData root_data;
    root_data.date = detail::getCurrentDateTime();
    root_data.owner = detail::getCurrentUser();
    root_ = std::make_shared< Directory >(root_data);
    curr_dir_ = root_;
    std::string line;
    while (std::getline(input, line)) {
      if (line.empty()) {
        continue;
      }
      std::istringstream iss(line);
      std::string command;
      iss >> command;
      if (command == "mkdir") {
        std::string name;
        iss >> name;
        this->mkdir(name);
      } else if (command == "touch") {
        std::string name;
        iss >> name;
        this->touch(name);
      } else if (command == "cd") {
        std::string path;
        iss >> path;
        this->cd(path);
      } else if (command == "write") {
        std::string name;
        size_t content_size;
        iss >> name >> content_size;

        std::string content(content_size, '\0');
        input.read(&content[0], content_size);

        char dummy;
        input.get(dummy);

        std::string end_marker;
        std::getline(input, end_marker);
        if (end_marker != "end_write") {
          throw std::runtime_error("start-state: corrupted state file");
        }

        this->write(name, content);
      }
    }
    input.close();
    this->cd("~");
    return true;
  }
  void FileSystem::packUint32(uint32_t value, std::string& out)
  {
    out.push_back((value >> 24) & 0xFF);
    out.push_back((value >> 16) & 0xFF);
    out.push_back((value >> 8) & 0xFF);
    out.push_back(value & 0xFF);
  }
  void
  FileSystem::archiveImpl(const std::string& base_path, std::shared_ptr< Directory > dir, std::string& archive_data)
  {
    for (auto it = dir->children_.begin(); it != dir->children_.end(); ++it) {
      std::string child_name = it->key_;
      std::string full_virt_path_ = base_path.empty() ? child_name : base_path + "/" + child_name;
      auto node = it->val_;
      if (node->isDirectory()) {
        auto sub_dir = std::static_pointer_cast< Directory >(node);
        archiveImpl(full_virt_path_, sub_dir, archive_data);
      } else {
        auto file = std::static_pointer_cast< File >(node);
        std::string content = "";

        const auto& blocks = file->getBlocks();
        Huffman huffman;
        for (auto it = blocks.begin(); it != blocks.end(); ++it) {
          huffman.buildTreeFromDictionary(*(*it)->out_dictionary);
          std::string bit_string = huffman.decompress((*it)->compressed_data, (*it)->total_bits_count);
          content += huffman.decode(bit_string);
        }

        packUint32(full_virt_path_.size(), archive_data);
        archive_data += full_virt_path_;

        packUint32(content.size(), archive_data);
        archive_data += content;
      }
    }
  }

  bool FileSystem::archive(const std::string& archive_name, const std::string& dir_path)
  {
    if (archive_name.empty() || dir_path.empty()) {
      throw std::runtime_error("archive: empty path");
    }
    if (curr_dir_->children_.has(archive_name)) {
      throw std::runtime_error("archive: file '" + archive_name + "' already exists");
    }

    auto target_node = navigateTo(dir_path);
    if (!target_node->isDirectory()) {
      throw std::runtime_error("archive: target is not a directory or not found");
    }
    auto target_dir = std::static_pointer_cast< Directory >(target_node);
    std::string archived_data = "";

    archiveImpl("", target_dir, archived_data);
    touch(archive_name);
    write(archive_name, archived_data);
    return true;
  }

}
