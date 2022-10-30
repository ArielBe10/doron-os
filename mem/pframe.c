#include "pframe.h"

#include "../lib/screen.h"
#include "../lib/logging.h"
#include "../lib/print.h"
#include "bitmap.h"
#include "utils.h"

#define MMAP_ADDRESS 0x800

#define MEM_REGION_AVIALABLE 1
#define MEM_REGION_RESERVED 2
#define MEM_REGION_ACPI_RECLAIMABLE 3
#define MEM_REGION_ACPI_NVS 4
#define MEM_REGION_BAD_MEMORY 5

typedef struct
{
    uint32_t base_address_low;
    uint32_t base_address_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t extended_attributes;
} mem_region_t;

uint32_t useable_frames_bitmap[BITMAP_SIZE(PAGE_COUNT)] = {0};

static void log_memory_regions(int regions_count, const mem_region_t *regions_arr);
static void set_useable_frames_bitmap(uint32_t *available_bitmap, int regions_count, const mem_region_t *regions_arr);
static void log_useable_frames(const uint32_t *useable_bitmap, int regions_count, const mem_region_t *regions_arr);

static size_t get_first_frame(mem_region_t region);
static size_t get_last_frame(mem_region_t region);

void log_memory_regions(int regions_count, const mem_region_t *regions_arr)
{
    INFO("memory regions count: %d", regions_count);
    INFO("memory regions array address: %p", regions_arr - 2);
    INFO("memory regions:");
    INFO("base_address     length           type     attributes");

    for (int i = 0; i < regions_count; i++)
    {
        mem_region_t region = regions_arr[i];
        INFO("%p%p %p%p %p %p", region.base_address_high, region.base_address_low, region.length_high,
             region.length_low, region.type, region.extended_attributes);
    }
}

void set_useable_frames_bitmap(uint32_t *bitmap, int regions_count, const mem_region_t *regions_arr)
{
    bitmap_bulk_set(bitmap, 0, PAGE_COUNT, 0);

    for (int region_index = 0; region_index < regions_count; region_index++)
    {
        mem_region_t region = regions_arr[region_index];
        size_t first_frame = get_first_frame(region);
        size_t last_frame = get_last_frame(region);
        if (region.type == MEM_REGION_AVIALABLE)
            bitmap_bulk_set(bitmap, first_frame, last_frame - first_frame + 1, 1);
    }
    for (int region_index = 0; region_index < regions_count; region_index++)
    {
        mem_region_t region = regions_arr[region_index];
        size_t first_frame = get_first_frame(region);
        size_t last_frame = get_last_frame(region);
        if (region.type != MEM_REGION_AVIALABLE)
            bitmap_bulk_set(bitmap, first_frame, last_frame - first_frame + 1, 0);
    }
}

size_t get_first_frame(mem_region_t region)
{
    if (region.type == MEM_REGION_AVIALABLE)
        return ALIGNED_UP(region.base_address_low, PAGE_SIZE) / PAGE_SIZE;
    return ALIGNED_DOWN(region.base_address_low, PAGE_SIZE) / PAGE_SIZE;
}

size_t get_last_frame(mem_region_t region)
{
    size_t next_first_frame;
    if (region.type == MEM_REGION_AVIALABLE)
        next_first_frame = ALIGNED_DOWN(region.base_address_low + region.length_low, PAGE_SIZE) / PAGE_SIZE;
    else
        next_first_frame = ALIGNED_UP(region.base_address_low + region.length_low, PAGE_SIZE) / PAGE_SIZE;
    if (next_first_frame == 0)
        return 0x000fffff;
    return next_first_frame - 1;
}

void log_useable_frames(const uint32_t *useable_bitmap, int regions_count, const mem_region_t *regions_arr)
{
    INFO("useable bitmap address: %p", useable_bitmap);
    INFO("useable bitmap size: %p", BITMAP_SIZE(PAGE_COUNT));
    INFO("useable frames:");
    INFO("regions_arr: %p", regions_arr);
    for (int i = 0; i < regions_count; i++)
    {
        mem_region_t region = regions_arr[i];
        size_t first_frame = get_first_frame(region);
        size_t last_frame = get_last_frame(region);

        // frame_start = ALIGNED_DOWN(frame_start - LOG_BITMAP_LINE, LOG_BITMAP_LINE);
        // frame_end = ALIGNED_UP(frame_end - LOG_BITMAP_LINE, LOG_BITMAP_LINE);
        INFO("region %d: %s", i, region.type == MEM_REGION_AVIALABLE ? "useable" : "unuseable");
        INFO("first frame: %p, last frame: %p", first_frame, last_frame);
        log_frames_bitmap(useable_frames_bitmap, first_frame, LOG_BITMAP_LINE * 3);
        DEBUG("...");
        log_frames_bitmap(useable_frames_bitmap, last_frame, LOG_BITMAP_LINE * 3);
        DEBUG("...");
    }
}

void log_frames_bitmap(const uint32_t *bitmap, size_t start_frame, int count)
{
    if (start_frame + count >= PAGE_COUNT) // overflow
        start_frame = PAGE_COUNT - count;

    size_t line_first_frame = start_frame;
    char line[2 * LOG_BITMAP_LINE] = {0};
    int line_index = 0;

    for (int frame_index = 0; frame_index < count; frame_index++)
    {
        line[line_index] = ((bitmap_check(bitmap, start_frame + frame_index) == 0) ? '#' : '.');
        line_index++;
        if (frame_index % 0x10 == 0xf)
        {
            line[line_index] = ' ';
            line_index++;
        }
        if (frame_index % LOG_BITMAP_LINE == LOG_BITMAP_LINE - 1)
        {
            line[line_index] = '\0';
            DEBUG("%p:%s", line_first_frame, line);
            line_first_frame += LOG_BITMAP_LINE;
            line_index = 0;
        }
    }
    if (count % LOG_BITMAP_LINE != 0)
    {
        line[line_index] = '\0';
        DEBUG("%p:%s", line_first_frame, line);
    }
}

void init_pframe(void)
{
    const int regions_count = *(uint16_t *)(MMAP_ADDRESS);
    const mem_region_t *const regions_arr = (mem_region_t *)(MMAP_ADDRESS + 2);

    INFO("");
    INFO("init pframe");
    log_memory_regions(regions_count, regions_arr);
    INFO("");

    set_useable_frames_bitmap(useable_frames_bitmap, regions_count, regions_arr);
    log_useable_frames(useable_frames_bitmap, regions_count, regions_arr);
    INFO("");
}
