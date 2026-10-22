/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "advice.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "diff.h"
#અન્તર્ભાવ "revision.h"
#અન્તર્ભાવ "gpg-interface.h"
#અન્તર્ભાવ "oid-array.h"
#અન્તર્ભાવ "column.h"
#અન્તર્ભાવ "ref-filter.h"
#અન્તર્ભાવ "date.h"
#અન્તર્ભાવ "write-or-die.h"
#અન્તર્ભાવ "object-file-convert.h"
#અન્તર્ભાવ "trailer.h"

સ્થિર અચલ અક્ષર * અચલ git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ખાલી
};

સ્થિર unsigned પૂર્ણાંક colopts;
સ્થિર પૂર્ણાંક force_sign_annotate;
સ્થિર પૂર્ણાંક config_sign_tag = -1; /* unspecified */

સ્થિર પૂર્ણાંક list_tags(સંરચના ref_filter *filter, સંરચના ref_sorting *sorting,
		     સંરચના ref_format *format)
{
	અક્ષર *to_free = ખાલી;

	જો (filter->લીટીઓ == -1)
		filter->લીટીઓ = 0;

	જો (!format->format) {
		જો (filter->લીટીઓ) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->લીટીઓ);
			format->format = to_free;
		} નહીંતર
			format->format = "%(refname:lstrip=2)";
	}

	જો (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	મુક્ત(to_free);

	પાછા 0;
}

પ્રકારવ્યાખ્યા પૂર્ણાંક (*each_tag_name_fn)(અચલ અક્ષર *name, અચલ અક્ષર *ref,
				અચલ સંરચના object_id *oid, શૂન્ય *cb_data);

સ્થિર પૂર્ણાંક for_each_tag_name(અચલ અક્ષર **argv, each_tag_name_fn fn,
			     શૂન્ય *cb_data)
{
	અચલ અક્ષર **p;
	સંરચના strbuf ref = STRBUF_INIT;
	પૂર્ણાંક had_error = 0;
	સંરચના object_id oid;

	માટે (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		જો (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			ચાલુ;
		}
		જો (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	પાછા had_error;
}

સ્થિર પૂર્ણાંક collect_tags(અચલ અક્ષર *name UNUSED, અચલ અક્ષર *ref,
			અચલ સંરચના object_id *oid, શૂન્ય *cb_data)
{
	સંરચના string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->વસ્તુઓ[ref_list->nr - 1].util = oiddup(oid);
	પાછા 0;
}

સ્થિર પૂર્ણાંક delete_tags(અચલ અક્ષર **argv)
{
	પૂર્ણાંક result;
	સંરચના string_list refs_to_delete = STRING_LIST_INIT_DUP;
	સંરચના string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (શૂન્ય *)&refs_to_delete);
	જો (refs_delete_refs(get_main_ref_store(the_repository), ખાલી, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		અચલ અક્ષર *name = item->string;
		સંરચના object_id *oid = item->util;
		જો (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		મુક્ત(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	પાછા result;
}

સ્થિર પૂર્ણાંક verify_tag(અચલ અક્ષર *name, અચલ અક્ષર *ref UNUSED,
		      અચલ સંરચના object_id *oid, શૂન્ય *cb_data)
{
	પૂર્ણાંક flags;
	સંરચના ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	જો (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	જો (gpg_verify_tag(the_repository, oid, name, flags))
		પાછા -1;

	જો (format->format)
		pretty_print_ref(name, oid, ખાલી, format);

	પાછા 0;
}

સ્થિર પૂર્ણાંક do_sign(સંરચના strbuf *buffer, સંરચના object_id **compat_oid,
		   સંરચના object_id *compat_oid_buf)
{
	અચલ સંરચના git_hash_algo *compat = the_repository->compat_hash_algo;
	સંરચના strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	સંરચના strbuf compat_buf = STRBUF_INIT;
	અક્ષર *keyid = get_signing_ચાવી();
	પૂર્ણાંક ret = -1;

	જો (sign_buffer(buffer, &sig, keyid, 0))
		જાઓ out;

	જો (compat) {
		અચલ સંરચના git_hash_algo *algo = the_repository->hash_algo;

		જો (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			જાઓ out;
		જો (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			જાઓ out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	જો (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	મુક્ત(keyid);
	પાછા ret;
}

સ્થિર અચલ અક્ષર tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

સ્થિર અચલ અક્ષર tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

સ્થિર પૂર્ણાંક git_tag_config(અચલ અક્ષર *var, અચલ અક્ષર *કિંમત,
			  અચલ સંરચના config_context *ctx, શૂન્ય *cb)
{
	જો (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "tag.sort")) {
		જો (!કિંમત)
			પાછા config_error_nonbool(var);
		string_list_append(cb, કિંમત);
		પાછા 0;
	}

	જો (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, કિંમત);
		પાછા 0;
	}

	જો (starts_with(var, "column."))
		પાછા git_column_config(var, કિંમત, "tag", &colopts);

	જો (git_color_config(var, કિંમત, cb) < 0)
		પાછા -1;

	પાછા git_default_config(var, કિંમત, ctx, cb);
}

સ્થિર શૂન્ય write_tag_body(પૂર્ણાંક fd, અચલ સંરચના object_id *oid)
{
	size_t size;
	ગણના object_type type;
	અક્ષર *buf, *sp, *orig;
	સંરચના strbuf payload = STRBUF_INIT;
	સંરચના strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	જો (!buf)
		પાછા;
	જો (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	જો (!sp || !size || type != OBJ_TAG) {
		મુક્ત(buf);
		પાછા;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	મુક્ત(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

સ્થિર પૂર્ણાંક build_tag_object(સંરચના strbuf *buf, પૂર્ણાંક sign, સંરચના object_id *result)
{
	સંરચના object_id *compat_oid = ખાલી, compat_oid_buf;
	જો (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		પાછા error(_("unable to sign the tag"));
	જો (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		પાછા error(_("unable to write tag file"));
	પાછા 0;
}

સંરચના create_tag_options {
	unsigned પૂર્ણાંક message_given:1;
	unsigned પૂર્ણાંક use_editor:1;
	unsigned પૂર્ણાંક sign;
	ગણના {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

સ્થિર અચલ અક્ષર message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

સ્થિર શૂન્ય create_tag(અચલ સંરચના object_id *object, અચલ અક્ષર *object_ref,
		       અચલ અક્ષર *tag,
		       સંરચના strbuf *buf, સંરચના create_tag_options *opt,
		       સંરચના object_id *prev, સંરચના object_id *result,
		       સંરચના strvec *trailer_args, અક્ષર *path)
{
	ગણના object_type type;
	સંરચના strbuf header = STRBUF_INIT;
	પૂર્ણાંક should_edit;

	type = odb_read_object_info(the_repository->objects, object, ખાલી);
	જો (type <= OBJ_NONE)
		die(_("bad object type."));

	જો (type == OBJ_TAG)
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
	જો (should_edit || trailer_args->nr) {
		પૂર્ણાંક fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		જો (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} નહીંતર જો (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} નહીંતર {
			સંરચના strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			જો (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_લીટી_str,
				      _(tag_template), tag, comment_લીટી_str);
			નહીંતર
				strbuf_commented_addf(&buf, comment_લીટી_str,
				      _(tag_template_nocleanup), tag, comment_લીટી_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		જો (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		જો (should_edit) {
			જો (launch_editor(path, buf, ખાલી)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} નહીંતર જો (trailer_args->nr) {
			strbuf_reset(buf);
			જો (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	જો (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_લીટી_str : ખાલી);

	જો (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_ઉમેરો(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	જો (build_tag_object(buf, opt->sign, result) < 0) {
		જો (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

સ્થિર શૂન્ય create_reflog_સંદેશ(અચલ સંરચના object_id *oid, સંરચના strbuf *sb)
{
	ગણના object_type type;
	સંરચના commit *c;
	અક્ષર *buf;
	size_t size;
	પૂર્ણાંક subject_len = 0;
	અચલ અક્ષર *subject_start;

	અક્ષર *rla = getenv("GIT_REFLOG_ACTION");
	જો (rla) {
		strbuf_addstr(sb, rla);
	} નહીંતર {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ખાલી);
	પસંદગી (type) {
	મૂળભૂત:
		strbuf_addstr(sb, "object of unknown type");
		તોડો;
	સ્થિતિ OBJ_COMMIT:
		જો ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_ઉમેરો(sb, sb->len, subject_start, subject_len);
		} નહીંતર {
			strbuf_addstr(sb, "commit object");
		}
		મુક્ત(buf);

		જો ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		તોડો;
	સ્થિતિ OBJ_વૃક્ષ:
		strbuf_addstr(sb, "tree object");
		તોડો;
	સ્થિતિ OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		તોડો;
	સ્થિતિ OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		તોડો;
	}
	strbuf_addch(sb, ')');
}

સંરચના સંદેશ_arg {
	પૂર્ણાંક given;
	સંરચના strbuf buf;
};

સ્થિર પૂર્ણાંક parse_સંદેશ_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના સંદેશ_arg *સંદેશ = opt->કિંમત;

	BUG_ON_OPT_NEG(unset);

	જો (!arg)
		પાછા -1;
	જો (સંદેશ->buf.len)
		strbuf_addstr(&(સંદેશ->buf), "\n\n");
	strbuf_addstr(&(સંદેશ->buf), arg);
	સંદેશ->given = 1;
	પાછા 0;
}

પૂર્ણાંક cmd_tag(પૂર્ણાંક argc,
	    અચલ અક્ષર **argv,
	    અચલ અક્ષર *prefix,
	    સંરચના repository *repo UNUSED)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના strbuf ref = STRBUF_INIT;
	સંરચના strbuf reflog_સંદેશ = STRBUF_INIT;
	સંરચના object_id object, prev;
	અચલ અક્ષર *object_ref, *tag;
	સંરચના create_tag_options opt;
	અક્ષર *cleanup_arg = ખાલી;
	પૂર્ણાંક create_reflog = 0;
	પૂર્ણાંક annotate = 0, force = 0;
	પૂર્ણાંક cmdmode = 0, create_tag_object = 0;
	અક્ષર *msgfile = ખાલી;
	અચલ અક્ષર *keyid = ખાલી;
	સંરચના સંદેશ_arg સંદેશ = { .buf = STRBUF_INIT };
	સંરચના ref_transaction *transaction;
	સંરચના strbuf err = STRBUF_INIT;
	સંરચના ref_filter filter = REF_FILTER_INIT;
	સંરચના ref_sorting *sorting;
	સંરચના string_list sorting_options = STRING_LIST_INIT_DUP;
	સંરચના ref_format format = REF_FORMAT_INIT;
	સંરચના strvec trailer_args = STRVEC_INIT;
	પૂર્ણાંક icase = 0;
	પૂર્ણાંક edit_flag = 0;
	સંરચના option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.કિંમત = &filter.લીટીઓ,
			.precision = કદ(filter.લીટીઓ),
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
		OPT_CALLBACK_F('m', "message", &સંદેશ, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_સંદેશ_arg),
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
			.કિંમત = &filter.points_at,
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
	પૂર્ણાંક ret = 0;
	અચલ અક્ષર *only_in_list = ખાલી;
	અક્ષર *path = ખાલી;

	setup_ref_filter_porcelain_સંદેશ();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	જો (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, કદ(opt));
	filter.લીટીઓ = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	જો (!cmdmode) {
		જો (argc == 0)
			cmdmode = 'l';
		નહીંતર જો (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.લીટીઓ != -1)
			cmdmode = 'l';
	}

	જો (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	જો (trailer_args.nr)
		trailer_config_init();

	જો (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	જો (keyid) {
		opt.sign = 1;
		set_signing_ચાવી(keyid);
	}
	create_tag_object = (opt.sign || annotate || સંદેશ.given || msgfile ||
			     edit_flag || trailer_args.nr);

	જો ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	જો (cmdmode == 'l' && filter.લીટીઓ != -1) {
		જો (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	જો (cmdmode == 'l') {
		જો (column_active(colopts)) {
			સંરચના column_options copts;
			memset(&copts, 0, કદ(copts));
			copts.padding = 2;
			જો (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		જો (column_active(colopts))
			stop_column_filter();
		જાઓ cleanup;
	}
	જો (filter.લીટીઓ != -1)
		only_in_list = "-n";
	નહીંતર જો (filter.with_commit)
		only_in_list = "--contains";
	નહીંતર જો (filter.no_commit)
		only_in_list = "--no-contains";
	નહીંતર જો (filter.points_at.nr)
		only_in_list = "--points-at";
	નહીંતર જો (filter.reachable_from)
		only_in_list = "--merged";
	નહીંતર જો (filter.unreachable_from)
		only_in_list = "--no-merged";
	જો (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	જો (cmdmode == 'd') {
		ret = delete_tags(argv);
		જાઓ cleanup;
	}
	જો (cmdmode == 'v') {
		જો (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		જાઓ cleanup;
	}

	જો (સંદેશ.given || msgfile) {
		જો (સંદેશ.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		જો (સંદેશ.given)
			strbuf_addbuf(&buf, &(સંદેશ.buf));
		નહીંતર {
			જો (!strcmp(msgfile, "-")) {
				જો (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} નહીંતર {
				જો (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	જો (argc > 2)
		die(_("too many arguments"));

	જો (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	જો (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	જો (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	નહીંતર જો (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = સંદેશ.given || msgfile;
	opt.use_editor = edit_flag;

	જો (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	નહીંતર જો (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	નહીંતર જો (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	નહીંતર
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_સંદેશ(&object, &reflog_સંદેશ);

	જો (create_tag_object) {
		જો (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	જો (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ખાલી, ખાલી,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_સંદેશ.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		જો (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	જો (path) {
		unlink_or_warn(path);
		મુક્ત(path);
	}
	ref_transaction_free(transaction);
	જો (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_સંદેશ);
	strbuf_release(&સંદેશ.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	મુક્ત(msgfile);
	પાછા ret;
}
