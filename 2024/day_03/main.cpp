// g++ main.cpp -o main
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <regex>

#define FILENAME "./input"
// #define FILENAME "./test_input"

std::vector<std::pair<int, int>> parse_input(std::string input, bool part_two) {
  std::vector<std::pair<int, int>> operands;
  bool enabled = true;

  std::regex regex;
  std::smatch match;

  if (part_two) {
    regex = "((mul\\((\\d{1,3}),(\\d{1,3})\\))|(do\\(\\))|(don't\\(\\)))";
  } else {
    regex = "(mul\\((\\d{1,3}),(\\d{1,3})\\))";
  }

  std::string sub_str = input;
  while (std::regex_search(sub_str, match, regex)) {
    if (match[part_two + 1].matched) {
      int op1 = 0, op2 = 0;

      if (enabled) {
        op1 = std::stoi(match[part_two + 2].str());
        op2 = std::stoi(match[part_two + 3].str());
      } else {
        op1 = op2 = 0;
      }

      operands.push_back({op1, op2});
    }
    else if (match[part_two + 4].matched && part_two) {
      enabled = true;
    }
    else if (match[part_two + 5].matched && part_two) {
      enabled = false;
    }

    sub_str = match.suffix().str();
  }

  return operands;
}

int main() {
  unsigned int sum_1 = 0;
  unsigned int sum_2 = 0;

  std::ifstream file(FILENAME);
  if (!file) {
    std::cerr << "Failed to open input file" << std::endl;
    std::exit(-1);
  }

  std::stringstream s;

  s << file.rdbuf();
  std::string input = s.str();


  // part one
  auto operands = parse_input(input, false);
  for (auto& operand : operands) {
    sum_1 += operand.first * operand.second; 
  }

  operands.clear();

  // part two
  operands = parse_input(input, true);
  for (auto& operand : operands) {
    sum_2 += operand.first * operand.second; 
  }
  

  std::cout << "Sum 1: " << sum_1 << std::endl;
  std::cout << "Sum 2: " << sum_2 << std::endl;

  return 0;
}