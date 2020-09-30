#include "image.hpp"
#include "filediskio.h"

using namespace std;

Image::Image(string filename, int bytes) {
  file_disk_setup(filename.c_str(), bytes);

  MKFS_PARM options;
  options.fmt = FM_FAT | FM_SFD;
  options.au_size = 0;
  options.align = 0;
  options.n_fat = 2;
  options.n_root = 224;
  options.mdt = 0xF0;
  options.sec_per_track = 18;
  options.n_heads = 2;
  options.d_num = 0x00;

  BYTE buf[FF_MAX_SS];

  FRESULT res = f_mkfs("", &options, buf, sizeof(buf));

  if (res != 0) {
    goto error;
  }

  fs = new FATFS;

  if (fs == NULL) {
    goto error;
  }

  res = f_mount(fs, "", 1);

  if (res != 0) {
    delete fs;
    goto error;
  }

  open = true;
  return;

 error:
  open = false;
  file_disk_free();
}

bool Image::is_open() {
  return open;
}

bool Image::add_file(File *file_object) {
  fstream *stream = file_object->get_stream();
  UINT size = file_object->get_size();

  FIL file;

  FRESULT res = f_open(&file, file_object->get_path_str().c_str(),
                       FA_WRITE | FA_CREATE_ALWAYS);

  if (res != 0) {
    return false;
  }

  char *buffer = new char[size];

  if (buffer == NULL) {
    return false;
  }

  stream->seekg(0, ios::beg);
  stream->read(buffer, size);

  if (stream->rdstate() && !stream->eof()) {
    delete [] buffer;
    return false;
  }

  UINT bytes_written = 0;
  res = f_write(&file, buffer, size, &bytes_written);

  delete [] buffer;
    
  if (res != 0 || bytes_written != size) {
    return false;
  }

  res = f_close(&file);

  if (res != 0) {
    return false;
  }

  return true;
}

bool Image::add_directory(Directory *dir) {
  FRESULT res = f_mkdir(dir->get_path_str().c_str());

  if (res != 0 && res != FR_EXIST) {
    return false;
  }

  for (int i = 0; i < dir->get_file_count(); i++) {
    if (!add_file((*dir)[i])) {
      return false;
    }
  }

  return true;
}

bool Image::operator<<(File *file_object) {
  if (file_object->is_directory()) {
    return add_directory((Directory *) file_object);
  } else {
    return add_file(file_object);
  }
}

void Image::close() {
  f_mount(NULL, "", 0);
  file_disk_free();
}
