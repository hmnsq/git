/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "advice.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "diff.h"
#അന്തര്ഭാവ "revision.h"
#അന്തര്ഭാവ "gpg-interface.h"
#അന്തര്ഭാവ "oid-array.h"
#അന്തര്ഭാവ "column.h"
#അന്തര്ഭാവ "ref-filter.h"
#അന്തര്ഭാവ "date.h"
#അന്തര്ഭാവ "write-or-die.h"
#അന്തര്ഭാവ "object-file-convert.h"
#അന്തര്ഭാവ "trailer.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ശൂന്യ
};

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ colopts;
സ്ഥിതം പൂർണ്ണസംഖ്യ force_sign_annotate;
സ്ഥിതം പൂർണ്ണസംഖ്യ config_sign_tag = -1; /* unspecified */

സ്ഥിതം പൂർണ്ണസംഖ്യ list_tags(ഘടന ref_filter *filter, ഘടന ref_sorting *sorting,
		     ഘടന ref_format *format)
{
	അക്ഷരം *to_free = ശൂന്യ;

	എങ്കിൽ (filter->വരികൾ == -1)
		filter->വരികൾ = 0;

	എങ്കിൽ (!format->format) {
		എങ്കിൽ (filter->വരികൾ) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->വരികൾ);
			format->format = to_free;
		} അല്ലെങ്കിൽ
			format->format = "%(refname:lstrip=2)";
	}

	എങ്കിൽ (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	സ്വതന്ത്രം(to_free);

	തിരികെ 0;
}

തരനിർവചനം പൂർണ്ണസംഖ്യ (*each_tag_name_fn)(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *ref,
				സ്ഥിരം ഘടന object_id *oid, ശൂന്യം *cb_data);

സ്ഥിതം പൂർണ്ണസംഖ്യ for_each_tag_name(സ്ഥിരം അക്ഷരം **argv, each_tag_name_fn fn,
			     ശൂന്യം *cb_data)
{
	സ്ഥിരം അക്ഷരം **p;
	ഘടന strbuf ref = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ had_error = 0;
	ഘടന object_id oid;

	വേണ്ടി (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			തുടരുക;
		}
		എങ്കിൽ (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	തിരികെ had_error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ collect_tags(സ്ഥിരം അക്ഷരം *name UNUSED, സ്ഥിരം അക്ഷരം *ref,
			സ്ഥിരം ഘടന object_id *oid, ശൂന്യം *cb_data)
{
	ഘടന string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->ഇനങ്ങൾ[ref_list->nr - 1].util = oiddup(oid);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_tags(സ്ഥിരം അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ result;
	ഘടന string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ഘടന string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (ശൂന്യം *)&refs_to_delete);
	എങ്കിൽ (refs_delete_refs(get_main_ref_store(the_repository), ശൂന്യ, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		സ്ഥിരം അക്ഷരം *name = item->string;
		ഘടന object_id *oid = item->util;
		എങ്കിൽ (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		സ്വതന്ത്രം(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	തിരികെ result;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ verify_tag(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *ref UNUSED,
		      സ്ഥിരം ഘടന object_id *oid, ശൂന്യം *cb_data)
{
	പൂർണ്ണസംഖ്യ flags;
	ഘടന ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	എങ്കിൽ (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	എങ്കിൽ (gpg_verify_tag(the_repository, oid, name, flags))
		തിരികെ -1;

	എങ്കിൽ (format->format)
		pretty_print_ref(name, oid, ശൂന്യ, format);

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_sign(ഘടന strbuf *buffer, ഘടന object_id **compat_oid,
		   ഘടന object_id *compat_oid_buf)
{
	സ്ഥിരം ഘടന git_hash_algo *compat = the_repository->compat_hash_algo;
	ഘടന strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ഘടന strbuf compat_buf = STRBUF_INIT;
	അക്ഷരം *keyid = get_signing_താക്കോൽ();
	പൂർണ്ണസംഖ്യ ret = -1;

	എങ്കിൽ (sign_buffer(buffer, &sig, keyid, 0))
		പോകുക out;

	എങ്കിൽ (compat) {
		സ്ഥിരം ഘടന git_hash_algo *algo = the_repository->hash_algo;

		എങ്കിൽ (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			പോകുക out;
		എങ്കിൽ (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			പോകുക out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	എങ്കിൽ (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	സ്വതന്ത്രം(keyid);
	തിരികെ ret;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

സ്ഥിതം സ്ഥിരം അക്ഷരം tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

സ്ഥിതം പൂർണ്ണസംഖ്യ git_tag_config(സ്ഥിരം അക്ഷരം *var, സ്ഥിരം അക്ഷരം *മൂല്യം,
			  സ്ഥിരം ഘടന config_context *ctx, ശൂന്യം *cb)
{
	എങ്കിൽ (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "tag.sort")) {
		എങ്കിൽ (!മൂല്യം)
			തിരികെ config_error_nonbool(var);
		string_list_append(cb, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, മൂല്യം);
		തിരികെ 0;
	}

	എങ്കിൽ (starts_with(var, "column."))
		തിരികെ git_column_config(var, മൂല്യം, "tag", &colopts);

	എങ്കിൽ (git_color_config(var, മൂല്യം, cb) < 0)
		തിരികെ -1;

	തിരികെ git_default_config(var, മൂല്യം, ctx, cb);
}

സ്ഥിതം ശൂന്യം write_tag_body(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന object_id *oid)
{
	size_t size;
	എണ്ണൽ object_type type;
	അക്ഷരം *buf, *sp, *orig;
	ഘടന strbuf payload = STRBUF_INIT;
	ഘടന strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	എങ്കിൽ (!buf)
		തിരികെ;
	എങ്കിൽ (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	എങ്കിൽ (!sp || !size || type != OBJ_TAG) {
		സ്വതന്ത്രം(buf);
		തിരികെ;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	സ്വതന്ത്രം(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ build_tag_object(ഘടന strbuf *buf, പൂർണ്ണസംഖ്യ sign, ഘടന object_id *result)
{
	ഘടന object_id *compat_oid = ശൂന്യ, compat_oid_buf;
	എങ്കിൽ (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		തിരികെ error(_("unable to sign the tag"));
	എങ്കിൽ (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		തിരികെ error(_("unable to write tag file"));
	തിരികെ 0;
}

ഘടന create_tag_options {
	unsigned പൂർണ്ണസംഖ്യ message_given:1;
	unsigned പൂർണ്ണസംഖ്യ use_editor:1;
	unsigned പൂർണ്ണസംഖ്യ sign;
	എണ്ണൽ {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

സ്ഥിതം സ്ഥിരം അക്ഷരം message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

സ്ഥിതം ശൂന്യം create_tag(സ്ഥിരം ഘടന object_id *object, സ്ഥിരം അക്ഷരം *object_ref,
		       സ്ഥിരം അക്ഷരം *tag,
		       ഘടന strbuf *buf, ഘടന create_tag_options *opt,
		       ഘടന object_id *prev, ഘടന object_id *result,
		       ഘടന strvec *trailer_args, അക്ഷരം *path)
{
	എണ്ണൽ object_type type;
	ഘടന strbuf header = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ should_edit;

	type = odb_read_object_info(the_repository->objects, object, ശൂന്യ);
	എങ്കിൽ (type <= OBJ_NONE)
		die(_("bad object type."));

	എങ്കിൽ (type == OBJ_TAG)
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
	എങ്കിൽ (should_edit || trailer_args->nr) {
		പൂർണ്ണസംഖ്യ fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		എങ്കിൽ (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} അല്ലെങ്കിൽ എങ്കിൽ (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} അല്ലെങ്കിൽ {
			ഘടന strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			എങ്കിൽ (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_വരി_str,
				      _(tag_template), tag, comment_വരി_str);
			അല്ലെങ്കിൽ
				strbuf_commented_addf(&buf, comment_വരി_str,
				      _(tag_template_nocleanup), tag, comment_വരി_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		എങ്കിൽ (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		എങ്കിൽ (should_edit) {
			എങ്കിൽ (launch_editor(path, buf, ശൂന്യ)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (trailer_args->nr) {
			strbuf_reset(buf);
			എങ്കിൽ (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	എങ്കിൽ (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_വരി_str : ശൂന്യ);

	എങ്കിൽ (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_തിരുകുക(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	എങ്കിൽ (build_tag_object(buf, opt->sign, result) < 0) {
		എങ്കിൽ (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

സ്ഥിതം ശൂന്യം create_reflog_സന്ദേശം(സ്ഥിരം ഘടന object_id *oid, ഘടന strbuf *sb)
{
	എണ്ണൽ object_type type;
	ഘടന commit *c;
	അക്ഷരം *buf;
	size_t size;
	പൂർണ്ണസംഖ്യ subject_len = 0;
	സ്ഥിരം അക്ഷരം *subject_start;

	അക്ഷരം *rla = getenv("GIT_REFLOG_ACTION");
	എങ്കിൽ (rla) {
		strbuf_addstr(sb, rla);
	} അല്ലെങ്കിൽ {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ശൂന്യ);
	തിരഞ്ഞെടുപ്പ് (type) {
	സ്ഥിരസ്ഥിതി:
		strbuf_addstr(sb, "object of unknown type");
		നിർത്തുക;
	അവസ്ഥ OBJ_COMMIT:
		എങ്കിൽ ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_തിരുകുക(sb, sb->len, subject_start, subject_len);
		} അല്ലെങ്കിൽ {
			strbuf_addstr(sb, "commit object");
		}
		സ്വതന്ത്രം(buf);

		എങ്കിൽ ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		നിർത്തുക;
	അവസ്ഥ OBJ_വൃക്ഷം:
		strbuf_addstr(sb, "tree object");
		നിർത്തുക;
	അവസ്ഥ OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		നിർത്തുക;
	അവസ്ഥ OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		നിർത്തുക;
	}
	strbuf_addch(sb, ')');
}

ഘടന സന്ദേശം_arg {
	പൂർണ്ണസംഖ്യ given;
	ഘടന strbuf buf;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_സന്ദേശം_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന സന്ദേശം_arg *സന്ദേശം = opt->മൂല്യം;

	BUG_ON_OPT_NEG(unset);

	എങ്കിൽ (!arg)
		തിരികെ -1;
	എങ്കിൽ (സന്ദേശം->buf.len)
		strbuf_addstr(&(സന്ദേശം->buf), "\n\n");
	strbuf_addstr(&(സന്ദേശം->buf), arg);
	സന്ദേശം->given = 1;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_tag(പൂർണ്ണസംഖ്യ argc,
	    സ്ഥിരം അക്ഷരം **argv,
	    സ്ഥിരം അക്ഷരം *prefix,
	    ഘടന repository *repo UNUSED)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strbuf ref = STRBUF_INIT;
	ഘടന strbuf reflog_സന്ദേശം = STRBUF_INIT;
	ഘടന object_id object, prev;
	സ്ഥിരം അക്ഷരം *object_ref, *tag;
	ഘടന create_tag_options opt;
	അക്ഷരം *cleanup_arg = ശൂന്യ;
	പൂർണ്ണസംഖ്യ create_reflog = 0;
	പൂർണ്ണസംഖ്യ annotate = 0, force = 0;
	പൂർണ്ണസംഖ്യ cmdmode = 0, create_tag_object = 0;
	അക്ഷരം *msgfile = ശൂന്യ;
	സ്ഥിരം അക്ഷരം *keyid = ശൂന്യ;
	ഘടന സന്ദേശം_arg സന്ദേശം = { .buf = STRBUF_INIT };
	ഘടന ref_transaction *transaction;
	ഘടന strbuf err = STRBUF_INIT;
	ഘടന ref_filter filter = REF_FILTER_INIT;
	ഘടന ref_sorting *sorting;
	ഘടന string_list sorting_options = STRING_LIST_INIT_DUP;
	ഘടന ref_format format = REF_FORMAT_INIT;
	ഘടന strvec trailer_args = STRVEC_INIT;
	പൂർണ്ണസംഖ്യ icase = 0;
	പൂർണ്ണസംഖ്യ edit_flag = 0;
	ഘടന option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.മൂല്യം = &filter.വരികൾ,
			.precision = വലുപ്പം(filter.വരികൾ),
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
		OPT_CALLBACK_F('m', "message", &സന്ദേശം, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_സന്ദേശം_arg),
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
			.മൂല്യം = &filter.points_at,
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
	പൂർണ്ണസംഖ്യ ret = 0;
	സ്ഥിരം അക്ഷരം *only_in_list = ശൂന്യ;
	അക്ഷരം *path = ശൂന്യ;

	setup_ref_filter_porcelain_സന്ദേശം();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	എങ്കിൽ (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, വലുപ്പം(opt));
	filter.വരികൾ = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	എങ്കിൽ (!cmdmode) {
		എങ്കിൽ (argc == 0)
			cmdmode = 'l';
		അല്ലെങ്കിൽ എങ്കിൽ (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.വരികൾ != -1)
			cmdmode = 'l';
	}

	എങ്കിൽ (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	എങ്കിൽ (trailer_args.nr)
		trailer_config_init();

	എങ്കിൽ (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	എങ്കിൽ (keyid) {
		opt.sign = 1;
		set_signing_താക്കോൽ(keyid);
	}
	create_tag_object = (opt.sign || annotate || സന്ദേശം.given || msgfile ||
			     edit_flag || trailer_args.nr);

	എങ്കിൽ ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	എങ്കിൽ (cmdmode == 'l' && filter.വരികൾ != -1) {
		എങ്കിൽ (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	എങ്കിൽ (cmdmode == 'l') {
		എങ്കിൽ (column_active(colopts)) {
			ഘടന column_options copts;
			memset(&copts, 0, വലുപ്പം(copts));
			copts.padding = 2;
			എങ്കിൽ (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		എങ്കിൽ (column_active(colopts))
			stop_column_filter();
		പോകുക cleanup;
	}
	എങ്കിൽ (filter.വരികൾ != -1)
		only_in_list = "-n";
	അല്ലെങ്കിൽ എങ്കിൽ (filter.with_commit)
		only_in_list = "--contains";
	അല്ലെങ്കിൽ എങ്കിൽ (filter.no_commit)
		only_in_list = "--no-contains";
	അല്ലെങ്കിൽ എങ്കിൽ (filter.points_at.nr)
		only_in_list = "--points-at";
	അല്ലെങ്കിൽ എങ്കിൽ (filter.reachable_from)
		only_in_list = "--merged";
	അല്ലെങ്കിൽ എങ്കിൽ (filter.unreachable_from)
		only_in_list = "--no-merged";
	എങ്കിൽ (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	എങ്കിൽ (cmdmode == 'd') {
		ret = delete_tags(argv);
		പോകുക cleanup;
	}
	എങ്കിൽ (cmdmode == 'v') {
		എങ്കിൽ (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		പോകുക cleanup;
	}

	എങ്കിൽ (സന്ദേശം.given || msgfile) {
		എങ്കിൽ (സന്ദേശം.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		എങ്കിൽ (സന്ദേശം.given)
			strbuf_addbuf(&buf, &(സന്ദേശം.buf));
		അല്ലെങ്കിൽ {
			എങ്കിൽ (!strcmp(msgfile, "-")) {
				എങ്കിൽ (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} അല്ലെങ്കിൽ {
				എങ്കിൽ (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	എങ്കിൽ (argc > 2)
		die(_("too many arguments"));

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	എങ്കിൽ (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	അല്ലെങ്കിൽ എങ്കിൽ (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = സന്ദേശം.given || msgfile;
	opt.use_editor = edit_flag;

	എങ്കിൽ (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	അല്ലെങ്കിൽ
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_സന്ദേശം(&object, &reflog_സന്ദേശം);

	എങ്കിൽ (create_tag_object) {
		എങ്കിൽ (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	എങ്കിൽ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ശൂന്യ, ശൂന്യ,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_സന്ദേശം.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		എങ്കിൽ (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	എങ്കിൽ (path) {
		unlink_or_warn(path);
		സ്വതന്ത്രം(path);
	}
	ref_transaction_free(transaction);
	എങ്കിൽ (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_സന്ദേശം);
	strbuf_release(&സന്ദേശം.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	സ്വതന്ത്രം(msgfile);
	തിരികെ ret;
}
