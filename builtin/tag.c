/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

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

ساكن ثابت حرف * ثابت git_tag_usage[] = {
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

ساكن unsigned صحيح colopts;
ساكن صحيح force_sign_annotate;
ساكن صحيح config_sign_tag = -1; /* unspecified */

ساكن صحيح list_tags(بنية ref_filter *filter, بنية ref_sorting *sorting,
		     بنية ref_format *format)
{
	حرف *to_free = عدم;

	إذا (filter->ليوك == -1)
		filter->ليوك = 0;

	إذا (!format->format) {
		إذا (filter->ليوك) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->ليوك);
			format->format = to_free;
		} وإلا
			format->format = "%(refname:lstrip=2)";
	}

	إذا (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	حرر(to_free);

	أرجع 0;
}

تعريفنوع صحيح (*each_tag_name_fn)(ثابت حرف *name, ثابت حرف *ref,
				ثابت بنية object_id *oid, فراغ *cb_data);

ساكن صحيح for_each_tag_name(ثابت حرف **argv, each_tag_name_fn fn,
			     فراغ *cb_data)
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
		إذا (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	أرجع had_error;
}

ساكن صحيح collect_tags(ثابت حرف *name UNUSED, ثابت حرف *ref,
			ثابت بنية object_id *oid, فراغ *cb_data)
{
	بنية string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->ابوبو[ref_list->nr - 1].util = oiddup(oid);
	أرجع 0;
}

ساكن صحيح delete_tags(ثابت حرف **argv)
{
	صحيح result;
	بنية string_list refs_to_delete = STRING_LIST_INIT_DUP;
	بنية string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (فراغ *)&refs_to_delete);
	إذا (refs_delete_refs(get_main_ref_store(the_repository), عدم, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ثابت حرف *name = item->string;
		بنية object_id *oid = item->util;
		إذا (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		حرر(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	أرجع result;
}

ساكن صحيح verify_tag(ثابت حرف *name, ثابت حرف *ref UNUSED,
		      ثابت بنية object_id *oid, فراغ *cb_data)
{
	صحيح flags;
	بنية ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	إذا (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	إذا (gpg_verify_tag(the_repository, oid, name, flags))
		أرجع -1;

	إذا (format->format)
		pretty_بغ_ref(name, oid, عدم, format);

	أرجع 0;
}

ساكن صحيح do_sign(بنية strbuf *buffer, بنية object_id **compat_oid,
		   بنية object_id *compat_oid_buf)
{
	ثابت بنية git_hash_algo *compat = the_repository->compat_hash_algo;
	بنية strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	بنية strbuf compat_buf = STRBUF_INIT;
	حرف *keyid = get_signing_مبودي();
	صحيح ret = -1;

	إذا (sign_buffer(buffer, &sig, keyid, 0))
		اذهب out;

	إذا (compat) {
		ثابت بنية git_hash_algo *algo = the_repository->hash_algo;

		إذا (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			اذهب out;
		إذا (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			اذهب out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	إذا (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	حرر(keyid);
	أرجع ret;
}

ساكن ثابت حرف tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ساكن ثابت حرف tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ساكن صحيح git_tag_config(ثابت حرف *var, ثابت حرف *درج,
			  ثابت بنية config_context *ctx, فراغ *cb)
{
	إذا (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, درج);
		أرجع 0;
	}

	إذا (!strcmp(var, "tag.sort")) {
		إذا (!درج)
			أرجع config_error_nonbool(var);
		string_list_append(cb, درج);
		أرجع 0;
	}

	إذا (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, درج);
		أرجع 0;
	}

	إذا (starts_with(var, "column."))
		أرجع git_column_config(var, درج, "tag", &colopts);

	إذا (git_color_config(var, درج, cb) < 0)
		أرجع -1;

	أرجع git_default_config(var, درج, ctx, cb);
}

ساكن فراغ write_tag_body(صحيح fd, ثابت بنية object_id *oid)
{
	size_t size;
	تعداد object_type type;
	حرف *buf, *sp, *orig;
	بنية strbuf payload = STRBUF_INIT;
	بنية strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	إذا (!buf)
		أرجع;
	إذا (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	إذا (!sp || !size || type != OBJ_TAG) {
		حرر(buf);
		أرجع;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	حرر(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ساكن صحيح build_tag_object(بنية strbuf *buf, صحيح sign, بنية object_id *result)
{
	بنية object_id *compat_oid = عدم, compat_oid_buf;
	إذا (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		أرجع error(_("unable to sign the tag"));
	إذا (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		أرجع error(_("unable to write tag file"));
	أرجع 0;
}

بنية create_tag_options {
	unsigned صحيح message_given:1;
	unsigned صحيح use_editor:1;
	unsigned صحيح sign;
	تعداد {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

ساكن ثابت حرف message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ساكن فراغ create_tag(ثابت بنية object_id *object, ثابت حرف *object_ref,
		       ثابت حرف *tag,
		       بنية strbuf *buf, بنية create_tag_options *opt,
		       بنية object_id *prev, بنية object_id *result,
		       بنية strvec *trailer_حجوجي, حرف *path)
{
	تعداد object_type type;
	بنية strbuf header = STRBUF_INIT;
	صحيح should_edit;

	type = odb_read_object_info(the_repository->objects, object, عدم);
	إذا (type <= OBJ_NONE)
		die(_("bad object type."));

	إذا (type == OBJ_TAG)
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
	إذا (should_edit || trailer_حجوجي->nr) {
		صحيح fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		إذا (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} وإلا إذا (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} وإلا {
			بنية strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			إذا (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_لي_str,
				      _(tag_template), tag, comment_لي_str);
			وإلا
				strbuf_commented_addf(&buf, comment_لي_str,
				      _(tag_template_nocleanup), tag, comment_لي_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		إذا (trailer_حجوجي->nr && amend_file_with_trailers(path, trailer_حجوجي))
			die(_("unable to pass trailers to --trailers"));

		إذا (should_edit) {
			إذا (launch_editor(path, buf, عدم)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} وإلا إذا (trailer_حجوجي->nr) {
			strbuf_reset(buf);
			إذا (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	إذا (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_لي_str : عدم);

	إذا (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_سك(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	إذا (build_tag_object(buf, opt->sign, result) < 0) {
		إذا (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

ساكن فراغ create_reflog_سكو(ثابت بنية object_id *oid, بنية strbuf *sb)
{
	تعداد object_type type;
	بنية commit *c;
	حرف *buf;
	size_t size;
	صحيح subject_len = 0;
	ثابت حرف *subject_start;

	حرف *rla = getenv("GIT_REFLOG_ACTION");
	إذا (rla) {
		strbuf_addstr(sb, rla);
	} وإلا {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, عدم);
	بدل (type) {
	افتراضي:
		strbuf_addstr(sb, "object of unknown type");
		اكسر;
	حالة OBJ_COMMIT:
		إذا ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_سك(sb, sb->len, subject_start, subject_len);
		} وإلا {
			strbuf_addstr(sb, "commit object");
		}
		حرر(buf);

		إذا ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		اكسر;
	حالة OBJ_بشيا:
		strbuf_addstr(sb, "tree object");
		اكسر;
	حالة OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		اكسر;
	حالة OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		اكسر;
	}
	strbuf_addch(sb, ')');
}

بنية سكو_arg {
	صحيح given;
	بنية strbuf buf;
};

ساكن صحيح parse_سكو_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية سكو_arg *سكو = opt->درج;

	BUG_ON_OPT_NEG(unset);

	إذا (!arg)
		أرجع -1;
	إذا (سكو->buf.len)
		strbuf_addstr(&(سكو->buf), "\n\n");
	strbuf_addstr(&(سكو->buf), arg);
	سكو->given = 1;
	أرجع 0;
}

صحيح cmd_tag(صحيح argc,
	    ثابت حرف **argv,
	    ثابت حرف *prefix,
	    بنية repository *repo UNUSED)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf ref = STRBUF_INIT;
	بنية strbuf reflog_سكو = STRBUF_INIT;
	بنية object_id object, prev;
	ثابت حرف *object_ref, *tag;
	بنية create_tag_options opt;
	حرف *cleanup_arg = عدم;
	صحيح create_reflog = 0;
	صحيح annotate = 0, force = 0;
	صحيح cmdmode = 0, create_tag_object = 0;
	حرف *msgfile = عدم;
	ثابت حرف *keyid = عدم;
	بنية سكو_arg سكو = { .buf = STRBUF_INIT };
	بنية ref_transaction *transaction;
	بنية strbuf err = STRBUF_INIT;
	بنية ref_filter filter = REF_FILTER_INIT;
	بنية ref_sorting *sorting;
	بنية string_list sorting_options = STRING_LIST_INIT_DUP;
	بنية ref_format format = REF_FORMAT_INIT;
	بنية strvec trailer_حجوجي = STRVEC_INIT;
	صحيح icase = 0;
	صحيح edit_flag = 0;
	بنية option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.درج = &filter.ليوك,
			.precision = حجم(filter.ليوك),
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
		OPT_CALLBACK_F('m', "message", &سكو, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_سكو_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_حجوجي, N_("trailer"),
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
			.درج = &filter.points_at,
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
	صحيح ret = 0;
	ثابت حرف *only_in_list = عدم;
	حرف *path = عدم;

	setup_ref_filter_porcelain_سكو();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	إذا (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, حجم(opt));
	filter.ليوك = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	إذا (!cmdmode) {
		إذا (argc == 0)
			cmdmode = 'l';
		وإلا إذا (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.ليوك != -1)
			cmdmode = 'l';
	}

	إذا (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	إذا (trailer_حجوجي.nr)
		trailer_config_init();

	إذا (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	إذا (keyid) {
		opt.sign = 1;
		set_signing_مبودي(keyid);
	}
	create_tag_object = (opt.sign || annotate || سكو.given || msgfile ||
			     edit_flag || trailer_حجوجي.nr);

	إذا ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	إذا (cmdmode == 'l' && filter.ليوك != -1) {
		إذا (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	إذا (cmdmode == 'l') {
		إذا (column_active(colopts)) {
			بنية column_options copts;
			memset(&copts, 0, حجم(copts));
			copts.padding = 2;
			إذا (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		إذا (column_active(colopts))
			stop_column_filter();
		اذهب cleanup;
	}
	إذا (filter.ليوك != -1)
		only_in_list = "-n";
	وإلا إذا (filter.with_commit)
		only_in_list = "--contains";
	وإلا إذا (filter.no_commit)
		only_in_list = "--no-contains";
	وإلا إذا (filter.points_at.nr)
		only_in_list = "--points-at";
	وإلا إذا (filter.reachable_from)
		only_in_list = "--merged";
	وإلا إذا (filter.unreachable_from)
		only_in_list = "--no-merged";
	إذا (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	إذا (cmdmode == 'd') {
		ret = delete_tags(argv);
		اذهب cleanup;
	}
	إذا (cmdmode == 'v') {
		إذا (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		اذهب cleanup;
	}

	إذا (سكو.given || msgfile) {
		إذا (سكو.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		إذا (سكو.given)
			strbuf_addbuf(&buf, &(سكو.buf));
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

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	إذا (argc > 2)
		die(_("too many arguments"));

	إذا (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	إذا (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	إذا (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	وإلا إذا (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = سكو.given || msgfile;
	opt.use_editor = edit_flag;

	إذا (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	وإلا إذا (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	وإلا إذا (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	وإلا
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_سكو(&object, &reflog_سكو);

	إذا (create_tag_object) {
		إذا (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_حجوجي, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   عدم, عدم,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_سكو.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		إذا (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	إذا (path) {
		unlink_or_warn(path);
		حرر(path);
	}
	ref_transaction_free(transaction);
	إذا (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_سكو);
	strbuf_release(&سكو.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_حجوجي);
	حرر(msgfile);
	أرجع ret;
}
