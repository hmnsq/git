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

static void command_loop(정수 input_fd, 정수 output_fd)
{
	char buffer[MAXCOMMAND];

	동안 (1) {
		size_t i;
		만약 (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			만약 (ferror(stdin))
				die("Input error");
			반환;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		동안 (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		만약 (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} 아니면 만약 (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			만약 (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			반환;
		} 아니면 {
			die("Bad command: %s", buffer);
		}
	}
}

정수 cmd_remote_fd(정수 argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	정수 input_fd = -1;
	정수 output_fd = -1;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	만약 (argc != 3)
		usage(usage_msg);

	input_fd = (정수)strtoul(argv[2], &end, 10);

	만약 ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	만약 (*end == '/' || !*end) {
		output_fd = input_fd;
	} 아니면 {
		char *end2;
		output_fd = (정수)strtoul(end + 1, &end2, 10);

		만약 ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	반환 0;
}
