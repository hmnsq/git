#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "abspath.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"

#ifndef NO_UNIX_SOCKETS

#अन्तर्भाव "config.h"
#अन्तर्भाव "tempfile.h"
#अन्तर्भाव "credential.h"
#अन्तर्भाव "unix-socket.h"

संरचना credential_cache_entry {
	संरचना credential item;
	timestamp_t expiration;
};
स्थिर संरचना credential_cache_entry *entries;
स्थिर पूर्णांक entries_nr;
स्थिर पूर्णांक entries_alloc;

स्थिर शून्य cache_credential(संरचना credential *c, पूर्णांक timeout)
{
	संरचना credential_cache_entry *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	स_नकल(&e->item, c, आकार(*c));
	स_रखो(c, 0, आकार(*c));
	e->expiration = समय(रिक्त) + timeout;
}

स्थिर संरचना credential_cache_entry *lookup_credential(स्थिरांक संरचना credential *c)
{
	पूर्णांक i;
	साठी (i = 0; i < entries_nr; i++) {
		संरचना credential *e = &entries[i].item;
		जर (credential_match(c, e, 0))
			परत &entries[i];
	}
	परत रिक्त;
}

स्थिर शून्य remove_credential(स्थिरांक संरचना credential *c, पूर्णांक match_password)
{
	संरचना credential_cache_entry *e;

	पूर्णांक i;
	साठी (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		जर (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

स्थिर timestamp_t check_expirations(शून्य)
{
	स्थिर timestamp_t wait_for_entry_until;
	पूर्णांक i = 0;
	timestamp_t now = समय(रिक्त);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	जर (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	पर्यंत (i < entries_nr) {
		जर (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			जर (i != entries_nr)
				स_नकल(&entries[i], &entries[entries_nr], आकार(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		अन्यथा {
			जर (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	जर (!entries_nr) {
		जर (wait_for_entry_until <= now)
			परत 0;
		next = wait_for_entry_until;
	}

	परत next - now;
}

स्थिर पूर्णांक read_request(खाता *fh, संरचना credential *c,
			संरचना strbuf *action, पूर्णांक *timeout)
{
	स्थिर संरचना strbuf item = STRBUF_INIT;
	स्थिरांक अक्षर *p;

	strbuf_getline_lf(&item, fh);
	जर (!skip_prefix(item.buf, "action=", &p))
		परत error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	जर (!skip_prefix(item.buf, "timeout=", &p))
		परत error("client sent bogus timeout line: %s", item.buf);
	*timeout = म_से_प(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	जर (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		परत -1;
	परत 0;
}

स्थिर शून्य serve_one_client(खाता *in, खाता *out)
{
	संरचना credential c = CREDENTIAL_INIT;
	संरचना strbuf action = STRBUF_INIT;
	पूर्णांक timeout = -1;

	जर (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	अन्यथा जर (!म_भेद(action.buf, "get")) {
		संरचना credential_cache_entry *e = lookup_credential(&c);
		जर (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			ख_लिखो(out, "capability[]=authtype\n");
			जर (e->item.username)
				ख_लिखो(out, "username=%s\n", e->item.username);
			जर (e->item.password)
				ख_लिखो(out, "password=%s\n", e->item.password);
			जर (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				ख_लिखो(out, "authtype=%s\n", e->item.authtype);
			जर (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				ख_लिखो(out, "credential=%s\n", e->item.credential);
			जर (e->item.password_expiry_utc != TIME_MAX)
				ख_लिखो(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			जर (e->item.oauth_refresh_token)
				ख_लिखो(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	अन्यथा जर (!म_भेद(action.buf, "exit")) {
		/*
		 * It's important that we clean up our socket first, and then
		 * signal the client only once we have finished the cleanup.
		 * Calling exit() directly does this, because we clean up in
		 * our atexit() handler, and then signal the client when our
		 * process actually ends, which closes the socket and gives
		 * them EOF.
		 */
		निकास(0);
	}
	अन्यथा जर (!म_भेद(action.buf, "erase"))
		remove_credential(&c, 1);
	अन्यथा जर (!म_भेद(action.buf, "store")) {
		जर (timeout < 0)
			warning("cache client didn't specify a timeout");
		अन्यथा जर ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		अन्यथा जर (c.ephemeral)
			warning("not storing ephemeral credential");
		अन्यथा {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	अन्यथा
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

स्थिर पूर्णांक serve_cache_loop(पूर्णांक fd)
{
	संरचना pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	जर (!wakeup)
		परत 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	जर (poll(&pfd, 1, 1000 * wakeup) < 0) {
		जर (त्रुटि_सं != EINTR)
			die_errno("poll failed");
		परत 1;
	}

	जर (pfd.revents & POLLIN) {
		पूर्णांक client, client2;
		खाता *in, *out;

		client = accept(fd, रिक्त, रिक्त);
		जर (client < 0) {
			warning_errno("accept failed");
			परत 1;
		}
		client2 = dup(client);
		जर (client2 < 0) {
			warning_errno("dup failed");
			बंद(client);
			परत 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		ख_बंद(in);
		ख_बंद(out);
	}
	परत 1;
}

स्थिर शून्य serve_cache(स्थिरांक अक्षर *socket_path, पूर्णांक debug)
{
	संरचना unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	पूर्णांक fd;

	fd = unix_stream_listen(socket_path, &opts);
	जर (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	म_लिखो("ok\n");
	ख_बंद(मानक_निकास);
	जर (!debug) {
		जर (!ख_व_खोलो("/dev/null", "w", मानक_त्रुटि))
			die_errno("unable to point stderr to /dev/null");
	}

	पर्यंत (serve_cache_loop(fd))
		; /* nothing */

	बंद(fd);
}

स्थिर स्थिरांक अक्षर permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
स्थिर शून्य init_socket_directory(स्थिरांक अक्षर *path)
{
	संरचना stat st;
	अक्षर *path_copy = xstrdup(path);
	अक्षर *dir = स_नाम(path_copy);

	जर (!stat(dir, &st)) {
		जर (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} अन्यथा {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		जर (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		जर (सूची_गढ़ो(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	जर (स_बदलो(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	मुक्त(path_copy);
}

पूर्णांक cmd_credential_cache_daemon(पूर्णांक argc,
				स्थिरांक अक्षर **argv,
				स्थिरांक अक्षर *prefix,
				संरचना repository *repo UNUSED)
{
	संरचना tempfile *socket_file;
	स्थिरांक अक्षर *socket_path;
	पूर्णांक ignore_sighup = 0;
	स्थिर स्थिरांक अक्षर *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		रिक्त
	};
	पूर्णांक debug = 0;
	स्थिरांक संरचना option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	जर (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	जर (!socket_path)
		usage_with_options(usage, options);

	जर (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	जर (ignore_sighup)
		संकेत(SIGHUP, संक_छोड़ो);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	परत 0;
}

#अन्यथा

पूर्णांक cmd_credential_cache_daemon(पूर्णांक argc,
स्थिरांक अक्षर **argv,
स्थिरांक अक्षर *prefix,
संरचना repository *repo UNUSED)
{
	स्थिरांक अक्षर * स्थिरांक usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		रिक्त
	};
	संरचना option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKET */
