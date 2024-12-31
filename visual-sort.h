#ifndef VISUAL_SORT_H
#define VISUAL_SORT_H

#include "list.h"
#include <stdint.h>

struct list* visual_sort_bubble(struct list* list);
struct list* visual_sort_selection(struct list* list);
struct list* visual_sort_insertion(struct list* list);
struct list* visual_sort_heap(struct list* list);
struct list* visual_sort_merge(struct list* list);
struct list* visual_sort_quick(struct list* list);
struct list* visual_sort_radix(struct list* list);


#endif
