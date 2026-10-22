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

স্থির ধ্রুবক অক্ষর * ধ্রুবক git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ফাঁকা
};

গণনা replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

কাঠামো show_তথ্য {
	কাঠামো repository *repo;
	ধ্রুবক অক্ষর *pattern;
	গণনা replace_format format;
};

স্থির পূর্ণসংখ্যা show_reference(ধ্রুবক কাঠামো reference *ref, শূন্য *cb_তথ্য)
{
	কাঠামো show_তথ্য *তথ্য = cb_তথ্য;

	যদি (!wildmatch(তথ্য->pattern, ref->name, 0)) {
		যদি (তথ্য->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		অন্যথা যদি (তথ্য->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		অন্যথা { /* data->format == REPLACE_FORMAT_LONG */
			কাঠামো object_id object;
			গণনা object_type obj_type, repl_type;

			যদি (repo_get_oid(তথ্য->repo, ref->name, &object))
				ফেরত error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(তথ্য->repo->objects, &object, ফাঁকা);
			repl_type = odb_read_object_info(তথ্য->repo->objects, ref->oid, ফাঁকা);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	ফেরত 0;
}

স্থির পূর্ণসংখ্যা list_replace_refs(ধ্রুবক অক্ষর *pattern, ধ্রুবক অক্ষর *format)
{
	কাঠামো show_তথ্য তথ্য;

	তথ্য.repo = the_repository;
	যদি (!pattern)
		pattern = "*";
	তথ্য.pattern = pattern;

	যদি (format == ফাঁকা || *format == '\0' || !strcmp(format, "short"))
		তথ্য.format = REPLACE_FORMAT_SHORT;
	অন্যথা যদি (!strcmp(format, "medium"))
		তথ্য.format = REPLACE_FORMAT_MEDIUM;
	অন্যথা যদি (!strcmp(format, "long"))
		তথ্য.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	অন্যথা
		ফেরত error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (শূন্য *)&তথ্য);

	ফেরত 0;
}

প্রকারলেখ পূর্ণসংখ্যা (*each_replace_name_fn)(ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *ref,
				    ধ্রুবক কাঠামো object_id *oid);

স্থির পূর্ণসংখ্যা for_each_replace_name(ধ্রুবক অক্ষর **argv, each_replace_name_fn fn)
{
	ধ্রুবক অক্ষর **p, *full_hex;
	কাঠামো strbuf ref = STRBUF_INIT;
	আকার_t base_len;
	পূর্ণসংখ্যা had_error = 0;
	কাঠামো object_id oid;
	ধ্রুবক অক্ষর *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	জন্য (p = argv; *p; p++) {
		যদি (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			চালু;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		যদি (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			চালু;
		}
		যদি (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ফেরত had_error;
}

স্থির পূর্ণসংখ্যা delete_replace_ref(ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *ref,
			      ধ্রুবক কাঠামো object_id *oid)
{
	যদি (refs_delete_ref(get_main_ref_store(the_repository), ফাঁকা, ref, oid, 0))
		ফেরত 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা check_ref_valid(কাঠামো object_id *object,
			    কাঠামো object_id *পূর্ব,
			    কাঠামো strbuf *ref,
			    পূর্ণসংখ্যা force)
{
	ধ্রুবক অক্ষর *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	যদি (check_refname_format(ref->buf, 0))
		ফেরত error(_("'%s' is not a valid ref name"), ref->buf);

	যদি (refs_read_ref(get_main_ref_store(the_repository), ref->buf, পূর্ব))
		oidclr(পূর্ব, the_repository->hash_algo);
	অন্যথা যদি (!force)
		ফেরত error(_("replace ref '%s' already exists"), ref->buf);
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা replace_object_oid(ধ্রুবক অক্ষর *object_ref,
			       কাঠামো object_id *object,
			       ধ্রুবক অক্ষর *replace_ref,
			       কাঠামো object_id *repl,
			       পূর্ণসংখ্যা force)
{
	কাঠামো object_id পূর্ব;
	গণনা object_type obj_type, repl_type;
	কাঠামো strbuf ref = STRBUF_INIT;
	কাঠামো ref_transaction *transaction;
	কাঠামো strbuf err = STRBUF_INIT;
	পূর্ণসংখ্যা res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ফাঁকা);
	repl_type = odb_read_object_info(the_repository->objects, repl, ফাঁকা);
	যদি (!force && obj_type != repl_type)
		ফেরত error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	যদি (check_ref_valid(object, &পূর্ব, &ref, force)) {
		strbuf_release(&ref);
		ফেরত -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	যদি (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &পূর্ব,
				   ফাঁকা, ফাঁকা, 0, ফাঁকা, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ফেরত res;
}

স্থির পূর্ণসংখ্যা replace_object(ধ্রুবক অক্ষর *object_ref, ধ্রুবক অক্ষর *replace_ref, পূর্ণসংখ্যা force)
{
	কাঠামো object_id object, repl;

	যদি (repo_get_oid(the_repository, object_ref, &object))
		ফেরত error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	যদি (repo_get_oid(the_repository, replace_ref, &repl))
		ফেরত error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ফেরত replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
স্থির পূর্ণসংখ্যা export_object(ধ্রুবক কাঠামো object_id *oid, গণনা object_type type,
			  পূর্ণসংখ্যা raw, ধ্রুবক অক্ষর *filename)
{
	কাঠামো child_process cmd = CHILD_PROCESS_INIT;
	পূর্ণসংখ্যা fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	যদি (fd < 0)
		ফেরত error_errno(_("unable to open %s for writing"), filename);

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
		ফেরত error(_("cat-file reported failure"));
	ফেরত 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
স্থির পূর্ণসংখ্যা import_object(কাঠামো object_id *oid, গণনা object_type type,
			  পূর্ণসংখ্যা raw, ধ্রুবক অক্ষর *filename)
{
	পূর্ণসংখ্যা fd;

	fd = open(filename, O_RDONLY);
	যদি (fd < 0)
		ফেরত error_errno(_("unable to open %s for reading"), filename);

	যদি (!raw && type == OBJ_বৃক্ষ) {
		কাঠামো child_process cmd = CHILD_PROCESS_INIT;
		কাঠামো strbuf ফলাফল = STRBUF_INIT;

		strvec_ঠেলো(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		যদি (start_command(&cmd)) {
			close(fd);
			ফেরত error(_("unable to spawn mktree"));
		}

		যদি (strbuf_read(&ফলাফল, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			ফেরত -1;
		}
		close(cmd.out);

		যদি (finish_command(&cmd)) {
			strbuf_release(&ফলাফল);
			ফেরত error(_("mktree reported failure"));
		}
		যদি (get_oid_hex(ফলাফল.buf, oid) < 0) {
			strbuf_release(&ফলাফল);
			ফেরত error(_("mktree did not return an object name"));
		}

		strbuf_release(&ফলাফল);
	} অন্যথা {
		কাঠামো stat st;
		পূর্ণসংখ্যা flags = সূচক_FORMAT_CHECK | সূচক_WRITE_OBJECT;

		যদি (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ফেরত -1;
		}
		যদি (সূচক_fd(the_repository->সূচক, oid, fd, &st, type, ফাঁকা, flags) < 0)
			ফেরত error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা edit_and_replace(ধ্রুবক অক্ষর *object_ref, পূর্ণসংখ্যা force, পূর্ণসংখ্যা raw)
{
	অক্ষর *tmpfile;
	গণনা object_type type;
	কাঠামো object_id old_oid, new_oid, পূর্ব;
	কাঠামো strbuf ref = STRBUF_INIT;

	যদি (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ফেরত error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ফাঁকা);
	যদি (type < 0)
		ফেরত error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	যদি (check_ref_valid(&old_oid, &পূর্ব, &ref, force)) {
		strbuf_release(&ref);
		ফেরত -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	যদি (export_object(&old_oid, type, raw, tmpfile)) {
		মুক্ত(tmpfile);
		ফেরত -1;
	}
	যদি (launch_editor(tmpfile, ফাঁকা, ফাঁকা) < 0) {
		মুক্ত(tmpfile);
		ফেরত error(_("editing object file failed"));
	}
	যদি (import_object(&new_oid, type, raw, tmpfile)) {
		মুক্ত(tmpfile);
		ফেরত -1;
	}
	মুক্ত(tmpfile);

	যদি (oideq(&old_oid, &new_oid))
		ফেরত error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	ফেরত replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

স্থির পূর্ণসংখ্যা replace_parents(কাঠামো strbuf *buf, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	কাঠামো strbuf new_parents = STRBUF_INIT;
	ধ্রুবক অক্ষর *parent_start, *parent_end;
	পূর্ণসংখ্যা i;
	ধ্রুবক unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	যখন (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	জন্য (i = 0; i < argc; i++) {
		কাঠামো object_id oid;
		কাঠামো commit *commit;

		যদি (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ফেরত error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		যদি (!commit) {
			strbuf_release(&new_parents);
			ফেরত error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ফেরত 0;
}

কাঠামো check_mergetag_তথ্য {
	পূর্ণসংখ্যা argc;
	ধ্রুবক অক্ষর **argv;
};

স্থির পূর্ণসংখ্যা check_one_mergetag(কাঠামো commit *commit UNUSED,
			       কাঠামো commit_extra_header *extra,
			       শূন্য *তথ্য)
{
	কাঠামো check_mergetag_তথ্য *mergetag_তথ্য = (কাঠামো check_mergetag_তথ্য *)তথ্য;
	ধ্রুবক অক্ষর *ref = mergetag_তথ্য->argv[0];
	কাঠামো object_id tag_oid;
	কাঠামো tag *tag;
	পূর্ণসংখ্যা i;

	hash_object_file(the_hash_algo, extra->মান, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	যদি (!tag)
		ফেরত error(_("bad mergetag in commit '%s'"), ref);
	যদি (parse_tag_বাফার(the_repository, tag, extra->মান, extra->len))
		ফেরত error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	জন্য (i = 1; i < mergetag_তথ্য->argc; i++) {
		কাঠামো object_id oid;
		যদি (repo_get_oid(the_repository, mergetag_তথ্য->argv[i], &oid) < 0)
			ফেরত error(_("not a valid object name: '%s'"),
				     mergetag_তথ্য->argv[i]);
		যদি (oideq(get_tagged_oid(tag), &oid))
			ফেরত 0; /* found */
	}

	ফেরত error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

স্থির পূর্ণসংখ্যা check_mergetags(কাঠামো commit *commit, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	কাঠামো check_mergetag_তথ্য mergetag_তথ্য;

	mergetag_তথ্য.argc = argc;
	mergetag_তথ্য.argv = argv;
	ফেরত for_each_mergetag(check_one_mergetag, commit, &mergetag_তথ্য);
}

স্থির পূর্ণসংখ্যা create_graft(পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv, পূর্ণসংখ্যা force, পূর্ণসংখ্যা gentle)
{
	কাঠামো object_id old_oid, new_oid;
	ধ্রুবক অক্ষর *old_ref = argv[0];
	কাঠামো commit *commit;
	কাঠামো strbuf buf = STRBUF_INIT;
	ধ্রুবক অক্ষর *বাফার;
	unsigned long আকার_2;

	যদি (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ফেরত error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	যদি (!commit)
		ফেরত error(_("could not parse %s"), old_ref);

	বাফার = repo_get_commit_বাফার(the_repository, commit, &আকার_2);
	strbuf_add(&buf, বাফার, আকার_2);
	repo_unuse_commit_বাফার(the_repository, commit, বাফার);

	যদি (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ফেরত -1;
	}

	যদি (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	যদি (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ফেরত -1;
	}

	যদি (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ফেরত error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	যদি (oideq(&commit->object.oid, &new_oid)) {
		যদি (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			ফেরত 0;
		}
		ফেরত error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	ফেরত replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

স্থির পূর্ণসংখ্যা convert_graft_file(পূর্ণসংখ্যা force)
{
	ধ্রুবক অক্ষর *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	কাঠামো strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	কাঠামো strvec args = STRVEC_INIT;

	যদি (!fp)
		ফেরত -1;

	no_graft_file_deprecated_advice = 1;
	যখন (strbuf_getline(&buf, fp) != EOF) {
		যদি (*buf.buf == '#')
			চালু;

		strvec_split(&args, buf.buf);
		যদি (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	যদি (!err.len)
		ফেরত unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ফেরত -1;
}

পূর্ণসংখ্যা cmd_replace(পূর্ণসংখ্যা argc,
		ধ্রুবক অক্ষর **argv,
		ধ্রুবক অক্ষর *prefix,
		কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা force = 0;
	পূর্ণসংখ্যা raw = 0;
	ধ্রুবক অক্ষর *format = ফাঁকা;
	গণনা {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	কাঠামো option options[] = {
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
	repo_config(the_repository, git_default_config, ফাঁকা);

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

	নির্বাচন (cmdmode) {
	ক্ষেত্র MODE_DELETE:
		যদি (argc < 1)
			usage_বার্তা_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ফেরত for_each_replace_name(argv, delete_replace_ref);

	ক্ষেত্র MODE_REPLACE:
		যদি (argc != 2)
			usage_বার্তা_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ফেরত replace_object(argv[0], argv[1], force);

	ক্ষেত্র MODE_EDIT:
		যদি (argc != 1)
			usage_বার্তা_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ফেরত edit_and_replace(argv[0], force, raw);

	ক্ষেত্র MODE_GRAFT:
		যদি (argc < 1)
			usage_বার্তা_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ফেরত create_graft(argc, argv, force, 0);

	ক্ষেত্র MODE_CONVERT_GRAFT_FILE:
		যদি (argc != 0)
			usage_বার্তা_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ফেরত !!convert_graft_file(force);

	ক্ষেত্র MODE_LIST:
		যদি (argc > 1)
			usage_বার্তা_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ফেরত list_replace_refs(argv[0], format);

	পূর্বনির্ধারিত:
		BUG("invalid cmdmode %d", (পূর্ণসংখ্যা)cmdmode);
	}
}
