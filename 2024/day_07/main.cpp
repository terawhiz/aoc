// g++ -Ofast -march=native main.cpp -o main
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <cmath>
#include <unordered_set>
#include <span>

#define FILENAME "./input"
// #define FILENAME "./test_input"

std::unordered_set<u_int64_t> checked;

void print_test_values_and_numbers(const std::vector<u_int64_t>& test_values, const std::vector<std::vector<u_int64_t>>& test_numbers) {
    // Print test_values
    std::cout << "Test values: ";
    for (const auto& value : test_values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Print test_numbers
    std::cout << "Test numbers: " << std::endl;
    for (size_t i = 0; i < test_numbers.size(); ++i) {
        std::cout << "Group " << i + 1 << ": ";
        for (const auto& number : test_numbers[i]) {
            std::cout << number << " ";
        }
        std::cout << std::endl;
    }
}

std::pair<std::vector<u_int64_t>,std::vector<std::vector<u_int64_t>>> get_input() {
  std::ifstream file(FILENAME);
  if (!file) {
    std::cerr << "Failed to open input file" << std::endl;
    std::exit(1);
  }

  std::string line;
  std::vector<u_int64_t> test_values;
  std::vector<std::vector<u_int64_t>> test_numbers;

  // parse page orders
  while (std::getline(file, line)) {
    if (line.empty()) break;

    std::replace(line.begin(), line.end(), ':', ' ');
    
    u_int64_t value;
    std::istringstream stream(line);

    stream >> value;

    std::istream_iterator<u_int64_t> itr(stream);
    std::vector<u_int64_t> numbers(itr, std::istream_iterator<u_int64_t>());
    
    test_values.push_back(value);
    test_numbers.push_back(numbers);
  }

  return {test_values, test_numbers};
}

inline u_int64_t concat(u_int64_t a, u_int64_t b) {
    // log10 lookup table, thanks claude
    static const u_int64_t powers[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    u_int64_t loga = b == 0 ? 1 : (b < 10 ? 1 : 
                     b < 100 ? 2 : 
                     b < 1000 ? 3 : 
                     b < 10000 ? 4 : 
                     b < 100000 ? 5 : 
                     b < 1000000 ? 6 : 
                     b < 10000000 ? 7 : 
                     b < 100000000 ? 8 : 9);
    return a * powers[loga] + b;
}

bool guess(u_int64_t value, std::span<u_int64_t> numbers, bool run_part_two) {

  // 10ms slower
  // if (checked.count(value)) {
  //   return true;
  // }

  if (!(numbers.size() ^ 1)){
    if (numbers[0] == value) {
      checked.insert(value);
      return true;
    } else return false;
  }
  
  u_int64_t a = numbers[0];
  u_int64_t b = numbers[1];

  // this order makes the program run fast by 20ms
  // Also HUGE speedup by ditching deque, thanks chino

  numbers[1] = a+b;
  std::span<u_int64_t> subvec(numbers.data()+1, numbers.size()-1);
  if (guess(value, subvec, run_part_two)) return true;
  numbers[0] = a;
  numbers[1] = b;
  // if (guess(value, std::vector<u_int64_t>(numbers.begin()+1, numbers.end()), run_part_two)) return true;

  numbers[1] = a*b;
  subvec = std::span<u_int64_t>(numbers.data()+1, numbers.size()-1);
  if (guess(value, subvec, run_part_two)) return true;
  numbers[0] = a;
  numbers[1] = b;
  // if (guess(value, std::vector<u_int64_t>(numbers.begin()+1, numbers.end()), run_part_two)) return true;
  
  if (run_part_two) {
    numbers[1] = concat(a, b);
    subvec = std::span<u_int64_t>(numbers.data()+1, numbers.size()-1);
    if (guess(value, subvec, run_part_two)) return true;
    numbers[0] = a;
    numbers[1] = b;
    // numbers.push_front(std::stoull(std::to_string(a) + std::to_string(b)));  // 120ms slow
    // if (guess(value, std::vector<u_int64_t>(numbers.begin()+1, numbers.end()), run_part_two)) return true;
  }

  return false;
}

u_int64_t get_calibration(std::vector<u_int64_t>& values, std::vector<std::vector<u_int64_t>> test_numbers, bool part_two) {
  u_int64_t calibration = 0;

  for (u_int64_t i = 0; i < values.size(); ++i) {
    guess(values[i], test_numbers[i], part_two) && \
      (calibration += values[i]);
  }

  return calibration;
}

int main() {
  auto [test_values, test_numbers] = get_input();

  // print_test_values_and_numbers(test_values, test_numbers);

  std::cout << "Part 1: " << get_calibration(test_values, test_numbers, false) << std::endl;
  std::cout << "Part 2: " << get_calibration(test_values, test_numbers, true) << std::endl;

  return 0;
}