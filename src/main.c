#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <ncurses.h>
#include <visual-sort.h>

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "");

    initscr();
    nocbreak();
    noecho();
    curs_set(0);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_GREEN, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_YELLOW, -1);

    visual_sort_main_menu();
    endwin();
}
