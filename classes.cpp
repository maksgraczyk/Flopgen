#include "classes.hpp"
#include "image.hpp"
#include <filesystem>
#include <iostream>

using namespace std;

File::File(string path, bool image) {
  if (image) {
    stream = new fstream(path, ios::in | ios::out | ios::binary | ios::trunc);
  } else {
    stream = new fstream(path, ios::in | ios::binary);
  }
  
  stream = new fstream(path, ios::in | ios::out | ios::binary);
  filename = filesystem::path(path).filename();
}

fstream *File::get_stream() {
  return stream;
}

string File::get_filename() {
  return filename;
}

int File::get_size() {
  streampos original = stream->tellg();
  stream->seekg(0, ios::beg);
  streampos begin = stream->tellg();
  stream->seekg(0, ios::end);
  streampos end = stream->tellg();
  stream->seekg(original);
  return end - begin;
}

Floppy::Floppy() {
  free_space = FLOPPY_SIZE;
}

bool Floppy::add_file(File file) {
  int size = file.get_size();
  
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
