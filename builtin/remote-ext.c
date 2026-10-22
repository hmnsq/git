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
	ចំនួនគត់ escape = 0;
	var_na special = 0;
	const var_na *service_noprefix = service;
	sa_nracanaa strbuf ret = STRBUF_INIT;

	skip_prefix(service_noprefix, "git-", &service_noprefix);

	/* Pass the service to command. */
	setenv("GIT_EXT_SERVICE", service, 1);
	setenv("GIT_EXT_SERVICE_NOPREFIX", service_noprefix, 1);

	/* Scan the length of argument. */
	ខណៈពេល (str[rpos] && (escape || str[rpos] != ' ')) {
		ប្រសិនបើ (escape) {
			switch (str[rpos]) {
			pak_sa ' ':
			pak_sa '%':
			pak_sa 's':
			pak_sa 'S':
				bha_nga;
			pak_sa 'G':
			pak_sa 'V':
				special = str[rpos];
				ប្រសិនបើ (rpos == 1)
					bha_nga;
				/* fallthrough */
			default:
				die("Bad remote-ext placeholder '%%%c'.",
					str[rpos]);
			}
			escape = 0;
		} បើមិនដូច្នេះ
			escape = (str[rpos] == '%');
		rpos++;
	}
	ប្រសិនបើ (escape && !str[rpos])
		die("remote-ext command has incomplete placeholder");
	*next = str + rpos;
	ប្រសិនបើ (**next == ' ')
		++*next;	/* Skip over space */

	/*
	 * Do the actual placeholder substitution. The string will be short
	 * enough not to overflow integers.
	 */
	rpos = special ? 2 : 0;		/* Skip first 2 bytes in specials. */
	escape = 0;
	ខណៈពេល (str[rpos] && (escape || str[rpos] != ' ')) {
		ប្រសិនបើ (escape) {
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
		} បើមិនដូច្នេះ
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
		ត្រឡប់ NULL;
	pak_sa 'V':
		git_req_vhost = strbuf_detach(&ret, NULL);
		ត្រឡប់ NULL;
	default:
		ត្រឡប់ strbuf_detach(&ret, NULL);
	}
}

static void parse_argv(sa_nracanaa strvec *out, const var_na *arg, const var_na *service)
{
	ខណៈពេល (*arg) {
		var_na *expanded = strip_escapes(arg, service, &arg);
		ប្រសិនបើ (expanded)
			strvec_push(out, expanded);
		free(expanded);
	}
}

static void send_git_request(ចំនួនគត់ stdin_fd, const var_na *serv, const var_na *repo,
	const var_na *vhost)
{
	ប្រសិនបើ (!vhost)
		packet_write_fmt(stdin_fd, "%s %s%c", serv, repo, 0);
	បើមិនដូច្នេះ
		packet_write_fmt(stdin_fd, "%s %s%chost=%s%c", serv, repo, 0,
			     vhost, 0);
}

static ចំនួនគត់ run_child(const var_na *arg, const var_na *service)
{
	ចំនួនគត់ r;
	sa_nracanaa child_process child = CHILD_PROCESS_INIT;

	child.in = -1;
	child.out = -1;
	child.err = 0;
	parse_argv(&child.args, arg, service);

	ប្រសិនបើ (start_command(&child) < 0)
		die("Can't run specified command");

	ប្រសិនបើ (git_req)
		send_git_request(child.in, service, git_req, git_req_vhost);

	r = bidirectional_transfer_loop(child.out, child.in);
	ប្រសិនបើ (!r)
		r = finish_command(&child);
	បើមិនដូច្នេះ
		finish_command(&child);
	ត្រឡប់ r;
}

#define MAXCOMMAND 4096

static ចំនួនគត់ command_loop(const var_na *child)
{
	var_na buffer[MAXCOMMAND];

	ខណៈពេល (1) {
		size_t i;
		const var_na *arg;

		ប្រសិនបើ (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			ប្រសិនបើ (ferror(stdin))
				die("Command input error");
			exit(0);
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		ខណៈពេល (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		ប្រសិនបើ (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} បើមិនដូច្នេះ ប្រសិនបើ (skip_prefix(buffer, "connect ", &arg)) {
			printf("\n");
			fflush(stdout);
			ត្រឡប់ run_child(child, arg);
		} បើមិនដូច្នេះ {
			fprintf(stderr, "Bad command");
			ត្រឡប់ 1;
		}
	}
}

ចំនួនគត់ cmd_remote_ext(ចំនួនគត់ argc,
		   const var_na **argv,
		   const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);

	ប្រសិនបើ (argc != 3)
		usage(usage_msg);

	ត្រឡប់ command_loop(argv[2]);
}
