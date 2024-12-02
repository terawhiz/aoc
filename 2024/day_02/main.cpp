// g++ main.cpp -o main
#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <sstream>

#define FILENAME "./input"
// #define FILENAME "./test_input"

enum order {
  ASC = 1 << 8,
  DESC = 1 << 0X10
};

void print_report(std::vector<int> report) {
  for (int level : report) {
      std::cout << level << " ";
  }
}

void print_reports(std::vector<std::vector<int>> reports) {
  for (const auto& report : reports) {
    print_report(report);
    std::cout << std::endl;
  }
}

std::vector<std::vector<int>> parse_reports(std::ifstream &file) {
  int level = 0;
  std::string line;
  std::vector<int> report;
  std::vector<std::vector<int>> reports;

  while (getline(file, line)) {
    std::istringstream iss(line);

    while (iss >> level) {
      report.push_back(level);
    }

    reports.push_back(report);
    report.clear();
  }

  return reports;
}

bool check_without_damp(std::vector<int> &report) {
  int order = 0;

  for (int idx = 0; idx < report.size() - 1; ++idx) {
    int curr = report[idx];
    int next = report[idx+1];

    if (curr < next) order |= ASC;
    else order |= DESC;

    int diff = std::abs(curr - next);
    if (diff < 1 || diff > 3 || (
      (order & ASC) && (order & DESC)
    )) {
      // print_report(report);
      return false;
    }
  }

  return true;
}

bool check_with_damp(std::vector<int> & report) {
  for (int idx = 0; idx < report.size(); ++idx) {
    auto dup = report;
    dup.erase(dup.begin() + idx);
    if (check_without_damp(dup)) return true;
  }

  return false;
}

// test case incompleme, and I'm dumb
// bool check_with_damp(std::vector<int> &report) {
//   int order = 0;
//   int defect = 0;

//   for (int idx = 0; idx < report.size() - 1; ++idx) {
//     int diff = 0;
//     int curr = report[idx];
//     int next = report[idx+1];

//     if (defect) {
//       print_report(report);
//       std::cout << " <---" << curr << " - " << next << std::endl;
//     }


//     if (curr < next) order |= ASC;
//     else order |= DESC;

//     diff = std::abs(curr - next);
//     if (diff > 3) return false;

//     if (diff < 1 || (order & ASC) && (order & DESC)) {

//       if (defect) return false;

//       defect++;
//       report.erase(report.begin() + idx);

//       idx = 0;
//       order = 0;

//     }
//   }

//   return true;
// }

int main() {
  // open file
  std::ifstream input(FILENAME);

  // parse reports
  auto reports = parse_reports(input);

  // print vector
  // print_reports(reports);

  // check if report is safe, without problem dampener
  int safeness = 0;
  for (std::vector<int> &report : reports) {
    check_without_damp(report) && safeness++;
  }

  // check if report is safe, with problem dampener
  int real_safeness = 0;
  for (std::vector<int> &report : reports) {
    check_with_damp(report) && real_safeness++;
  }

  // // find total safe reports
  std::cout << "Safe reports without dampener: " << safeness << std::endl;
  std::cout << "Safe reports with dampener: " << real_safeness << std::endl;

  return 0;
}