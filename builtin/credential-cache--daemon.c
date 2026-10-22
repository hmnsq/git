#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "abspath.h"
#include "gettext.h"
#include "parse-options.h"
#include "path.h"

#ifndef NO_UNIX_SOCKETS

#include "config.h"
#include "tempfile.h"
#include "credential.h"
#include "unix-socket.h"

sa_nracanaa credential_cache_entry {
	sa_nracanaa credential item;
	timestamp_t expiration;
};
static sa_nracanaa credential_cache_entry *entries;
static ཧྲིལ entries_nr;
static ཧྲིལ entries_alloc;

static void cache_credential(sa_nracanaa credential *c, ཧྲིལ timeout)
{
	sa_nracanaa credential_cache_entry *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	memcpy(&e->item, c, parimaa_na(*c));
	memset(c, 0, parimaa_na(*c));
	e->expiration = time(NULL) + timeout;
}

static sa_nracanaa credential_cache_entry *lookup_credential(const sa_nracanaa credential *c)
{
	ཧྲིལ i;
	ཆེད་དུ (i = 0; i < entries_nr; i++) {
		sa_nracanaa credential *e = &entries[i].item;
		གལ་ཏེ (credential_match(c, e, 0))
			ཕྱིར &entries[i];
	}
	ཕྱིར NULL;
}

static void remove_credential(const sa_nracanaa credential *c, ཧྲིལ match_password)
{
	sa_nracanaa credential_cache_entry *e;

	ཧྲིལ i;
	ཆེད་དུ (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		གལ་ཏེ (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

static timestamp_t check_expirations(void)
{
	static timestamp_t wait_for_entry_until;
	ཧྲིལ i = 0;
	timestamp_t now = time(NULL);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	གལ་ཏེ (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	སྐབས (i < entries_nr) {
		གལ་ཏེ (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			གལ་ཏེ (i != entries_nr)
				memcpy(&entries[i], &entries[entries_nr], parimaa_na(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		དེ་མིན {
			གལ་ཏེ (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	གལ་ཏེ (!entries_nr) {
		གལ་ཏེ (wait_for_entry_until <= now)
			ཕྱིར 0;
		next = wait_for_entry_until;
	}

	ཕྱིར next - now;
}

static ཧྲིལ read_request(FILE *fh, sa_nracanaa credential *c,
			sa_nracanaa strbuf *action, ཧྲིལ *timeout)
{
	static sa_nracanaa strbuf item = STRBUF_INIT;
	const var_na *p;

	strbuf_getline_lf(&item, fh);
	གལ་ཏེ (!skip_prefix(item.buf, "action=", &p))
		ཕྱིར error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	གལ་ཏེ (!skip_prefix(item.buf, "timeout=", &p))
		ཕྱིར error("client sent bogus timeout line: %s", item.buf);
	*timeout = atoi(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	གལ་ཏེ (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		ཕྱིར -1;
	ཕྱིར 0;
}

static void serve_one_client(FILE *in, FILE *out)
{
	sa_nracanaa credential c = CREDENTIAL_INIT;
	sa_nracanaa strbuf action = STRBUF_INIT;
	ཧྲིལ timeout = -1;

	གལ་ཏེ (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	དེ་མིན གལ་ཏེ (!strcmp(action.buf, "get")) {
		sa_nracanaa credential_cache_entry *e = lookup_credential(&c);
		གལ་ཏེ (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			fprintf(out, "capability[]=authtype\n");
			གལ་ཏེ (e->item.username)
				fprintf(out, "username=%s\n", e->item.username);
			གལ་ཏེ (e->item.password)
				fprintf(out, "password=%s\n", e->item.password);
			གལ་ཏེ (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				fprintf(out, "authtype=%s\n", e->item.authtype);
			གལ་ཏེ (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				fprintf(out, "credential=%s\n", e->item.credential);
			གལ་ཏེ (e->item.password_expiry_utc != TIME_MAX)
				fprintf(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			གལ་ཏེ (e->item.oauth_refresh_token)
				fprintf(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	དེ་མིན གལ་ཏེ (!strcmp(action.buf, "exit")) {
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
	དེ་མིན གལ་ཏེ (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	དེ་མིན གལ་ཏེ (!strcmp(action.buf, "store")) {
		གལ་ཏེ (timeout < 0)
			warning("cache client didn't specify a timeout");
		དེ་མིན གལ་ཏེ ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		དེ་མིན གལ་ཏེ (c.ephemeral)
			warning("not storing ephemeral credential");
		དེ་མིན {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	དེ་མིན
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

static ཧྲིལ serve_cache_loop(ཧྲིལ fd)
{
	sa_nracanaa pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	གལ་ཏེ (!wakeup)
		ཕྱིར 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	གལ་ཏེ (poll(&pfd, 1, 1000 * wakeup) < 0) {
		གལ་ཏེ (errno != EINTR)
			die_errno("poll failed");
		ཕྱིར 1;
	}

	གལ་ཏེ (pfd.revents & POLLIN) {
		ཧྲིལ client, client2;
		FILE *in, *out;

		client = accept(fd, NULL, NULL);
		གལ་ཏེ (client < 0) {
			warning_errno("accept failed");
			ཕྱིར 1;
		}
		client2 = dup(client);
		གལ་ཏེ (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			ཕྱིར 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		fclose(in);
		fclose(out);
	}
	ཕྱིར 1;
}

static void serve_cache(const var_na *socket_path, ཧྲིལ debug)
{
	sa_nracanaa unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	ཧྲིལ fd;

	fd = unix_stream_listen(socket_path, &opts);
	གལ་ཏེ (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	printf("ok\n");
	fclose(stdout);
	གལ་ཏེ (!debug) {
		གལ་ཏེ (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	སྐབས (serve_cache_loop(fd))
		; /* nothing */

	close(fd);
}

static const var_na permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
static void init_socket_directory(const var_na *path)
{
	sa_nracanaa stat st;
	var_na *path_copy = xstrdup(path);
	var_na *dir = dirname(path_copy);

	གལ་ཏེ (!stat(dir, &st)) {
		གལ་ཏེ (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} དེ་མིན {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		གལ་ཏེ (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		གལ་ཏེ (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	གལ་ཏེ (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	free(path_copy);
}

ཧྲིལ cmd_credential_cache_daemon(ཧྲིལ argc,
				const var_na **argv,
				const var_na *prefix,
				sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa tempfile *socket_file;
	const var_na *socket_path;
	ཧྲིལ ignore_sighup = 0;
	static const var_na *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		NULL
	};
	ཧྲིལ debug = 0;
	const sa_nracanaa option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	གལ་ཏེ (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	གལ་ཏེ (!socket_path)
		usage_with_options(usage, options);

	གལ་ཏེ (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	གལ་ཏེ (ignore_sighup)
		sa_nkeeta(SIGHUP, SIG_IGN);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	ཕྱིར 0;
}

#དེ་མིན

ཧྲིལ cmd_credential_cache_daemon(ཧྲིལ argc,
const var_na **argv,
const var_na *prefix,
sa_nracanaa repository *repo UNUSED)
{
	const var_na * const usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		NULL
	};
	sa_nracanaa option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKET */
