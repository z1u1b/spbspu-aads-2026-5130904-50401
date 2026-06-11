#include "file_system.hpp"
#include <stdexcept>
#include "utils.hpp"
#include "../common/my_queue/queue.hpp"

namespace zubarev
{
  FileSystem::FileSystem():
    current_path_str_("/")
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
    if (curr_dir_->children_.has(name_dir)) {
      throw std::runtime_error("rmdir: " + name_dir + ": No such file or directory");
    }
    curr_dir_->children_.drop(name_dir);
    return true;
  }
  bool FileSystem::rm(const std::string& name)
  {
    if (curr_dir_->children_.has(name)) {
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
        block->compressed_data = huffman.compress(huffman.encode(text));

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
        block->compressed_data = huffman.compress(huffman.encode(text));

        block->out_dictionary = std::make_shared< BSTree< char, std::string, Comparator< char > > >(huffman.getCodes());

        data_block_.add(cur_hash, block);
        file_ptr->addBlock(block);
      }
    }
    return false;
  }

  bool FileSystem::cd(const std::string& path)
  {
    Queue< std::string > dirs = detail::resolvePath(path);
    if (dirs.top() == "~") {
      curr_dir_ = root_;
      current_path_str_ = "~";
    }
    while (!dirs.empty()) {
      if (curr_dir_->children_.has(dirs.top()) && curr_dir_->children_[dirs.top()]->isDirectory()) {
        curr_dir_ = std::static_pointer_cast< Directory >(curr_dir_->children_[dirs.top()]);
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

    if (!src_from.target_ || !src_to.target_) {
      throw std::runtime_error("mv: : No such directory");
    }
    if (src_from.parent_dir_ == src_to.parent_dir_) {
      std::shared_ptr< FSNode > tmp = src_from.target_;
      src_from.parent_dir_->children_.drop(src_from.target_name_);
      src_from.parent_dir_->children_.add(src_to.target_name_, tmp);
    } else if ()
      return false;
  }

  bool FileSystem::cp(const std::string& from, const std::string& to)
  {
    return false;
  }

  std::string FileSystem::cat(const std::string& name) const
  {

    return "";
  }

  std::string FileSystem::pwd() const
  {

    return "/";
  }

  topit::Vector< std::string > FileSystem::ls() const
  {
    return {};
  }

  std::string FileSystem::tree(const std::string& path) const
  {
    return "";
  }

  topit::Vector< std::string > FileSystem::search(const std::string& name) const
  {
    return {};
  }
}
