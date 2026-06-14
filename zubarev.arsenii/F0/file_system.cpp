#include "file_system.hpp"
#include <stdexcept>
#include "utils.hpp"
#include "../common/my_queue/queue.hpp"

namespace zubarev
{
  FileSystem::FindResult FileSystem::navigateTo(const std::string& path) const
  {
    FindResult result;
    Queue< std::string > dirs = detail::resolvePath(path);
    if (dirs.empty()) {
      return result;
    }
    std::shared_ptr< Directory > current = curr_dir_;
    if (path[0] == '/' || dirs.top() == "~") {
      current = root_;
      if (dirs.top() == "~") {
        dirs.drop();
      }
    }
    while (!dirs.empty()) {
      auto next_name = dirs.top();
      dirs.drop();

      auto it = current->children_.find(next_name);

      if (dirs.empty()) {
        result.parent_dir_ = current;
        result.target_name_ = next_name;
        if (it != current->children_.end()) {
          {
            result.target_ = it->val_;
          }
          return result;
        }

        if (it == current->children_.end() || !it->val_->isDirectory()) {
          return result;
        }
        current = std::static_pointer_cast< Directory >(it->val_);
      }
    }
    return result;
  }
  FileSystem::FileSystem():
    current_path_str_("~")
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
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_dir = std::make_shared< Directory >(new_data);
    curr_dir_->children_.add(name_dir, new_dir);
    return true;
  }

  bool FileSystem::rmdir(const std::string& name_dir)
  {
    if (!curr_dir_->children_.has(name_dir)) {
      throw std::runtime_error("rmdir: " + name_dir + ": No such file or directory");
    }
    curr_dir_->children_.drop(name_dir);
    return true;
  }
  bool FileSystem::rm(const std::string& name)
  {
    if (!curr_dir_->children_.has(name)) {
      throw std::runtime_error("rm: " + name + ": No such file or directory");
    } else if (curr_dir_->children_[name]->isDirectory()) {
      throw std::runtime_error("rm: " + name + ": is a directory");
    }
    curr_dir_->children_.drop(name);
    return true;
  }

  bool FileSystem::touch(const std::string& name_file)
  {
    if (curr_dir_->children_.has(name_file)) {
      throw std::runtime_error("mkdir: " + name_file + ": File exists");
    }
    FileMetaData new_data;
    new_data.date = detail::getCurrentDateTime();
    new_data.owner = detail::getCurrentUser();

    auto new_file = std::make_shared< File >(new_data);
    curr_dir_->children_.add(name_file, new_file);
    return true;
  }

  bool FileSystem::write(const std::string& name_file, const std::string& text)
  {
    if (!curr_dir_->children_.has(name_file)) {
      throw std::runtime_error("write: " + name_file + ": No such file");
    } else if (curr_dir_->children_[name_file]->isDirectory()) {
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

      SipHash hasher;
      uint64_t cur_hash = hasher(cur_str);
      if (data_block_.has(cur_hash)) {
        file_ptr->addBlock(data_block_[cur_hash]);
      } else {
        Huffman huffman;
        auto block = std::make_shared< DataBlock >();
        block->content_hash = cur_hash;
        block->original_size = cur_size;
        block->compressed_data = huffman.compress(huffman.encode(cur_str));

        block->out_dictionary = std::make_shared< BSTree< char, std::string, Comparator< char > > >(huffman.getCodes());

        data_block_.add(cur_hash, block);
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

      SipHash hasher;
      uint64_t cur_hash = hasher(cur_str);
      if (data_block_.has(cur_hash)) {
        file_ptr->addBlock(data_block_[cur_hash]);
      } else {
        Huffman huffman;
        auto block = std::make_shared< DataBlock >();
        block->content_hash = cur_hash;
        block->original_size = cur_size;
        block->compressed_data = huffman.compress(huffman.encode(cur_str));

        block->out_dictionary = std::make_shared< BSTree< char, std::string, Comparator< char > > >(huffman.getCodes());

        data_block_.add(cur_hash, block);
        file_ptr->addBlock(block);
      }
    }
    return true;
  }

  bool FileSystem::cd(const std::string& path)
  {
    Queue< std::string > dirs = detail::resolvePath(path);

    if (dirs.top() == "~") {
      curr_dir_ = root_;
      current_path_str_ = "~";
      dirs.drop();
    }
    while (!dirs.empty()) {

      std::string next_dir = dirs.top();
      if (next_dir == "..") {
        if (current_path_str_ == "~") {
          continue;
        }

        FindResult find_curr_dir = navigateTo(current_path_str_);
        curr_dir_ = find_curr_dir.parent_dir_;

        auto pos = current_path_str_.rfind('/');
        if (pos == std::string::npos) {
          current_path_str_ = "~";
        } else {
          current_path_str_ = current_path_str_.substr(0, pos);
        }

      } else if (curr_dir_->children_.has(next_dir) && curr_dir_->children_[next_dir]->isDirectory()) {
        curr_dir_ = std::static_pointer_cast< Directory >(curr_dir_->children_[next_dir]);
        current_path_str_ += "/";
        current_path_str_ += next_dir;
      } else {
        throw std::runtime_error("cd: " + dirs.top() + ": No such directory");
      }
      dirs.drop();
    }
    return true;
  }

  bool FileSystem::mv(const std::string& from, const std::string& to)
  {
    FindResult src_from = navigateTo(from);
    FindResult src_to = navigateTo(to);

    if (!src_from.target_ || !src_from.parent_dir_) {
      throw std::runtime_error("mv: : No such directory");
    }

    if (src_from.target_ == src_to.target_) {
      throw std::runtime_error("mv: : Recursion moving");
    }

    if (src_to.target_ && src_to.target_->isDirectory()) {
      auto tmp_dir = std::static_pointer_cast< Directory >(src_to.target_);

      if (tmp_dir->children_.has(src_from.target_name_)) {
        throw std::runtime_error("mv: : Is already been");
      }
      std::shared_ptr< FSNode > tmp = src_from.target_;
      src_from.parent_dir_->children_.drop(src_from.target_name_);
      tmp_dir->children_.add(src_from.target_name_, tmp);

      return true;
    }

    if (!src_to.target_) {
      if (!src_to.parent_dir_) {
        throw std::runtime_error("mv: : No such directory");
      }
      std::shared_ptr< FSNode > tmp = src_from.target_;
      src_from.parent_dir_->children_.drop(src_from.target_name_);
      src_from.parent_dir_->children_.add(src_to.target_name_, tmp);
      return true;
    }

    return false;
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
        new_dir->children_.add(child_name, cloneDirectory(sub_dir));

      } else {
        auto sub_file = std::static_pointer_cast< File >(child_node);
        new_dir->children_.add(child_name, cloneFile(sub_file));
      }
    }
    return new_dir;
  }

  bool FileSystem::cp(const std::string& from, const std::string& to)
  {

    FindResult src_from = navigateTo(from);
    FindResult src_to = navigateTo(to);

    if (!src_from.target_ || !src_from.parent_dir_) {
      throw std::runtime_error("cp: : No such directory");
    }
    if (src_from.target_ == src_to.target_) {
      throw std::runtime_error("cp: : identical files");
    }

    std::shared_ptr< FSNode > clone_node = nullptr;

    if (src_from.target_->isDirectory()) {
      clone_node = cloneDirectory(std::static_pointer_cast< Directory >(src_from.target_));
    } else {
      clone_node = cloneFile(std::static_pointer_cast< File >(src_from.target_));
    }

    if (src_to.target_ && src_to.target_->isDirectory()) {
      auto target = std::static_pointer_cast< Directory >(src_to.target_);
      if (target->children_.has(src_from.target_name_)) {
        target->children_.drop(src_from.target_name_);
      }
      target->children_.add(src_from.target_name_, clone_node);
      return true;
    } else if (src_to.target_ && !src_to.target_->isDirectory() && !src_from.target_->isDirectory()) {
      src_to.parent_dir_->children_.drop(src_to.target_name_);
      src_to.parent_dir_->children_.add(src_to.target_name_, clone_node);
    }

    if (!src_to.target_) {
      if (!src_to.parent_dir_) {
        throw std::runtime_error("cp: : No such directory");
      }
      src_to.parent_dir_->children_.add(src_to.target_name_, clone_node);
      return true;
    }
    return false;
  }

  std::string FileSystem::cat(const std::string& name) const
  {
    FindResult src = navigateTo(name);
    if (!src.target_) {
      throw std::runtime_error("cat: " + name + ": No such directory");
    } else if (src.target_->isDirectory()) {
      throw std::runtime_error("cat: " + name + ": Is a directory");
    }
    std::shared_ptr< File > file = std::static_pointer_cast< File >(src.target_);
    std::string output_str = "";
    topit::Vector< std::shared_ptr< DataBlock > > blocks = file->getBlocks();
    Huffman huffman;
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
      output_str += huffman.decode(huffman.decompress((*it)->compressed_data, (*it)->total_bits_count));
    }
    return output_str;
  }

  std::string FileSystem::pwd() const
  {
    return current_path_str_;
  }

  topit::Vector< std::string > FileSystem::ls(const std::string& path) const
  {
    topit::Vector< std::string > result;
    std::shared_ptr< Directory > target_dir = nullptr;

    if (path.empty()) {
      target_dir = curr_dir_;
    } else {
      FindResult search_node = navigateTo(path);
      if (!search_node.target_) {
        return result;
      }
      if (search_node.target_->isDirectory()) {
        target_dir = std::static_pointer_cast< Directory >(search_node.target_);
      } else {
        result.pushBack(search_node.target_name_);
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

    for (auto it = dir->children_.begin(); it != dir->children_.end(); ++it) {
      bool isLast = false;
      auto next = it;
      ++next;
      if (next == dir->children_.end()) {
        isLast = true;
      }
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
      if (it->val_->isDirectory()) {
        ++count_dir;
        auto child_dir = std::static_pointer_cast< Directory >(it->val_);
        treeImpl(child_dir, childPrefix, tree_str, count_dir, count_files);
      } else {
        count_files += 1;
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
      FindResult found_target = navigateTo(path);
      if (found_target.target_->isDirectory()) {
        throw std::runtime_error("tree: " + path + " [error opening dir]");
      }
      target_dir = std::static_pointer_cast< Directory >(found_target.target_);
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
}
