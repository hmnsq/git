/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#تعریف_کن USE_THE_REPOSITORY_VARIABLE
#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "advice.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "path.h"
#ضمن "tag.h"
#ضمن "parse-options.h"
#ضمن "diff.h"
#ضمن "revision.h"
#ضمن "gpg-interface.h"
#ضمن "oid-array.h"
#ضمن "column.h"
#ضمن "ref-filter.h"
#ضمن "date.h"
#ضمن "write-or-die.h"
#ضمن "object-file-convert.h"
#ضمن "trailer.h"

ساكن ثابت حرف * ثابت git_برچسب_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	عدم
};

ساكن بیعلامت صحيح colopts;
ساكن صحيح force_sign_annotate;
ساكن صحيح config_sign_برچسب = -1; /* unspecified */

ساكن صحيح list_برچسبها(بنية ref_filter *filter, بنية ref_ترتیبدهی *ترتیبدهی,
		     بنية ref_format *format)
{
	حرف *مقصد_free = عدم;

	إذا (filter->سطرها == -1)
		filter->سطرها = 0;

	إذا (!format->format) {
		إذا (filter->سطرها) {
			مقصد_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->سطرها);
			format->format = مقصد_free;
		} وإلا
			format->format = "%(refname:lstrip=2)";
	}

	إذا (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_برچسب_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_برچسبها, ترتیبدهی, format);

	حرر(مقصد_free);

	أرجع 0;
}

تعريفنوع صحيح (*each_برچسب_name_fn)(ثابت حرف *name, ثابت حرف *ref,
				ثابت بنية object_id *oid, فراغ *cb_داده);

ساكن صحيح for_each_برچسب_name(ثابت حرف **argv, each_برچسب_name_fn fn,
			     فراغ *cb_داده)
{
	ثابت حرف **p;
	بنية strbuf ref = STRBUF_INIT;
	صحيح had_error = 0;
	بنية object_id oid;

	لكل (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		إذا (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			استمر;
		}
		إذا (fn(*p, ref.buf, &oid, cb_داده))
			had_error = 1;
	}
	strbuf_release(&ref);
	أرجع had_error;
}

ساكن صحيح collect_برچسبها(ثابت حرف *name استفادهنشده, ثابت حرف *ref,
			ثابت بنية object_id *oid, فراغ *cb_داده)
{
	بنية string_list *ref_list = cb_داده;

	string_list_append(ref_list, ref);
	ref_list->اقلام[ref_list->nr - 1].util = oiddup(oid);
	أرجع 0;
}

ساكن صحيح delete_برچسبها(ثابت حرف **argv)
{
	صحيح نتیجه;
	بنية string_list refs_مقصد_delete = STRING_LIST_INIT_DUP;
	بنية string_list_قلم *قلم;

	نتیجه = for_each_برچسب_name(argv, collect_برچسبها, (فراغ *)&refs_مقصد_delete);
	إذا (refs_delete_refs(get_main_ref_store(the_repository), عدم, &refs_مقصد_delete, REF_NO_DEREF))
		نتیجه = 1;

	for_each_string_list_قلم(قلم, &refs_مقصد_delete) {
		ثابت حرف *name = قلم->string;
		بنية object_id *oid = قلم->util;
		إذا (!refs_ref_exists(get_main_ref_store(the_repository), name))
			چاپ_کن(_("Deleted tag '%s' (was %s)\n"),
				قلم->string + 10,
				repo_پیدا_کن_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		حرر(oid);
	}
	string_list_clear(&refs_مقصد_delete, 0);
	أرجع نتیجه;
}

ساكن صحيح verify_برچسب(ثابت حرف *name, ثابت حرف *ref استفادهنشده,
		      ثابت بنية object_id *oid, فراغ *cb_داده)
{
	صحيح flags;
	بنية ref_format *format = cb_داده;
	flags = GPG_VERIFY_VERBOSE;

	إذا (format->format)
		flags = GPG_VERIFY_OMIT_وضعیت;

	إذا (gpg_verify_برچسب(the_repository, oid, name, flags))
		أرجع -1;

	إذا (format->format)
		pretty_print_ref(name, oid, عدم, format);

	أرجع 0;
}

ساكن صحيح do_sign(بنية strbuf *میانگیر, بنية object_id **compat_oid,
		   بنية object_id *compat_oid_buf)
{
	ثابت بنية git_hash_algo *compat = the_repository->compat_hash_algo;
	بنية strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	بنية strbuf compat_buf = STRBUF_INIT;
	حرف *keyid = get_signing_کلید();
	صحيح ret = -1;

	إذا (sign_میانگیر(میانگیر, &sig, keyid, 0))
		اذهب بیرون;

	إذا (compat) {
		ثابت بنية git_hash_algo *algo = the_repository->hash_algo;

		إذا (convert_object_file(the_repository ,&compat_buf, algo, compat,
					میانگیر->buf, میانگیر->len, OBJ_برچسب, 1))
			اذهب بیرون;
		إذا (sign_میانگیر(&compat_buf, &compat_sig, keyid, 0))
			اذهب بیرون;
		جمع_کن_سرآیند_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_برچسب, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	إذا (compat_sig.len)
		جمع_کن_سرآیند_signature(میانگیر, &compat_sig, compat);

	strbuf_addbuf(میانگیر, &sig);
	ret = 0;
بیرون:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	حرر(keyid);
	أرجع ret;
}

ساكن ثابت حرف برچسب_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ساكن ثابت حرف برچسب_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ساكن صحيح git_برچسب_config(ثابت حرف *var, ثابت حرف *مقدار,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "tag.gpgsign")) {
		config_sign_برچسب = git_config_bool(var, مقدار);
		أرجع 0;
	}

	إذا (!strcmp(var, "tag.sort")) {
		إذا (!مقدار)
			أرجع config_error_nonbool(var);
		string_list_append(cb, مقدار);
		أرجع 0;
	}

	إذا (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, مقدار);
		أرجع 0;
	}

	إذا (starts_with(var, "column."))
		أرجع git_ستونی_config(var, مقدار, "tag", &colopts);

	إذا (git_color_config(var, مقدار, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, مقدار, ctx, cb);
}

ساكن فراغ write_برچسب_body(صحيح fd, ثابت بنية object_id *oid)
{
	اندازه_t اندازه;
	تعداد object_type type;
	حرف *buf, *sp, *orig;
	بنية strbuf payload = STRBUF_INIT;
	بنية strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &اندازه);
	إذا (!buf)
		أرجع;
	إذا (parse_signature(buf, اندازه, &payload, &signature)) {
		buf = payload.buf;
		اندازه = payload.len;
	}
	/* skip header */
	sp = جستجوی_رشته(buf, "\n\n");

	إذا (!sp || !اندازه || type != OBJ_برچسب) {
		حرر(buf);
		أرجع;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + اندازه - sp);

	حرر(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ساكن صحيح build_برچسب_object(بنية strbuf *buf, صحيح sign, بنية object_id *نتیجه)
{
	بنية object_id *compat_oid = عدم, compat_oid_buf;
	إذا (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		أرجع error(_("unable to sign the tag"));
	إذا (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_برچسب, نتیجه, compat_oid, 0) < 0)
		أرجع error(_("unable to write tag file"));
	أرجع 0;
}

بنية create_برچسب_options {
	بیعلامت صحيح پیام_given:1;
	بیعلامت صحيح use_editor:1;
	بیعلامت صحيح sign;
	تعداد {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_نما;
};

ساكن ثابت حرف پیام_advice_nested_برچسب[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ساكن فراغ create_برچسب(ثابت بنية object_id *object, ثابت حرف *object_ref,
		       ثابت حرف *برچسب,
		       بنية strbuf *buf, بنية create_برچسب_options *opt,
		       بنية object_id *prev, بنية object_id *نتیجه,
		       بنية strvec *trailer_آرگومانها, حرف *مسیر)
{
	تعداد object_type type;
	بنية strbuf سرآیند = STRBUF_INIT;
	صحيح should_edit;

	type = odb_read_object_info(the_repository->objects, object, عدم);
	إذا (type <= OBJ_NONE)
		die(_("bad object type."));

	إذا (type == OBJ_برچسب)
		advise_if_enabled(ADVICE_NESTED_برچسب, _(پیام_advice_nested_برچسب),
				  برچسب, object_ref);

	strbuf_addf(&سرآیند,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_مقصد_hex(object),
		    type_name(type),
		    برچسب,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->پیام_given;
	إذا (should_edit || trailer_آرگومانها->nr) {
		صحيح fd;

		/* write the template message before editing: */
		fd = xopen(مسیر, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		إذا (opt->پیام_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} وإلا إذا (!is_null_oid(prev)) {
			write_برچسب_body(fd, prev);
		} وإلا {
			بنية strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			إذا (opt->cleanup_نما == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_سطر_str,
				      _(برچسب_template), برچسب, comment_سطر_str);
			وإلا
				strbuf_commented_addf(&buf, comment_سطر_str,
				      _(برچسب_template_nocleanup), برچسب, comment_سطر_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		إذا (trailer_آرگومانها->nr && amend_file_with_trailers(مسیر, trailer_آرگومانها))
			die(_("unable to pass trailers to --trailers"));

		إذا (should_edit) {
			إذا (launch_editor(مسیر, buf, عدم)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				خروج(1);
			}
		} وإلا إذا (trailer_آرگومانها->nr) {
			strbuf_reset(buf);
			إذا (strbuf_read_file(buf, مسیر, 0) < 0)
				die_errno(_("failed to read '%s'"), مسیر);
		}
	}

	إذا (opt->cleanup_نما != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_نما == CLEANUP_ALL ? comment_سطر_str : عدم);

	إذا (!opt->پیام_given && !buf->len)
		die(_("no tag message?"));

	strbuf_درج(buf, 0, سرآیند.buf, سرآیند.len);
	strbuf_release(&سرآیند);

	إذا (build_برچسب_object(buf, opt->sign, نتیجه) < 0) {
		إذا (مسیر)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				مسیر);
		خروج(128);
	}
}

ساكن فراغ create_reflog_پیام(ثابت بنية object_id *oid, بنية strbuf *sb)
{
	تعداد object_type type;
	بنية commit *c;
	حرف *buf;
	اندازه_t اندازه;
	صحيح subject_len = 0;
	ثابت حرف *subject_شروع;

	حرف *rla = getenv("GIT_REFLOG_ACTION");
	إذا (rla) {
		strbuf_addstr(sb, rla);
	} وإلا {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_جمع_کن_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, عدم);
	بدل (type) {
	افتراضي:
		strbuf_addstr(sb, "object of unknown type");
		اكسر;
	حالة OBJ_COMMIT:
		إذا ((buf = odb_read_object(the_repository->objects, oid, &type, &اندازه))) {
			subject_len = پیدا_کن_commit_subject(buf, &subject_شروع);
			strbuf_درج(sb, sb->len, subject_شروع, subject_len);
		} وإلا {
			strbuf_addstr(sb, "commit object");
		}
		حرر(buf);

		إذا ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_نما(SHORT)));
		اكسر;
	حالة OBJ_درخت:
		strbuf_addstr(sb, "tree object");
		اكسر;
	حالة OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		اكسر;
	حالة OBJ_برچسب:
		strbuf_addstr(sb, "other tag object");
		اكسر;
	}
	strbuf_addch(sb, ')');
}

بنية پیام_arg {
	صحيح given;
	بنية strbuf buf;
};

ساكن صحيح parse_پیام_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية پیام_arg *پیام = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (!arg)
		أرجع -1;
	إذا (پیام->buf.len)
		strbuf_addstr(&(پیام->buf), "\n\n");
	strbuf_addstr(&(پیام->buf), arg);
	پیام->given = 1;
	أرجع 0;
}

صحيح cmd_برچسب(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *پیشوند,
	    بنية repository *repo استفادهنشده)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf ref = STRBUF_INIT;
	بنية strbuf reflog_پیام = STRBUF_INIT;
	بنية object_id object, prev;
	ثابت حرف *object_ref, *برچسب;
	بنية create_برچسب_options opt;
	حرف *cleanup_arg = عدم;
	صحيح create_reflog = 0;
	صحيح annotate = 0, force = 0;
	صحيح cmdmode = 0, create_برچسب_object = 0;
	حرف *msgfile = عدم;
	ثابت حرف *keyid = عدم;
	بنية پیام_arg پیام = { .buf = STRBUF_INIT };
	بنية ref_transaction *transaction;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_ترتیبدهی *ترتیبدهی;
	بنية string_list ترتیبدهی_options = STRING_LIST_INIT_DUP;
	بنية ref_format format = REF_FORMAT_INIT;
	بنية strvec trailer_آرگومانها = STRVEC_INIT;
	صحيح icase = 0;
	صحيح edit_flag = 0;
	بنية option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.مقدار = &filter.سطرها,
			.precision = حجم(filter.سطرها),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_گروه(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &پیام, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_پیام_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_آرگومانها, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_گروه(N_("Tag listing options")),
		OPT_ستونی(0, "column", &colopts, N_("show tag list in columns")),
		OPT_شامل_است(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_شامل_است(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالی,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_مرتب_سازی(&ترتیبدهی_options),
		{
			.type = OPTION_CALLBACK,
			.long_name = "points-at",
			.مقدار = &filter.points_at,
			.argh = N_("object"),
			.help = N_("print only tags of the object"),
			.flags = PARSE_OPT_LASTARG_DEFAULT,
			.callback = parse_opt_object_name,
			.defval = (intptr_t) "HEAD",
		},
		OPT_STRING(  0 , "format", &format.format, N_("format"),
			   N_("format to use for the output")),
		OPT__COLOR(&format.use_color, N_("respect format colors")),
		OPT_BOOL('i', "ignore-case", &icase, N_("sorting and filtering are case insensitive")),
		OPT_انتها()
	};
	صحيح ret = 0;
	ثابت حرف *only_درون_list = عدم;
	حرف *مسیر = عدم;

	راهاندازی_ref_filter_porcelain_پیام();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_برچسب_config, &ترتیبدهی_options);
	إذا (!ترتیبدهی_options.nr)
		string_list_append(&ترتیبدهی_options, "refname");

	memset(&opt, 0, حجم(opt));
	filter.سطرها = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, پیشوند, options, git_برچسب_usage, 0);

	إذا (!cmdmode) {
		إذا (argc == 0)
			cmdmode = 'l';
		وإلا إذا (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.سطرها != -1)
			cmdmode = 'l';
	}

	إذا (cmdmode == 'l')
		راهاندازی_auto_pager("tag", 1);

	إذا (trailer_آرگومانها.nr)
		trailer_config_init();

	إذا (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_برچسب > 0;

	إذا (keyid) {
		opt.sign = 1;
		مجموعه_signing_کلید(keyid);
	}
	create_برچسب_object = (opt.sign || annotate || پیام.given || msgfile ||
			     edit_flag || trailer_آرگومانها.nr);

	إذا ((create_برچسب_object || force) && (cmdmode != 0))
		usage_with_options(git_برچسب_usage, options);

	finalize_colopts(&colopts, -1);
	إذا (cmdmode == 'l' && filter.سطرها != -1) {
		إذا (explicitly_enable_ستونی(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	ترتیبدهی = ref_ترتیبدهی_options(&ترتیبدهی_options);
	ref_ترتیبدهی_مجموعه_مرتب_سازی_flags_all(ترتیبدهی, REF_ترتیبدهی_ICASE, icase);
	filter.ignore_case = icase;
	إذا (cmdmode == 'l') {
		إذا (ستونی_فعال(colopts)) {
			بنية ستونی_options copts;
			memset(&copts, 0, حجم(copts));
			copts.padding = 2;
			إذا (پیاپی_ستونی_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_برچسبها(&filter, ترتیبدهی, &format);
		إذا (ستونی_فعال(colopts))
			stop_ستونی_filter();
		اذهب cleanup;
	}
	إذا (filter.سطرها != -1)
		only_درون_list = "-n";
	وإلا إذا (filter.with_commit)
		only_درون_list = "--contains";
	وإلا إذا (filter.no_commit)
		only_درون_list = "--no-contains";
	وإلا إذا (filter.points_at.nr)
		only_درون_list = "--points-at";
	وإلا إذا (filter.reachable_from)
		only_درون_list = "--merged";
	وإلا إذا (filter.unreachable_from)
		only_درون_list = "--no-merged";
	إذا (only_درون_list)
		die(_("the '%s' option is only allowed in list mode"), only_درون_list);
	إذا (cmdmode == 'd') {
		ret = delete_برچسبها(argv);
		اذهب cleanup;
	}
	إذا (cmdmode == 'v') {
		إذا (format.format && verify_ref_format(&format))
			usage_with_options(git_برچسب_usage, options);
		ret = for_each_برچسب_name(argv, verify_برچسب, &format);
		اذهب cleanup;
	}

	إذا (پیام.given || msgfile) {
		إذا (پیام.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		إذا (پیام.given)
			strbuf_addbuf(&buf, &(پیام.buf));
		وإلا {
			إذا (!strcmp(msgfile, "-")) {
				إذا (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} وإلا {
				إذا (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	برچسب = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	إذا (argc > 2)
		die(_("too many arguments"));

	إذا (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	إذا (check_برچسب_ref(&ref, برچسب))
		die(_("'%s' is not a valid tag name."), برچسب);

	إذا (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	وإلا إذا (!force)
		die(_("tag '%s' already exists"), برچسب);

	opt.پیام_given = پیام.given || msgfile;
	opt.use_editor = edit_flag;

	إذا (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_نما = CLEANUP_ALL;
	وإلا إذا (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_نما = CLEANUP_NONE;
	وإلا إذا (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_نما = CLEANUP_SPACE;
	وإلا
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_پیام(&object, &reflog_پیام);

	إذا (create_برچسب_object) {
		إذا (force_sign_annotate && !annotate)
			opt.sign = 1;
		مسیر = repo_git_مسیر(the_repository, "TAG_EDITMSG");
		create_برچسب(&object, object_ref, برچسب, &buf, &opt, &prev, &object,
			   &trailer_آرگومانها, مسیر);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   عدم, عدم,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_پیام.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		إذا (مسیر)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				مسیر);
		die("%s", err.buf);
	}
	إذا (مسیر) {
		unlink_or_warn(مسیر);
		حرر(مسیر);
	}
	ref_transaction_free(transaction);
	إذا (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		چاپ_کن(_("Updated tag '%s' (was %s)\n"), برچسب,
		       repo_پیدا_کن_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_ترتیبدهی_release(ترتیبدهی);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_پیام);
	strbuf_release(&پیام.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_آرگومانها);
	حرر(msgfile);
	أرجع ret;
}
