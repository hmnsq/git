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

static const char * const git_tag_usage[] = {
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

static unsigned целое colopts;
static целое force_sign_annotate;
static целое config_sign_tag = -1; /* unspecified */

static целое list_tags(struct ref_filter *filter, struct ref_sorting *sorting,
		     struct ref_format *format)
{
	char *to_free = NULL;

	если (filter->lines == -1)
		filter->lines = 0;

	если (!format->format) {
		если (filter->lines) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->lines);
			format->format = to_free;
		} иначе
			format->format = "%(refname:lstrip=2)";
	}

	если (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	free(to_free);

	вернуть 0;
}

typedef целое (*each_tag_name_fn)(const char *name, const char *ref,
				const struct object_id *oid, void *cb_data);

static целое for_each_tag_name(const char **argv, each_tag_name_fn fn,
			     void *cb_data)
{
	const char **p;
	struct strbuf ref = STRBUF_INIT;
	целое had_error = 0;
	struct object_id oid;

	для (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		если (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			continue;
		}
		если (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	вернуть had_error;
}

static целое collect_tags(const char *name UNUSED, const char *ref,
			const struct object_id *oid, void *cb_data)
{
	struct string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->items[ref_list->nr - 1].util = oiddup(oid);
	вернуть 0;
}

static целое delete_tags(const char **argv)
{
	целое result;
	struct string_list refs_to_delete = STRING_LIST_INIT_DUP;
	struct string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (void *)&refs_to_delete);
	если (refs_delete_refs(get_main_ref_store(the_repository), NULL, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		const char *name = item->string;
		struct object_id *oid = item->util;
		если (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		free(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	вернуть result;
}

static целое verify_tag(const char *name, const char *ref UNUSED,
		      const struct object_id *oid, void *cb_data)
{
	целое flags;
	struct ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	если (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	если (gpg_verify_tag(the_repository, oid, name, flags))
		вернуть -1;

	если (format->format)
		pretty_print_ref(name, oid, NULL, format);

	вернуть 0;
}

static целое do_sign(struct strbuf *buffer, struct object_id **compat_oid,
		   struct object_id *compat_oid_buf)
{
	const struct git_hash_algo *compat = the_repository->compat_hash_algo;
	struct strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	struct strbuf compat_buf = STRBUF_INIT;
	char *keyid = get_signing_key();
	целое ret = -1;

	если (sign_buffer(buffer, &sig, keyid, 0))
		goto out;

	если (compat) {
		const struct git_hash_algo *algo = the_repository->hash_algo;

		если (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			goto out;
		если (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			goto out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	если (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	free(keyid);
	вернуть ret;
}

static const char tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

static const char tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

static целое git_tag_config(const char *var, const char *value,
			  const struct config_context *ctx, void *cb)
{
	если (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, value);
		вернуть 0;
	}

	если (!strcmp(var, "tag.sort")) {
		если (!value)
			вернуть config_error_nonbool(var);
		string_list_append(cb, value);
		вернуть 0;
	}

	если (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, value);
		вернуть 0;
	}

	если (starts_with(var, "column."))
		вернуть git_column_config(var, value, "tag", &colopts);

	если (git_color_config(var, value, cb) < 0)
		вернуть -1;

	вернуть git_default_config(var, value, ctx, cb);
}

static void write_tag_body(целое fd, const struct object_id *oid)
{
	size_t size;
	enum object_type type;
	char *buf, *sp, *orig;
	struct strbuf payload = STRBUF_INIT;
	struct strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	если (!buf)
		вернуть;
	если (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	если (!sp || !size || type != OBJ_TAG) {
		free(buf);
		вернуть;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	free(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

static целое build_tag_object(struct strbuf *buf, целое sign, struct object_id *result)
{
	struct object_id *compat_oid = NULL, compat_oid_buf;
	если (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		вернуть error(_("unable to sign the tag"));
	если (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		вернуть error(_("unable to write tag file"));
	вернуть 0;
}

struct create_tag_options {
	unsigned целое message_given:1;
	unsigned целое use_editor:1;
	unsigned целое sign;
	enum {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

static const char message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

static void create_tag(const struct object_id *object, const char *object_ref,
		       const char *tag,
		       struct strbuf *buf, struct create_tag_options *opt,
		       struct object_id *prev, struct object_id *result,
		       struct strvec *trailer_args, char *path)
{
	enum object_type type;
	struct strbuf header = STRBUF_INIT;
	целое should_edit;

	type = odb_read_object_info(the_repository->objects, object, NULL);
	если (type <= OBJ_NONE)
		die(_("bad object type."));

	если (type == OBJ_TAG)
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
	если (should_edit || trailer_args->nr) {
		целое fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		если (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} иначе если (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} иначе {
			struct strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			если (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_line_str,
				      _(tag_template), tag, comment_line_str);
			иначе
				strbuf_commented_addf(&buf, comment_line_str,
				      _(tag_template_nocleanup), tag, comment_line_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		если (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		если (should_edit) {
			если (launch_editor(path, buf, NULL)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} иначе если (trailer_args->nr) {
			strbuf_reset(buf);
			если (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	если (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_line_str : NULL);

	если (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_insert(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	если (build_tag_object(buf, opt->sign, result) < 0) {
		если (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

static void create_reflog_msg(const struct object_id *oid, struct strbuf *sb)
{
	enum object_type type;
	struct commit *c;
	char *buf;
	size_t size;
	целое subject_len = 0;
	const char *subject_start;

	char *rla = getenv("GIT_REFLOG_ACTION");
	если (rla) {
		strbuf_addstr(sb, rla);
	} иначе {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, NULL);
	switch (type) {
	default:
		strbuf_addstr(sb, "object of unknown type");
		break;
	case OBJ_COMMIT:
		если ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_insert(sb, sb->len, subject_start, subject_len);
		} иначе {
			strbuf_addstr(sb, "commit object");
		}
		free(buf);

		если ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		break;
	case OBJ_TREE:
		strbuf_addstr(sb, "tree object");
		break;
	case OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		break;
	case OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		break;
	}
	strbuf_addch(sb, ')');
}

struct msg_arg {
	целое given;
	struct strbuf buf;
};

static целое parse_msg_arg(const struct option *opt, const char *arg, целое unset)
{
	struct msg_arg *msg = opt->value;

	BUG_ON_OPT_NEG(unset);

	если (!arg)
		вернуть -1;
	если (msg->buf.len)
		strbuf_addstr(&(msg->buf), "\n\n");
	strbuf_addstr(&(msg->buf), arg);
	msg->given = 1;
	вернуть 0;
}

целое cmd_tag(целое argc,
	    const char **argv,
	    const char *prefix,
	    struct repository *repo UNUSED)
{
	struct strbuf buf = STRBUF_INIT;
	struct strbuf ref = STRBUF_INIT;
	struct strbuf reflog_msg = STRBUF_INIT;
	struct object_id object, prev;
	const char *object_ref, *tag;
	struct create_tag_options opt;
	char *cleanup_arg = NULL;
	целое create_reflog = 0;
	целое annotate = 0, force = 0;
	целое cmdmode = 0, create_tag_object = 0;
	char *msgfile = NULL;
	const char *keyid = NULL;
	struct msg_arg msg = { .buf = STRBUF_INIT };
	struct ref_transaction *transaction;
	struct strbuf err = STRBUF_INIT;
	struct ref_filter filter = REF_FILTER_INIT;
	struct ref_sorting *sorting;
	struct string_list sorting_options = STRING_LIST_INIT_DUP;
	struct ref_format format = REF_FORMAT_INIT;
	struct strvec trailer_args = STRVEC_INIT;
	целое icase = 0;
	целое edit_flag = 0;
	struct option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.value = &filter.lines,
			.precision = sizeof(filter.lines),
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
	целое ret = 0;
	const char *only_in_list = NULL;
	char *path = NULL;

	setup_ref_filter_porcelain_msg();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	если (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, sizeof(opt));
	filter.lines = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	если (!cmdmode) {
		если (argc == 0)
			cmdmode = 'l';
		иначе если (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.lines != -1)
			cmdmode = 'l';
	}

	если (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	если (trailer_args.nr)
		trailer_config_init();

	если (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	если (keyid) {
		opt.sign = 1;
		set_signing_key(keyid);
	}
	create_tag_object = (opt.sign || annotate || msg.given || msgfile ||
			     edit_flag || trailer_args.nr);

	если ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	если (cmdmode == 'l' && filter.lines != -1) {
		если (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	если (cmdmode == 'l') {
		если (column_active(colopts)) {
			struct column_options copts;
			memset(&copts, 0, sizeof(copts));
			copts.padding = 2;
			если (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		если (column_active(colopts))
			stop_column_filter();
		goto cleanup;
	}
	если (filter.lines != -1)
		only_in_list = "-n";
	иначе если (filter.with_commit)
		only_in_list = "--contains";
	иначе если (filter.no_commit)
		only_in_list = "--no-contains";
	иначе если (filter.points_at.nr)
		only_in_list = "--points-at";
	иначе если (filter.reachable_from)
		only_in_list = "--merged";
	иначе если (filter.unreachable_from)
		only_in_list = "--no-merged";
	если (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	если (cmdmode == 'd') {
		ret = delete_tags(argv);
		goto cleanup;
	}
	если (cmdmode == 'v') {
		если (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		goto cleanup;
	}

	если (msg.given || msgfile) {
		если (msg.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		если (msg.given)
			strbuf_addbuf(&buf, &(msg.buf));
		иначе {
			если (!strcmp(msgfile, "-")) {
				если (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} иначе {
				если (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	если (argc > 2)
		die(_("too many arguments"));

	если (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	если (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	если (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	иначе если (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = msg.given || msgfile;
	opt.use_editor = edit_flag;

	если (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	иначе если (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	иначе если (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	иначе
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_msg(&object, &reflog_msg);

	если (create_tag_object) {
		если (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	если (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   NULL, NULL,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_msg.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		если (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	если (path) {
		unlink_or_warn(path);
		free(path);
	}
	ref_transaction_free(transaction);
	если (force && !is_null_oid(&prev) && !oideq(&prev, &object))
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
	вернуть ret;
}
