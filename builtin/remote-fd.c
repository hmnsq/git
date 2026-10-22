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

static void command_loop(цяло input_fd, цяло output_fd)
{
	char buffer[MAXCOMMAND];

	докато (1) {
		size_t i;
		ако (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ако (ferror(stdin))
				die("Input error");
			върни;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		докато (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ако (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} иначе ако (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			ако (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			върни;
		} иначе {
			die("Bad command: %s", buffer);
		}
	}
}

цяло cmd_remote_fd(цяло argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	цяло input_fd = -1;
	цяло output_fd = -1;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	ако (argc != 3)
		usage(usage_msg);

	input_fd = (цяло)strtoul(argv[2], &end, 10);

	ако ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	ако (*end == '/' || !*end) {
		output_fd = input_fd;
	} иначе {
		char *end2;
		output_fd = (цяло)strtoul(end + 1, &end2, 10);

		ако ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	върни 0;
}
