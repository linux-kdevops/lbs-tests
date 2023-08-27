#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_UP(n, d) __KERNEL_DIV_ROUND_UP(n, d)

#define WARN_ON(x) 							\
	do { 								\
		if ((x))						\
			printf("WARN: %s:%d\n", __FILE__, __LINE__);	\
	} while (0)

#define PAGE_SIZE 4096
#define PAGE_SHIFT 12
