// g++ main.cpp -o main 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define FILENAME "./input"
// #define FILENAME "./test_input"

int xmas_occurrence = 0;
int x_mas_occurrence = 0;

typedef std::vector<std::vector<char>> matrix_t;

struct coords {
  int i;
  int j;
};

typedef struct {
  struct coords cor_one;
  struct coords cor_two;
  char cor_one_value;
  char cor_two_value;
} pair_t;

void print_matrix(matrix_t& matrix) {
    // Print column indices
    std::cout << "    ";  // Indentation for row indices
    for (size_t col = 0; col < matrix[0].size(); ++col) {
        std::cout << col << ' ';
    }
    std::cout << std::endl;

    // Print each row with the row index
    for (size_t row = 0; row < matrix.size(); ++row) {
        // Print the row index followed by the row's contents
        std::cout << row << " | ";
        for (char cell : matrix[row]) {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}

matrix_t get_matrix() {
  matrix_t matrix;
  std::stringstream s;
  std::string input;
  std::string line;

  std::ifstream file(FILENAME);
  if (!file) {
    std::cerr << "Failed to open input file" << std::endl;
    std::exit(1);
  }

  while (std::getline(file, line)) {
    std::vector<char> row;

    for (int j = 0; j < line.size(); ++j) {
      row.push_back(line[j]);
    }

    matrix.push_back(row);
  }

  return matrix;
}

bool validate_direction(int max_row, int max_col, int row, int col) {
  if (
    row < 0 || row > max_row || col < 0 || col > max_col
  ) return false;

  // std::cout << row << " " << max_row << " - " << col << " " << max_col << std::endl;
  return true;
}

void check_direction(
    const matrix_t& matrix, std::string& word, int word_idx,
    int row, int col, 
    std::pair<int, int> dir
  ) {

  if (word_idx == 3) return;

  int i = dir.first;
  int j = dir.second;

  if (!validate_direction(matrix.size()-1, matrix[0].size()-1, row+i, col+j)) return;

  if (word[++word_idx] == matrix[row+i][col+j]) {
    // std::cout << "checking matrix[" << word_idx << "]: " << matrix[row+i][col+j] << "at: " << row+i << " " << col+j << std::endl;
    if (word[word_idx] == 'S' && word_idx == 3) {
      xmas_occurrence++;
      return;
    }

    return check_direction(matrix, word, word_idx, row+i, col+j, dir);
  }
}

void part_one(matrix_t matrix, std::string word) {
  std::vector<std::pair<int, int>> dirs = {
    // row, col
    {-1, -1}, // top left
    {-1,  0}, // top mid
    {-1,  1}, // top right
    { 0,  1}, // mid right
    { 0, -1}, // mid left
    { 1, -1}, // bottom left
    { 1,  0}, // bottom mid
    { 1,  1}, // bottom right
  };

  for (int row = 0; row < matrix.size(); ++row) {
    for (int col = 0; col < matrix[row].size(); ++col) {

      if (matrix[row][col] != word[0]) continue;

      // check all 8 directions
      for (auto& dir : dirs)
        check_direction(matrix, word, 0, row, col, dir);
    }
  }
}

void check_corners(matrix_t matrix, int row, int col) {
  std::vector<std::pair<char,char>> cross;
  std::vector<pair_t> pairs = {
    { {-1, -1}, {1,  1}, '\0', '\0' },
    { {-1,  1}, {1, -1}, '\0', '\0' },
  };

  int x = 0;
  
  // std::cout << "--------------------------- start ----------------------------" << std::endl;

  for (pair_t& pair: pairs) {
    if (!validate_direction(matrix.size()-1, matrix[0].size()-1, row+pair.cor_one.i, col+pair.cor_one.j)) return;
    if (!validate_direction(matrix.size()-1, matrix[0].size()-1, row+pair.cor_two.i, col+pair.cor_two.j)) return;

    pair.cor_one_value = matrix[row+pair.cor_one.i][col+pair.cor_one.j];
    pair.cor_two_value = matrix[row+pair.cor_two.i][col+pair.cor_two.j];

    if (pair.cor_one_value == pair.cor_two_value) return;
    if (
      (pair.cor_one_value == 'M' || pair.cor_one_value == 'S') && \
      (pair.cor_two_value == 'M' || pair.cor_two_value == 'S')
    ) {
      // std::cout << "corner one: " << pair.cor_one_value << " corner two: " << pair.cor_two_value << std::endl;
      x++;
    }
  }
  // std::cout << "---------------------------- end ---------------------------" << std::endl;

  if (x == 2) {
    x_mas_occurrence++;
    // std::cout << "x: " << row << " y: " << col << "\t" << "total: " << x_mas_occurrence << std::endl;
  }
}

void part_two(matrix_t matrix, std::string word) {
  for (int row = 0; row < matrix.size(); ++row) {
    for (int col = 0; col < matrix[row].size(); ++col) {

      if (matrix[row][col] != word[0]) continue;

      check_corners(matrix, row, col);
    }
  }
}

int main() {

  matrix_t matrix = get_matrix();
  // print_matrix(matrix);

  int rows = matrix.size();
  int cols = matrix[0].size();

  part_one(matrix, "XMAS");
  part_two(matrix, "A");

  std::cout << "XMAS: " << xmas_occurrence << std::endl;
  std::cout << "X-MAS: " << x_mas_occurrence << std::endl;

  return 0;
}