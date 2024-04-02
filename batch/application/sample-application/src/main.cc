// Copyright 2024 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0]
              << " <input-file-name> <output-file-name>\\n";
    return 1;
  }

  std::string const input_file_name = argv[1];
  std::string const output_file_name = argv[2];

  std::ifstream input_file(input_file_name);
  std::ofstream output_file(output_file_name);

  // Check if files were opened successfully
  if (!input_file.is_open()) {
    std::cerr << "Error opening input file: " << input_file_name << "\n";
    return 1;
  }

  // Read the entire file contents into a string.
  std::string contents;
  getline(input_file, contents, '\0');

  // Convert contents to uppercase.
  for (char& c : contents) {
    c = toupper(c);
  }

  if (!output_file.is_open()) {
    std::cerr << "Error opening output file: " << output_file_name << "\n";
    return 1;
  }
  output_file << contents;

  // Close the files
  input_file.close();
  output_file.close();

  return 0;
}