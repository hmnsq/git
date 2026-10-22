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

struct credential_cache_entry {
	struct credential item;
	timestamp_t expiration;
};
static struct credential_cache_entry *entries;
static 整数 entries_nr;
static 整数 entries_alloc;

static void cache_credential(struct credential *c, 整数 timeout)
{
	struct credential_cache_entry *e;

	ALLOC_GROW(entries, entries_nr + 1, entries_alloc);
	e = &entries[entries_nr++];

	/* take ownership of pointers */
	memcpy(&e->item, c, sizeof(*c));
	memset(c, 0, sizeof(*c));
	e->expiration = time(NULL) + timeout;
}

static struct credential_cache_entry *lookup_credential(const struct credential *c)
{
	整数 i;
	ために (i = 0; i < entries_nr; i++) {
		struct credential *e = &entries[i].item;
		もし (credential_match(c, e, 0))
			戻る &entries[i];
	}
	戻る NULL;
}

static void remove_credential(const struct credential *c, 整数 match_password)
{
	struct credential_cache_entry *e;

	整数 i;
	ために (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		もし (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

static timestamp_t check_expirations(void)
{
	static timestamp_t wait_for_entry_until;
	整数 i = 0;
	timestamp_t now = time(NULL);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	もし (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	ながら (i < entries_nr) {
		もし (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			もし (i != entries_nr)
				memcpy(&entries[i], &entries[entries_nr], sizeof(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		否则 {
			もし (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	もし (!entries_nr) {
		もし (wait_for_entry_until <= now)
			戻る 0;
		next = wait_for_entry_until;
	}

	戻る next - now;
}

static 整数 read_request(FILE *fh, struct credential *c,
			struct strbuf *action, 整数 *timeout)
{
	static struct strbuf item = STRBUF_INIT;
	const char *p;

	strbuf_getline_lf(&item, fh);
	もし (!skip_prefix(item.buf, "action=", &p))
		戻る error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	もし (!skip_prefix(item.buf, "timeout=", &p))
		戻る error("client sent bogus timeout line: %s", item.buf);
	*timeout = atoi(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	もし (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		戻る -1;
	戻る 0;
}

static void serve_one_client(FILE *in, FILE *out)
{
	struct credential c = CREDENTIAL_INIT;
	struct strbuf action = STRBUF_INIT;
	整数 timeout = -1;

	もし (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	否则 もし (!strcmp(action.buf, "get")) {
		struct credential_cache_entry *e = lookup_credential(&c);
		もし (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			fprintf(out, "capability[]=authtype\n");
			もし (e->item.username)
				fprintf(out, "username=%s\n", e->item.username);
			もし (e->item.password)
				fprintf(out, "password=%s\n", e->item.password);
			もし (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				fprintf(out, "authtype=%s\n", e->item.authtype);
			もし (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				fprintf(out, "credential=%s\n", e->item.credential);
			もし (e->item.password_expiry_utc != TIME_MAX)
				fprintf(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			もし (e->item.oauth_refresh_token)
				fprintf(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	否则 もし (!strcmp(action.buf, "exit")) {
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
	否则 もし (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	否则 もし (!strcmp(action.buf, "store")) {
		もし (timeout < 0)
			warning("cache client didn't specify a timeout");
		否则 もし ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		否则 もし (c.ephemeral)
			warning("not storing ephemeral credential");
		否则 {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	否则
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

static 整数 serve_cache_loop(整数 fd)
{
	struct pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	もし (!wakeup)
		戻る 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	もし (poll(&pfd, 1, 1000 * wakeup) < 0) {
		もし (errno != EINTR)
			die_errno("poll failed");
		戻る 1;
	}

	もし (pfd.revents & POLLIN) {
		整数 client, client2;
		FILE *in, *out;

		client = accept(fd, NULL, NULL);
		もし (client < 0) {
			warning_errno("accept failed");
			戻る 1;
		}
		client2 = dup(client);
		もし (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			戻る 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		fclose(in);
		fclose(out);
	}
	戻る 1;
}

static void serve_cache(const char *socket_path, 整数 debug)
{
	struct unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	整数 fd;

	fd = unix_stream_listen(socket_path, &opts);
	もし (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	printf("ok\n");
	fclose(stdout);
	もし (!debug) {
		もし (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	ながら (serve_cache_loop(fd))
		; /* nothing */

	close(fd);
}

static const char permissions_advice[] = N_(
"The permissions on your socket directory are too loose; other\n"
"users may be able to read your cached credentials. Consider running:\n"
"\n"
"	chmod 0700 %s");
static void init_socket_directory(const char *path)
{
	struct stat st;
	char *path_copy = xstrdup(path);
	char *dir = dirname(path_copy);

	もし (!stat(dir, &st)) {
		もし (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} 否则 {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		もし (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		もし (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	もし (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	free(path_copy);
}

整数 cmd_credential_cache_daemon(整数 argc,
				const char **argv,
				const char *prefix,
				struct repository *repo UNUSED)
{
	struct tempfile *socket_file;
	const char *socket_path;
	整数 ignore_sighup = 0;
	static const char *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		NULL
	};
	整数 debug = 0;
	const struct option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	もし (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	もし (!socket_path)
		usage_with_options(usage, options);

	もし (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	もし (ignore_sighup)
		signal(SIGHUP, SIG_IGN);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	戻る 0;
}

#否则

整数 cmd_credential_cache_daemon(整数 argc,
const char **argv,
const char *prefix,
struct repository *repo UNUSED)
{
	const char * const usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		"",
		"credential-cache--daemon is disabled in this build of Git",
		NULL
	};
	struct option options[] = { OPT_END() };

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	die(_("credential-cache--daemon unavailable; no unix socket support"));
}

#endif /* NO_UNIX_SOCKET */
