#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "gettext.h"
#ضمن "lockfile.h"
#ضمن "credential.h"
#ضمن "path.h"
#ضمن "string-list.h"
#ضمن "parse-options.h"
#ضمن "url.h"
#ضمن "write-or-die.h"

ساكن بنية قفل_file credential_قفل;

ساكن صحيح parse_credential_file(ثابت حرف *fn,
				  بنية credential *c,
				  فراغ (*تطابق_cb)(بنية credential *),
				  فراغ (*other_cb)(بنية strbuf *),
				  صحيح تطابق_password)
{
	FILE *fh;
	بنية strbuf سطر = STRBUF_INIT;
	بنية credential مدخل = CREDENTIAL_INIT;
	صحيح یافت_credential = 0;

	fh = fopen(fn, "r");
	إذا (!fh) {
		إذا (errno != ENOENT && errno != EACCES)
			die_errno("unable to open %s", fn);
		أرجع یافت_credential;
	}

	طالما (strbuf_getline_lf(&سطر, fh) != EOF) {
		إذا (!credential_from_url_gently(&مدخل, سطر.buf, 1) &&
		    مدخل.username && مدخل.password &&
		    credential_تطابق(c, &مدخل, تطابق_password)) {
			یافت_credential = 1;
			إذا (تطابق_cb) {
				تطابق_cb(&مدخل);
				اكسر;
			}
		}
		وإلا إذا (other_cb)
			other_cb(&سطر);
	}

	credential_clear(&مدخل);
	strbuf_release(&سطر);
	fclose(fh);
	أرجع یافت_credential;
}

ساكن فراغ print_مدخل(بنية credential *c)
{
	چاپ_کن("username=%s\n", c->username);
	چاپ_کن("password=%s\n", c->password);
}

ساكن فراغ print_سطر(بنية strbuf *buf)
{
	strbuf_addch(buf, '\n');
	write_or_die(get_قفل_file_fd(&credential_قفل), buf->buf, buf->len);
}

ساكن فراغ rewrite_credential_file(ثابت حرف *fn, بنية credential *c,
				    بنية strbuf *extra, صحيح تطابق_password)
{
	صحيح timeout_ms = 1000;

	repo_config_get_int(the_repository, "credentialstore.locktimeoutms", &timeout_ms);
	إذا (hold_قفل_file_for_update_timeout(&credential_قفل, fn, 0, timeout_ms) < 0)
		die_errno(_("unable to get credential storage lock in %d ms"), timeout_ms);
	إذا (extra)
		print_سطر(extra);
	parse_credential_file(fn, c, عدم, print_سطر, تطابق_password);
	إذا (commit_قفل_file(&credential_قفل) < 0)
		die_errno("unable to write credential store");
}

ساكن صحيح is_rfc3986_reserved_or_unreserved(حرف ch)
{
	إذا (is_rfc3986_unreserved(ch))
		أرجع 1;
	بدل (ch) {
		حالة '!': حالة '*': حالة '\'': حالة '(': حالة ')': حالة ';':
		حالة ':': حالة '@': حالة '&': حالة '=': حالة '+': حالة '$':
		حالة ',': حالة '/': حالة '?': حالة '#': حالة '[': حالة ']':
			أرجع 1;
	}
	أرجع 0;
}

ساكن فراغ store_credential_file(ثابت حرف *fn, بنية credential *c)
{
	بنية strbuf buf = STRBUF_INIT;

	strbuf_addf(&buf, "%s://", c->protocol);
	strbuf_addstr_urlencode(&buf, c->username, is_rfc3986_unreserved);
	strbuf_addch(&buf, ':');
	strbuf_addstr_urlencode(&buf, c->password, is_rfc3986_unreserved);
	strbuf_addch(&buf, '@');
	إذا (c->host)
		strbuf_addstr_urlencode(&buf, c->host, is_rfc3986_unreserved);
	إذا (c->مسیر) {
		strbuf_addch(&buf, '/');
		strbuf_addstr_urlencode(&buf, c->مسیر,
					is_rfc3986_reserved_or_unreserved);
	}

	rewrite_credential_file(fn, c, &buf, 0);
	strbuf_release(&buf);
}

ساكن فراغ store_credential(ثابت بنية string_list *fns, بنية credential *c)
{
	بنية string_list_قلم *fn;

	/*
	 * Sanity check that what we are storing is actually sensible.
	 * In particular, we can't make a URL without a protocol field.
	 * Without either a host or pathname (depending on the scheme),
	 * we have no primary key. And without a username and password,
	 * we are not actually storing a credential.
	 */
	إذا (!c->protocol || !(c->host || c->مسیر) || !c->username || !c->password)
		أرجع;

	for_each_string_list_قلم(fn, fns)
		إذا (!access(fn->string, F_OK)) {
			store_credential_file(fn->string, c);
			أرجع;
		}
	/*
	 * Write credential to the filename specified by fns->items[0], thus
	 * creating it
	 */
	إذا (fns->nr)
		store_credential_file(fns->اقلام[0].string, c);
}

ساكن فراغ remove_credential(ثابت بنية string_list *fns, بنية credential *c)
{
	بنية string_list_قلم *fn;

	/*
	 * Sanity check that we actually have something to match
	 * against. The input we get is a restrictive pattern,
	 * so technically a blank credential means "erase everything".
	 * But it is too easy to accidentally send this, since it is equivalent
	 * to empty input. So explicitly disallow it, and require that the
	 * pattern have some actual content to match.
	 */
	إذا (!c->protocol && !c->host && !c->مسیر && !c->username)
		أرجع;
	for_each_string_list_قلم(fn, fns)
		إذا (!access(fn->string, F_OK))
			rewrite_credential_file(fn->string, c, عدم, 1);
}

ساكن فراغ lookup_credential(ثابت بنية string_list *fns, بنية credential *c)
{
	بنية string_list_قلم *fn;

	for_each_string_list_قلم(fn, fns)
		إذا (parse_credential_file(fn->string, c, print_مدخل, عدم, 0))
			أرجع; /* Found credential */
}

صحيح cmd_credential_store(صحيح argc,
			 ثابت حرف **argv,
			 ثابت حرف *پیشوند,
			 بنية repository *repo استفادهنشده)
{
	ثابت حرف * ثابت usage[] = {
		"git credential-store [<options>] <action>",
		عدم
	};
	ثابت حرف *op;
	بنية credential c = CREDENTIAL_INIT;
	بنية string_list fns = STRING_LIST_INIT_DUP;
	حرف *file = عدم;
	بنية option options[] = {
		OPT_STRING(0, "file", &file, "path",
			   "fetch and store credentials in <path>"),
		OPT_انتها()
	};

	umask(077);

	argc = parse_options(argc, (ثابت حرف **)argv, پیشوند, options, usage, 0);
	إذا (argc != 1)
		usage_with_options(usage, options);
	op = argv[0];

	إذا (file) {
		string_list_append(&fns, file);
	} وإلا {
		إذا ((file = interpolate_مسیر("~/.git-credentials", 0)))
			string_list_append_nodup(&fns, file);
		file = xdg_config_home("credentials");
		إذا (file)
			string_list_append_nodup(&fns, file);
	}
	إذا (!fns.nr)
		die("unable to set up default path; use --file");

	إذا (credential_read(&c, stdin, CREDENTIAL_OP_HELPER) < 0)
		die("unable to read credential");

	إذا (!strcmp(op, "get"))
		lookup_credential(&fns, &c);
	وإلا إذا (!strcmp(op, "erase"))
		remove_credential(&fns, &c);
	وإلا إذا (!strcmp(op, "store"))
		store_credential(&fns, &c);
	وإلا
		; /* Ignore unknown operation. */

	string_list_clear(&fns, 0);
	credential_clear(&c);
	أرجع 0;
}
