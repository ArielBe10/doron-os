#include "kheap.h"
#include "../lib/logging.h"
#include "pframe_alloc.h"

static size_t kheap_end = 0;

void init_kheap()
{
    init_pframe_alloc();
    INFO("init kheap");
}

void *kmalloc_aligned(size_t size, size_t alignment)
{
    kheap_end = ALIGNED_UP(kheap_end, alignment);

    if (kheap_end % PAGE_SIZE != 0)
    {
        if (kheap_end / PAGE_SIZE == (kheap_end + size - 1) / PAGE_SIZE) // if fits in current page
        {
            void *addr = (void *)kheap_end;
            kheap_end += size;
            DEBUG("allocated 0x%x bytes with alignment of 0x%x in existing frame in addresses 0x%p-0x%p",
                  size, alignment, addr, addr + size - 1);
            return addr;
        }
        else
        {
            kheap_end = ALIGNED_UP(kheap_end, PAGE_SIZE); // if bytes don't fit in current page, move to next page
            DEBUG("required bytes don't fit in current page");
        }
    }

    if (kheap_end % PAGE_SIZE == 0) // if at start of new page, allocate the required number of pages
    {
        int page_count = ALIGNED_UP(size, PAGE_SIZE) / PAGE_SIZE;
        kheap_end = allocate_frames(page_count);
        void *addr = (void *)kheap_end;
        kheap_end += size;
        DEBUG("allocated 0x%x bytes with alignment of 0x%x in %d new frames in addresses 0x%p-0x%p",
              size, alignment, page_count, addr, addr + size - 1);
        return addr;
    }
}

void *kmalloc(size_t size)
{
    return kmalloc_aligned(size, 1);
}