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
#شامل "builtin.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "refs.h"
#شامل "parse-options.h"
#شامل "path.h"
#شامل "run-command.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "replace-object.h"
#شامل "tag.h"
#شامل "wildmatch.h"

ایستا ثابت نویسه * ثابت git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	پوچ
};

شمارش replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ساختار show_data {
	ساختار repository *repo;
	ثابت نویسه *pattern;
	شمارش replace_format format;
};

ایستا صحیح show_reference(ثابت ساختار reference *ref, تهی *cb_data)
{
	ساختار show_data *data = cb_data;

	اگر (!wildmatch(data->pattern, ref->name, 0)) {
		اگر (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		وگرنه اگر (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		وگرنه { /* data->format == REPLACE_FORMAT_LONG */
			ساختار object_id object;
			شمارش object_type obj_type, repl_type;

			اگر (repo_get_oid(data->repo, ref->name, &object))
				بازگشت error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, پوچ);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, پوچ);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	بازگشت 0;
}

ایستا صحیح list_replace_refs(ثابت نویسه *pattern, ثابت نویسه *format)
{
	ساختار show_data data;

	data.repo = the_repository;
	اگر (!pattern)
		pattern = "*";
	data.pattern = pattern;

	اگر (format == پوچ || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	وگرنه اگر (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	وگرنه اگر (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	وگرنه
		بازگشت error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (تهی *)&data);

	بازگشت 0;
}

تعریفنوع صحیح (*each_replace_name_fn)(ثابت نویسه *name, ثابت نویسه *ref,
				    ثابت ساختار object_id *oid);

ایستا صحیح for_each_replace_name(ثابت نویسه **argv, each_replace_name_fn fn)
{
	ثابت نویسه **p, *full_hex;
	ساختار strbuf ref = STRBUF_INIT;
	size_t base_len;
	صحیح had_error = 0;
	ساختار object_id oid;
	ثابت نویسه *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	برای (p = argv; *p; p++) {
		اگر (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			ادامه;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		اگر (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			ادامه;
		}
		اگر (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	بازگشت had_error;
}

ایستا صحیح delete_replace_ref(ثابت نویسه *name, ثابت نویسه *ref,
			      ثابت ساختار object_id *oid)
{
	اگر (refs_delete_ref(get_main_ref_store(the_repository), پوچ, ref, oid, 0))
		بازگشت 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	بازگشت 0;
}

ایستا صحیح check_ref_valid(ساختار object_id *object,
			    ساختار object_id *prev,
			    ساختار strbuf *ref,
			    صحیح force)
{
	ثابت نویسه *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	اگر (check_refname_format(ref->buf, 0))
		بازگشت error(_("'%s' is not a valid ref name"), ref->buf);

	اگر (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	وگرنه اگر (!force)
		بازگشت error(_("replace ref '%s' already exists"), ref->buf);
	بازگشت 0;
}

ایستا صحیح replace_object_oid(ثابت نویسه *object_ref,
			       ساختار object_id *object,
			       ثابت نویسه *replace_ref,
			       ساختار object_id *repl,
			       صحیح force)
{
	ساختار object_id prev;
	شمارش object_type obj_type, repl_type;
	ساختار strbuf ref = STRBUF_INIT;
	ساختار ref_transaction *transaction;
	ساختار strbuf err = STRBUF_INIT;
	صحیح res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, پوچ);
	repl_type = odb_read_object_info(the_repository->objects, repl, پوچ);
	اگر (!force && obj_type != repl_type)
		بازگشت error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	اگر (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		بازگشت -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   پوچ, پوچ, 0, پوچ, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	بازگشت res;
}

ایستا صحیح replace_object(ثابت نویسه *object_ref, ثابت نویسه *replace_ref, صحیح force)
{
	ساختار object_id object, repl;

	اگر (repo_get_oid(the_repository, object_ref, &object))
		بازگشت error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	اگر (repo_get_oid(the_repository, replace_ref, &repl))
		بازگشت error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	بازگشت replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ایستا صحیح export_object(ثابت ساختار object_id *oid, شمارش object_type type,
			  صحیح raw, ثابت نویسه *filename)
{
	ساختار child_process cmd = CHILD_PROCESS_INIT;
	صحیح fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	اگر (fd < 0)
		بازگشت error_errno(_("unable to open %s for writing"), filename);

	strvec_افزودن(&cmd.args, "--no-replace-objects");
	strvec_افزودن(&cmd.args, "cat-file");
	اگر (raw)
		strvec_افزودن(&cmd.args, type_name(type));
	وگرنه
		strvec_افزودن(&cmd.args, "-p");
	strvec_افزودن(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	اگر (run_command(&cmd))
		بازگشت error(_("cat-file reported failure"));
	بازگشت 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ایستا صحیح import_object(ساختار object_id *oid, شمارش object_type type,
			  صحیح raw, ثابت نویسه *filename)
{
	صحیح fd;

	fd = open(filename, O_RDONLY);
	اگر (fd < 0)
		بازگشت error_errno(_("unable to open %s for reading"), filename);

	اگر (!raw && type == OBJ_درخت) {
		ساختار child_process cmd = CHILD_PROCESS_INIT;
		ساختار strbuf result = STRBUF_INIT;

		strvec_افزودن(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		اگر (start_command(&cmd)) {
			close(fd);
			بازگشت error(_("unable to spawn mktree"));
		}

		اگر (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			بازگشت -1;
		}
		close(cmd.out);

		اگر (finish_command(&cmd)) {
			strbuf_release(&result);
			بازگشت error(_("mktree reported failure"));
		}
		اگر (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			بازگشت error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} وگرنه {
		ساختار stat st;
		صحیح flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		اگر (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			بازگشت -1;
		}
		اگر (index_fd(the_repository->index, oid, fd, &st, type, پوچ, flags) < 0)
			بازگشت error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	بازگشت 0;
}

ایستا صحیح edit_and_replace(ثابت نویسه *object_ref, صحیح force, صحیح raw)
{
	نویسه *tmpfile;
	شمارش object_type type;
	ساختار object_id old_oid, new_oid, prev;
	ساختار strbuf ref = STRBUF_INIT;

	اگر (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		بازگشت error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, پوچ);
	اگر (type < 0)
		بازگشت error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	اگر (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		بازگشت -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	اگر (export_object(&old_oid, type, raw, tmpfile)) {
		آزاد(tmpfile);
		بازگشت -1;
	}
	اگر (launch_editor(tmpfile, پوچ, پوچ) < 0) {
		آزاد(tmpfile);
		بازگشت error(_("editing object file failed"));
	}
	اگر (import_object(&new_oid, type, raw, tmpfile)) {
		آزاد(tmpfile);
		بازگشت -1;
	}
	آزاد(tmpfile);

	اگر (oideq(&old_oid, &new_oid))
		بازگشت error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	بازگشت replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ایستا صحیح replace_parents(ساختار strbuf *buf, صحیح argc, ثابت نویسه **argv)
{
	ساختار strbuf new_parents = STRBUF_INIT;
	ثابت نویسه *parent_start, *parent_end;
	صحیح i;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	وقتی (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	برای (i = 0; i < argc; i++) {
		ساختار object_id oid;
		ساختار commit *commit;

		اگر (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			بازگشت error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		اگر (!commit) {
			strbuf_release(&new_parents);
			بازگشت error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	بازگشت 0;
}

ساختار check_mergetag_data {
	صحیح argc;
	ثابت نویسه **argv;
};

ایستا صحیح check_one_mergetag(ساختار commit *commit UNUSED,
			       ساختار commit_extra_header *extra,
			       تهی *data)
{
	ساختار check_mergetag_data *mergetag_data = (ساختار check_mergetag_data *)data;
	ثابت نویسه *ref = mergetag_data->argv[0];
	ساختار object_id tag_oid;
	ساختار tag *tag;
	صحیح i;

	hash_object_file(the_hash_algo, extra->مقدار, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	اگر (!tag)
		بازگشت error(_("bad mergetag in commit '%s'"), ref);
	اگر (parse_tag_buffer(the_repository, tag, extra->مقدار, extra->len))
		بازگشت error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	برای (i = 1; i < mergetag_data->argc; i++) {
		ساختار object_id oid;
		اگر (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			بازگشت error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		اگر (oideq(get_tagged_oid(tag), &oid))
			بازگشت 0; /* found */
	}

	بازگشت error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

ایستا صحیح check_mergetags(ساختار commit *commit, صحیح argc, ثابت نویسه **argv)
{
	ساختار check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	بازگشت for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

ایستا صحیح create_graft(صحیح argc, ثابت نویسه **argv, صحیح force, صحیح gentle)
{
	ساختار object_id old_oid, new_oid;
	ثابت نویسه *old_ref = argv[0];
	ساختار commit *commit;
	ساختار strbuf buf = STRBUF_INIT;
	ثابت نویسه *buffer;
	unsigned long size;

	اگر (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		بازگشت error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	اگر (!commit)
		بازگشت error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	اگر (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		بازگشت -1;
	}

	اگر (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	اگر (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		بازگشت -1;
	}

	اگر (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		بازگشت error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	اگر (oideq(&commit->object.oid, &new_oid)) {
		اگر (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			بازگشت 0;
		}
		بازگشت error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	بازگشت replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ایستا صحیح convert_graft_file(صحیح force)
{
	ثابت نویسه *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ساختار strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ساختار strvec args = STRVEC_INIT;

	اگر (!fp)
		بازگشت -1;

	no_graft_file_deprecated_advice = 1;
	وقتی (strbuf_getline(&buf, fp) != EOF) {
		اگر (*buf.buf == '#')
			ادامه;

		strvec_split(&args, buf.buf);
		اگر (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	اگر (!err.len)
		بازگشت unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	بازگشت -1;
}

صحیح cmd_replace(صحیح argc,
		ثابت نویسه **argv,
		ثابت نویسه *prefix,
		ساختار repository *repo UNUSED)
{
	صحیح force = 0;
	صحیح raw = 0;
	ثابت نویسه *format = پوچ;
	شمارش {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	ساختار option options[] = {
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
	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	اگر (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	اگر (format && cmdmode != MODE_LIST)
		usage_پیام_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	اگر (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_پیام_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	اگر (raw && cmdmode != MODE_EDIT)
		usage_پیام_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	انتخاب (cmdmode) {
	حالت MODE_DELETE:
		اگر (argc < 1)
			usage_پیام_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		بازگشت for_each_replace_name(argv, delete_replace_ref);

	حالت MODE_REPLACE:
		اگر (argc != 2)
			usage_پیام_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		بازگشت replace_object(argv[0], argv[1], force);

	حالت MODE_EDIT:
		اگر (argc != 1)
			usage_پیام_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		بازگشت edit_and_replace(argv[0], force, raw);

	حالت MODE_GRAFT:
		اگر (argc < 1)
			usage_پیام_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		بازگشت create_graft(argc, argv, force, 0);

	حالت MODE_CONVERT_GRAFT_FILE:
		اگر (argc != 0)
			usage_پیام_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		بازگشت !!convert_graft_file(force);

	حالت MODE_LIST:
		اگر (argc > 1)
			usage_پیام_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		بازگشت list_replace_refs(argv[0], format);

	پیشفرض:
		BUG("invalid cmdmode %d", (صحیح)cmdmode);
	}
}
