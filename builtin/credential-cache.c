#include "builtin.h"
#include "credential.h"
#include "gettext.h"
#include "parse-options.h"
#include "path.h"
#include "strbuf.h"
#include "write-or-die.h"

#ifndef NO_UNIX_SOCKETS

#include "unix-socket.h"
#include "run-command.h"

#define FLAG_SPAWN 0x1
#define FLAG_RELAY 0x2

#ifdef GIT_WINDOWS_NATIVE

static 정수 connection_closed(정수 error)
{
	반환 (error == EINVAL);
}

static 정수 connection_fatally_broken(정수 error)
{
	반환 (error != ENOENT) && (error != ENETDOWN);
}

#아니면

static 정수 connection_closed(정수 error)
{
	반환 error == ECONNRESET || error == ECONNABORTED;
}

static 정수 connection_fatally_broken(정수 error)
{
	반환 (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

static 정수 send_request(const char *socket, const struct strbuf *out)
{
	정수 got_data = 0;
	정수 fd = unix_stream_connect(socket, 0);

	만약 (fd < 0)
		반환 -1;

	만약 (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	동안 (1) {
		char in[1024];
		정수 r;

		r = read_in_full(fd, in, sizeof(in));
		만약 (r == 0 || (r < 0 && connection_closed(errno)))
			break;
		만약 (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	반환 got_data;
}

static void spawn_daemon(const char *socket)
{
	struct child_process daemon = CHILD_PROCESS_INIT;
	char buf[128];
	정수 r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     NULL);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	만약 (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, sizeof(buf));
	만약 (r < 0)
		die_errno("unable to read result code from cache daemon");
	만약 (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

static void do_cache(const char *socket, const char *action, 정수 timeout,
		     정수 flags)
{
	struct strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	만약 (flags & FLAG_RELAY) {
		만약 (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	만약 (send_request(socket, &buf) < 0) {
		만약 (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		만약 (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			만약 (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

static char *get_socket_path(void)
{
	struct stat sb;
	char *old_dir, *socket;
	old_dir = interpolate_path("~/.git-credential-cache", 0);
	만약 (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	아니면
		socket = xdg_cache_home("credential/socket");
	free(old_dir);
	반환 socket;
}

static void announce_capabilities(void)
{
	struct credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

정수 cmd_credential_cache(정수 argc,
			 const char **argv,
			 const char *prefix,
			 struct repository *repo UNUSED)
{
	const char *socket_path_arg = NULL;
	char *socket_path;
	정수 timeout = 900;
	const char *op;
	const char * const usage[] = {
		"git credential-cache [<options>] <action>",
		NULL
	};
	struct option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_path_arg, "path",
			   "path of cache-daemon socket"),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	만약 (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	만약 (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	만약 (!socket_path)
		socket_path = get_socket_path();
	만약 (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	만약 (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	아니면 만약 (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	아니면 만약 (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	아니면 만약 (!strcmp(op, "capability"))
		announce_capabilities();
	아니면
		; /* ignore unknown operation */

	free(socket_path);
	반환 0;
}

#아니면

정수 cmd_credential_cache(정수 argc, const char **argv, const char *prefix,
			 struct repository *repo UNUSED)
{
	const char * const usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		NULL
	};
	struct option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKETS */
