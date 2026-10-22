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

static void command_loop(ចំនួនគត់ input_fd, ចំនួនគត់ output_fd)
{
	var_na buffer[MAXCOMMAND];

	ខណៈពេល (1) {
		size_t i;
		ប្រសិនបើ (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ប្រសិនបើ (ferror(stdin))
				die("Input error");
			ត្រឡប់;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		ខណៈពេល (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ប្រសិនបើ (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} បើមិនដូច្នេះ ប្រសិនបើ (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			ប្រសិនបើ (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			ត្រឡប់;
		} បើមិនដូច្នេះ {
			die("Bad command: %s", buffer);
		}
	}
}

ចំនួនគត់ cmd_remote_fd(ចំនួនគត់ argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ចំនួនគត់ input_fd = -1;
	ចំនួនគត់ output_fd = -1;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	ប្រសិនបើ (argc != 3)
		usage(usage_msg);

	input_fd = (ចំនួនគត់)strtoul(argv[2], &end, 10);

	ប្រសិនបើ ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	ប្រសិនបើ (*end == '/' || !*end) {
		output_fd = input_fd;
	} បើមិនដូច្នេះ {
		var_na *end2;
		output_fd = (ចំនួនគត់)strtoul(end + 1, &end2, 10);

		ប្រសិនបើ ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	ត្រឡប់ 0;
}
