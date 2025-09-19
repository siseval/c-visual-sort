#include <stdlib.h>
#include <string.h>
#include <cli.h>

struct cli_menu* cli_menu_create(const char* top_text, const char* left_str, const char* right_str, const int selected_col, const int normal_col, const bool bold)
{
    struct cli_menu* menu = malloc(sizeof(struct cli_menu));
    if (!menu)
    {
        fprintf(stderr, "cli_menu_create: menu malloc failed\n");
        return NULL;
    }

    strcpy(menu->top_text, top_text);
    strcpy(menu->left, left_str);
    strcpy(menu->right, right_str);
    menu->selected_col = selected_col;
    menu->normal_col = normal_col;
    menu->bold = bold;
    menu->has_selected = false;
    menu->num_buttons = 0;
    menu->selected = 0;

    return menu;
}

void cli_menu_add_button(struct cli_menu *menu, const char* text)
{
    if (menu->num_buttons >= 20)
    {
        fprintf(stderr, "cli_menu_add_button: maximum number of buttons reached\n");
        return;
    }
    strcpy(menu->buttons[menu->num_buttons].text, text);
    menu->num_buttons++;
}

void cli_menu_button_draw(const struct cli_button button, const struct cli_menu menu, const int col)
{
    attron(COLOR_PAIR(col));

    cli_move_center_h(-((int)(strlen(button.text) + strlen(menu.left) + strlen(menu.right)) / 2));
    printw("%s%s%s\n\r", menu.left, button.text, menu.right);

    attroff(COLOR_PAIR(col));
}

void cli_menu_draw_buttons(const struct cli_menu menu, const uint8_t gaps[])
{
    for (int i = 0; i < menu.num_buttons; i++) 
    {
        const struct cli_button b = menu.buttons[i];
        for (int j = 0; j < gaps[i]; j++) 
        {
            printw("\n");
        }
        cli_menu_button_draw(b, menu, menu.selected == i ? menu.selected_col : menu.normal_col);
    }
}

int cli_menu_run(struct cli_menu *menu, const uint8_t gaps[], const int16_t dy, const bool clear_screen)
{
    menu->bold ? attron(A_BOLD) : attroff(A_BOLD);
    int selection = menu->selected;
    while (!menu->has_selected)
    {
        if (clear_screen)
        {
            erase();
        }
        cli_move_center_v(-(cli_menu_get_height(*menu, gaps) / 2) + dy);
        cli_move_center_h(-(strlen(menu->top_text) / 2));

        printw("%s", menu->top_text);
        cli_menu_draw_buttons(*menu, gaps);

        selection = cli_menu_handle_input(menu);
    }
    attroff(A_BOLD);
    return selection;
}

void cli_menu_display_labels(const struct cli_menu menu, const uint8_t gaps[], const int16_t dy, const bool clear_screen)
{
    menu.bold ? attron(A_BOLD) : attroff(A_BOLD);

    if (clear_screen)
    {
        erase();
    }

    cli_move_center_v(-(cli_menu_get_height(menu, gaps) / 2) + dy);
    cli_move_center_h(-(strlen(menu.top_text) / 2));

    printw("%s", menu.top_text);
    cli_menu_draw_buttons(menu, gaps);
}

int cli_menu_get_height(const struct cli_menu menu, const uint8_t gaps[])
{
    int height = menu.num_buttons + 1;
    for (int i = 0; i < menu.num_buttons; i++)
    {
        height += gaps[i];
    }
    return height;
}

struct cli_button cli_menu_get_button(const struct cli_menu menu, const uint8_t index)
{
    return menu.buttons[index]; 
}

int cli_menu_handle_input(struct cli_menu *menu) 
{
    switch (getch()) 
    {
        case 'k':
        case 'w':
            menu->selected -= menu->selected <= 0 ? 0 : 1;
            break;

        case 'j':
        case 's':
            menu->selected += menu->selected >= menu->num_buttons - 1 ? 0 : 1;
            break;

        case ' ':
        case 'f':
        case K_ENTER:
            menu->has_selected = true;
            break;
    }
    return menu->selected;
}

void cli_draw_box(const uint8_t box_width, const uint8_t box_height, const uint8_t start_x, const uint8_t start_y, const bool draw_sides)
{
    int8_t top_y = start_y;
    int8_t bot_y = start_y + box_height + 1;
    int8_t left_x = start_x;
    int8_t right_x = start_x + box_width;

    mvaddstr(top_y, left_x, BOX_TOP_LEFT_STR);
    mvaddstr(bot_y, left_x, BOX_BOT_LEFT_STR);
    for (int i = 1; i <= right_x - left_x; i++)
    {
        mvaddstr(top_y, left_x + i, BOX_TOP_LINE_STR);
        mvaddstr(bot_y, left_x + i, BOX_BOT_LINE_STR);
    }
    mvaddstr(top_y, right_x + 1, BOX_TOP_RIGHT_STR);
    mvaddstr(bot_y, right_x + 1, BOX_BOT_RIGHT_STR); 
    if (!draw_sides)
    {
        return;
    }
    for (int i = 1; i < bot_y - top_y; i++)
    {
        mvaddstr(top_y + i, left_x, BOX_VERT_LINE_STR);
        mvaddstr(top_y + i, right_x + 1, BOX_VERT_LINE_STR);
    }
}

int cli_get_cur_x()
{
    int y, x;
    getyx(stdscr, y, x);
    return x;
}

int cli_get_cur_y(void)
{
    int y, x;
    getyx(stdscr, y, x);
    return y;
}

int cli_get_scrw(void)
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}

int cli_get_scrh(void)
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrh;
}

void cli_move_center_v(const int dy)
{
    move(cli_get_scrh() / 2 + dy, cli_get_cur_x());
}

void cli_move_center_h(const int dx)
{
    move(cli_get_cur_y(), cli_get_scrw() / 2 + dx);
}
