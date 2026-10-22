/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "path.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "replace-object.h"
#ಅನ್ತರ್ಭಾವ "tag.h"
#ಅನ್ತರ್ಭಾವ "wildmatch.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ಖಾಲಿ
};

ಎಣಿಕೆ replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ರಚನೆ show_ದತ್ತಾಂಶ {
	ರಚನೆ repository *repo;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ನಮೂನೆ;
	ಎಣಿಕೆ replace_format format;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show_reference(ಸ್ಥಿರಾಂಕ ರಚನೆ reference *ref, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ)
{
	ರಚನೆ show_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ = cb_ದತ್ತಾಂಶ;

	ಆದರೆ (!wildmatch(ದತ್ತಾಂಶ->ನಮೂನೆ, ref->name, 0)) {
		ಆದರೆ (ದತ್ತಾಂಶ->format == REPLACE_FORMAT_SHORT)
			ಬರೆ("%s\n", ref->name);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ದತ್ತಾಂಶ->format == REPLACE_FORMAT_MEDIUM)
			ಬರೆ("%s -> %s\n", ref->name, oid_ಗಮ್ಯ_hex(ref->oid));
		ಇಲ್ಲದಿದ್ದರೆ { /* data->format == REPLACE_FORMAT_LONG */
			ರಚನೆ object_id object;
			ಎಣಿಕೆ object_type obj_type, repl_type;

			ಆದರೆ (repo_get_oid(ದತ್ತಾಂಶ->repo, ref->name, &object))
				ಹಿಂತಿರುಗಿ error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(ದತ್ತಾಂಶ->repo->objects, &object, ಖಾಲಿ);
			repl_type = odb_read_object_info(ದತ್ತಾಂಶ->repo->objects, ref->oid, ಖಾಲಿ);

			ಬರೆ("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_ಗಮ್ಯ_hex(ref->oid), type_name(repl_type));
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_replace_refs(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ನಮೂನೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format)
{
	ರಚನೆ show_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;

	ದತ್ತಾಂಶ.repo = the_repository;
	ಆದರೆ (!ನಮೂನೆ)
		ನಮೂನೆ = "*";
	ದತ್ತಾಂಶ.ನಮೂನೆ = ನಮೂನೆ;

	ಆದರೆ (format == ಖಾಲಿ || *format == '\0' || !strcmp(format, "short"))
		ದತ್ತಾಂಶ.format = REPLACE_FORMAT_SHORT;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(format, "medium"))
		ದತ್ತಾಂಶ.format = REPLACE_FORMAT_MEDIUM;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!strcmp(format, "long"))
		ದತ್ತಾಂಶ.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (ಶೂನ್ಯ *)&ದತ್ತಾಂಶ);

	ಹಿಂತಿರುಗಿ 0;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*each_replace_name_fn)(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ for_each_replace_name(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, each_replace_name_fn fn)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **p, *ತುಂಬಿದ_hex;
	ರಚನೆ strbuf ref = STRBUF_INIT;
	ಗಾತ್ರ_t ತಳ_len;
	ಪೂರ್ಣಾಂಕ had_error = 0;
	ರಚನೆ object_id oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_replace_ref_ತಳ = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_ತಳ);
	ತಳ_len = ref.len;

	ಗಾಗಿ (p = argv; *p; p++) {
		ಆದರೆ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			ಮುಂದುವರಿ;
		}

		strbuf_setlen(&ref, ತಳ_len);
		strbuf_ಕೂಡಿಸು_oid_hex(&ref, &oid);
		ತುಂಬಿದ_hex = ref.buf + ತಳ_len;

		ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), ತುಂಬಿದ_hex);
			had_error = 1;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (fn(ತುಂಬಿದ_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ಹಿಂತಿರುಗಿ had_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ delete_replace_ref(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಆದರೆ (refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ, ref, oid, 0))
		ಹಿಂತಿರುಗಿ 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_ref_valid(ರಚನೆ object_id *object,
			    ರಚನೆ object_id *prev,
			    ರಚನೆ strbuf *ref,
			    ಪೂರ್ಣಾಂಕ force)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *git_replace_ref_ತಳ = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_ತಳ, oid_ಗಮ್ಯ_hex(object));
	ಆದರೆ (check_refname_format(ref->buf, 0))
		ಹಿಂತಿರುಗಿ error(_("'%s' is not a valid ref name"), ref->buf);

	ಆದರೆ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!force)
		ಹಿಂತಿರುಗಿ error(_("replace ref '%s' already exists"), ref->buf);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ replace_object_oid(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref,
			       ರಚನೆ object_id *object,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *replace_ref,
			       ರಚನೆ object_id *repl,
			       ಪೂರ್ಣಾಂಕ force)
{
	ರಚನೆ object_id prev;
	ಎಣಿಕೆ object_type obj_type, repl_type;
	ರಚನೆ strbuf ref = STRBUF_INIT;
	ರಚನೆ ref_transaction *transaction;
	ರಚನೆ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಂಕ res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ಖಾಲಿ);
	repl_type = odb_read_object_info(the_repository->objects, repl, ಖಾಲಿ);
	ಆದರೆ (!force && obj_type != repl_type)
		ಹಿಂತಿರುಗಿ error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ಆದರೆ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		ಹಿಂತಿರುಗಿ -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಆದರೆ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ಖಾಲಿ, ಖಾಲಿ, 0, ಖಾಲಿ, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ಹಿಂತಿರುಗಿ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ replace_object(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *replace_ref, ಪೂರ್ಣಾಂಕ force)
{
	ರಚನೆ object_id object, repl;

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &object))
		ಹಿಂತಿರುಗಿ error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ಆದರೆ (repo_get_oid(the_repository, replace_ref, &repl))
		ಹಿಂತಿರುಗಿ error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ಹಿಂತಿರುಗಿ replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ export_object(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid, ಎಣಿಕೆ object_type type,
			  ಪೂರ್ಣಾಂಕ raw, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
	ಪೂರ್ಣಾಂಕ fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ error_errno(_("unable to open %s for writing"), filename);

	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "--no-replace-objects");
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "cat-file");
	ಆದರೆ (raw)
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, type_name(type));
	ಇಲ್ಲದಿದ್ದರೆ
		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "-p");
	strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, oid_ಗಮ್ಯ_hex(oid));
	cmd.git_cmd = 1;
	cmd.ಹೊರಹೋಗು = fd;

	ಆದರೆ (ಸತತ_command(&cmd))
		ಹಿಂತಿರುಗಿ error(_("cat-file reported failure"));
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ import_object(ರಚನೆ object_id *oid, ಎಣಿಕೆ object_type type,
			  ಪೂರ್ಣಾಂಕ raw, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename)
{
	ಪೂರ್ಣಾಂಕ fd;

	fd = open(filename, O_RDONLY);
	ಆದರೆ (fd < 0)
		ಹಿಂತಿರುಗಿ error_errno(_("unable to open %s for reading"), filename);

	ಆದರೆ (!raw && type == OBJ_ವೃಕ್ಷ) {
		ರಚನೆ ಮಗು_process cmd = ಮಗು_PROCESS_INIT;
		ರಚನೆ strbuf ಫಲಿತಾಂಶ = STRBUF_INIT;

		strvec_ತಳ್ಳು(&cmd.ವಾದಗಳು, "mktree");
		cmd.git_cmd = 1;
		cmd.ಒಳ = fd;
		cmd.ಹೊರಹೋಗು = -1;

		ಆದರೆ (ಆರಂಭ_command(&cmd)) {
			close(fd);
			ಹಿಂತಿರುಗಿ error(_("unable to spawn mktree"));
		}

		ಆದರೆ (strbuf_read(&ಫಲಿತಾಂಶ, cmd.ಹೊರಹೋಗು, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.ಹೊರಹೋಗು);
			ಹಿಂತಿರುಗಿ -1;
		}
		close(cmd.ಹೊರಹೋಗು);

		ಆದರೆ (ಮುಕ್ತಾಯ_command(&cmd)) {
			strbuf_release(&ಫಲಿತಾಂಶ);
			ಹಿಂತಿರುಗಿ error(_("mktree reported failure"));
		}
		ಆದರೆ (get_oid_hex(ಫಲಿತಾಂಶ.buf, oid) < 0) {
			strbuf_release(&ಫಲಿತಾಂಶ);
			ಹಿಂತಿರುಗಿ error(_("mktree did not return an object name"));
		}

		strbuf_release(&ಫಲಿತಾಂಶ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ stat st;
		ಪೂರ್ಣಾಂಕ flags = ಸೂಚ್ಯಂಕ_FORMAT_CHECK | ಸೂಚ್ಯಂಕ_WRITE_OBJECT;

		ಆದರೆ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ಹಿಂತಿರುಗಿ -1;
		}
		ಆದರೆ (ಸೂಚ್ಯಂಕ_fd(the_repository->ಸೂಚ್ಯಂಕ, oid, fd, &st, type, ಖಾಲಿ, flags) < 0)
			ಹಿಂತಿರುಗಿ error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ edit_and_replace(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref, ಪೂರ್ಣಾಂಕ force, ಪೂರ್ಣಾಂಕ raw)
{
	ಅಕ್ಷರ *tmpfile;
	ಎಣಿಕೆ object_type type;
	ರಚನೆ object_id old_oid, new_oid, prev;
	ರಚನೆ strbuf ref = STRBUF_INIT;

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ಹಿಂತಿರುಗಿ error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ಖಾಲಿ);
	ಆದರೆ (type < 0)
		ಹಿಂತಿರುಗಿ error(_("unable to get object type for %s"),
			     oid_ಗಮ್ಯ_hex(&old_oid));

	ಆದರೆ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		ಹಿಂತಿರುಗಿ -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_ಮಾರ್ಗ(the_repository, "REPLACE_EDITOBJ");
	ಆದರೆ (export_object(&old_oid, type, raw, tmpfile)) {
		ಬಿಡುಗಡೆ(tmpfile);
		ಹಿಂತಿರುಗಿ -1;
	}
	ಆದರೆ (launch_editor(tmpfile, ಖಾಲಿ, ಖಾಲಿ) < 0) {
		ಬಿಡುಗಡೆ(tmpfile);
		ಹಿಂತಿರುಗಿ error(_("editing object file failed"));
	}
	ಆದರೆ (import_object(&new_oid, type, raw, tmpfile)) {
		ಬಿಡುಗಡೆ(tmpfile);
		ಹಿಂತಿರುಗಿ -1;
	}
	ಬಿಡುಗಡೆ(tmpfile);

	ಆದರೆ (oideq(&old_oid, &new_oid))
		ಹಿಂತಿರುಗಿ error(_("new object is the same as the old one: '%s'"), oid_ಗಮ್ಯ_hex(&old_oid));

	ಹಿಂತಿರುಗಿ replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ replace_parents(ರಚನೆ strbuf *buf, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ strbuf new_parents = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_ಆರಂಭ, *ಪೋಷಕ_ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ i;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	ಪೋಷಕ_ಆರಂಭ = buf->buf;
	ಪೋಷಕ_ಆರಂಭ += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	ಪೋಷಕ_ಕೊನೆ = ಪೋಷಕ_ಆರಂಭ;

	ಆಗ (starts_with(ಪೋಷಕ_ಕೊನೆ, "parent "))
		ಪೋಷಕ_ಕೊನೆ += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ರಚನೆ object_id oid;
		ರಚನೆ commit *commit;

		ಆದರೆ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ಹಿಂತಿರುಗಿ error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ಆದರೆ (!commit) {
			strbuf_release(&new_parents);
			ಹಿಂತಿರುಗಿ error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_ಗಮ್ಯ_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, ಪೋಷಕ_ಆರಂಭ - buf->buf, ಪೋಷಕ_ಕೊನೆ - ಪೋಷಕ_ಆರಂಭ,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ check_mergetag_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ argc;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_one_mergetag(ರಚನೆ commit *commit ಬಳಸದ,
			       ರಚನೆ commit_extra_ಶೀರ್ಷಿಕೆ *extra,
			       ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ check_mergetag_ದತ್ತಾಂಶ *mergetag_ದತ್ತಾಂಶ = (ರಚನೆ check_mergetag_ದತ್ತಾಂಶ *)ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref = mergetag_ದತ್ತಾಂಶ->argv[0];
	ರಚನೆ object_id ಟ್ಯಾಗ್_oid;
	ರಚನೆ ಟ್ಯಾಗ್ *ಟ್ಯಾಗ್;
	ಪೂರ್ಣಾಂಕ i;

	hash_object_file(the_hash_algo, extra->ಮೌಲ್ಯ, extra->len,
			 OBJ_ಟ್ಯಾಗ್, &ಟ್ಯಾಗ್_oid);
	ಟ್ಯಾಗ್ = lookup_ಟ್ಯಾಗ್(the_repository, &ಟ್ಯಾಗ್_oid);
	ಆದರೆ (!ಟ್ಯಾಗ್)
		ಹಿಂತಿರುಗಿ error(_("bad mergetag in commit '%s'"), ref);
	ಆದರೆ (parse_ಟ್ಯಾಗ್_ಬಫರ್(the_repository, ಟ್ಯಾಗ್, extra->ಮೌಲ್ಯ, extra->len))
		ಹಿಂತಿರುಗಿ error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	ಗಾಗಿ (i = 1; i < mergetag_ದತ್ತಾಂಶ->argc; i++) {
		ರಚನೆ object_id oid;
		ಆದರೆ (repo_get_oid(the_repository, mergetag_ದತ್ತಾಂಶ->argv[i], &oid) < 0)
			ಹಿಂತಿರುಗಿ error(_("not a valid object name: '%s'"),
				     mergetag_ದತ್ತಾಂಶ->argv[i]);
		ಆದರೆ (oideq(get_tagged_oid(ಟ್ಯಾಗ್), &oid))
			ಹಿಂತಿರುಗಿ 0; /*ಸಿಕ್ಕಿತು*/
	}

	ಹಿಂತಿರುಗಿ error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_ಗಮ್ಯ_hex(&ಟ್ಯಾಗ್_oid));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_mergetags(ರಚನೆ commit *commit, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ರಚನೆ check_mergetag_ದತ್ತಾಂಶ mergetag_ದತ್ತಾಂಶ;

	mergetag_ದತ್ತಾಂಶ.argc = argc;
	mergetag_ದತ್ತಾಂಶ.argv = argv;
	ಹಿಂತಿರುಗಿ for_each_mergetag(check_one_mergetag, commit, &mergetag_ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ create_graft(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ force, ಪೂರ್ಣಾಂಕ gentle)
{
	ರಚನೆ object_id old_oid, new_oid;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *old_ref = argv[0];
	ರಚನೆ commit *commit;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2;

	ಆದರೆ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ಹಿಂತಿರುಗಿ error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ಆದರೆ (!commit)
		ಹಿಂತಿರುಗಿ error(_("could not parse %s"), old_ref);

	ಬಫರ್ = repo_get_commit_ಬಫರ್(the_repository, commit, &ಗಾತ್ರ_2);
	strbuf_ಕೂಡಿಸು(&buf, ಬಫರ್, ಗಾತ್ರ_2);
	repo_unuse_commit_ಬಫರ್(the_repository, commit, ಬಫರ್);

	ಆದರೆ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ಆದರೆ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ಹಿಂತಿರುಗಿ error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ಆದರೆ (oideq(&commit->object.oid, &new_oid)) {
		ಆದರೆ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_ಗಮ್ಯ_hex(&commit->object.oid));
			ಹಿಂತಿರುಗಿ 0;
		}
		ಹಿಂತಿರುಗಿ error(_("new commit is the same as the old one: '%s'"),
			     oid_ಗಮ್ಯ_hex(&commit->object.oid));
	}

	ಹಿಂತಿರುಗಿ replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ convert_graft_file(ಪೂರ್ಣಾಂಕ force)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ರಚನೆ strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ರಚನೆ strvec ವಾದಗಳು = STRVEC_INIT;

	ಆದರೆ (!fp)
		ಹಿಂತಿರುಗಿ -1;

	no_graft_file_deprecated_advice = 1;
	ಆಗ (strbuf_getline(&buf, fp) != EOF) {
		ಆದರೆ (*buf.buf == '#')
			ಮುಂದುವರಿ;

		strvec_ವಿಭಜಿಸು(&ವಾದಗಳು, buf.buf);
		ಆದರೆ (ವಾದಗಳು.nr && create_graft(ವಾದಗಳು.nr, ವಾದಗಳು.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&ವಾದಗಳು);
	}
	fclose(fp);

	strbuf_release(&buf);

	ಆದರೆ (!err.len)
		ಹಿಂತಿರುಗಿ unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ಹಿಂತಿರುಗಿ -1;
}

ಪೂರ್ಣಾಂಕ cmd_replace(ಪೂರ್ಣಾಂಕ argc,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ force = 0;
	ಪೂರ್ಣಾಂಕ raw = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format = ಖಾಲಿ;
	ಎಣಿಕೆ {
		ಬಹುಲಕ_UNSPECIFIED = 0,
		ಬಹುಲಕ_LIST,
		ಬಹುಲಕ_DELETE,
		ಬಹುಲಕ_EDIT,
		ಬಹುಲಕ_GRAFT,
		ಬಹುಲಕ_CONVERT_GRAFT_FILE,
		ಬಹುಲಕ_REPLACE
	} cmdmode = ಬಹುಲಕ_UNSPECIFIED;
	ರಚನೆ option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list replace refs"), ಬಹುಲಕ_LIST),
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete replace refs"), ಬಹುಲಕ_DELETE),
		OPT_CMDMODE('e', "edit", &cmdmode, N_("edit existing object"), ಬಹುಲಕ_EDIT),
		OPT_CMDMODE('g', "graft", &cmdmode, N_("change a commit's parents"), ಬಹುಲಕ_GRAFT),
		OPT_CMDMODE(0, "convert-graft-file", &cmdmode, N_("convert existing graft file"), ಬಹುಲಕ_CONVERT_GRAFT_FILE),
		OPT_BOOL_F('f', "force", &force, N_("replace the ref if it exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "raw", &raw, N_("do not pretty-print contents for --edit")),
		OPT_STRING(0, "format", &format, N_("format"), N_("use this format")),
		OPT_ಕೊನೆ()
	};

	disable_replace_refs();
	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_replace_usage, 0);

	ಆದರೆ (!cmdmode)
		cmdmode = argc ? ಬಹುಲಕ_REPLACE : ಬಹುಲಕ_LIST;

	ಆದರೆ (format && cmdmode != ಬಹುಲಕ_LIST)
		usage_ಸಂದೇಶ_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ಆದರೆ (force &&
	    cmdmode != ಬಹುಲಕ_REPLACE &&
	    cmdmode != ಬಹುಲಕ_EDIT &&
	    cmdmode != ಬಹುಲಕ_GRAFT &&
	    cmdmode != ಬಹುಲಕ_CONVERT_GRAFT_FILE)
		usage_ಸಂದೇಶ_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ಆದರೆ (raw && cmdmode != ಬಹುಲಕ_EDIT)
		usage_ಸಂದೇಶ_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	ಆಯ್ಕೆ (cmdmode) {
	ಸ್ಥಿತಿ ಬಹುಲಕ_DELETE:
		ಆದರೆ (argc < 1)
			usage_ಸಂದೇಶ_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ಹಿಂತಿರುಗಿ for_each_replace_name(argv, delete_replace_ref);

	ಸ್ಥಿತಿ ಬಹುಲಕ_REPLACE:
		ಆದರೆ (argc != 2)
			usage_ಸಂದೇಶ_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ಹಿಂತಿರುಗಿ replace_object(argv[0], argv[1], force);

	ಸ್ಥಿತಿ ಬಹುಲಕ_EDIT:
		ಆದರೆ (argc != 1)
			usage_ಸಂದೇಶ_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ಹಿಂತಿರುಗಿ edit_and_replace(argv[0], force, raw);

	ಸ್ಥಿತಿ ಬಹುಲಕ_GRAFT:
		ಆದರೆ (argc < 1)
			usage_ಸಂದೇಶ_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ಹಿಂತಿರುಗಿ create_graft(argc, argv, force, 0);

	ಸ್ಥಿತಿ ಬಹುಲಕ_CONVERT_GRAFT_FILE:
		ಆದರೆ (argc != 0)
			usage_ಸಂದೇಶ_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ಹಿಂತಿರುಗಿ !!convert_graft_file(force);

	ಸ್ಥಿತಿ ಬಹುಲಕ_LIST:
		ಆದರೆ (argc > 1)
			usage_ಸಂದೇಶ_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ಹಿಂತಿರುಗಿ list_replace_refs(argv[0], format);

	ಪೂರ್ವನಿಯೋಜಿತ:
		BUG("invalid cmdmode %d", (ಪೂರ್ಣಾಂಕ)cmdmode);
	}
}
