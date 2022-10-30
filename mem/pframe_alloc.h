#pragma once

#include <stddef.h>

void init_pframe_alloc(void);
size_t allocate_frame(void);
void free_frame(size_t addr);
size_t allocate_frames(int count);
int check_free(int frame_idx, int count);
