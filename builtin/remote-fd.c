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

static void command_loop(ကိန်းပြည့် input_fd, ကိန်းပြည့် output_fd)
{
	var_na buffer[MAXCOMMAND];

	နေစဉ် (1) {
		size_t i;
		အကယ်၍ (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			အကယ်၍ (ferror(stdin))
				die("Input error");
			ပြန်ပေး;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		နေစဉ် (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		အကယ်၍ (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} မဟုတ်ပါက အကယ်၍ (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			အကယ်၍ (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			ပြန်ပေး;
		} မဟုတ်ပါက {
			die("Bad command: %s", buffer);
		}
	}
}

ကိန်းပြည့် cmd_remote_fd(ကိန်းပြည့် argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် input_fd = -1;
	ကိန်းပြည့် output_fd = -1;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	အကယ်၍ (argc != 3)
		usage(usage_msg);

	input_fd = (ကိန်းပြည့်)strtoul(argv[2], &end, 10);

	အကယ်၍ ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	အကယ်၍ (*end == '/' || !*end) {
		output_fd = input_fd;
	} မဟုတ်ပါက {
		var_na *end2;
		output_fd = (ကိန်းပြည့်)strtoul(end + 1, &end2, 10);

		အကယ်၍ ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	ပြန်ပေး 0;
}
