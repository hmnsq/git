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

static void command_loop(จำนวนเต็ม input_fd, จำนวนเต็ม output_fd)
{
	var_na buffer[MAXCOMMAND];

	ขณะ (1) {
		size_t i;
		ถ้า (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ถ้า (ferror(stdin))
				die("Input error");
			คืน;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		ขณะ (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ถ้า (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} มิฉะนั้น ถ้า (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			ถ้า (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			คืน;
		} มิฉะนั้น {
			die("Bad command: %s", buffer);
		}
	}
}

จำนวนเต็ม cmd_remote_fd(จำนวนเต็ม argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม input_fd = -1;
	จำนวนเต็ม output_fd = -1;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	ถ้า (argc != 3)
		usage(usage_msg);

	input_fd = (จำนวนเต็ม)strtoul(argv[2], &end, 10);

	ถ้า ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	ถ้า (*end == '/' || !*end) {
		output_fd = input_fd;
	} มิฉะนั้น {
		var_na *end2;
		output_fd = (จำนวนเต็ม)strtoul(end + 1, &end2, 10);

		ถ้า ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	คืน 0;
}
