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

static void command_loop(ຈຳນວນເຕັມ input_fd, ຈຳນວນເຕັມ output_fd)
{
	var_na buffer[MAXCOMMAND];

	ໃນຂະນະທີ່ (1) {
		size_t i;
		ຖ້າ (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ຖ້າ (ferror(stdin))
				die("Input error");
			ສົ່ງຄືນ;
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		ໃນຂະນະທີ່ (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ຖ້າ (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (starts_with(buffer, "connect ")) {
			printf("\n");
			fflush(stdout);
			ຖ້າ (bidirectional_transfer_loop(input_fd,
				output_fd))
				die("Copying data between file descriptors failed");
			ສົ່ງຄືນ;
		} ບໍ່ດັ່ງນັ້ນ {
			die("Bad command: %s", buffer);
		}
	}
}

ຈຳນວນເຕັມ cmd_remote_fd(ຈຳນວນເຕັມ argc,
		  const var_na **argv,
		  const var_na *prefix,
		  sa_nracanaa repository *repo UNUSED)
{
	ຈຳນວນເຕັມ input_fd = -1;
	ຈຳນວນເຕັມ output_fd = -1;
	var_na *end;

	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);
	ຖ້າ (argc != 3)
		usage(usage_msg);

	input_fd = (ຈຳນວນເຕັມ)strtoul(argv[2], &end, 10);

	ຖ້າ ((end == argv[2]) || (*end != ',' && *end != '/' && *end))
		die("Bad URL syntax");

	ຖ້າ (*end == '/' || !*end) {
		output_fd = input_fd;
	} ບໍ່ດັ່ງນັ້ນ {
		var_na *end2;
		output_fd = (ຈຳນວນເຕັມ)strtoul(end + 1, &end2, 10);

		ຖ້າ ((end2 == end + 1) || (*end2 != '/' && *end2))
			die("Bad URL syntax");
	}

	command_loop(input_fd, output_fd);
	ສົ່ງຄືນ 0;
}
