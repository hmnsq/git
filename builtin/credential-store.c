#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن ساخت تالا_file credential_تالا;

ساکن صحیح parse_credential_file(ثابت حرف *fn,
				  ساخت credential *c,
				  خالی (*مطابقت_cb)(ساخت credential *),
				  خالی (*other_cb)(ساخت strbuf *),
				  صحیح مطابقت_password)
{
	FILE *fh;
	ساخت strbuf سطر = STRBUF_INIT;
	ساخت credential اندراج_2 = CREDENTIAL_INIT;
	صحیح ملا_credential = 0;

	fh = fopen(fn, "r");
	اگر (!fh) {
		اگر (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		واپس ملا_credential;
	}

	جب (strbuf_getline_lf(&سطر, fh) != EOF) {
		اگر (!credential_from_url_gently(&اندراج_2, سطر.buf, 1) &&
		    اندراج_2.username && اندراج_2.password &&
		    credential_مطابقت(c, &اندراج_2, مطابقت_password)) {
			ملا_credential = 1;
			اگر (مطابقت_cb) {
				مطابقت_cb(&اندراج_2);
				توڑو;
			}
		}
		ورنہ اگر (other_cb)
			other_cb(&سطر);
	}

	credential_clear(&اندراج_2);
	strbuf_release(&سطر);
	fclose(fh);
	واپس ملا_credential;
}

ساکن خالی print_اندراج(ساخت credential *c)
{
	لکھو("username=%s\n", c->username);
	لکھو("password=%s\n", c->password);
}

ساکن خالی print_سطر(ساخت strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_تالا_file_fd(&credential_تالا), buf->buf, buf->len);
}

ساکن خالی rewrite_credential_file(ثابت حرف *fn, ساخت credential *c,
				    ساخت strbuf *extra, صحیح مطابقت_password)
{
	صحیح timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	اگر (hold_تالا_file_for_update_timeout(&credential_تالا, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	اگر (extra)
		print_سطر(extra);
	parse_credential_file(fn, c, تہی, print_سطر, مطابقت_password);
	اگر (commit_تالا_file(&credential_تالا) < 0)
		die_errno("unable to write credential store");
}

ساکن صحیح is_rfc3986_reserved_or_unreserved(حرف ch)
{
	اگر (is_rfc3986_unreserved(ch))
		واپس 1;
	انتخاب (ch) {
		حالت '!': حالت '*': حالت '\'': حالت '(': حالت ')': حالت ';':
		حالت ':': حالت '@': حالت '&': حالت '=': حالت '+': حالت '$':
		حالت ',': حالت '/': حالت '?': حالت '#': حالت '[': حالت ']':
			واپس 1;
	}
	واپس 0;
}

ساکن خالی store_credential_file(ثابت حرف *fn, ساخت credential *c)
{
	ساخت strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	اگر (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	اگر (c->راستہ) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->راستہ,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

ساکن خالی store_credential(ثابت ساخت string_list *fns, ساخت credential *c)
{
	ساخت string_list_شے *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	اگر (!c->protocol || !(c->host || c->راستہ) || !c->username || !c->password)
		واپس;

	for_each_string_list_شے(fn, fns)
		اگر (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			واپس;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	اگر (fns->nr)
		store_credential_file(fns->اشیاء[0].string, c);
}

ساکن خالی remove_credential(ثابت ساخت string_list *fns, ساخت credential *c)
{
	ساخت string_list_شے *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	اگر (!c->protocol && !c->host && !c->راستہ && !c->username)
		واپس;
	for_each_string_list_شے(fn, fns)
		اگر (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, تہی, 1);
}

ساکن خالی lookup_credential(ثابت ساخت string_list *fns, ساخت credential *c)
{
	ساخت string_list_شے *fn;

	for_each_string_list_شے(fn, fns)
		اگر (parse_credential_file(fn->string, c, print_اندراج, تہی, 0))
			واپس; /* Found credential */
}

صحیح cmd_credential_store(صحیح argc,
			 ثابت حرف **argv,
			 ثابت حرف *سابقہ,
			 ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف * ثابت usage[] = {
		"git credential-store [<options>] <action>",
		تہی
	};
	ثابت حرف *op;
	ساخت credential c = CREDENTIAL_INIT;
	ساخت string_list fns = STRING_LIST_INIT_DUP;
	حرف *file = تہی;
	ساخت option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_انتہا()
	};

	umask(077);

	argc = parse_options(argc, (ثابت حرف **)argv, سابقہ, options, usage, 0);
	اگر (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	اگر (file) {
		string_list_append(&fns, file);
	} ورنہ {
		اگر ((file = interpolate_راستہ("~/.git-credentials", 0)))
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
	ورنہ اگر (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	ورنہ اگر (!strcmp(op, "store"))
		store_credential(&fns, &c);
	ورنہ
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	واپس 0;
}
