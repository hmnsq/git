#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "abspath.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "path.h"

#ifndef NO_UNIX_SOCKETS

#شامل "config.h"
#شامل "tempfile.h"
#شامل "credential.h"
#شامل "unix-socket.h"

ساختار credential_cache_entry {
	ساختار credential item;
	timestamp_t expiration;
};
ایستا ساختار credential_cache_entry *entries;
ایستا صحیح entries_nr;
ایستا صحیح entries_alloc;

ایستا تهی cache_credential(ساختار credential *c, صحیح timeout)
{
	ساختار credential_cache_entry *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	memcpy(&e->item, c, اندازه(*c));
	memset(c, 0, اندازه(*c));
	e->expiration = time(پوچ) + timeout;
}

ایستا ساختار credential_cache_entry *lookup_credential(ثابت ساختار credential *c)
{
	صحیح i;
	برای (i = 0; i < entries_nr; i++) {
		ساختار credential *e = &entries[i].item;
		اگر (credential_match(c, e, 0))
			بازگشت &entries[i];
	}
	بازگشت پوچ;
}

ایستا تهی remove_credential(ثابت ساختار credential *c, صحیح match_password)
{
	ساختار credential_cache_entry *e;

	صحیح i;
	برای (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		اگر (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

ایستا timestamp_t check_expirations(تهی)
{
	ایستا timestamp_t wait_for_entry_until;
	صحیح i = 0;
	timestamp_t now = time(پوچ);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	اگر (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	وقتی (i < entries_nr) {
		اگر (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			اگر (i != entries_nr)
				memcpy(&entries[i], &entries[entries_nr], اندازه(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		وگرنه {
			اگر (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	اگر (!entries_nr) {
		اگر (wait_for_entry_until <= now)
			بازگشت 0;
		next = wait_for_entry_until;
	}

	بازگشت next - now;
}

ایستا صحیح read_request(FILE *fh, ساختار credential *c,
			ساختار strbuf *action, صحیح *timeout)
{
	ایستا ساختار strbuf item = STRBUF_INIT;
	ثابت نویسه *p;

	strbuf_getline_lf(&item, fh);
	اگر (!skip_prefix(item.buf, "action=", &p))
		بازگشت error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	اگر (!skip_prefix(item.buf, "timeout=", &p))
		بازگشت error("client sent bogus timeout line: %s", item.buf);
	*timeout = atoi(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	اگر (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		بازگشت -1;
	بازگشت 0;
}

ایستا تهی serve_one_client(FILE *in, FILE *out)
{
	ساختار credential c = CREDENTIAL_INIT;
	ساختار strbuf action = STRBUF_INIT;
	صحیح timeout = -1;

	اگر (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	وگرنه اگر (!strcmp(action.buf, "get")) {
		ساختار credential_cache_entry *e = lookup_credential(&c);
		اگر (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			fprintf(out, "capability[]=authtype\n");
			اگر (e->item.username)
				fprintf(out, "username=%s\n", e->item.username);
			اگر (e->item.password)
				fprintf(out, "password=%s\n", e->item.password);
			اگر (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				fprintf(out, "authtype=%s\n", e->item.authtype);
			اگر (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				fprintf(out, "credential=%s\n", e->item.credential);
			اگر (e->item.password_expiry_utc != TIME_MAX)
				fprintf(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			اگر (e->item.oauth_refresh_token)
				fprintf(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	وگرنه اگر (!strcmp(action.buf, "exit")) {
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
	وگرنه اگر (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	وگرنه اگر (!strcmp(action.buf, "store")) {
		اگر (timeout < 0)
			warning("cache client didn't specify a timeout");
		وگرنه اگر ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		وگرنه اگر (c.ephemeral)
			warning("not storing ephemeral credential");
		وگرنه {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	وگرنه
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

ایستا صحیح serve_cache_loop(صحیح fd)
{
	ساختار pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	اگر (!wakeup)
		بازگشت 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	اگر (poll(&pfd, 1, 1000 * wakeup) < 0) {
		اگر (errno != EINTR)
			die_errno("poll failed");
		بازگشت 1;
	}

	اگر (pfd.revents & POLLIN) {
		صحیح client, client2;
		FILE *in, *out;

		client = accept(fd, پوچ, پوچ);
		اگر (client < 0) {
			warning_errno("accept failed");
			بازگشت 1;
		}
		client2 = dup(client);
		اگر (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			بازگشت 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		fclose(in);
		fclose(out);
	}
	بازگشت 1;
}

ایستا تهی serve_cache(ثابت نویسه *socket_path, صحیح debug)
{
	ساختار unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	صحیح fd;

	fd = unix_stream_listen(socket_path, &opts);
	اگر (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	printf("ok\n");
	fclose(stdout);
	اگر (!debug) {
		اگر (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	وقتی (serve_cache_loop(fd))
		; /* nothing */

	close(fd);
}

ایستا ثابت نویسه permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
ایستا تهی init_socket_directory(ثابت نویسه *path)
{
	ساختار stat st;
	نویسه *path_copy = xstrdup(path);
	نویسه *dir = dirname(path_copy);

	اگر (!stat(dir, &st)) {
		اگر (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} وگرنه {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		اگر (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		اگر (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	اگر (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	آزاد(path_copy);
}

صحیح cmd_credential_cache_daemon(صحیح argc,
				ثابت نویسه **argv,
				ثابت نویسه *prefix,
				ساختار repository *repo UNUSED)
{
	ساختار tempfile *socket_file;
	ثابت نویسه *socket_path;
	صحیح ignore_sighup = 0;
	ایستا ثابت نویسه *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		پوچ
	};
	صحیح debug = 0;
	ثابت ساختار option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	اگر (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	اگر (!socket_path)
		usage_with_options(usage, options);

	اگر (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	اگر (ignore_sighup)
		سیگنال(SIGHUP, SIG_IGN);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	بازگشت 0;
}

#وگرنه

صحیح cmd_credential_cache_daemon(صحیح argc,
ثابت نویسه **argv,
ثابت نویسه *prefix,
ساختار repository *repo UNUSED)
{
	ثابت نویسه * ثابت usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		پوچ
	};
	ساختار option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKET */
