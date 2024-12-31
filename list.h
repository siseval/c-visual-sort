#ifndef LIST_H
#define LIST_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define list(T) list_create(32)

struct list
{
    uint64_t size;
    uint64_t data_size;
    uint64_t count;
    void **data;
};

struct list* list_create(const uint64_t size);
struct list* list_create_copy(const struct list* list);
void list_destroy(struct list* list);

void list_append(struct list* list, void* data);
void list_insert(struct list* list, void* data, const uint64_t index);
void list_replace(struct list* list, void* data, const uint64_t index);

void* list_remove_at(struct list* list, const uint64_t index);
void* list_remove(struct list* list, void* data);
void list_clear(struct list* list);

struct list* list_reverse(struct list* list);
struct list* list_swap(struct list* list, const uint64_t index_a, const uint64_t index_b);

struct list* list_get_sublist(const struct list* list, const uint64_t from_index, const uint64_t to_index);

void* list_get(const struct list* list, const uint64_t index);
bool list_contains(const struct list* list, const void* data);

int64_t list_get_max_int(const struct list* list);
int64_t list_get_min_int(const struct list* list);
bool list_is_sorted_int(struct list* list);

void list_print_int(const struct list* list);
void list_print_char(const struct list* list);

#endif
