
#include "../lib/logging.h"
#include "../lib/terminal.h"
#include "../lib/shell.h"
#include "../hardware/isr.h"
#include "../mem/bitmap.h"
#include "../mem/kheap.h"
#include "../mem/pframe_alloc.h"
#include "../mem/paging.h"

#define BITMAP_LENGTH 50

void kernel_main()
{
    init_logging(DEBUG_LEVEL);
    init_idt();
    init_kheap();

    init_paging();
}
