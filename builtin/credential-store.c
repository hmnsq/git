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

static sa_nracanaa lock_file credential_lock;

static puur_naa_nka parse_credential_file(const var_na *fn,
				  sa_nracanaa credential *c,
				  void (*match_cb)(sa_nracanaa credential *),
				  void (*other_cb)(sa_nracanaa strbuf *),
				  puur_naa_nka match_password)
{
	FILE *fh;
	sa_nracanaa strbuf line = STRBUF_INIT;
	sa_nracanaa credential entry = CREDENTIAL_INIT;
	puur_naa_nka found_credential = 0;

	fh = fopen(fn, "r");
	ᯀᯮᯝ (!fh) {
		ᯀᯮᯝ (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		ᯀᯮᯞ found_credential;
	}

	ᯀᯬᯝ (strbuf_getline_lf(&line, fh) != EOF) {
		ᯀᯮᯝ (!credential_from_url_gently(&entry, line.buf, 1) &&
		    entry.username && entry.password &&
		    credential_match(c, &entry, match_password)) {
			found_credential = 1;
			ᯀᯮᯝ (match_cb) {
				match_cb(&entry);
				bha_nga;
			}
		}
		ᯀᯮᯝᯂᯪ ᯀᯮᯝ (other_cb)
			other_cb(&line);
	}

	credential_clear(&entry);
	strbuf_release(&line);
	fclose(fh);
	ᯀᯮᯞ found_credential;
}

static void print_entry(sa_nracanaa credential *c)
{
	printf("username=%s\n", c->username);
	printf("password=%s\n", c->password);
}

static void print_line(sa_nracanaa strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_lock_file_fd(&credential_lock), buf->buf, buf->len);
}

static void rewrite_credential_file(const var_na *fn, sa_nracanaa credential *c,
				    sa_nracanaa strbuf *extra, puur_naa_nka match_password)
{
	puur_naa_nka timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	ᯀᯮᯝ (hold_lock_file_for_update_timeout(&credential_lock, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	ᯀᯮᯝ (extra)
		print_line(extra);
	parse_credential_file(fn, c, NULL, print_line, match_password);
	ᯀᯮᯝ (commit_lock_file(&credential_lock) < 0)
		die_errno("unable to write credential store");
}

static puur_naa_nka is_rfc3986_reserved_or_unreserved(var_na ch)
{
	ᯀᯮᯝ (is_rfc3986_unreserved(ch))
		ᯀᯮᯞ 1;
	switch (ch) {
		pak_sa '!': pak_sa '*': pak_sa '\'': pak_sa '(': pak_sa ')': pak_sa ';':
		pak_sa ':': pak_sa '@': pak_sa '&': pak_sa '=': pak_sa '+': pak_sa '$':
		pak_sa ',': pak_sa '/': pak_sa '?': pak_sa '#': pak_sa '[': pak_sa ']':
			ᯀᯮᯞ 1;
	}
	ᯀᯮᯞ 0;
}

static void store_credential_file(const var_na *fn, sa_nracanaa credential *c)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	ᯀᯮᯝ (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	ᯀᯮᯝ (c->path) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->path,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

static void store_credential(const sa_nracanaa string_list *fns, sa_nracanaa credential *c)
{
	sa_nracanaa string_list_item *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	ᯀᯮᯝ (!c->protocol || !(c->host || c->path) || !c->username || !c->password)
		ᯀᯮᯞ;

	for_each_string_list_item(fn, fns)
		ᯀᯮᯝ (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			ᯀᯮᯞ;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	ᯀᯮᯝ (fns->nr)
		store_credential_file(fns->items[0].string, c);
}

static void remove_credential(const sa_nracanaa string_list *fns, sa_nracanaa credential *c)
{
	sa_nracanaa string_list_item *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	ᯀᯮᯝ (!c->protocol && !c->host && !c->path && !c->username)
		ᯀᯮᯞ;
	for_each_string_list_item(fn, fns)
		ᯀᯮᯝ (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, NULL, 1);
}

static void lookup_credential(const sa_nracanaa string_list *fns, sa_nracanaa credential *c)
{
	sa_nracanaa string_list_item *fn;

	for_each_string_list_item(fn, fns)
		ᯀᯮᯝ (parse_credential_file(fn->string, c, print_entry, NULL, 0))
			ᯀᯮᯞ; /* Found credential */
}

puur_naa_nka cmd_credential_store(puur_naa_nka argc,
			 const var_na **argv,
			 const var_na *prefix,
			 sa_nracanaa repository *repo UNUSED)
{
	const var_na * const usage[] = {
		"git credential-store [<options>] <action>",
		NULL
	};
	const var_na *op;
	sa_nracanaa credential c = CREDENTIAL_INIT;
	sa_nracanaa string_list fns = STRING_LIST_INIT_DUP;
	var_na *file = NULL;
	sa_nracanaa option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_END()
	};

	umask(077);

	argc = parse_options(argc, (const var_na **)argv, prefix, options, usage, 0);
	ᯀᯮᯝ (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	ᯀᯮᯝ (file) {
		string_list_append(&fns, file);
	} ᯀᯮᯝᯂᯪ {
		ᯀᯮᯝ ((file = interpolate_path("~/.git-credentials", 0)))
			string_list_append_nodup(&fns, file);
		file = xdg_config_home("credentials");
		ᯀᯮᯝ (file)
			string_list_append_nodup(&fns, file);
	}
	ᯀᯮᯝ (!fns.nr)
		die("unable to set up default path; use --file");

	ᯀᯮᯝ (credential_read(&c, stdin, CREDENTIAL_OP_HELPER) < 0)
		die("unable to read credential");

	ᯀᯮᯝ (!strcmp(op, "get"))
		lookup_credential(&fns, &c);
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcmp(op, "store"))
		store_credential(&fns, &c);
	ᯀᯮᯝᯂᯪ
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	ᯀᯮᯞ 0;
}
