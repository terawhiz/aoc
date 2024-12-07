// g++ main.cpp -o main 
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <deque>

#define FILENAME "./input"
// #define FILENAME "./test_input"

enum Forward {
  UNMAPPED = 'U',
  OBSTACLE = '#',
  BLOCK    = 'O',
  CLEAR    = '.',
  VISITED  = 'X',
  CORNER   = '+',
  DUNNO    = 'D'
};

typedef std::vector<std::vector<char>> matrix_t;

struct pos_t {
  int x;
  int y;
};

unsigned moves = 0;
unsigned blocks = 0;

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

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

std::pair<matrix_t,pos_t> get_matrix() {
  matrix_t matrix;
  std::stringstream s;
  std::string input;
  std::string line;

  std::ifstream file(FILENAME);
  if (!file) {
    std::cerr << "Failed to open input file" << std::endl;
    std::exit(1);
  }

  int x = 0;
  int y = 0;

  while (std::getline(file, line)) {
    std::vector<char> row;

    for (int j = 0; j < line.size(); ++j) {
      row.push_back(line[j]);
      if (line[j] == '^') {
        x = matrix.size();
        y = j;
      }
    }

    matrix.push_back(row);
  }

  return { matrix, {x, y}};
}

enum Forward in_mapped_area(matrix_t& matrix, int max_row, int max_col, int row, int col) {
  if (row < 0 || row > max_row || col < 0 || col > max_col) {
    return UNMAPPED;
  } 
  
  return static_cast<Forward>(matrix[row][col]);
}

std::pair<int,int> turn_right(std::pair<int,int> dir) {
  return {
    dir.second,
    -dir.first
  };
}

std::pair<int,int> get_guard_direction(char guard) {
  switch (guard) {
    case '^': // up
      return {-1, 0};
    case '>': // right
      return {0, 1};
    case 'V': // down
      return {1, 0};
    case '<': // left
      return {0, -1};
  }

  return {0, 0};
}

bool make_moves(matrix_t& matrix, pos_t& pos, std::pair<int, int> dir) {
  while (true) {
    int i = dir.first;
    int j = dir.second;
    char chr = '\0';

    switch (
      in_mapped_area(
        matrix, matrix.size() - 1, matrix[0].size() - 1,
        pos.x + i, pos.y + j
      )
    ) {
      case UNMAPPED:
        ++moves;
        return false;

      case CORNER:
        chr = matrix[pos.x + i + i][pos.y + j + j];
        if (chr == VISITED || chr == CLEAR) {
          pos.x += i;
          pos.y += j;
          break;
        }

        return true;

      case BLOCK:
      case OBSTACLE:
        matrix[pos.x][pos.y] = '+';
        dir = turn_right(dir);
        break;

      case CLEAR:
        ++moves;
        matrix[pos.x + i][pos.y + j] = 'X';
      case VISITED:

        pos.x += i;
        pos.y += j;
        break;
    }
  }
}


void part_one(matrix_t matrix, pos_t pos) {
  std::pair<int,int> dir = get_guard_direction(matrix[pos.x][pos.y]);

  matrix[pos.x][pos.y] = 'X';
  make_moves(matrix, pos, dir);
}

void part_two(matrix_t matrix, pos_t guard_pos) {
  std::pair<int,int> dir = get_guard_direction(matrix[guard_pos.x][guard_pos.y]);
  
  // hardcoded optimization, 0.32s avg
  // for (int i = 9; i < matrix.size()-3; ++i) {
  //   for (int j = 0; j < matrix[0].size()-4; ++j) {

  for (int i = 0; i < matrix.size(); ++i) {
    for (int j = 0; j < matrix[0].size(); ++j) {
      auto dummy = matrix;
      auto pos = guard_pos;

      if (
        (i == pos.x && j == pos.y) || \
        dummy[i][j] == '#'
      ) continue;

      dummy[i][j] = 'O';

      dummy[pos.x][pos.y] = 'X';
      if (make_moves(dummy, pos, dir)) {
        blocks++;
      }
    }
  }
}

int main() {
  auto [matrix, guard_pos] = get_matrix();
  // print_matrix(matrix);

  int rows = matrix.size();
  int cols = matrix[0].size();

  part_one(matrix, guard_pos);
  std::cout << "Moves: " << moves << std::endl;

  part_two(matrix, guard_pos);
  std::cout << "Blocks: " << blocks << std::endl;

  return 0;
}