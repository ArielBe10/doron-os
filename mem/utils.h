#define ALIGNED_DOWN(addr, alignment) (((addr) / (alignment)) * (alignment))
#define ALIGNED_UP(addr, alignment) (ALIGNED_DOWN(addr, alignment) + (((addr) % (alignment) == 0) ? 0 : (alignment)))

#define PAGE_SIZE (0x1000)
#define PAGE_COUNT (1 << 20)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))