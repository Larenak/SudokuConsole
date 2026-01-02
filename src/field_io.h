#ifndef FIELD_IO_H
#define FIELD_IO_H

#include <vector>

void clear_lines(int lines);

void print_field(const std::vector<std::vector<int>> field, bool isInFile);

std::vector<std::vector<int>> read_field();
void enable_ansi();
#endif // FIELD_IO_H