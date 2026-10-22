#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "lockfile.h"
#அந்தர்பாவ "credential.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "url.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான கட்டமைப்பு lock_file credential_lock;

நிலையான முழு parse_credential_file(மாறிலி எழுத்து *fn,
				  கட்டமைப்பு credential *c,
				  வெறுமை (*match_cb)(கட்டமைப்பு credential *),
				  வெறுமை (*other_cb)(கட்டமைப்பு strbuf *),
				  முழு match_password)
{
	FILE *fh;
	கட்டமைப்பு strbuf வரி = STRBUF_INIT;
	கட்டமைப்பு credential entry = CREDENTIAL_INIT;
	முழு found_credential = 0;

	fh = fopen(fn, "r");
	எனில் (!fh) {
		எனில் (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		திரும்பு found_credential;
	}

	போது (strbuf_getline_lf(&வரி, fh) != EOF) {
		எனில் (!credential_from_url_gently(&entry, வரி.buf, 1) &&
		    entry.username && entry.password &&
		    credential_match(c, &entry, match_password)) {
			found_credential = 1;
			எனில் (match_cb) {
				match_cb(&entry);
				நிறுத்து;
			}
		}
		இல்லையெனில் எனில் (other_cb)
			other_cb(&வரி);
	}

	credential_clear(&entry);
	strbuf_release(&வரி);
	fclose(fh);
	திரும்பு found_credential;
}

நிலையான வெறுமை print_entry(கட்டமைப்பு credential *c)
{
	printf("username=%s\n", c->username);
	printf("password=%s\n", c->password);
}

நிலையான வெறுமை print_வரி(கட்டமைப்பு strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_lock_file_fd(&credential_lock), buf->buf, buf->len);
}

நிலையான வெறுமை rewrite_credential_file(மாறிலி எழுத்து *fn, கட்டமைப்பு credential *c,
				    கட்டமைப்பு strbuf *extra, முழு match_password)
{
	முழு timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	எனில் (hold_lock_file_for_update_timeout(&credential_lock, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	எனில் (extra)
		print_வரி(extra);
	parse_credential_file(fn, c, வெற்று, print_வரி, match_password);
	எனில் (commit_lock_file(&credential_lock) < 0)
		die_errno("unable to write credential store");
}

நிலையான முழு is_rfc3986_reserved_or_unreserved(எழுத்து ch)
{
	எனில் (is_rfc3986_unreserved(ch))
		திரும்பு 1;
	தேர்வு (ch) {
		நிலை '!': நிலை '*': நிலை '\'': நிலை '(': நிலை ')': நிலை ';':
		நிலை ':': நிலை '@': நிலை '&': நிலை '=': நிலை '+': நிலை '$':
		நிலை ',': நிலை '/': நிலை '?': நிலை '#': நிலை '[': நிலை ']':
			திரும்பு 1;
	}
	திரும்பு 0;
}

நிலையான வெறுமை store_credential_file(மாறிலி எழுத்து *fn, கட்டமைப்பு credential *c)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	எனில் (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	எனில் (c->path) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->path,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

நிலையான வெறுமை store_credential(மாறிலி கட்டமைப்பு string_list *fns, கட்டமைப்பு credential *c)
{
	கட்டமைப்பு string_list_item *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	எனில் (!c->protocol || !(c->host || c->path) || !c->username || !c->password)
		திரும்பு;

	for_each_string_list_item(fn, fns)
		எனில் (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			திரும்பு;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	எனில் (fns->nr)
		store_credential_file(fns->பொருட்கள்[0].string, c);
}

நிலையான வெறுமை remove_credential(மாறிலி கட்டமைப்பு string_list *fns, கட்டமைப்பு credential *c)
{
	கட்டமைப்பு string_list_item *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	எனில் (!c->protocol && !c->host && !c->path && !c->username)
		திரும்பு;
	for_each_string_list_item(fn, fns)
		எனில் (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, வெற்று, 1);
}

நிலையான வெறுமை lookup_credential(மாறிலி கட்டமைப்பு string_list *fns, கட்டமைப்பு credential *c)
{
	கட்டமைப்பு string_list_item *fn;

	for_each_string_list_item(fn, fns)
		எனில் (parse_credential_file(fn->string, c, print_entry, வெற்று, 0))
			திரும்பு; /* Found credential */
}

முழு cmd_credential_store(முழு argc,
			 மாறிலி எழுத்து **argv,
			 மாறிலி எழுத்து *prefix,
			 கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து * மாறிலி usage[] = {
		"git credential-store [<options>] <action>",
		வெற்று
	};
	மாறிலி எழுத்து *op;
	கட்டமைப்பு credential c = CREDENTIAL_INIT;
	கட்டமைப்பு string_list fns = STRING_LIST_INIT_DUP;
	எழுத்து *file = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_END()
	};

	umask(077);

	argc = parse_options(argc, (மாறிலி எழுத்து **)argv, prefix, options, usage, 0);
	எனில் (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	எனில் (file) {
		string_list_append(&fns, file);
	} இல்லையெனில் {
		எனில் ((file = interpolate_path("~/.git-credentials", 0)))
			string_list_append_nodup(&fns, file);
		file = xdg_config_home("credentials");
		எனில் (file)
			string_list_append_nodup(&fns, file);
	}
	எனில் (!fns.nr)
		die("unable to set up default path; use --file");

	எனில் (credential_read(&c, stdin, CREDENTIAL_OP_HELPER) < 0)
		die("unable to read credential");

	எனில் (!strcmp(op, "get"))
		lookup_credential(&fns, &c);
	இல்லையெனில் எனில் (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	இல்லையெனில் எனில் (!strcmp(op, "store"))
		store_credential(&fns, &c);
	இல்லையெனில்
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	திரும்பு 0;
}
