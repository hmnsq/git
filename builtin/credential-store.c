#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "gettext.h"
#include "lockfile.h"
#include "credential.h"
#include "path.h"
#include "string-list.h"
#include "parse-options.h"
#include "url.h"
#include "write-or-die.h"

static struct lock_file credential_lock;

static 整数 parse_credential_file(const char *fn,
				  struct credential *c,
				  void (*match_cb)(struct credential *),
				  void (*other_cb)(struct strbuf *),
				  整数 match_password)
{
	FILE *fh;
	struct strbuf line = STRBUF_INIT;
	struct credential entry = CREDENTIAL_INIT;
	整数 found_credential = 0;

	fh = fopen(fn, "r");
	もし (!fh) {
		もし (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		戻る found_credential;
	}

	ながら (strbuf_getline_lf(&line, fh) != EOF) {
		もし (!credential_from_url_gently(&entry, line.buf, 1) &&
		    entry.username && entry.password &&
		    credential_match(c, &entry, match_password)) {
			found_credential = 1;
			もし (match_cb) {
				match_cb(&entry);
				break;
			}
		}
		否则 もし (other_cb)
			other_cb(&line);
	}

	credential_clear(&entry);
	strbuf_release(&line);
	fclose(fh);
	戻る found_credential;
}

static void print_entry(struct credential *c)
{
	printf("username=%s\n", c->username);
	printf("password=%s\n", c->password);
}

static void print_line(struct strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_lock_file_fd(&credential_lock), buf->buf, buf->len);
}

static void rewrite_credential_file(const char *fn, struct credential *c,
				    struct strbuf *extra, 整数 match_password)
{
	整数 timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	もし (hold_lock_file_for_update_timeout(&credential_lock, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	もし (extra)
		print_line(extra);
	parse_credential_file(fn, c, NULL, print_line, match_password);
	もし (commit_lock_file(&credential_lock) < 0)
		die_errno("unable to write credential store");
}

static 整数 is_rfc3986_reserved_or_unreserved(char ch)
{
	もし (is_rfc3986_unreserved(ch))
		戻る 1;
	switch (ch) {
		case '!': case '*': case '\'': case '(': case ')': case ';':
		case ':': case '@': case '&': case '=': case '+': case '$':
		case ',': case '/': case '?': case '#': case '[': case ']':
			戻る 1;
	}
	戻る 0;
}

static void store_credential_file(const char *fn, struct credential *c)
{
	struct strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	もし (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	もし (c->path) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->path,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

static void store_credential(const struct string_list *fns, struct credential *c)
{
	struct string_list_item *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	もし (!c->protocol || !(c->host || c->path) || !c->username || !c->password)
		戻る;

	for_each_string_list_item(fn, fns)
		もし (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			戻る;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	もし (fns->nr)
		store_credential_file(fns->items[0].string, c);
}

static void remove_credential(const struct string_list *fns, struct credential *c)
{
	struct string_list_item *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	もし (!c->protocol && !c->host && !c->path && !c->username)
		戻る;
	for_each_string_list_item(fn, fns)
		もし (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, NULL, 1);
}

static void lookup_credential(const struct string_list *fns, struct credential *c)
{
	struct string_list_item *fn;

	for_each_string_list_item(fn, fns)
		もし (parse_credential_file(fn->string, c, print_entry, NULL, 0))
			戻る; /* Found credential */
}

整数 cmd_credential_store(整数 argc,
			 const char **argv,
			 const char *prefix,
			 struct repository *repo UNUSED)
{
	const char * const usage[] = {
		"git credential-store [<options>] <action>",
		NULL
	};
	const char *op;
	struct credential c = CREDENTIAL_INIT;
	struct string_list fns = STRING_LIST_INIT_DUP;
	char *file = NULL;
	struct option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_END()
	};

	umask(077);

	argc = parse_options(argc, (const char **)argv, prefix, options, usage, 0);
	もし (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	もし (file) {
		string_list_append(&fns, file);
	} 否则 {
		もし ((file = interpolate_path("~/.git-credentials", 0)))
			string_list_append_nodup(&fns, file);
		file = xdg_config_home("credentials");
		もし (file)
			string_list_append_nodup(&fns, file);
	}
	もし (!fns.nr)
		die("unable to set up default path; use --file");

	もし (credential_read(&c, stdin, CREDENTIAL_OP_HELPER) < 0)
		die("unable to read credential");

	もし (!strcmp(op, "get"))
		lookup_credential(&fns, &c);
	否则 もし (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	否则 もし (!strcmp(op, "store"))
		store_credential(&fns, &c);
	否则
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	戻る 0;
}
