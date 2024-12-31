#ifndef VISUAL_SORT_H
#define VISUAL_SORT_H

#include "list.h"
#include "cli.h"

#include <locale.h>
#include <stdint.h>
#include <time.h>

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

void visual_sort_animate(const uint64_t list_size, const uint64_t max_num_size, const uint64_t frame_time_ms, const enum visual_sort_type sort_type);

#endif
