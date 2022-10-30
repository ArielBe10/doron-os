#pragma once

#include <stddef.h>
#include <stdint.h>

#define LOG_BITMAP_LINE 0x40

extern uint32_t useable_frames_bitmap[];

void log_frames_bitmap(const uint32_t *bitmap, size_t start_frame, int count);
void init_pframe(void);
