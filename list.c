#include "list.h"


static bool list_realloc(struct list* list, uint64_t size)
{
    void** tmp = realloc(list->data, size); 
    if (!tmp)
    {
        fprintf(stderr, "list_realloc: realloc failed\n");
        free(list->data);
        free(list);
        return false;
    }
    list->data = tmp;
    return true;
}

struct list* list_create(const uint64_t size)
{
    struct list* list = calloc(1, sizeof(struct list));
    if (!list)
    {
        fprintf(stderr, "list_create: list malloc failed\n");
        return NULL;
    }

    list->size = size;
    list->data_size = sizeof(void*);
    list->count = 0;
    list->data = calloc((1 + size), list->data_size);

    if (!list->data) 
    {
        fprintf(stderr, "list_create: data malloc failed\n");
        free(list);
        return NULL;
    }

    return list;
}

struct list* list_create_copy(const struct list* list)
{
    struct list* list_copy = list_create(32);
    for (uint64_t i = 0; i < list->count; i++)
    {
        list_append(list_copy, list_get(list, i));
    }
    return list_copy;
}

void list_destroy(struct list* list)
{
    free(list->data);
    free(list);
}

void list_append(struct list* list, void* data)
{
    if (list->count >= list->size)
    {
        if (!list_realloc(list, list->data_size + (list->size * 2) * list->data_size))
        {
            return; 
        }
        list->size *= 2;
    }
    list->data[1 + list->count] = data;
    list->count++;
}

void list_insert(struct list* list, void* data, const uint64_t index)
{
    if (index >= list->count)
    {
        fprintf(stderr, "list_index: index out of bounds\n");
        return;
    }
 
    if (list->count >= list->size)
    {
        if (!list_realloc(list, list->data_size + (list->size * 2) * list->data_size))
        {
            return; 
        }
        list->size *= 2;
    }

    for (uint64_t i = 1 + list->count; i > 1 + index; i--)
    {
        list->data[i] = list->data[i - 1];    
    }
    list->data[1 + index] = data;
    list->count++;
}

void list_replace(struct list* list, void* data, const uint64_t index)
{
    if (index >= list->count)
    {
        fprintf(stderr, "list_replace: index out of bounds\n");
        return;
    }

    list->data[1 + index] = data;
}


void* list_remove_at(struct list* list, const uint64_t index)
{
    if (index >= list->count)
    {
        fprintf(stderr, "list_remove: index out of bounds\n");
        return NULL;
    }
    void* data_buf = list->data[1 + index];

    for (uint64_t i = index + 2; i <= list->count; i++)
    {
        list->data[(i - 1)] = list->data[i];
    }

    list->data[list->count] = NULL;
    list->count--;

    return data_buf;
}

void* list_remove(struct list* list, void* data)
{
    for (uint64_t i = 0; i < list->count; i++)
    {
        if (list_get(list, i) == data)
        {
            return list_remove_at(list, i);
        }
    }
    fprintf(stderr, "list_remove: data not in list.\n");
    return NULL;
}

void list_clear(struct list* list)
{
    list->count = 0;
}


struct list* list_reverse(struct list* list)
{
    uint64_t left_index = 0;
    uint64_t right_index = list->count - 1;

    while (left_index < right_index)
    {
        void* left_data = list_get(list, left_index);
        void* right_data = list_get(list, right_index);
        list_replace(list, left_data, right_index);
        list_replace(list, right_data, left_index);
        left_index++;
        right_index--;
    }
    return list;
}

struct list *list_swap(struct list* list, const uint64_t index_a, const uint64_t index_b)
{
    if (index_a < 0 || index_a >= list->count || index_b < 0 || index_b >= list->count)
    {
        fprintf(stderr, "list_swap: index out of bounds: a: %llu, b: %llu\n", index_a, index_b);
        return NULL;
    }
    void* data_buf = list->data[1 + index_a];
    list->data[1 + index_a] = list->data[1 + index_b];
    list->data[1 + index_b] = data_buf;
    return list;
}


struct list* list_get_sublist(const struct list* list, const uint64_t from_index, const uint64_t to_index)
{
    struct list* sublist = list_create(32);
    for (uint64_t i = from_index; i < to_index; i++)
    {
        list_append(sublist, list_get(list, i));
    }
    return sublist;
}


void* list_get(const struct list* list, const uint64_t index)
{
    if (index >= list->count)
    {
        fprintf(stderr, "list_get: index out of bounds: %llu\n", index);
        return NULL;
    }
    return list->data[1 + index];
}

bool list_contains(const struct list* list, const void* data)
{
    for (uint64_t i = 0; i < list->count; i++)
    {
        if (list->data[1 + i] == data)
        {
            return true;
        }
    }
    return false;
}

int64_t list_get_max_int(const struct list* list)
{
    if (list->count == 0)
    {
        fprintf(stderr, "list_get_max: list is empty\n");
        return 0;
    }
    int64_t max = (int64_t)list->data[1];
    for (uint64_t i = 1; i <= list->count; i++)
    {
        if ((int64_t)list->data[i] > max)
        {
            max = (int64_t)list->data[i];
        }
    }
    return max;
}

int64_t list_get_min_int(const struct list* list)
{
    if (list->count == 0)
    {
        fprintf(stderr, "list_get_min: list is empty\n");
        return 0;
    }
    int64_t min = (int64_t)list->data[1];
    for (uint64_t i = 1; i <= list->count; i++)
    {
        if ((int64_t)list->data[i] < min)
        {
            min = (int64_t)list->data[i];
        }
    }
    return min;
}

bool list_is_sorted_int(struct list* list)
{
    for (uint64_t i = 0; i < list->count - 1; i++)
    {
        if ((int64_t)list_get(list, i) > (int64_t)list_get(list, i + 1))
        {
            return false;
        }
    }
    return true;
}


void list_print_int(const struct list* list)
{
    printf("(");
    for (uint64_t i = 0; i < list->count; i++)
    {
        printf(i == list->count - 1 ? "%lld" : "%lld, ", (int64_t)list_get(list, i));
    }
    printf(")");
}

void list_print_char(const struct list* list)
{
    printf("(");
    for (uint64_t i = 0; i < list->count; i++)
    {
        printf(i == list->count - 1 ? "%c" : "%c, ", (char)(int64_t)list_get(list, i));
    }
    printf(")");
}
