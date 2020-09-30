#ifndef _CLASSES_HPP
#define _CLASSES_HPP

#include <string>
#include <vector>
#include <fstream>

#define FLOPPY_SIZE 1474560

class File {
private:
  std::fstream *stream;
  std::string filename;
public:
  File(std::string path, bool image);
  std::fstream *get_stream();
  std::string get_filename();
  int get_size();
};

class Floppy {
private:
  std::vector<File> files;
  int free_space;
public:
  Floppy();
  bool add_file(File file);
  bool save(std::string filename);
};

#endif
