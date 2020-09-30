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
      cerr << entry.path()
           << " is neither a directory nor a regular file, ignoring." << endl;
    }
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

Floppy::Floppy() {
  free_space = FLOPPY_SIZE;
}

bool Floppy::add_file(File *file) {
  int size = file->get_size();
  
  if (size > free_space) {
    return false;
  }

  files.push_back(file);
  return true;
}

bool Floppy::save(string filename) {
  Image image(filename, FLOPPY_SIZE);

  if (!image.is_open()) {
    return false;
  }

  for (unsigned long i = 0; i < files.size(); i++) {
    bool success = image << files[i];

    if (!success) {
      image.close();
      return false;
    }
  }
  
  image.close();
  return true;
}

/*** Floppy class ends here ***/
