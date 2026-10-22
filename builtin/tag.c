/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#অন্তর্ভাৱ "builtin.h"
#অন্তর্ভাৱ "advice.h"
#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ "editor.h"
#অন্তর্ভাৱ "environment.h"
#অন্তর্ভাৱ "gettext.h"
#অন্তর্ভাৱ "hex.h"
#অন্তর্ভাৱ "refs.h"
#অন্তর্ভাৱ "object-file.h"
#অন্তর্ভাৱ "object-name.h"
#অন্তর্ভাৱ "odb.h"
#অন্তর্ভাৱ "path.h"
#অন্তর্ভাৱ "tag.h"
#অন্তর্ভাৱ "parse-options.h"
#অন্তর্ভাৱ "diff.h"
#অন্তর্ভাৱ "revision.h"
#অন্তর্ভাৱ "gpg-interface.h"
#অন্তর্ভাৱ "oid-array.h"
#অন্তর্ভাৱ "column.h"
#অন্তর্ভাৱ "ref-filter.h"
#অন্তর্ভাৱ "date.h"
#অন্তর্ভাৱ "write-or-die.h"
#অন্তর্ভাৱ "object-file-convert.h"
#অন্তর্ভাৱ "trailer.h"

স্থির ধ্রুৱ ৱর্ণ * ধ্রুৱ git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	রিক্ত
};

স্থির unsigned পূর্ণাঙ্ক colopts;
স্থির পূর্ণাঙ্ক force_sign_annotate;
স্থির পূর্ণাঙ্ক config_sign_tag = -1; /* unspecified */

স্থির পূর্ণাঙ্ক list_tags(সংরচনা ref_filter *filter, সংরচনা ref_sorting *sorting,
		     সংরচনা ref_format *format)
{
	ৱর্ণ *to_free = রিক্ত;

	যদি (filter->সাৰিগুলি == -1)
		filter->সাৰিগুলি = 0;

	যদি (!format->format) {
		যদি (filter->সাৰিগুলি) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->সাৰিগুলি);
			format->format = to_free;
		} অন্যথা
			format->format = "%(refname:lstrip=2)";
	}

	যদি (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	মুক্ত(to_free);

	ঘূৰাই 0;
}

প্রকারনির্দেশ পূর্ণাঙ্ক (*each_tag_name_fn)(ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *ref,
				ধ্রুৱ সংরচনা object_id *oid, শূন্য *cb_data);

স্থির পূর্ণাঙ্ক for_each_tag_name(ধ্রুৱ ৱর্ণ **argv, each_tag_name_fn fn,
			     শূন্য *cb_data)
{
	ধ্রুৱ ৱর্ণ **p;
	সংরচনা strbuf ref = STRBUF_INIT;
	পূর্ণাঙ্ক had_error = 0;
	সংরচনা object_id oid;

	বাবে (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		যদি (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			অনুৱর্ত;
		}
		যদি (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	ঘূৰাই had_error;
}

স্থির পূর্ণাঙ্ক collect_tags(ধ্রুৱ ৱর্ণ *name UNUSED, ধ্রুৱ ৱর্ণ *ref,
			ধ্রুৱ সংরচনা object_id *oid, শূন্য *cb_data)
{
	সংরচনা string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->বস্তুসমূহ[ref_list->nr - 1].util = oiddup(oid);
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক delete_tags(ধ্রুৱ ৱর্ণ **argv)
{
	পূর্ণাঙ্ক result;
	সংরচনা string_list refs_to_delete = STRING_LIST_INIT_DUP;
	সংরচনা string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (শূন্য *)&refs_to_delete);
	যদি (refs_delete_refs(get_main_ref_store(the_repository), রিক্ত, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ধ্রুৱ ৱর্ণ *name = item->string;
		সংরচনা object_id *oid = item->util;
		যদি (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		মুক্ত(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	ঘূৰাই result;
}

স্থির পূর্ণাঙ্ক verify_tag(ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *ref UNUSED,
		      ধ্রুৱ সংরচনা object_id *oid, শূন্য *cb_data)
{
	পূর্ণাঙ্ক flags;
	সংরচনা ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	যদি (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	যদি (gpg_verify_tag(the_repository, oid, name, flags))
		ঘূৰাই -1;

	যদি (format->format)
		pretty_print_ref(name, oid, রিক্ত, format);

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক do_sign(সংরচনা strbuf *buffer, সংরচনা object_id **compat_oid,
		   সংরচনা object_id *compat_oid_buf)
{
	ধ্রুৱ সংরচনা git_hash_algo *compat = the_repository->compat_hash_algo;
	সংরচনা strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	সংরচনা strbuf compat_buf = STRBUF_INIT;
	ৱর্ণ *keyid = get_signing_চাবি();
	পূর্ণাঙ্ক ret = -1;

	যদি (sign_buffer(buffer, &sig, keyid, 0))
		গচ্ছ out;

	যদি (compat) {
		ধ্রুৱ সংরচনা git_hash_algo *algo = the_repository->hash_algo;

		যদি (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			গচ্ছ out;
		যদি (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			গচ্ছ out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	যদি (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	মুক্ত(keyid);
	ঘূৰাই ret;
}

স্থির ধ্রুৱ ৱর্ণ tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

স্থির ধ্রুৱ ৱর্ণ tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

স্থির পূর্ণাঙ্ক git_tag_config(ধ্রুৱ ৱর্ণ *var, ধ্রুৱ ৱর্ণ *মান,
			  ধ্রুৱ সংরচনা config_context *ctx, শূন্য *cb)
{
	যদি (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, মান);
		ঘূৰাই 0;
	}

	যদি (!strcmp(var, "tag.sort")) {
		যদি (!মান)
			ঘূৰাই config_error_nonbool(var);
		string_list_append(cb, মান);
		ঘূৰাই 0;
	}

	যদি (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, মান);
		ঘূৰাই 0;
	}

	যদি (starts_with(var, "column."))
		ঘূৰাই git_column_config(var, মান, "tag", &colopts);

	যদি (git_color_config(var, মান, cb) < 0)
		ঘূৰাই -1;

	ঘূৰাই git_default_config(var, মান, ctx, cb);
}

স্থির শূন্য write_tag_body(পূর্ণাঙ্ক fd, ধ্রুৱ সংরচনা object_id *oid)
{
	size_t size;
	গণনা object_type type;
	ৱর্ণ *buf, *sp, *orig;
	সংরচনা strbuf payload = STRBUF_INIT;
	সংরচনা strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	যদি (!buf)
		ঘূৰাই;
	যদি (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	যদি (!sp || !size || type != OBJ_TAG) {
		মুক্ত(buf);
		ঘূৰাই;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	মুক্ত(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

স্থির পূর্ণাঙ্ক build_tag_object(সংরচনা strbuf *buf, পূর্ণাঙ্ক sign, সংরচনা object_id *result)
{
	সংরচনা object_id *compat_oid = রিক্ত, compat_oid_buf;
	যদি (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		ঘূৰাই error(_("unable to sign the tag"));
	যদি (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		ঘূৰাই error(_("unable to write tag file"));
	ঘূৰাই 0;
}

সংরচনা create_tag_options {
	unsigned পূর্ণাঙ্ক message_given:1;
	unsigned পূর্ণাঙ্ক use_editor:1;
	unsigned পূর্ণাঙ্ক sign;
	গণনা {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

স্থির ধ্রুৱ ৱর্ণ message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

স্থির শূন্য create_tag(ধ্রুৱ সংরচনা object_id *object, ধ্রুৱ ৱর্ণ *object_ref,
		       ধ্রুৱ ৱর্ণ *tag,
		       সংরচনা strbuf *buf, সংরচনা create_tag_options *opt,
		       সংরচনা object_id *prev, সংরচনা object_id *result,
		       সংরচনা strvec *trailer_args, ৱর্ণ *path)
{
	গণনা object_type type;
	সংরচনা strbuf header = STRBUF_INIT;
	পূর্ণাঙ্ক should_edit;

	type = odb_read_object_info(the_repository->objects, object, রিক্ত);
	যদি (type <= OBJ_NONE)
		die(_("bad object type."));

	যদি (type == OBJ_TAG)
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
	যদি (should_edit || trailer_args->nr) {
		পূর্ণাঙ্ক fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		যদি (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} অন্যথা যদি (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} অন্যথা {
			সংরচনা strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			যদি (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_সাৰি_str,
				      _(tag_template), tag, comment_সাৰি_str);
			অন্যথা
				strbuf_commented_addf(&buf, comment_সাৰি_str,
				      _(tag_template_nocleanup), tag, comment_সাৰি_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		যদি (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		যদি (should_edit) {
			যদি (launch_editor(path, buf, রিক্ত)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} অন্যথা যদি (trailer_args->nr) {
			strbuf_reset(buf);
			যদি (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	যদি (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_সাৰি_str : রিক্ত);

	যদি (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_ঢোকাও(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	যদি (build_tag_object(buf, opt->sign, result) < 0) {
		যদি (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

স্থির শূন্য create_reflog_বাৰ্তা(ধ্রুৱ সংরচনা object_id *oid, সংরচনা strbuf *sb)
{
	গণনা object_type type;
	সংরচনা commit *c;
	ৱর্ণ *buf;
	size_t size;
	পূর্ণাঙ্ক subject_len = 0;
	ধ্রুৱ ৱর্ণ *subject_start;

	ৱর্ণ *rla = getenv("GIT_REFLOG_ACTION");
	যদি (rla) {
		strbuf_addstr(sb, rla);
	} অন্যথা {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, রিক্ত);
	ৱিকল্প (type) {
	স্ৱভাৱ:
		strbuf_addstr(sb, "object of unknown type");
		ভঙ্গ;
	পক্ষ OBJ_COMMIT:
		যদি ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_ঢোকাও(sb, sb->len, subject_start, subject_len);
		} অন্যথা {
			strbuf_addstr(sb, "commit object");
		}
		মুক্ত(buf);

		যদি ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		ভঙ্গ;
	পক্ষ OBJ_বৃক্ষ:
		strbuf_addstr(sb, "tree object");
		ভঙ্গ;
	পক্ষ OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		ভঙ্গ;
	পক্ষ OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		ভঙ্গ;
	}
	strbuf_addch(sb, ')');
}

সংরচনা বাৰ্তা_arg {
	পূর্ণাঙ্ক given;
	সংরচনা strbuf buf;
};

স্থির পূর্ণাঙ্ক parse_বাৰ্তা_arg(ধ্রুৱ সংরচনা option *opt, ধ্রুৱ ৱর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা বাৰ্তা_arg *বাৰ্তা = opt->মান;

	BUG_ON_OPT_NEG(unset);

	যদি (!arg)
		ঘূৰাই -1;
	যদি (বাৰ্তা->buf.len)
		strbuf_addstr(&(বাৰ্তা->buf), "\n\n");
	strbuf_addstr(&(বাৰ্তা->buf), arg);
	বাৰ্তা->given = 1;
	ঘূৰাই 0;
}

পূর্ণাঙ্ক cmd_tag(পূর্ণাঙ্ক argc,
	    ধ্রুৱ ৱর্ণ **argv,
	    ধ্রুৱ ৱর্ণ *prefix,
	    সংরচনা repository *repo UNUSED)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা strbuf ref = STRBUF_INIT;
	সংরচনা strbuf reflog_বাৰ্তা = STRBUF_INIT;
	সংরচনা object_id object, prev;
	ধ্রুৱ ৱর্ণ *object_ref, *tag;
	সংরচনা create_tag_options opt;
	ৱর্ণ *cleanup_arg = রিক্ত;
	পূর্ণাঙ্ক create_reflog = 0;
	পূর্ণাঙ্ক annotate = 0, force = 0;
	পূর্ণাঙ্ক cmdmode = 0, create_tag_object = 0;
	ৱর্ণ *msgfile = রিক্ত;
	ধ্রুৱ ৱর্ণ *keyid = রিক্ত;
	সংরচনা বাৰ্তা_arg বাৰ্তা = { .buf = STRBUF_INIT };
	সংরচনা ref_transaction *transaction;
	সংরচনা strbuf err = STRBUF_INIT;
	সংরচনা ref_filter filter = REF_FILTER_INIT;
	সংরচনা ref_sorting *sorting;
	সংরচনা string_list sorting_options = STRING_LIST_INIT_DUP;
	সংরচনা ref_format format = REF_FORMAT_INIT;
	সংরচনা strvec trailer_args = STRVEC_INIT;
	পূর্ণাঙ্ক icase = 0;
	পূর্ণাঙ্ক edit_flag = 0;
	সংরচনা option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.মান = &filter.সাৰিগুলি,
			.precision = পরিমাণ(filter.সাৰিগুলি),
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
		OPT_CALLBACK_F('m', "message", &বাৰ্তা, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_বাৰ্তা_arg),
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
			.মান = &filter.points_at,
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
	পূর্ণাঙ্ক ret = 0;
	ধ্রুৱ ৱর্ণ *only_in_list = রিক্ত;
	ৱর্ণ *path = রিক্ত;

	setup_ref_filter_porcelain_বাৰ্তা();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	যদি (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, পরিমাণ(opt));
	filter.সাৰিগুলি = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	যদি (!cmdmode) {
		যদি (argc == 0)
			cmdmode = 'l';
		অন্যথা যদি (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.সাৰিগুলি != -1)
			cmdmode = 'l';
	}

	যদি (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	যদি (trailer_args.nr)
		trailer_config_init();

	যদি (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	যদি (keyid) {
		opt.sign = 1;
		set_signing_চাবি(keyid);
	}
	create_tag_object = (opt.sign || annotate || বাৰ্তা.given || msgfile ||
			     edit_flag || trailer_args.nr);

	যদি ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	যদি (cmdmode == 'l' && filter.সাৰিগুলি != -1) {
		যদি (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	যদি (cmdmode == 'l') {
		যদি (column_active(colopts)) {
			সংরচনা column_options copts;
			memset(&copts, 0, পরিমাণ(copts));
			copts.padding = 2;
			যদি (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		যদি (column_active(colopts))
			stop_column_filter();
		গচ্ছ cleanup;
	}
	যদি (filter.সাৰিগুলি != -1)
		only_in_list = "-n";
	অন্যথা যদি (filter.with_commit)
		only_in_list = "--contains";
	অন্যথা যদি (filter.no_commit)
		only_in_list = "--no-contains";
	অন্যথা যদি (filter.points_at.nr)
		only_in_list = "--points-at";
	অন্যথা যদি (filter.reachable_from)
		only_in_list = "--merged";
	অন্যথা যদি (filter.unreachable_from)
		only_in_list = "--no-merged";
	যদি (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	যদি (cmdmode == 'd') {
		ret = delete_tags(argv);
		গচ্ছ cleanup;
	}
	যদি (cmdmode == 'v') {
		যদি (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		গচ্ছ cleanup;
	}

	যদি (বাৰ্তা.given || msgfile) {
		যদি (বাৰ্তা.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		যদি (বাৰ্তা.given)
			strbuf_addbuf(&buf, &(বাৰ্তা.buf));
		অন্যথা {
			যদি (!strcmp(msgfile, "-")) {
				যদি (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} অন্যথা {
				যদি (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	যদি (argc > 2)
		die(_("too many arguments"));

	যদি (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	যদি (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	যদি (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	অন্যথা যদি (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = বাৰ্তা.given || msgfile;
	opt.use_editor = edit_flag;

	যদি (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	অন্যথা যদি (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	অন্যথা যদি (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	অন্যথা
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_বাৰ্তা(&object, &reflog_বাৰ্তা);

	যদি (create_tag_object) {
		যদি (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   রিক্ত, রিক্ত,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_বাৰ্তা.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		যদি (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	যদি (path) {
		unlink_or_warn(path);
		মুক্ত(path);
	}
	ref_transaction_free(transaction);
	যদি (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_বাৰ্তা);
	strbuf_release(&বাৰ্তা.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	মুক্ত(msgfile);
	ঘূৰাই ret;
}
