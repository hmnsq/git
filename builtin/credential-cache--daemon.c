#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "abspath.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"

#ifndef NO_UNIX_SOCKETS

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "tempfile.h"
#અન્તર્ભાવ "credential.h"
#અન્તર્ભાવ "unix-socket.h"

સંરચના credential_cache_entry {
	સંરચના credential item;
	timestamp_t expiration;
};
સ્થિર સંરચના credential_cache_entry *entries;
સ્થિર પૂર્ણાંક entries_nr;
સ્થિર પૂર્ણાંક entries_alloc;

સ્થિર શૂન્ય cache_credential(સંરચના credential *c, પૂર્ણાંક timeout)
{
	સંરચના credential_cache_entry *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	memcpy(&e->item, c, કદ(*c));
	memset(c, 0, કદ(*c));
	e->expiration = time(ખાલી) + timeout;
}

સ્થિર સંરચના credential_cache_entry *lookup_credential(અચલ સંરચના credential *c)
{
	પૂર્ણાંક i;
	માટે (i = 0; i < entries_nr; i++) {
		સંરચના credential *e = &entries[i].item;
		જો (credential_match(c, e, 0))
			પાછા &entries[i];
	}
	પાછા ખાલી;
}

સ્થિર શૂન્ય remove_credential(અચલ સંરચના credential *c, પૂર્ણાંક match_password)
{
	સંરચના credential_cache_entry *e;

	પૂર્ણાંક i;
	માટે (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		જો (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

સ્થિર timestamp_t check_expirations(શૂન્ય)
{
	સ્થિર timestamp_t wait_for_entry_until;
	પૂર્ણાંક i = 0;
	timestamp_t now = time(ખાલી);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	જો (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	જ્યારે (i < entries_nr) {
		જો (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			જો (i != entries_nr)
				memcpy(&entries[i], &entries[entries_nr], કદ(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		નહીંતર {
			જો (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	જો (!entries_nr) {
		જો (wait_for_entry_until <= now)
			પાછા 0;
		next = wait_for_entry_until;
	}

	પાછા next - now;
}

સ્થિર પૂર્ણાંક read_request(FILE *fh, સંરચના credential *c,
			સંરચના strbuf *action, પૂર્ણાંક *timeout)
{
	સ્થિર સંરચના strbuf item = STRBUF_INIT;
	અચલ અક્ષર *p;

	strbuf_getline_lf(&item, fh);
	જો (!skip_prefix(item.buf, "action=", &p))
		પાછા error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	જો (!skip_prefix(item.buf, "timeout=", &p))
		પાછા error("client sent bogus timeout line: %s", item.buf);
	*timeout = atoi(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	જો (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		પાછા -1;
	પાછા 0;
}

સ્થિર શૂન્ય serve_one_client(FILE *in, FILE *out)
{
	સંરચના credential c = CREDENTIAL_INIT;
	સંરચના strbuf action = STRBUF_INIT;
	પૂર્ણાંક timeout = -1;

	જો (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	નહીંતર જો (!strcmp(action.buf, "get")) {
		સંરચના credential_cache_entry *e = lookup_credential(&c);
		જો (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			fprintf(out, "capability[]=authtype\n");
			જો (e->item.username)
				fprintf(out, "username=%s\n", e->item.username);
			જો (e->item.password)
				fprintf(out, "password=%s\n", e->item.password);
			જો (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				fprintf(out, "authtype=%s\n", e->item.authtype);
			જો (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				fprintf(out, "credential=%s\n", e->item.credential);
			જો (e->item.password_expiry_utc != TIME_MAX)
				fprintf(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			જો (e->item.oauth_refresh_token)
				fprintf(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	નહીંતર જો (!strcmp(action.buf, "exit")) {
		/*
		 * It's important that we clean up our socket first, and then
		 * signal the client only once we have finished the cleanup.
		 * Calling exit() directly does this, because we clean up in
		 * our atexit() handler, and then signal the client when our
		 * process actually ends, which closes the socket and gives
		 * them EOF.
		 */
		exit(0);
	}
	નહીંતર જો (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	નહીંતર જો (!strcmp(action.buf, "store")) {
		જો (timeout < 0)
			warning("cache client didn't specify a timeout");
		નહીંતર જો ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		નહીંતર જો (c.ephemeral)
			warning("not storing ephemeral credential");
		નહીંતર {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	નહીંતર
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

સ્થિર પૂર્ણાંક serve_cache_loop(પૂર્ણાંક fd)
{
	સંરચના pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	જો (!wakeup)
		પાછા 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	જો (poll(&pfd, 1, 1000 * wakeup) < 0) {
		જો (errno != EINTR)
			die_errno("poll failed");
		પાછા 1;
	}

	જો (pfd.revents & POLLIN) {
		પૂર્ણાંક client, client2;
		FILE *in, *out;

		client = accept(fd, ખાલી, ખાલી);
		જો (client < 0) {
			warning_errno("accept failed");
			પાછા 1;
		}
		client2 = dup(client);
		જો (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			પાછા 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		fclose(in);
		fclose(out);
	}
	પાછા 1;
}

સ્થિર શૂન્ય serve_cache(અચલ અક્ષર *socket_path, પૂર્ણાંક debug)
{
	સંરચના unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	પૂર્ણાંક fd;

	fd = unix_stream_listen(socket_path, &opts);
	જો (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	printf("ok\n");
	fclose(stdout);
	જો (!debug) {
		જો (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	જ્યારે (serve_cache_loop(fd))
		; /* nothing */

	close(fd);
}

સ્થિર અચલ અક્ષર permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
સ્થિર શૂન્ય init_socket_directory(અચલ અક્ષર *path)
{
	સંરચના stat st;
	અક્ષર *path_copy = xstrdup(path);
	અક્ષર *dir = dirname(path_copy);

	જો (!stat(dir, &st)) {
		જો (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} નહીંતર {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		જો (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		જો (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	જો (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	મુક્ત(path_copy);
}

પૂર્ણાંક cmd_credential_cache_daemon(પૂર્ણાંક argc,
				અચલ અક્ષર **argv,
				અચલ અક્ષર *prefix,
				સંરચના repository *repo UNUSED)
{
	સંરચના tempfile *socket_file;
	અચલ અક્ષર *socket_path;
	પૂર્ણાંક ignore_sighup = 0;
	સ્થિર અચલ અક્ષર *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		ખાલી
	};
	પૂર્ણાંક debug = 0;
	અચલ સંરચના option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	જો (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	જો (!socket_path)
		usage_with_options(usage, options);

	જો (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	જો (ignore_sighup)
		સંકેત(SIGHUP, SIG_IGN);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	પાછા 0;
}

#નહીંતર

પૂર્ણાંક cmd_credential_cache_daemon(પૂર્ણાંક argc,
અચલ અક્ષર **argv,
અચલ અક્ષર *prefix,
સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર * અચલ usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		ખાલી
	};
	સંરચના option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKET */
