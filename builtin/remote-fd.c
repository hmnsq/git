#include "builtin.h"
#include "transport.h"

static const char usage_msg[] =
	"git remote-fd <remote> <url>";

/*
 * URL syntax:
 *	'fd::<inoutfd>[/<anything>]'		Read/write socket pair
 *						<inoutfd>.
 *	'fd::<infd>,<outfd>[/<anything>]'	Read pipe <infd> and write
 *						pipe <outfd>.
 *	[foo] indicates 'foo' is optional. <anything> is any string.
 *
 * The data output to <outfd>/<inoutfd> should be passed unmolested to
 * git-receive-pack/git-upload-pack/git-upload-archive and output of
 * git-receive-pack/git-upload-pack/git-upload-archive should be passed
 * unmolested to <infd>/<inoutfd>.
 *
 */

#define MAXCOMMAND 4096

static void command_loop(整数 input_fd, 整数 output_fd)
{
	char buffer[MAXCOMMAND];

	当 (1) {
		size_t i;
		如果 (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			如果 (ferror(stdin))
				die("Input error");
			返回;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		当 (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		如果 (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} 否则 如果 (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			如果 (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			返回;
		} 否则 {
			die("Bad command: %s", buffer);
		}
	}
}

整数 cmd_remote_fd(整数 argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	整数 input_fd = -1;
	整数 output_fd = -1;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	如果 (argc != 3)
		usage(usage_msg);

	input_fd = (整数)strtoul(argv[2], &end, 10);

	如果 ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	如果 (*end == '/' || !*end) {
		output_fd = input_fd;
	} 否则 {
		char *end2;
		output_fd = (整数)strtoul(end + 1, &end2, 10);

		如果 ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	返回 0;
}
