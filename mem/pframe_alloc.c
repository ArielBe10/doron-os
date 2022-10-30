#include <stdint.h>
#include "pframe.h"
#include "pframe_alloc.h"
#include "utils.h"
#include "bitmap.h"
#include "../lib/logging.h"
#include "../lib/string.h"

#define RESERVED_FRAMES 0x100

static uint32_t free_frames_bitmap[BITMAP_SIZE(PAGE_COUNT)] = {0};

void init_pframe_alloc(void)
{
    init_pframe();
    memcpy(free_frames_bitmap, useable_frames_bitmap, BITMAP_SIZE(PAGE_COUNT));
    DEBUG("useable copied to free");
    bitmap_bulk_set(free_frames_bitmap, 0, RESERVED_FRAMES, 0);
    INFO("first 0x%x frames are reserved for system", RESERVED_FRAMES);
}

size_t allocate_frame(void)
{
    int frame = bitmap_find(free_frames_bitmap, BITMAP_SIZE(PAGE_COUNT), 1);
    DEBUG("allocated frame: %p", frame);
    bitmap_set(free_frames_bitmap, frame, 0);
    log_frames_bitmap(free_frames_bitmap, ALIGNED_DOWN(frame, LOG_BITMAP_LINE), LOG_BITMAP_LINE);
    return frame * PAGE_SIZE;
}

size_t allocate_frames(int count)
{
    int frame = bitmap_find_multiple(free_frames_bitmap, BITMAP_SIZE(PAGE_COUNT), 1, count);
    DEBUG("allocated %d frames on address: %p", count, frame);
    bitmap_bulk_set(free_frames_bitmap, frame, count, 0);
    log_frames_bitmap(free_frames_bitmap, ALIGNED_DOWN(frame, LOG_BITMAP_LINE), LOG_BITMAP_LINE);
    return frame * PAGE_SIZE;
}

int check_free(int frame_idx, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (!bitmap_check(free_frames_bitmap, frame_idx + i))
            return 0;
    }
    return 1;
}

void free_frame(size_t frame_addr)
{
    int frame = frame_addr / PAGE_SIZE;
    if (bitmap_check(useable_frames_bitmap, frame) == 0)
        ERROR("attemp to free unuseable frame: %p", frame);
    else if (bitmap_check(free_frames_bitmap, frame) != 0)
        ERROR("attemp to double free frame: %p", frame);
    else
    {
        DEBUG("freed frame: %p", frame);
        bitmap_set(free_frames_bitmap, frame, 1);
        log_frames_bitmap(free_frames_bitmap, ALIGNED_DOWN(frame, LOG_BITMAP_LINE), LOG_BITMAP_LINE);
    }
}