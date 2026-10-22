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

static void command_loop(puur_naa_nka input_fd, puur_naa_nka output_fd)
{
	var_na buffer[MAXCOMMAND];

	ᨀᨛ (1) {
		size_t i;
		ᨀᨘ (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ᨀᨘ (ferror(stdin))
				die("Input error");
			ᨀᨘᨒ;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		ᨀᨛ (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ᨀᨘ (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} ᨀᨘᨁ ᨀᨘ (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			ᨀᨘ (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			ᨀᨘᨒ;
		} ᨀᨘᨁ {
			die("Bad command: %s", buffer);
		}
	}
}

puur_naa_nka cmd_remote_fd(puur_naa_nka argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka input_fd = -1;
	puur_naa_nka output_fd = -1;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	ᨀᨘ (argc != 3)
		usage(usage_msg);

	input_fd = (puur_naa_nka)strtoul(argv[2], &end, 10);

	ᨀᨘ ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	ᨀᨘ (*end == '/' || !*end) {
		output_fd = input_fd;
	} ᨀᨘᨁ {
		var_na *end2;
		output_fd = (puur_naa_nka)strtoul(end + 1, &end2, 10);

		ᨀᨘ ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	ᨀᨘᨒ 0;
}
