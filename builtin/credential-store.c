#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "gettext.h"
#شامل "lockfile.h"
#شامل "credential.h"
#شامل "path.h"
#شامل "string-list.h"
#شامل "parse-options.h"
#شامل "url.h"
#شامل "write-or-die.h"

ایستا ساختار lock_file credential_lock;

ایستا صحیح parse_credential_file(ثابت نویسه *fn,
				  ساختار credential *c,
				  تهی (*match_cb)(ساختار credential *),
				  تهی (*other_cb)(ساختار strbuf *),
				  صحیح match_password)
{
	FILE *fh;
	ساختار strbuf سطر = STRBUF_INIT;
	ساختار credential entry = CREDENTIAL_INIT;
	صحیح found_credential = 0;

	fh = fopen(fn, "r");
	اگر (!fh) {
		اگر (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		بازگشت found_credential;
	}

	وقتی (strbuf_getline_lf(&سطر, fh) != EOF) {
		اگر (!credential_from_url_gently(&entry, سطر.buf, 1) &&
		    entry.username && entry.password &&
		    credential_match(c, &entry, match_password)) {
			found_credential = 1;
			اگر (match_cb) {
				match_cb(&entry);
				بشکن;
			}
		}
		وگرنه اگر (other_cb)
			other_cb(&سطر);
	}

	credential_clear(&entry);
	strbuf_release(&سطر);
	fclose(fh);
	بازگشت found_credential;
}

ایستا تهی print_entry(ساختار credential *c)
{
	printf("username=%s\n", c->username);
	printf("password=%s\n", c->password);
}

ایستا تهی print_سطر(ساختار strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_lock_file_fd(&credential_lock), buf->buf, buf->len);
}

ایستا تهی rewrite_credential_file(ثابت نویسه *fn, ساختار credential *c,
				    ساختار strbuf *extra, صحیح match_password)
{
	صحیح timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	اگر (hold_lock_file_for_update_timeout(&credential_lock, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	اگر (extra)
		print_سطر(extra);
	parse_credential_file(fn, c, پوچ, print_سطر, match_password);
	اگر (commit_lock_file(&credential_lock) < 0)
		die_errno("unable to write credential store");
}

ایستا صحیح is_rfc3986_reserved_or_unreserved(نویسه ch)
{
	اگر (is_rfc3986_unreserved(ch))
		بازگشت 1;
	انتخاب (ch) {
		حالت '!': حالت '*': حالت '\'': حالت '(': حالت ')': حالت ';':
		حالت ':': حالت '@': حالت '&': حالت '=': حالت '+': حالت '$':
		حالت ',': حالت '/': حالت '?': حالت '#': حالت '[': حالت ']':
			بازگشت 1;
	}
	بازگشت 0;
}

ایستا تهی store_credential_file(ثابت نویسه *fn, ساختار credential *c)
{
	ساختار strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	اگر (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	اگر (c->path) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->path,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

ایستا تهی store_credential(ثابت ساختار string_list *fns, ساختار credential *c)
{
	ساختار string_list_item *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	اگر (!c->protocol || !(c->host || c->path) || !c->username || !c->password)
		بازگشت;

	for_each_string_list_item(fn, fns)
		اگر (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			بازگشت;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	اگر (fns->nr)
		store_credential_file(fns->اقلام[0].string, c);
}

ایستا تهی remove_credential(ثابت ساختار string_list *fns, ساختار credential *c)
{
	ساختار string_list_item *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	اگر (!c->protocol && !c->host && !c->path && !c->username)
		بازگشت;
	for_each_string_list_item(fn, fns)
		اگر (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, پوچ, 1);
}

ایستا تهی lookup_credential(ثابت ساختار string_list *fns, ساختار credential *c)
{
	ساختار string_list_item *fn;

	for_each_string_list_item(fn, fns)
		اگر (parse_credential_file(fn->string, c, print_entry, پوچ, 0))
			بازگشت; /* Found credential */
}

صحیح cmd_credential_store(صحیح argc,
			 ثابت نویسه **argv,
			 ثابت نویسه *prefix,
			 ساختار repository *repo UNUSED)
{
	ثابت نویسه * ثابت usage[] = {
		"git credential-store [<options>] <action>",
		پوچ
	};
	ثابت نویسه *op;
	ساختار credential c = CREDENTIAL_INIT;
	ساختار string_list fns = STRING_LIST_INIT_DUP;
	نویسه *file = پوچ;
	ساختار option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_END()
	};

	umask(077);

	argc = parse_options(argc, (ثابت نویسه **)argv, prefix, options, usage, 0);
	اگر (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	اگر (file) {
		string_list_append(&fns, file);
	} وگرنه {
		اگر ((file = interpolate_path("~/.git-credentials", 0)))
			string_list_append_nodup(&fns, file);
		file = xdg_config_home("credentials");
		اگر (file)
			string_list_append_nodup(&fns, file);
	}
	اگر (!fns.nr)
		die("unable to set up default path; use --file");

	اگر (credential_read(&c, stdin, CREDENTIAL_OP_HELPER) < 0)
		die("unable to read credential");

	اگر (!strcmp(op, "get"))
		lookup_credential(&fns, &c);
	وگرنه اگر (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	وگرنه اگر (!strcmp(op, "store"))
		store_credential(&fns, &c);
	وگرنه
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	بازگشت 0;
}
