#pragma once

#include <stddef.h>

typedef struct _page_table_entry_t page_table_entry_t;
typedef struct _page_table_t page_table_t;
typedef struct _page_directory_entry_t page_directory_entry_t;
typedef struct _page_directory_t page_directory_t;

void allocate_frame_to_page(page_table_entry_t *page, int is_kernel, int is_writeable);
page_table_entry_t *get_page(size_t address, int create, page_directory_t *dir);
void init_paging(void);
void switch_page_directory(page_directory_t *dir);
