#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "lockfile.h"
#અન્તર્ભાવ "credential.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "url.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર સંરચના lock_file credential_lock;

સ્થિર પૂર્ણાંક parse_credential_file(અચલ અક્ષર *fn,
				  સંરચના credential *c,
				  શૂન્ય (*match_cb)(સંરચના credential *),
				  શૂન્ય (*other_cb)(સંરચના strbuf *),
				  પૂર્ણાંક match_password)
{
	FILE *fh;
	સંરચના strbuf લીટી = STRBUF_INIT;
	સંરચના credential entry = CREDENTIAL_INIT;
	પૂર્ણાંક found_credential = 0;

	fh = fopen(fn, "r");
	જો (!fh) {
		જો (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		પાછા found_credential;
	}

	જ્યારે (strbuf_getline_lf(&લીટી, fh) != EOF) {
		જો (!credential_from_url_gently(&entry, લીટી.buf, 1) &&
		    entry.username && entry.password &&
		    credential_match(c, &entry, match_password)) {
			found_credential = 1;
			જો (match_cb) {
				match_cb(&entry);
				તોડો;
			}
		}
		નહીંતર જો (other_cb)
			other_cb(&લીટી);
	}

	credential_clear(&entry);
	strbuf_release(&લીટી);
	fclose(fh);
	પાછા found_credential;
}

સ્થિર શૂન્ય print_entry(સંરચના credential *c)
{
	printf("username=%s\n", c->username);
	printf("password=%s\n", c->password);
}

સ્થિર શૂન્ય print_લીટી(સંરચના strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_lock_file_fd(&credential_lock), buf->buf, buf->len);
}

સ્થિર શૂન્ય rewrite_credential_file(અચલ અક્ષર *fn, સંરચના credential *c,
				    સંરચના strbuf *extra, પૂર્ણાંક match_password)
{
	પૂર્ણાંક timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	જો (hold_lock_file_for_update_timeout(&credential_lock, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	જો (extra)
		print_લીટી(extra);
	parse_credential_file(fn, c, ખાલી, print_લીટી, match_password);
	જો (commit_lock_file(&credential_lock) < 0)
		die_errno("unable to write credential store");
}

સ્થિર પૂર્ણાંક is_rfc3986_reserved_or_unreserved(અક્ષર ch)
{
	જો (is_rfc3986_unreserved(ch))
		પાછા 1;
	પસંદગી (ch) {
		સ્થિતિ '!': સ્થિતિ '*': સ્થિતિ '\'': સ્થિતિ '(': સ્થિતિ ')': સ્થિતિ ';':
		સ્થિતિ ':': સ્થિતિ '@': સ્થિતિ '&': સ્થિતિ '=': સ્થિતિ '+': સ્થિતિ '$':
		સ્થિતિ ',': સ્થિતિ '/': સ્થિતિ '?': સ્થિતિ '#': સ્થિતિ '[': સ્થિતિ ']':
			પાછા 1;
	}
	પાછા 0;
}

સ્થિર શૂન્ય store_credential_file(અચલ અક્ષર *fn, સંરચના credential *c)
{
	સંરચના strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	જો (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	જો (c->path) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->path,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

સ્થિર શૂન્ય store_credential(અચલ સંરચના string_list *fns, સંરચના credential *c)
{
	સંરચના string_list_item *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	જો (!c->protocol || !(c->host || c->path) || !c->username || !c->password)
		પાછા;

	for_each_string_list_item(fn, fns)
		જો (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			પાછા;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	જો (fns->nr)
		store_credential_file(fns->વસ્તુઓ[0].string, c);
}

સ્થિર શૂન્ય remove_credential(અચલ સંરચના string_list *fns, સંરચના credential *c)
{
	સંરચના string_list_item *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	જો (!c->protocol && !c->host && !c->path && !c->username)
		પાછા;
	for_each_string_list_item(fn, fns)
		જો (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, ખાલી, 1);
}

સ્થિર શૂન્ય lookup_credential(અચલ સંરચના string_list *fns, સંરચના credential *c)
{
	સંરચના string_list_item *fn;

	for_each_string_list_item(fn, fns)
		જો (parse_credential_file(fn->string, c, print_entry, ખાલી, 0))
			પાછા; /* Found credential */
}

પૂર્ણાંક cmd_credential_store(પૂર્ણાંક argc,
			 અચલ અક્ષર **argv,
			 અચલ અક્ષર *prefix,
			 સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર * અચલ usage[] = {
		"git credential-store [<options>] <action>",
		ખાલી
	};
	અચલ અક્ષર *op;
	સંરચના credential c = CREDENTIAL_INIT;
	સંરચના string_list fns = STRING_LIST_INIT_DUP;
	અક્ષર *file = ખાલી;
	સંરચના option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_END()
	};

	umask(077);

	argc = parse_options(argc, (અચલ અક્ષર **)argv, prefix, options, usage, 0);
	જો (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	જો (file) {
		string_list_append(&fns, file);
	} નહીંતર {
		જો ((file = interpolate_path("~/.git-credentials", 0)))
			string_list_append_nodup(&fns, file);
		file = xdg_config_home("credentials");
		જો (file)
			string_list_append_nodup(&fns, file);
	}
	જો (!fns.nr)
		die("unable to set up default path; use --file");

	જો (credential_read(&c, stdin, CREDENTIAL_OP_HELPER) < 0)
		die("unable to read credential");

	જો (!strcmp(op, "get"))
		lookup_credential(&fns, &c);
	નહીંતર જો (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	નહીંતર જો (!strcmp(op, "store"))
		store_credential(&fns, &c);
	નહીંતર
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	પાછા 0;
}
