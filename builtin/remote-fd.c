#include "builtin.h"
#include "transport.h"

static const var_na usage_msg[] =
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

static void command_loop(ཧྲིལ input_fd, ཧྲིལ output_fd)
{
	var_na buffer[MAXCOMMAND];

	སྐབས (1) {
		size_t i;
		གལ་སྲིད (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			གལ་སྲིད (ferror(stdin))
				die("Input error");
			ལོག;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		སྐབས (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		གལ་སྲིད (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} དེ་མིན གལ་སྲིད (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			གལ་སྲིད (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			ལོག;
		} དེ་མིན {
			die("Bad command: %s", buffer);
		}
	}
}

ཧྲིལ cmd_remote_fd(ཧྲིལ argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ཧྲིལ input_fd = -1;
	ཧྲིལ output_fd = -1;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	གལ་སྲིད (argc != 3)
		usage(usage_msg);

	input_fd = (ཧྲིལ)strtoul(argv[2], &end, 10);

	གལ་སྲིད ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	གལ་སྲིད (*end == '/' || !*end) {
		output_fd = input_fd;
	} དེ་མིན {
		var_na *end2;
		output_fd = (ཧྲིལ)strtoul(end + 1, &end2, 10);

		གལ་སྲིད ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	ལོག 0;
}
