#pragma once

#include <stddef.h>

typedef char (*compare_t)(const void *elem1, const void *elem2);

void sort(void *arr, size_t elem_size, size_t arr_length, compare_t comp);
