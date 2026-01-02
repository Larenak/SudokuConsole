#include <random>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <windows.h>
#include "field_io.h"
#include "module.h"

bool isCorrect(const std::vector<std::vector<int>> &field, int i, int j, int square_num)
{
    for (int k = 0; k < 9; k++)
        if (field[i][k] == square_num && k != j)
            return false;

    for (int k = 0; k < 9; k++)
        if (field[k][j] == square_num && k != i)
            return false;

    int bi = (i / 3) * 3;
    int bj = (j / 3) * 3;
    for (int di = 0; di < 3; di++)
        for (int dj = 0; dj < 3; dj++)
            if (field[bi + di][bj + dj] == square_num && (bi + di != i || bj + dj != j))
                return false;

    return true;
}

std::vector<std::vector<int>> get_field()
{
    std::mt19937 rng(std::time(nullptr));
    std::vector<std::vector<int>> field(9, std::vector<int>(9, 0));
    std::vector<std::vector<int>> used_numbers(9, std::vector<int>(9, 0));
    int i = 0;
    int j = 0;
    while (i < 9)
    {
        bool placed = false;

        std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::shuffle(nums.begin(), nums.end(), rng);

        for (int square_num : nums)
        {
            if (square_num <= used_numbers[i][j])
                continue; // Если это число уже пробовали в данной клетке - пропускаем

            if (isCorrect(field, i, j, square_num))
            {
                field[i][j] = square_num;
                used_numbers[i][j] = square_num;
                placed = true;
                break;
            }
        }
        if (placed)
        {
            j++;
            if (j == 9)
            {
                j = 0;
                i++;
            }
        }
        else
        {
            field[i][j] = 0;
            used_numbers[i][j] = 0;

            j--;
            if (j < 0)
            {
                j = 8;
                i--;
            }
        }
    }

    return field;
}
std::vector<std::vector<int>> get_unsolved_field(std::vector<std::vector<int>> field, int number_empty_squares)
{
    std::mt19937 rng(std::time(nullptr));
    for (int i = 0; i < number_empty_squares; i++)
    {
        int index_empty_square = rng() % 81;
        field[index_empty_square / 9][index_empty_square % 9] = 0;
    }
    print_field(field, true);
    return field;
}

void start_play_sudoku(std::vector<std::vector<int>> field, std::vector<std::vector<int>> unsolved_field, int number_empty_squares)
{
    bool isExit = false;
    while (number_empty_squares > 0 && !isExit)
    {
        std::cout << "\nВведите номер ряда(число над доской) и строки(число слева от доски) клетки, в которую хотите ввести значение."
                     " Или введите \"0 0\", если хотите сохраниться и выйти: \n";
        int i = 0;
        int j = 0;

        while ((i < 1 || i > 9 || j < 1 || j > 9) && (!isExit))
        {
            std::cin >> i >> j;
            if (i == 0 && j == 0)
            {
                isExit = true;
                print_field(unsolved_field, true);
                clear_lines(35);
                std::cout << "Игра успешно сохранена!\n";
                continue;
            }
            else if (i < 1 || i > 9 || j < 1 || j > 9)
            {
                clear_lines(2);
                std::cout << "Недопустимые значения!(нет такого ряда или строки). Введите снова:\n";
            }
            else if (unsolved_field[j - 1][i - 1] != 0)
            {
                clear_lines(2);
                std::cout << "На этой клетке уже записано число! Введите снова: \n";
                i = 0;
                j = 0;
            }
        }
        if (isExit)
        {
            continue;
        }
        clear_lines(4);
        // Уменьшаем на 1 т.к. нумерация элементов вектора начинается с 0
        i--;
        j--;
        std::cout << "\nВведите число от 1 до 9:\n";
        int value = 0;
        while (value > 9 || value < 1)
        {
            std::cin >> value;
            if (value > 9 || value < 1)
            {
                clear_lines(2);
                std::cout << "Недопустимое значение! Число не находится в диапазоне от 1 до 9. Введите снова:\n";
            }
        }
        clear_lines(2);
        if (field[j][i] == value)
        {
            unsolved_field[j][i] = value;
            number_empty_squares--;
            clear_lines(24);
            print_field(unsolved_field, true);
            print_field(unsolved_field, false);
        }
        else
        {
            clear_lines(24);
            print_field(unsolved_field, true);
            print_field(unsolved_field, false);
            std::cout << "Неверное значение!";
        }
    }
    if (!isExit) // Возможное сообщение о победе только тогда, когда пользователь не вышел из игры
    {
        clear_lines(24);
        print_field(field, false);
        std::cout << "\nУ вас получилось!";
    }
}
bool solve_field(std::vector<std::vector<int>> &board)
{

    std::vector<std::pair<int, int>> empty_cells;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (board[r][c] == 0)
                empty_cells.push_back({r, c});

    int index = 0;
    while (index < empty_cells.size())
    {
        int i = empty_cells[index].first;
        int j = empty_cells[index].second;
        bool found = false;
        // Начинаем подбор со следующего числа, чтобы не повторять уже проверенные варианты
        int start = board[i][j] + 1;
        for (int num = start; num <= 9; num++)
        {
            if (isCorrect(board, i, j, num))
            {
                board[i][j] = num;
                found = true;
                break;
            }
        }
        if (found)
        {
            index++;
        }
        else
        {
            board[i][j] = 0;
            index--;
            if (index < 0)
                return false;
        }
    }
    return true;
}
void start_solve_field()
{
    clear_lines(35);
    std::vector<std::vector<int>> unsolved_field = read_field();
    if (unsolved_field == std::vector<std::vector<int>>{})
    {
        clear_lines(3);
        std::cout << "\nНеверная запись поля! В файл было записано чистое поле."
                     "Введите ваше Судоку в файл повторно, а затем запустите решение заново.\n";
        print_field(std::vector<std::vector<int>>(9, std::vector<int>(9, 0)), true); // Запись чистого корректного поля
        return;
    }
    bool isSolved = solve_field(unsolved_field);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (!isCorrect(unsolved_field, i, j, unsolved_field[i][j]))
            {
                isSolved = false;
            }
        }
    }
    if (isSolved)
    {
        print_field(unsolved_field, true);
        print_field(unsolved_field, false);
        std::cout << "\nСудоку решено!";
    }
    else
    {
        std::cout << "Решений у данного Судоку нет.\n";
    }
}

void play_random_sudoku()
{
    clear_lines(37);
    std::mt19937 rng(std::time(nullptr));
    std::vector<std::vector<int>> field = get_field();
    int number_empty_squares = rng() % 10 + 30; // Оптимальное кол-во пустых клеток поля (чем больше, тем сложнее игра)
    std::vector<std::vector<int>> unsolved_field = get_unsolved_field(field, number_empty_squares);
    print_field(unsolved_field, true);
    clear_lines(6);
    print_field(unsolved_field, false);
    start_play_sudoku(field, unsolved_field, number_empty_squares);
}

void load_and_play_sudoku()
{
    std::vector<std::vector<int>> unsolved_field = read_field();
    std::vector<std::vector<int>> field = unsolved_field;
    solve_field(field);
    int number_empty_squares = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (unsolved_field[i][j] == 0)
            {
                // Считаем кол-во пустых клеток и на основе этого создаём решение поля
                number_empty_squares++;
            }
        }
    }
    clear_lines(35);
    print_field(unsolved_field, false);
    start_play_sudoku(field, unsolved_field, number_empty_squares);
}

void menu()
{
    int operation = 1;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    enable_ansi();

    while (operation != 0)
    {
        std::cout << "\n====================================================\n"
                     "Добро пожаловать в Судоку! Выберите режим игры:\n"
                     "1. Играть в случайное Судоку.\n"
                     "2. Решить ваше Судоку. Оно должно быть в файле field.txt (Помечайте пустые клетки поля с помощью \".\")\n"
                     "3. Загрузить игру из файла.\n"
                     "0. Выход\n";
        std::cin >> operation;
        if (operation == 1)
        {
            play_random_sudoku();
        }
        else if (operation == 2)
        {
            start_solve_field();
        }
        else if (operation == 3)
        {
            load_and_play_sudoku();
        }
        else if (operation != 0)
        {
            std::cout << "Неверный режим! Попробуйте снвоа.";
        }
    }
}