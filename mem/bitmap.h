#pragma once

#include <stdbool.h>
#include <stdint.h>

#define BITMAP_BYTE(index) ((index) / 32)
#define BITMAP_OFFSET(index) ((index) % 32)
#define BITMAP_SIZE(length) (((length) + 31) / 32)

bool bitmap_check(const uint32_t *bitmap, int index);
void bitmap_set(uint32_t *bitmap, int index, bool state);
void bitmap_bulk_set(uint32_t *bitmap, int index, int count, bool state);
int bitmap_find(const uint32_t *bitmap, int length, bool state);
int bitmap_find_multiple(const uint32_t *bitmap, int length, bool state, int count);
