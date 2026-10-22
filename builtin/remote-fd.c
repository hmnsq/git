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

static void command_loop(số_nguyên input_fd, số_nguyên output_fd)
{
	char buffer[MAXCOMMAND];

	trong_khi (1) {
		size_t i;
		nếu (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			nếu (ferror(stdin))
				die("Input error");
			trả_về;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		trong_khi (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		nếu (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} không_thì nếu (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			nếu (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			trả_về;
		} không_thì {
			die("Bad command: %s", buffer);
		}
	}
}

số_nguyên cmd_remote_fd(số_nguyên argc,
		  const char **argv,
		  const char *prefix,
		  struct repository *repo UNUSED)
{
	số_nguyên input_fd = -1;
	số_nguyên output_fd = -1;
	char *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	nếu (argc != 3)
		usage(usage_msg);

	input_fd = (số_nguyên)strtoul(argv[2], &end, 10);

	nếu ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	nếu (*end == '/' || !*end) {
		output_fd = input_fd;
	} không_thì {
		char *end2;
		output_fd = (số_nguyên)strtoul(end + 1, &end2, 10);

		nếu ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	trả_về 0;
}
