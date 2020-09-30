#ifndef _CLASSES_HPP
#define _CLASSES_HPP

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#define FLOPPY_SIZE 1474560

class File {
private:
  std::fstream *stream;
protected:
  std::filesystem::path path;
public:
  File(std::filesystem::path path, bool image, bool init_stream=true);
  virtual std::fstream *get_stream();
  std::string get_path_str();
  virtual bool is_directory();
  virtual int get_size();
};

class Directory : public File {
private:
  std::vector<File *> files;
  int total_size;
public:
  Directory(std::filesystem::path path);
  std::fstream *get_stream();
  bool is_directory();
  int get_size();
  File *operator[](int index);
  int get_file_count();
};

class Floppy {
private:
  std::vector<File *> files;
  int free_space;
public:
  Floppy();
  bool add_file(File *file);
  bool save(std::string filename);
};

#endif
