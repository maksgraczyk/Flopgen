/*
  Flopgen: a tool for automatic creation of FAT-formatted floppy disk images
  Copyright (C) 2020 Maksymilian Graczyk.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <string>
#include "classes.hpp"
#include "fatfs/ff.h"

class Image {
private:
  bool open;
  FATFS *fs;
  bool add_file(File *file_object, bool path_with_dirs=true);
  bool add_directory(Directory *dir);
public:
  Image(std::string filename, FloppySize size, int code_page);
  ~Image();
  bool is_open();
  bool operator<<(File *file_object);
};

#endif
