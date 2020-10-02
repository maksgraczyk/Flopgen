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

#include "classes.hpp"
#include <iostream>
#include "cli/CLI11.hpp"

using namespace std;

struct Options {
  string output_filename;
  vector<string> filenames;
  FloppySize floppy_size;
  int code_page;
};

inline bool compare_file_sizes(File *f1, File *f2) {
  return f1->get_size() > f2->get_size();
}

bool get_filenames(vector<string> &result, istream *input);
bool run(Options &options);

int main(int argc, char** argv) {
  CLI::App app
    {"Flopgen: a tool for automatic creation of IMG floppy disk images"};

  Options options;

  auto output_opt = app.add_option("-o,--output", options.output_filename,
                                   "Set base output filename without .img. All images will be of form (name)1.img, (name)2.img etc.");
  output_opt->required();

  options.code_page = 437;
  auto code_page_opt = app.add_option("-c,--codepage", options.code_page,
                                      "Set code page (default: 437)");

  int size = 1440;
  auto size_opt = app.add_option("-s,--size", size,
                                 "Set size of each image in kilobytes (default: 1440)");
  size_opt->check(CLI::IsMember({360, 720, 1200, 1440, 2880}));

  string input;
  auto input_opt = app.add_option("-i,--input", input,\
                                  "Set filename of input list of files separated by a new line (use - for stdin)");

  vector<string> filenames;
  auto filenames_opt = app.add_option("FILES/DIRS", filenames,
                                      "Specify files/directories to be added to images (directories are processed recursively)");

  CLI11_PARSE(app, argc, argv);

  switch (options.code_page) {
  case 437:
  case 720:
  case 737:
  case 771:
  case 775:
  case 850:
  case 852:
  case 855:
  case 857:
  case 860:
  case 861:
  case 862:
  case 863:
  case 864:
  case 865:
  case 866:
  case 869:
  case 932:
  case 936:
  case 949:
  case 950:
    break;

  default:
    cerr << "The code page must be one of the following:" << endl;
    cerr << "437, 720, 737, 771, 775, 850, 852, 855, 857, 860, "
      "861, 862, 863, 864, 865, 866, 869, 932, 936, 949, 950" << endl;
    return 1;
  }

  if (input == "" && filenames.size() == 0) {
    cerr << "You must either specify files/directories to be added "
      "to images or use -i/--input." << endl;
    cerr << "Run with --help for more information." << endl;
    return 106;
  }

  if (filenames.size() > 0) {
    options.filenames = filenames;
  } else if (input == "-") {
    get_filenames(options.filenames, &cin);
  } else {
    ifstream stream(input);
    bool result = get_filenames(options.filenames, &stream);
    stream.close();

    if (!result) {
      return 1;
    }
  }

  options.floppy_size = static_cast<FloppySize>(size);
  
  if (!run(options)) {
    return 1;
  }
  
  return 0;
}

bool get_filenames(vector<string> &result, istream *input) {
  while (!input->eof()) {
    string filename;
    getline(*input, filename);

    if (input->rdstate() != 0 && !input->eof()) {
      cerr << "Could not read the list of files due to an error, aborting."
           << endl;
      return false;
    }
    
    result.push_back(filename);
  }

  return true;
}

bool run(Options &options) {
  vector<File *> files;

  for (string &filename : options.filenames) {
    File *file = File::get_instance(filename);
    if (file != NULL) {
      files.push_back(file);
    }
  }

  if (files.size() == 0) {
    cerr << "There are no files/directories to be added to images!" << endl;
    return false;
  }

  sort(files.begin(), files.end(), compare_file_sizes);

  Floppy *current_floppy = new Floppy(options.floppy_size, options.code_page);

  if (current_floppy == NULL) {
    cerr << "Could not allocate memory for a floppy image, aborting." << endl;
    
    for (File *file : files) {
      delete file;
    }

    return false;
  }

  int count = 1;
  bool at_least_one_file = false;
  
  for (File *file : files) {
    if (!current_floppy->fit_capacity(file)) {
      cerr << file->get_path_str()
           << (file->is_directory() ? " (directory)" : "")
           << " takes more disk space than the image size, ignoring." << endl;
      continue;
    }

    if (!current_floppy->add_file(file)) {
      if (!current_floppy->save(options.output_filename + to_string(count++)
                                + ".img")) {
        cerr << "Could not produce a floppy image number " << count - 1
             << " due to an error, aborting." << endl;
        
        delete current_floppy;

        for (File *file : files) {
          delete file;
        }

        return false;
      }

      delete current_floppy;
      current_floppy = new Floppy(options.floppy_size, options.code_page);

      if (current_floppy == NULL) {
        cerr << "Could not allocate memory for a floppy image, aborting."
             << endl;
        
        for (File *file : files) {
          delete file;
        }

        return false;
      }

      if (!current_floppy->add_file(file)) {
        cerr << "Could not add " << file->get_path_str() << " "
             << (file->is_directory() ? "(directory)" : "") << " to an image "
             << "for the second time due to an error, aborting." << endl;

        delete current_floppy;

        for (File *file : files) {
          delete file;
        }

        return false;
      }
    }

    at_least_one_file = true;
  }

  if (!at_least_one_file) {
    delete current_floppy;

    for (File *file : files) {
      delete file;
    }
    
    cerr << "There are no files/directories to be added to images!" << endl;
    return false;
  }
  
  bool result = current_floppy->save(options.output_filename + to_string(count)
                                     + ".img");

  delete current_floppy;

  for (File *file : files) {
    delete file;
  }

  if (!result) {
    cerr << "Could not produce a floppy disk image number " << count
         << " due to an error, aborting." << endl;
    return false;
  }

  cout << "Successfully created " << count << " floppy disk image"
       << (count > 1 ? "s" : "") << "." << endl;

  return result;
}
