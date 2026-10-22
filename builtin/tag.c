/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "advice.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "path.h"
#شامل "tag.h"
#شامل "parse-options.h"
#شامل "diff.h"
#شامل "revision.h"
#شامل "gpg-interface.h"
#شامل "oid-array.h"
#شامل "column.h"
#شامل "ref-filter.h"
#شامل "date.h"
#شامل "write-or-die.h"
#شامل "object-file-convert.h"
#شامل "trailer.h"

ساکن ثابت حرف * ثابت git_لیبل_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	تہی
};

ساکن غیرعلامتی صحیح colopts;
ساکن صحیح force_sign_annotate;
ساکن صحیح config_sign_لیبل = -1; /* unspecified */

ساکن صحیح list_لیبل_فہرست(ساخت ref_filter *filter, ساخت ref_ترتیب_کاری *ترتیب_کاری,
		     ساخت ref_format *format)
{
	حرف *مقصد_free = تہی;

	اگر (filter->سطریں == -1)
		filter->سطریں = 0;

	اگر (!format->format) {
		اگر (filter->سطریں) {
			مقصد_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->سطریں);
			format->format = مقصد_free;
		} ورنہ
			format->format = "%(refname:lstrip=2)";
	}

	اگر (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_لیبل_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_لیبل_فہرست, ترتیب_کاری, format);

	آزاد(مقصد_free);

	واپس 0;
}

قسمتعریف صحیح (*each_لیبل_name_fn)(ثابت حرف *name, ثابت حرف *ref,
				ثابت ساخت object_id *oid, خالی *cb_ڈیٹا);

ساکن صحیح for_each_لیبل_name(ثابت حرف **argv, each_لیبل_name_fn fn,
			     خالی *cb_ڈیٹا)
{
	ثابت حرف **p;
	ساخت strbuf ref = STRBUF_INIT;
	صحیح had_error = 0;
	ساخت object_id oid;

	برائے (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		اگر (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			جاری;
		}
		اگر (fn(*p, ref.buf, &oid, cb_ڈیٹا))
			had_error = 1;
	}
	strbuf_release(&ref);
	واپس had_error;
}

ساکن صحیح collect_لیبل_فہرست(ثابت حرف *name غیر_مستعمل, ثابت حرف *ref,
			ثابت ساخت object_id *oid, خالی *cb_ڈیٹا)
{
	ساخت string_list *ref_list = cb_ڈیٹا;

	string_list_append(ref_list, ref);
	ref_list->اشیاء[ref_list->nr - 1].util = oiddup(oid);
	واپس 0;
}

ساکن صحیح delete_لیبل_فہرست(ثابت حرف **argv)
{
	صحیح نتیجہ;
	ساخت string_list refs_مقصد_delete = STRING_LIST_INIT_DUP;
	ساخت string_list_شے *شے;

	نتیجہ = for_each_لیبل_name(argv, collect_لیبل_فہرست, (خالی *)&refs_مقصد_delete);
	اگر (refs_delete_refs(get_main_ref_store(the_repository), تہی, &refs_مقصد_delete, REF_NO_DEREF))
		نتیجہ = 1;

	for_each_string_list_شے(شے, &refs_مقصد_delete) {
		ثابت حرف *name = شے->string;
		ساخت object_id *oid = شے->util;
		اگر (!refs_ref_exists(get_main_ref_store(the_repository), name))
			لکھو(_("Deleted tag '%s' (was %s)\n"),
				شے->string + 10,
				repo_ڈھونڈو_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		آزاد(oid);
	}
	string_list_clear(&refs_مقصد_delete, 0);
	واپس نتیجہ;
}

ساکن صحیح verify_لیبل(ثابت حرف *name, ثابت حرف *ref غیر_مستعمل,
		      ثابت ساخت object_id *oid, خالی *cb_ڈیٹا)
{
	صحیح flags;
	ساخت ref_format *format = cb_ڈیٹا;
	flags = GPG_VERIFY_VERBOSE;

	اگر (format->format)
		flags = GPG_VERIFY_OMIT_حالت;

	اگر (gpg_verify_لیبل(the_repository, oid, name, flags))
		واپس -1;

	اگر (format->format)
		pretty_print_ref(name, oid, تہی, format);

	واپس 0;
}

ساکن صحیح do_sign(ساخت strbuf *بفر, ساخت object_id **compat_oid,
		   ساخت object_id *compat_oid_buf)
{
	ثابت ساخت git_hash_algo *compat = the_repository->compat_hash_algo;
	ساخت strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ساخت strbuf compat_buf = STRBUF_INIT;
	حرف *keyid = get_signing_کلید();
	صحیح ret = -1;

	اگر (sign_بفر(بفر, &sig, keyid, 0))
		جاؤ برآمد;

	اگر (compat) {
		ثابت ساخت git_hash_algo *algo = the_repository->hash_algo;

		اگر (convert_object_file(the_repository ,&compat_buf, algo, compat,
					بفر->buf, بفر->len, OBJ_لیبل, 1))
			جاؤ برآمد;
		اگر (sign_بفر(&compat_buf, &compat_sig, keyid, 0))
			جاؤ برآمد;
		جوڑو_سرخی_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_لیبل, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	اگر (compat_sig.len)
		جوڑو_سرخی_signature(بفر, &compat_sig, compat);

	strbuf_addbuf(بفر, &sig);
	ret = 0;
برآمد:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	آزاد(keyid);
	واپس ret;
}

ساکن ثابت حرف لیبل_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ساکن ثابت حرف لیبل_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ساکن صحیح git_لیبل_config(ثابت حرف *var, ثابت حرف *قدر,
			  ثابت ساخت config_context *ctx, خالی *cb)
{
	اگر (!strcmp(var, "tag.gpgsign")) {
		config_sign_لیبل = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "tag.sort")) {
		اگر (!قدر)
			واپس config_error_nonbool(var);
		string_list_append(cb, قدر);
		واپس 0;
	}

	اگر (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, قدر);
		واپس 0;
	}

	اگر (starts_with(var, "column."))
		واپس git_عمود_config(var, قدر, "tag", &colopts);

	اگر (git_color_config(var, قدر, cb) < 0)
		واپس -1;

	واپس git_default_config(var, قدر, ctx, cb);
}

ساکن خالی write_لیبل_body(صحیح fd, ثابت ساخت object_id *oid)
{
	سائز_t سائز;
	شمار object_type type;
	حرف *buf, *sp, *orig;
	ساخت strbuf payload = STRBUF_INIT;
	ساخت strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &سائز);
	اگر (!buf)
		واپس;
	اگر (parse_signature(buf, سائز, &payload, &signature)) {
		buf = payload.buf;
		سائز = payload.len;
	}
	/* skip header */
	sp = لڑی_تلاش(buf, "\n\n");

	اگر (!sp || !سائز || type != OBJ_لیبل) {
		آزاد(buf);
		واپس;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + سائز - sp);

	آزاد(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ساکن صحیح build_لیبل_object(ساخت strbuf *buf, صحیح sign, ساخت object_id *نتیجہ)
{
	ساخت object_id *compat_oid = تہی, compat_oid_buf;
	اگر (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		واپس error(_("unable to sign the tag"));
	اگر (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_لیبل, نتیجہ, compat_oid, 0) < 0)
		واپس error(_("unable to write tag file"));
	واپس 0;
}

ساخت create_لیبل_options {
	غیرعلامتی صحیح پیغام_given:1;
	غیرعلامتی صحیح use_editor:1;
	غیرعلامتی صحیح sign;
	شمار {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_منوال;
};

ساکن ثابت حرف پیغام_advice_nested_لیبل[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ساکن خالی create_لیبل(ثابت ساخت object_id *object, ثابت حرف *object_ref,
		       ثابت حرف *لیبل,
		       ساخت strbuf *buf, ساخت create_لیبل_options *opt,
		       ساخت object_id *پچھلا_2, ساخت object_id *نتیجہ,
		       ساخت strvec *trailer_دلائل, حرف *راستہ)
{
	شمار object_type type;
	ساخت strbuf سرخی = STRBUF_INIT;
	صحیح should_edit;

	type = odb_read_object_info(the_repository->objects, object, تہی);
	اگر (type <= OBJ_NONE)
		die(_("bad object type."));

	اگر (type == OBJ_لیبل)
		advise_if_enabled(ADVICE_NESTED_لیبل, _(پیغام_advice_nested_لیبل),
				  لیبل, object_ref);

	strbuf_addf(&سرخی,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_مقصد_hex(object),
		    type_name(type),
		    لیبل,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->پیغام_given;
	اگر (should_edit || trailer_دلائل->nr) {
		صحیح fd;

		/* write the template message before editing: */
		fd = xopen(راستہ, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		اگر (opt->پیغام_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} ورنہ اگر (!is_null_oid(پچھلا_2)) {
			write_لیبل_body(fd, پچھلا_2);
		} ورنہ {
			ساخت strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			اگر (opt->cleanup_منوال == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_سطر_str,
				      _(لیبل_template), لیبل, comment_سطر_str);
			ورنہ
				strbuf_commented_addf(&buf, comment_سطر_str,
				      _(لیبل_template_nocleanup), لیبل, comment_سطر_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		اگر (trailer_دلائل->nr && amend_file_with_trailers(راستہ, trailer_دلائل))
			die(_("unable to pass trailers to --trailers"));

		اگر (should_edit) {
			اگر (launch_editor(راستہ, buf, تہی)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				باہر_نکلو(1);
			}
		} ورنہ اگر (trailer_دلائل->nr) {
			strbuf_reset(buf);
			اگر (strbuf_read_file(buf, راستہ, 0) < 0)
				die_errno(_("failed to read '%s'"), راستہ);
		}
	}

	اگر (opt->cleanup_منوال != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_منوال == CLEANUP_ALL ? comment_سطر_str : تہی);

	اگر (!opt->پیغام_given && !buf->len)
		die(_("no tag message?"));

	strbuf_داخل_کرو(buf, 0, سرخی.buf, سرخی.len);
	strbuf_release(&سرخی);

	اگر (build_لیبل_object(buf, opt->sign, نتیجہ) < 0) {
		اگر (راستہ)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				راستہ);
		باہر_نکلو(128);
	}
}

ساکن خالی create_reflog_پیغام(ثابت ساخت object_id *oid, ساخت strbuf *sb)
{
	شمار object_type type;
	ساخت commit *c;
	حرف *buf;
	سائز_t سائز;
	صحیح subject_len = 0;
	ثابت حرف *subject_آغاز;

	حرف *rla = getenv("GIT_REFLOG_ACTION");
	اگر (rla) {
		strbuf_addstr(sb, rla);
	} ورنہ {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_جوڑو_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, تہی);
	انتخاب (type) {
	طےشدہ:
		strbuf_addstr(sb, "object of unknown type");
		توڑو;
	حالت OBJ_COMMIT:
		اگر ((buf = odb_read_object(the_repository->objects, oid, &type, &سائز))) {
			subject_len = ڈھونڈو_commit_subject(buf, &subject_آغاز);
			strbuf_داخل_کرو(sb, sb->len, subject_آغاز, subject_len);
		} ورنہ {
			strbuf_addstr(sb, "commit object");
		}
		آزاد(buf);

		اگر ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_منوال(SHORT)));
		توڑو;
	حالت OBJ_درخت:
		strbuf_addstr(sb, "tree object");
		توڑو;
	حالت OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		توڑو;
	حالت OBJ_لیبل:
		strbuf_addstr(sb, "other tag object");
		توڑو;
	}
	strbuf_addch(sb, ')');
}

ساخت پیغام_arg {
	صحیح given;
	ساخت strbuf buf;
};

ساکن صحیح parse_پیغام_arg(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت پیغام_arg *پیغام = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!arg)
		واپس -1;
	اگر (پیغام->buf.len)
		strbuf_addstr(&(پیغام->buf), "\n\n");
	strbuf_addstr(&(پیغام->buf), arg);
	پیغام->given = 1;
	واپس 0;
}

صحیح cmd_لیبل(صحیح argc,
	    ثابت حرف **argv,
	    ثابت حرف *سابقہ,
	    ساخت repository *repo غیر_مستعمل)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strbuf ref = STRBUF_INIT;
	ساخت strbuf reflog_پیغام = STRBUF_INIT;
	ساخت object_id object, پچھلا_2;
	ثابت حرف *object_ref, *لیبل;
	ساخت create_لیبل_options opt;
	حرف *cleanup_arg = تہی;
	صحیح create_reflog = 0;
	صحیح annotate = 0, force = 0;
	صحیح cmdmode = 0, create_لیبل_object = 0;
	حرف *msgfile = تہی;
	ثابت حرف *keyid = تہی;
	ساخت پیغام_arg پیغام = { .buf = STRBUF_INIT };
	ساخت ref_transaction *transaction;
	ساخت strbuf err = STRBUF_INIT;
	ساخت ref_filter filter = REF_FILTER_INIT;
	ساخت ref_ترتیب_کاری *ترتیب_کاری;
	ساخت string_list ترتیب_کاری_options = STRING_LIST_INIT_DUP;
	ساخت ref_format format = REF_FORMAT_INIT;
	ساخت strvec trailer_دلائل = STRVEC_INIT;
	صحیح icase = 0;
	صحیح edit_flag = 0;
	ساخت option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.قدر = &filter.سطریں,
			.precision = حجم(filter.سطریں),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_گروہ(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &پیغام, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_پیغام_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_دلائل, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_گروہ(N_("Tag listing options")),
		OPT_عمود(0, "column", &colopts, N_("show tag list in columns")),
		OPT_شامل_ہے(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_شامل_ہے(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_خالی_خانے,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_ترتیب_دو(&ترتیب_کاری_options),
		{
			.type = OPTION_CALLBACK,
			.long_name = "points-at",
			.قدر = &filter.points_at,
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
		OPT_انتہا()
	};
	صحیح ret = 0;
	ثابت حرف *only_اندر_list = تہی;
	حرف *راستہ = تہی;

	تیاری_ref_filter_porcelain_پیغام();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_لیبل_config, &ترتیب_کاری_options);
	اگر (!ترتیب_کاری_options.nr)
		string_list_append(&ترتیب_کاری_options, "refname");

	memset(&opt, 0, حجم(opt));
	filter.سطریں = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, سابقہ, options, git_لیبل_usage, 0);

	اگر (!cmdmode) {
		اگر (argc == 0)
			cmdmode = 'l';
		ورنہ اگر (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.سطریں != -1)
			cmdmode = 'l';
	}

	اگر (cmdmode == 'l')
		تیاری_auto_pager("tag", 1);

	اگر (trailer_دلائل.nr)
		trailer_config_init();

	اگر (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_لیبل > 0;

	اگر (keyid) {
		opt.sign = 1;
		سیٹ_signing_کلید(keyid);
	}
	create_لیبل_object = (opt.sign || annotate || پیغام.given || msgfile ||
			     edit_flag || trailer_دلائل.nr);

	اگر ((create_لیبل_object || force) && (cmdmode != 0))
		usage_with_options(git_لیبل_usage, options);

	finalize_colopts(&colopts, -1);
	اگر (cmdmode == 'l' && filter.سطریں != -1) {
		اگر (explicitly_enable_عمود(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	ترتیب_کاری = ref_ترتیب_کاری_options(&ترتیب_کاری_options);
	ref_ترتیب_کاری_سیٹ_ترتیب_دو_flags_all(ترتیب_کاری, REF_ترتیب_کاری_ICASE, icase);
	filter.ignore_case = icase;
	اگر (cmdmode == 'l') {
		اگر (عمود_فعال(colopts)) {
			ساخت عمود_options copts;
			memset(&copts, 0, حجم(copts));
			copts.padding = 2;
			اگر (تسلسل_عمود_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_لیبل_فہرست(&filter, ترتیب_کاری, &format);
		اگر (عمود_فعال(colopts))
			stop_عمود_filter();
		جاؤ cleanup;
	}
	اگر (filter.سطریں != -1)
		only_اندر_list = "-n";
	ورنہ اگر (filter.with_commit)
		only_اندر_list = "--contains";
	ورنہ اگر (filter.no_commit)
		only_اندر_list = "--no-contains";
	ورنہ اگر (filter.points_at.nr)
		only_اندر_list = "--points-at";
	ورنہ اگر (filter.reachable_from)
		only_اندر_list = "--merged";
	ورنہ اگر (filter.unreachable_from)
		only_اندر_list = "--no-merged";
	اگر (only_اندر_list)
		die(_("the '%s' option is only allowed in list mode"), only_اندر_list);
	اگر (cmdmode == 'd') {
		ret = delete_لیبل_فہرست(argv);
		جاؤ cleanup;
	}
	اگر (cmdmode == 'v') {
		اگر (format.format && verify_ref_format(&format))
			usage_with_options(git_لیبل_usage, options);
		ret = for_each_لیبل_name(argv, verify_لیبل, &format);
		جاؤ cleanup;
	}

	اگر (پیغام.given || msgfile) {
		اگر (پیغام.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		اگر (پیغام.given)
			strbuf_addbuf(&buf, &(پیغام.buf));
		ورنہ {
			اگر (!strcmp(msgfile, "-")) {
				اگر (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} ورنہ {
				اگر (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	لیبل = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	اگر (argc > 2)
		die(_("too many arguments"));

	اگر (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	اگر (check_لیبل_ref(&ref, لیبل))
		die(_("'%s' is not a valid tag name."), لیبل);

	اگر (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &پچھلا_2))
		oidclr(&پچھلا_2, the_repository->hash_algo);
	ورنہ اگر (!force)
		die(_("tag '%s' already exists"), لیبل);

	opt.پیغام_given = پیغام.given || msgfile;
	opt.use_editor = edit_flag;

	اگر (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_منوال = CLEANUP_ALL;
	ورنہ اگر (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_منوال = CLEANUP_NONE;
	ورنہ اگر (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_منوال = CLEANUP_SPACE;
	ورنہ
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_پیغام(&object, &reflog_پیغام);

	اگر (create_لیبل_object) {
		اگر (force_sign_annotate && !annotate)
			opt.sign = 1;
		راستہ = repo_git_راستہ(the_repository, "TAG_EDITMSG");
		create_لیبل(&object, object_ref, لیبل, &buf, &opt, &پچھلا_2, &object,
			   &trailer_دلائل, راستہ);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &پچھلا_2,
				   تہی, تہی,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_پیغام.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		اگر (راستہ)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				راستہ);
		die("%s", err.buf);
	}
	اگر (راستہ) {
		unlink_or_warn(راستہ);
		آزاد(راستہ);
	}
	ref_transaction_free(transaction);
	اگر (force && !is_null_oid(&پچھلا_2) && !oideq(&پچھلا_2, &object))
		لکھو(_("Updated tag '%s' (was %s)\n"), لیبل,
		       repo_ڈھونڈو_unique_abbrev(the_repository, &پچھلا_2, DEFAULT_ABBREV));

cleanup:
	ref_ترتیب_کاری_release(ترتیب_کاری);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_پیغام);
	strbuf_release(&پیغام.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_دلائل);
	آزاد(msgfile);
	واپس ret;
}
