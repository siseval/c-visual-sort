#ifndef VISUAL_SORT_H
#define VISUAL_SORT_H

#include <stdint.h>
#include <unistd.h>

enum visual_sort_type
{
    VISUAL_SORT_BUBBLE,
    VISUAL_SORT_SELECTION,
    VISUAL_SORT_INSERTION,
    VISUAL_SORT_HEAP,
    VISUAL_SORT_MERGE,
    VISUAL_SORT_QUICK,
    VISUAL_SORT_RADIX
};

const static char* visual_sort_type_str[] = 
{
    "bubble",
    "selection",
    "insertion",
    "heap",
    "merge",
    "quick",
    "radix"
};

const static uint16_t visual_sort_type_ms[] = 
{
    5,
    30,
    5,
    30,
    30,
    30,
    30
};

void visual_sort_main_menu();

#endif
