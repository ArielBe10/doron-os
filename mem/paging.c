#include "utils.h"
#include "paging.h"
#include "pframe_alloc.h"
#include "kheap.h"
#include "../lib/string.h"
#include "../lib/logging.h"

#include <stdint.h>

struct _page_table_entry_t
{
    uint8_t present : 1;
    uint8_t writable : 1;
    uint8_t usermode : 1;
    uint8_t write_through : 1;
    uint8_t disable_cache : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t page_attribute_table : 1;
    uint8_t global : 1;
    uint8_t available : 3;
    uint32_t frame_addr : 20;
};

struct _page_table_t
{
    page_table_entry_t entries[1024];
};

struct _page_directory_entry_t
{
    uint8_t present : 1;
    uint8_t writable : 1;
    uint8_t usermode : 1;
    uint8_t write_through : 1;
    uint8_t disable_cache : 1;
    uint8_t accessed : 1;
    uint8_t available1 : 1;
    uint8_t page_size : 1;
    uint8_t available2 : 4;
    uint32_t page_table_addr : 20;
};

struct _page_directory_t
{
    page_directory_entry_t entries[1024];
    page_table_t *page_tables[1024];
};

static page_directory_t *current_directory;

void allocate_frame_to_page(page_table_entry_t *page, int is_kernel, int is_writeable)
{
    size_t frame_addr = allocate_frame();
    page->present = 1;
    page->writable = is_writeable ? 1 : 0;
    page->accessed = 0;
    page->usermode = is_kernel ? 0 : 1;
    page->frame_addr = frame_addr / PAGE_SIZE;
}

page_table_entry_t *get_page(size_t address, int create, page_directory_t *dir)
{
    size_t page_idx = address / PAGE_SIZE;
    size_t table_idx = page_idx / 1024;
    if (dir->entries[table_idx].present) // if table is already assigned
    {
        return &dir->page_tables[table_idx]->entries[page_idx];
    }
    else if (create)
    {
        dir->page_tables[table_idx] = kmalloc_aligned(sizeof(page_table_t), PAGE_SIZE);
        memset(dir->page_tables[table_idx], 0, sizeof(page_table_t));

        dir->entries[table_idx].present = 1;
        dir->entries[table_idx].usermode = 1;
        dir->entries[table_idx].writable = 1;
        dir->entries[table_idx].page_table_addr = ((size_t)dir->page_tables[table_idx]) / PAGE_SIZE;

        DEBUG("created new page table of index 0x%x for page 0x%x of address %p",
              table_idx, page_idx, address);
        return &dir->page_tables[table_idx]->entries[page_idx];
    }
    else
    {
        INFO("page table not created");
        return 0;
    }
}

// identity mapped
void init_paging(void)
{
    INFO("setting up paging");
    page_directory_t *dir = kmalloc_aligned(sizeof(page_directory_t), PAGE_SIZE);
    memset(dir, 0, sizeof(page_directory_t));
    DEBUG("created identity page directory in address: %p", dir);

    for (int page_idx = 0; page_idx < 1024 * 2; page_idx++)
    {
        size_t address = page_idx * PAGE_SIZE;
        page_table_entry_t *page = get_page(address, 1, dir);
        memset(page, 0, sizeof(page_table_entry_t));
        page->present = 1;
        page->frame_addr = page_idx;
    }

    switch_page_directory(dir); // enables paging
    INFO("finished setting up paging");
}

void switch_page_directory(page_directory_t *dir)
{
    INFO("switching to new page directory: %p", dir);
    current_directory = dir;
    asm volatile("mov %0, %%cr3" ::"r"(&dir->page_tables));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0"
                 : "=r"(cr0));
    cr0 |= 0x80000000; // enable paging
    asm volatile("mov %0, %%cr0" ::"r"(cr0));
    INFO("finished switching page directory");
}
