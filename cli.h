#ifndef MENU_H
#define MENU_H

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define K_ENTER 10

#define BOX_TOP_LEFT_STR "╭"
#define BOX_TOP_RIGHT_STR "╮"
#define BOX_BOT_LEFT_STR "╰"
#define BOX_BOT_RIGHT_STR "╯"
#define BOX_VERT_LINE_STR "│"
#define BOX_TOP_LINE_STR "─"
#define BOX_BOT_LINE_STR "─"


struct cli_button 
{
    char text[32];
};

struct cli_menu 
{
    char top_text[32];
    char left[32];
    char right[32];
    int selected_col;
    int normal_col;
    bool bold;     
    bool has_selected;
    int num_buttons;
    int selected;
    struct cli_button buttons[20];
};

struct cli_menu* cli_menu_create(const char* top_text, const char* left_str, const char* right_str, const int selected_col, const int normal_col, const bool bold);
void cli_menu_add_button(struct cli_menu *menu, const char* text);

struct cli_button cli_menu_get_button(const struct cli_menu menu, const uint8_t index);

void cli_menu_button_draw(const struct cli_button button, const struct cli_menu menu, const int color);
void cli_menu_draw_buttons(const struct cli_menu menu, const uint8_t gaps[]);
int cli_menu_run(struct cli_menu *menu, const uint8_t gaps[], const int16_t dy, const bool clear_screen);

void cli_menu_display_labels(const struct cli_menu menu, const uint8_t gaps[], const int16_t dy, const bool clear_screen);

int cli_menu_get_height(const struct cli_menu menu, const uint8_t gaps[]);

int cli_menu_handle_input(struct cli_menu *menu);

void cli_draw_box(const uint8_t box_width, const uint8_t box_height, const uint8_t start_x, const uint8_t start_y, const bool draw_sides);

int cli_get_cur_x();
int cli_get_cur_y();
int cli_get_scrw();
int cli_get_scrh();
void cli_move_center_v(int dy);
void cli_move_center_h(int dx);

#endif
