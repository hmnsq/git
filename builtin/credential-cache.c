#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "credential.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_UNIX_SOCKETS

#ਅਨ੍ਤਰ੍ਭਾਵ "unix-socket.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_SPAWN 0x1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FLAG_RELAY 0x2

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GIT_WINDOWS_NATIVE

ਸਥਿਰ ਪੂਰਨ connection_closed(ਪੂਰਨ error)
{
	ਵਾਪਸ (error == EINVAL);
}

ਸਥਿਰ ਪੂਰਨ connection_fatally_broken(ਪੂਰਨ error)
{
	ਵਾਪਸ (error != ENOENT) && (error != ENETDOWN);
}

#ਨਹੀਂਤਾਂ

ਸਥਿਰ ਪੂਰਨ connection_closed(ਪੂਰਨ error)
{
	ਵਾਪਸ error == ECONNRESET || error == ECONNABORTED;
}

ਸਥਿਰ ਪੂਰਨ connection_fatally_broken(ਪੂਰਨ error)
{
	ਵਾਪਸ (error != ENOENT) && (error != ECONNREFUSED);
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ send_request(ਅਚਲ ਅੱਖਰ *socket, ਅਚਲ ਬਣਤਰ strbuf *ਬਾਹਰਲਾ)
{
	ਪੂਰਨ got_ਅੰਕੜੇ = 0;
	ਪੂਰਨ fd = unix_ਧਾਰਾ_connect(socket, 0);

	ਜੇ (fd < 0)
		ਵਾਪਸ -1;

	ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(fd, ਬਾਹਰਲਾ->buf, ਬਾਹਰਲਾ->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	ਜਦੋਂ (1) {
		ਅੱਖਰ ਵਿੱਚ[1024];
		ਪੂਰਨ r;

		r = read_ਵਿੱਚ_ਭਰਿਆ(fd, ਵਿੱਚ, ਆਕਾਰ(ਵਿੱਚ));
		ਜੇ (r == 0 || (r < 0 && connection_closed(errno)))
			ਰੋਕੋ;
		ਜੇ (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, ਵਿੱਚ, r);
		got_ਅੰਕੜੇ = 1;
	}
	close(fd);
	ਵਾਪਸ got_ਅੰਕੜੇ;
}

ਸਥਿਰ ਖਾਲੀ spawn_daemon(ਅਚਲ ਅੱਖਰ *socket)
{
	ਬਣਤਰ ਬੱਚਾ_process daemon = ਬੱਚਾ_PROCESS_INIT;
	ਅੱਖਰ buf[128];
	ਪੂਰਨ r;

	strvec_pushl(&daemon.ਦਲੀਲਾਂ,
		     "credential-cache--daemon", socket,
		     ਨਿਰਾ);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.ਬਾਹਰਲਾ = -1;

	ਜੇ (ਸ਼ੁਰੂ_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_ਵਿੱਚ_ਭਰਿਆ(daemon.ਬਾਹਰਲਾ, buf, ਆਕਾਰ(buf));
	ਜੇ (r < 0)
		die_errno("unable to read result code from cache daemon");
	ਜੇ (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	ਬੱਚਾ_process_clear(&daemon);
	close(daemon.ਬਾਹਰਲਾ);
}

ਸਥਿਰ ਖਾਲੀ do_cache(ਅਚਲ ਅੱਖਰ *socket, ਅਚਲ ਅੱਖਰ *action, ਪੂਰਨ timeout,
		     ਪੂਰਨ flags)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	ਜੇ (flags & FLAG_RELAY) {
		ਜੇ (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	ਜੇ (send_request(socket, &buf) < 0) {
		ਜੇ (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		ਜੇ (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			ਜੇ (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

ਸਥਿਰ ਅੱਖਰ *get_socket_ਰਸਤਾ(ਖਾਲੀ)
{
	ਬਣਤਰ stat sb;
	ਅੱਖਰ *old_dir, *socket;
	old_dir = interpolate_ਰਸਤਾ("~/.git-credential-cache", 0);
	ਜੇ (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_ਬਹੁਲਕ))
		socket = xstrfmt("%s/socket", old_dir);
	ਨਹੀਂਤਾਂ
		socket = xdg_cache_home("credential/socket");
	ਮੁਕਤ(old_dir);
	ਵਾਪਸ socket;
}

ਸਥਿਰ ਖਾਲੀ announce_capabilities(ਖਾਲੀ)
{
	ਬਣਤਰ credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

ਪੂਰਨ cmd_credential_cache(ਪੂਰਨ argc,
			 ਅਚਲ ਅੱਖਰ **argv,
			 ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *socket_ਰਸਤਾ_arg = ਨਿਰਾ;
	ਅੱਖਰ *socket_ਰਸਤਾ;
	ਪੂਰਨ timeout = 900;
	ਅਚਲ ਅੱਖਰ *op;
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		"git credential-cache [<options>] <action>",
		ਨਿਰਾ
	};
	ਬਣਤਰ option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_ਰਸਤਾ_arg, "path",
			   "path of cache-daemon socket"),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	ਜੇ (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	ਜੇ (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_ਰਸਤਾ = xstrdup_or_null(socket_ਰਸਤਾ_arg);
	ਜੇ (!socket_ਰਸਤਾ)
		socket_ਰਸਤਾ = get_socket_ਰਸਤਾ();
	ਜੇ (!socket_ਰਸਤਾ)
		die("unable to find a suitable socket path; use --socket");

	ਜੇ (!strcmp(op, "exit"))
		do_cache(socket_ਰਸਤਾ, op, timeout, 0);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_ਰਸਤਾ, op, timeout, FLAG_RELAY);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(op, "store"))
		do_cache(socket_ਰਸਤਾ, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(op, "capability"))
		announce_capabilities();
	ਨਹੀਂਤਾਂ
		; /* ignore unknown operation */

	ਮੁਕਤ(socket_ਰਸਤਾ);
	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ

ਪੂਰਨ cmd_credential_cache(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		ਨਿਰਾ
	};
	ਬਣਤਰ option options[] = { OPT_ਸਿਰਾ() };

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#ਜੇ_ਅੰਤ /* NO_UNIX_SOCKETS */
