#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "abspath.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "path.h"

#اگر_تعریف_نشده NO_UNIX_SOCKETS

#ضمن "config.h"
#ضمن "tempfile.h"
#ضمن "credential.h"
#ضمن "unix-socket.h"

بنية credential_cache_مدخل {
	بنية credential قلم;
	timestamp_t expiration;
};
ساكن بنية credential_cache_مدخل *entries;
ساكن صحيح entries_nr;
ساكن صحيح entries_alloc;

ساكن فراغ cache_credential(بنية credential *c, صحيح timeout)
{
	بنية credential_cache_مدخل *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	کپی_حافظه(&e->قلم, c, حجم(*c));
	memset(c, 0, حجم(*c));
	e->expiration = زمان(عدم) + timeout;
}

ساكن بنية credential_cache_مدخل *lookup_credential(ثابت بنية credential *c)
{
	صحيح i;
	لكل (i = 0; i < entries_nr; i++) {
		بنية credential *e = &entries[i].قلم;
		إذا (credential_تطابق(c, e, 0))
			أرجع &entries[i];
	}
	أرجع عدم;
}

ساكن فراغ remove_credential(ثابت بنية credential *c, صحيح تطابق_password)
{
	بنية credential_cache_مدخل *e;

	صحيح i;
	لكل (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		إذا (credential_تطابق(c, &e->قلم, تطابق_password))
			e->expiration = 0;
	}
}

ساكن timestamp_t check_expirations(فراغ)
{
	ساكن timestamp_t wait_for_مدخل_until;
	صحيح i = 0;
	timestamp_t now = زمان(عدم);
	timestamp_t بعدی = زمان_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	إذا (!wait_for_مدخل_until)
		wait_for_مدخل_until = now + 30;

	طالما (i < entries_nr) {
		إذا (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].قلم);
			إذا (i != entries_nr)
				کپی_حافظه(&entries[i], &entries[entries_nr], حجم(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_مدخل_until = now + 30;
		}
		وإلا {
			إذا (entries[i].expiration < بعدی)
				بعدی = entries[i].expiration;
			i++;
		}
	}

	إذا (!entries_nr) {
		إذا (wait_for_مدخل_until <= now)
			أرجع 0;
		بعدی = wait_for_مدخل_until;
	}

	أرجع بعدی - now;
}

ساكن صحيح read_request(FILE *fh, بنية credential *c,
			بنية strbuf *action, صحيح *timeout)
{
	ساكن بنية strbuf قلم = STRBUF_INIT;
	ثابت حرف *p;

	strbuf_getline_lf(&قلم, fh);
	إذا (!skip_پیشوند(قلم.buf, "action=", &p))
		أرجع error("client sent bogus action line: %s", قلم.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&قلم, fh);
	إذا (!skip_پیشوند(قلم.buf, "timeout=", &p))
		أرجع error("client sent bogus timeout line: %s", قلم.buf);
	*timeout = atoi(p);

	credential_مجموعه_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	إذا (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		أرجع -1;
	أرجع 0;
}

ساكن فراغ serve_one_client(FILE *درون, FILE *بیرون)
{
	بنية credential c = CREDENTIAL_INIT;
	بنية strbuf action = STRBUF_INIT;
	صحيح timeout = -1;

	إذا (read_request(درون, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	وإلا إذا (!strcmp(action.buf, "get")) {
		بنية credential_cache_مدخل *e = lookup_credential(&c);
		إذا (e) {
			e->قلم.capa_authtype.request_initial = 1;
			e->قلم.capa_authtype.request_helper = 1;

			fprintf(بیرون, "capability[]=authtype\n");
			إذا (e->قلم.username)
				fprintf(بیرون, "username=%s\n", e->قلم.username);
			إذا (e->قلم.password)
				fprintf(بیرون, "password=%s\n", e->قلم.password);
			إذا (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->قلم.authtype)
				fprintf(بیرون, "authtype=%s\n", e->قلم.authtype);
			إذا (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->قلم.credential)
				fprintf(بیرون, "credential=%s\n", e->قلم.credential);
			إذا (e->قلم.password_expiry_utc != زمان_MAX)
				fprintf(بیرون, "password_expiry_utc=%"PRItime"\n",
					e->قلم.password_expiry_utc);
			إذا (e->قلم.oauth_refresh_token)
				fprintf(بیرون, "oauth_refresh_token=%s\n",
					e->قلم.oauth_refresh_token);
		}
	}
	وإلا إذا (!strcmp(action.buf, "exit")) {
		/*
		 * It's important that we clean up our socket first, and then
		 * signal the client only once we have finished the cleanup.
		 * Calling exit() directly does this, because we clean up in
		 * our atexit() handler, and then signal the client when our
		 * process actually ends, which closes the socket and gives
		 * them EOF.
		 */
		خروج(0);
	}
	وإلا إذا (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	وإلا إذا (!strcmp(action.buf, "store")) {
		إذا (timeout < 0)
			warning("cache client didn't specify a timeout");
		وإلا إذا ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		وإلا إذا (c.ephemeral)
			warning("not storing ephemeral credential");
		وإلا {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	وإلا
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

ساكن صحيح serve_cache_loop(صحيح fd)
{
	بنية pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	إذا (!wakeup)
		أرجع 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	إذا (poll(&pfd, 1, 1000 * wakeup) < 0) {
		إذا (errno != EINTR)
			die_errno("poll failed");
		أرجع 1;
	}

	إذا (pfd.revents & POLLIN) {
		صحيح client, client2;
		FILE *درون, *بیرون;

		client = accept(fd, عدم, عدم);
		إذا (client < 0) {
			warning_errno("accept failed");
			أرجع 1;
		}
		client2 = dup(client);
		إذا (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			أرجع 1;
		}

		درون = xfdopen(client, "r");
		بیرون = xfdopen(client2, "w");
		serve_one_client(درون, بیرون);
		fclose(درون);
		fclose(بیرون);
	}
	أرجع 1;
}

ساكن فراغ serve_cache(ثابت حرف *socket_مسیر, صحيح debug)
{
	بنية unix_جریان_listen_opts opts = UNIX_جریان_LISTEN_OPTS_INIT;
	صحيح fd;

	fd = unix_جریان_listen(socket_مسیر, &opts);
	إذا (fd < 0)
		die_errno("unable to bind to '%s'", socket_مسیر);

	چاپ_کن("ok\n");
	fclose(stdout);
	إذا (!debug) {
		إذا (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	طالما (serve_cache_loop(fd))
		; /* nothing */

	close(fd);
}

ساكن ثابت حرف permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
ساكن فراغ init_socket_directory(ثابت حرف *مسیر)
{
	بنية stat st;
	حرف *مسیر_copy = xstrdup(مسیر);
	حرف *dir = dirname(مسیر_copy);

	إذا (!stat(dir, &st)) {
		إذا (st.st_نما & 077)
			die(_(permissions_advice), dir);
	} وإلا {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		إذا (امن_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		إذا (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	إذا (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	حرر(مسیر_copy);
}

صحيح cmd_credential_cache_daemon(صحيح argc,
				ثابت حرف **argv,
				ثابت حرف *پیشوند,
				بنية repository *repo استفادهنشده)
{
	بنية tempfile *socket_file;
	ثابت حرف *socket_مسیر;
	صحيح ignore_sighup = 0;
	ساكن ثابت حرف *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		عدم
	};
	صحيح debug = 0;
	ثابت بنية option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_انتها()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	socket_مسیر = argv[0];

	إذا (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	إذا (!socket_مسیر)
		usage_with_options(usage, options);

	إذا (!is_absolute_مسیر(socket_مسیر))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_مسیر);
	socket_file = ثبات_tempfile(socket_مسیر);

	إذا (ignore_sighup)
		إشارة(SIGHUP, SIG_IGN);

	serve_cache(socket_مسیر, debug);
	delete_tempfile(&socket_file);

	أرجع 0;
}

#وإلا

صحيح cmd_credential_cache_daemon(صحيح argc,
ثابت حرف **argv,
ثابت حرف *پیشوند,
بنية repository *repo استفادهنشده)
{
	ثابت حرف * ثابت usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		عدم
	};
	بنية option options[] = { OPT_انتها() };

	argc = parse_options(argc, argv, پیشوند, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#پایان_اگر /* NO_UNIX_SOCKET */
