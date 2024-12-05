// g++ main.cpp -o main 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <map>
#include <unordered_set>

// #define FILENAME "./test_input"
#define FILENAME "./input"

using graph_t   = std::map<int,std::unordered_set<int>>;
using update_t  = std::vector<std::vector<int>>;

std::pair<graph_t,update_t> 
get_data() {
  std::ifstream file(FILENAME);
  if (!file) {
    std::cerr << "Failed to open input file" << std::endl;
    std::exit(1);
  }

  std::string line;
  graph_t pg_dep;

  // parse page orders
  while (std::getline(file, line)) {
    int pipe_pos;
    
    if (line.find("|") == std::string::npos) {
      break;
    }

    std::replace(line.begin(), line.end(), '|', ' ');
    std::istringstream stream(line);
    std::istream_iterator<int> itr(stream);

    int before = *itr;
    int after  = *++itr;
    pg_dep[after].insert(before);
  }

  std::vector<std::vector<int>> updates;

  // parse updates
  while (getline(file, line)) {
    int num;

    std::replace(line.begin(), line.end(), ',', ' ');
    std::istringstream stream(line);
    std::vector<int> update{
      std::istream_iterator<int>(stream), 
      std::istream_iterator<int>()
    };

    updates.push_back(update);
  }

  return {pg_dep, updates};
}


int main() {
  unsigned sum = 0;
  unsigned sum_2 = 0;

  auto [pg_dep, updates] = get_data();
  
  auto check_valid_update = [&](int a, int b) {
    return pg_dep.count(b) && pg_dep.at(b).count(a);
  };

  for (auto update : updates) {
    if (std::is_sorted(update.begin(), update.end(), check_valid_update)) {
      sum += update[update.size() / 2];
      continue;
    }

    std::sort(update.begin(), update.end(), check_valid_update);
    sum_2 += update[update.size() / 2];
  }

  std::cout << "Part 1 sum: " << sum << std::endl;
  std::cout << "Part 2 sum: " << sum_2 << std::endl;

  return 0;
}