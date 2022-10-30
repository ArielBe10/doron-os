#include "array.h"
#include <stdbool.h>
#include "string.h"

#define CALL_COMP(comp, elem1, elem2, elem_size) \
    ((comp) != NULL ? comp((elem1), (elem2)) : memcmp((elem1), (elem2), (elem_size), true))

static void bubble_sort(void *arr, size_t elem_size, size_t arr_length, compare_t comp);

void sort(void *arr, size_t elem_size, size_t arr_length, compare_t comp)
{
    return bubble_sort(arr, elem_size, arr_length, comp);
}

void bubble_sort(void *arr, size_t elem_size, size_t arr_length, compare_t comp)
{
    char *_arr = arr;
    for (size_t i = 0; i < arr_length - 1; i++)
    {
        for (size_t j = 0; j < arr_length - i - 1; j++)
        {
            if (CALL_COMP(comp, &_arr[elem_size * j], &_arr[elem_size * (j + 1)], elem_size) > 0)
            {
                memswap(&_arr[elem_size * j], &_arr[elem_size * (j + 1)], elem_size);
            }
        }
    }
}
