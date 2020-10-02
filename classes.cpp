#include "classes.hpp"
#include "image.hpp"
#include <iostream>

using namespace std;

/*** File class begins here ***/

File::File(filesystem::path path, bool image, bool init_stream) {
  if (init_stream) {
    if (image) {
      stream = new fstream(path,
                           ios::in | ios::out | ios::binary | ios::trunc);
    } else {
      stream = new fstream(path, ios::in | ios::binary);
    }
  } else {
    stream = NULL;
  }
  
  this->path = path;
}

File::~File() {
  if (stream != NULL) {
    stream->close();
    delete stream;
  }
}

File *File::get_instance(string path) {
  filesystem::path fs_path(path);
  File *instance;

  if (!filesystem::exists(fs_path)) {
    cerr << fs_path << " does not exist, ignoring." << endl;
    return NULL;
  }
  
  if (filesystem::is_directory(fs_path)) {
    instance = new Directory(fs_path);
  } else if (filesystem::is_regular_file(fs_path)) {
    instance = new File(fs_path, false, true);
  } else {
    cerr << fs_path.generic_string();
    cerr << " is neither a directory nor a regular file, ignoring." << endl;
    return NULL;
  }

  if (instance == NULL) {
    cerr << "Could not allocate memory for " << path << ", ignoring." << endl;
  }

  return instance;
}

fstream *File::get_stream() {
  return stream;
}

bool File::is_directory() {
  return false;
}

string File::get_path_str() {
  return path.generic_string();
}

int File::get_size() {
  return filesystem::file_size(path);
}

/*** File class ends here ***/

/*** Directory class begins here ***/

Directory::Directory(filesystem::path path) : File(path, false, false) {
  filesystem::directory_iterator dir(this->path);
  total_size = 0;

  for (const filesystem::directory_entry &entry : dir) {
    if (entry.is_regular_file()) {
      File *file = new File(entry.path(), false);
      files.push_back(file);
      total_size += file->get_size();
    } else if (entry.is_directory()) {
      File *directory = new Directory(entry.path());
      files.push_back(directory);
      total_size += directory->get_size();
    } else {
      cerr << entry.path().generic_string()
           << " is neither a directory nor a regular file, ignoring." << endl;
    }
  }
}

Directory::~Directory() {
  for (File *file : files) {
    delete file;
  }
}

fstream *Directory::get_stream() {
  return NULL;
}

bool Directory::is_directory() {
  return true;
}

int Directory::get_size() {
  return total_size;
}

File *Directory::operator[](int index) {
  return files[index];
}

int Directory::get_file_count() {
  return files.size();
}

/*** Directory class ends here ***/

/*** Floppy class begins here ***/

Floppy::Floppy(FloppySize size, int code_page) {
  this->size = size;
  this->code_page = code_page;

  switch (size) {
  case _360K:
  case _720K:
  case _2880K:
    cluster_size = 1024;
    break;

  default:
    cluster_size = 512;
    break;
  }
  
  free_space = (size * BYTES_IN_KB) / cluster_size - 2;
}

bool Floppy::fit(File *file, int *cl_size) {
  int file_size = file->get_size();
  int c_size = file_size / cluster_size
    + ((file_size % cluster_size) > 0);

  if (c_size > free_space) {
    return false;
  }

  if (cl_size != NULL) {
    *cl_size = c_size;
  }
  
  return true;
}

bool Floppy::fit_capacity(File *file) {
  int file_size = file->get_size();
  int c_size = file_size / cluster_size
    + ((file_size % cluster_size) > 0);

  if (c_size > (size * BYTES_IN_KB) / cluster_size - 2) {
    return false;
  }
  
  return true;
}

bool Floppy::add_file(File *file) {
  int cl_size;
  
  if (!fit(file, &cl_size)) {
    return false;
  }

  files.push_back(file);
  free_space -= cl_size;
  
  return true;
}

int Floppy::get_size() {
  return size * BYTES_IN_KB;
}

bool Floppy::save(string filename) {
  Image image(filename, size, code_page);

  if (!image.is_open()) {
    return false;
  }

  for (unsigned long i = 0; i < files.size(); i++) {
    bool success = image << files[i];

    if (!success) {
      return false;
    }
  }
  
  return true;
}

/*** Floppy class ends here ***/
