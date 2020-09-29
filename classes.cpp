#include "classes.hpp"
#include "fatfs/ff.h"
#include "filediskio.h"
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
  file_disk_setup(filename.c_str(), FLOPPY_SIZE);

  FATFS fs;
  
  MKFS_PARM options;
  options.fmt = FM_FAT | FM_SFD;
  options.au_size = 0;
  options.align = 0;
  options.n_fat = 2;
  options.n_root = 0;

  BYTE buf[FF_MAX_SS];
  
  FRESULT res = f_mkfs("", &options, buf, sizeof(buf));

  if (res != 0) {
    goto error;
  }

  res = f_mount(&fs, "", 0);

  if (res != 0) {
    goto error;
  }

  for (unsigned long i = 0; i < files.size(); i++) {
    File file_object = files[i];
    fstream *stream = file_object.get_stream();
    UINT size = file_object.get_size();

    FIL file;

    res = f_open(&file, file_object.get_filename().c_str(),
                 FA_WRITE | FA_CREATE_ALWAYS);

    if (res != 0) {
      goto error;
    }

    char *buffer = new char[size];

    if (buffer == NULL) {
      goto error;
    }

    stream->seekg(0, ios::beg);
    stream->read(buffer, size);

    if (stream->rdstate() && !stream->eof()) {
      delete [] buffer;
      goto error;
    }

    UINT bytes_written = 0;
    res = f_write(&file, buffer, size, &bytes_written);

    delete [] buffer;
    
    if (res != 0 || bytes_written != size) {
      goto error;
    }

    res = f_close(&file);

    if (res != 0) {
      goto error;
    }
  }
  
  file_disk_free();
  return true;

 error:
  file_disk_free();
  return false;
}
