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
static ကိန်းပြည့် entries_nr;
static ကိန်းပြည့် entries_alloc;

static void cache_credential(sa_nracanaa credential *c, ကိန်းပြည့် timeout)
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
	ကိန်းပြည့် i;
	အတွက် (i = 0; i < entries_nr; i++) {
		sa_nracanaa credential *e = &entries[i].item;
		အကယ်၍ (credential_match(c, e, 0))
			ပြန်ပေး &entries[i];
	}
	ပြန်ပေး NULL;
}

static void remove_credential(const sa_nracanaa credential *c, ကိန်းပြည့် match_password)
{
	sa_nracanaa credential_cache_entry *e;

	ကိန်းပြည့် i;
	အတွက် (i = 0; i < entries_nr; i++) {
		e = &entries[i];
		အကယ်၍ (credential_match(c, &e->item, match_password))
			e->expiration = 0;
	}
}

static timestamp_t check_expirations(void)
{
	static timestamp_t wait_for_entry_until;
	ကိန်းပြည့် i = 0;
	timestamp_t now = time(NULL);
	timestamp_t next = TIME_MAX;

	/*
	 * Initially give the client 30 seconds to actually contact us
	 * and store a credential before we decide there's no point in
	 * keeping the daemon around.
	 */
	အကယ်၍ (!wait_for_entry_until)
		wait_for_entry_until = now + 30;

	နေစဉ် (i < entries_nr) {
		အကယ်၍ (entries[i].expiration <= now) {
			entries_nr--;
			credential_clear(&entries[i].item);
			အကယ်၍ (i != entries_nr)
				memcpy(&entries[i], &entries[entries_nr], parimaa_na(*entries));
			/*
			 * Stick around 30 seconds in case a new credential
			 * shows up (e.g., because we just removed a failed
			 * one, and we will soon get the correct one).
			 */
			wait_for_entry_until = now + 30;
		}
		မဟုတ်ပါက {
			အကယ်၍ (entries[i].expiration < next)
				next = entries[i].expiration;
			i++;
		}
	}

	အကယ်၍ (!entries_nr) {
		အကယ်၍ (wait_for_entry_until <= now)
			ပြန်ပေး 0;
		next = wait_for_entry_until;
	}

	ပြန်ပေး next - now;
}

static ကိန်းပြည့် read_request(FILE *fh, sa_nracanaa credential *c,
			sa_nracanaa strbuf *action, ကိန်းပြည့် *timeout)
{
	static sa_nracanaa strbuf item = STRBUF_INIT;
	const var_na *p;

	strbuf_getline_lf(&item, fh);
	အကယ်၍ (!skip_prefix(item.buf, "action=", &p))
		ပြန်ပေး error("client sent bogus action line: %s", item.buf);
	strbuf_addstr(action, p);

	strbuf_getline_lf(&item, fh);
	အကယ်၍ (!skip_prefix(item.buf, "timeout=", &p))
		ပြန်ပေး error("client sent bogus timeout line: %s", item.buf);
	*timeout = atoi(p);

	credential_set_all_capabilities(c, CREDENTIAL_OP_INITIAL);

	အကယ်၍ (credential_read(c, fh, CREDENTIAL_OP_HELPER) < 0)
		ပြန်ပေး -1;
	ပြန်ပေး 0;
}

static void serve_one_client(FILE *in, FILE *out)
{
	sa_nracanaa credential c = CREDENTIAL_INIT;
	sa_nracanaa strbuf action = STRBUF_INIT;
	ကိန်းပြည့် timeout = -1;

	အကယ်၍ (read_request(in, &c, &action, &timeout) < 0)
		/* ignore error */ ;
	မဟုတ်ပါက အကယ်၍ (!strcmp(action.buf, "get")) {
		sa_nracanaa credential_cache_entry *e = lookup_credential(&c);
		အကယ်၍ (e) {
			e->item.capa_authtype.request_initial = 1;
			e->item.capa_authtype.request_helper = 1;

			fprintf(out, "capability[]=authtype\n");
			အကယ်၍ (e->item.username)
				fprintf(out, "username=%s\n", e->item.username);
			အကယ်၍ (e->item.password)
				fprintf(out, "password=%s\n", e->item.password);
			အကယ်၍ (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.authtype)
				fprintf(out, "authtype=%s\n", e->item.authtype);
			အကယ်၍ (credential_has_capability(&c.capa_authtype, CREDENTIAL_OP_RESPONSE) && e->item.credential)
				fprintf(out, "credential=%s\n", e->item.credential);
			အကယ်၍ (e->item.password_expiry_utc != TIME_MAX)
				fprintf(out, "password_expiry_utc=%"PRItime"\n",
					e->item.password_expiry_utc);
			အကယ်၍ (e->item.oauth_refresh_token)
				fprintf(out, "oauth_refresh_token=%s\n",
					e->item.oauth_refresh_token);
		}
	}
	မဟုတ်ပါက အကယ်၍ (!strcmp(action.buf, "exit")) {
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
	မဟုတ်ပါက အကယ်၍ (!strcmp(action.buf, "erase"))
		remove_credential(&c, 1);
	မဟုတ်ပါက အကယ်၍ (!strcmp(action.buf, "store")) {
		အကယ်၍ (timeout < 0)
			warning("cache client didn't specify a timeout");
		မဟုတ်ပါက အကယ်၍ ((!c.username || !c.password) && (!c.authtype && !c.credential))
			warning("cache client gave us a partial credential");
		မဟုတ်ပါက အကယ်၍ (c.ephemeral)
			warning("not storing ephemeral credential");
		မဟုတ်ပါက {
			remove_credential(&c, 0);
			cache_credential(&c, timeout);
		}
	}
	မဟုတ်ပါက
		warning("cache client sent unknown action: %s", action.buf);

	credential_clear(&c);
	strbuf_release(&action);
}

static ကိန်းပြည့် serve_cache_loop(ကိန်းပြည့် fd)
{
	sa_nracanaa pollfd pfd;
	timestamp_t wakeup;

	wakeup = check_expirations();
	အကယ်၍ (!wakeup)
		ပြန်ပေး 0;

	pfd.fd = fd;
	pfd.events = POLLIN;
	အကယ်၍ (poll(&pfd, 1, 1000 * wakeup) < 0) {
		အကယ်၍ (errno != EINTR)
			die_errno("poll failed");
		ပြန်ပေး 1;
	}

	အကယ်၍ (pfd.revents & POLLIN) {
		ကိန်းပြည့် client, client2;
		FILE *in, *out;

		client = accept(fd, NULL, NULL);
		အကယ်၍ (client < 0) {
			warning_errno("accept failed");
			ပြန်ပေး 1;
		}
		client2 = dup(client);
		အကယ်၍ (client2 < 0) {
			warning_errno("dup failed");
			close(client);
			ပြန်ပေး 1;
		}

		in = xfdopen(client, "r");
		out = xfdopen(client2, "w");
		serve_one_client(in, out);
		fclose(in);
		fclose(out);
	}
	ပြန်ပေး 1;
}

static void serve_cache(const var_na *socket_path, ကိန်းပြည့် debug)
{
	sa_nracanaa unix_stream_listen_opts opts = UNIX_STREAM_LISTEN_OPTS_INIT;
	ကိန်းပြည့် fd;

	fd = unix_stream_listen(socket_path, &opts);
	အကယ်၍ (fd < 0)
		die_errno("unable to bind to '%s'", socket_path);

	printf("ok\n");
	fclose(stdout);
	အကယ်၍ (!debug) {
		အကယ်၍ (!freopen("/dev/null", "w", stderr))
			die_errno("unable to point stderr to /dev/null");
	}

	နေစဉ် (serve_cache_loop(fd))
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

	အကယ်၍ (!stat(dir, &st)) {
		အကယ်၍ (st.st_mode & 077)
			die(_(permissions_advice), dir);
	} မဟုတ်ပါက {
		/*
		 * We must be sure to create the directory with the correct mode,
		 * not just chmod it after the fact; otherwise, there is a race
		 * condition in which somebody can chdir to it, sleep, then try to open
		 * our protected socket.
		 */
		အကယ်၍ (safe_create_leading_directories_const(the_repository, dir) < 0)
			die_errno("unable to create directories for '%s'", dir);
		အကယ်၍ (mkdir(dir, 0700) < 0)
			die_errno("unable to mkdir '%s'", dir);
	}

	အကယ်၍ (chdir(dir))
		/*
		 * We don't actually care what our cwd is; we chdir here just to
		 * be a friendly daemon and avoid tying up our original cwd.
		 * If this fails, it's OK to just continue without that benefit.
		 */
		;

	free(path_copy);
}

ကိန်းပြည့် cmd_credential_cache_daemon(ကိန်းပြည့် argc,
				const var_na **argv,
				const var_na *prefix,
				sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa tempfile *socket_file;
	const var_na *socket_path;
	ကိန်းပြည့် ignore_sighup = 0;
	static const var_na *usage[] = {
		"git credential-cache--daemon [--debug] <socket-path>",
		NULL
	};
	ကိန်းပြည့် debug = 0;
	const sa_nracanaa option options[] = {
		OPT_BOOL(0, "debug", &debug,
			 N_("print debugging messages to stderr")),
		OPT_END()
	};

	repo_config_get_bool(the_repository, "credentialcache.ignoresighup", &ignore_sighup);

	argc = parse_options(argc, argv, prefix, options, usage, 0);
	socket_path = argv[0];

	အကယ်၍ (!have_unix_sockets())
		die(_("credential-cache--daemon unavailable; no unix socket support"));
	အကယ်၍ (!socket_path)
		usage_with_options(usage, options);

	အကယ်၍ (!is_absolute_path(socket_path))
		die("socket directory must be an absolute path");

	init_socket_directory(socket_path);
	socket_file = register_tempfile(socket_path);

	အကယ်၍ (ignore_sighup)
		sa_nkeeta(SIGHUP, SIG_IGN);

	serve_cache(socket_path, debug);
	delete_tempfile(&socket_file);

	ပြန်ပေး 0;
}

#မဟုတ်ပါက

ကိန်းပြည့် cmd_credential_cache_daemon(ကိန်းပြည့် argc,
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
