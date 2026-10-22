#include "builtin.h"
#include "transport.h"
#include "run-command.h"
#include "pkt-line.h"

static const char usage_msg[] =
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

static char *git_req;
static char *git_req_vhost;

static char *strip_escapes(const char *str, const char *service,
	const char **next)
{
	size_t rpos = 0;
	ціле escape = 0;
	char special = 0;
	const char *service_noprefix = service;
	struct strbuf ret = STRBUF_INIT;

	skip_prefix(service_noprefix, "git-", &service_noprefix);

	/* Pass the service to command. */
	setenv("GIT_EXT_SERVICE", service, 1);
	setenv("GIT_EXT_SERVICE_NOPREFIX", service_noprefix, 1);

	/* Scan the length of argument. */
	поки (str[rpos] && (escape || str[rpos] != ' ')) {
		якщо (escape) {
			switch (str[rpos]) {
			case ' ':
			case '%':
			case 's':
			case 'S':
				break;
			case 'G':
			case 'V':
				special = str[rpos];
				якщо (rpos == 1)
					break;
				/* fallthrough */
			default:
				die("Bad remote-ext placeholder '%%%c'.",
					str[rpos]);
			}
			escape = 0;
		} інакше
			escape = (str[rpos] == '%');
		rpos++;
	}
	якщо (escape && !str[rpos])
		die("remote-ext command has incomplete placeholder");
	*next = str + rpos;
	якщо (**next == ' ')
		++*next;	/* Skip over space */

	/*
	 * Do the actual placeholder substitution. The string will be short
	 * enough not to overflow integers.
	 */
	rpos = special ? 2 : 0;		/* Skip first 2 bytes in specials. */
	escape = 0;
	поки (str[rpos] && (escape || str[rpos] != ' ')) {
		якщо (escape) {
			switch (str[rpos]) {
			case ' ':
			case '%':
				strbuf_addch(&ret, str[rpos]);
				break;
			case 's':
				strbuf_addstr(&ret, service_noprefix);
				break;
			case 'S':
				strbuf_addstr(&ret, service);
				break;
			}
			escape = 0;
		} інакше
			switch (str[rpos]) {
			case '%':
				escape = 1;
				break;
			default:
				strbuf_addch(&ret, str[rpos]);
				break;
			}
		rpos++;
	}
	switch (special) {
	case 'G':
		git_req = strbuf_detach(&ret, NULL);
		повернути NULL;
	case 'V':
		git_req_vhost = strbuf_detach(&ret, NULL);
		повернути NULL;
	default:
		повернути strbuf_detach(&ret, NULL);
	}
}

static void parse_argv(struct strvec *out, const char *arg, const char *service)
{
	поки (*arg) {
		char *expanded = strip_escapes(arg, service, &arg);
		якщо (expanded)
			strvec_push(out, expanded);
		free(expanded);
	}
}

static void send_git_request(ціле stdin_fd, const char *serv, const char *repo,
	const char *vhost)
{
	якщо (!vhost)
		packet_write_fmt(stdin_fd, "%s %s%c", serv, repo, 0);
	інакше
		packet_write_fmt(stdin_fd, "%s %s%chost=%s%c", serv, repo, 0,
			     vhost, 0);
}

static ціле run_child(const char *arg, const char *service)
{
	ціле r;
	struct child_process child = CHILD_PROCESS_INIT;

	child.in = -1;
	child.out = -1;
	child.err = 0;
	parse_argv(&child.args, arg, service);

	якщо (start_command(&child) < 0)
		die("Can't run specified command");

	якщо (git_req)
		send_git_request(child.in, service, git_req, git_req_vhost);

	r = bidirectional_transfer_loop(child.out, child.in);
	якщо (!r)
		r = finish_command(&child);
	інакше
		finish_command(&child);
	повернути r;
}

#define MAXCOMMAND 4096

static ціле command_loop(const char *child)
{
	char buffer[MAXCOMMAND];

	поки (1) {
		size_t i;
		const char *arg;

		якщо (!fgets(buffer, MAXCOMMAND - 1, stdin)) {
			якщо (ferror(stdin))
				die("Command input error");
			exit(0);
		}
		/* Strip end of line characters. */
		i = strlen(buffer);
		поки (i > 0 && isspace(buffer[i - 1]))
			buffer[--i] = 0;

		якщо (!strcmp(buffer, "capabilities")) {
			printf("*connect\n\n");
			fflush(stdout);
		} інакше якщо (skip_prefix(buffer, "connect ", &arg)) {
			printf("\n");
			fflush(stdout);
			повернути run_child(child, arg);
		} інакше {
			fprintf(stderr, "Bad command");
			повернути 1;
		}
	}
}

ціле cmd_remote_ext(ціле argc,
		   const char **argv,
		   const char *prefix,
		   struct repository *repo UNUSED)
{
	BUG_ON_NON_EMPTY_PREFIX(prefix);

	show_usage_if_asked(argc, argv, usage_msg);

	якщо (argc != 3)
		usage(usage_msg);

	повернути command_loop(argv[2]);
}
