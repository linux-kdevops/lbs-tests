#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define DIV_ROUND_UP(n, d) __KERNEL_DIV_ROUND_UP(n, d)

#define WARN_ON(x) 							\
	do { 								\
		if ((x))						\
			printf("WARN: %s : %d\n", __FILE__, __LINE__);	\
	} while (0)

static bool repeat_idx_round_down = false;
static bool repeat_next_round_up = false;

static void usage(const char *cmd)
{
	printf("Usage:\n");
	printf("%s\n", cmd);
	printf("--help                           this help menu\n");
	printf("--repeat-idx-round-down          repeats round_down() on idx\n");
	printf("--repeat-next-round-up           repeats round_up() on next\n");
	exit(1);
}

void check_arg(const char *cmd, char *arg)
{
	if (strcmp(arg, "--repeat-idx-round-down") == 0) {
		repeat_idx_round_down = true;
		return;
	}
	if (strcmp(arg, "--repeat-next-round-up") == 0) {
		repeat_next_round_up = true;
		return;
	}
	usage(cmd);
}

int main(int argc, char *argv[])
{
	unsigned int i;
	unsigned int nrpages = 4;
	const char *cmd_argv = argv[0];

	while (--argc > 0)
		check_arg(cmd_argv, argv[argc]);

	for (i=0; i<50; i++) {
		unsigned int idx = round_down(i, nrpages);
		unsigned int idx_next = round_up(i, nrpages);
		unsigned int div_up = DIV_ROUND_UP(i, nrpages);

		if (repeat_idx_round_down)
			idx = round_down(idx, nrpages);
		if (repeat_next_round_up)
			idx_next = round_up(idx_next, nrpages);

		/* this verifies alignment always works */
		WARN_ON(idx & (nrpages - 1));
		WARN_ON(idx_next & (nrpages - 1));

		printf("i: %10u   idx: %10u   next: %10u   div_up: %10u\n", i, idx, idx_next, div_up);
	}

	return 0;
}
