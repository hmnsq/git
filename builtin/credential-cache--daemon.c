#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "abspath.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NO_UNIX_SOCKETS

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tempfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "credential.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unix-socket.h"

ਬਣਤਰ credential_cache_ਇੰਦਰਾਜ {
	ਬਣਤਰ credential ਵਸਤੂ;
	timestamp_t expiration;
};
ਸਥਿਰ ਬਣਤਰ credential_cache_ਇੰਦਰਾਜ *entries;
ਸਥਿਰ ਪੂਰਨ entries_nr;
ਸਥਿਰ ਪੂਰਨ entries_alloc;

ਸਥਿਰ ਖਾਲੀ cache_credential(ਬਣਤਰ credential *c, ਪੂਰਨ timeout)
{
	ਬਣਤਰ credential_cache_ਇੰਦਰਾਜ *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&e->ਵਸਤੂ, c, ਆਕਾਰ(*c));
	memset(c, 0, ਆਕਾਰ(*c));
	e->expiration = ਸਮਾਂ(ਨਿਰਾ) + timeout;
}

ਸਥਿਰ ਬਣਤਰ credential_cache_ਇੰਦਰਾਜ *lookup_credential(ਅਚਲ ਬਣਤਰ credential *c)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < entries_nr; i++) {
		ਬਣਤਰ credential *e = &entries[i].ਵਸਤੂ;
		ਜੇ (credential_ਮੇਲ(c, e, 0))
			ਵਾਪਸ &entries[i];
	}
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ remove_credential(ਅਚਲ ਬਣਤਰ credential *c, ਪੂਰਨ ਮੇਲ_password)
{
	ਬਣਤਰ credential_cache_ਇੰਦਰਾਜ *e;

	ਪੂਰਨ i;
	ਲਈ (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		ਜੇ (credential_ਮੇਲ(c, &e->ਵਸਤੂ, ਮੇਲ_password))
			e->expiration = 0;
	}
}

ਸਥਿਰ timestamp_t check_expirations(ਖਾਲੀ)
{
	ਸਥਿਰ timestamp_t wait_for_ਇੰਦਰਾਜ_until;
	ਪੂਰਨ i = 0;
	timestamp_t now = ਸਮਾਂ(ਨਿਰਾ);
	timestamp_t ਅਗਲਾ = ਸਮਾਂ_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	ਜੇ (!wait_for_ਇੰਦਰਾਜ_until)
		wait_for_ਇੰਦਰਾਜ_until = now + 30;

	ਜਦੋਂ (i < entries_nr) {
		ਜੇ (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].ਵਸਤੂ);
			ਜੇ (i != entries_nr)
				ਸਮ੍ਰਿਤੀ_ਨਕਲ(&entries[i], &entries[entries_nr], ਆਕਾਰ(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_ਇੰਦਰਾਜ_until = now + 30;
		}
		ਨਹੀਂਤਾਂ {
			ਜੇ (entries[i].expiration < ਅਗਲਾ)
				ਅਗਲਾ = entries[i].expiration;
			i++;
		}
	}

	ਜੇ (!entries_nr) {
		ਜੇ (wait_for_ਇੰਦਰਾਜ_until <= now)
			ਵਾਪਸ 0;
		ਅਗਲਾ = wait_for_ਇੰਦਰਾਜ_until;
	}

	ਵਾਪਸ ਅਗਲਾ - now;
}

ਸਥਿਰ ਪੂਰਨ read_request(FILE *fh, ਬਣਤਰ credential *c,
			ਬਣਤਰ strbuf *action, ਪੂਰਨ *timeout)
{
	ਸਥਿਰ ਬਣਤਰ strbuf ਵਸਤੂ = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *p;

	strbuf_getline_lf(&ਵਸਤੂ, fh);
	ਜੇ (!skip_ਅਗੇਤਰ(ਵਸਤੂ.buf, "action=", &p))
		ਵਾਪਸ error("client sent bogus action line: %s", ਵਸਤੂ.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&ਵਸਤੂ, fh);
	ਜੇ (!skip_ਅਗੇਤਰ(ਵਸਤੂ.buf, "timeout=", &p))
		ਵਾਪਸ error("client sent bogus timeout line: %s", ਵਸਤੂ.buf);
	*timeout = atoi(p);

	credential_ਸੈੱਟ_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	ਜੇ (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		ਵਾਪਸ -1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ serve_one_client(FILE *ਵਿੱਚ, FILE *ਬਾਹਰਲਾ)
{
	ਬਣਤਰ credential c = CREDENTIAL_INIT;
	ਬਣਤਰ strbuf action = STRBUF_INIT;
	ਪੂਰਨ timeout = -1;

	ਜੇ (read_request(ਵਿੱਚ, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(action.buf, "get")) {
		ਬਣਤਰ credential_cache_ਇੰਦਰਾਜ *e = lookup_credential(&c);
		ਜੇ (e) {
			e->ਵਸਤੂ.capa_authtype.request_initial = 1;
			e->ਵਸਤੂ.capa_authtype.request_helper = 1;

			fprintf(ਬਾਹਰਲਾ, "capability[]=authtype\n");
			ਜੇ (e->ਵਸਤੂ.username)
				fprintf(ਬਾਹਰਲਾ, "username=%s\n", e->ਵਸਤੂ.username);
			ਜੇ (e->ਵਸਤੂ.password)
				fprintf(ਬਾਹਰਲਾ, "password=%s\n", e->ਵਸਤੂ.password);
			ਜੇ (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->ਵਸਤੂ.authtype)
				fprintf(ਬਾਹਰਲਾ, "authtype=%s\n", e->ਵਸਤੂ.authtype);
			ਜੇ (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->ਵਸਤੂ.credential)
				fprintf(ਬਾਹਰਲਾ, "credential=%s\n", e->ਵਸਤੂ.credential);
			ਜੇ (e->ਵਸਤੂ.password_expiry_utc != ਸਮਾਂ_MAX)
				fprintf(ਬਾਹਰਲਾ, "password_expiry_utc=%"PRItime"\n",
					e->ਵਸਤੂ.password_expiry_utc);
			ਜੇ (e->ਵਸਤੂ.oauth_refresh_token)
				fprintf(ਬਾਹਰਲਾ, "oauth_refresh_token=%s\n",
					e->ਵਸਤੂ.oauth_refresh_token);
		}
	}
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(action.buf, "exit")) {
		/*
		 * It's important that we clean up our socket first, and then
		 * signal the client only once we have finished the cleanup.
		 * Calling exit() directly does this, because we clean up in
		 * our atexit() handler, and then signal the client when our
		 * process actually ends, which closes the socket and gives
		 * them EOF.
		 */
		ਬਾਹਰ_ਨਿਕਲੋ(0);
	}
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(action.buf, "store")) {
		ਜੇ (timeout < 0)
			warning("cache client didn't specify a timeout");
		ਨਹੀਂਤਾਂ ਜੇ ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		ਨਹੀਂਤਾਂ ਜੇ (c.ephemeral)
			warning("not storing ephemeral credential");
		ਨਹੀਂਤਾਂ {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	ਨਹੀਂਤਾਂ
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

ਸਥਿਰ ਪੂਰਨ serve_cache_loop(ਪੂਰਨ fd)
{
	ਬਣਤਰ pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	ਜੇ (!wakeup)
		ਵਾਪਸ 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	ਜੇ (poll(&pfd, 1, 1000 * wakeup) < 0) {
		ਜੇ (errno != EINTR)
			die_errno("poll failed");
		ਵਾਪਸ 1;
	}

	ਜੇ (pfd.revents & POLLIN) {
		ਪੂਰਨ client, client2;
		FILE *ਵਿੱਚ, *ਬਾਹਰਲਾ;

		client = accept(fd, ਨਿਰਾ, ਨਿਰਾ);
		ਜੇ (client < 0) {
			warning_errno("accept failed");
			ਵਾਪਸ 1;
		}
		client2 = dup(client);
		ਜੇ (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			ਵਾਪਸ 1;
		}

		ਵਿੱਚ = xfdopen(client, "r");
		ਬਾਹਰਲਾ = xfdopen(client2, "w");
		serve_one_client(ਵਿੱਚ, ਬਾਹਰਲਾ);
		fclose(ਵਿੱਚ);
		fclose(ਬਾਹਰਲਾ);
	}
	ਵਾਪਸ 1;
}

ਸਥਿਰ ਖਾਲੀ serve_cache(ਅਚਲ ਅੱਖਰ *socket_ਰਸਤਾ, ਪੂਰਨ debug)
{
	ਬਣਤਰ unix_ਧਾਰਾ_listen_opts opts = UNIX_ਧਾਰਾ_LISTEN_OPTS_INIT;
	ਪੂਰਨ fd;

	fd = unix_ਧਾਰਾ_listen(socket_ਰਸਤਾ, &opts);
	ਜੇ (fd < 0)
		die_errno("unable to bind to '%s'", socket_ਰਸਤਾ);

	ਲਿਖੋ("ok\n");
	fclose(stdout);
	ਜੇ (!debug) {
		ਜੇ (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	ਜਦੋਂ (serve_cache_loop(fd))
		; /* nothing */

	close(fd);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
ਸਥਿਰ ਖਾਲੀ init_socket_directory(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
	ਬਣਤਰ stat st;
	ਅੱਖਰ *ਰਸਤਾ_copy = xstrdup(ਰਸਤਾ);
	ਅੱਖਰ *dir = dirname(ਰਸਤਾ_copy);

	ਜੇ (!stat(dir, &st)) {
		ਜੇ (st.st_ਬਹੁਲਕ & 077)
			die(_(permissions_advice), dir);
	} ਨਹੀਂਤਾਂ {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		ਜੇ (ਸੁਰੱਖਿਅਤ_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		ਜੇ (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	ਜੇ (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	ਮੁਕਤ(ਰਸਤਾ_copy);
}

ਪੂਰਨ cmd_credential_cache_daemon(ਪੂਰਨ argc,
				ਅਚਲ ਅੱਖਰ **argv,
				ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
				ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ tempfile *socket_file;
	ਅਚਲ ਅੱਖਰ *socket_ਰਸਤਾ;
	ਪੂਰਨ ignore_sighup = 0;
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		ਨਿਰਾ
	};
	ਪੂਰਨ debug = 0;
	ਅਚਲ ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_ਸਿਰਾ()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	socket_ਰਸਤਾ = argv[0];

	ਜੇ (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	ਜੇ (!socket_ਰਸਤਾ)
		usage_with_options(usage, options);

	ਜੇ (!is_absolute_ਰਸਤਾ(socket_ਰਸਤਾ))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_ਰਸਤਾ);
	socket_file = ਰਜਿਸਟਰ_tempfile(socket_ਰਸਤਾ);

	ਜੇ (ignore_sighup)
		ਸੰਕੇਤ(SIGHUP, SIG_IGN);

	serve_cache(socket_ਰਸਤਾ, debug);
	delete_tempfile(&socket_file);

	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ

ਪੂਰਨ cmd_credential_cache_daemon(ਪੂਰਨ argc,
ਅਚਲ ਅੱਖਰ **argv,
ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ * ਅਚਲ usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		ਨਿਰਾ
	};
	ਬਣਤਰ option options[] = { OPT_ਸਿਰਾ() };

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#ਜੇ_ਅੰਤ /* NO_UNIX_SOCKET */
