#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "kernel.h"
#include "__ffs.h"

static unsigned int order = 2;
static unsigned int offset = 0;
static unsigned int count = 50;

static void usage(const char *cmd)
{
	printf("Usage:\n");
	printf("%s\n", cmd);
	printf("--help                           this help menu\n");
	printf("--order                          page order, default is 2 for 16k if on x86\n");
	printf("--offset                         initial count offset, default is 0\n");
	printf("--count                          how many times to loop, default is 50\n");
	exit(1);
}

void check_arg(const char *cmd, char *argv[], int *argc)
{
	if (strcmp(argv[*argc], "--order") == 0) {
		*argc = (*argc) + 1;
		if (*argc <= 1)
			usage(cmd);
		order = atoi(argv[*argc]);
		return;
	}
	if (strcmp(argv[*argc], "--offset") == 0) {
		*argc = (*argc) + 1;
		if (*argc <= 1)
			usage(cmd);
		offset = atoi(argv[*argc]);
		return;
	}
	if (strcmp(argv[*argc], "--count") == 0) {
		*argc = (*argc) + 1;
		if (*argc <= 1)
			usage(cmd);
		count = atoi(argv[*argc]);
		return;
	}
	usage(cmd);
}

static void kernel_ffs(void)
{
	unsigned int pos;
	unsigned int start = offset;
	unsigned nrpages = 1 << order;
	bool unaligned;

	for (pos = start; pos < start + count; pos++) {
		unsigned int this_order = order;
		unsigned int index = pos >> PAGE_SHIFT;

		unaligned = false;

		if (index & (nrpages-1)) {
			unaligned = true;
			this_order = __ffs(index);
		}

		printf("%10s pos: %u    index: %u  ffs(%u) : %u\n",
		       unaligned ? "unaligned" : "",
		       pos,          index, index, __ffs(index));
	}
}

int main(int argc, char *argv[])
{
	unsigned int i;
	unsigned int nrpages;
	const char *cmd_argv = argv[0];
	bool last_idx_set = false;

	for (i=1; i < argc; i++)
		check_arg(cmd_argv, argv, &i);

	kernel_ffs();

	return 0;
}
