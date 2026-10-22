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

static void command_loop(целое input_fd, целое output_fd)
{
	char buffer[MAXCOMMAND];

	пока (1) {
		size_t i;
		если (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			если (ferror(stdin))
				die("Input error");
			вернуть;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		пока (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		если (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} иначе если (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			если (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			вернуть;
		} иначе {
			die("Bad command: %s", buffer);
		}
	}
}

целое cmd_remote_fd(целое argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	целое input_fd = -1;
	целое output_fd = -1;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	если (argc != 3)
		usage(usage_msg);

	input_fd = (целое)strtoul(argv[2], &end, 10);

	если ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	если (*end == '/' || !*end) {
		output_fd = input_fd;
	} иначе {
		char *end2;
		output_fd = (целое)strtoul(end + 1, &end2, 10);

		если ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	вернуть 0;
}
