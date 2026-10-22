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

static void command_loop(ціле input_fd, ціле output_fd)
{
	char buffer[MAXCOMMAND];

	поки (1) {
		size_t i;
		якщо (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			якщо (ferror(stdin))
				die("Input error");
			повернути;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		поки (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		якщо (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} інакше якщо (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			якщо (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			повернути;
		} інакше {
			die("Bad command: %s", buffer);
		}
	}
}

ціле cmd_remote_fd(ціле argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	ціле input_fd = -1;
	ціле output_fd = -1;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	якщо (argc != 3)
		usage(usage_msg);

	input_fd = (ціле)strtoul(argv[2], &end, 10);

	якщо ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	якщо (*end == '/' || !*end) {
		output_fd = input_fd;
	} інакше {
		char *end2;
		output_fd = (ціле)strtoul(end + 1, &end2, 10);

		якщо ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	повернути 0;
}
