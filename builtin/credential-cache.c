#अन्तर्भाव "builtin.h"
#अन्तर्भाव "credential.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "strbuf.h"
#अन्तर्भाव "write-or-die.h"

#यदि_अपरिभाषितम् NO_UNIX_SOCKETS

#अन्तर्भाव "unix-socket.h"
#अन्तर्भाव "run-command.h"

#परिभाषय FLAG_SPAWN 0x1
#परिभाषय FLAG_RELAY 0x2

#यदि_परिभाषितम् GIT_WINDOWS_NATIVE

स्थिर पूर्णाङ्क connection_closed(पूर्णाङ्क error)
{
	प्रत्यागम (error == EINVAL);
}

स्थिर पूर्णाङ्क connection_fatally_broken(पूर्णाङ्क error)
{
	प्रत्यागम (error != ENOENT) && (error != ENETDOWN);
}

#अन्यथा

स्थिर पूर्णाङ्क connection_closed(पूर्णाङ्क error)
{
	प्रत्यागम error == ECONNRESET || error == ECONNABORTED;
}

स्थिर पूर्णाङ्क connection_fatally_broken(पूर्णाङ्क error)
{
	प्रत्यागम (error != ENOENT) && (error != ECONNREFUSED);
}

#यदि_अन्तः

स्थिर पूर्णाङ्क send_request(ध्रुव वर्ण *socket, ध्रुव संरचना strbuf *बहिर्गमनम्)
{
	पूर्णाङ्क got_दत्त = 0;
	पूर्णाङ्क fd = unix_प्रवाहः_connect(socket, 0);

	यदि (fd < 0)
		प्रत्यागम -1;

	यदि (write_अन्तर्_पूर्णम्(fd, बहिर्गमनम्->buf, बहिर्गमनम्->len) < 0)
		die_errno("unable to write to cache daemon");
	shutdown(fd, SHUT_WR);

	यावत् (1) {
		वर्ण अन्तर्[1024];
		पूर्णाङ्क r;

		r = read_अन्तर्_पूर्णम्(fd, अन्तर्, परिमाण(अन्तर्));
		यदि (r == 0 || (r < 0 && connection_closed(errno)))
			भङ्ग;
		यदि (r < 0)
			die_errno("read error from cache daemon");
		write_or_die(1, अन्तर्, r);
		got_दत्त = 1;
	}
	close(fd);
	प्रत्यागम got_दत्त;
}

स्थिर शून्य spawn_daemon(ध्रुव वर्ण *socket)
{
	संरचना child_process daemon = CHILD_PROCESS_INIT;
	वर्ण buf[128];
	पूर्णाङ्क r;

	strvec_pushl(&daemon.तर्काः,
		     "credential-cache--daemon", socket,
		     रिक्त);
	daemon.git_cmd = 1;
	daemon.no_stdin = 1;
	daemon.बहिर्गमनम् = -1;

	यदि (start_command(&daemon))
		die_errno("unable to start cache daemon");
	r = read_अन्तर्_पूर्णम्(daemon.बहिर्गमनम्, buf, परिमाण(buf));
	यदि (r < 0)
		die_errno("unable to read result code from cache daemon");
	यदि (r != 3 || memcmp(buf, "ok\n", 3))
		die("cache daemon did not start: %.*s", r, buf);

	child_process_clear(&daemon);
	close(daemon.बहिर्गमनम्);
}

स्थिर शून्य do_cache(ध्रुव वर्ण *socket, ध्रुव वर्ण *action, पूर्णाङ्क timeout,
		     पूर्णाङ्क flags)
{
	संरचना strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "action=%s\n", action);
	strbuf_addf(&buf, "timeout=%d\n", timeout);
	यदि (flags & FLAG_RELAY) {
		यदि (strbuf_read(&buf, 0, 0) < 0)
			die_errno("unable to relay credential");
	}

	यदि (send_request(socket, &buf) < 0) {
		यदि (connection_fatally_broken(errno))
			die_errno("unable to connect to cache daemon");
		यदि (flags & FLAG_SPAWN) {
			spawn_daemon(socket);
			यदि (send_request(socket, &buf) < 0)
				die_errno("unable to connect to cache daemon");
		}
	}
	strbuf_release(&buf);
}

स्थिर वर्ण *get_socket_मार्गः(शून्य)
{
	संरचना stat sb;
	वर्ण *old_dir, *socket;
	old_dir = interpolate_मार्गः("~/.git-credential-cache", 0);
	यदि (old_dir && !stat(old_dir, &sb) && S_ISDIR(sb.st_बहुलकम्))
		socket = xstrfmt("%s/socket", old_dir);
	अन्यथा
		socket = xdg_cache_home("credential/socket");
	मुक्त(old_dir);
	प्रत्यागम socket;
}

स्थिर शून्य announce_capabilities(शून्य)
{
	संरचना credential c = CREDENTIAL_INIT;
	c.capa_authtype.request_initial = 1;
	credential_announce_capabilities(&c, stdout);
}

पूर्णाङ्क cmd_credential_cache(पूर्णाङ्क argc,
			 ध्रुव वर्ण **argv,
			 ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण *socket_मार्गः_arg = रिक्त;
	वर्ण *socket_मार्गः;
	पूर्णाङ्क timeout = 900;
	ध्रुव वर्ण *op;
	ध्रुव वर्ण * ध्रुव usage[] = {
		"git credential-cache [<options>] <action>",
		रिक्त
	};
	संरचना option options[] = {
		OPT_INTEGER(0, "timeout", &timeout,
			    "number of seconds to cache credentials"),
		OPT_STRING(0, "socket", &socket_मार्गः_arg, "path",
			   "path of cache-daemon socket"),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	यदि (!argc)
		usage_with_options(usage, options);
	op = argv[0];

	यदि (!have_unix_sockets())
		die(_("credential-cache unavailable; no unix socket support"));

	socket_मार्गः = xstrdup_or_null(socket_मार्गः_arg);
	यदि (!socket_मार्गः)
		socket_मार्गः = get_socket_मार्गः();
	यदि (!socket_मार्गः)
		die("unable to find a suitable socket path; use --socket");

	यदि (!वाक्यं_तुलय(op, "exit"))
		do_cache(socket_मार्गः, op, timeout, 0);
	अन्यथा यदि (!वाक्यं_तुलय(op, "get") || !वाक्यं_तुलय(op, "erase"))
		do_cache(socket_मार्गः, op, timeout, FLAG_RELAY);
	अन्यथा यदि (!वाक्यं_तुलय(op, "store"))
		do_cache(socket_मार्गः, op, timeout, FLAG_RELAY|FLAG_SPAWN);
	अन्यथा यदि (!वाक्यं_तुलय(op, "capability"))
		announce_capabilities();
	अन्यथा
		; /* ignore unknown operation */

	मुक्त(socket_मार्गः);
	प्रत्यागम 0;
}

#अन्यथा

पूर्णाङ्क cmd_credential_cache(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
			 संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण * ध्रुव usage[] = {
		"git credential-cache [options] <action>",
		"",
		"credential-cache is disabled in this build of Git",
		रिक्त
	};
	संरचना option options[] = { OPT_अन्तः() };

	argc = parse_options(argc, argv, उपसर्गः, options, usage, 0);
	die(_("credential-cache unavailable; no unix socket support"));
}

#यदि_अन्तः /* NO_UNIX_SOCKETS */
