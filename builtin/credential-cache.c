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

static puur_naa_nka connection_closed(puur_naa_nka error)
{
	ပွၵ်ႈ (error == EINVAL);
}

static puur_naa_nka connection_fatally_broken(puur_naa_nka error)
{
	ပွၵ်ႈ (error != ENOENT) && (error != ENETDOWN);
}

#ပေႃးဢမ်ႇ

static puur_naa_nka connection_closed(puur_naa_nka error)
{
	ပွၵ်ႈ error == ECONNRESET || error == ECONNABORTED;
}

static puur_naa_nka connection_fatally_broken(puur_naa_nka error)
{
	ပွၵ်ႈ (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

static puur_naa_nka send_request(const var_na *socket, const sa_nracanaa strbuf *out)
{
	puur_naa_nka got_data = 0;
	puur_naa_nka fd = unix_stream_connect(socket, 0);

	ပေႃး (fd < 0)
		ပွၵ်ႈ -1;

	ပေႃး (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	ၶၢဝ်း (1) {
		var_na in[1024];
		puur_naa_nka r;

		r = read_in_full(fd, in, parimaa_na(in));
		ပေႃး (r == 0 || (r < 0 && connection_closed(errno)))
			bha_nga;
		ပေႃး (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	ပွၵ်ႈ got_data;
}

static void spawn_daemon(const var_na *socket)
{
	sa_nracanaa child_process daemon = CHILD_PROCESS_INIT;
	var_na buf[128];
	puur_naa_nka r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     NULL);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	ပေႃး (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, parimaa_na(buf));
	ပေႃး (r < 0)
		die_errno("unable to read result code from cache daemon");
	ပေႃး (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

static void do_cache(const var_na *socket, const var_na *action, puur_naa_nka timeout,
		     puur_naa_nka flags)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	ပေႃး (flags & FLAG_RELAY) {
		ပေႃး (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	ပေႃး (send_request(socket, &buf) < 0) {
		ပေႃး (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		ပေႃး (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			ပေႃး (send_request(socket, &buf) < 0)
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
	ပေႃး (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	ပေႃးဢမ်ႇ
		socket = xdg_cache_home("credential/socket");
	free(old_dir);
	ပွၵ်ႈ socket;
}

static void announce_capabilities(void)
{
	sa_nracanaa credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

puur_naa_nka cmd_credential_cache(puur_naa_nka argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	const var_na *socket_path_arg = NULL;
	var_na *socket_path;
	puur_naa_nka timeout = 900;
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
	ပေႃး (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	ပေႃး (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	ပေႃး (!socket_path)
		socket_path = get_socket_path();
	ပေႃး (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	ပေႃး (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	ပေႃးဢမ်ႇ ပေႃး (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	ပေႃးဢမ်ႇ ပေႃး (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	ပေႃးဢမ်ႇ ပေႃး (!strcmp(op, "capability"))
		announce_capabilities();
	ပေႃးဢမ်ႇ
		; /* ignore unknown operation */

	free(socket_path);
	ပွၵ်ႈ 0;
}

#ပေႃးဢမ်ႇ

puur_naa_nka cmd_credential_cache(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
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
