#include "classes.hpp"

using namespace std;

File::File(string path) {
  stream = new fstream(path, ios::in | ios::out | ios::binary);
}

fstream *File::get_stream() {
  return stream;
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
