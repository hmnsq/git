#شامل "builtin.h"
#شامل "credential.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "strbuf.h"
#شامل "write-or-die.h"

#ifndef NO_UNIX_SOCKETS

#شامل "unix-socket.h"
#شامل "run-command.h"

#define FLAG_SPAWN 0x1
#define FLAG_RELAY 0x2

#ifdef GIT_WINDOWS_NATIVE

ایستا صحیح connection_closed(صحیح error)
{
	بازگشت (error == EINVAL);
}

ایستا صحیح connection_fatally_broken(صحیح error)
{
	بازگشت (error != ENOENT) && (error != ENETDOWN);
}

#وگرنه

ایستا صحیح connection_closed(صحیح error)
{
	بازگشت error == ECONNRESET || error == ECONNABORTED;
}

ایستا صحیح connection_fatally_broken(صحیح error)
{
	بازگشت (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

ایستا صحیح send_request(ثابت نویسه *socket, ثابت ساختار strbuf *out)
{
	صحیح got_data = 0;
	صحیح fd = unix_stream_connect(socket, 0);

	اگر (fd < 0)
		بازگشت -1;

	اگر (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	وقتی (1) {
		نویسه in[1024];
		صحیح r;

		r = read_in_full(fd, in, اندازه(in));
		اگر (r == 0 || (r < 0 && connection_closed(errno)))
			بشکن;
		اگر (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	بازگشت got_data;
}

ایستا تهی spawn_daemon(ثابت نویسه *socket)
{
	ساختار child_process daemon = CHILD_PROCESS_INIT;
	نویسه buf[128];
	صحیح r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     پوچ);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	اگر (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, اندازه(buf));
	اگر (r < 0)
		die_errno("unable to read result code from cache daemon");
	اگر (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

ایستا تهی do_cache(ثابت نویسه *socket, ثابت نویسه *action, صحیح timeout,
		     صحیح flags)
{
	ساختار strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	اگر (flags & FLAG_RELAY) {
		اگر (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	اگر (send_request(socket, &buf) < 0) {
		اگر (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		اگر (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			اگر (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

ایستا نویسه *get_socket_path(تهی)
{
	ساختار stat sb;
	نویسه *old_dir, *socket;
	old_dir = interpolate_path("~/.git-credential-cache", 0);
	اگر (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	وگرنه
		socket = xdg_cache_home("credential/socket");
	آزاد(old_dir);
	بازگشت socket;
}

ایستا تهی announce_capabilities(تهی)
{
	ساختار credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

صحیح cmd_credential_cache(صحیح argc,
			 ثابت نویسه **argv,
			 ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ثابت نویسه *socket_path_arg = پوچ;
	نویسه *socket_path;
	صحیح timeout = 900;
	ثابت نویسه *op;
	ثابت نویسه * ثابت usage[] = {
		"git credential-cache [<options>] <action>",
		پوچ
	};
	ساختار option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_path_arg, "path",
			   "path of cache-daemon socket"),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	اگر (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	اگر (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	اگر (!socket_path)
		socket_path = get_socket_path();
	اگر (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	اگر (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	وگرنه اگر (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	وگرنه اگر (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	وگرنه اگر (!strcmp(op, "capability"))
		announce_capabilities();
	وگرنه
		; /* ignore unknown operation */

	آزاد(socket_path);
	بازگشت 0;
}

#وگرنه

صحیح cmd_credential_cache(صحیح argc, ثابت نویسه **argv, ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ثابت نویسه * ثابت usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		پوچ
	};
	ساختار option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKETS */
