#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <getopt.h>

#include "common.h"
//#include "bitmap.h"
//#include "min_heap.h"
//#include "list.h"
//#include "rbtree.h"
//#include "workqueue.h"

int control_msg_level = 0;
static char *target = NULL;

#define common_print_version(program_name)				\
	do {								\
		printf("%s version-%s\n", program_name, VERSION);	\
	} while(0)

#define usage(program_name)						\
	printf("Usage: %s [OPTIONS] target\n"				\
		"Run program as you wish\n\n"				\
		"Examples:\n"						\
		"Only print error message\n"				\
		"\t" #program_name " -l 2 TARGET\n\n"			\
		"Options:\n"						\
		"-l, --log-level=LEVEL		Display printing "	\
		"message according to given LEVEL (0 - debug, "		\
		"1 - verbose, 2 - notice)\n"				\
		"-V, --version			Display version "	\
		"information\n"						\
		"-h, --help			Show how to use\n\n"	\
		"Have a nice day!\n", program_name);

static const char *optstring = "l:Vh?";
static const struct option longopts[] = {
	{"log-level",	1, NULL, 'l'},
	{"version",	0, NULL, 'V'},
	{"help",	0, NULL, 'h'},
	{NULL,		0, NULL, 0}
};

static int get_options(const int argc, char **argv)
{
	int opt, i;
	char *endp;

	while (1) {
		opt = getopt_long(argc, argv, optstring, longopts, &i);
		if (opt == -1)
			break;
		switch (opt) {
			case 'l':
				control_msg_level = strtol(optarg, &endp, 0);
				if (*endp != '\0' || endp == optarg ||
					control_msg_level < 0 ||
					control_msg_level > 3) {
					WARN("bad debugging level '%s'", optarg);
					return -1;
				}
				break;
			case 'V':
				common_print_version(argv[0]);
				exit(0);
			case 'h':
			case '?':
			default:
				usage(argv[0])
				exit(0);
		}
	}

	if (optind != argc)
		target = strdup(argv[optind]);

	if (!target) {
		WARN("No target found!");
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int err;

	err = get_options(argc, argv);
	if (err)
		return err;

	MSG(0, "Run %s\n", target);

	free(target);

	return 0;
}
