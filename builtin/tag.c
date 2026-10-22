/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "advice.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "diff.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "revision.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gpg-interface.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oid-array.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "column.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ref-filter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file-convert.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trailer.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_ਟੈਗ_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ਨਿਰਾ
};

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ colopts;
ਸਥਿਰ ਪੂਰਨ force_sign_annotate;
ਸਥਿਰ ਪੂਰਨ config_sign_ਟੈਗ = -1; /* unspecified */

ਸਥਿਰ ਪੂਰਨ list_ਟੈਗ_ਸੂਚੀ(ਬਣਤਰ ref_filter *filter, ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ,
		     ਬਣਤਰ ref_format *format)
{
	ਅੱਖਰ *ਮੰਜ਼ਿਲ_free = ਨਿਰਾ;

	ਜੇ (filter->ਸਤਰਾਂ == -1)
		filter->ਸਤਰਾਂ = 0;

	ਜੇ (!format->format) {
		ਜੇ (filter->ਸਤਰਾਂ) {
			ਮੰਜ਼ਿਲ_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->ਸਤਰਾਂ);
			format->format = ਮੰਜ਼ਿਲ_free;
		} ਨਹੀਂਤਾਂ
			format->format = "%(refname:lstrip=2)";
	}

	ਜੇ (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_ਟੈਗ_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_ਟੈਗ_ਸੂਚੀ, ਛਾਂਟੀ, format);

	ਮੁਕਤ(ਮੰਜ਼ਿਲ_free);

	ਵਾਪਸ 0;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*each_ਟੈਗ_name_fn)(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ref,
				ਅਚਲ ਬਣਤਰ object_id *oid, ਖਾਲੀ *cb_ਅੰਕੜੇ);

ਸਥਿਰ ਪੂਰਨ for_each_ਟੈਗ_name(ਅਚਲ ਅੱਖਰ **argv, each_ਟੈਗ_name_fn fn,
			     ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਅਚਲ ਅੱਖਰ **p;
	ਬਣਤਰ strbuf ref = STRBUF_INIT;
	ਪੂਰਨ had_error = 0;
	ਬਣਤਰ object_id oid;

	ਲਈ (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		ਜੇ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			ਜਾਰੀ;
		}
		ਜੇ (fn(*p, ref.buf, &oid, cb_ਅੰਕੜੇ))
			had_error = 1;
	}
	strbuf_release(&ref);
	ਵਾਪਸ had_error;
}

ਸਥਿਰ ਪੂਰਨ collect_ਟੈਗ_ਸੂਚੀ(ਅਚਲ ਅੱਖਰ *name ਅਣਵਰਤਿਆ, ਅਚਲ ਅੱਖਰ *ref,
			ਅਚਲ ਬਣਤਰ object_id *oid, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ string_list *ref_list = cb_ਅੰਕੜੇ;

	string_list_append(ref_list, ref);
	ref_list->ਚੀਜ਼ਾਂ[ref_list->nr - 1].util = oiddup(oid);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ delete_ਟੈਗ_ਸੂਚੀ(ਅਚਲ ਅੱਖਰ **argv)
{
	ਪੂਰਨ ਨਤੀਜਾ;
	ਬਣਤਰ string_list refs_ਮੰਜ਼ਿਲ_delete = STRING_LIST_INIT_DUP;
	ਬਣਤਰ string_list_ਵਸਤੂ *ਵਸਤੂ;

	ਨਤੀਜਾ = for_each_ਟੈਗ_name(argv, collect_ਟੈਗ_ਸੂਚੀ, (ਖਾਲੀ *)&refs_ਮੰਜ਼ਿਲ_delete);
	ਜੇ (refs_delete_refs(get_main_ref_store(the_repository), ਨਿਰਾ, &refs_ਮੰਜ਼ਿਲ_delete, REF_NO_DEREF))
		ਨਤੀਜਾ = 1;

	for_each_string_list_ਵਸਤੂ(ਵਸਤੂ, &refs_ਮੰਜ਼ਿਲ_delete) {
		ਅਚਲ ਅੱਖਰ *name = ਵਸਤੂ->string;
		ਬਣਤਰ object_id *oid = ਵਸਤੂ->util;
		ਜੇ (!refs_ref_exists(get_main_ref_store(the_repository), name))
			ਲਿਖੋ(_("Deleted tag '%s' (was %s)\n"),
				ਵਸਤੂ->string + 10,
				repo_ਲੱਭੋ_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		ਮੁਕਤ(oid);
	}
	string_list_clear(&refs_ਮੰਜ਼ਿਲ_delete, 0);
	ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਪੂਰਨ verify_ਟੈਗ(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ref ਅਣਵਰਤਿਆ,
		      ਅਚਲ ਬਣਤਰ object_id *oid, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਪੂਰਨ flags;
	ਬਣਤਰ ref_format *format = cb_ਅੰਕੜੇ;
	flags = GPG_VERIFY_VERBOSE;

	ਜੇ (format->format)
		flags = GPG_VERIFY_OMIT_ਸਥਿਤੀ;

	ਜੇ (gpg_verify_ਟੈਗ(the_repository, oid, name, flags))
		ਵਾਪਸ -1;

	ਜੇ (format->format)
		pretty_print_ref(name, oid, ਨਿਰਾ, format);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ do_sign(ਬਣਤਰ strbuf *ਬਫਰ, ਬਣਤਰ object_id **compat_oid,
		   ਬਣਤਰ object_id *compat_oid_buf)
{
	ਅਚਲ ਬਣਤਰ git_hash_algo *compat = the_repository->compat_hash_algo;
	ਬਣਤਰ strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ਬਣਤਰ strbuf compat_buf = STRBUF_INIT;
	ਅੱਖਰ *keyid = get_signing_ਕੁੰਜੀ();
	ਪੂਰਨ ret = -1;

	ਜੇ (sign_ਬਫਰ(ਬਫਰ, &sig, keyid, 0))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (compat) {
		ਅਚਲ ਬਣਤਰ git_hash_algo *algo = the_repository->hash_algo;

		ਜੇ (convert_object_file(the_repository ,&compat_buf, algo, compat,
					ਬਫਰ->buf, ਬਫਰ->len, OBJ_ਟੈਗ, 1))
			ਜਾਓ ਬਾਹਰਲਾ;
		ਜੇ (sign_ਬਫਰ(&compat_buf, &compat_sig, keyid, 0))
			ਜਾਓ ਬਾਹਰਲਾ;
		ਜੋੜੋ_ਸਿਰਲੇਖ_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_ਟੈਗ, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	ਜੇ (compat_sig.len)
		ਜੋੜੋ_ਸਿਰਲੇਖ_signature(ਬਫਰ, &compat_sig, compat);

	strbuf_addbuf(ਬਫਰ, &sig);
	ret = 0;
ਬਾਹਰਲਾ:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	ਮੁਕਤ(keyid);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਟੈਗ_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਟੈਗ_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ਸਥਿਰ ਪੂਰਨ git_ਟੈਗ_config(ਅਚਲ ਅੱਖਰ *var, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ,
			  ਅਚਲ ਬਣਤਰ config_context *ctx, ਖਾਲੀ *cb)
{
	ਜੇ (!strcmp(var, "tag.gpgsign")) {
		config_sign_ਟੈਗ = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "tag.sort")) {
		ਜੇ (!ਮੁੱਲ)
			ਵਾਪਸ config_error_nonbool(var);
		string_list_append(cb, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, ਮੁੱਲ);
		ਵਾਪਸ 0;
	}

	ਜੇ (starts_with(var, "column."))
		ਵਾਪਸ git_ਕਾਲਮ_config(var, ਮੁੱਲ, "tag", &colopts);

	ਜੇ (git_color_config(var, ਮੁੱਲ, cb) < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ git_default_config(var, ਮੁੱਲ, ctx, cb);
}

ਸਥਿਰ ਖਾਲੀ write_ਟੈਗ_body(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਮਾਪ_t ਮਾਪ;
	ਗਿਣਤੀ object_type type;
	ਅੱਖਰ *buf, *sp, *orig;
	ਬਣਤਰ strbuf payload = STRBUF_INIT;
	ਬਣਤਰ strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &ਮਾਪ);
	ਜੇ (!buf)
		ਵਾਪਸ;
	ਜੇ (parse_signature(buf, ਮਾਪ, &payload, &signature)) {
		buf = payload.buf;
		ਮਾਪ = payload.len;
	}
	/* skip header */
	sp = ਲੜੀ_ਖੋਜ(buf, "\n\n");

	ਜੇ (!sp || !ਮਾਪ || type != OBJ_ਟੈਗ) {
		ਮੁਕਤ(buf);
		ਵਾਪਸ;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + ਮਾਪ - sp);

	ਮੁਕਤ(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ਸਥਿਰ ਪੂਰਨ build_ਟੈਗ_object(ਬਣਤਰ strbuf *buf, ਪੂਰਨ sign, ਬਣਤਰ object_id *ਨਤੀਜਾ)
{
	ਬਣਤਰ object_id *compat_oid = ਨਿਰਾ, compat_oid_buf;
	ਜੇ (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		ਵਾਪਸ error(_("unable to sign the tag"));
	ਜੇ (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_ਟੈਗ, ਨਤੀਜਾ, compat_oid, 0) < 0)
		ਵਾਪਸ error(_("unable to write tag file"));
	ਵਾਪਸ 0;
}

ਬਣਤਰ create_ਟੈਗ_options {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੁਨੇਹਾ_given:1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ use_editor:1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sign;
	ਗਿਣਤੀ {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_ਬਹੁਲਕ;
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਸੁਨੇਹਾ_advice_nested_ਟੈਗ[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ਸਥਿਰ ਖਾਲੀ create_ਟੈਗ(ਅਚਲ ਬਣਤਰ object_id *object, ਅਚਲ ਅੱਖਰ *object_ref,
		       ਅਚਲ ਅੱਖਰ *ਟੈਗ,
		       ਬਣਤਰ strbuf *buf, ਬਣਤਰ create_ਟੈਗ_options *opt,
		       ਬਣਤਰ object_id *prev, ਬਣਤਰ object_id *ਨਤੀਜਾ,
		       ਬਣਤਰ strvec *trailer_ਦਲੀਲਾਂ, ਅੱਖਰ *ਰਸਤਾ)
{
	ਗਿਣਤੀ object_type type;
	ਬਣਤਰ strbuf ਸਿਰਲੇਖ = STRBUF_INIT;
	ਪੂਰਨ should_edit;

	type = odb_read_object_info(the_repository->objects, object, ਨਿਰਾ);
	ਜੇ (type <= OBJ_NONE)
		die(_("bad object type."));

	ਜੇ (type == OBJ_ਟੈਗ)
		advise_if_enabled(ADVICE_NESTED_ਟੈਗ, _(ਸੁਨੇਹਾ_advice_nested_ਟੈਗ),
				  ਟੈਗ, object_ref);

	strbuf_addf(&ਸਿਰਲੇਖ,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_ਮੰਜ਼ਿਲ_hex(object),
		    type_name(type),
		    ਟੈਗ,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->ਸੁਨੇਹਾ_given;
	ਜੇ (should_edit || trailer_ਦਲੀਲਾਂ->nr) {
		ਪੂਰਨ fd;

		/* write the template message before editing: */
		fd = xopen(ਰਸਤਾ, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ਜੇ (opt->ਸੁਨੇਹਾ_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} ਨਹੀਂਤਾਂ ਜੇ (!is_null_oid(prev)) {
			write_ਟੈਗ_body(fd, prev);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			ਜੇ (opt->cleanup_ਬਹੁਲਕ == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_ਸਤਰ_str,
				      _(ਟੈਗ_template), ਟੈਗ, comment_ਸਤਰ_str);
			ਨਹੀਂਤਾਂ
				strbuf_commented_addf(&buf, comment_ਸਤਰ_str,
				      _(ਟੈਗ_template_nocleanup), ਟੈਗ, comment_ਸਤਰ_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		ਜੇ (trailer_ਦਲੀਲਾਂ->nr && amend_file_with_trailers(ਰਸਤਾ, trailer_ਦਲੀਲਾਂ))
			die(_("unable to pass trailers to --trailers"));

		ਜੇ (should_edit) {
			ਜੇ (launch_editor(ਰਸਤਾ, buf, ਨਿਰਾ)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
		} ਨਹੀਂਤਾਂ ਜੇ (trailer_ਦਲੀਲਾਂ->nr) {
			strbuf_reset(buf);
			ਜੇ (strbuf_read_file(buf, ਰਸਤਾ, 0) < 0)
				die_errno(_("failed to read '%s'"), ਰਸਤਾ);
		}
	}

	ਜੇ (opt->cleanup_ਬਹੁਲਕ != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_ਬਹੁਲਕ == CLEANUP_ALL ? comment_ਸਤਰ_str : ਨਿਰਾ);

	ਜੇ (!opt->ਸੁਨੇਹਾ_given && !buf->len)
		die(_("no tag message?"));

	strbuf_ਪਾਓ(buf, 0, ਸਿਰਲੇਖ.buf, ਸਿਰਲੇਖ.len);
	strbuf_release(&ਸਿਰਲੇਖ);

	ਜੇ (build_ਟੈਗ_object(buf, opt->sign, ਨਤੀਜਾ) < 0) {
		ਜੇ (ਰਸਤਾ)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				ਰਸਤਾ);
		ਬਾਹਰ_ਨਿਕਲੋ(128);
	}
}

ਸਥਿਰ ਖਾਲੀ create_reflog_ਸੁਨੇਹਾ(ਅਚਲ ਬਣਤਰ object_id *oid, ਬਣਤਰ strbuf *sb)
{
	ਗਿਣਤੀ object_type type;
	ਬਣਤਰ commit *c;
	ਅੱਖਰ *buf;
	ਮਾਪ_t ਮਾਪ;
	ਪੂਰਨ subject_len = 0;
	ਅਚਲ ਅੱਖਰ *subject_ਸ਼ੁਰੂ;

	ਅੱਖਰ *rla = getenv("GIT_REFLOG_ACTION");
	ਜੇ (rla) {
		strbuf_addstr(sb, rla);
	} ਨਹੀਂਤਾਂ {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_ਜੋੜੋ_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ਨਿਰਾ);
	ਚੋਣ (type) {
	ਮੂਲ:
		strbuf_addstr(sb, "object of unknown type");
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_COMMIT:
		ਜੇ ((buf = odb_read_object(the_repository->objects, oid, &type, &ਮਾਪ))) {
			subject_len = ਲੱਭੋ_commit_subject(buf, &subject_ਸ਼ੁਰੂ);
			strbuf_ਪਾਓ(sb, sb->len, subject_ਸ਼ੁਰੂ, subject_len);
		} ਨਹੀਂਤਾਂ {
			strbuf_addstr(sb, "commit object");
		}
		ਮੁਕਤ(buf);

		ਜੇ ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_ਬਹੁਲਕ(SHORT)));
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_ਰੁੱਖ:
		strbuf_addstr(sb, "tree object");
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		ਰੋਕੋ;
	ਹਾਲਤ OBJ_ਟੈਗ:
		strbuf_addstr(sb, "other tag object");
		ਰੋਕੋ;
	}
	strbuf_addch(sb, ')');
}

ਬਣਤਰ ਸੁਨੇਹਾ_arg {
	ਪੂਰਨ given;
	ਬਣਤਰ strbuf buf;
};

ਸਥਿਰ ਪੂਰਨ parse_ਸੁਨੇਹਾ_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ ਸੁਨੇਹਾ_arg *ਸੁਨੇਹਾ = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!arg)
		ਵਾਪਸ -1;
	ਜੇ (ਸੁਨੇਹਾ->buf.len)
		strbuf_addstr(&(ਸੁਨੇਹਾ->buf), "\n\n");
	strbuf_addstr(&(ਸੁਨੇਹਾ->buf), arg);
	ਸੁਨੇਹਾ->given = 1;
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_ਟੈਗ(ਪੂਰਨ argc,
	    ਅਚਲ ਅੱਖਰ **argv,
	    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	    ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf ref = STRBUF_INIT;
	ਬਣਤਰ strbuf reflog_ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ object_id object, prev;
	ਅਚਲ ਅੱਖਰ *object_ref, *ਟੈਗ;
	ਬਣਤਰ create_ਟੈਗ_options opt;
	ਅੱਖਰ *cleanup_arg = ਨਿਰਾ;
	ਪੂਰਨ create_reflog = 0;
	ਪੂਰਨ annotate = 0, force = 0;
	ਪੂਰਨ cmdmode = 0, create_ਟੈਗ_object = 0;
	ਅੱਖਰ *msgfile = ਨਿਰਾ;
	ਅਚਲ ਅੱਖਰ *keyid = ਨਿਰਾ;
	ਬਣਤਰ ਸੁਨੇਹਾ_arg ਸੁਨੇਹਾ = { .buf = STRBUF_INIT };
	ਬਣਤਰ ref_transaction *transaction;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਬਣਤਰ ref_filter filter = REF_FILTER_INIT;
	ਬਣਤਰ ref_ਛਾਂਟੀ *ਛਾਂਟੀ;
	ਬਣਤਰ string_list ਛਾਂਟੀ_options = STRING_LIST_INIT_DUP;
	ਬਣਤਰ ref_format format = REF_FORMAT_INIT;
	ਬਣਤਰ strvec trailer_ਦਲੀਲਾਂ = STRVEC_INIT;
	ਪੂਰਨ icase = 0;
	ਪੂਰਨ edit_flag = 0;
	ਬਣਤਰ option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.ਮੁੱਲ = &filter.ਸਤਰਾਂ,
			.precision = ਆਕਾਰ(filter.ਸਤਰਾਂ),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_ਸਮੂਹ(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &ਸੁਨੇਹਾ, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_ਸੁਨੇਹਾ_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_ਦਲੀਲਾਂ, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_ਸਮੂਹ(N_("Tag listing options")),
		OPT_ਕਾਲਮ(0, "column", &colopts, N_("show tag list in columns")),
		OPT_ਸ਼ਾਮਲ_ਹੈ(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_ਸ਼ਾਮਲ_ਹੈ(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ਖਾਲੀ_ਥਾਵਾਂ,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_ਛਾਂਟੋ(&ਛਾਂਟੀ_options),
		{
			.type = OPTION_CALLBACK,
			.long_name = "points-at",
			.ਮੁੱਲ = &filter.points_at,
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
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ ret = 0;
	ਅਚਲ ਅੱਖਰ *only_ਵਿੱਚ_list = ਨਿਰਾ;
	ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;

	ਤਿਆਰੀ_ref_filter_porcelain_ਸੁਨੇਹਾ();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_ਟੈਗ_config, &ਛਾਂਟੀ_options);
	ਜੇ (!ਛਾਂਟੀ_options.nr)
		string_list_append(&ਛਾਂਟੀ_options, "refname");

	memset(&opt, 0, ਆਕਾਰ(opt));
	filter.ਸਤਰਾਂ = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_ਟੈਗ_usage, 0);

	ਜੇ (!cmdmode) {
		ਜੇ (argc == 0)
			cmdmode = 'l';
		ਨਹੀਂਤਾਂ ਜੇ (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.ਸਤਰਾਂ != -1)
			cmdmode = 'l';
	}

	ਜੇ (cmdmode == 'l')
		ਤਿਆਰੀ_auto_pager("tag", 1);

	ਜੇ (trailer_ਦਲੀਲਾਂ.nr)
		trailer_config_init();

	ਜੇ (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_ਟੈਗ > 0;

	ਜੇ (keyid) {
		opt.sign = 1;
		ਸੈੱਟ_signing_ਕੁੰਜੀ(keyid);
	}
	create_ਟੈਗ_object = (opt.sign || annotate || ਸੁਨੇਹਾ.given || msgfile ||
			     edit_flag || trailer_ਦਲੀਲਾਂ.nr);

	ਜੇ ((create_ਟੈਗ_object || force) && (cmdmode != 0))
		usage_with_options(git_ਟੈਗ_usage, options);

	finalize_colopts(&colopts, -1);
	ਜੇ (cmdmode == 'l' && filter.ਸਤਰਾਂ != -1) {
		ਜੇ (explicitly_enable_ਕਾਲਮ(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	ਛਾਂਟੀ = ref_ਛਾਂਟੀ_options(&ਛਾਂਟੀ_options);
	ref_ਛਾਂਟੀ_ਸੈੱਟ_ਛਾਂਟੋ_flags_all(ਛਾਂਟੀ, REF_ਛਾਂਟੀ_ICASE, icase);
	filter.ignore_case = icase;
	ਜੇ (cmdmode == 'l') {
		ਜੇ (ਕਾਲਮ_ਸਰਗਰਮ(colopts)) {
			ਬਣਤਰ ਕਾਲਮ_options copts;
			memset(&copts, 0, ਆਕਾਰ(copts));
			copts.padding = 2;
			ਜੇ (ਲੜੀਵਾਰ_ਕਾਲਮ_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_ਟੈਗ_ਸੂਚੀ(&filter, ਛਾਂਟੀ, &format);
		ਜੇ (ਕਾਲਮ_ਸਰਗਰਮ(colopts))
			stop_ਕਾਲਮ_filter();
		ਜਾਓ cleanup;
	}
	ਜੇ (filter.ਸਤਰਾਂ != -1)
		only_ਵਿੱਚ_list = "-n";
	ਨਹੀਂਤਾਂ ਜੇ (filter.with_commit)
		only_ਵਿੱਚ_list = "--contains";
	ਨਹੀਂਤਾਂ ਜੇ (filter.no_commit)
		only_ਵਿੱਚ_list = "--no-contains";
	ਨਹੀਂਤਾਂ ਜੇ (filter.points_at.nr)
		only_ਵਿੱਚ_list = "--points-at";
	ਨਹੀਂਤਾਂ ਜੇ (filter.reachable_from)
		only_ਵਿੱਚ_list = "--merged";
	ਨਹੀਂਤਾਂ ਜੇ (filter.unreachable_from)
		only_ਵਿੱਚ_list = "--no-merged";
	ਜੇ (only_ਵਿੱਚ_list)
		die(_("the '%s' option is only allowed in list mode"), only_ਵਿੱਚ_list);
	ਜੇ (cmdmode == 'd') {
		ret = delete_ਟੈਗ_ਸੂਚੀ(argv);
		ਜਾਓ cleanup;
	}
	ਜੇ (cmdmode == 'v') {
		ਜੇ (format.format && verify_ref_format(&format))
			usage_with_options(git_ਟੈਗ_usage, options);
		ret = for_each_ਟੈਗ_name(argv, verify_ਟੈਗ, &format);
		ਜਾਓ cleanup;
	}

	ਜੇ (ਸੁਨੇਹਾ.given || msgfile) {
		ਜੇ (ਸੁਨੇਹਾ.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		ਜੇ (ਸੁਨੇਹਾ.given)
			strbuf_addbuf(&buf, &(ਸੁਨੇਹਾ.buf));
		ਨਹੀਂਤਾਂ {
			ਜੇ (!strcmp(msgfile, "-")) {
				ਜੇ (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} ਨਹੀਂਤਾਂ {
				ਜੇ (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	ਟੈਗ = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	ਜੇ (argc > 2)
		die(_("too many arguments"));

	ਜੇ (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	ਜੇ (check_ਟੈਗ_ref(&ref, ਟੈਗ))
		die(_("'%s' is not a valid tag name."), ਟੈਗ);

	ਜੇ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	ਨਹੀਂਤਾਂ ਜੇ (!force)
		die(_("tag '%s' already exists"), ਟੈਗ);

	opt.ਸੁਨੇਹਾ_given = ਸੁਨੇਹਾ.given || msgfile;
	opt.use_editor = edit_flag;

	ਜੇ (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_ਬਹੁਲਕ = CLEANUP_ALL;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_ਬਹੁਲਕ = CLEANUP_NONE;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_ਬਹੁਲਕ = CLEANUP_SPACE;
	ਨਹੀਂਤਾਂ
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_ਸੁਨੇਹਾ(&object, &reflog_ਸੁਨੇਹਾ);

	ਜੇ (create_ਟੈਗ_object) {
		ਜੇ (force_sign_annotate && !annotate)
			opt.sign = 1;
		ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "TAG_EDITMSG");
		create_ਟੈਗ(&object, object_ref, ਟੈਗ, &buf, &opt, &prev, &object,
			   &trailer_ਦਲੀਲਾਂ, ਰਸਤਾ);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ਜੇ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ਨਿਰਾ, ਨਿਰਾ,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_ਸੁਨੇਹਾ.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ਜੇ (ਰਸਤਾ)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				ਰਸਤਾ);
		die("%s", err.buf);
	}
	ਜੇ (ਰਸਤਾ) {
		unlink_or_warn(ਰਸਤਾ);
		ਮੁਕਤ(ਰਸਤਾ);
	}
	ref_transaction_free(transaction);
	ਜੇ (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		ਲਿਖੋ(_("Updated tag '%s' (was %s)\n"), ਟੈਗ,
		       repo_ਲੱਭੋ_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_ਛਾਂਟੀ_release(ਛਾਂਟੀ);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_ਸੁਨੇਹਾ);
	strbuf_release(&ਸੁਨੇਹਾ.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_ਦਲੀਲਾਂ);
	ਮੁਕਤ(msgfile);
	ਵਾਪਸ ret;
}
