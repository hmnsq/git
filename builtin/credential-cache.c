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

static số_nguyên connection_closed(số_nguyên error)
{
	trả_về (error == EINVAL);
}

static số_nguyên connection_fatally_broken(số_nguyên error)
{
	trả_về (error != ENOENT) && (error != ENETDOWN);
}

#không_thì

static số_nguyên connection_closed(số_nguyên error)
{
	trả_về error == ECONNRESET || error == ECONNABORTED;
}

static số_nguyên connection_fatally_broken(số_nguyên error)
{
	trả_về (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

static số_nguyên send_request(const char *socket, const struct strbuf *out)
{
	số_nguyên got_data = 0;
	số_nguyên fd = unix_stream_connect(socket, 0);

	nếu (fd < 0)
		trả_về -1;

	nếu (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	trong_khi (1) {
		char in[1024];
		số_nguyên r;

		r = read_in_full(fd, in, sizeof(in));
		nếu (r == 0 || (r < 0 && connection_closed(errno)))
			break;
		nếu (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	trả_về got_data;
}

static void spawn_daemon(const char *socket)
{
	struct child_process daemon = CHILD_PROCESS_INIT;
	char buf[128];
	số_nguyên r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     NULL);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	nếu (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, sizeof(buf));
	nếu (r < 0)
		die_errno("unable to read result code from cache daemon");
	nếu (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

static void do_cache(const char *socket, const char *action, số_nguyên timeout,
		     số_nguyên flags)
{
	struct strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	nếu (flags & FLAG_RELAY) {
		nếu (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	nếu (send_request(socket, &buf) < 0) {
		nếu (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		nếu (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			nếu (send_request(socket, &buf) < 0)
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
	nếu (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	không_thì
		socket = xdg_cache_home("credential/socket");
	free(old_dir);
	trả_về socket;
}

static void announce_capabilities(void)
{
	struct credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

số_nguyên cmd_credential_cache(số_nguyên argc,
			 const char **argv,
			 const char *prefix,
			 struct repository *repo UNUSED)
{
	const char *socket_path_arg = NULL;
	char *socket_path;
	số_nguyên timeout = 900;
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
	nếu (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	nếu (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	nếu (!socket_path)
		socket_path = get_socket_path();
	nếu (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	nếu (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	không_thì nếu (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	không_thì nếu (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	không_thì nếu (!strcmp(op, "capability"))
		announce_capabilities();
	không_thì
		; /* ignore unknown operation */

	free(socket_path);
	trả_về 0;
}

#không_thì

số_nguyên cmd_credential_cache(số_nguyên argc, const char **argv, const char *prefix,
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
