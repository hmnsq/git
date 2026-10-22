/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#घोषणा USE_THE_REPOSITORY_VARIABLE
#घोषणा DISABLE_SIGN_COMPARE_WARNINGS

#समावेश "builtin.h"
#समावेश "advice.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "path.h"
#समावेश "tag.h"
#समावेश "parse-options.h"
#समावेश "diff.h"
#समावेश "revision.h"
#समावेश "gpg-interface.h"
#समावेश "oid-array.h"
#समावेश "column.h"
#समावेश "ref-filter.h"
#समावेश "date.h"
#समावेश "write-or-die.h"
#समावेश "object-file-convert.h"
#समावेश "trailer.h"

अटल स्थिर अक्षर * स्थिर git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	शून्य
};

अटल अचिन्हित पूर्णांक colopts;
अटल पूर्णांक force_sign_annotate;
अटल पूर्णांक config_sign_tag = -1; /* unspecified */

अटल पूर्णांक list_tags(काष्ठा ref_filter *filter, काष्ठा ref_sorting *sorting,
		     काष्ठा ref_format *format)
{
	अक्षर *to_free = शून्य;

	अगर (filter->पंक्तियाँ == -1)
		filter->पंक्तियाँ = 0;

	अगर (!format->format) {
		अगर (filter->पंक्तियाँ) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->पंक्तियाँ);
			format->format = to_free;
		} अन्यथा
			format->format = "%(refname:lstrip=2)";
	}

	अगर (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	मुक्त(to_free);

	वापस 0;
}

प्रकार पूर्णांक (*each_tag_name_fn)(स्थिर अक्षर *name, स्थिर अक्षर *ref,
				स्थिर काष्ठा object_id *oid, व्योम *cb_तथ्य);

अटल पूर्णांक for_each_tag_name(स्थिर अक्षर **argv, each_tag_name_fn fn,
			     व्योम *cb_तथ्य)
{
	स्थिर अक्षर **p;
	काष्ठा strbuf ref = STRBUF_INIT;
	पूर्णांक had_error = 0;
	काष्ठा object_id oid;

	क्रम (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		अगर (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			जारी;
		}
		अगर (fn(*p, ref.buf, &oid, cb_तथ्य))
			had_error = 1;
	}
	strbuf_release(&ref);
	वापस had_error;
}

अटल पूर्णांक collect_tags(स्थिर अक्षर *name UNUSED, स्थिर अक्षर *ref,
			स्थिर काष्ठा object_id *oid, व्योम *cb_तथ्य)
{
	काष्ठा string_list *ref_list = cb_तथ्य;

	string_list_append(ref_list, ref);
	ref_list->वस्तुएँ[ref_list->nr - 1].util = oiddup(oid);
	वापस 0;
}

अटल पूर्णांक delete_tags(स्थिर अक्षर **argv)
{
	पूर्णांक परिणाम;
	काष्ठा string_list refs_to_delete = STRING_LIST_INIT_DUP;
	काष्ठा string_list_item *item;

	परिणाम = for_each_tag_name(argv, collect_tags, (व्योम *)&refs_to_delete);
	अगर (refs_delete_refs(get_main_ref_store(the_repository), शून्य, &refs_to_delete, REF_NO_DEREF))
		परिणाम = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		स्थिर अक्षर *name = item->string;
		काष्ठा object_id *oid = item->util;
		अगर (!refs_ref_exists(get_main_ref_store(the_repository), name))
			म_लिखो(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		मुक्त(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	वापस परिणाम;
}

अटल पूर्णांक verify_tag(स्थिर अक्षर *name, स्थिर अक्षर *ref UNUSED,
		      स्थिर काष्ठा object_id *oid, व्योम *cb_तथ्य)
{
	पूर्णांक flags;
	काष्ठा ref_format *format = cb_तथ्य;
	flags = GPG_VERIFY_VERBOSE;

	अगर (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	अगर (gpg_verify_tag(the_repository, oid, name, flags))
		वापस -1;

	अगर (format->format)
		pretty_print_ref(name, oid, शून्य, format);

	वापस 0;
}

अटल पूर्णांक do_sign(काष्ठा strbuf *बफर, काष्ठा object_id **compat_oid,
		   काष्ठा object_id *compat_oid_buf)
{
	स्थिर काष्ठा git_hash_algo *compat = the_repository->compat_hash_algo;
	काष्ठा strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	काष्ठा strbuf compat_buf = STRBUF_INIT;
	अक्षर *keyid = get_signing_कुंजी();
	पूर्णांक ret = -1;

	अगर (sign_बफर(बफर, &sig, keyid, 0))
		जाओ out;

	अगर (compat) {
		स्थिर काष्ठा git_hash_algo *algo = the_repository->hash_algo;

		अगर (convert_object_file(the_repository ,&compat_buf, algo, compat,
					बफर->buf, बफर->len, OBJ_TAG, 1))
			जाओ out;
		अगर (sign_बफर(&compat_buf, &compat_sig, keyid, 0))
			जाओ out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	अगर (compat_sig.len)
		add_header_signature(बफर, &compat_sig, compat);

	strbuf_addbuf(बफर, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	मुक्त(keyid);
	वापस ret;
}

अटल स्थिर अक्षर tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

अटल स्थिर अक्षर tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

अटल पूर्णांक git_tag_config(स्थिर अक्षर *var, स्थिर अक्षर *मूल्य,
			  स्थिर काष्ठा config_context *ctx, व्योम *cb)
{
	अगर (!म_भेद(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (!म_भेद(var, "tag.sort")) {
		अगर (!मूल्य)
			वापस config_error_nonbool(var);
		string_list_append(cb, मूल्य);
		वापस 0;
	}

	अगर (!म_भेद(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, मूल्य);
		वापस 0;
	}

	अगर (starts_with(var, "column."))
		वापस git_column_config(var, मूल्य, "tag", &colopts);

	अगर (git_color_config(var, मूल्य, cb) < 0)
		वापस -1;

	वापस git_default_config(var, मूल्य, ctx, cb);
}

अटल व्योम write_tag_body(पूर्णांक fd, स्थिर काष्ठा object_id *oid)
{
	माप_प्रकार आकार;
	क्रमागत object_type type;
	अक्षर *buf, *sp, *orig;
	काष्ठा strbuf payload = STRBUF_INIT;
	काष्ठा strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	अगर (!buf)
		वापस;
	अगर (parse_signature(buf, आकार, &payload, &signature)) {
		buf = payload.buf;
		आकार = payload.len;
	}
	/* skip header */
	sp = म_माला(buf, "\n\n");

	अगर (!sp || !आकार || type != OBJ_TAG) {
		मुक्त(buf);
		वापस;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + आकार - sp);

	मुक्त(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

अटल पूर्णांक build_tag_object(काष्ठा strbuf *buf, पूर्णांक sign, काष्ठा object_id *परिणाम)
{
	काष्ठा object_id *compat_oid = शून्य, compat_oid_buf;
	अगर (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		वापस error(_("unable to sign the tag"));
	अगर (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, परिणाम, compat_oid, 0) < 0)
		वापस error(_("unable to write tag file"));
	वापस 0;
}

काष्ठा create_tag_options {
	अचिन्हित पूर्णांक message_given:1;
	अचिन्हित पूर्णांक use_editor:1;
	अचिन्हित पूर्णांक sign;
	क्रमागत {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

अटल स्थिर अक्षर message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

अटल व्योम create_tag(स्थिर काष्ठा object_id *object, स्थिर अक्षर *object_ref,
		       स्थिर अक्षर *tag,
		       काष्ठा strbuf *buf, काष्ठा create_tag_options *opt,
		       काष्ठा object_id *पूर्व, काष्ठा object_id *परिणाम,
		       काष्ठा strvec *trailer_args, अक्षर *path)
{
	क्रमागत object_type type;
	काष्ठा strbuf header = STRBUF_INIT;
	पूर्णांक should_edit;

	type = odb_read_object_info(the_repository->objects, object, शून्य);
	अगर (type <= OBJ_NONE)
		die(_("bad object type."));

	अगर (type == OBJ_TAG)
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
	अगर (should_edit || trailer_args->nr) {
		पूर्णांक fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		अगर (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} अन्यथा अगर (!is_null_oid(पूर्व)) {
			write_tag_body(fd, पूर्व);
		} अन्यथा {
			काष्ठा strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			अगर (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_पंक्ति_str,
				      _(tag_template), tag, comment_पंक्ति_str);
			अन्यथा
				strbuf_commented_addf(&buf, comment_पंक्ति_str,
				      _(tag_template_nocleanup), tag, comment_पंक्ति_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		बंद(fd);

		अगर (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		अगर (should_edit) {
			अगर (launch_editor(path, buf, शून्य)) {
				ख_लिखो(मानक_त्रुटि,
					_("Please supply the message using either -m or -F option.\n"));
				निकास(1);
			}
		} अन्यथा अगर (trailer_args->nr) {
			strbuf_reset(buf);
			अगर (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	अगर (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_पंक्ति_str : शून्य);

	अगर (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_जोड़ो(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	अगर (build_tag_object(buf, opt->sign, परिणाम) < 0) {
		अगर (path)
			ख_लिखो(मानक_त्रुटि, _("The tag message has been left in %s\n"),
				path);
		निकास(128);
	}
}

अटल व्योम create_reflog_संदेश(स्थिर काष्ठा object_id *oid, काष्ठा strbuf *sb)
{
	क्रमागत object_type type;
	काष्ठा commit *c;
	अक्षर *buf;
	माप_प्रकार आकार;
	पूर्णांक subject_len = 0;
	स्थिर अक्षर *subject_start;

	अक्षर *rla = दो_पर्या("GIT_REFLOG_ACTION");
	अगर (rla) {
		strbuf_addstr(sb, rla);
	} अन्यथा {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, शून्य);
	चयन (type) {
	शेष:
		strbuf_addstr(sb, "object of unknown type");
		अवरोध;
	हाल OBJ_COMMIT:
		अगर ((buf = odb_read_object(the_repository->objects, oid, &type, &आकार))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_जोड़ो(sb, sb->len, subject_start, subject_len);
		} अन्यथा {
			strbuf_addstr(sb, "commit object");
		}
		मुक्त(buf);

		अगर ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		अवरोध;
	हाल OBJ_वृक्ष:
		strbuf_addstr(sb, "tree object");
		अवरोध;
	हाल OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		अवरोध;
	हाल OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		अवरोध;
	}
	strbuf_addch(sb, ')');
}

काष्ठा संदेश_arg {
	पूर्णांक given;
	काष्ठा strbuf buf;
};

अटल पूर्णांक parse_संदेश_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा संदेश_arg *संदेश = opt->मूल्य;

	BUG_ON_OPT_NEG(unset);

	अगर (!arg)
		वापस -1;
	अगर (संदेश->buf.len)
		strbuf_addstr(&(संदेश->buf), "\n\n");
	strbuf_addstr(&(संदेश->buf), arg);
	संदेश->given = 1;
	वापस 0;
}

पूर्णांक cmd_tag(पूर्णांक argc,
	    स्थिर अक्षर **argv,
	    स्थिर अक्षर *prefix,
	    काष्ठा repository *repo UNUSED)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf ref = STRBUF_INIT;
	काष्ठा strbuf reflog_संदेश = STRBUF_INIT;
	काष्ठा object_id object, पूर्व;
	स्थिर अक्षर *object_ref, *tag;
	काष्ठा create_tag_options opt;
	अक्षर *cleanup_arg = शून्य;
	पूर्णांक create_reflog = 0;
	पूर्णांक annotate = 0, force = 0;
	पूर्णांक cmdmode = 0, create_tag_object = 0;
	अक्षर *msgfile = शून्य;
	स्थिर अक्षर *keyid = शून्य;
	काष्ठा संदेश_arg संदेश = { .buf = STRBUF_INIT };
	काष्ठा ref_transaction *transaction;
	काष्ठा strbuf err = STRBUF_INIT;
	काष्ठा ref_filter filter = REF_FILTER_INIT;
	काष्ठा ref_sorting *sorting;
	काष्ठा string_list sorting_options = STRING_LIST_INIT_DUP;
	काष्ठा ref_format format = REF_FORMAT_INIT;
	काष्ठा strvec trailer_args = STRVEC_INIT;
	पूर्णांक icase = 0;
	पूर्णांक edit_flag = 0;
	काष्ठा option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.मूल्य = &filter.पंक्तियाँ,
			.precision = माप(filter.पंक्तियाँ),
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
	स्थिर अक्षर *only_in_list = शून्य;
	अक्षर *path = शून्य;

	setup_ref_filter_porcelain_संदेश();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	अगर (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	स_रखो(&opt, 0, माप(opt));
	filter.पंक्तियाँ = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	अगर (!cmdmode) {
		अगर (argc == 0)
			cmdmode = 'l';
		अन्यथा अगर (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.पंक्तियाँ != -1)
			cmdmode = 'l';
	}

	अगर (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	अगर (trailer_args.nr)
		trailer_config_init();

	अगर (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	अगर (keyid) {
		opt.sign = 1;
		set_signing_कुंजी(keyid);
	}
	create_tag_object = (opt.sign || annotate || संदेश.given || msgfile ||
			     edit_flag || trailer_args.nr);

	अगर ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	अगर (cmdmode == 'l' && filter.पंक्तियाँ != -1) {
		अगर (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	अगर (cmdmode == 'l') {
		अगर (column_active(colopts)) {
			काष्ठा column_options copts;
			स_रखो(&copts, 0, माप(copts));
			copts.padding = 2;
			अगर (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		अगर (column_active(colopts))
			stop_column_filter();
		जाओ cleanup;
	}
	अगर (filter.पंक्तियाँ != -1)
		only_in_list = "-n";
	अन्यथा अगर (filter.with_commit)
		only_in_list = "--contains";
	अन्यथा अगर (filter.no_commit)
		only_in_list = "--no-contains";
	अन्यथा अगर (filter.points_at.nr)
		only_in_list = "--points-at";
	अन्यथा अगर (filter.reachable_from)
		only_in_list = "--merged";
	अन्यथा अगर (filter.unreachable_from)
		only_in_list = "--no-merged";
	अगर (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	अगर (cmdmode == 'd') {
		ret = delete_tags(argv);
		जाओ cleanup;
	}
	अगर (cmdmode == 'v') {
		अगर (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		जाओ cleanup;
	}

	अगर (संदेश.given || msgfile) {
		अगर (संदेश.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		अगर (संदेश.given)
			strbuf_addbuf(&buf, &(संदेश.buf));
		अन्यथा {
			अगर (!म_भेद(msgfile, "-")) {
				अगर (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} अन्यथा {
				अगर (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	अगर (argc > 2)
		die(_("too many arguments"));

	अगर (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	अगर (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	अगर (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &पूर्व))
		oidclr(&पूर्व, the_repository->hash_algo);
	अन्यथा अगर (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = संदेश.given || msgfile;
	opt.use_editor = edit_flag;

	अगर (!cleanup_arg || !म_भेद(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	अन्यथा अगर (!म_भेद(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	अन्यथा अगर (!म_भेद(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	अन्यथा
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_संदेश(&object, &reflog_संदेश);

	अगर (create_tag_object) {
		अगर (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &पूर्व, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	अगर (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &पूर्व,
				   शून्य, शून्य,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_संदेश.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		अगर (path)
			ख_लिखो(मानक_त्रुटि,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	अगर (path) {
		unlink_or_warn(path);
		मुक्त(path);
	}
	ref_transaction_free(transaction);
	अगर (force && !is_null_oid(&पूर्व) && !oideq(&पूर्व, &object))
		म_लिखो(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &पूर्व, DEFAULT_ABBREV));

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
	वापस ret;
}
