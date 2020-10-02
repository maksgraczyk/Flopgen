#ifndef _CLASSES_HPP
#define _CLASSES_HPP

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#define BYTES_IN_KB 1024

class File {
private:
  std::fstream *stream;
protected:
  std::filesystem::path path;
public:
  File(std::filesystem::path path, bool image, bool init_stream=true);
  virtual ~File();
  static File *get_instance(std::string path);
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
  ~Directory();
  std::fstream *get_stream();
  bool is_directory();
  int get_size();
  File *operator[](int index);
  int get_file_count();
};

enum FloppySize {
  _360K = 360,
  _720K = 720,
  _1200K = 1200,
  _1440K = 1440,
  _2880K = 2880
};

class Floppy {
private:
  std::vector<File *> files;
  FloppySize size;
  int free_space;
  int cluster_size;
  int code_page;
public:
  Floppy(FloppySize size, int code_page);
  bool fit(File *file, int *cl_size=NULL);
  bool fit_capacity(File *file);
  bool add_file(File *file);
  int get_size();
  bool save(std::string filename);
};

#endif
