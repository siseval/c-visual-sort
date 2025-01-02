#ifndef VISUAL_SORT_H
#define VISUAL_SORT_H

#include "list.h"
#include "cli.h"

#include <locale.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

enum visual_sort_type
{
    VISUAL_SORT_BUBBLE,
    VISUAL_SORT_SELECTION,
    VISUAL_SORT_INSERTION,
    VISUAL_SORT_HEAP,
    VISUAL_SORT_QUICK,
    VISUAL_SORT_RADIX
};

const static char* visual_sort_type_str[] = 
{
    "bubble",
    "selection",
    "insertion",
    "heap",
    "quick",
    "radix"
};

const static uint16_t visual_sort_type_ms[] = 
{
    5,
    30,
    5,
    15,
    15,
    15
};

void visual_sort_main_menu();

#endif
