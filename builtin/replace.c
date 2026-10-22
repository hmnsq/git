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
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "path.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "replace-object.h"
#অন্তর্ভাব "tag.h"
#অন্তর্ভাব "wildmatch.h"

স্থির ধ্রুব বর্ণ * ধ্রুব git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	রিক্ত
};

গণনা replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

সংরচনা show_data {
	সংরচনা repository *repo;
	ধ্রুব বর্ণ *pattern;
	গণনা replace_format format;
};

স্থির পূর্ণাঙ্ক show_reference(ধ্রুব সংরচনা reference *ref, শূন্য *cb_data)
{
	সংরচনা show_data *data = cb_data;

	যদি (!wildmatch(data->pattern, ref->name, 0)) {
		যদি (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		অন্যথা যদি (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		অন্যথা { /* data->format == REPLACE_FORMAT_LONG */
			সংরচনা object_id object;
			গণনা object_type obj_type, repl_type;

			যদি (repo_get_oid(data->repo, ref->name, &object))
				প্রত্যাগম error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, রিক্ত);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, রিক্ত);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক list_replace_refs(ধ্রুব বর্ণ *pattern, ধ্রুব বর্ণ *format)
{
	সংরচনা show_data data;

	data.repo = the_repository;
	যদি (!pattern)
		pattern = "*";
	data.pattern = pattern;

	যদি (format == রিক্ত || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	অন্যথা যদি (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	অন্যথা যদি (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	অন্যথা
		প্রত্যাগম error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (শূন্য *)&data);

	প্রত্যাগম 0;
}

প্রকারনির্দেশ পূর্ণাঙ্ক (*each_replace_name_fn)(ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *ref,
				    ধ্রুব সংরচনা object_id *oid);

স্থির পূর্ণাঙ্ক for_each_replace_name(ধ্রুব বর্ণ **argv, each_replace_name_fn fn)
{
	ধ্রুব বর্ণ **p, *full_hex;
	সংরচনা strbuf ref = STRBUF_INIT;
	size_t base_len;
	পূর্ণাঙ্ক had_error = 0;
	সংরচনা object_id oid;
	ধ্রুব বর্ণ *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	প্রতি (p = argv; *p; p++) {
		যদি (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			অনুবর্ত;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		যদি (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			অনুবর্ত;
		}
		যদি (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	প্রত্যাগম had_error;
}

স্থির পূর্ণাঙ্ক delete_replace_ref(ধ্রুব বর্ণ *name, ধ্রুব বর্ণ *ref,
			      ধ্রুব সংরচনা object_id *oid)
{
	যদি (refs_delete_ref(get_main_ref_store(the_repository), রিক্ত, ref, oid, 0))
		প্রত্যাগম 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক check_ref_valid(সংরচনা object_id *object,
			    সংরচনা object_id *prev,
			    সংরচনা strbuf *ref,
			    পূর্ণাঙ্ক force)
{
	ধ্রুব বর্ণ *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	যদি (check_refname_format(ref->buf, 0))
		প্রত্যাগম error(_("'%s' is not a valid ref name"), ref->buf);

	যদি (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	অন্যথা যদি (!force)
		প্রত্যাগম error(_("replace ref '%s' already exists"), ref->buf);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক replace_object_oid(ধ্রুব বর্ণ *object_ref,
			       সংরচনা object_id *object,
			       ধ্রুব বর্ণ *replace_ref,
			       সংরচনা object_id *repl,
			       পূর্ণাঙ্ক force)
{
	সংরচনা object_id prev;
	গণনা object_type obj_type, repl_type;
	সংরচনা strbuf ref = STRBUF_INIT;
	সংরচনা ref_transaction *transaction;
	সংরচনা strbuf err = STRBUF_INIT;
	পূর্ণাঙ্ক res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, রিক্ত);
	repl_type = odb_read_object_info(the_repository->objects, repl, রিক্ত);
	যদি (!force && obj_type != repl_type)
		প্রত্যাগম error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	যদি (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		প্রত্যাগম -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   রিক্ত, রিক্ত, 0, রিক্ত, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	প্রত্যাগম res;
}

স্থির পূর্ণাঙ্ক replace_object(ধ্রুব বর্ণ *object_ref, ধ্রুব বর্ণ *replace_ref, পূর্ণাঙ্ক force)
{
	সংরচনা object_id object, repl;

	যদি (repo_get_oid(the_repository, object_ref, &object))
		প্রত্যাগম error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	যদি (repo_get_oid(the_repository, replace_ref, &repl))
		প্রত্যাগম error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	প্রত্যাগম replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
স্থির পূর্ণাঙ্ক export_object(ধ্রুব সংরচনা object_id *oid, গণনা object_type type,
			  পূর্ণাঙ্ক raw, ধ্রুব বর্ণ *filename)
{
	সংরচনা child_process cmd = CHILD_PROCESS_INIT;
	পূর্ণাঙ্ক fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	যদি (fd < 0)
		প্রত্যাগম error_errno(_("unable to open %s for writing"), filename);

	strvec_ঠেলো(&cmd.args, "--no-replace-objects");
	strvec_ঠেলো(&cmd.args, "cat-file");
	যদি (raw)
		strvec_ঠেলো(&cmd.args, type_name(type));
	অন্যথা
		strvec_ঠেলো(&cmd.args, "-p");
	strvec_ঠেলো(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	যদি (run_command(&cmd))
		প্রত্যাগম error(_("cat-file reported failure"));
	প্রত্যাগম 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
স্থির পূর্ণাঙ্ক import_object(সংরচনা object_id *oid, গণনা object_type type,
			  পূর্ণাঙ্ক raw, ধ্রুব বর্ণ *filename)
{
	পূর্ণাঙ্ক fd;

	fd = open(filename, O_RDONLY);
	যদি (fd < 0)
		প্রত্যাগম error_errno(_("unable to open %s for reading"), filename);

	যদি (!raw && type == OBJ_বৃক্ষ) {
		সংরচনা child_process cmd = CHILD_PROCESS_INIT;
		সংরচনা strbuf result = STRBUF_INIT;

		strvec_ঠেলো(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		যদি (start_command(&cmd)) {
			close(fd);
			প্রত্যাগম error(_("unable to spawn mktree"));
		}

		যদি (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			প্রত্যাগম -1;
		}
		close(cmd.out);

		যদি (finish_command(&cmd)) {
			strbuf_release(&result);
			প্রত্যাগম error(_("mktree reported failure"));
		}
		যদি (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			প্রত্যাগম error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} অন্যথা {
		সংরচনা stat st;
		পূর্ণাঙ্ক flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		যদি (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			প্রত্যাগম -1;
		}
		যদি (index_fd(the_repository->index, oid, fd, &st, type, রিক্ত, flags) < 0)
			প্রত্যাগম error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক edit_and_replace(ধ্রুব বর্ণ *object_ref, পূর্ণাঙ্ক force, পূর্ণাঙ্ক raw)
{
	বর্ণ *tmpfile;
	গণনা object_type type;
	সংরচনা object_id old_oid, new_oid, prev;
	সংরচনা strbuf ref = STRBUF_INIT;

	যদি (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		প্রত্যাগম error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, রিক্ত);
	যদি (type < 0)
		প্রত্যাগম error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	যদি (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		প্রত্যাগম -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	যদি (export_object(&old_oid, type, raw, tmpfile)) {
		মুক্ত(tmpfile);
		প্রত্যাগম -1;
	}
	যদি (launch_editor(tmpfile, রিক্ত, রিক্ত) < 0) {
		মুক্ত(tmpfile);
		প্রত্যাগম error(_("editing object file failed"));
	}
	যদি (import_object(&new_oid, type, raw, tmpfile)) {
		মুক্ত(tmpfile);
		প্রত্যাগম -1;
	}
	মুক্ত(tmpfile);

	যদি (oideq(&old_oid, &new_oid))
		প্রত্যাগম error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	প্রত্যাগম replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

স্থির পূর্ণাঙ্ক replace_parents(সংরচনা strbuf *buf, পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	সংরচনা strbuf new_parents = STRBUF_INIT;
	ধ্রুব বর্ণ *parent_start, *parent_end;
	পূর্ণাঙ্ক i;
	ধ্রুব unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	যাবত্ (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	প্রতি (i = 0; i < argc; i++) {
		সংরচনা object_id oid;
		সংরচনা commit *commit;

		যদি (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			প্রত্যাগম error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		যদি (!commit) {
			strbuf_release(&new_parents);
			প্রত্যাগম error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	প্রত্যাগম 0;
}

সংরচনা check_mergetag_data {
	পূর্ণাঙ্ক argc;
	ধ্রুব বর্ণ **argv;
};

স্থির পূর্ণাঙ্ক check_one_mergetag(সংরচনা commit *commit UNUSED,
			       সংরচনা commit_extra_header *extra,
			       শূন্য *data)
{
	সংরচনা check_mergetag_data *mergetag_data = (সংরচনা check_mergetag_data *)data;
	ধ্রুব বর্ণ *ref = mergetag_data->argv[0];
	সংরচনা object_id tag_oid;
	সংরচনা tag *tag;
	পূর্ণাঙ্ক i;

	hash_object_file(the_hash_algo, extra->মান, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	যদি (!tag)
		প্রত্যাগম error(_("bad mergetag in commit '%s'"), ref);
	যদি (parse_tag_buffer(the_repository, tag, extra->মান, extra->len))
		প্রত্যাগম error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	প্রতি (i = 1; i < mergetag_data->argc; i++) {
		সংরচনা object_id oid;
		যদি (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			প্রত্যাগম error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		যদি (oideq(get_tagged_oid(tag), &oid))
			প্রত্যাগম 0; /* found */
	}

	প্রত্যাগম error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

স্থির পূর্ণাঙ্ক check_mergetags(সংরচনা commit *commit, পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv)
{
	সংরচনা check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	প্রত্যাগম for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

স্থির পূর্ণাঙ্ক create_graft(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, পূর্ণাঙ্ক force, পূর্ণাঙ্ক gentle)
{
	সংরচনা object_id old_oid, new_oid;
	ধ্রুব বর্ণ *old_ref = argv[0];
	সংরচনা commit *commit;
	সংরচনা strbuf buf = STRBUF_INIT;
	ধ্রুব বর্ণ *buffer;
	unsigned long size;

	যদি (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		প্রত্যাগম error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	যদি (!commit)
		প্রত্যাগম error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	যদি (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		প্রত্যাগম -1;
	}

	যদি (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	যদি (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		প্রত্যাগম -1;
	}

	যদি (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		প্রত্যাগম error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	যদি (oideq(&commit->object.oid, &new_oid)) {
		যদি (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			প্রত্যাগম 0;
		}
		প্রত্যাগম error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	প্রত্যাগম replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

স্থির পূর্ণাঙ্ক convert_graft_file(পূর্ণাঙ্ক force)
{
	ধ্রুব বর্ণ *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	সংরচনা strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	সংরচনা strvec args = STRVEC_INIT;

	যদি (!fp)
		প্রত্যাগম -1;

	no_graft_file_deprecated_advice = 1;
	যাবত্ (strbuf_getline(&buf, fp) != EOF) {
		যদি (*buf.buf == '#')
			অনুবর্ত;

		strvec_split(&args, buf.buf);
		যদি (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	যদি (!err.len)
		প্রত্যাগম unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	প্রত্যাগম -1;
}

পূর্ণাঙ্ক cmd_replace(পূর্ণাঙ্ক argc,
		ধ্রুব বর্ণ **argv,
		ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক force = 0;
	পূর্ণাঙ্ক raw = 0;
	ধ্রুব বর্ণ *format = রিক্ত;
	গণনা {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	সংরচনা option options[] = {
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
	repo_config(the_repository, git_default_config, রিক্ত);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	যদি (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	যদি (format && cmdmode != MODE_LIST)
		usage_বার্তা_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	যদি (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_বার্তা_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	যদি (raw && cmdmode != MODE_EDIT)
		usage_বার্তা_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	বিকল্প (cmdmode) {
	পক্ষ MODE_DELETE:
		যদি (argc < 1)
			usage_বার্তা_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		প্রত্যাগম for_each_replace_name(argv, delete_replace_ref);

	পক্ষ MODE_REPLACE:
		যদি (argc != 2)
			usage_বার্তা_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		প্রত্যাগম replace_object(argv[0], argv[1], force);

	পক্ষ MODE_EDIT:
		যদি (argc != 1)
			usage_বার্তা_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		প্রত্যাগম edit_and_replace(argv[0], force, raw);

	পক্ষ MODE_GRAFT:
		যদি (argc < 1)
			usage_বার্তা_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		প্রত্যাগম create_graft(argc, argv, force, 0);

	পক্ষ MODE_CONVERT_GRAFT_FILE:
		যদি (argc != 0)
			usage_বার্তা_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		প্রত্যাগম !!convert_graft_file(force);

	পক্ষ MODE_LIST:
		যদি (argc > 1)
			usage_বার্তা_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		প্রত্যাগম list_replace_refs(argv[0], format);

	স্বভাব:
		BUG("invalid cmdmode %d", (পূর্ণাঙ্ক)cmdmode);
	}
}
