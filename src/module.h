#ifndef MODULE_H
#define MODULE_H

#include <vector>

bool isCorrect(const std::vector<std::vector<int>> &field, int i, int j, int square_num);
std::vector<std::vector<int>> get_field();
std::vector<std::vector<int>> get_unsolved_field(const std::vector<std::vector<int>> field, int number_empty_squares);

void start_play_sudoku(const std::vector<std::vector<int>> field,
                       std::vector<std::vector<int>> unsolved_field,
                       int number_empty_squares);

bool solve_field(std::vector<std::vector<int>> &board);
void start_solve_field();
void menu();
void play_random_sudoku();
void load_and_play_sudoku();

#endif // MODULE_H