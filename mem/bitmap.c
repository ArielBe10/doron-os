#include "bitmap.h"

bool bitmap_check(const uint32_t *bitmap, int index)
{
    uint32_t byte = bitmap[BITMAP_BYTE(index)];
    return (byte & (1 << BITMAP_OFFSET(index))) == 0 ? 0 : 1;
}

void bitmap_set(uint32_t *bitmap, int index, bool state)
{
    if (state == 0)
        bitmap[BITMAP_BYTE(index)] &= ~(1 << BITMAP_OFFSET(index));
    else
        bitmap[BITMAP_BYTE(index)] |= (1 << BITMAP_OFFSET(index));
}

void bitmap_bulk_set(uint32_t *bitmap, int index, int count, bool state)
{
    for (int i = 0; i < count; i++)
        bitmap_set(bitmap, index + i, state);
}

int bitmap_find(const uint32_t *bitmap, int length, bool state)
{
    uint32_t skip;
    if (state)
        skip = 0;
    else
        skip = 0xffffffff;

    for (int byte = 0; byte < BITMAP_SIZE(length); byte++)
    {
        if (bitmap[byte] != skip)
        {
            for (int offset = 0; offset < 32; offset++)
            {
                int index = 32 * byte + offset;
                if (bitmap_check(bitmap, index) == state)
                    return index;
            }
        }
    }
}

int bitmap_find_multiple(const uint32_t *bitmap, int length, bool state, int count)
{
    uint32_t skip;
    if (state)
        skip = 0;
    else
        skip = 0xffffffff;

    for (int byte = 0; byte < BITMAP_SIZE(length); byte++)
    {
        if (bitmap[byte] != skip)
        {
            for (int offset = 0; offset < 32; offset++)
            {
                int index = 32 * byte + offset;

                int used = 0;
                for (int index_offset = 0; index_offset < count; index_offset++)
                {
                    if (bitmap_check(bitmap, index + index_offset) != state)
                    {
                        used = 1;
                        break;
                    }
                }
                if (!used)
                    return index;
            }
        }
    }
}
