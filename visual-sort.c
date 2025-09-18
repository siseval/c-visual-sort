#include "visual-sort.h"

static int32_t iteration = 0;
static int64_t max_num = 0;
static int16_t frame_ms = 0;
static double ms_mult = 1.0;
static bool show_ui = false;
static bool should_quit = false;


static void update_frame_ms(const int16_t ms)
{
    timeout(frame_ms * ms_mult);
}

static void set_ms_mult(const double mult)
{
    ms_mult = mult;
    if (ms_mult < 0.1)
    {
        ms_mult = 0.1;
    }
    update_frame_ms(frame_ms);
}

static void toggle_ui(void)
{
    show_ui = !show_ui;
}

static void handle_input(void)
{
    switch (getch())
    {
        case 'q':
            should_quit = true;
            return;
        case 'k':
            set_ms_mult(ms_mult + 0.5);
            break;
        case 'j':
            set_ms_mult(ms_mult - 0.5);
            break;
        case 'u':
            toggle_ui();
    }
    refresh();
}

static void draw_ui(const enum visual_sort_type sort_type)
{
    char algorithm_str[32];
    sprintf(algorithm_str, " ALGORITHM: %s", visual_sort_type_str[sort_type]);

    char iteration_str[32];
    sprintf(iteration_str, " ITERATION: %d", iteration++);

    char max_num_str[32];
    sprintf(max_num_str, " MAX NUMBER: %lld", max_num);

    char frame_ms_str[32];
    sprintf(frame_ms_str, " FRAME MS: %d", frame_ms);

    char strings[4][32] = { *algorithm_str, *iteration_str, *max_num_str, *frame_ms_str };
    uint16_t box_width = 0;
    for (uint8_t i = 0; i < 32; i++)
    {
        uint16_t len = strlen(strings[i]);
        if (len > box_width)
        {
            box_width = len + 1;
        }
    }

    mvaddstr(1, 2, algorithm_str);
    mvaddstr(2, 2, iteration_str);
    // mvaddstr(3, 2, max_num_str);
    // mvaddstr(4, 2, frame_ms_str);
    cli_draw_box(box_width, 2, 1, 0, true);
}


static void draw_iteration(struct list* list, const enum visual_sort_type sort_type)
{
    uint64_t screen_height = cli_get_scrh();
    uint64_t screen_width = cli_get_scrw();
    uint64_t start_x = screen_width / 2 - list->count / 2;
    uint64_t start_y = screen_height - 2;

    erase();

    for (uint64_t i = 0; i < list->count; i++)
    {
        int64_t data = (int64_t)list_get(list, i);
        int64_t scaled_height = (double)data / max_num * (2 * start_y);
        for (int64_t j = 0; j < scaled_height / 2; j++)
        {
            mvaddstr(start_y - j, start_x + i, "▌");
        }
        if (scaled_height % 2 || scaled_height == 0)
        {
            mvaddstr(start_y - scaled_height / 2, start_x + i, "▖");
        }
    }
    if (show_ui)
    {
        draw_ui(sort_type);
    }

    refresh();
}

static void do_iteration(struct list* list, const enum visual_sort_type sort_type)
{
    draw_iteration(list, sort_type);
    handle_input();
}

static struct list* visual_sort_bubble(struct list* list)
{
    for (uint64_t i = 0; i < list->count - 1; i++)
    {
        bool is_sorted = true;
        for (uint64_t j = 0; j < list->count - i - 1; j++)
        {
            if ((int64_t)list_get(list, j) > (int64_t)list_get(list, j + 1))
            {
                is_sorted = false;
                list_swap(list, j, j + 1);
                do_iteration(list, VISUAL_SORT_BUBBLE);
                if (should_quit)
                {
                    return list;
                }
            }
        }
        if (is_sorted)
        {
            break;
        }
    }
    return list;
}

static struct list* visual_sort_selection(struct list* list)
{
    for (uint64_t i = 0; i < list->count; i++)
    {
        int64_t min = (int64_t)INT_MAX;
        uint64_t min_index = 0;

        for (uint64_t j = i; j < list->count; j++)
        {
            int64_t cur_data = (int64_t)list_get(list, j);
            if (cur_data < min)
            {
                min = cur_data;
                min_index = j;
            }
        }
        list_swap(list, i, min_index);
        do_iteration(list, VISUAL_SORT_SELECTION);
        if (should_quit)
        {
            return list;
        }
    }
    return list;
}

static struct list* visual_sort_insertion(struct list* list)
{
    for (int64_t i = 1; i < list->count; i++)
    {
        for (uint64_t j = i; j > 0; j--)
        {
            if ((int64_t)list_get(list, j - 1) > (int64_t)list_get(list, j))
            {
                list_swap(list, j - 1, j);
                do_iteration(list, VISUAL_SORT_INSERTION);
                if (should_quit)
                {
                    return list;
                }
            }
        }
    }
    return list;
}

static struct list* visual_sort_heap(struct list* list)
{
    for (int64_t i = 1; i < list->count; i++)
    {
        uint64_t index = i;
        uint64_t parent_index = (index - 1) / 2;

        while (index > 0 && (int64_t)list_get(list, index) > (int64_t)list_get(list, parent_index))
        {
            list_swap(list, index, parent_index);
            do_iteration(list, VISUAL_SORT_HEAP);
            if (should_quit)
            {
                return list;
            }
            index = parent_index;
            parent_index = (index - 1) / 2;
        }
    }
    for (uint64_t i = list->count - 1; i > 0; i--)
    {
        list_swap(list, 0, i);
        do_iteration(list, VISUAL_SORT_HEAP);
        if (should_quit)
        {
            return list;
        }
        uint64_t index = 0;

        while (index * 2 + 1 < i)
        {
            uint64_t left_index = index * 2 + 1;
            uint64_t right_index = index * 2 + 2;
            int64_t left_data = (int64_t)list_get(list, left_index);
            int64_t right_data = (int64_t)list_get(list, right_index);
            
            uint64_t candidate_index = right_index >= i || left_data >= right_data ? left_index : right_index;
            if ((int64_t)list_get(list, index) < (int64_t)list_get(list, candidate_index))
            {
                list_swap(list, index, candidate_index);
                do_iteration(list, VISUAL_SORT_HEAP);
                if (should_quit)
                {
                    return list;
                }
                index = candidate_index;
            }
            else
            {
                break;
            }
        }
    }
    return list; 
}

static int64_t quick_sort_partition(struct list* list, const int64_t low_index, const int64_t high_index)
{
    int64_t pivot_index = low_index + rand() % (high_index - low_index);
    int64_t pivot_data = (int64_t)list_get(list, pivot_index);
    list_swap(list, pivot_index, high_index);
    do_iteration(list, VISUAL_SORT_QUICK);
    if (should_quit)
    {
        return 0;
    }

    int64_t left_index = low_index;
    int64_t right_index = high_index - 1;

    while (left_index <= right_index)
    {
        while (left_index <= right_index && (int64_t)list_get(list, left_index) <= pivot_data)
        {
            left_index++;
        }
        while (right_index >= left_index && (int64_t)list_get(list, right_index) >= pivot_data)
        {
            right_index--;
        }
        if (left_index < right_index)
        {
            list_swap(list, left_index, right_index);
            do_iteration(list, VISUAL_SORT_QUICK);
            if (should_quit)
            {
                return 0;
            }
        }
    }

    list_swap(list, left_index, high_index);
    do_iteration(list, VISUAL_SORT_QUICK);
    if (should_quit)
    {
        return 0;
    }
    return left_index;
}

static struct list* quick_sort_recursive(struct list* list, const int64_t low_index, const int64_t high_index)
{
    if (low_index >= high_index)
    {
        return list;
    }
    int64_t pivot_index = quick_sort_partition(list, low_index, high_index);
    if (should_quit)
    {
        return list;
    }

    quick_sort_recursive(list, low_index, pivot_index - 1);
    quick_sort_recursive(list, pivot_index + 1, high_index);

    return list;
}

static struct list* visual_sort_quick(struct list* list)
{
    return quick_sort_recursive(list, 0, list->count - 1);
}

static struct list* bucket_sort_by_digit(struct list* list, const uint64_t exponent)
{
    struct list* buckets = list_create(32);
    for (uint8_t i = 0; i < 20; i++)
    {
        list_append(buckets, list_create(32));
    }

    for (uint64_t i = 0; i < list->count; i++)
    {
        int64_t number = (int64_t)list_get(list, i);
        int16_t sign = number < 0 ? -1 : 1;
        int64_t number_digit = (((sign * number) / exponent) % 10) * sign;
        list_append(list_get(buckets, 10 + number_digit), (void*)number);
    }

    uint64_t index = 0;
    for (uint8_t i = 0; i < 20; i++)
    {
        struct list* bucket = list_get(buckets, i);
        for (uint64_t j = 0; j < bucket->count; j++)
        {
            list_replace(list, list_get(bucket, j), index);
            do_iteration(list, VISUAL_SORT_RADIX);
            if (should_quit)
            {
                return list;
            }
            index++;
        }
        free(bucket);
    }
    free(buckets);

    return list;
}

static struct list* visual_sort_radix(struct list* list)
{
    uint16_t max_digit_count = 1;
    int64_t list_max = (int64_t)list_get_max_int(list);

    for (uint64_t i = 1; list_max / i > 0; i *= 10)
    {
        bucket_sort_by_digit(list, i);
    }

    return list;
}

static void animate(const uint64_t list_size, const uint64_t max_num_size, const uint64_t frame_time_ms, const enum visual_sort_type sort_type)
{
    iteration = 0;
    max_num = max_num_size;
    frame_ms = frame_time_ms;

    struct list* list = list_create(list_size);
    for (uint64_t i = 0; i < list_size; i++)
    {
        list_append(list, (void*)(rand() % max_num_size));
    }
    switch (sort_type)
    {
        case VISUAL_SORT_BUBBLE:
            visual_sort_bubble(list);
            break;
        case VISUAL_SORT_SELECTION:
            visual_sort_selection(list);
            break;
        case VISUAL_SORT_INSERTION:
            visual_sort_insertion(list);
            break;
        case VISUAL_SORT_HEAP:
            visual_sort_heap(list);
            break;
        case VISUAL_SORT_QUICK:
            visual_sort_quick(list);
            break;
        case VISUAL_SORT_RADIX:
            visual_sort_radix(list);
            break;
    }
    if (list_is_sorted_int(list))
    {
        usleep(3000000);
    }
    list_destroy(list);
}

void visual_sort_main_menu()
{
    struct cli_menu* main_menu = cli_menu_create("-= SELECT ALGORITHM =-", ":: ", " ::", 4, 1, true);
    cli_menu_add_button(main_menu, "Screensaver");
    cli_menu_add_button(main_menu, "Bubble");
    cli_menu_add_button(main_menu, "Selection");
    cli_menu_add_button(main_menu, "Insertion");
    cli_menu_add_button(main_menu, "Heap");
    cli_menu_add_button(main_menu, "Quick");
    cli_menu_add_button(main_menu, "Radix");
    cli_menu_add_button(main_menu, "Quit");

    while (true)
    {
        should_quit = false;
        frame_ms = 10;
        set_ms_mult(1);

        int selection = cli_menu_run(main_menu, (uint8_t[]){3, 1, 1, 1, 1, 1, 1, 2}, 0, true);
        if (selection == 7)
        {
            free(main_menu);
            return;
        }
        if (selection == 0)
        {
            enum visual_sort_type type = rand() % 6;
            animate(cli_get_scrw() - 4, 10000, visual_sort_type_ms[type], type);
            if (!should_quit)
            {
                continue;
            }
        }

        set_ms_mult(10);
        animate(80, 10000, visual_sort_type_ms[selection - 1], selection - 1);
        main_menu->has_selected = false;
    }

    free(main_menu);
}
