/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ایستا ثابت نویسه * ثابت git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	پوچ
};

ایستا unsigned صحیح colopts;
ایستا صحیح force_sign_annotate;
ایستا صحیح config_sign_tag = -1; /* unspecified */

ایستا صحیح list_tags(ساختار ref_filter *filter, ساختار ref_sorting *sorting,
		     ساختار ref_format *format)
{
	نویسه *to_free = پوچ;

	اگر (filter->سطرها == -1)
		filter->سطرها = 0;

	اگر (!format->format) {
		اگر (filter->سطرها) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->سطرها);
			format->format = to_free;
		} وگرنه
			format->format = "%(refname:lstrip=2)";
	}

	اگر (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	آزاد(to_free);

	بازگشت 0;
}

تعریفنوع صحیح (*each_tag_name_fn)(ثابت نویسه *name, ثابت نویسه *ref,
				ثابت ساختار object_id *oid, تهی *cb_data);

ایستا صحیح for_each_tag_name(ثابت نویسه **argv, each_tag_name_fn fn,
			     تهی *cb_data)
{
	ثابت نویسه **p;
	ساختار strbuf ref = STRBUF_INIT;
	صحیح had_error = 0;
	ساختار object_id oid;

	برای (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		اگر (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			ادامه;
		}
		اگر (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	بازگشت had_error;
}

ایستا صحیح collect_tags(ثابت نویسه *name UNUSED, ثابت نویسه *ref,
			ثابت ساختار object_id *oid, تهی *cb_data)
{
	ساختار string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->اقلام[ref_list->nr - 1].util = oiddup(oid);
	بازگشت 0;
}

ایستا صحیح delete_tags(ثابت نویسه **argv)
{
	صحیح result;
	ساختار string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ساختار string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (تهی *)&refs_to_delete);
	اگر (refs_delete_refs(get_main_ref_store(the_repository), پوچ, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ثابت نویسه *name = item->string;
		ساختار object_id *oid = item->util;
		اگر (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		آزاد(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	بازگشت result;
}

ایستا صحیح verify_tag(ثابت نویسه *name, ثابت نویسه *ref UNUSED,
		      ثابت ساختار object_id *oid, تهی *cb_data)
{
	صحیح flags;
	ساختار ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	اگر (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	اگر (gpg_verify_tag(the_repository, oid, name, flags))
		بازگشت -1;

	اگر (format->format)
		pretty_print_ref(name, oid, پوچ, format);

	بازگشت 0;
}

ایستا صحیح do_sign(ساختار strbuf *buffer, ساختار object_id **compat_oid,
		   ساختار object_id *compat_oid_buf)
{
	ثابت ساختار git_hash_algo *compat = the_repository->compat_hash_algo;
	ساختار strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ساختار strbuf compat_buf = STRBUF_INIT;
	نویسه *keyid = get_signing_کلید();
	صحیح ret = -1;

	اگر (sign_buffer(buffer, &sig, keyid, 0))
		برو out;

	اگر (compat) {
		ثابت ساختار git_hash_algo *algo = the_repository->hash_algo;

		اگر (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			برو out;
		اگر (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			برو out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	اگر (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	آزاد(keyid);
	بازگشت ret;
}

ایستا ثابت نویسه tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ایستا ثابت نویسه tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ایستا صحیح git_tag_config(ثابت نویسه *var, ثابت نویسه *مقدار,
			  ثابت ساختار config_context *ctx, تهی *cb)
{
	اگر (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "tag.sort")) {
		اگر (!مقدار)
			بازگشت config_error_nonbool(var);
		string_list_append(cb, مقدار);
		بازگشت 0;
	}

	اگر (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, مقدار);
		بازگشت 0;
	}

	اگر (starts_with(var, "column."))
		بازگشت git_column_config(var, مقدار, "tag", &colopts);

	اگر (git_color_config(var, مقدار, cb) < 0)
		بازگشت -1;

	بازگشت git_default_config(var, مقدار, ctx, cb);
}

ایستا تهی write_tag_body(صحیح fd, ثابت ساختار object_id *oid)
{
	size_t size;
	شمارش object_type type;
	نویسه *buf, *sp, *orig;
	ساختار strbuf payload = STRBUF_INIT;
	ساختار strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	اگر (!buf)
		بازگشت;
	اگر (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	اگر (!sp || !size || type != OBJ_TAG) {
		آزاد(buf);
		بازگشت;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	آزاد(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ایستا صحیح build_tag_object(ساختار strbuf *buf, صحیح sign, ساختار object_id *result)
{
	ساختار object_id *compat_oid = پوچ, compat_oid_buf;
	اگر (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		بازگشت error(_("unable to sign the tag"));
	اگر (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		بازگشت error(_("unable to write tag file"));
	بازگشت 0;
}

ساختار create_tag_options {
	unsigned صحیح message_given:1;
	unsigned صحیح use_editor:1;
	unsigned صحیح sign;
	شمارش {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

ایستا ثابت نویسه message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ایستا تهی create_tag(ثابت ساختار object_id *object, ثابت نویسه *object_ref,
		       ثابت نویسه *tag,
		       ساختار strbuf *buf, ساختار create_tag_options *opt,
		       ساختار object_id *prev, ساختار object_id *result,
		       ساختار strvec *trailer_args, نویسه *path)
{
	شمارش object_type type;
	ساختار strbuf header = STRBUF_INIT;
	صحیح should_edit;

	type = odb_read_object_info(the_repository->objects, object, پوچ);
	اگر (type <= OBJ_NONE)
		die(_("bad object type."));

	اگر (type == OBJ_TAG)
		advise_if_enabled(ADVICE_NESTED_TAG, _(message_advice_nested_tag),
				  tag, object_ref);

	strbuf_addf(&header,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_to_hex(object),
		    type_name(type),
		    tag,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->message_given;
	اگر (should_edit || trailer_args->nr) {
		صحیح fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		اگر (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} وگرنه اگر (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} وگرنه {
			ساختار strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			اگر (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_سطر_str,
				      _(tag_template), tag, comment_سطر_str);
			وگرنه
				strbuf_commented_addf(&buf, comment_سطر_str,
				      _(tag_template_nocleanup), tag, comment_سطر_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		اگر (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		اگر (should_edit) {
			اگر (launch_editor(path, buf, پوچ)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} وگرنه اگر (trailer_args->nr) {
			strbuf_reset(buf);
			اگر (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	اگر (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_سطر_str : پوچ);

	اگر (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_درج(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	اگر (build_tag_object(buf, opt->sign, result) < 0) {
		اگر (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

ایستا تهی create_reflog_پیام(ثابت ساختار object_id *oid, ساختار strbuf *sb)
{
	شمارش object_type type;
	ساختار commit *c;
	نویسه *buf;
	size_t size;
	صحیح subject_len = 0;
	ثابت نویسه *subject_start;

	نویسه *rla = getenv("GIT_REFLOG_ACTION");
	اگر (rla) {
		strbuf_addstr(sb, rla);
	} وگرنه {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, پوچ);
	انتخاب (type) {
	پیشفرض:
		strbuf_addstr(sb, "object of unknown type");
		بشکن;
	حالت OBJ_COMMIT:
		اگر ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_درج(sb, sb->len, subject_start, subject_len);
		} وگرنه {
			strbuf_addstr(sb, "commit object");
		}
		آزاد(buf);

		اگر ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		بشکن;
	حالت OBJ_درخت:
		strbuf_addstr(sb, "tree object");
		بشکن;
	حالت OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		بشکن;
	حالت OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		بشکن;
	}
	strbuf_addch(sb, ')');
}

ساختار پیام_arg {
	صحیح given;
	ساختار strbuf buf;
};

ایستا صحیح parse_پیام_arg(ثابت ساختار option *opt, ثابت نویسه *arg, صحیح unset)
{
	ساختار پیام_arg *پیام = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	اگر (!arg)
		بازگشت -1;
	اگر (پیام->buf.len)
		strbuf_addstr(&(پیام->buf), "\n\n");
	strbuf_addstr(&(پیام->buf), arg);
	پیام->given = 1;
	بازگشت 0;
}

صحیح cmd_tag(صحیح argc,
	    ثابت نویسه **argv,
	    ثابت نویسه *prefix,
	    ساختار repository *repo UNUSED)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار strbuf ref = STRBUF_INIT;
	ساختار strbuf reflog_پیام = STRBUF_INIT;
	ساختار object_id object, prev;
	ثابت نویسه *object_ref, *tag;
	ساختار create_tag_options opt;
	نویسه *cleanup_arg = پوچ;
	صحیح create_reflog = 0;
	صحیح annotate = 0, force = 0;
	صحیح cmdmode = 0, create_tag_object = 0;
	نویسه *msgfile = پوچ;
	ثابت نویسه *keyid = پوچ;
	ساختار پیام_arg پیام = { .buf = STRBUF_INIT };
	ساختار ref_transaction *transaction;
	ساختار strbuf err = STRBUF_INIT;
	ساختار ref_filter filter = REF_FILTER_INIT;
	ساختار ref_sorting *sorting;
	ساختار string_list sorting_options = STRING_LIST_INIT_DUP;
	ساختار ref_format format = REF_FORMAT_INIT;
	ساختار strvec trailer_args = STRVEC_INIT;
	صحیح icase = 0;
	صحیح edit_flag = 0;
	ساختار option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.مقدار = &filter.سطرها,
			.precision = اندازه(filter.سطرها),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_GROUP(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &پیام, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_پیام_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_args, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_GROUP(N_("Tag listing options")),
		OPT_COLUMN(0, "column", &colopts, N_("show tag list in columns")),
		OPT_CONTAINS(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_CONTAINS(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_empty,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_SORT(&sorting_options),
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
		OPT_END()
	};
	صحیح ret = 0;
	ثابت نویسه *only_in_list = پوچ;
	نویسه *path = پوچ;

	setup_ref_filter_porcelain_پیام();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	اگر (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, اندازه(opt));
	filter.سطرها = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	اگر (!cmdmode) {
		اگر (argc == 0)
			cmdmode = 'l';
		وگرنه اگر (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.سطرها != -1)
			cmdmode = 'l';
	}

	اگر (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	اگر (trailer_args.nr)
		trailer_config_init();

	اگر (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	اگر (keyid) {
		opt.sign = 1;
		set_signing_کلید(keyid);
	}
	create_tag_object = (opt.sign || annotate || پیام.given || msgfile ||
			     edit_flag || trailer_args.nr);

	اگر ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	اگر (cmdmode == 'l' && filter.سطرها != -1) {
		اگر (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	اگر (cmdmode == 'l') {
		اگر (column_active(colopts)) {
			ساختار column_options copts;
			memset(&copts, 0, اندازه(copts));
			copts.padding = 2;
			اگر (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		اگر (column_active(colopts))
			stop_column_filter();
		برو cleanup;
	}
	اگر (filter.سطرها != -1)
		only_in_list = "-n";
	وگرنه اگر (filter.with_commit)
		only_in_list = "--contains";
	وگرنه اگر (filter.no_commit)
		only_in_list = "--no-contains";
	وگرنه اگر (filter.points_at.nr)
		only_in_list = "--points-at";
	وگرنه اگر (filter.reachable_from)
		only_in_list = "--merged";
	وگرنه اگر (filter.unreachable_from)
		only_in_list = "--no-merged";
	اگر (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	اگر (cmdmode == 'd') {
		ret = delete_tags(argv);
		برو cleanup;
	}
	اگر (cmdmode == 'v') {
		اگر (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		برو cleanup;
	}

	اگر (پیام.given || msgfile) {
		اگر (پیام.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		اگر (پیام.given)
			strbuf_addbuf(&buf, &(پیام.buf));
		وگرنه {
			اگر (!strcmp(msgfile, "-")) {
				اگر (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} وگرنه {
				اگر (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	اگر (argc > 2)
		die(_("too many arguments"));

	اگر (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	اگر (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	اگر (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	وگرنه اگر (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = پیام.given || msgfile;
	opt.use_editor = edit_flag;

	اگر (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	وگرنه اگر (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	وگرنه اگر (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	وگرنه
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_پیام(&object, &reflog_پیام);

	اگر (create_tag_object) {
		اگر (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   پوچ, پوچ,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_پیام.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		اگر (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	اگر (path) {
		unlink_or_warn(path);
		آزاد(path);
	}
	ref_transaction_free(transaction);
	اگر (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_پیام);
	strbuf_release(&پیام.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	آزاد(msgfile);
	بازگشت ret;
}
