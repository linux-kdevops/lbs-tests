#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "kernel.h"

static bool all = false;
static bool req_both_alignment = false;
static unsigned int min_order = 2;
static unsigned int offset = 0;
static unsigned int count = 50;

#define MAX_PAGECACHE_ORDER 20

static void usage(const char *cmd)
{
	printf("Usage:\n");
	printf("%s\n", cmd);
	printf("--help                           this help menu\n");
	printf("--order                          the minimum order to use, default is 2\n");
	printf("--offset                         initial count offset, default is 0\n");
	printf("--count                          how many times to loop, default is 50\n");
	printf("--offset-is-address              the given offset should be treated as an address instead of an index\n");
	printf("--all                            list all the orders which could be used\n");
	printf("--req_both_alignment             require both min order and max order alignment\n");
	exit(1);
}

void check_arg(const char *cmd, char *argv[], int *argc)
{
	if (strcmp(argv[*argc], "--all") == 0) {
		all = true;
		return;
	}
	if (strcmp(argv[*argc], "--req_both_alignment") == 0) {
		req_both_alignment = true;
		return;
	}
	if (strcmp(argv[*argc], "--order") == 0) {
		*argc = (*argc) + 1;
		if (*argc <= 1)
			usage(cmd);
		min_order = atoi(argv[*argc]);
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

static const char *bool_str(unsigned int order, bool val)
{
	if (order < min_order)
		return "Invalid";
	if (val)
		return "True";
	return "False";
}

int main(int argc, char *argv[])
{
	unsigned int i;
	unsigned int min_nrpages;
	unsigned int order;
	unsigned int idx;
	const char *cmd_argv = argv[0];
	bool last_idx_set = false;

	for (i=1; i < argc; i++)
		check_arg(cmd_argv, argv, &i);

	min_nrpages = 1UL << min_order;
	printf("Min-order: %u  nrpages: %u, Offset: %u  Count: %u\n", min_order, min_nrpages, offset, count);

	for (idx = offset; idx <= offset + count; idx++) {
		bool min_aligned = false;
		bool found = false;

		if ((idx & (min_nrpages - 1)) == 0)
			min_aligned = true;

		if (req_both_alignment && !min_aligned)
			continue;

		for (order = MAX_PAGECACHE_ORDER; order <= MAX_PAGECACHE_ORDER && order >= min_order; order--) {
			unsigned int nrpages = 1UL << order;
			bool aligned = false;

			if ((idx & (nrpages - 1)) == 0) {
				aligned = true;
				WARN_ON(idx & (min_nrpages -1));
				found = true;
			}

			if (req_both_alignment && !aligned)
				continue;

			printf("idx: %5u  order: %5u  min-order-aligned: %s         max-order-aligned: %s\n",
				idx,      order,      bool_str(order, min_aligned), bool_str(order, aligned));

			if (found)
				break;
		}
	}

	return 0;
}
