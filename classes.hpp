#include <string>
#include <vector>
#include <fstream>

#define FLOPPY_SIZE_KB 1440

class Floppy {
private:
  std::vector<std::fstream> files;
  int free_space;
public:
  Floppy();
  bool add_file(std::fstream file);
  int get_free_space();
  void save(std::string filename);
};

class File {
private:
  std::fstream stream;
public:
  File(std::string path);
  std::fstream get_stream();
  int get_size();
};
