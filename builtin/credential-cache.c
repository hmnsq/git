#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "credential.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "strbuf.h"
#અન્તર્ભાવ "write-or-die.h"

#ifndef NO_UNIX_SOCKETS

#અન્તર્ભાવ "unix-socket.h"
#અન્તર્ભાવ "run-command.h"

#define FLAG_SPAWN 0x1
#define FLAG_RELAY 0x2

#ifdef GIT_WINDOWS_NATIVE

સ્થિર પૂર્ણાંક connection_closed(પૂર્ણાંક error)
{
	પાછા (error == EINVAL);
}

સ્થિર પૂર્ણાંક connection_fatally_broken(પૂર્ણાંક error)
{
	પાછા (error != ENOENT) && (error != ENETDOWN);
}

#નહીંતર

સ્થિર પૂર્ણાંક connection_closed(પૂર્ણાંક error)
{
	પાછા error == ECONNRESET || error == ECONNABORTED;
}

સ્થિર પૂર્ણાંક connection_fatally_broken(પૂર્ણાંક error)
{
	પાછા (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

સ્થિર પૂર્ણાંક send_request(અચલ અક્ષર *socket, અચલ સંરચના strbuf *out)
{
	પૂર્ણાંક got_data = 0;
	પૂર્ણાંક fd = unix_stream_connect(socket, 0);

	જો (fd < 0)
		પાછા -1;

	જો (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	જ્યારે (1) {
		અક્ષર in[1024];
		પૂર્ણાંક r;

		r = read_in_full(fd, in, કદ(in));
		જો (r == 0 || (r < 0 && connection_closed(errno)))
			તોડો;
		જો (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	પાછા got_data;
}

સ્થિર શૂન્ય spawn_daemon(અચલ અક્ષર *socket)
{
	સંરચના child_process daemon = CHILD_PROCESS_INIT;
	અક્ષર buf[128];
	પૂર્ણાંક r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     ખાલી);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	જો (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, કદ(buf));
	જો (r < 0)
		die_errno("unable to read result code from cache daemon");
	જો (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

સ્થિર શૂન્ય do_cache(અચલ અક્ષર *socket, અચલ અક્ષર *action, પૂર્ણાંક timeout,
		     પૂર્ણાંક flags)
{
	સંરચના strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	જો (flags & FLAG_RELAY) {
		જો (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	જો (send_request(socket, &buf) < 0) {
		જો (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		જો (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			જો (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

સ્થિર અક્ષર *get_socket_path(શૂન્ય)
{
	સંરચના stat sb;
	અક્ષર *old_dir, *socket;
	old_dir = interpolate_path("~/.git-credential-cache", 0);
	જો (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	નહીંતર
		socket = xdg_cache_home("credential/socket");
	મુક્ત(old_dir);
	પાછા socket;
}

સ્થિર શૂન્ય announce_capabilities(શૂન્ય)
{
	સંરચના credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

પૂર્ણાંક cmd_credential_cache(પૂર્ણાંક argc,
			 અચલ અક્ષર **argv,
			 અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *socket_path_arg = ખાલી;
	અક્ષર *socket_path;
	પૂર્ણાંક timeout = 900;
	અચલ અક્ષર *op;
	અચલ અક્ષર * અચલ usage[] = {
		"git credential-cache [<options>] <action>",
		ખાલી
	};
	સંરચના option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_path_arg, "path",
			   "path of cache-daemon socket"),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	જો (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	જો (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	જો (!socket_path)
		socket_path = get_socket_path();
	જો (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	જો (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	નહીંતર જો (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	નહીંતર જો (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	નહીંતર જો (!strcmp(op, "capability"))
		announce_capabilities();
	નહીંતર
		; /* ignore unknown operation */

	મુક્ત(socket_path);
	પાછા 0;
}

#નહીંતર

પૂર્ણાંક cmd_credential_cache(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર * અચલ usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		ખાલી
	};
	સંરચના option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKETS */
