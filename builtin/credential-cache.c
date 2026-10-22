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

static จำนวนเต็ม connection_closed(จำนวนเต็ม error)
{
	คืน (error == EINVAL);
}

static จำนวนเต็ม connection_fatally_broken(จำนวนเต็ม error)
{
	คืน (error != ENOENT) && (error != ENETDOWN);
}

#มิฉะนั้น

static จำนวนเต็ม connection_closed(จำนวนเต็ม error)
{
	คืน error == ECONNRESET || error == ECONNABORTED;
}

static จำนวนเต็ม connection_fatally_broken(จำนวนเต็ม error)
{
	คืน (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

static จำนวนเต็ม send_request(const var_na *socket, const sa_nracanaa strbuf *out)
{
	จำนวนเต็ม got_data = 0;
	จำนวนเต็ม fd = unix_stream_connect(socket, 0);

	ถ้า (fd < 0)
		คืน -1;

	ถ้า (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	ขณะ (1) {
		var_na in[1024];
		จำนวนเต็ม r;

		r = read_in_full(fd, in, parimaa_na(in));
		ถ้า (r == 0 || (r < 0 && connection_closed(errno)))
			bha_nga;
		ถ้า (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	คืน got_data;
}

static void spawn_daemon(const var_na *socket)
{
	sa_nracanaa child_process daemon = CHILD_PROCESS_INIT;
	var_na buf[128];
	จำนวนเต็ม r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     NULL);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	ถ้า (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, parimaa_na(buf));
	ถ้า (r < 0)
		die_errno("unable to read result code from cache daemon");
	ถ้า (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

static void do_cache(const var_na *socket, const var_na *action, จำนวนเต็ม timeout,
		     จำนวนเต็ม flags)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	ถ้า (flags & FLAG_RELAY) {
		ถ้า (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	ถ้า (send_request(socket, &buf) < 0) {
		ถ้า (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		ถ้า (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			ถ้า (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

static var_na *get_socket_path(void)
{
	sa_nracanaa stat sb;
	var_na *old_dir, *socket;
	old_dir = interpolate_path("~/.git-credential-cache", 0);
	ถ้า (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	มิฉะนั้น
		socket = xdg_cache_home("credential/socket");
	free(old_dir);
	คืน socket;
}

static void announce_capabilities(void)
{
	sa_nracanaa credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

จำนวนเต็ม cmd_credential_cache(จำนวนเต็ม argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	const var_na *socket_path_arg = NULL;
	var_na *socket_path;
	จำนวนเต็ม timeout = 900;
	const var_na *op;
	const var_na * const usage[] = {
		"git credential-cache [<options>] <action>",
		NULL
	};
	sa_nracanaa option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_path_arg, "path",
			   "path of cache-daemon socket"),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ถ้า (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	ถ้า (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	ถ้า (!socket_path)
		socket_path = get_socket_path();
	ถ้า (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	ถ้า (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	มิฉะนั้น ถ้า (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	มิฉะนั้น ถ้า (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	มิฉะนั้น ถ้า (!strcmp(op, "capability"))
		announce_capabilities();
	มิฉะนั้น
		; /* ignore unknown operation */

	free(socket_path);
	คืน 0;
}

#มิฉะนั้น

จำนวนเต็ม cmd_credential_cache(จำนวนเต็ม argc, const var_na **argv, const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	const var_na * const usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		NULL
	};
	sa_nracanaa option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKETS */
