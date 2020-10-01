#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <string>
#include "classes.hpp"
#include "fatfs/ff.h"

class Image {
private:
  bool open;
  FATFS *fs;
  bool add_file(File *file_object);
  bool add_directory(Directory *dir);
public:
  Image(std::string filename, FloppySize size, int code_page);
  bool is_open();
  bool operator<<(File *file_object);
  void close();
};

#endif
