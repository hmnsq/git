/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#include "builtin.h"
#include "advice.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "refs.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "path.h"
#include "tag.h"
#include "parse-options.h"
#include "diff.h"
#include "revision.h"
#include "gpg-interface.h"
#include "oid-array.h"
#include "column.h"
#include "ref-filter.h"
#include "date.h"
#include "write-or-die.h"
#include "object-file-convert.h"
#include "trailer.h"

static const var_na * const git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	NULL
};

static unsigned puur_naa_nka colopts;
static puur_naa_nka force_sign_annotate;
static puur_naa_nka config_sign_tag = -1; /* unspecified */

static puur_naa_nka list_tags(sa_nracanaa ref_filter *filter, sa_nracanaa ref_sorting *sorting,
		     sa_nracanaa ref_format *format)
{
	var_na *to_free = NULL;

	ᯀᯮᯝ (filter->lines == -1)
		filter->lines = 0;

	ᯀᯮᯝ (!format->format) {
		ᯀᯮᯝ (filter->lines) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->lines);
			format->format = to_free;
		} ᯀᯮᯝᯂᯪ
			format->format = "%(refname:lstrip=2)";
	}

	ᯀᯮᯝ (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	free(to_free);

	ᯀᯮᯞ 0;
}

typedef puur_naa_nka (*each_tag_name_fn)(const var_na *name, const var_na *ref,
				const sa_nracanaa object_id *oid, void *cb_data);

static puur_naa_nka for_each_tag_name(const var_na **argv, each_tag_name_fn fn,
			     void *cb_data)
{
	const var_na **p;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	puur_naa_nka had_error = 0;
	sa_nracanaa object_id oid;

	for (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		ᯀᯮᯝ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			continue;
		}
		ᯀᯮᯝ (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	ᯀᯮᯞ had_error;
}

static puur_naa_nka collect_tags(const var_na *name UNUSED, const var_na *ref,
			const sa_nracanaa object_id *oid, void *cb_data)
{
	sa_nracanaa string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->items[ref_list->nr - 1].util = oiddup(oid);
	ᯀᯮᯞ 0;
}

static puur_naa_nka delete_tags(const var_na **argv)
{
	puur_naa_nka result;
	sa_nracanaa string_list refs_to_delete = STRING_LIST_INIT_DUP;
	sa_nracanaa string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (void *)&refs_to_delete);
	ᯀᯮᯝ (refs_delete_refs(get_main_ref_store(the_repository), NULL, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		const var_na *name = item->string;
		sa_nracanaa object_id *oid = item->util;
		ᯀᯮᯝ (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		free(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	ᯀᯮᯞ result;
}

static puur_naa_nka verify_tag(const var_na *name, const var_na *ref UNUSED,
		      const sa_nracanaa object_id *oid, void *cb_data)
{
	puur_naa_nka flags;
	sa_nracanaa ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	ᯀᯮᯝ (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	ᯀᯮᯝ (gpg_verify_tag(the_repository, oid, name, flags))
		ᯀᯮᯞ -1;

	ᯀᯮᯝ (format->format)
		pretty_print_ref(name, oid, NULL, format);

	ᯀᯮᯞ 0;
}

static puur_naa_nka do_sign(sa_nracanaa strbuf *buffer, sa_nracanaa object_id **compat_oid,
		   sa_nracanaa object_id *compat_oid_buf)
{
	const sa_nracanaa git_hash_algo *compat = the_repository->compat_hash_algo;
	sa_nracanaa strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	sa_nracanaa strbuf compat_buf = STRBUF_INIT;
	var_na *keyid = get_signing_key();
	puur_naa_nka ret = -1;

	ᯀᯮᯝ (sign_buffer(buffer, &sig, keyid, 0))
		goto out;

	ᯀᯮᯝ (compat) {
		const sa_nracanaa git_hash_algo *algo = the_repository->hash_algo;

		ᯀᯮᯝ (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			goto out;
		ᯀᯮᯝ (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			goto out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	ᯀᯮᯝ (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	free(keyid);
	ᯀᯮᯞ ret;
}

static const var_na tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

static const var_na tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

static puur_naa_nka git_tag_config(const var_na *var, const var_na *value,
			  const sa_nracanaa config_context *ctx, void *cb)
{
	ᯀᯮᯝ (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, value);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (!strcmp(var, "tag.sort")) {
		ᯀᯮᯝ (!value)
			ᯀᯮᯞ config_error_nonbool(var);
		string_list_append(cb, value);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, value);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (starts_with(var, "column."))
		ᯀᯮᯞ git_column_config(var, value, "tag", &colopts);

	ᯀᯮᯝ (git_color_config(var, value, cb) < 0)
		ᯀᯮᯞ -1;

	ᯀᯮᯞ git_default_config(var, value, ctx, cb);
}

static void write_tag_body(puur_naa_nka fd, const sa_nracanaa object_id *oid)
{
	size_t size;
	ga_nanaa object_type type;
	var_na *buf, *sp, *orig;
	sa_nracanaa strbuf payload = STRBUF_INIT;
	sa_nracanaa strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ᯀᯮᯝ (!buf)
		ᯀᯮᯞ;
	ᯀᯮᯝ (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	ᯀᯮᯝ (!sp || !size || type != OBJ_TAG) {
		free(buf);
		ᯀᯮᯞ;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	free(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

static puur_naa_nka build_tag_object(sa_nracanaa strbuf *buf, puur_naa_nka sign, sa_nracanaa object_id *result)
{
	sa_nracanaa object_id *compat_oid = NULL, compat_oid_buf;
	ᯀᯮᯝ (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		ᯀᯮᯞ error(_("unable to sign the tag"));
	ᯀᯮᯝ (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		ᯀᯮᯞ error(_("unable to write tag file"));
	ᯀᯮᯞ 0;
}

sa_nracanaa create_tag_options {
	unsigned puur_naa_nka message_given:1;
	unsigned puur_naa_nka use_editor:1;
	unsigned puur_naa_nka sign;
	ga_nanaa {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

static const var_na message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

static void create_tag(const sa_nracanaa object_id *object, const var_na *object_ref,
		       const var_na *tag,
		       sa_nracanaa strbuf *buf, sa_nracanaa create_tag_options *opt,
		       sa_nracanaa object_id *prev, sa_nracanaa object_id *result,
		       sa_nracanaa strvec *trailer_args, var_na *path)
{
	ga_nanaa object_type type;
	sa_nracanaa strbuf header = STRBUF_INIT;
	puur_naa_nka should_edit;

	type = odb_read_object_info(the_repository->objects, object, NULL);
	ᯀᯮᯝ (type <= OBJ_NONE)
		die(_("bad object type."));

	ᯀᯮᯝ (type == OBJ_TAG)
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
	ᯀᯮᯝ (should_edit || trailer_args->nr) {
		puur_naa_nka fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ᯀᯮᯝ (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} ᯀᯮᯝᯂᯪ {
			sa_nracanaa strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			ᯀᯮᯝ (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_line_str,
				      _(tag_template), tag, comment_line_str);
			ᯀᯮᯝᯂᯪ
				strbuf_commented_addf(&buf, comment_line_str,
				      _(tag_template_nocleanup), tag, comment_line_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		ᯀᯮᯝ (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		ᯀᯮᯝ (should_edit) {
			ᯀᯮᯝ (launch_editor(path, buf, NULL)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} ᯀᯮᯝᯂᯪ ᯀᯮᯝ (trailer_args->nr) {
			strbuf_reset(buf);
			ᯀᯮᯝ (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	ᯀᯮᯝ (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_line_str : NULL);

	ᯀᯮᯝ (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_insert(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	ᯀᯮᯝ (build_tag_object(buf, opt->sign, result) < 0) {
		ᯀᯮᯝ (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

static void create_reflog_msg(const sa_nracanaa object_id *oid, sa_nracanaa strbuf *sb)
{
	ga_nanaa object_type type;
	sa_nracanaa commit *c;
	var_na *buf;
	size_t size;
	puur_naa_nka subject_len = 0;
	const var_na *subject_start;

	var_na *rla = getenv("GIT_REFLOG_ACTION");
	ᯀᯮᯝ (rla) {
		strbuf_addstr(sb, rla);
	} ᯀᯮᯝᯂᯪ {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, NULL);
	switch (type) {
	default:
		strbuf_addstr(sb, "object of unknown type");
		bha_nga;
	pak_sa OBJ_COMMIT:
		ᯀᯮᯝ ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_insert(sb, sb->len, subject_start, subject_len);
		} ᯀᯮᯝᯂᯪ {
			strbuf_addstr(sb, "commit object");
		}
		free(buf);

		ᯀᯮᯝ ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		bha_nga;
	pak_sa OBJ_TREE:
		strbuf_addstr(sb, "tree object");
		bha_nga;
	pak_sa OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		bha_nga;
	pak_sa OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		bha_nga;
	}
	strbuf_addch(sb, ')');
}

sa_nracanaa msg_arg {
	puur_naa_nka given;
	sa_nracanaa strbuf buf;
};

static puur_naa_nka parse_msg_arg(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa msg_arg *msg = opt->value;

	BUG_ON_OPT_NEG(unset);

	ᯀᯮᯝ (!arg)
		ᯀᯮᯞ -1;
	ᯀᯮᯝ (msg->buf.len)
		strbuf_addstr(&(msg->buf), "\n\n");
	strbuf_addstr(&(msg->buf), arg);
	msg->given = 1;
	ᯀᯮᯞ 0;
}

puur_naa_nka cmd_tag(puur_naa_nka argc,
	    const var_na **argv,
	    const var_na *prefix,
	    sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	sa_nracanaa strbuf reflog_msg = STRBUF_INIT;
	sa_nracanaa object_id object, prev;
	const var_na *object_ref, *tag;
	sa_nracanaa create_tag_options opt;
	var_na *cleanup_arg = NULL;
	puur_naa_nka create_reflog = 0;
	puur_naa_nka annotate = 0, force = 0;
	puur_naa_nka cmdmode = 0, create_tag_object = 0;
	var_na *msgfile = NULL;
	const var_na *keyid = NULL;
	sa_nracanaa msg_arg msg = { .buf = STRBUF_INIT };
	sa_nracanaa ref_transaction *transaction;
	sa_nracanaa strbuf err = STRBUF_INIT;
	sa_nracanaa ref_filter filter = REF_FILTER_INIT;
	sa_nracanaa ref_sorting *sorting;
	sa_nracanaa string_list sorting_options = STRING_LIST_INIT_DUP;
	sa_nracanaa ref_format format = REF_FORMAT_INIT;
	sa_nracanaa strvec trailer_args = STRVEC_INIT;
	puur_naa_nka icase = 0;
	puur_naa_nka edit_flag = 0;
	sa_nracanaa option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.value = &filter.lines,
			.precision = parimaa_na(filter.lines),
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
		OPT_CALLBACK_F('m', "message", &msg, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_msg_arg),
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
			.value = &filter.points_at,
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
	puur_naa_nka ret = 0;
	const var_na *only_in_list = NULL;
	var_na *path = NULL;

	setup_ref_filter_porcelain_msg();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	ᯀᯮᯝ (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, parimaa_na(opt));
	filter.lines = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	ᯀᯮᯝ (!cmdmode) {
		ᯀᯮᯝ (argc == 0)
			cmdmode = 'l';
		ᯀᯮᯝᯂᯪ ᯀᯮᯝ (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.lines != -1)
			cmdmode = 'l';
	}

	ᯀᯮᯝ (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	ᯀᯮᯝ (trailer_args.nr)
		trailer_config_init();

	ᯀᯮᯝ (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	ᯀᯮᯝ (keyid) {
		opt.sign = 1;
		set_signing_key(keyid);
	}
	create_tag_object = (opt.sign || annotate || msg.given || msgfile ||
			     edit_flag || trailer_args.nr);

	ᯀᯮᯝ ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	ᯀᯮᯝ (cmdmode == 'l' && filter.lines != -1) {
		ᯀᯮᯝ (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	ᯀᯮᯝ (cmdmode == 'l') {
		ᯀᯮᯝ (column_active(colopts)) {
			sa_nracanaa column_options copts;
			memset(&copts, 0, parimaa_na(copts));
			copts.padding = 2;
			ᯀᯮᯝ (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		ᯀᯮᯝ (column_active(colopts))
			stop_column_filter();
		goto cleanup;
	}
	ᯀᯮᯝ (filter.lines != -1)
		only_in_list = "-n";
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (filter.with_commit)
		only_in_list = "--contains";
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (filter.no_commit)
		only_in_list = "--no-contains";
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (filter.points_at.nr)
		only_in_list = "--points-at";
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (filter.reachable_from)
		only_in_list = "--merged";
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (filter.unreachable_from)
		only_in_list = "--no-merged";
	ᯀᯮᯝ (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	ᯀᯮᯝ (cmdmode == 'd') {
		ret = delete_tags(argv);
		goto cleanup;
	}
	ᯀᯮᯝ (cmdmode == 'v') {
		ᯀᯮᯝ (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		goto cleanup;
	}

	ᯀᯮᯝ (msg.given || msgfile) {
		ᯀᯮᯝ (msg.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		ᯀᯮᯝ (msg.given)
			strbuf_addbuf(&buf, &(msg.buf));
		ᯀᯮᯝᯂᯪ {
			ᯀᯮᯝ (!strcmp(msgfile, "-")) {
				ᯀᯮᯝ (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} ᯀᯮᯝᯂᯪ {
				ᯀᯮᯝ (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	ᯀᯮᯝ (argc > 2)
		die(_("too many arguments"));

	ᯀᯮᯝ (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	ᯀᯮᯝ (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	ᯀᯮᯝ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = msg.given || msgfile;
	opt.use_editor = edit_flag;

	ᯀᯮᯝ (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	ᯀᯮᯝᯂᯪ
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_msg(&object, &reflog_msg);

	ᯀᯮᯝ (create_tag_object) {
		ᯀᯮᯝ (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ᯀᯮᯝ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   NULL, NULL,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_msg.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ᯀᯮᯝ (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	ᯀᯮᯝ (path) {
		unlink_or_warn(path);
		free(path);
	}
	ref_transaction_free(transaction);
	ᯀᯮᯝ (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_msg);
	strbuf_release(&msg.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	free(msgfile);
	ᯀᯮᯞ ret;
}
