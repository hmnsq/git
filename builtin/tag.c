/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "advice.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "diff.h"
#அந்தர்பாவ "revision.h"
#அந்தர்பாவ "gpg-interface.h"
#அந்தர்பாவ "oid-array.h"
#அந்தர்பாவ "column.h"
#அந்தர்பாவ "ref-filter.h"
#அந்தர்பாவ "date.h"
#அந்தர்பாவ "write-or-die.h"
#அந்தர்பாவ "object-file-convert.h"
#அந்தர்பாவ "trailer.h"

நிலையான மாறிலி எழுத்து * மாறிலி git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	வெற்று
};

நிலையான unsigned முழு colopts;
நிலையான முழு force_sign_annotate;
நிலையான முழு config_sign_tag = -1; /* unspecified */

நிலையான முழு list_tags(கட்டமைப்பு ref_filter *filter, கட்டமைப்பு ref_sorting *sorting,
		     கட்டமைப்பு ref_format *format)
{
	எழுத்து *to_free = வெற்று;

	எனில் (filter->வரிகள் == -1)
		filter->வரிகள் = 0;

	எனில் (!format->format) {
		எனில் (filter->வரிகள்) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->வரிகள்);
			format->format = to_free;
		} இல்லையெனில்
			format->format = "%(refname:lstrip=2)";
	}

	எனில் (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	விடுவி(to_free);

	திரும்பு 0;
}

வகைவரை முழு (*each_tag_name_fn)(மாறிலி எழுத்து *name, மாறிலி எழுத்து *ref,
				மாறிலி கட்டமைப்பு object_id *oid, வெறுமை *cb_data);

நிலையான முழு for_each_tag_name(மாறிலி எழுத்து **argv, each_tag_name_fn fn,
			     வெறுமை *cb_data)
{
	மாறிலி எழுத்து **p;
	கட்டமைப்பு strbuf ref = STRBUF_INIT;
	முழு had_error = 0;
	கட்டமைப்பு object_id oid;

	க்கு (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		எனில் (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			தொடர்;
		}
		எனில் (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	திரும்பு had_error;
}

நிலையான முழு collect_tags(மாறிலி எழுத்து *name UNUSED, மாறிலி எழுத்து *ref,
			மாறிலி கட்டமைப்பு object_id *oid, வெறுமை *cb_data)
{
	கட்டமைப்பு string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->பொருட்கள்[ref_list->nr - 1].util = oiddup(oid);
	திரும்பு 0;
}

நிலையான முழு delete_tags(மாறிலி எழுத்து **argv)
{
	முழு result;
	கட்டமைப்பு string_list refs_to_delete = STRING_LIST_INIT_DUP;
	கட்டமைப்பு string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (வெறுமை *)&refs_to_delete);
	எனில் (refs_delete_refs(get_main_ref_store(the_repository), வெற்று, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		மாறிலி எழுத்து *name = item->string;
		கட்டமைப்பு object_id *oid = item->util;
		எனில் (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		விடுவி(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	திரும்பு result;
}

நிலையான முழு verify_tag(மாறிலி எழுத்து *name, மாறிலி எழுத்து *ref UNUSED,
		      மாறிலி கட்டமைப்பு object_id *oid, வெறுமை *cb_data)
{
	முழு flags;
	கட்டமைப்பு ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	எனில் (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	எனில் (gpg_verify_tag(the_repository, oid, name, flags))
		திரும்பு -1;

	எனில் (format->format)
		pretty_print_ref(name, oid, வெற்று, format);

	திரும்பு 0;
}

நிலையான முழு do_sign(கட்டமைப்பு strbuf *buffer, கட்டமைப்பு object_id **compat_oid,
		   கட்டமைப்பு object_id *compat_oid_buf)
{
	மாறிலி கட்டமைப்பு git_hash_algo *compat = the_repository->compat_hash_algo;
	கட்டமைப்பு strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	கட்டமைப்பு strbuf compat_buf = STRBUF_INIT;
	எழுத்து *keyid = get_signing_சாவி();
	முழு ret = -1;

	எனில் (sign_buffer(buffer, &sig, keyid, 0))
		செல் out;

	எனில் (compat) {
		மாறிலி கட்டமைப்பு git_hash_algo *algo = the_repository->hash_algo;

		எனில் (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			செல் out;
		எனில் (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			செல் out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	எனில் (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	விடுவி(keyid);
	திரும்பு ret;
}

நிலையான மாறிலி எழுத்து tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

நிலையான மாறிலி எழுத்து tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

நிலையான முழு git_tag_config(மாறிலி எழுத்து *var, மாறிலி எழுத்து *மதிப்பு,
			  மாறிலி கட்டமைப்பு config_context *ctx, வெறுமை *cb)
{
	எனில் (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "tag.sort")) {
		எனில் (!மதிப்பு)
			திரும்பு config_error_nonbool(var);
		string_list_append(cb, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, மதிப்பு);
		திரும்பு 0;
	}

	எனில் (starts_with(var, "column."))
		திரும்பு git_column_config(var, மதிப்பு, "tag", &colopts);

	எனில் (git_color_config(var, மதிப்பு, cb) < 0)
		திரும்பு -1;

	திரும்பு git_default_config(var, மதிப்பு, ctx, cb);
}

நிலையான வெறுமை write_tag_body(முழு fd, மாறிலி கட்டமைப்பு object_id *oid)
{
	size_t size;
	எண்ணிக்கை object_type type;
	எழுத்து *buf, *sp, *orig;
	கட்டமைப்பு strbuf payload = STRBUF_INIT;
	கட்டமைப்பு strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	எனில் (!buf)
		திரும்பு;
	எனில் (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	எனில் (!sp || !size || type != OBJ_TAG) {
		விடுவி(buf);
		திரும்பு;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	விடுவி(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

நிலையான முழு build_tag_object(கட்டமைப்பு strbuf *buf, முழு sign, கட்டமைப்பு object_id *result)
{
	கட்டமைப்பு object_id *compat_oid = வெற்று, compat_oid_buf;
	எனில் (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		திரும்பு error(_("unable to sign the tag"));
	எனில் (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		திரும்பு error(_("unable to write tag file"));
	திரும்பு 0;
}

கட்டமைப்பு create_tag_options {
	unsigned முழு message_given:1;
	unsigned முழு use_editor:1;
	unsigned முழு sign;
	எண்ணிக்கை {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

நிலையான மாறிலி எழுத்து message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

நிலையான வெறுமை create_tag(மாறிலி கட்டமைப்பு object_id *object, மாறிலி எழுத்து *object_ref,
		       மாறிலி எழுத்து *tag,
		       கட்டமைப்பு strbuf *buf, கட்டமைப்பு create_tag_options *opt,
		       கட்டமைப்பு object_id *prev, கட்டமைப்பு object_id *result,
		       கட்டமைப்பு strvec *trailer_args, எழுத்து *path)
{
	எண்ணிக்கை object_type type;
	கட்டமைப்பு strbuf header = STRBUF_INIT;
	முழு should_edit;

	type = odb_read_object_info(the_repository->objects, object, வெற்று);
	எனில் (type <= OBJ_NONE)
		die(_("bad object type."));

	எனில் (type == OBJ_TAG)
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
	எனில் (should_edit || trailer_args->nr) {
		முழு fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		எனில் (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} இல்லையெனில் எனில் (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} இல்லையெனில் {
			கட்டமைப்பு strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			எனில் (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_வரி_str,
				      _(tag_template), tag, comment_வரி_str);
			இல்லையெனில்
				strbuf_commented_addf(&buf, comment_வரி_str,
				      _(tag_template_nocleanup), tag, comment_வரி_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		எனில் (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		எனில் (should_edit) {
			எனில் (launch_editor(path, buf, வெற்று)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} இல்லையெனில் எனில் (trailer_args->nr) {
			strbuf_reset(buf);
			எனில் (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	எனில் (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_வரி_str : வெற்று);

	எனில் (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_செருகு(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	எனில் (build_tag_object(buf, opt->sign, result) < 0) {
		எனில் (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

நிலையான வெறுமை create_reflog_செய்தி(மாறிலி கட்டமைப்பு object_id *oid, கட்டமைப்பு strbuf *sb)
{
	எண்ணிக்கை object_type type;
	கட்டமைப்பு commit *c;
	எழுத்து *buf;
	size_t size;
	முழு subject_len = 0;
	மாறிலி எழுத்து *subject_start;

	எழுத்து *rla = getenv("GIT_REFLOG_ACTION");
	எனில் (rla) {
		strbuf_addstr(sb, rla);
	} இல்லையெனில் {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, வெற்று);
	தேர்வு (type) {
	இயல்பு:
		strbuf_addstr(sb, "object of unknown type");
		நிறுத்து;
	நிலை OBJ_COMMIT:
		எனில் ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_செருகு(sb, sb->len, subject_start, subject_len);
		} இல்லையெனில் {
			strbuf_addstr(sb, "commit object");
		}
		விடுவி(buf);

		எனில் ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		நிறுத்து;
	நிலை OBJ_மரம்:
		strbuf_addstr(sb, "tree object");
		நிறுத்து;
	நிலை OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		நிறுத்து;
	நிலை OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		நிறுத்து;
	}
	strbuf_addch(sb, ')');
}

கட்டமைப்பு செய்தி_arg {
	முழு given;
	கட்டமைப்பு strbuf buf;
};

நிலையான முழு parse_செய்தி_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு செய்தி_arg *செய்தி = opt->மதிப்பு;

	BUG_ON_OPT_NEG(unset);

	எனில் (!arg)
		திரும்பு -1;
	எனில் (செய்தி->buf.len)
		strbuf_addstr(&(செய்தி->buf), "\n\n");
	strbuf_addstr(&(செய்தி->buf), arg);
	செய்தி->given = 1;
	திரும்பு 0;
}

முழு cmd_tag(முழு argc,
	    மாறிலி எழுத்து **argv,
	    மாறிலி எழுத்து *prefix,
	    கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf ref = STRBUF_INIT;
	கட்டமைப்பு strbuf reflog_செய்தி = STRBUF_INIT;
	கட்டமைப்பு object_id object, prev;
	மாறிலி எழுத்து *object_ref, *tag;
	கட்டமைப்பு create_tag_options opt;
	எழுத்து *cleanup_arg = வெற்று;
	முழு create_reflog = 0;
	முழு annotate = 0, force = 0;
	முழு cmdmode = 0, create_tag_object = 0;
	எழுத்து *msgfile = வெற்று;
	மாறிலி எழுத்து *keyid = வெற்று;
	கட்டமைப்பு செய்தி_arg செய்தி = { .buf = STRBUF_INIT };
	கட்டமைப்பு ref_transaction *transaction;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	கட்டமைப்பு ref_filter filter = REF_FILTER_INIT;
	கட்டமைப்பு ref_sorting *sorting;
	கட்டமைப்பு string_list sorting_options = STRING_LIST_INIT_DUP;
	கட்டமைப்பு ref_format format = REF_FORMAT_INIT;
	கட்டமைப்பு strvec trailer_args = STRVEC_INIT;
	முழு icase = 0;
	முழு edit_flag = 0;
	கட்டமைப்பு option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.மதிப்பு = &filter.வரிகள்,
			.precision = அளவு(filter.வரிகள்),
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
		OPT_CALLBACK_F('m', "message", &செய்தி, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_செய்தி_arg),
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
			.மதிப்பு = &filter.points_at,
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
	முழு ret = 0;
	மாறிலி எழுத்து *only_in_list = வெற்று;
	எழுத்து *path = வெற்று;

	setup_ref_filter_porcelain_செய்தி();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	எனில் (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, அளவு(opt));
	filter.வரிகள் = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	எனில் (!cmdmode) {
		எனில் (argc == 0)
			cmdmode = 'l';
		இல்லையெனில் எனில் (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.வரிகள் != -1)
			cmdmode = 'l';
	}

	எனில் (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	எனில் (trailer_args.nr)
		trailer_config_init();

	எனில் (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	எனில் (keyid) {
		opt.sign = 1;
		set_signing_சாவி(keyid);
	}
	create_tag_object = (opt.sign || annotate || செய்தி.given || msgfile ||
			     edit_flag || trailer_args.nr);

	எனில் ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	எனில் (cmdmode == 'l' && filter.வரிகள் != -1) {
		எனில் (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	எனில் (cmdmode == 'l') {
		எனில் (column_active(colopts)) {
			கட்டமைப்பு column_options copts;
			memset(&copts, 0, அளவு(copts));
			copts.padding = 2;
			எனில் (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		எனில் (column_active(colopts))
			stop_column_filter();
		செல் cleanup;
	}
	எனில் (filter.வரிகள் != -1)
		only_in_list = "-n";
	இல்லையெனில் எனில் (filter.with_commit)
		only_in_list = "--contains";
	இல்லையெனில் எனில் (filter.no_commit)
		only_in_list = "--no-contains";
	இல்லையெனில் எனில் (filter.points_at.nr)
		only_in_list = "--points-at";
	இல்லையெனில் எனில் (filter.reachable_from)
		only_in_list = "--merged";
	இல்லையெனில் எனில் (filter.unreachable_from)
		only_in_list = "--no-merged";
	எனில் (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	எனில் (cmdmode == 'd') {
		ret = delete_tags(argv);
		செல் cleanup;
	}
	எனில் (cmdmode == 'v') {
		எனில் (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		செல் cleanup;
	}

	எனில் (செய்தி.given || msgfile) {
		எனில் (செய்தி.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		எனில் (செய்தி.given)
			strbuf_addbuf(&buf, &(செய்தி.buf));
		இல்லையெனில் {
			எனில் (!strcmp(msgfile, "-")) {
				எனில் (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} இல்லையெனில் {
				எனில் (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	எனில் (argc > 2)
		die(_("too many arguments"));

	எனில் (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	எனில் (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	எனில் (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	இல்லையெனில் எனில் (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = செய்தி.given || msgfile;
	opt.use_editor = edit_flag;

	எனில் (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	இல்லையெனில் எனில் (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	இல்லையெனில் எனில் (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	இல்லையெனில்
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_செய்தி(&object, &reflog_செய்தி);

	எனில் (create_tag_object) {
		எனில் (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	எனில் (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   வெற்று, வெற்று,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_செய்தி.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		எனில் (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	எனில் (path) {
		unlink_or_warn(path);
		விடுவி(path);
	}
	ref_transaction_free(transaction);
	எனில் (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_செய்தி);
	strbuf_release(&செய்தி.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	விடுவி(msgfile);
	திரும்பு ret;
}
