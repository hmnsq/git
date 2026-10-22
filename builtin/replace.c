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
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "refs.h"
#ضمن "parse-options.h"
#ضمن "path.h"
#ضمن "run-command.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "replace-object.h"
#ضمن "tag.h"
#ضمن "wildmatch.h"

ساكن ثابت حرف * ثابت git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	عدم
};

تعداد replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

بنية show_data {
	بنية repository *repo;
	ثابت حرف *pattern;
	تعداد replace_format format;
};

ساكن صحيح show_reference(ثابت بنية reference *ref, فراغ *cb_data)
{
	بنية show_data *data = cb_data;

	אם (!wildmatch(data->pattern, ref->name, 0)) {
		אם (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		אחרת אם (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		אחרת { /* data->format == REPLACE_FORMAT_LONG */
			بنية object_id object;
			تعداد object_type obj_type, repl_type;

			אם (repo_get_oid(data->repo, ref->name, &object))
				החזר error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, عدم);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, عدم);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	החזר 0;
}

ساكن صحيح list_replace_refs(ثابت حرف *pattern, ثابت حرف *format)
{
	بنية show_data data;

	data.repo = the_repository;
	אם (!pattern)
		pattern = "*";
	data.pattern = pattern;

	אם (format == عدم || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	אחרת אם (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	אחרת אם (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	אחרת
		החזר error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (فراغ *)&data);

	החזר 0;
}

تعريفنوع صحيح (*each_replace_name_fn)(ثابت حرف *name, ثابت حرف *ref,
				    ثابت بنية object_id *oid);

ساكن صحيح for_each_replace_name(ثابت حرف **argv, each_replace_name_fn fn)
{
	ثابت حرف **p, *full_hex;
	بنية strbuf ref = STRBUF_INIT;
	size_t base_len;
	صحيح had_error = 0;
	بنية object_id oid;
	ثابت حرف *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	עבור (p = argv; *p; p++) {
		אם (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			استمر;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		אם (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			استمر;
		}
		אם (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	החזר had_error;
}

ساكن صحيح delete_replace_ref(ثابت حرف *name, ثابت حرف *ref,
			      ثابت بنية object_id *oid)
{
	אם (refs_delete_ref(get_main_ref_store(the_repository), عدم, ref, oid, 0))
		החזר 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	החזר 0;
}

ساكن صحيح check_ref_valid(بنية object_id *object,
			    بنية object_id *prev,
			    بنية strbuf *ref,
			    صحيح force)
{
	ثابت حرف *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	אם (check_refname_format(ref->buf, 0))
		החזר error(_("'%s' is not a valid ref name"), ref->buf);

	אם (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	אחרת אם (!force)
		החזר error(_("replace ref '%s' already exists"), ref->buf);
	החזר 0;
}

ساكن صحيح replace_object_oid(ثابت حرف *object_ref,
			       بنية object_id *object,
			       ثابت حرف *replace_ref,
			       بنية object_id *repl,
			       صحيح force)
{
	بنية object_id prev;
	تعداد object_type obj_type, repl_type;
	بنية strbuf ref = STRBUF_INIT;
	بنية ref_transaction *transaction;
	بنية strbuf err = STRBUF_INIT;
	صحيح res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, عدم);
	repl_type = odb_read_object_info(the_repository->objects, repl, عدم);
	אם (!force && obj_type != repl_type)
		החזר error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	אם (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		החזר -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	אם (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   عدم, عدم, 0, عدم, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	החזר res;
}

ساكن صحيح replace_object(ثابت حرف *object_ref, ثابت حرف *replace_ref, صحيح force)
{
	بنية object_id object, repl;

	אם (repo_get_oid(the_repository, object_ref, &object))
		החזר error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	אם (repo_get_oid(the_repository, replace_ref, &repl))
		החזר error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	החזר replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ساكن صحيح export_object(ثابت بنية object_id *oid, تعداد object_type type,
			  صحيح raw, ثابت حرف *filename)
{
	بنية child_process cmd = CHILD_PROCESS_INIT;
	صحيح fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	אם (fd < 0)
		החזר error_errno(_("unable to open %s for writing"), filename);

	strvec_push(&cmd.args, "--no-replace-objects");
	strvec_push(&cmd.args, "cat-file");
	אם (raw)
		strvec_push(&cmd.args, type_name(type));
	אחרת
		strvec_push(&cmd.args, "-p");
	strvec_push(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	אם (run_command(&cmd))
		החזר error(_("cat-file reported failure"));
	החזר 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ساكن صحيح import_object(بنية object_id *oid, تعداد object_type type,
			  صحيح raw, ثابت حرف *filename)
{
	صحيح fd;

	fd = open(filename, O_RDONLY);
	אם (fd < 0)
		החזר error_errno(_("unable to open %s for reading"), filename);

	אם (!raw && type == OBJ_TREE) {
		بنية child_process cmd = CHILD_PROCESS_INIT;
		بنية strbuf result = STRBUF_INIT;

		strvec_push(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		אם (start_command(&cmd)) {
			close(fd);
			החזר error(_("unable to spawn mktree"));
		}

		אם (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			החזר -1;
		}
		close(cmd.out);

		אם (finish_command(&cmd)) {
			strbuf_release(&result);
			החזר error(_("mktree reported failure"));
		}
		אם (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			החזר error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} אחרת {
		بنية stat st;
		صحيح flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		אם (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			החזר -1;
		}
		אם (index_fd(the_repository->index, oid, fd, &st, type, عدم, flags) < 0)
			החזר error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	החזר 0;
}

ساكن صحيح edit_and_replace(ثابت حرف *object_ref, صحيح force, صحيح raw)
{
	حرف *tmpfile;
	تعداد object_type type;
	بنية object_id old_oid, new_oid, prev;
	بنية strbuf ref = STRBUF_INIT;

	אם (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		החזר error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, عدم);
	אם (type < 0)
		החזר error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	אם (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		החזר -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	אם (export_object(&old_oid, type, raw, tmpfile)) {
		حرر(tmpfile);
		החזר -1;
	}
	אם (launch_editor(tmpfile, عدم, عدم) < 0) {
		حرر(tmpfile);
		החזר error(_("editing object file failed"));
	}
	אם (import_object(&new_oid, type, raw, tmpfile)) {
		حرر(tmpfile);
		החזר -1;
	}
	حرر(tmpfile);

	אם (oideq(&old_oid, &new_oid))
		החזר error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	החזר replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ساكن صحيح replace_parents(بنية strbuf *buf, صحيح argc, ثابت حرف **argv)
{
	بنية strbuf new_parents = STRBUF_INIT;
	ثابت حرف *parent_start, *parent_end;
	صحيح i;
	ثابت unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	כלעוד (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	עבור (i = 0; i < argc; i++) {
		بنية object_id oid;
		بنية commit *commit;

		אם (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			החזר error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		אם (!commit) {
			strbuf_release(&new_parents);
			החזר error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	החזר 0;
}

بنية check_mergetag_data {
	صحيح argc;
	ثابت حرف **argv;
};

ساكن صحيح check_one_mergetag(بنية commit *commit UNUSED,
			       بنية commit_extra_header *extra,
			       فراغ *data)
{
	بنية check_mergetag_data *mergetag_data = (بنية check_mergetag_data *)data;
	ثابت حرف *ref = mergetag_data->argv[0];
	بنية object_id tag_oid;
	بنية tag *tag;
	صحيح i;

	hash_object_file(the_hash_algo, extra->value, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	אם (!tag)
		החזר error(_("bad mergetag in commit '%s'"), ref);
	אם (parse_tag_buffer(the_repository, tag, extra->value, extra->len))
		החזר error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	עבור (i = 1; i < mergetag_data->argc; i++) {
		بنية object_id oid;
		אם (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			החזר error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		אם (oideq(get_tagged_oid(tag), &oid))
			החזר 0; /* found */
	}

	החזר error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

ساكن صحيح check_mergetags(بنية commit *commit, صحيح argc, ثابت حرف **argv)
{
	بنية check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	החזר for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

ساكن صحيح create_graft(صحيح argc, ثابت حرف **argv, صحيح force, صحيح gentle)
{
	بنية object_id old_oid, new_oid;
	ثابت حرف *old_ref = argv[0];
	بنية commit *commit;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *buffer;
	unsigned long size;

	אם (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		החזר error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	אם (!commit)
		החזר error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	אם (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		החזר -1;
	}

	אם (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	אם (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		החזר -1;
	}

	אם (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		החזר error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	אם (oideq(&commit->object.oid, &new_oid)) {
		אם (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			החזר 0;
		}
		החזר error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	החזר replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ساكن صحيح convert_graft_file(صحيح force)
{
	ثابت حرف *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	بنية strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	بنية strvec args = STRVEC_INIT;

	אם (!fp)
		החזר -1;

	no_graft_file_deprecated_advice = 1;
	כלעוד (strbuf_getline(&buf, fp) != EOF) {
		אם (*buf.buf == '#')
			استمر;

		strvec_split(&args, buf.buf);
		אם (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	אם (!err.len)
		החזר unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	החזר -1;
}

صحيح cmd_replace(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	صحيح force = 0;
	صحيح raw = 0;
	ثابت حرف *format = عدم;
	تعداد {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	بنية option options[] = {
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
	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	אם (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	אם (format && cmdmode != MODE_LIST)
		usage_msg_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	אם (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_msg_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	אם (raw && cmdmode != MODE_EDIT)
		usage_msg_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	بدل (cmdmode) {
	حالة MODE_DELETE:
		אם (argc < 1)
			usage_msg_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		החזר for_each_replace_name(argv, delete_replace_ref);

	حالة MODE_REPLACE:
		אם (argc != 2)
			usage_msg_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		החזר replace_object(argv[0], argv[1], force);

	حالة MODE_EDIT:
		אם (argc != 1)
			usage_msg_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		החזר edit_and_replace(argv[0], force, raw);

	حالة MODE_GRAFT:
		אם (argc < 1)
			usage_msg_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		החזר create_graft(argc, argv, force, 0);

	حالة MODE_CONVERT_GRAFT_FILE:
		אם (argc != 0)
			usage_msg_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		החזר !!convert_graft_file(force);

	حالة MODE_LIST:
		אם (argc > 1)
			usage_msg_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		החזר list_replace_refs(argv[0], format);

	افتراضي:
		BUG("invalid cmdmode %d", (صحيح)cmdmode);
	}
}
