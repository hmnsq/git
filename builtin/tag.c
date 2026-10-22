/*
 * Builtin "git tag"
 *
 * Copyright (c) 2007 Kristian Høgsberg <krh@redhat.com>,
 *                    Carlos Rica <jasampler@gmail.com>
 * Based on git-tag.sh and mktag.c by Linus Torvalds.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_ಟ್ಯಾಗ್_usage[] = {
	N_("git tag [-a | -s | -u <key-id>] [-f] [-m <msg> | -F <file>] [-e]\n"
	   "        [(--trailer <token>[(=|:)<value>])...]\n"
	   "        <tagname> [<commit> | <object>]"),
	N_("git tag -d <tagname>..."),
	N_("git tag [-n[<num>]] -l [--contains <commit>] [--no-contains <commit>]\n"
	   "        [--points-at <object>] [--column[=<options>] | --no-column]\n"
	   "        [--create-reflog] [--sort=<key>] [--format=<format>]\n"
	   "        [--merged <commit>] [--no-merged <commit>] [<pattern>...]"),
	N_("git tag -v [--format=<format>] <tagname>..."),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ colopts;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ force_sign_annotate;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ config_sign_ಟ್ಯಾಗ್ = -1; /* unspecified */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_ಟ್ಯಾಗುಗಳು(ರಚನೆ ref_filter *filter, ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ,
		     ರಚನೆ ref_format *format)
{
	ಅಕ್ಷರ *ಗಮ್ಯ_free = ಖಾಲಿ;

	ಆದರೆ (filter->ಸಾಲುಗಳು == -1)
		filter->ಸಾಲುಗಳು = 0;

	ಆದರೆ (!format->format) {
		ಆದರೆ (filter->ಸಾಲುಗಳು) {
			ಗಮ್ಯ_free = xstrfmt("%s %%(contents:lines=%d)",
					  "%(align:15)%(refname:lstrip=2)%(end)",
					  filter->ಸಾಲುಗಳು);
			format->format = ಗಮ್ಯ_free;
		} ಇಲ್ಲದಿದ್ದರೆ
			format->format = "%(refname:lstrip=2)";
	}

	ಆದರೆ (verify_ref_format(format))
		die(_("unable to parse format string"));
	filter->with_commit_ಟ್ಯಾಗ್_algo = 1;
	filter_and_format_refs(filter, FILTER_REFS_ಟ್ಯಾಗುಗಳು, ವಿಂಗಡಣೆ, format);

	ಬಿಡುಗಡೆ(ಗಮ್ಯ_free);

	ಹಿಂತಿರುಗಿ 0;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*each_ಟ್ಯಾಗ್_name_fn)(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
				ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ for_each_ಟ್ಯಾಗ್_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, each_ಟ್ಯಾಗ್_name_fn fn,
			     ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p;
	ರಚನೆ strbuf ref = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ had_error = 0;
	ರಚನೆ object_id oid;

	ಗಾಗಿ (p = argv; *p; p++) {
		strbuf_reset(&ref);
		strbuf_addf(&ref, "refs/tags/%s", *p);
		ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("tag '%s' not found."), *p);
			had_error = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (fn(*p, ref.buf, &oid, cb_ದತ್ತಾಂಶ))
			had_error = 1;
	}
	strbuf_release(&ref);
	ಹಿಂತಿರುಗಿ had_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ collect_ಟ್ಯಾಗುಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name ಬಳಸದ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
			ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ string_list *ref_list = cb_ದತ್ತಾಂಶ;

	string_list_append(ref_list, ref);
	ref_list->ವಸ್ತುಗಳು[ref_list->nr - 1].util = oiddup(oid);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_ಟ್ಯಾಗುಗಳು(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
	ರಚನೆ string_list refs_ಗಮ್ಯ_delete = STRING_LIST_INIT_DUP;
	ರಚನೆ string_list_ಅಂಶ *ಅಂಶ;

	ಫಲಿತಾಂಶ = for_each_ಟ್ಯಾಗ್_name(argv, collect_ಟ್ಯಾಗುಗಳು, (ಶೂನ್ಯ *)&refs_ಗಮ್ಯ_delete);
	ಆದರೆ (refs_delete_refs(get_main_ref_store(the_repository), ಖಾಲಿ, &refs_ಗಮ್ಯ_delete, REF_NO_DEREF))
		ಫಲಿತಾಂಶ = 1;

	for_each_string_list_ಅಂಶ(ಅಂಶ, &refs_ಗಮ್ಯ_delete) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = ಅಂಶ->string;
		ರಚನೆ object_id *oid = ಅಂಶ->util;
		ಆದರೆ (!refs_ref_exists(get_main_ref_store(the_repository), name))
			ಬರೆ(_("Deleted tag '%s' (was %s)\n"),
				ಅಂಶ->string + 10,
				repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, oid, DEFAULT_ABBREV));

		ಬಿಡುಗಡೆ(oid);
	}
	string_list_clear(&refs_ಗಮ್ಯ_delete, 0);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ verify_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref ಬಳಸದ,
		      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ flags;
	ರಚನೆ ref_format *format = cb_ದತ್ತಾಂಶ;
	flags = GPG_VERIFY_VERBOSE;

	ಆದರೆ (format->format)
		flags = GPG_VERIFY_OMIT_ಸ್ಥಿತಿ;

	ಆದರೆ (gpg_verify_ಟ್ಯಾಗ್(the_repository, oid, name, flags))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (format->format)
		pretty_print_ref(name, oid, ಖಾಲಿ, format);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_sign(ರಚನೆ strbuf *ಬಫರ್, ರಚನೆ object_id **compat_oid,
		   ರಚನೆ object_id *compat_oid_buf)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ git_hash_algo *compat = the_repository->compat_hash_algo;
	ರಚನೆ strbuf sig = STRBUF_INIT, compat_sig = STRBUF_INIT;
	ರಚನೆ strbuf compat_buf = STRBUF_INIT;
	ಅಕ್ಷರ *keyid = get_signing_ಕೀಲಿ();
	ಪೂರ್ಣಾಂಕ ret = -1;

	ಆದರೆ (sign_ಬಫರ್(ಬಫರ್, &sig, keyid, 0))
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (compat) {
		ಸ್ಥಿರಾಂಕ ರಚನೆ git_hash_algo *algo = the_repository->hash_algo;

		ಆದರೆ (convert_object_file(the_repository ,&compat_buf, algo, compat,
					ಬಫರ್->buf, ಬಫರ್->len, OBJ_ಟ್ಯಾಗ್, 1))
			ಹೋಗು ಹೊರಹೋಗು;
		ಆದರೆ (sign_ಬಫರ್(&compat_buf, &compat_sig, keyid, 0))
			ಹೋಗು ಹೊರಹೋಗು;
		ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ_signature(&compat_buf, &sig, algo);
		strbuf_addbuf(&compat_buf, &compat_sig);
		hash_object_file(compat, compat_buf.buf, compat_buf.len,
				 OBJ_ಟ್ಯಾಗ್, compat_oid_buf);
		*compat_oid = compat_oid_buf;
	}

	ಆದರೆ (compat_sig.len)
		ಕೂಡಿಸು_ಶೀರ್ಷಿಕೆ_signature(ಬಫರ್, &compat_sig, compat);

	strbuf_addbuf(ಬಫರ್, &sig);
	ret = 0;
ಹೊರಹೋಗು:
	strbuf_release(&sig);
	strbuf_release(&compat_sig);
	strbuf_release(&compat_buf);
	ಬಿಡುಗಡೆ(keyid);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಟ್ಯಾಗ್_template[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be ignored.\n");

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಟ್ಯಾಗ್_template_nocleanup[] =
	N_("\nWrite a message for tag:\n  %s\n"
	"Lines starting with '%s' will be kept; you may remove them"
	" yourself if you want to.\n");

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_ಟ್ಯಾಗ್_config(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *var, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ config_context *ctx, ಶೂನ್ಯ *cb)
{
	ಆದರೆ (!strcmp(var, "tag.gpgsign")) {
		config_sign_ಟ್ಯಾಗ್ = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "tag.sort")) {
		ಆದರೆ (!ಮೌಲ್ಯ)
			ಹಿಂತಿರುಗಿ config_error_nonbool(var);
		string_list_append(cb, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!strcmp(var, "tag.forcesignannotated")) {
		force_sign_annotate = git_config_bool(var, ಮೌಲ್ಯ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (starts_with(var, "column."))
		ಹಿಂತಿರುಗಿ git_ಸ್ತಂಭ_config(var, ಮೌಲ್ಯ, "tag", &colopts);

	ಆದರೆ (git_color_config(var, ಮೌಲ್ಯ, cb) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ git_default_config(var, ಮೌಲ್ಯ, ctx, cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ write_ಟ್ಯಾಗ್_body(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಎಣಿಕೆ object_type type;
	ಅಕ್ಷರ *buf, *sp, *orig;
	ರಚನೆ strbuf payload = STRBUF_INIT;
	ರಚನೆ strbuf signature = STRBUF_INIT;

	orig = buf = odb_read_object(the_repository->objects, oid, &type, &ಗಾತ್ರ_2);
	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (parse_signature(buf, ಗಾತ್ರ_2, &payload, &signature)) {
		buf = payload.buf;
		ಗಾತ್ರ_2 = payload.len;
	}
	/* skip header */
	sp = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(buf, "\n\n");

	ಆದರೆ (!sp || !ಗಾತ್ರ_2 || type != OBJ_ಟ್ಯಾಗ್) {
		ಬಿಡುಗಡೆ(buf);
		ಹಿಂತಿರುಗಿ;
	}
	sp += 2; /* skip the 2 LFs */
	write_or_die(fd, sp, buf + ಗಾತ್ರ_2 - sp);

	ಬಿಡುಗಡೆ(orig);
	strbuf_release(&payload);
	strbuf_release(&signature);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ build_ಟ್ಯಾಗ್_object(ರಚನೆ strbuf *buf, ಪೂರ್ಣಾಂಕ sign, ರಚನೆ object_id *ಫಲಿತಾಂಶ)
{
	ರಚನೆ object_id *compat_oid = ಖಾಲಿ, compat_oid_buf;
	ಆದರೆ (sign && do_sign(buf, &compat_oid, &compat_oid_buf) < 0)
		ಹಿಂತಿರುಗಿ error(_("unable to sign the tag"));
	ಆದರೆ (odb_write_object_ext(the_repository->objects, buf->buf,
				 buf->len, OBJ_ಟ್ಯಾಗ್, ಫಲಿತಾಂಶ, compat_oid, 0) < 0)
		ಹಿಂತಿರುಗಿ error(_("unable to write tag file"));
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ create_ಟ್ಯಾಗ್_options {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸಂದೇಶ_given:1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ use_editor:1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sign;
	ಎಣಿಕೆ {
		CLEANUP_NONE,
		CLEANUP_SPACE,
		CLEANUP_ALL
	} cleanup_ಬಹುಲಕ;
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸಂದೇಶ_advice_nested_ಟ್ಯಾಗ್[] =
	N_("You have created a nested tag. The object referred to by your new tag is\n"
	   "already a tag. If you meant to tag the object that it points to, use:\n"
	   "\n"
	   "\tgit tag -f %s %s^{}");

ಸ್ಥಿರ ಶೂನ್ಯ create_ಟ್ಯಾಗ್(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *object, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್,
		       ರಚನೆ strbuf *buf, ರಚನೆ create_ಟ್ಯಾಗ್_options *opt,
		       ರಚನೆ object_id *prev, ರಚನೆ object_id *ಫಲಿತಾಂಶ,
		       ರಚನೆ strvec *trailer_ವಾದಗಳು, ಅಕ್ಷರ *ಮಾರ್ಗ)
{
	ಎಣಿಕೆ object_type type;
	ರಚನೆ strbuf ಶೀರ್ಷಿಕೆ = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ should_edit;

	type = odb_read_object_info(the_repository->objects, object, ಖಾಲಿ);
	ಆದರೆ (type <= OBJ_NONE)
		die(_("bad object type."));

	ಆದರೆ (type == OBJ_ಟ್ಯಾಗ್)
		advise_if_enabled(ADVICE_NESTED_ಟ್ಯಾಗ್, _(ಸಂದೇಶ_advice_nested_ಟ್ಯಾಗ್),
				  ಟ್ಯಾಗ್, object_ref);

	strbuf_addf(&ಶೀರ್ಷಿಕೆ,
		    "object %s\n"
		    "type %s\n"
		    "tag %s\n"
		    "tagger %s\n\n",
		    oid_ಗಮ್ಯ_hex(object),
		    type_name(type),
		    ಟ್ಯಾಗ್,
		    git_committer_info(IDENT_STRICT));

	should_edit = opt->use_editor || !opt->ಸಂದೇಶ_given;
	ಆದರೆ (should_edit || trailer_ವಾದಗಳು->nr) {
		ಪೂರ್ಣಾಂಕ fd;

		/* write the template message before editing: */
		fd = xopen(ಮಾರ್ಗ, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ಆದರೆ (opt->ಸಂದೇಶ_given && buf->len) {
			strbuf_complete(buf, '\n');
			write_or_die(fd, buf->buf, buf->len);
			strbuf_reset(buf);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_null_oid(prev)) {
			write_ಟ್ಯಾಗ್_body(fd, prev);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ strbuf buf = STRBUF_INIT;
			strbuf_addch(&buf, '\n');
			ಆದರೆ (opt->cleanup_ಬಹುಲಕ == CLEANUP_ALL)
				strbuf_commented_addf(&buf, comment_ಸಾಲು_str,
				      _(ಟ್ಯಾಗ್_template), ಟ್ಯಾಗ್, comment_ಸಾಲು_str);
			ಇಲ್ಲದಿದ್ದರೆ
				strbuf_commented_addf(&buf, comment_ಸಾಲು_str,
				      _(ಟ್ಯಾಗ್_template_nocleanup), ಟ್ಯಾಗ್, comment_ಸಾಲು_str);
			write_or_die(fd, buf.buf, buf.len);
			strbuf_release(&buf);
		}
		close(fd);

		ಆದರೆ (trailer_ವಾದಗಳು->nr && amend_file_with_trailers(ಮಾರ್ಗ, trailer_ವಾದಗಳು))
			die(_("unable to pass trailers to --trailers"));

		ಆದರೆ (should_edit) {
			ಆದರೆ (launch_editor(ಮಾರ್ಗ, buf, ಖಾಲಿ)) {
				fprintf(stderr,
					_("Please supply the message using either -m or -F option.\n"));
				ನಿರ್ಗಮಿಸು(1);
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (trailer_ವಾದಗಳು->nr) {
			strbuf_reset(buf);
			ಆದರೆ (strbuf_read_file(buf, ಮಾರ್ಗ, 0) < 0)
				die_errno(_("failed to read '%s'"), ಮಾರ್ಗ);
		}
	}

	ಆದರೆ (opt->cleanup_ಬಹುಲಕ != CLEANUP_NONE)
		strbuf_stripspace(buf,
		  opt->cleanup_ಬಹುಲಕ == CLEANUP_ALL ? comment_ಸಾಲು_str : ಖಾಲಿ);

	ಆದರೆ (!opt->ಸಂದೇಶ_given && !buf->len)
		die(_("no tag message?"));

	strbuf_ಸೇರಿಸು(buf, 0, ಶೀರ್ಷಿಕೆ.buf, ಶೀರ್ಷಿಕೆ.len);
	strbuf_release(&ಶೀರ್ಷಿಕೆ);

	ಆದರೆ (build_ಟ್ಯಾಗ್_object(buf, opt->sign, ಫಲಿತಾಂಶ) < 0) {
		ಆದರೆ (ಮಾರ್ಗ)
			fprintf(stderr, _("The tag message has been left in %s\n"),
				ಮಾರ್ಗ);
		ನಿರ್ಗಮಿಸು(128);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ create_reflog_ಸಂದೇಶ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ರಚನೆ strbuf *sb)
{
	ಎಣಿಕೆ object_type type;
	ರಚನೆ commit *c;
	ಅಕ್ಷರ *buf;
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಪೂರ್ಣಾಂಕ subject_len = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subject_ಆರಂಭ;

	ಅಕ್ಷರ *rla = getenv("GIT_REFLOG_ACTION");
	ಆದರೆ (rla) {
		strbuf_addstr(sb, rla);
	} ಇಲ್ಲದಿದ್ದರೆ {
		strbuf_addstr(sb, "tag: tagging ");
		strbuf_ಕೂಡಿಸು_unique_abbrev(sb, oid, DEFAULT_ABBREV);
	}

	strbuf_addstr(sb, " (");
	type = odb_read_object_info(the_repository->objects, oid, ಖಾಲಿ);
	ಆಯ್ಕೆ (type) {
	ಪೂರ್ವನಿಯೋಜಿತ:
		strbuf_addstr(sb, "object of unknown type");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_COMMIT:
		ಆದರೆ ((buf = odb_read_object(the_repository->objects, oid, &type, &ಗಾತ್ರ_2))) {
			subject_len = ಕಂಡುಹಿಡಿ_commit_subject(buf, &subject_ಆರಂಭ);
			strbuf_ಸೇರಿಸು(sb, sb->len, subject_ಆರಂಭ, subject_len);
		} ಇಲ್ಲದಿದ್ದರೆ {
			strbuf_addstr(sb, "commit object");
		}
		ಬಿಡುಗಡೆ(buf);

		ಆದರೆ ((c = lookup_commit_reference(the_repository, oid)))
			strbuf_addf(sb, ", %s", show_date(c->date, 0, DATE_ಬಹುಲಕ(SHORT)));
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_ವೃಕ್ಷ:
		strbuf_addstr(sb, "tree object");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_BLOB:
		strbuf_addstr(sb, "blob object");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ OBJ_ಟ್ಯಾಗ್:
		strbuf_addstr(sb, "other tag object");
		ನಿಲ್ಲಿಸು;
	}
	strbuf_addch(sb, ')');
}

ರಚನೆ ಸಂದೇಶ_arg {
	ಪೂರ್ಣಾಂಕ given;
	ರಚನೆ strbuf buf;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಸಂದೇಶ_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ ಸಂದೇಶ_arg *ಸಂದೇಶ = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!arg)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (ಸಂದೇಶ->buf.len)
		strbuf_addstr(&(ಸಂದೇಶ->buf), "\n\n");
	strbuf_addstr(&(ಸಂದೇಶ->buf), arg);
	ಸಂದೇಶ->given = 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_ಟ್ಯಾಗ್(ಪೂರ್ಣಾಂಕ argc,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	    ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf ref = STRBUF_INIT;
	ರಚನೆ strbuf reflog_ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ object_id object, prev;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref, *ಟ್ಯಾಗ್;
	ರಚನೆ create_ಟ್ಯಾಗ್_options opt;
	ಅಕ್ಷರ *cleanup_arg = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ create_reflog = 0;
	ಪೂರ್ಣಾಂಕ annotate = 0, force = 0;
	ಪೂರ್ಣಾಂಕ cmdmode = 0, create_ಟ್ಯಾಗ್_object = 0;
	ಅಕ್ಷರ *msgfile = ಖಾಲಿ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *keyid = ಖಾಲಿ;
	ರಚನೆ ಸಂದೇಶ_arg ಸಂದೇಶ = { .buf = STRBUF_INIT };
	ರಚನೆ ref_transaction *transaction;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ರಚನೆ ref_filter filter = REF_FILTER_INIT;
	ರಚನೆ ref_ವಿಂಗಡಣೆ *ವಿಂಗಡಣೆ;
	ರಚನೆ string_list ವಿಂಗಡಣೆ_options = STRING_LIST_INIT_DUP;
	ರಚನೆ ref_format format = REF_FORMAT_INIT;
	ರಚನೆ strvec trailer_ವಾದಗಳು = STRVEC_INIT;
	ಪೂರ್ಣಾಂಕ icase = 0;
	ಪೂರ್ಣಾಂಕ edit_flag = 0;
	ರಚನೆ option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list tag names"), 'l'),
		{
			.type = OPTION_INTEGER,
			.short_name = 'n',
			.ಮೌಲ್ಯ = &filter.ಸಾಲುಗಳು,
			.precision = ಗಾತ್ರ(filter.ಸಾಲುಗಳು),
			.argh = N_("n"),
			.help = N_("print <n> lines of each tag message"),
			.flags = PARSE_OPT_OPTARG,
			.defval = 1,
		},
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete tags"), 'd'),
		OPT_CMDMODE('v', "verify", &cmdmode, N_("verify tags"), 'v'),

		OPT_ಗುಂಪು(N_("Tag creation options")),
		OPT_BOOL('a', "annotate", &annotate,
					N_("annotated tag, needs a message")),
		OPT_CALLBACK_F('m', "message", &ಸಂದೇಶ, N_("message"),
			       N_("tag message"), PARSE_OPT_NONEG, parse_ಸಂದೇಶ_arg),
		OPT_FILENAME('F', "file", &msgfile, N_("read message from file")),
		OPT_STRVEC(0, "trailer", &trailer_ವಾದಗಳು, N_("trailer"),
			   N_("add custom trailer(s)")),
		OPT_BOOL('e', "edit", &edit_flag, N_("force edit of tag message")),
		OPT_BOOL('s', "sign", &opt.sign, N_("annotated and GPG-signed tag")),
		OPT_CLEANUP(&cleanup_arg),
		OPT_STRING('u', "local-user", &keyid, N_("key-id"),
					N_("use another key to sign the tag")),
		OPT__FORCE(&force, N_("replace the tag if exists"), 0),
		OPT_BOOL(0, "create-reflog", &create_reflog, N_("create a reflog")),

		OPT_ಗುಂಪು(N_("Tag listing options")),
		OPT_ಸ್ತಂಭ(0, "column", &colopts, N_("show tag list in columns")),
		OPT_ಒಳಗೊಂಡಿದೆ(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_NO_ಒಳಗೊಂಡಿದೆ(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_WITH(&filter.with_commit, N_("print only tags that contain the commit")),
		OPT_WITHOUT(&filter.no_commit, N_("print only tags that don't contain the commit")),
		OPT_MERGED(&filter, N_("print only tags that are merged")),
		OPT_NO_MERGED(&filter, N_("print only tags that are not merged")),
		OPT_BOOL(0, "omit-empty",  &format.array_opts.omit_ಖಾಲಿಗಳು,
			N_("do not output a newline after empty formatted refs")),
		OPT_REF_ವಿಂಗಡಿಸು(&ವಿಂಗಡಣೆ_options),
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
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *only_ಒಳ_list = ಖಾಲಿ;
	ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;

	ಸಿದ್ಧತೆ_ref_filter_porcelain_ಸಂದೇಶ();

	/*
	 * Try to set sort keys from config. If config does not set any,
	 * fall back on default (refname) sorting.
	 */
	repo_config(the_repository, git_ಟ್ಯಾಗ್_config, &ವಿಂಗಡಣೆ_options);
	ಆದರೆ (!ವಿಂಗಡಣೆ_options.nr)
		string_list_append(&ವಿಂಗಡಣೆ_options, "refname");

	memset(&opt, 0, ಗಾತ್ರ(opt));
	filter.ಸಾಲುಗಳು = -1;
	opt.sign = -1;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_ಟ್ಯಾಗ್_usage, 0);

	ಆದರೆ (!cmdmode) {
		ಆದರೆ (argc == 0)
			cmdmode = 'l';
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter.with_commit || filter.no_commit ||
			 filter.reachable_from || filter.unreachable_from ||
			 filter.points_at.nr || filter.ಸಾಲುಗಳು != -1)
			cmdmode = 'l';
	}

	ಆದರೆ (cmdmode == 'l')
		ಸಿದ್ಧತೆ_auto_pager("tag", 1);

	ಆದರೆ (trailer_ವಾದಗಳು.nr)
		trailer_config_init();

	ಆದರೆ (opt.sign == -1)
		opt.sign = cmdmode ? 0 : config_sign_ಟ್ಯಾಗ್ > 0;

	ಆದರೆ (keyid) {
		opt.sign = 1;
		ಗಣ_signing_ಕೀಲಿ(keyid);
	}
	create_ಟ್ಯಾಗ್_object = (opt.sign || annotate || ಸಂದೇಶ.given || msgfile ||
			     edit_flag || trailer_ವಾದಗಳು.nr);

	ಆದರೆ ((create_ಟ್ಯಾಗ್_object || force) && (cmdmode != 0))
		usage_with_options(git_ಟ್ಯಾಗ್_usage, options);

	finalize_colopts(&colopts, -1);
	ಆದರೆ (cmdmode == 'l' && filter.ಸಾಲುಗಳು != -1) {
		ಆದರೆ (explicitly_enable_ಸ್ತಂಭ(colopts))
			die(_("options '%s' and '%s' cannot be used together"), "--column", "-n");
		colopts = 0;
	}
	ವಿಂಗಡಣೆ = ref_ವಿಂಗಡಣೆ_options(&ವಿಂಗಡಣೆ_options);
	ref_ವಿಂಗಡಣೆ_ಗಣ_ವಿಂಗಡಿಸು_flags_all(ವಿಂಗಡಣೆ, REF_ವಿಂಗಡಣೆ_ICASE, icase);
	filter.ignore_case = icase;
	ಆದರೆ (cmdmode == 'l') {
		ಆದರೆ (ಸ್ತಂಭ_ಸಕ್ರಿಯ(colopts)) {
			ರಚನೆ ಸ್ತಂಭ_options copts;
			memset(&copts, 0, ಗಾತ್ರ(copts));
			copts.padding = 2;
			ಆದರೆ (ಸತತ_ಸ್ತಂಭ_filter(colopts, &copts))
				die(_("could not start 'git column'"));
		}
		filter.name_patterns = argv;
		ret = list_ಟ್ಯಾಗುಗಳು(&filter, ವಿಂಗಡಣೆ, &format);
		ಆದರೆ (ಸ್ತಂಭ_ಸಕ್ರಿಯ(colopts))
			stop_ಸ್ತಂಭ_filter();
		ಹೋಗು cleanup;
	}
	ಆದರೆ (filter.ಸಾಲುಗಳು != -1)
		only_ಒಳ_list = "-n";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter.with_commit)
		only_ಒಳ_list = "--contains";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter.no_commit)
		only_ಒಳ_list = "--no-contains";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter.points_at.nr)
		only_ಒಳ_list = "--points-at";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter.reachable_from)
		only_ಒಳ_list = "--merged";
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (filter.unreachable_from)
		only_ಒಳ_list = "--no-merged";
	ಆದರೆ (only_ಒಳ_list)
		die(_("the '%s' option is only allowed in list mode"), only_ಒಳ_list);
	ಆದರೆ (cmdmode == 'd') {
		ret = delete_ಟ್ಯಾಗುಗಳು(argv);
		ಹೋಗು cleanup;
	}
	ಆದರೆ (cmdmode == 'v') {
		ಆದರೆ (format.format && verify_ref_format(&format))
			usage_with_options(git_ಟ್ಯಾಗ್_usage, options);
		ret = for_each_ಟ್ಯಾಗ್_name(argv, verify_ಟ್ಯಾಗ್, &format);
		ಹೋಗು cleanup;
	}

	ಆದರೆ (ಸಂದೇಶ.given || msgfile) {
		ಆದರೆ (ಸಂದೇಶ.given && msgfile)
			die(_("options '%s' and '%s' cannot be used together"), "-F", "-m");
		ಆದರೆ (ಸಂದೇಶ.given)
			strbuf_addbuf(&buf, &(ಸಂದೇಶ.buf));
		ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (!strcmp(msgfile, "-")) {
				ಆದರೆ (strbuf_read(&buf, 0, 1024) < 0)
					die_errno(_("cannot read '%s'"), msgfile);
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಆದರೆ (strbuf_read_file(&buf, msgfile, 1024) < 0)
					die_errno(_("could not open or read '%s'"),
						msgfile);
			}
		}
	}

	ಟ್ಯಾಗ್ = argv[0];

	object_ref = argc == 2 ? argv[1] : "HEAD";
	ಆದರೆ (argc > 2)
		die(_("too many arguments"));

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &object))
		die(_("Failed to resolve '%s' as a valid ref."), object_ref);

	ಆದರೆ (check_ಟ್ಯಾಗ್_ref(&ref, ಟ್ಯಾಗ್))
		die(_("'%s' is not a valid tag name."), ಟ್ಯಾಗ್);

	ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &prev))
		oidclr(&prev, the_repository->hash_algo);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!force)
		die(_("tag '%s' already exists"), ಟ್ಯಾಗ್);

	opt.ಸಂದೇಶ_given = ಸಂದೇಶ.given || msgfile;
	opt.use_editor = edit_flag;

	ಆದರೆ (!cleanup_arg || !strcmp(cleanup_arg, "strip"))
		opt.cleanup_ಬಹುಲಕ = CLEANUP_ALL;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(cleanup_arg, "verbatim"))
		opt.cleanup_ಬಹುಲಕ = CLEANUP_NONE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(cleanup_arg, "whitespace"))
		opt.cleanup_ಬಹುಲಕ = CLEANUP_SPACE;
	ಇಲ್ಲದಿದ್ದರೆ
		die(_("Invalid cleanup mode %s"), cleanup_arg);

	create_reflog_ಸಂದೇಶ(&object, &reflog_ಸಂದೇಶ);

	ಆದರೆ (create_ಟ್ಯಾಗ್_object) {
		ಆದರೆ (force_sign_annotate && !annotate)
			opt.sign = 1;
		ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "TAG_EDITMSG");
		create_ಟ್ಯಾಗ್(&object, object_ref, ಟ್ಯಾಗ್, &buf, &opt, &prev, &object,
			   &trailer_ವಾದಗಳು, ಮಾರ್ಗ);
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಆದರೆ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, &object, &prev,
				   ಖಾಲಿ, ಖಾಲಿ,
				   create_reflog ? REF_FORCE_CREATE_REFLOG : 0,
				   reflog_ಸಂದೇಶ.buf, &err) ||
	    ref_transaction_commit(transaction, &err)) {
		ಆದರೆ (ಮಾರ್ಗ)
			fprintf(stderr,
				_("The tag message has been left in %s\n"),
				ಮಾರ್ಗ);
		die("%s", err.buf);
	}
	ಆದರೆ (ಮಾರ್ಗ) {
		unlink_or_warn(ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	}
	ref_transaction_free(transaction);
	ಆದರೆ (force && !is_null_oid(&prev) && !oideq(&prev, &object))
		ಬರೆ(_("Updated tag '%s' (was %s)\n"), ಟ್ಯಾಗ್,
		       repo_ಕಂಡುಹಿಡಿ_unique_abbrev(the_repository, &prev, DEFAULT_ABBREV));

cleanup:
	ref_ವಿಂಗಡಣೆ_release(ವಿಂಗಡಣೆ);
	ref_filter_clear(&filter);
	strbuf_release(&buf);
	strbuf_release(&ref);
	strbuf_release(&reflog_ಸಂದೇಶ);
	strbuf_release(&ಸಂದೇಶ.buf);
	strbuf_release(&err);
	strvec_clear(&trailer_ವಾದಗಳು);
	ಬಿಡುಗಡೆ(msgfile);
	ಹಿಂತಿರುಗಿ ret;
}
