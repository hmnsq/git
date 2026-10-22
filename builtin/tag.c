/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#define USE_THE_REPOSITORY_VARIABLE
#define DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "advice.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "diff.h"
#ಅನ್ತರ್ಭಾವ "revision.h"
#ಅನ್ತರ್ಭಾವ "gpg-interface.h"
#ಅನ್ತರ್ಭಾವ "oid-array.h"
#ಅನ್ತರ್ಭಾವ "column.h"
#ಅನ್ತರ್ಭಾವ "ref-filter.h"
#ಅನ್ತರ್ಭಾವ "date.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"
#ಅನ್ತರ್ಭಾವ "object-file-convert.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_tag_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ರಿಕ್ತ
};

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ colopts;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ force_sign_annotate;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ config_sign_tag = -1; /* unspecified */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_tags(ಸಂರಚನಾ ref_filter *filter, ಸಂರಚನಾ ref_sorting *sorting,
		     ಸಂರಚನಾ ref_format *format)
{
	ವರ್ಣ *to_free = ರಿಕ್ತ;

	ಯದಿ (filter->ಸಾಲುಗಳು == -1)
		filter->ಸಾಲುಗಳು = 0;

	ಯದಿ (!format->format) {
		ಯದಿ (filter->ಸಾಲುಗಳು) {
			to_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->ಸಾಲುಗಳು);
			format->format = to_free;
		} ಅನ್ಯಥಾ
			format->format = "%(refname:lstrip=2)";
	}

	ಯದಿ (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_tag_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_TAGS, sorting, format);

	ಮುಕ್ತ(to_free);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*each_tag_name_fn)(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *ref,
				ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಶೂನ್ಯ *cb_data);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ for_each_tag_name(ಧ್ರುವ ವರ್ಣ **argv, each_tag_name_fn fn,
			     ಶೂನ್ಯ *cb_data)
{
	ಧ್ರುವ ವರ್ಣ **p;
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ had_error = 0;
	ಸಂರಚನಾ object_id oid;

	ಪ್ರತಿ (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (fn(*p, ref.buf, &oid, cb_data))
			had_error = 1;
	}
	strbuf_release(&ref);
	ಪ್ರತ್ಯಾಗಮ had_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ collect_tags(ಧ್ರುವ ವರ್ಣ *name UNUSED, ಧ್ರುವ ವರ್ಣ *ref,
			ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ string_list *ref_list = cb_data;

	string_list_append(ref_list, ref);
	ref_list->ವಸ್ತುಗಳು[ref_list->nr - 1].util = oiddup(oid);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_tags(ಧ್ರುವ ವರ್ಣ **argv)
{
	ಪೂರ್ಣಾಙ್ಕ result;
	ಸಂರಚನಾ string_list refs_to_delete = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ string_list_item *item;

	result = for_each_tag_name(argv, collect_tags, (ಶೂನ್ಯ *)&refs_to_delete);
	ಯದಿ (refs_delete_refs(get_main_ref_store(the_repository), ರಿಕ್ತ, &refs_to_delete, REF_NO_DEREF))
		result = 1;

	for_each_string_list_item(item, &refs_to_delete) {
		ಧ್ರುವ ವರ್ಣ *name = item->string;
		ಸಂರಚನಾ object_id *oid = item->util;
		ಯದಿ (!refs_ref_exists(get_main_ref_store(the_repository), name))
			printf(_("Deleted tag '%s' (was %s)\n"),
				item->string + 10,
				repo_find_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		ಮುಕ್ತ(oid);
	}
	string_list_clear(&refs_to_delete, 0);
	ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ verify_tag(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *ref UNUSED,
		      ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಶೂನ್ಯ *cb_data)
{
	ಪೂರ್ಣಾಙ್ಕ flags;
	ಸಂರಚನಾ ref_format *format = cb_data;
	flags = GPG_VERIFY_VERBOSE;

	ಯದಿ (format->format)
		flags = GPG_VERIFY_OMIT_STATUS;

	ಯದಿ (gpg_verify_tag(the_repository, oid, name, flags))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (format->format)
		pretty_print_ref(name, oid, ರಿಕ್ತ, format);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_sign(ಸಂರಚನಾ strbuf *buffer, ಸಂರಚನಾ object_id **compat_oid,
		   ಸಂರಚನಾ object_id *compat_oid_buf)
{
	ಧ್ರುವ ಸಂರಚನಾ git_hash_algo *compat = the_repository->compat_hash_algo;
	ಸಂರಚನಾ strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ಸಂರಚನಾ strbuf compat_buf = STRBUF_INIT;
	ವರ್ಣ *keyid = get_signing_ಕೀಲಿ();
	ಪೂರ್ಣಾಙ್ಕ ret = -1;

	ಯದಿ (sign_buffer(buffer, &sig, keyid, 0))
		ಗಚ್ಛ out;

	ಯದಿ (compat) {
		ಧ್ರುವ ಸಂರಚನಾ git_hash_algo *algo = the_repository->hash_algo;

		ಯದಿ (convert_object_file(the_repository ,&compat_buf, algo, compat,
					buffer->buf, buffer->len, OBJ_TAG, 1))
			ಗಚ್ಛ out;
		ಯದಿ (sign_buffer(&compat_buf, &compat_sig, keyid, 0))
			ಗಚ್ಛ out;
		add_header_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_TAG, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	ಯದಿ (compat_sig.len)
		add_header_signature(buffer, &compat_sig, compat);

	strbuf_addbuf(buffer, &sig);
	ret = 0;
out:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	ಮುಕ್ತ(keyid);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ tag_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ tag_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_tag_config(ಧ್ರುವ ವರ್ಣ *var, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ,
			  ಧ್ರುವ ಸಂರಚನಾ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಯದಿ (!strcmp(var, "tag.gpgsign")) {
		config_sign_tag = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "tag.sort")) {
		ಯದಿ (!ಮೌಲ್ಯ)
			ಪ್ರತ್ಯಾಗಮ config_error_nonbool(var);
		string_list_append(cb, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, ಮೌಲ್ಯ);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (starts_with(var, "column."))
		ಪ್ರತ್ಯಾಗಮ git_column_config(var, ಮೌಲ್ಯ, "tag", &colopts);

	ಯದಿ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಪ್ರತ್ಯಾಗಮ -1;

	ಪ್ರತ್ಯಾಗಮ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_tag_body(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	size_t size;
	ಗಣನಾ object_type type;
	ವರ್ಣ *buf, *sp, *orig;
	ಸಂರಚನಾ strbuf payload = STRBUF_INIT;
	ಸಂರಚನಾ strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (parse_signature(buf, size, &payload, &signature)) {
		buf = payload.buf;
		size = payload.len;
	}
	/* skip header */
	sp = strstr(buf, "\n\n");

	ಯದಿ (!sp || !size || type != OBJ_TAG) {
		ಮುಕ್ತ(buf);
		ಪ್ರತ್ಯಾಗಮ;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + size - sp);

	ಮುಕ್ತ(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ build_tag_object(ಸಂರಚನಾ strbuf *buf, ಪೂರ್ಣಾಙ್ಕ sign, ಸಂರಚನಾ object_id *result)
{
	ಸಂರಚನಾ object_id *compat_oid = ರಿಕ್ತ, compat_oid_buf;
	ಯದಿ (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to sign the tag"));
	ಯದಿ (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_TAG, result, compat_oid, 0) < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to write tag file"));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ create_tag_options {
	unsigned ಪೂರ್ಣಾಙ್ಕ message_given:1;
	unsigned ಪೂರ್ಣಾಙ್ಕ use_editor:1;
	unsigned ಪೂರ್ಣಾಙ್ಕ sign;
	ಗಣನಾ {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_mode;
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ message_advice_nested_tag[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ಸ್ಥಿರ ಶೂನ್ಯ create_tag(ಧ್ರುವ ಸಂರಚನಾ object_id *object, ಧ್ರುವ ವರ್ಣ *object_ref,
		       ಧ್ರುವ ವರ್ಣ *tag,
		       ಸಂರಚನಾ strbuf *buf, ಸಂರಚನಾ create_tag_options *opt,
		       ಸಂರಚನಾ object_id *prev, ಸಂರಚನಾ object_id *result,
		       ಸಂರಚನಾ strvec *trailer_args, ವರ್ಣ *path)
{
	ಗಣನಾ object_type type;
	ಸಂರಚನಾ strbuf header = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ should_edit;

	type = odb_read_object_info(the_repository->objects, object, ರಿಕ್ತ);
	ಯದಿ (type <= OBJ_NONE)
		die(_("bad object type."));

	ಯದಿ (type == OBJ_TAG)
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
	ಯದಿ (should_edit || trailer_args->nr) {
		ಪೂರ್ಣಾಙ್ಕ fd;

		/* write the template message before editing: */
		fd = xopen(path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ಯದಿ (opt->message_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} ಅನ್ಯಥಾ ಯದಿ (!is_null_oid(prev)) {
			write_tag_body(fd, prev);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			ಯದಿ (opt->cleanup_mode == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_ಸಾಲು_str,
				      _(tag_template), tag, comment_ಸಾಲು_str);
			ಅನ್ಯಥಾ
				strbuf_commented_addf(&buf, comment_ಸಾಲು_str,
				      _(tag_template_nocleanup), tag, comment_ಸಾಲು_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		ಯದಿ (trailer_args->nr && amend_file_with_trailers(path, trailer_args))
			die(_("unable to pass trailers to --trailers"));

		ಯದಿ (should_edit) {
			ಯದಿ (launch_editor(path, buf, ರಿಕ್ತ)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				exit(1);
			}
		} ಅನ್ಯಥಾ ಯದಿ (trailer_args->nr) {
			strbuf_reset(buf);
			ಯದಿ (strbuf_read_file(buf, path, 0) < 0)
				die_errno(_("failed to read '%s'"), path);
		}
	}

	ಯದಿ (opt->cleanup_mode != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_mode == CLEANUP_ALL ? comment_ಸಾಲು_str : ರಿಕ್ತ);

	ಯದಿ (!opt->message_given && !buf->len)
		die(_("no tag message?"));

	strbuf_ಸೇರಿಸು(buf, 0, header.buf, header.len);
	strbuf_release(&header);

	ಯದಿ (build_tag_object(buf, opt->sign, result) < 0) {
		ಯದಿ (path)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				path);
		exit(128);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ create_reflog_ಸಂದೇಶ(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಸಂರಚನಾ strbuf *sb)
{
	ಗಣನಾ object_type type;
	ಸಂರಚನಾ commit *c;
	ವರ್ಣ *buf;
	size_t size;
	ಪೂರ್ಣಾಙ್ಕ subject_len = 0;
	ಧ್ರುವ ವರ್ಣ *subject_start;

	ವರ್ಣ *rla = getenv("GIT_REFLOG_ACTION");
	ಯದಿ (rla) {
		strbuf_addstr(sb, rla);
	} ಅನ್ಯಥಾ {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_add_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ರಿಕ್ತ);
	ವಿಕಲ್ಪ (type) {
	ಸ್ವಭಾವ:
		strbuf_addstr(sb, "object of unknown type");
		ಭಙ್ಗ;
	ಪಕ್ಷ OBJ_COMMIT:
		ಯದಿ ((buf = odb_read_object(the_repository->objects, oid, &type, &size))) {
			subject_len = find_commit_subject(buf, &subject_start);
			strbuf_ಸೇರಿಸು(sb, sb->len, subject_start, subject_len);
		} ಅನ್ಯಥಾ {
			strbuf_addstr(sb, "commit object");
		}
		ಮುಕ್ತ(buf);

		ಯದಿ ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_MODE(SHORT)));
		ಭಙ್ಗ;
	ಪಕ್ಷ OBJ_ವೃಕ್ಷ:
		strbuf_addstr(sb, "tree object");
		ಭಙ್ಗ;
	ಪಕ್ಷ OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		ಭಙ್ಗ;
	ಪಕ್ಷ OBJ_TAG:
		strbuf_addstr(sb, "other tag object");
		ಭಙ್ಗ;
	}
	strbuf_addch(sb, ')');
}

ಸಂರಚನಾ ಸಂದೇಶ_arg {
	ಪೂರ್ಣಾಙ್ಕ given;
	ಸಂರಚನಾ strbuf buf;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_ಸಂದೇಶ_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ ಸಂದೇಶ_arg *ಸಂದೇಶ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!arg)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (ಸಂದೇಶ->buf.len)
		strbuf_addstr(&(ಸಂದೇಶ->buf), "\n\n");
	strbuf_addstr(&(ಸಂದೇಶ->buf), arg);
	ಸಂದೇಶ->given = 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_tag(ಪೂರ್ಣಾಙ್ಕ argc,
	    ಧ್ರುವ ವರ್ಣ **argv,
	    ಧ್ರುವ ವರ್ಣ *prefix,
	    ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;
	ಸಂರಚನಾ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ object_id object, prev;
	ಧ್ರುವ ವರ್ಣ *object_ref, *tag;
	ಸಂರಚನಾ create_tag_options opt;
	ವರ್ಣ *cleanup_arg = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ create_reflog = 0;
	ಪೂರ್ಣಾಙ್ಕ annotate = 0, force = 0;
	ಪೂರ್ಣಾಙ್ಕ cmdmode = 0, create_tag_object = 0;
	ವರ್ಣ *msgfile = ರಿಕ್ತ;
	ಧ್ರುವ ವರ್ಣ *keyid = ರಿಕ್ತ;
	ಸಂರಚನಾ ಸಂದೇಶ_arg ಸಂದೇಶ = { .buf = STRBUF_INIT };
	ಸಂರಚನಾ ref_transaction *transaction;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಸಂರಚನಾ ref_filter filter = REF_FILTER_INIT;
	ಸಂರಚನಾ ref_sorting *sorting;
	ಸಂರಚನಾ string_list sorting_options = STRING_LIST_INIT_DUP;
	ಸಂರಚನಾ ref_format format = REF_FORMAT_INIT;
	ಸಂರಚನಾ strvec trailer_args = STRVEC_INIT;
	ಪೂರ್ಣಾಙ್ಕ icase = 0;
	ಪೂರ್ಣಾಙ್ಕ edit_flag = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.ಮೌಲ್ಯ = &filter.ಸಾಲುಗಳು,
			.precision = ಪರಿಮಾಣ(filter.ಸಾಲುಗಳು),
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
		OPT_CALLBACK_F('m', "message", &ಸಂದೇಶ, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_ಸಂದೇಶ_arg),
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
			.ಮೌಲ್ಯ = &filter.points_at,
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
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಧ್ರುವ ವರ್ಣ *only_in_list = ರಿಕ್ತ;
	ವರ್ಣ *path = ರಿಕ್ತ;

	setup_ref_filter_porcelain_ಸಂದೇಶ();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_tag_config, &sorting_options);
	ಯದಿ (!sorting_options.nr)
		string_list_append(&sorting_options, "refname");

	memset(&opt, 0, ಪರಿಮಾಣ(opt));
	filter.ಸಾಲುಗಳು = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, prefix, options, git_tag_usage, 0);

	ಯದಿ (!cmdmode) {
		ಯದಿ (argc == 0)
			cmdmode = 'l';
		ಅನ್ಯಥಾ ಯದಿ (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.ಸಾಲುಗಳು != -1)
			cmdmode = 'l';
	}

	ಯದಿ (cmdmode == 'l')
		setup_auto_pager("tag", 1);

	ಯದಿ (trailer_args.nr)
		trailer_config_init();

	ಯದಿ (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_tag > 0;

	ಯದಿ (keyid) {
		opt.sign = 1;
		set_signing_ಕೀಲಿ(keyid);
	}
	create_tag_object = (opt.sign || annotate || ಸಂದೇಶ.given || msgfile ||
			     edit_flag || trailer_args.nr);

	ಯದಿ ((create_tag_object || force) && (cmdmode != 0))
		usage_with_options(git_tag_usage, options);

	finalize_colopts(&colopts, -1);
	ಯದಿ (cmdmode == 'l' && filter.ಸಾಲುಗಳು != -1) {
		ಯದಿ (explicitly_enable_column(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	sorting = ref_sorting_options(&sorting_options);
	ref_sorting_set_sort_flags_all(sorting, REF_SORTING_ICASE, icase);
	filter.ignore_case = icase;
	ಯದಿ (cmdmode == 'l') {
		ಯದಿ (column_active(colopts)) {
			ಸಂರಚನಾ column_options copts;
			memset(&copts, 0, ಪರಿಮಾಣ(copts));
			copts.padding = 2;
			ಯದಿ (run_column_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_tags(&filter, sorting, &format);
		ಯದಿ (column_active(colopts))
			stop_column_filter();
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (filter.ಸಾಲುಗಳು != -1)
		only_in_list = "-n";
	ಅನ್ಯಥಾ ಯದಿ (filter.with_commit)
		only_in_list = "--contains";
	ಅನ್ಯಥಾ ಯದಿ (filter.no_commit)
		only_in_list = "--no-contains";
	ಅನ್ಯಥಾ ಯದಿ (filter.points_at.nr)
		only_in_list = "--points-at";
	ಅನ್ಯಥಾ ಯದಿ (filter.reachable_from)
		only_in_list = "--merged";
	ಅನ್ಯಥಾ ಯದಿ (filter.unreachable_from)
		only_in_list = "--no-merged";
	ಯದಿ (only_in_list)
		die(_("the '%s' option is only allowed in list mode"), only_in_list);
	ಯದಿ (cmdmode == 'd') {
		ret = delete_tags(argv);
		ಗಚ್ಛ cleanup;
	}
	ಯದಿ (cmdmode == 'v') {
		ಯದಿ (format.format && verify_ref_format(&format))
			usage_with_options(git_tag_usage, options);
		ret = for_each_tag_name(argv, verify_tag, &format);
		ಗಚ್ಛ cleanup;
	}

	ಯದಿ (ಸಂದೇಶ.given || msgfile) {
		ಯದಿ (ಸಂದೇಶ.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		ಯದಿ (ಸಂದೇಶ.given)
			strbuf_addbuf(&buf, &(ಸಂದೇಶ.buf));
		ಅನ್ಯಥಾ {
			ಯದಿ (!strcmp(msgfile, "-")) {
				ಯದಿ (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} ಅನ್ಯಥಾ {
				ಯದಿ (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	tag = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	ಯದಿ (argc > 2)
		die(_("too many arguments"));

	ಯದಿ (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	ಯದಿ (check_tag_ref(&ref, tag))
		die(_("'%s' is not a valid tag name."), tag);

	ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	ಅನ್ಯಥಾ ಯದಿ (!force)
		die(_("tag '%s' already exists"), tag);

	opt.message_given = ಸಂದೇಶ.given || msgfile;
	opt.use_editor = edit_flag;

	ಯದಿ (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_mode = CLEANUP_ALL;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_mode = CLEANUP_NONE;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_mode = CLEANUP_SPACE;
	ಅನ್ಯಥಾ
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_ಸಂದೇಶ(&object, &reflog_ಸಂದೇಶ);

	ಯದಿ (create_tag_object) {
		ಯದಿ (force_sign_annotate && !annotate)
			opt.sign = 1;
		path = repo_git_path(the_repository, "TAG_EDITMSG");
		create_tag(&object, object_ref, tag, &buf, &opt, &prev, &object,
			   &trailer_args, path);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಯದಿ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ರಿಕ್ತ, ರಿಕ್ತ,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_ಸಂದೇಶ.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ಯದಿ (path)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				path);
		die("%s", err.buf);
	}
	ಯದಿ (path) {
		unlink_or_warn(path);
		ಮುಕ್ತ(path);
	}
	ref_transaction_free(transaction);
	ಯದಿ (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		printf(_("Updated tag '%s' (was %s)\n"), tag,
		       repo_find_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_sorting_release(sorting);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_ಸಂದೇಶ);
	strbuf_release(&ಸಂದೇಶ.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_args);
	ಮುಕ್ತ(msgfile);
	ಪ್ರತ್ಯಾಗಮ ret;
}
