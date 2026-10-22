/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#अन्तर्भाव "builtin.h"
#अन्तर्भाव "advice.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "diff.h"
#अन्तर्भाव "revision.h"
#अन्तर्भाव "gpg-interface.h"
#अन्तर्भाव "oid-array.h"
#अन्तर्भाव "column.h"
#अन्तर्भाव "ref-filter.h"
#अन्तर्भाव "date.h"
#अन्तर्भाव "write-or-die.h"
#अन्तर्भाव "object-file-convert.h"
#अन्तर्भाव "trailer.h"

स्थिर स्थिरांक अक्षर * स्थिरांक git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	रिक्त
};

स्थिर अचिन्हित पूर्णांक colopts;
स्थिर पूर्णांक force_sign_annotate;
स्थिर पूर्णांक config_sign_tag = -1; /* unspecified */

स्थिर पूर्णांक list_tags(संरचना ref_filter *filter, संरचना ref_sorting *sorting,
		     संरचना ref_format *format)
{
	अक्षर *to_free = रिक्त;

	जर (filter->ओळी == -1)
		filter->ओळी = 0;

	जर (!format->format) {
		जर (filter->ओळी) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->ओळी);
			format->format = to_free;
		} अन्यथा
			format->format = "%(refname:lstrip=2)";
	}

	जर (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	मुक्त(to_free);

	परत 0;
}

प्रकारव्याख्या पूर्णांक (*each_tag_name_fn)(स्थिरांक अक्षर *name, स्थिरांक अक्षर *ref,
				स्थिरांक संरचना object_id *oid, शून्य *cb_data);

स्थिर पूर्णांक for_each_tag_name(स्थिरांक अक्षर **argv, each_tag_name_fn fn,
			     शून्य *cb_data)
{
	स्थिरांक अक्षर **p;
	संरचना strbuf ref = STRBUF_INIT;
	पूर्णांक had_error = 0;
	संरचना object_id oid;

	साठी (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		जर (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			सुरूठेवा;
		}
		जर (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	परत had_error;
}

स्थिर पूर्णांक collect_tags(स्थिरांक अक्षर *name UNUSED, स्थिरांक अक्षर *ref,
			स्थिरांक संरचना object_id *oid, शून्य *cb_data)
{
	संरचना string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->वस्तुसंच[ref_list->nr - 1].util = oiddup(oid);
	परत 0;
}

स्थिर पूर्णांक delete_tags(स्थिरांक अक्षर **argv)
{
	पूर्णांक result;
	संरचना string_list refs_to_delete = STRING_LIST_INIT_DUP;
	संरचना string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (शून्य *)&refs_to_delete);
	जर (refs_delete_refs(get_main_ref_store(the_repository), रिक्त, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		स्थिरांक अक्षर *name = item->string;
		संरचना object_id *oid = item->util;
		जर (!refs_ref_exists(get_main_ref_store(the_repository), name))
			म_लिखो(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		मुक्त(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	परत result;
}

स्थिर पूर्णांक verify_tag(स्थिरांक अक्षर *name, स्थिरांक अक्षर *ref UNUSED,
		      स्थिरांक संरचना object_id *oid, शून्य *cb_data)
{
	पूर्णांक flags;
	संरचना ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	जर (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	जर (gpg_verify_tag(the_repository, oid, name, flags))
		परत -1;

	जर (format->format)
		pretty_print_ref(name, oid, रिक्त, format);

	परत 0;
}

स्थिर पूर्णांक do_sign(संरचना strbuf *buffer, संरचना object_id **compat_oid,
		   संरचना object_id *compat_oid_buf)
{
	स्थिरांक संरचना git_hash_algo *compat = the_repository->compat_hash_algo;
	संरचना strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	संरचना strbuf compat_buf = STRBUF_INIT;
	अक्षर *keyid = get_signing_किल्ली();
	पूर्णांक ret = -1;

	जर (sign_buffer(buffer, &sig, keyid, 0))
		जा out;

	जर (compat) {
		स्थिरांक संरचना git_hash_algo *algo = the_repository->hash_algo;

		जर (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			जा out;
		जर (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			जा out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	जर (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	मुक्त(keyid);
	परत ret;
}

स्थिर स्थिरांक अक्षर tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

स्थिर स्थिरांक अक्षर tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

स्थिर पूर्णांक git_tag_config(स्थिरांक अक्षर *var, स्थिरांक अक्षर *मूल्य,
			  स्थिरांक संरचना config_context *ctx, शून्य *cb)
{
	जर (!म_भेद(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (!म_भेद(var, "tag.sort")) {
		जर (!मूल्य)
			परत config_error_nonbool(var);
		string_list_append(cb, मूल्य);
		परत 0;
	}

	जर (!म_भेद(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, मूल्य);
		परत 0;
	}

	जर (starts_with(var, "column."))
		परत git_column_config(var, मूल्य, "tag", &colopts);

	जर (git_color_config(var, मूल्य, cb) < 0)
		परत -1;

	परत git_default_config(var, मूल्य, ctx, cb);
}

स्थिर शून्य write_tag_body(पूर्णांक fd, स्थिरांक संरचना object_id *oid)
{
	माप_प्रकार size;
	गणना object_type type;
	अक्षर *buf, *sp, *orig;
	संरचना strbuf payload = STRBUF_INIT;
	संरचना strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	जर (!buf)
		परत;
	जर (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = म_माला(buf, "\n\n");

	जर (!sp || !size || type != OBJ_TAG) {
		मुक्त(buf);
		परत;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	मुक्त(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

स्थिर पूर्णांक build_tag_object(संरचना strbuf *buf, पूर्णांक sign, संरचना object_id *result)
{
	संरचना object_id *compat_oid = रिक्त, compat_oid_buf;
	जर (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		परत error(_("unable to sign the tag"));
	जर (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		परत error(_("unable to write tag file"));
	परत 0;
}

संरचना create_tag_options {
	अचिन्हित पूर्णांक message_given:1;
	अचिन्हित पूर्णांक use_editor:1;
	अचिन्हित पूर्णांक sign;
	गणना {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

स्थिर स्थिरांक अक्षर message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

स्थिर शून्य create_tag(स्थिरांक संरचना object_id *object, स्थिरांक अक्षर *object_ref,
		       स्थिरांक अक्षर *tag,
		       संरचना strbuf *buf, संरचना create_tag_options *opt,
		       संरचना object_id *prev, संरचना object_id *result,
		       संरचना strvec *trailer_args, अक्षर *path)
{
	गणना object_type type;
	संरचना strbuf header = STRBUF_INIT;
	पूर्णांक should_edit;

	type = odb_read_object_info(the_repository->objects, object, रिक्त);
	जर (type <= OBJ_NONE)
		die(_("bad object type."));

	जर (type == OBJ_TAG)
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
	जर (should_edit || trailer_args->nr) {
		पूर्णांक fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		जर (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} अन्यथा जर (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} अन्यथा {
			संरचना strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			जर (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_ओळ_str,
				      _(tag_template), tag, comment_ओळ_str);
			अन्यथा
				strbuf_commented_addf(&buf, comment_ओळ_str,
				      _(tag_template_nocleanup), tag, comment_ओळ_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		बंद(fd);

		जर (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		जर (should_edit) {
			जर (launch_editor(path, buf, रिक्त)) {
				ख_लिखो(मानक_त्रुटि,
					_("Please supply the message using either -m or -F option.\n"));
				निकास(1);
			}
		} अन्यथा जर (trailer_args->nr) {
			strbuf_reset(buf);
			जर (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	जर (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_ओळ_str : रिक्त);

	जर (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_घाला(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	जर (build_tag_object(buf, opt->sign, result) < 0) {
		जर (path)
			ख_लिखो(मानक_त्रुटि, _("The tag message has been left in %s\n"),
				path);
		निकास(128);
	}
}

स्थिर शून्य create_reflog_संदेश(स्थिरांक संरचना object_id *oid, संरचना strbuf *sb)
{
	गणना object_type type;
	संरचना commit *c;
	अक्षर *buf;
	माप_प्रकार size;
	पूर्णांक subject_len = 0;
	स्थिरांक अक्षर *subject_start;

	अक्षर *rla = दो_पर्या("GIT_REFLOG_ACTION");
	जर (rla) {
		strbuf_addstr(sb, rla);
	} अन्यथा {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, रिक्त);
	निवड (type) {
	पूर्वनिर्धारित:
		strbuf_addstr(sb, "object of unknown type");
		थांबा;
	स्थिती OBJ_COMMIT:
		जर ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_घाला(sb, sb->len, subject_start, subject_len);
		} अन्यथा {
			strbuf_addstr(sb, "commit object");
		}
		मुक्त(buf);

		जर ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		थांबा;
	स्थिती OBJ_वृक्ष:
		strbuf_addstr(sb, "tree object");
		थांबा;
	स्थिती OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		थांबा;
	स्थिती OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		थांबा;
	}
	strbuf_addch(sb, ')');
}

संरचना संदेश_arg {
	पूर्णांक given;
	संरचना strbuf buf;
};

स्थिर पूर्णांक parse_संदेश_arg(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना संदेश_arg *संदेश = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	जर (!arg)
		परत -1;
	जर (संदेश->buf.len)
		strbuf_addstr(&(संदेश->buf), "\n\n");
	strbuf_addstr(&(संदेश->buf), arg);
	संदेश->given = 1;
	परत 0;
}

पूर्णांक cmd_tag(पूर्णांक argc,
	    स्थिरांक अक्षर **argv,
	    स्थिरांक अक्षर *prefix,
	    संरचना repository *repo UNUSED)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf ref = STRBUF_INIT;
	संरचना strbuf reflog_संदेश = STRBUF_INIT;
	संरचना object_id object, prev;
	स्थिरांक अक्षर *object_ref, *tag;
	संरचना create_tag_options opt;
	अक्षर *cleanup_arg = रिक्त;
	पूर्णांक create_reflog = 0;
	पूर्णांक annotate = 0, force = 0;
	पूर्णांक cmdmode = 0, create_tag_object = 0;
	अक्षर *msgfile = रिक्त;
	स्थिरांक अक्षर *keyid = रिक्त;
	संरचना संदेश_arg संदेश = { .buf = STRBUF_INIT };
	संरचना ref_transaction *transaction;
	संरचना strbuf err = STRBUF_INIT;
	संरचना ref_filter filter = REF_FILTER_INIT;
	संरचना ref_sorting *sorting;
	संरचना string_list sorting_options = STRING_LIST_INIT_DUP;
	संरचना ref_format format = REF_FORMAT_INIT;
	संरचना strvec trailer_args = STRVEC_INIT;
	पूर्णांक icase = 0;
	पूर्णांक edit_flag = 0;
	संरचना option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.मूल्य = &filter.ओळी,
			.precision = आकार(filter.ओळी),
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
		OPT_CALLBACK_F('m', "message", &संदेश, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_संदेश_arg),
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
			.मूल्य = &filter.points_at,
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
	पूर्णांक ret = 0;
	स्थिरांक अक्षर *only_in_list = रिक्त;
	अक्षर *path = रिक्त;

	setup_ref_filter_porcelain_संदेश();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	जर (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	स_रखो(&opt, 0, आकार(opt));
	filter.ओळी = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	जर (!cmdmode) {
		जर (argc == 0)
			cmdmode = 'l';
		अन्यथा जर (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.ओळी != -1)
			cmdmode = 'l';
	}

	जर (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	जर (trailer_args.nr)
		trailer_config_init();

	जर (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	जर (keyid) {
		opt.sign = 1;
		set_signing_किल्ली(keyid);
	}
	create_tag_object = (opt.sign || annotate || संदेश.given || msgfile ||
			     edit_flag || trailer_args.nr);

	जर ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	जर (cmdmode == 'l' && filter.ओळी != -1) {
		जर (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	जर (cmdmode == 'l') {
		जर (column_active(colopts)) {
			संरचना column_options copts;
			स_रखो(&copts, 0, आकार(copts));
			copts.padding = 2;
			जर (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		जर (column_active(colopts))
			stop_column_filter();
		जा cleanup;
	}
	जर (filter.ओळी != -1)
		only_in_list = "-n";
	अन्यथा जर (filter.with_commit)
		only_in_list = "--contains";
	अन्यथा जर (filter.no_commit)
		only_in_list = "--no-contains";
	अन्यथा जर (filter.points_at.nr)
		only_in_list = "--points-at";
	अन्यथा जर (filter.reachable_from)
		only_in_list = "--merged";
	अन्यथा जर (filter.unreachable_from)
		only_in_list = "--no-merged";
	जर (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	जर (cmdmode == 'd') {
		ret = delete_tags(argv);
		जा cleanup;
	}
	जर (cmdmode == 'v') {
		जर (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		जा cleanup;
	}

	जर (संदेश.given || msgfile) {
		जर (संदेश.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		जर (संदेश.given)
			strbuf_addbuf(&buf, &(संदेश.buf));
		अन्यथा {
			जर (!म_भेद(msgfile, "-")) {
				जर (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} अन्यथा {
				जर (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	जर (argc > 2)
		die(_("too many arguments"));

	जर (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	जर (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	जर (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	अन्यथा जर (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = संदेश.given || msgfile;
	opt.use_editor = edit_flag;

	जर (!cleanup_arg || !म_भेद(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	अन्यथा जर (!म_भेद(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	अन्यथा जर (!म_भेद(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	अन्यथा
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_संदेश(&object, &reflog_संदेश);

	जर (create_tag_object) {
		जर (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	जर (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   रिक्त, रिक्त,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_संदेश.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		जर (path)
			ख_लिखो(मानक_त्रुटि,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	जर (path) {
		unlink_or_warn(path);
		मुक्त(path);
	}
	ref_transaction_free(transaction);
	जर (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		म_लिखो(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_संदेश);
	strbuf_release(&संदेश.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	मुक्त(msgfile);
	परत ret;
}
