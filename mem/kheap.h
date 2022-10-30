#pragma once

#include <stddef.h>
#include "utils.h"

void init_kheap();
void *kmalloc_aligned(size_t size, size_t alignment);
void *kmalloc(size_t size);