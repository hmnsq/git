/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#define USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ರಿಕ್ತ
};

ಗಣನಾ replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ಸಂರಚನಾ show_data {
	ಸಂರಚನಾ repository *repo;
	ಧ್ರುವ ವರ್ಣ *pattern;
	ಗಣನಾ replace_format format;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show_reference(ಧ್ರುವ ಸಂರಚನಾ reference *ref, ಶೂನ್ಯ *cb_data)
{
	ಸಂರಚನಾ show_data *data = cb_data;

	ಯದಿ (!wildmatch(data->pattern, ref->name, 0)) {
		ಯದಿ (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		ಅನ್ಯಥಾ ಯದಿ (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		ಅನ್ಯಥಾ { /* data->format == REPLACE_FORMAT_LONG */
			ಸಂರಚನಾ object_id object;
			ಗಣನಾ object_type obj_type, repl_type;

			ಯದಿ (repo_get_oid(data->repo, ref->name, &object))
				ಪ್ರತ್ಯಾಗಮ error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, ರಿಕ್ತ);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, ರಿಕ್ತ);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_replace_refs(ಧ್ರುವ ವರ್ಣ *pattern, ಧ್ರುವ ವರ್ಣ *format)
{
	ಸಂರಚನಾ show_data data;

	data.repo = the_repository;
	ಯದಿ (!pattern)
		pattern = "*";
	data.pattern = pattern;

	ಯದಿ (format == ರಿಕ್ತ || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	ಅನ್ಯಥಾ ಯದಿ (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (ಶೂನ್ಯ *)&data);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*each_replace_name_fn)(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *ref,
				    ಧ್ರುವ ಸಂರಚನಾ object_id *oid);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ for_each_replace_name(ಧ್ರುವ ವರ್ಣ **argv, each_replace_name_fn fn)
{
	ಧ್ರುವ ವರ್ಣ **p, *full_hex;
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;
	size_t base_len;
	ಪೂರ್ಣಾಙ್ಕ had_error = 0;
	ಸಂರಚನಾ object_id oid;
	ಧ್ರುವ ವರ್ಣ *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	ಪ್ರತಿ (p = argv; *p; p++) {
		ಯದಿ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			ಅನುವರ್ತ;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			ಅನುವರ್ತ;
		}
		ಯದಿ (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ಪ್ರತ್ಯಾಗಮ had_error;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ delete_replace_ref(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *ref,
			      ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	ಯದಿ (refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ, ref, oid, 0))
		ಪ್ರತ್ಯಾಗಮ 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_ref_valid(ಸಂರಚನಾ object_id *object,
			    ಸಂರಚನಾ object_id *prev,
			    ಸಂರಚನಾ strbuf *ref,
			    ಪೂರ್ಣಾಙ್ಕ force)
{
	ಧ್ರುವ ವರ್ಣ *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	ಯದಿ (check_refname_format(ref->buf, 0))
		ಪ್ರತ್ಯಾಗಮ error(_("'%s' is not a valid ref name"), ref->buf);

	ಯದಿ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	ಅನ್ಯಥಾ ಯದಿ (!force)
		ಪ್ರತ್ಯಾಗಮ error(_("replace ref '%s' already exists"), ref->buf);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ replace_object_oid(ಧ್ರುವ ವರ್ಣ *object_ref,
			       ಸಂರಚನಾ object_id *object,
			       ಧ್ರುವ ವರ್ಣ *replace_ref,
			       ಸಂರಚನಾ object_id *repl,
			       ಪೂರ್ಣಾಙ್ಕ force)
{
	ಸಂರಚನಾ object_id prev;
	ಗಣನಾ object_type obj_type, repl_type;
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;
	ಸಂರಚನಾ ref_transaction *transaction;
	ಸಂರಚನಾ strbuf err = STRBUF_INIT;
	ಪೂರ್ಣಾಙ್ಕ res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ರಿಕ್ತ);
	repl_type = odb_read_object_info(the_repository->objects, repl, ರಿಕ್ತ);
	ಯದಿ (!force && obj_type != repl_type)
		ಪ್ರತ್ಯಾಗಮ error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ಯದಿ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ಯದಿ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ರಿಕ್ತ, ರಿಕ್ತ, 0, ರಿಕ್ತ, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ಪ್ರತ್ಯಾಗಮ res;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ replace_object(ಧ್ರುವ ವರ್ಣ *object_ref, ಧ್ರುವ ವರ್ಣ *replace_ref, ಪೂರ್ಣಾಙ್ಕ force)
{
	ಸಂರಚನಾ object_id object, repl;

	ಯದಿ (repo_get_oid(the_repository, object_ref, &object))
		ಪ್ರತ್ಯಾಗಮ error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ಯದಿ (repo_get_oid(the_repository, replace_ref, &repl))
		ಪ್ರತ್ಯಾಗಮ error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ಪ್ರತ್ಯಾಗಮ replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ export_object(ಧ್ರುವ ಸಂರಚನಾ object_id *oid, ಗಣನಾ object_type type,
			  ಪೂರ್ಣಾಙ್ಕ raw, ಧ್ರುವ ವರ್ಣ *filename)
{
	ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
	ಪೂರ್ಣಾಙ್ಕ fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("unable to open %s for writing"), filename);

	strvec_ತಳ್ಳು(&cmd.args, "--no-replace-objects");
	strvec_ತಳ್ಳು(&cmd.args, "cat-file");
	ಯದಿ (raw)
		strvec_ತಳ್ಳು(&cmd.args, type_name(type));
	ಅನ್ಯಥಾ
		strvec_ತಳ್ಳು(&cmd.args, "-p");
	strvec_ತಳ್ಳು(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	ಯದಿ (run_command(&cmd))
		ಪ್ರತ್ಯಾಗಮ error(_("cat-file reported failure"));
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ import_object(ಸಂರಚನಾ object_id *oid, ಗಣನಾ object_type type,
			  ಪೂರ್ಣಾಙ್ಕ raw, ಧ್ರುವ ವರ್ಣ *filename)
{
	ಪೂರ್ಣಾಙ್ಕ fd;

	fd = open(filename, O_RDONLY);
	ಯದಿ (fd < 0)
		ಪ್ರತ್ಯಾಗಮ error_errno(_("unable to open %s for reading"), filename);

	ಯದಿ (!raw && type == OBJ_ವೃಕ್ಷ) {
		ಸಂರಚನಾ child_process cmd = CHILD_PROCESS_INIT;
		ಸಂರಚನಾ strbuf result = STRBUF_INIT;

		strvec_ತಳ್ಳು(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		ಯದಿ (start_command(&cmd)) {
			close(fd);
			ಪ್ರತ್ಯಾಗಮ error(_("unable to spawn mktree"));
		}

		ಯದಿ (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		close(cmd.out);

		ಯದಿ (finish_command(&cmd)) {
			strbuf_release(&result);
			ಪ್ರತ್ಯಾಗಮ error(_("mktree reported failure"));
		}
		ಯದಿ (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			ಪ್ರತ್ಯಾಗಮ error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ stat st;
		ಪೂರ್ಣಾಙ್ಕ flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		ಯದಿ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ಪ್ರತ್ಯಾಗಮ -1;
		}
		ಯದಿ (index_fd(the_repository->index, oid, fd, &st, type, ರಿಕ್ತ, flags) < 0)
			ಪ್ರತ್ಯಾಗಮ error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ edit_and_replace(ಧ್ರುವ ವರ್ಣ *object_ref, ಪೂರ್ಣಾಙ್ಕ force, ಪೂರ್ಣಾಙ್ಕ raw)
{
	ವರ್ಣ *tmpfile;
	ಗಣನಾ object_type type;
	ಸಂರಚನಾ object_id old_oid, new_oid, prev;
	ಸಂರಚನಾ strbuf ref = STRBUF_INIT;

	ಯದಿ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ರಿಕ್ತ);
	ಯದಿ (type < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	ಯದಿ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	ಯದಿ (export_object(&old_oid, type, raw, tmpfile)) {
		ಮುಕ್ತ(tmpfile);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಯದಿ (launch_editor(tmpfile, ರಿಕ್ತ, ರಿಕ್ತ) < 0) {
		ಮುಕ್ತ(tmpfile);
		ಪ್ರತ್ಯಾಗಮ error(_("editing object file failed"));
	}
	ಯದಿ (import_object(&new_oid, type, raw, tmpfile)) {
		ಮುಕ್ತ(tmpfile);
		ಪ್ರತ್ಯಾಗಮ -1;
	}
	ಮುಕ್ತ(tmpfile);

	ಯದಿ (oideq(&old_oid, &new_oid))
		ಪ್ರತ್ಯಾಗಮ error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	ಪ್ರತ್ಯಾಗಮ replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ replace_parents(ಸಂರಚನಾ strbuf *buf, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ strbuf new_parents = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *parent_start, *parent_end;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಧ್ರುವ unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	ಯಾವತ್ (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಸಂರಚನಾ object_id oid;
		ಸಂರಚನಾ commit *commit;

		ಯದಿ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ಪ್ರತ್ಯಾಗಮ error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ಯದಿ (!commit) {
			strbuf_release(&new_parents);
			ಪ್ರತ್ಯಾಗಮ error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ check_mergetag_data {
	ಪೂರ್ಣಾಙ್ಕ argc;
	ಧ್ರುವ ವರ್ಣ **argv;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_one_mergetag(ಸಂರಚನಾ commit *commit UNUSED,
			       ಸಂರಚನಾ commit_extra_header *extra,
			       ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ check_mergetag_data *mergetag_data = (ಸಂರಚನಾ check_mergetag_data *)data;
	ಧ್ರುವ ವರ್ಣ *ref = mergetag_data->argv[0];
	ಸಂರಚನಾ object_id tag_oid;
	ಸಂರಚನಾ tag *tag;
	ಪೂರ್ಣಾಙ್ಕ i;

	hash_object_file(the_hash_algo, extra->ಮೌಲ್ಯ, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	ಯದಿ (!tag)
		ಪ್ರತ್ಯಾಗಮ error(_("bad mergetag in commit '%s'"), ref);
	ಯದಿ (parse_tag_buffer(the_repository, tag, extra->ಮೌಲ್ಯ, extra->len))
		ಪ್ರತ್ಯಾಗಮ error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	ಪ್ರತಿ (i = 1; i < mergetag_data->argc; i++) {
		ಸಂರಚನಾ object_id oid;
		ಯದಿ (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			ಪ್ರತ್ಯಾಗಮ error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		ಯದಿ (oideq(get_tagged_oid(tag), &oid))
			ಪ್ರತ್ಯಾಗಮ 0; /* found */
	}

	ಪ್ರತ್ಯಾಗಮ error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_mergetags(ಸಂರಚನಾ commit *commit, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಸಂರಚನಾ check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	ಪ್ರತ್ಯಾಗಮ for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ create_graft(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ force, ಪೂರ್ಣಾಙ್ಕ gentle)
{
	ಸಂರಚನಾ object_id old_oid, new_oid;
	ಧ್ರುವ ವರ್ಣ *old_ref = argv[0];
	ಸಂರಚನಾ commit *commit;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಧ್ರುವ ವರ್ಣ *buffer;
	unsigned long size;

	ಯದಿ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ಪ್ರತ್ಯಾಗಮ error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ಯದಿ (!commit)
		ಪ್ರತ್ಯಾಗಮ error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	ಯದಿ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ಯದಿ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ಪ್ರತ್ಯಾಗಮ -1;
	}

	ಯದಿ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ಪ್ರತ್ಯಾಗಮ error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ಯದಿ (oideq(&commit->object.oid, &new_oid)) {
		ಯದಿ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		ಪ್ರತ್ಯಾಗಮ error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	ಪ್ರತ್ಯಾಗಮ replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ convert_graft_file(ಪೂರ್ಣಾಙ್ಕ force)
{
	ಧ್ರುವ ವರ್ಣ *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ಸಂರಚನಾ strvec args = STRVEC_INIT;

	ಯದಿ (!fp)
		ಪ್ರತ್ಯಾಗಮ -1;

	no_graft_file_deprecated_advice = 1;
	ಯಾವತ್ (strbuf_getline(&buf, fp) != EOF) {
		ಯದಿ (*buf.buf == '#')
			ಅನುವರ್ತ;

		strvec_split(&args, buf.buf);
		ಯದಿ (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	ಯದಿ (!err.len)
		ಪ್ರತ್ಯಾಗಮ unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ cmd_replace(ಪೂರ್ಣಾಙ್ಕ argc,
		ಧ್ರುವ ವರ್ಣ **argv,
		ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0;
	ಪೂರ್ಣಾಙ್ಕ raw = 0;
	ಧ್ರುವ ವರ್ಣ *format = ರಿಕ್ತ;
	ಗಣನಾ {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	ಸಂರಚನಾ option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list replace refs"), MODE_LIST),
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete replace refs"), MODE_DELETE),
		OPT_CMDMODE('e', "edit", &cmdmode, N_("edit existing object"), MODE_EDIT),
		OPT_CMDMODE('g', "graft", &cmdmode, N_("change a commit's parents"), MODE_GRAFT),
		OPT_CMDMODE(0, "convert-graft-file", &cmdmode, N_("convert existing graft file"), MODE_CONVERT_GRAFT_FILE),
		OPT_BOOL_F('f', "force", &force, N_("replace the ref if it exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "raw", &raw, N_("do not pretty-print contents for --edit")),
		OPT_STRING(0, "format", &format, N_("format"), N_("use this format")),
		OPT_END()
	};

	disable_replace_refs();
	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	ಯದಿ (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	ಯದಿ (format && cmdmode != MODE_LIST)
		usage_ಸಂದೇಶ_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ಯದಿ (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_ಸಂದೇಶ_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ಯದಿ (raw && cmdmode != MODE_EDIT)
		usage_ಸಂದೇಶ_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	ವಿಕಲ್ಪ (cmdmode) {
	ಪಕ್ಷ MODE_DELETE:
		ಯದಿ (argc < 1)
			usage_ಸಂದೇಶ_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ಪ್ರತ್ಯಾಗಮ for_each_replace_name(argv, delete_replace_ref);

	ಪಕ್ಷ MODE_REPLACE:
		ಯದಿ (argc != 2)
			usage_ಸಂದೇಶ_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ಪ್ರತ್ಯಾಗಮ replace_object(argv[0], argv[1], force);

	ಪಕ್ಷ MODE_EDIT:
		ಯದಿ (argc != 1)
			usage_ಸಂದೇಶ_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ಪ್ರತ್ಯಾಗಮ edit_and_replace(argv[0], force, raw);

	ಪಕ್ಷ MODE_GRAFT:
		ಯದಿ (argc < 1)
			usage_ಸಂದೇಶ_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ಪ್ರತ್ಯಾಗಮ create_graft(argc, argv, force, 0);

	ಪಕ್ಷ MODE_CONVERT_GRAFT_FILE:
		ಯದಿ (argc != 0)
			usage_ಸಂದೇಶ_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ಪ್ರತ್ಯಾಗಮ !!convert_graft_file(force);

	ಪಕ್ಷ MODE_LIST:
		ಯದಿ (argc > 1)
			usage_ಸಂದೇಶ_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ಪ್ರತ್ಯಾಗಮ list_replace_refs(argv[0], format);

	ಸ್ವಭಾವ:
		BUG("invalid cmdmode %d", (ಪೂರ್ಣಾಙ್ಕ)cmdmode);
	}
}
