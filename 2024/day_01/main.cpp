// g++ ./main.cpp -o main
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

// #define FILENAME "./test_input"
#define FILENAME "./input"

std::pair<std::vector<int>, std::vector<int>> parse_file(std::ifstream &file) {
  std::string line;
  std::vector<int> firstArr, secondArr;

  while (std::getline(file, line)) {
    std::size_t pos = line.find("   ");

    firstArr.push_back(std::atoll(line.substr(0, pos).c_str()));
    secondArr.push_back(std::atoll(line.substr(pos+3).c_str()));
  }

  return {firstArr, secondArr};
}

int main() {
  int total_similarity = 0;
  int similarity = 0;
  int total_distance = 0;
  int temp_distance = 0;
  std::vector<int> arr1, arr2;

  // open file, read file contents
  std::ifstream inputFile(FILENAME);
  if (!inputFile) {
    std::cerr << "Failed to open file" << std::endl;
  }

  // parse file contents
  auto arrs = parse_file(inputFile);
  arr1 = arrs.first;
  arr2 = arrs.second;

  // sort arrays
  std::sort(arr1.begin(), arr1.end(), std::less<int>());
  std::sort(arr2.begin(), arr2.end(), std::less<int>());

  // find total distance
  for (int idx = 0; idx < arr1.size(); ++idx) {
    temp_distance = std::abs(arr1[idx] - arr2[idx]);
    total_distance += temp_distance;
  }

  // find total similarity
  for (auto &elem : arr1 ) {
    int count = std::count(arr2.begin(), arr2.end(), elem);
    total_similarity += elem * count;
  }

  std::cout << "Total distance: " << total_distance << std::endl;
  std::cout << "Total similarity: " << total_similarity << std::endl;

  return 0;
}