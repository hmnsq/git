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
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "path.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "replace-object.h"
#അന്തര്ഭാവ "tag.h"
#അന്തര്ഭാവ "wildmatch.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ശൂന്യ
};

എണ്ണൽ replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ഘടന show_data {
	ഘടന repository *repo;
	സ്ഥിരം അക്ഷരം *pattern;
	എണ്ണൽ replace_format format;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ show_reference(സ്ഥിരം ഘടന reference *ref, ശൂന്യം *cb_data)
{
	ഘടന show_data *data = cb_data;

	എങ്കിൽ (!wildmatch(data->pattern, ref->name, 0)) {
		എങ്കിൽ (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		അല്ലെങ്കിൽ എങ്കിൽ (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		അല്ലെങ്കിൽ { /* data->format == REPLACE_FORMAT_LONG */
			ഘടന object_id object;
			എണ്ണൽ object_type obj_type, repl_type;

			എങ്കിൽ (repo_get_oid(data->repo, ref->name, &object))
				തിരികെ error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, ശൂന്യ);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, ശൂന്യ);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ list_replace_refs(സ്ഥിരം അക്ഷരം *pattern, സ്ഥിരം അക്ഷരം *format)
{
	ഘടന show_data data;

	data.repo = the_repository;
	എങ്കിൽ (!pattern)
		pattern = "*";
	data.pattern = pattern;

	എങ്കിൽ (format == ശൂന്യ || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	അല്ലെങ്കിൽ എങ്കിൽ (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	അല്ലെങ്കിൽ
		തിരികെ error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (ശൂന്യം *)&data);

	തിരികെ 0;
}

തരനിർവചനം പൂർണ്ണസംഖ്യ (*each_replace_name_fn)(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *ref,
				    സ്ഥിരം ഘടന object_id *oid);

സ്ഥിതം പൂർണ്ണസംഖ്യ for_each_replace_name(സ്ഥിരം അക്ഷരം **argv, each_replace_name_fn fn)
{
	സ്ഥിരം അക്ഷരം **p, *full_hex;
	ഘടന strbuf ref = STRBUF_INIT;
	size_t base_len;
	പൂർണ്ണസംഖ്യ had_error = 0;
	ഘടന object_id oid;
	സ്ഥിരം അക്ഷരം *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	വേണ്ടി (p = argv; *p; p++) {
		എങ്കിൽ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			തുടരുക;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			തുടരുക;
		}
		എങ്കിൽ (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	തിരികെ had_error;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ delete_replace_ref(സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *ref,
			      സ്ഥിരം ഘടന object_id *oid)
{
	എങ്കിൽ (refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ, ref, oid, 0))
		തിരികെ 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_ref_valid(ഘടന object_id *object,
			    ഘടന object_id *prev,
			    ഘടന strbuf *ref,
			    പൂർണ്ണസംഖ്യ force)
{
	സ്ഥിരം അക്ഷരം *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	എങ്കിൽ (check_refname_format(ref->buf, 0))
		തിരികെ error(_("'%s' is not a valid ref name"), ref->buf);

	എങ്കിൽ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	അല്ലെങ്കിൽ എങ്കിൽ (!force)
		തിരികെ error(_("replace ref '%s' already exists"), ref->buf);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ replace_object_oid(സ്ഥിരം അക്ഷരം *object_ref,
			       ഘടന object_id *object,
			       സ്ഥിരം അക്ഷരം *replace_ref,
			       ഘടന object_id *repl,
			       പൂർണ്ണസംഖ്യ force)
{
	ഘടന object_id prev;
	എണ്ണൽ object_type obj_type, repl_type;
	ഘടന strbuf ref = STRBUF_INIT;
	ഘടന ref_transaction *transaction;
	ഘടന strbuf err = STRBUF_INIT;
	പൂർണ്ണസംഖ്യ res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ശൂന്യ);
	repl_type = odb_read_object_info(the_repository->objects, repl, ശൂന്യ);
	എങ്കിൽ (!force && obj_type != repl_type)
		തിരികെ error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	എങ്കിൽ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		തിരികെ -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	എങ്കിൽ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ശൂന്യ, ശൂന്യ, 0, ശൂന്യ, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	തിരികെ res;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ replace_object(സ്ഥിരം അക്ഷരം *object_ref, സ്ഥിരം അക്ഷരം *replace_ref, പൂർണ്ണസംഖ്യ force)
{
	ഘടന object_id object, repl;

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &object))
		തിരികെ error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	എങ്കിൽ (repo_get_oid(the_repository, replace_ref, &repl))
		തിരികെ error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	തിരികെ replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ export_object(സ്ഥിരം ഘടന object_id *oid, എണ്ണൽ object_type type,
			  പൂർണ്ണസംഖ്യ raw, സ്ഥിരം അക്ഷരം *filename)
{
	ഘടന child_process cmd = CHILD_PROCESS_INIT;
	പൂർണ്ണസംഖ്യ fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	എങ്കിൽ (fd < 0)
		തിരികെ error_errno(_("unable to open %s for writing"), filename);

	strvec_തള്ളുക(&cmd.args, "--no-replace-objects");
	strvec_തള്ളുക(&cmd.args, "cat-file");
	എങ്കിൽ (raw)
		strvec_തള്ളുക(&cmd.args, type_name(type));
	അല്ലെങ്കിൽ
		strvec_തള്ളുക(&cmd.args, "-p");
	strvec_തള്ളുക(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	എങ്കിൽ (run_command(&cmd))
		തിരികെ error(_("cat-file reported failure"));
	തിരികെ 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ import_object(ഘടന object_id *oid, എണ്ണൽ object_type type,
			  പൂർണ്ണസംഖ്യ raw, സ്ഥിരം അക്ഷരം *filename)
{
	പൂർണ്ണസംഖ്യ fd;

	fd = open(filename, O_RDONLY);
	എങ്കിൽ (fd < 0)
		തിരികെ error_errno(_("unable to open %s for reading"), filename);

	എങ്കിൽ (!raw && type == OBJ_വൃക്ഷം) {
		ഘടന child_process cmd = CHILD_PROCESS_INIT;
		ഘടന strbuf result = STRBUF_INIT;

		strvec_തള്ളുക(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		എങ്കിൽ (start_command(&cmd)) {
			close(fd);
			തിരികെ error(_("unable to spawn mktree"));
		}

		എങ്കിൽ (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			തിരികെ -1;
		}
		close(cmd.out);

		എങ്കിൽ (finish_command(&cmd)) {
			strbuf_release(&result);
			തിരികെ error(_("mktree reported failure"));
		}
		എങ്കിൽ (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			തിരികെ error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} അല്ലെങ്കിൽ {
		ഘടന stat st;
		പൂർണ്ണസംഖ്യ flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		എങ്കിൽ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			തിരികെ -1;
		}
		എങ്കിൽ (index_fd(the_repository->index, oid, fd, &st, type, ശൂന്യ, flags) < 0)
			തിരികെ error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ edit_and_replace(സ്ഥിരം അക്ഷരം *object_ref, പൂർണ്ണസംഖ്യ force, പൂർണ്ണസംഖ്യ raw)
{
	അക്ഷരം *tmpfile;
	എണ്ണൽ object_type type;
	ഘടന object_id old_oid, new_oid, prev;
	ഘടന strbuf ref = STRBUF_INIT;

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		തിരികെ error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ശൂന്യ);
	എങ്കിൽ (type < 0)
		തിരികെ error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	എങ്കിൽ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		തിരികെ -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	എങ്കിൽ (export_object(&old_oid, type, raw, tmpfile)) {
		സ്വതന്ത്രം(tmpfile);
		തിരികെ -1;
	}
	എങ്കിൽ (launch_editor(tmpfile, ശൂന്യ, ശൂന്യ) < 0) {
		സ്വതന്ത്രം(tmpfile);
		തിരികെ error(_("editing object file failed"));
	}
	എങ്കിൽ (import_object(&new_oid, type, raw, tmpfile)) {
		സ്വതന്ത്രം(tmpfile);
		തിരികെ -1;
	}
	സ്വതന്ത്രം(tmpfile);

	എങ്കിൽ (oideq(&old_oid, &new_oid))
		തിരികെ error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	തിരികെ replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ replace_parents(ഘടന strbuf *buf, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന strbuf new_parents = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *parent_start, *parent_end;
	പൂർണ്ണസംഖ്യ i;
	സ്ഥിരം unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	ആകുമ്പോൾ (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	വേണ്ടി (i = 0; i < argc; i++) {
		ഘടന object_id oid;
		ഘടന commit *commit;

		എങ്കിൽ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			തിരികെ error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		എങ്കിൽ (!commit) {
			strbuf_release(&new_parents);
			തിരികെ error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	തിരികെ 0;
}

ഘടന check_mergetag_data {
	പൂർണ്ണസംഖ്യ argc;
	സ്ഥിരം അക്ഷരം **argv;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ check_one_mergetag(ഘടന commit *commit UNUSED,
			       ഘടന commit_extra_header *extra,
			       ശൂന്യം *data)
{
	ഘടന check_mergetag_data *mergetag_data = (ഘടന check_mergetag_data *)data;
	സ്ഥിരം അക്ഷരം *ref = mergetag_data->argv[0];
	ഘടന object_id tag_oid;
	ഘടന tag *tag;
	പൂർണ്ണസംഖ്യ i;

	hash_object_file(the_hash_algo, extra->മൂല്യം, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	എങ്കിൽ (!tag)
		തിരികെ error(_("bad mergetag in commit '%s'"), ref);
	എങ്കിൽ (parse_tag_buffer(the_repository, tag, extra->മൂല്യം, extra->len))
		തിരികെ error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	വേണ്ടി (i = 1; i < mergetag_data->argc; i++) {
		ഘടന object_id oid;
		എങ്കിൽ (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			തിരികെ error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		എങ്കിൽ (oideq(get_tagged_oid(tag), &oid))
			തിരികെ 0; /* found */
	}

	തിരികെ error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_mergetags(ഘടന commit *commit, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	ഘടന check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	തിരികെ for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ create_graft(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, പൂർണ്ണസംഖ്യ force, പൂർണ്ണസംഖ്യ gentle)
{
	ഘടന object_id old_oid, new_oid;
	സ്ഥിരം അക്ഷരം *old_ref = argv[0];
	ഘടന commit *commit;
	ഘടന strbuf buf = STRBUF_INIT;
	സ്ഥിരം അക്ഷരം *buffer;
	unsigned long size;

	എങ്കിൽ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		തിരികെ error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	എങ്കിൽ (!commit)
		തിരികെ error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	എങ്കിൽ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		തിരികെ -1;
	}

	എങ്കിൽ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	എങ്കിൽ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		തിരികെ -1;
	}

	എങ്കിൽ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		തിരികെ error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	എങ്കിൽ (oideq(&commit->object.oid, &new_oid)) {
		എങ്കിൽ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			തിരികെ 0;
		}
		തിരികെ error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	തിരികെ replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ convert_graft_file(പൂർണ്ണസംഖ്യ force)
{
	സ്ഥിരം അക്ഷരം *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ഘടന strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ഘടന strvec args = STRVEC_INIT;

	എങ്കിൽ (!fp)
		തിരികെ -1;

	no_graft_file_deprecated_advice = 1;
	ആകുമ്പോൾ (strbuf_getline(&buf, fp) != EOF) {
		എങ്കിൽ (*buf.buf == '#')
			തുടരുക;

		strvec_split(&args, buf.buf);
		എങ്കിൽ (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	എങ്കിൽ (!err.len)
		തിരികെ unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	തിരികെ -1;
}

പൂർണ്ണസംഖ്യ cmd_replace(പൂർണ്ണസംഖ്യ argc,
		സ്ഥിരം അക്ഷരം **argv,
		സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ force = 0;
	പൂർണ്ണസംഖ്യ raw = 0;
	സ്ഥിരം അക്ഷരം *format = ശൂന്യ;
	എണ്ണൽ {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	ഘടന option options[] = {
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
	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	എങ്കിൽ (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	എങ്കിൽ (format && cmdmode != MODE_LIST)
		usage_സന്ദേശം_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	എങ്കിൽ (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_സന്ദേശം_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	എങ്കിൽ (raw && cmdmode != MODE_EDIT)
		usage_സന്ദേശം_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	തിരഞ്ഞെടുപ്പ് (cmdmode) {
	അവസ്ഥ MODE_DELETE:
		എങ്കിൽ (argc < 1)
			usage_സന്ദേശം_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		തിരികെ for_each_replace_name(argv, delete_replace_ref);

	അവസ്ഥ MODE_REPLACE:
		എങ്കിൽ (argc != 2)
			usage_സന്ദേശം_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		തിരികെ replace_object(argv[0], argv[1], force);

	അവസ്ഥ MODE_EDIT:
		എങ്കിൽ (argc != 1)
			usage_സന്ദേശം_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		തിരികെ edit_and_replace(argv[0], force, raw);

	അവസ്ഥ MODE_GRAFT:
		എങ്കിൽ (argc < 1)
			usage_സന്ദേശം_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		തിരികെ create_graft(argc, argv, force, 0);

	അവസ്ഥ MODE_CONVERT_GRAFT_FILE:
		എങ്കിൽ (argc != 0)
			usage_സന്ദേശം_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		തിരികെ !!convert_graft_file(force);

	അവസ്ഥ MODE_LIST:
		എങ്കിൽ (argc > 1)
			usage_സന്ദേശം_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		തിരികെ list_replace_refs(argv[0], format);

	സ്ഥിരസ്ഥിതി:
		BUG("invalid cmdmode %d", (പൂർണ്ണസംഖ്യ)cmdmode);
	}
}
