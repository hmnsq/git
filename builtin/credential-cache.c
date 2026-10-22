#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "credential.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "strbuf.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

#ifndef NO_UNIX_SOCKETS

#ଅନ୍ତର୍ଭାଵ "unix-socket.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"

#define FLAG_SPAWN 0x1
#define FLAG_RELAY 0x2

#ifdef GIT_WINDOWS_NATIVE

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା connection_closed(ପୂର୍ଣ୍ଣସଂଖ୍ୟା error)
{
	ଫେରନ୍ତୁ (error == EINVAL);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା connection_fatally_broken(ପୂର୍ଣ୍ଣସଂଖ୍ୟା error)
{
	ଫେରନ୍ତୁ (error != ENOENT) && (error != ENETDOWN);
}

#ଅନ୍ୟଥା

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା connection_closed(ପୂର୍ଣ୍ଣସଂଖ୍ୟା error)
{
	ଫେରନ୍ତୁ error == ECONNRESET || error == ECONNABORTED;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା connection_fatally_broken(ପୂର୍ଣ୍ଣସଂଖ୍ୟା error)
{
	ଫେରନ୍ତୁ (error != ENOENT) && (error != ECONNREFUSED);
}

#endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା send_request(ଅଚଳ ଅକ୍ଷର *socket, ଅଚଳ ସଂରଚନା strbuf *out)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା got_data = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = unix_stream_connect(socket, 0);

	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ -1;

	ଯଦି (write_in_full(fd, out->buf, out->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	ଯେତେବେଳେ (1) {
		ଅକ୍ଷର in[1024];
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

		r = read_in_full(fd, in, ଆକାର(in));
		ଯଦି (r == 0 || (r < 0 && connection_closed(errno)))
			ଭାଙ୍ଗ;
		ଯଦି (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, in, r);
		got_data = 1;
	}
	close(fd);
	ଫେରନ୍ତୁ got_data;
}

ସ୍ଥିର ଶୂନ୍ୟ spawn_daemon(ଅଚଳ ଅକ୍ଷର *socket)
{
	ସଂରଚନା child_process daemon = CHILD_PROCESS_INIT;
	ଅକ୍ଷର buf[128];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

	strvec_pushl(&daemon.args,
		     "credential-cache--daemon", socket,
		     ଖାଲି);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.out = -1;

	ଯଦି (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_in_full(daemon.out, buf, ଆକାର(buf));
	ଯଦି (r < 0)
		die_errno("unable to read result code from cache daemon");
	ଯଦି (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.out);
}

ସ୍ଥିର ଶୂନ୍ୟ do_cache(ଅଚଳ ଅକ୍ଷର *socket, ଅଚଳ ଅକ୍ଷର *action, ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	ଯଦି (flags & FLAG_RELAY) {
		ଯଦି (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	ଯଦି (send_request(socket, &buf) < 0) {
		ଯଦି (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		ଯଦି (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			ଯଦି (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

ସ୍ଥିର ଅକ୍ଷର *get_socket_path(ଶୂନ୍ୟ)
{
	ସଂରଚନା stat sb;
	ଅକ୍ଷର *old_dir, *socket;
	old_dir = interpolate_path("~/.git-credential-cache", 0);
	ଯଦି (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_mode))
		socket = xstrfmt("%s/socket", old_dir);
	ଅନ୍ୟଥା
		socket = xdg_cache_home("credential/socket");
	ମୁକ୍ତ(old_dir);
	ଫେରନ୍ତୁ socket;
}

ସ୍ଥିର ଶୂନ୍ୟ announce_capabilities(ଶୂନ୍ୟ)
{
	ସଂରଚନା credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_credential_cache(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
			 ଅଚଳ ଅକ୍ଷର **argv,
			 ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *socket_path_arg = ଖାଲି;
	ଅକ୍ଷର *socket_path;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା timeout = 900;
	ଅଚଳ ଅକ୍ଷର *op;
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		"git credential-cache [<options>] <action>",
		ଖାଲି
	};
	ସଂରଚନା option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_path_arg, "path",
			   "path of cache-daemon socket"),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	ଯଦି (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	ଯଦି (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_path = xstrdup_or_null(socket_path_arg);
	ଯଦି (!socket_path)
		socket_path = get_socket_path();
	ଯଦି (!socket_path)
		die("unable to find a suitable socket path; use --socket");

	ଯଦି (!strcmp(op, "exit"))
		do_cache(socket_path, op, timeout, 0);
	ଅନ୍ୟଥା ଯଦି (!strcmp(op, "get") || !strcmp(op, "erase"))
		do_cache(socket_path, op, timeout, FLAG_RELAY);
	ଅନ୍ୟଥା ଯଦି (!strcmp(op, "store"))
		do_cache(socket_path, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	ଅନ୍ୟଥା ଯଦି (!strcmp(op, "capability"))
		announce_capabilities();
	ଅନ୍ୟଥା
		; /* ignore unknown operation */

	ମୁକ୍ତ(socket_path);
	ଫେରନ୍ତୁ 0;
}

#ଅନ୍ୟଥା

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_credential_cache(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
			 ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		ଖାଲି
	};
	ସଂରଚନା option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKETS */
