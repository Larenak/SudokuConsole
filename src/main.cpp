#include "module.h"
#include "field_io.h"
#include <windows.h>
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    enable_ansi();

    menu();
    return 0;
}