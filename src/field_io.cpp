#include <iostream>
#include <fstream>
#include <windows.h>
#include "field_io.h"

void clear_lines(int lines)
{
    for (int i = 0; i < lines; i++)
    {
        std::cout << "\x1b[1F";
        std::cout << "\x1b[2K";
    }
}

void print_field(std::vector<std::vector<int>> field, bool isInFile)
{
    if (!isInFile)
    {
        std::ifstream input("field.txt");
        std::string line;
        while (std::getline(input, line))
        {
            std::cout << line << std::endl;
        }
        input.close();
    }
    else
    {
        std::ofstream output("field.txt");
        output << "\n    1 2 3   4 5 6   7 8 9\n";
        output << "  +-------+-------+-------+\n";

        for (int i = 0; i < 9; i++)
        {
            output << i + 1 << " | ";

            for (int j = 0; j < 9; j++)
            {
                if (field[i][j] == 0)
                    output << ". ";
                else
                    output << field[i][j] << " ";

                if ((j + 1) % 3 == 0)
                    output << "| ";
            }

            output << "\n";

            if ((i + 1) % 3 == 0)
                output << "  +-------+-------+-------+\n";
        }
        output.close();
    }
}
std::vector<std::vector<int>> read_field()
{
    std::ifstream input("field.txt");
    std::string character;
    std::vector<std::vector<int>> field(9, std::vector<int>(9, 0));
    int i = 0;
    int k = 0;
    int nums_count = 0; // Для проверки корректности поля добавляем счётчик чисел
    while (input >> character && i < 9)
    {

        // Когда k == 3 мы полностью считали числа со строки поля. Обнуляем k и переходим к следующей строке.
        if (k == 3)
        {
            i++;
            k = 0;
            for (int index = 0; index < 2; index++)
            {
                input >> character; // Переход между строками
            }
        }
        if (character == "|")
        {
            for (int j = 0; j < 3; j++)
            {
                try
                {
                    input >> character;
                    if (character == "|")
                    {
                        // Поле в файле записано неверно т.к. пытаемся считать лишний символ "|"
                        // Возвращаем пустой вектор
                        return {};
                    }
                    nums_count++;
                    if (character != ".")
                    {
                        field[i][j + k * 3] = stoi(character);
                    }
                    else
                    {
                        field[i][j + k * 3] = 0;
                    }
                }
                catch (...)
                {
                    return {};
                }
            }
            k++;
        }
    }
    input.close();
    if (nums_count != 81)
    {
        return {};
    }
    return field;
}
void enable_ansi()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}