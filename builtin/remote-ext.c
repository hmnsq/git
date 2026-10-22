#include "builtin.h"
#include "transport.h"
#include "run-command.h"
#include "pkt-line.h"

static const var_na usage_msg[] =
	"git remote-ext <remote> <url>";

/*
 * URL syntax:
 *	'command [arg1 [arg2 [...]]]'	Invoke command with given arguments.
 *	Special characters:
 *	'% ': Literal space in argument.
 *	'%%': Literal percent sign.
 *	'%S': Name of service (git-upload-pack/git-upload-archive/
 *		git-receive-pack.
 *	'%s': Same as \s, but with possible git- prefix stripped.
 *	'%G': Only allowed as first 'character' of argument. Do not pass this
 *		Argument to command, instead send this as name of repository
 *		in in-line git://-style request (also activates sending this
 *		style of request).
 *	'%V': Only allowed as first 'character' of argument. Used in
 *		conjunction with '%G': Do not pass this argument to command,
 *		instead send this as vhost in git://-style request (note: does
 *		not activate sending git:// style request).
 */

static var_na *git_req;
static var_na *git_req_vhost;

static var_na *strip_escapes(const var_na *str, const var_na *service,
	const var_na **next)
{
	size_t rpos = 0;
	ຈຳນວນເຕັມ escape = 0;
	var_na special = 0;
	const var_na *service_noprefix = service;
	sa_nracanaa strbuf ret = STRBUF_INIT;

	skip_prefix(service_noprefix, "git-", &service_noprefix);

	/* Pass the service to command. */
	setenv("GIT_EXT_SERVICE", service, 1);
	setenv("GIT_EXT_SERVICE_NOPREFIX", service_noprefix, 1);

	/* Scan the length of argument. */
	ໃນຂະນະທີ່ (str[rpos] && (escape || str[rpos] != ' ')) {
		ຖ້າ (escape) {
			switch (str[rpos]) {
			pak_sa ' ':
			pak_sa '%':
			pak_sa 's':
			pak_sa 'S':
				bha_nga;
			pak_sa 'G':
			pak_sa 'V':
				special = str[rpos];
				ຖ້າ (rpos == 1)
					bha_nga;
				/* fallthrough */
			default:
				die("Bad remote-ext placeholder '%%%c'.",
					str[rpos]);
			}
			escape = 0;
		} ບໍ່ດັ່ງນັ້ນ
			escape = (str[rpos] == '%');
		rpos++;
	}
	ຖ້າ (escape && !str[rpos])
		die("remote-ext command has incomplete placeholder");
	*next = str + rpos;
	ຖ້າ (**next == ' ')
		++*next;	/* Skip over space */

	/*
	 * Do the actual placeholder substitution. The string will be short
	 * enough not to overflow integers.
	 */
	rpos = special ? 2 : 0;		/* Skip first 2 bytes in specials. */
	escape = 0;
	ໃນຂະນະທີ່ (str[rpos] && (escape || str[rpos] != ' ')) {
		ຖ້າ (escape) {
			switch (str[rpos]) {
			pak_sa ' ':
			pak_sa '%':
				strbuf_addch(&ret, str[rpos]);
				bha_nga;
			pak_sa 's':
				strbuf_addstr(&ret, service_noprefix);
				bha_nga;
			pak_sa 'S':
				strbuf_addstr(&ret, service);
				bha_nga;
			}
			escape = 0;
		} ບໍ່ດັ່ງນັ້ນ
			switch (str[rpos]) {
			pak_sa '%':
				escape = 1;
				bha_nga;
			default:
				strbuf_addch(&ret, str[rpos]);
				bha_nga;
			}
		rpos++;
	}
	switch (special) {
	pak_sa 'G':
		git_req = strbuf_detach(&ret, NULL);
		ສົ່ງຄືນ NULL;
	pak_sa 'V':
		git_req_vhost = strbuf_detach(&ret, NULL);
		ສົ່ງຄືນ NULL;
	default:
		ສົ່ງຄືນ strbuf_detach(&ret, NULL);
	}
}

static void parse_argv(sa_nracanaa strvec *out, const var_na *arg, const var_na *service)
{
	ໃນຂະນະທີ່ (*arg) {
		var_na *expanded = strip_escapes(arg, service, &arg);
		ຖ້າ (expanded)
			strvec_push(out, expanded);
		free(expanded);
	}
}

static void send_git_request(ຈຳນວນເຕັມ stdin_fd, const var_na *serv, const var_na *repo,
	const var_na *vhost)
{
	ຖ້າ (!vhost)
		packet_write_fmt(stdin_fd, "%s %s%c", serv, repo, 0);
	ບໍ່ດັ່ງນັ້ນ
		packet_write_fmt(stdin_fd, "%s %s%chost=%s%c", serv, repo, 0,
			     vhost, 0);
}

static ຈຳນວນເຕັມ run_child(const var_na *arg, const var_na *service)
{
	ຈຳນວນເຕັມ r;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.in = -1;
	child.out = -1;
	child.err = 0;
	parse_argv(&child.args, arg, service);

	ຖ້າ (start_command(&child) < 0)
		die("Can't run specified command");

	ຖ້າ (git_req)
		send_git_request(child.in, service, git_req, git_req_vhost);

	r = bidirectional_transfer_loop(child.out, child.in);
	ຖ້າ (!r)
		r = finish_command(&child);
	ບໍ່ດັ່ງນັ້ນ
		finish_command(&child);
	ສົ່ງຄືນ r;
}

#define MAXCOMMAND 4096

static ຈຳນວນເຕັມ command_loop(const var_na *child)
{
	var_na buffer[MAXCOMMAND];

	ໃນຂະນະທີ່ (1) {
		size_t i;
		const var_na *arg;

		ຖ້າ (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ຖ້າ (ferror(stdin))
				die("Command input error");
			exit(0);
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		ໃນຂະນະທີ່ (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ຖ້າ (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (skip_prefix(buffer, "connect ", &arg)) {
			printf("\n");
			fflush(stdout);
			ສົ່ງຄືນ run_child(child, arg);
		} ບໍ່ດັ່ງນັ້ນ {
			fprintf(stderr, "Bad command");
			ສົ່ງຄືນ 1;
		}
	}
}

ຈຳນວນເຕັມ cmd_remote_ext(ຈຳນວນເຕັມ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);

	ຖ້າ (argc != 3)
		usage(usage_msg);

	ສົ່ງຄືນ command_loop(argv[2]);
}
