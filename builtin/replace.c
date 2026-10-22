/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "replace-object.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tag.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "wildmatch.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ਨਿਰਾ
};

ਗਿਣਤੀ replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ਬਣਤਰ show_ਅੰਕੜੇ {
	ਬਣਤਰ repository *repo;
	ਅਚਲ ਅੱਖਰ *ਨਮੂਨਾ;
	ਗਿਣਤੀ replace_format format;
};

ਸਥਿਰ ਪੂਰਨ show_reference(ਅਚਲ ਬਣਤਰ reference *ref, ਖਾਲੀ *cb_ਅੰਕੜੇ)
{
	ਬਣਤਰ show_ਅੰਕੜੇ *ਅੰਕੜੇ = cb_ਅੰਕੜੇ;

	ਜੇ (!wildmatch(ਅੰਕੜੇ->ਨਮੂਨਾ, ref->name, 0)) {
		ਜੇ (ਅੰਕੜੇ->format == REPLACE_FORMAT_SHORT)
			ਲਿਖੋ("%s\n", ref->name);
		ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕੜੇ->format == REPLACE_FORMAT_MEDIUM)
			ਲਿਖੋ("%s -> %s\n", ref->name, oid_ਮੰਜ਼ਿਲ_hex(ref->oid));
		ਨਹੀਂਤਾਂ { /* data->format == REPLACE_FORMAT_LONG */
			ਬਣਤਰ object_id object;
			ਗਿਣਤੀ object_type obj_type, repl_type;

			ਜੇ (repo_get_oid(ਅੰਕੜੇ->repo, ref->name, &object))
				ਵਾਪਸ error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(ਅੰਕੜੇ->repo->objects, &object, ਨਿਰਾ);
			repl_type = odb_read_object_info(ਅੰਕੜੇ->repo->objects, ref->oid, ਨਿਰਾ);

			ਲਿਖੋ("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_ਮੰਜ਼ਿਲ_hex(ref->oid), type_name(repl_type));
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ list_replace_refs(ਅਚਲ ਅੱਖਰ *ਨਮੂਨਾ, ਅਚਲ ਅੱਖਰ *format)
{
	ਬਣਤਰ show_ਅੰਕੜੇ ਅੰਕੜੇ;

	ਅੰਕੜੇ.repo = the_repository;
	ਜੇ (!ਨਮੂਨਾ)
		ਨਮੂਨਾ = "*";
	ਅੰਕੜੇ.ਨਮੂਨਾ = ਨਮੂਨਾ;

	ਜੇ (format == ਨਿਰਾ || *format == '\0' || !strcmp(format, "short"))
		ਅੰਕੜੇ.format = REPLACE_FORMAT_SHORT;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(format, "medium"))
		ਅੰਕੜੇ.format = REPLACE_FORMAT_MEDIUM;
	ਨਹੀਂਤਾਂ ਜੇ (!strcmp(format, "long"))
		ਅੰਕੜੇ.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (ਖਾਲੀ *)&ਅੰਕੜੇ);

	ਵਾਪਸ 0;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*each_replace_name_fn)(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ref,
				    ਅਚਲ ਬਣਤਰ object_id *oid);

ਸਥਿਰ ਪੂਰਨ for_each_replace_name(ਅਚਲ ਅੱਖਰ **argv, each_replace_name_fn fn)
{
	ਅਚਲ ਅੱਖਰ **p, *ਭਰਿਆ_hex;
	ਬਣਤਰ strbuf ref = STRBUF_INIT;
	ਮਾਪ_t ਅਧਾਰ_len;
	ਪੂਰਨ had_error = 0;
	ਬਣਤਰ object_id oid;
	ਅਚਲ ਅੱਖਰ *git_replace_ref_ਅਧਾਰ = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_ਅਧਾਰ);
	ਅਧਾਰ_len = ref.len;

	ਲਈ (p = argv; *p; p++) {
		ਜੇ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			ਜਾਰੀ;
		}

		strbuf_setlen(&ref, ਅਧਾਰ_len);
		strbuf_ਜੋੜੋ_oid_hex(&ref, &oid);
		ਭਰਿਆ_hex = ref.buf + ਅਧਾਰ_len;

		ਜੇ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), ਭਰਿਆ_hex);
			had_error = 1;
			ਜਾਰੀ;
		}
		ਜੇ (fn(ਭਰਿਆ_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ਵਾਪਸ had_error;
}

ਸਥਿਰ ਪੂਰਨ delete_replace_ref(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ref,
			      ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਜੇ (refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ, ref, oid, 0))
		ਵਾਪਸ 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ check_ref_valid(ਬਣਤਰ object_id *object,
			    ਬਣਤਰ object_id *prev,
			    ਬਣਤਰ strbuf *ref,
			    ਪੂਰਨ force)
{
	ਅਚਲ ਅੱਖਰ *git_replace_ref_ਅਧਾਰ = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_ਅਧਾਰ, oid_ਮੰਜ਼ਿਲ_hex(object));
	ਜੇ (check_refname_format(ref->buf, 0))
		ਵਾਪਸ error(_("'%s' is not a valid ref name"), ref->buf);

	ਜੇ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	ਨਹੀਂਤਾਂ ਜੇ (!force)
		ਵਾਪਸ error(_("replace ref '%s' already exists"), ref->buf);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ replace_object_oid(ਅਚਲ ਅੱਖਰ *object_ref,
			       ਬਣਤਰ object_id *object,
			       ਅਚਲ ਅੱਖਰ *replace_ref,
			       ਬਣਤਰ object_id *repl,
			       ਪੂਰਨ force)
{
	ਬਣਤਰ object_id prev;
	ਗਿਣਤੀ object_type obj_type, repl_type;
	ਬਣਤਰ strbuf ref = STRBUF_INIT;
	ਬਣਤਰ ref_transaction *transaction;
	ਬਣਤਰ strbuf err = STRBUF_INIT;
	ਪੂਰਨ res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ਨਿਰਾ);
	repl_type = odb_read_object_info(the_repository->objects, repl, ਨਿਰਾ);
	ਜੇ (!force && obj_type != repl_type)
		ਵਾਪਸ error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ਜੇ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		ਵਾਪਸ -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ਜੇ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ਨਿਰਾ, ਨਿਰਾ, 0, ਨਿਰਾ, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ਵਾਪਸ res;
}

ਸਥਿਰ ਪੂਰਨ replace_object(ਅਚਲ ਅੱਖਰ *object_ref, ਅਚਲ ਅੱਖਰ *replace_ref, ਪੂਰਨ force)
{
	ਬਣਤਰ object_id object, repl;

	ਜੇ (repo_get_oid(the_repository, object_ref, &object))
		ਵਾਪਸ error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ਜੇ (repo_get_oid(the_repository, replace_ref, &repl))
		ਵਾਪਸ error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ਵਾਪਸ replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ਸਥਿਰ ਪੂਰਨ export_object(ਅਚਲ ਬਣਤਰ object_id *oid, ਗਿਣਤੀ object_type type,
			  ਪੂਰਨ raw, ਅਚਲ ਅੱਖਰ *filename)
{
	ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
	ਪੂਰਨ fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ਜੇ (fd < 0)
		ਵਾਪਸ error_errno(_("unable to open %s for writing"), filename);

	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "--no-replace-objects");
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "cat-file");
	ਜੇ (raw)
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, type_name(type));
	ਨਹੀਂਤਾਂ
		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "-p");
	strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, oid_ਮੰਜ਼ਿਲ_hex(oid));
	cmd.git_cmd = 1;
	cmd.ਬਾਹਰਲਾ = fd;

	ਜੇ (ਲੜੀਵਾਰ_command(&cmd))
		ਵਾਪਸ error(_("cat-file reported failure"));
	ਵਾਪਸ 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ਸਥਿਰ ਪੂਰਨ import_object(ਬਣਤਰ object_id *oid, ਗਿਣਤੀ object_type type,
			  ਪੂਰਨ raw, ਅਚਲ ਅੱਖਰ *filename)
{
	ਪੂਰਨ fd;

	fd = open(filename, O_RDONLY);
	ਜੇ (fd < 0)
		ਵਾਪਸ error_errno(_("unable to open %s for reading"), filename);

	ਜੇ (!raw && type == OBJ_ਰੁੱਖ) {
		ਬਣਤਰ ਬੱਚਾ_process cmd = ਬੱਚਾ_PROCESS_INIT;
		ਬਣਤਰ strbuf ਨਤੀਜਾ = STRBUF_INIT;

		strvec_ਧੱਕੋ(&cmd.ਦਲੀਲਾਂ, "mktree");
		cmd.git_cmd = 1;
		cmd.ਵਿੱਚ = fd;
		cmd.ਬਾਹਰਲਾ = -1;

		ਜੇ (ਸ਼ੁਰੂ_command(&cmd)) {
			close(fd);
			ਵਾਪਸ error(_("unable to spawn mktree"));
		}

		ਜੇ (strbuf_read(&ਨਤੀਜਾ, cmd.ਬਾਹਰਲਾ, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.ਬਾਹਰਲਾ);
			ਵਾਪਸ -1;
		}
		close(cmd.ਬਾਹਰਲਾ);

		ਜੇ (ਅੰਤ_command(&cmd)) {
			strbuf_release(&ਨਤੀਜਾ);
			ਵਾਪਸ error(_("mktree reported failure"));
		}
		ਜੇ (get_oid_hex(ਨਤੀਜਾ.buf, oid) < 0) {
			strbuf_release(&ਨਤੀਜਾ);
			ਵਾਪਸ error(_("mktree did not return an object name"));
		}

		strbuf_release(&ਨਤੀਜਾ);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ stat st;
		ਪੂਰਨ flags = ਸੂਚਕਾਂਕ_FORMAT_CHECK | ਸੂਚਕਾਂਕ_WRITE_OBJECT;

		ਜੇ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ਵਾਪਸ -1;
		}
		ਜੇ (ਸੂਚਕਾਂਕ_fd(the_repository->ਸੂਚਕਾਂਕ, oid, fd, &st, type, ਨਿਰਾ, flags) < 0)
			ਵਾਪਸ error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ edit_and_replace(ਅਚਲ ਅੱਖਰ *object_ref, ਪੂਰਨ force, ਪੂਰਨ raw)
{
	ਅੱਖਰ *tmpfile;
	ਗਿਣਤੀ object_type type;
	ਬਣਤਰ object_id old_oid, new_oid, prev;
	ਬਣਤਰ strbuf ref = STRBUF_INIT;

	ਜੇ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ਵਾਪਸ error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ਨਿਰਾ);
	ਜੇ (type < 0)
		ਵਾਪਸ error(_("unable to get object type for %s"),
			     oid_ਮੰਜ਼ਿਲ_hex(&old_oid));

	ਜੇ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		ਵਾਪਸ -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_ਰਸਤਾ(the_repository, "REPLACE_EDITOBJ");
	ਜੇ (export_object(&old_oid, type, raw, tmpfile)) {
		ਮੁਕਤ(tmpfile);
		ਵਾਪਸ -1;
	}
	ਜੇ (launch_editor(tmpfile, ਨਿਰਾ, ਨਿਰਾ) < 0) {
		ਮੁਕਤ(tmpfile);
		ਵਾਪਸ error(_("editing object file failed"));
	}
	ਜੇ (import_object(&new_oid, type, raw, tmpfile)) {
		ਮੁਕਤ(tmpfile);
		ਵਾਪਸ -1;
	}
	ਮੁਕਤ(tmpfile);

	ਜੇ (oideq(&old_oid, &new_oid))
		ਵਾਪਸ error(_("new object is the same as the old one: '%s'"), oid_ਮੰਜ਼ਿਲ_hex(&old_oid));

	ਵਾਪਸ replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ਸਥਿਰ ਪੂਰਨ replace_parents(ਬਣਤਰ strbuf *buf, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ strbuf new_parents = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_ਸ਼ੁਰੂ, *ਮਾਪਾ_ਸਿਰਾ;
	ਪੂਰਨ i;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	ਮਾਪਾ_ਸ਼ੁਰੂ = buf->buf;
	ਮਾਪਾ_ਸ਼ੁਰੂ += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	ਮਾਪਾ_ਸਿਰਾ = ਮਾਪਾ_ਸ਼ੁਰੂ;

	ਜਦੋਂ (starts_with(ਮਾਪਾ_ਸਿਰਾ, "parent "))
		ਮਾਪਾ_ਸਿਰਾ += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	ਲਈ (i = 0; i < argc; i++) {
		ਬਣਤਰ object_id oid;
		ਬਣਤਰ commit *commit;

		ਜੇ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ਵਾਪਸ error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ਜੇ (!commit) {
			strbuf_release(&new_parents);
			ਵਾਪਸ error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, ਮਾਪਾ_ਸ਼ੁਰੂ - buf->buf, ਮਾਪਾ_ਸਿਰਾ - ਮਾਪਾ_ਸ਼ੁਰੂ,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ਵਾਪਸ 0;
}

ਬਣਤਰ check_mergetag_ਅੰਕੜੇ {
	ਪੂਰਨ argc;
	ਅਚਲ ਅੱਖਰ **argv;
};

ਸਥਿਰ ਪੂਰਨ check_one_mergetag(ਬਣਤਰ commit *commit ਅਣਵਰਤਿਆ,
			       ਬਣਤਰ commit_extra_ਸਿਰਲੇਖ *extra,
			       ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ check_mergetag_ਅੰਕੜੇ *mergetag_ਅੰਕੜੇ = (ਬਣਤਰ check_mergetag_ਅੰਕੜੇ *)ਅੰਕੜੇ;
	ਅਚਲ ਅੱਖਰ *ref = mergetag_ਅੰਕੜੇ->argv[0];
	ਬਣਤਰ object_id ਟੈਗ_oid;
	ਬਣਤਰ ਟੈਗ *ਟੈਗ;
	ਪੂਰਨ i;

	hash_object_file(the_hash_algo, extra->ਮੁੱਲ, extra->len,
			 OBJ_ਟੈਗ, &ਟੈਗ_oid);
	ਟੈਗ = lookup_ਟੈਗ(the_repository, &ਟੈਗ_oid);
	ਜੇ (!ਟੈਗ)
		ਵਾਪਸ error(_("bad mergetag in commit '%s'"), ref);
	ਜੇ (parse_ਟੈਗ_ਬਫਰ(the_repository, ਟੈਗ, extra->ਮੁੱਲ, extra->len))
		ਵਾਪਸ error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	ਲਈ (i = 1; i < mergetag_ਅੰਕੜੇ->argc; i++) {
		ਬਣਤਰ object_id oid;
		ਜੇ (repo_get_oid(the_repository, mergetag_ਅੰਕੜੇ->argv[i], &oid) < 0)
			ਵਾਪਸ error(_("not a valid object name: '%s'"),
				     mergetag_ਅੰਕੜੇ->argv[i]);
		ਜੇ (oideq(get_tagged_oid(ਟੈਗ), &oid))
			ਵਾਪਸ 0; /*ਮਿਲਿਆ*/
	}

	ਵਾਪਸ error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_ਮੰਜ਼ਿਲ_hex(&ਟੈਗ_oid));
}

ਸਥਿਰ ਪੂਰਨ check_mergetags(ਬਣਤਰ commit *commit, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਬਣਤਰ check_mergetag_ਅੰਕੜੇ mergetag_ਅੰਕੜੇ;

	mergetag_ਅੰਕੜੇ.argc = argc;
	mergetag_ਅੰਕੜੇ.argv = argv;
	ਵਾਪਸ for_each_mergetag(check_one_mergetag, commit, &mergetag_ਅੰਕੜੇ);
}

ਸਥਿਰ ਪੂਰਨ create_graft(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਪੂਰਨ force, ਪੂਰਨ gentle)
{
	ਬਣਤਰ object_id old_oid, new_oid;
	ਅਚਲ ਅੱਖਰ *old_ref = argv[0];
	ਬਣਤਰ commit *commit;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਅਚਲ ਅੱਖਰ *ਬਫਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮਾਪ;

	ਜੇ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ਵਾਪਸ error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ਜੇ (!commit)
		ਵਾਪਸ error(_("could not parse %s"), old_ref);

	ਬਫਰ = repo_get_commit_ਬਫਰ(the_repository, commit, &ਮਾਪ);
	strbuf_ਜੋੜੋ(&buf, ਬਫਰ, ਮਾਪ);
	repo_unuse_commit_ਬਫਰ(the_repository, commit, ਬਫਰ);

	ਜੇ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ਵਾਪਸ -1;
	}

	ਜੇ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ਜੇ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ਵਾਪਸ -1;
	}

	ਜੇ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ਵਾਪਸ error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ਜੇ (oideq(&commit->object.oid, &new_oid)) {
		ਜੇ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
			ਵਾਪਸ 0;
		}
		ਵਾਪਸ error(_("new commit is the same as the old one: '%s'"),
			     oid_ਮੰਜ਼ਿਲ_hex(&commit->object.oid));
	}

	ਵਾਪਸ replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ਸਥਿਰ ਪੂਰਨ convert_graft_file(ਪੂਰਨ force)
{
	ਅਚਲ ਅੱਖਰ *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ਬਣਤਰ strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ਬਣਤਰ strvec ਦਲੀਲਾਂ = STRVEC_INIT;

	ਜੇ (!fp)
		ਵਾਪਸ -1;

	no_graft_file_deprecated_advice = 1;
	ਜਦੋਂ (strbuf_getline(&buf, fp) != EOF) {
		ਜੇ (*buf.buf == '#')
			ਜਾਰੀ;

		strvec_ਵੰਡੋ(&ਦਲੀਲਾਂ, buf.buf);
		ਜੇ (ਦਲੀਲਾਂ.nr && create_graft(ਦਲੀਲਾਂ.nr, ਦਲੀਲਾਂ.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&ਦਲੀਲਾਂ);
	}
	fclose(fp);

	strbuf_release(&buf);

	ਜੇ (!err.len)
		ਵਾਪਸ unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ਵਾਪਸ -1;
}

ਪੂਰਨ cmd_replace(ਪੂਰਨ argc,
		ਅਚਲ ਅੱਖਰ **argv,
		ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ force = 0;
	ਪੂਰਨ raw = 0;
	ਅਚਲ ਅੱਖਰ *format = ਨਿਰਾ;
	ਗਿਣਤੀ {
		ਬਹੁਲਕ_UNSPECIFIED = 0,
		ਬਹੁਲਕ_LIST,
		ਬਹੁਲਕ_DELETE,
		ਬਹੁਲਕ_EDIT,
		ਬਹੁਲਕ_GRAFT,
		ਬਹੁਲਕ_CONVERT_GRAFT_FILE,
		ਬਹੁਲਕ_REPLACE
	} cmdmode = ਬਹੁਲਕ_UNSPECIFIED;
	ਬਣਤਰ option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list replace refs"), ਬਹੁਲਕ_LIST),
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete replace refs"), ਬਹੁਲਕ_DELETE),
		OPT_CMDMODE('e', "edit", &cmdmode, N_("edit existing object"), ਬਹੁਲਕ_EDIT),
		OPT_CMDMODE('g', "graft", &cmdmode, N_("change a commit's parents"), ਬਹੁਲਕ_GRAFT),
		OPT_CMDMODE(0, "convert-graft-file", &cmdmode, N_("convert existing graft file"), ਬਹੁਲਕ_CONVERT_GRAFT_FILE),
		OPT_BOOL_F('f', "force", &force, N_("replace the ref if it exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "raw", &raw, N_("do not pretty-print contents for --edit")),
		OPT_STRING(0, "format", &format, N_("format"), N_("use this format")),
		OPT_ਸਿਰਾ()
	};

	disable_replace_refs();
	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_replace_usage, 0);

	ਜੇ (!cmdmode)
		cmdmode = argc ? ਬਹੁਲਕ_REPLACE : ਬਹੁਲਕ_LIST;

	ਜੇ (format && cmdmode != ਬਹੁਲਕ_LIST)
		usage_ਸੁਨੇਹਾ_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ਜੇ (force &&
	    cmdmode != ਬਹੁਲਕ_REPLACE &&
	    cmdmode != ਬਹੁਲਕ_EDIT &&
	    cmdmode != ਬਹੁਲਕ_GRAFT &&
	    cmdmode != ਬਹੁਲਕ_CONVERT_GRAFT_FILE)
		usage_ਸੁਨੇਹਾ_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ਜੇ (raw && cmdmode != ਬਹੁਲਕ_EDIT)
		usage_ਸੁਨੇਹਾ_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	ਚੋਣ (cmdmode) {
	ਹਾਲਤ ਬਹੁਲਕ_DELETE:
		ਜੇ (argc < 1)
			usage_ਸੁਨੇਹਾ_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ਵਾਪਸ for_each_replace_name(argv, delete_replace_ref);

	ਹਾਲਤ ਬਹੁਲਕ_REPLACE:
		ਜੇ (argc != 2)
			usage_ਸੁਨੇਹਾ_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ਵਾਪਸ replace_object(argv[0], argv[1], force);

	ਹਾਲਤ ਬਹੁਲਕ_EDIT:
		ਜੇ (argc != 1)
			usage_ਸੁਨੇਹਾ_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ਵਾਪਸ edit_and_replace(argv[0], force, raw);

	ਹਾਲਤ ਬਹੁਲਕ_GRAFT:
		ਜੇ (argc < 1)
			usage_ਸੁਨੇਹਾ_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ਵਾਪਸ create_graft(argc, argv, force, 0);

	ਹਾਲਤ ਬਹੁਲਕ_CONVERT_GRAFT_FILE:
		ਜੇ (argc != 0)
			usage_ਸੁਨੇਹਾ_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ਵਾਪਸ !!convert_graft_file(force);

	ਹਾਲਤ ਬਹੁਲਕ_LIST:
		ਜੇ (argc > 1)
			usage_ਸੁਨੇਹਾ_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ਵਾਪਸ list_replace_refs(argv[0], format);

	ਮੂਲ:
		BUG("invalid cmdmode %d", (ਪੂਰਨ)cmdmode);
	}
}
