#include "filediskio.h"
#include "classes.hpp"
#include <iostream>

#define SECTOR_SIZE 512

using namespace std;

static File *file_disk = NULL;
static string *path = NULL;
static int bytes_count = 0;

void file_disk_setup(const char *filename, int bytes) {
  path = new string(filename);
  bytes_count = bytes;
}

void file_disk_free() {
  if (file_disk != NULL) {
    delete file_disk;
    file_disk = NULL;
  }

  if (path != NULL) {
    delete path;
    path = NULL;
  }
}

DSTATUS file_disk_status() {
  if (file_disk == NULL) {
    return STA_NOINIT;
  }

  return 0;
}

DSTATUS file_disk_initialize() {
  if (path == NULL) {
    return STA_NOINIT;
  }

  if (file_disk == NULL) {
    file_disk = new File(*path, true);
    fstream *stream = file_disk->get_stream();

    for (int i = 0; i < bytes_count; i++) {
      *stream << (char) 0;

      if (stream->rdstate()) {
        cout << stream->rdstate() << endl;
        return STA_NOINIT;
      }
    }

    stream->flush();
    stream->sync();
  }
  
  return file_disk_status();
}

DRESULT file_disk_read(BYTE *buff, LBA_t sector, UINT count) {
  fstream *stream = file_disk->get_stream();
  streamsize byte_size = count * SECTOR_SIZE;
  streampos byte_start = sector * SECTOR_SIZE;

  stream->seekg(byte_start, ios::beg);

  if (stream->rdstate()) {
    return RES_ERROR;
  }
  
  stream->read((char *) buff, byte_size);

  if (stream->rdstate() && !stream->eof()) {
    return RES_ERROR;
  }
  
  return RES_OK;
}

DRESULT file_disk_write(const BYTE *buff, LBA_t sector, UINT count) {
  fstream *stream = file_disk->get_stream();
  streamsize byte_size = count * SECTOR_SIZE;
  streampos byte_start = sector * SECTOR_SIZE;

  stream->seekp(byte_start, ios::beg);

  if (stream->rdstate()) {
    return RES_ERROR;
  }

  stream->write((const char *) buff, byte_size);

  if (stream->rdstate()) {
    return RES_ERROR;
  }

  stream->flush();
  stream->sync();

  return RES_OK;
}

DRESULT file_disk_ioctl(BYTE cmd, void *buff) {
  int *int_buff = (int *) buff;
  
  switch (cmd) {
  case GET_SECTOR_COUNT:
    *int_buff = bytes_count / SECTOR_SIZE;
    break;

  case GET_SECTOR_SIZE:
    *int_buff = SECTOR_SIZE;
    break;

  case GET_BLOCK_SIZE:
    *int_buff = 1;
    break;
  }

  return RES_OK;
}
