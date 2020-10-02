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
  std::string get_path_str(bool with_dirs=true);
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
