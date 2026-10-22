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
static ціле entries_nr;
static ціле entries_alloc;

static void cache_credential(struct credential *c, ціле timeout)
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
	ціле i;
	для (i = 0; i < entries_nr; i++) {
		struct credential *e = &entries[i].item;
		якщо (credential_match(c, e, 0))
			повернути &entries[i];
	}
	повернути NULL;
}

static void remove_credential(const struct credential *c, ціле match_password)
{
	struct credential_cache_entry *e;

	ціле i;
	для (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		якщо (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

static timestamp_t check_expirations(void)
{
	static timestamp_t wait_for_entry_until;
	ціле i = 0;
	timestamp_t now = time(NULL);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	якщо (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	поки (i < entries_nr) {
		якщо (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			якщо (i != entries_nr)
				memcpy(&entries[i], &entries[entries_nr], sizeof(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		інакше {
			якщо (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	якщо (!entries_nr) {
		якщо (wait_for_entry_until <= now)
			повернути 0;
		next = wait_for_entry_until;
	}

	повернути next - now;
}

static ціле read_request(FILE *fh, struct credential *c,
			struct strbuf *action, ціле *timeout)
{
	static struct strbuf item = STRBUF_INIT;
	const char *p;

	strbuf_getline_lf(&item, fh);
	якщо (!skip_prefix(item.buf, "action=", &p))
		повернути error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	якщо (!skip_prefix(item.buf, "timeout=", &p))
		повернути error("client sent bogus timeout line: %s", item.buf);
	*timeout = atoi(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	якщо (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		повернути -1;
	повернути 0;
}

static void serve_one_client(FILE *in, FILE *out)
{
	struct credential c = CREDENTIAL_INIT;
	struct strbuf action = STRBUF_INIT;
	ціле timeout = -1;

	якщо (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	інакше якщо (!strcmp(action.buf, "get")) {
		struct credential_cache_entry *e = lookup_credential(&c);
		якщо (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			fprintf(out, "capability[]=authtype\n");
			якщо (e->item.username)
				fprintf(out, "username=%s\n", e->item.username);
			якщо (e->item.password)
				fprintf(out, "password=%s\n", e->item.password);
			якщо (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				fprintf(out, "authtype=%s\n", e->item.authtype);
			якщо (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				fprintf(out, "credential=%s\n", e->item.credential);
			якщо (e->item.password_expiry_utc != TIME_MAX)
				fprintf(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			якщо (e->item.oauth_refresh_token)
				fprintf(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	інакше якщо (!strcmp(action.buf, "exit")) {
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
	інакше якщо (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	інакше якщо (!strcmp(action.buf, "store")) {
		якщо (timeout < 0)
			warning("cache client didn't specify a timeout");
		інакше якщо ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		інакше якщо (c.ephemeral)
			warning("not storing ephemeral credential");
		інакше {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	інакше
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

static ціле serve_cache_loop(ціле fd)
{
	struct pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	якщо (!wakeup)
		повернути 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	якщо (poll(&pfd, 1, 1000 * wakeup) < 0) {
		якщо (errno != EINTR)
			die_errno("poll failed");
		повернути 1;
	}

	якщо (pfd.revents & POLLIN) {
		ціле client, client2;
		FILE *in, *out;

		client = accept(fd, NULL, NULL);
		якщо (client < 0) {
			warning_errno("accept failed");
			повернути 1;
		}
		client2 = dup(client);
		якщо (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			повернути 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		fclose(in);
		fclose(out);
	}
	повернути 1;
}

static void serve_cache(const char *socket_path, ціле debug)
{
	struct unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	ціле fd;

	fd = unix_stream_listen(socket_path, &opts);
	якщо (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	printf("ok\n");
	fclose(stdout);
	якщо (!debug) {
		якщо (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	поки (serve_cache_loop(fd))
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

	якщо (!stat(dir, &st)) {
		якщо (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} інакше {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		якщо (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		якщо (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	якщо (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	free(path_copy);
}

ціле cmd_credential_cache_daemon(ціле argc,
				const char **argv,
				const char *prefix,
				struct repository *repo UNUSED)
{
	struct tempfile *socket_file;
	const char *socket_path;
	ціле ignore_sighup = 0;
	static const char *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		NULL
	};
	ціле debug = 0;
	const struct option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	якщо (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	якщо (!socket_path)
		usage_with_options(usage, options);

	якщо (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	якщо (ignore_sighup)
		signal(SIGHUP, SIG_IGN);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	повернути 0;
}

#інакше

ціле cmd_credential_cache_daemon(ціле argc,
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
