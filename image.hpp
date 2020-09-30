#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <string>
#include "classes.hpp"
#include "fatfs/ff.h"

class Image {
private:
  bool open;
  FATFS *fs;
public:
  Image(std::string filename, int bytes);
  bool is_open();
  bool operator<<(File file_object);
  void close();
};

#endif
