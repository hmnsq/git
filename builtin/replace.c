/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن ثابت حرف * ثابت git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	تہی
};

شمار replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ساخت show_ڈیٹا {
	ساخت repository *repo;
	ثابت حرف *نمونہ;
	شمار replace_format format;
};

ساکن صحیح show_reference(ثابت ساخت reference *ref, خالی *cb_ڈیٹا)
{
	ساخت show_ڈیٹا *ڈیٹا = cb_ڈیٹا;

	اگر (!wildmatch(ڈیٹا->نمونہ, ref->name, 0)) {
		اگر (ڈیٹا->format == REPLACE_FORMAT_SHORT)
			لکھو("%s\n", ref->name);
		ورنہ اگر (ڈیٹا->format == REPLACE_FORMAT_MEDIUM)
			لکھو("%s -> %s\n", ref->name, oid_مقصد_hex(ref->oid));
		ورنہ { /* data->format == REPLACE_FORMAT_LONG */
			ساخت object_id object;
			شمار object_type obj_type, repl_type;

			اگر (repo_get_oid(ڈیٹا->repo, ref->name, &object))
				واپس error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(ڈیٹا->repo->objects, &object, تہی);
			repl_type = odb_read_object_info(ڈیٹا->repo->objects, ref->oid, تہی);

			لکھو("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_مقصد_hex(ref->oid), type_name(repl_type));
		}
	}

	واپس 0;
}

ساکن صحیح list_replace_refs(ثابت حرف *نمونہ, ثابت حرف *format)
{
	ساخت show_ڈیٹا ڈیٹا;

	ڈیٹا.repo = the_repository;
	اگر (!نمونہ)
		نمونہ = "*";
	ڈیٹا.نمونہ = نمونہ;

	اگر (format == تہی || *format == '\0' || !strcmp(format, "short"))
		ڈیٹا.format = REPLACE_FORMAT_SHORT;
	ورنہ اگر (!strcmp(format, "medium"))
		ڈیٹا.format = REPLACE_FORMAT_MEDIUM;
	ورنہ اگر (!strcmp(format, "long"))
		ڈیٹا.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	ورنہ
		واپس error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (خالی *)&ڈیٹا);

	واپس 0;
}

قسمتعریف صحیح (*each_replace_name_fn)(ثابت حرف *name, ثابت حرف *ref,
				    ثابت ساخت object_id *oid);

ساکن صحیح for_each_replace_name(ثابت حرف **argv, each_replace_name_fn fn)
{
	ثابت حرف **p, *بھرا_hex;
	ساخت strbuf ref = STRBUF_INIT;
	سائز_t بنیاد_len;
	صحیح had_error = 0;
	ساخت object_id oid;
	ثابت حرف *git_replace_ref_بنیاد = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_بنیاد);
	بنیاد_len = ref.len;

	برائے (p = argv; *p; p++) {
		اگر (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			جاری;
		}

		strbuf_setlen(&ref, بنیاد_len);
		strbuf_جوڑو_oid_hex(&ref, &oid);
		بھرا_hex = ref.buf + بنیاد_len;

		اگر (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), بھرا_hex);
			had_error = 1;
			جاری;
		}
		اگر (fn(بھرا_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	واپس had_error;
}

ساکن صحیح delete_replace_ref(ثابت حرف *name, ثابت حرف *ref,
			      ثابت ساخت object_id *oid)
{
	اگر (refs_delete_ref(get_main_ref_store(the_repository), تہی, ref, oid, 0))
		واپس 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	واپس 0;
}

ساکن صحیح check_ref_valid(ساخت object_id *object,
			    ساخت object_id *پچھلا_2,
			    ساخت strbuf *ref,
			    صحیح force)
{
	ثابت حرف *git_replace_ref_بنیاد = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_بنیاد, oid_مقصد_hex(object));
	اگر (check_refname_format(ref->buf, 0))
		واپس error(_("'%s' is not a valid ref name"), ref->buf);

	اگر (refs_read_ref(get_main_ref_store(the_repository), ref->buf, پچھلا_2))
		oidclr(پچھلا_2, the_repository->hash_algo);
	ورنہ اگر (!force)
		واپس error(_("replace ref '%s' already exists"), ref->buf);
	واپس 0;
}

ساکن صحیح replace_object_oid(ثابت حرف *object_ref,
			       ساخت object_id *object,
			       ثابت حرف *replace_ref,
			       ساخت object_id *repl,
			       صحیح force)
{
	ساخت object_id پچھلا_2;
	شمار object_type obj_type, repl_type;
	ساخت strbuf ref = STRBUF_INIT;
	ساخت ref_transaction *transaction;
	ساخت strbuf err = STRBUF_INIT;
	صحیح res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, تہی);
	repl_type = odb_read_object_info(the_repository->objects, repl, تہی);
	اگر (!force && obj_type != repl_type)
		واپس error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	اگر (check_ref_valid(object, &پچھلا_2, &ref, force)) {
		strbuf_release(&ref);
		واپس -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	اگر (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &پچھلا_2,
				   تہی, تہی, 0, تہی, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	واپس res;
}

ساکن صحیح replace_object(ثابت حرف *object_ref, ثابت حرف *replace_ref, صحیح force)
{
	ساخت object_id object, repl;

	اگر (repo_get_oid(the_repository, object_ref, &object))
		واپس error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	اگر (repo_get_oid(the_repository, replace_ref, &repl))
		واپس error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	واپس replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ساکن صحیح export_object(ثابت ساخت object_id *oid, شمار object_type type,
			  صحیح raw, ثابت حرف *filename)
{
	ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
	صحیح fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	اگر (fd < 0)
		واپس error_errno(_("unable to open %s for writing"), filename);

	strvec_ڈالو(&cmd.دلائل, "--no-replace-objects");
	strvec_ڈالو(&cmd.دلائل, "cat-file");
	اگر (raw)
		strvec_ڈالو(&cmd.دلائل, type_name(type));
	ورنہ
		strvec_ڈالو(&cmd.دلائل, "-p");
	strvec_ڈالو(&cmd.دلائل, oid_مقصد_hex(oid));
	cmd.git_cmd = 1;
	cmd.برآمد = fd;

	اگر (تسلسل_command(&cmd))
		واپس error(_("cat-file reported failure"));
	واپس 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ساکن صحیح import_object(ساخت object_id *oid, شمار object_type type,
			  صحیح raw, ثابت حرف *filename)
{
	صحیح fd;

	fd = open(filename, O_RDONLY);
	اگر (fd < 0)
		واپس error_errno(_("unable to open %s for reading"), filename);

	اگر (!raw && type == OBJ_درخت) {
		ساخت بچہ_process cmd = بچہ_PROCESS_INIT;
		ساخت strbuf نتیجہ = STRBUF_INIT;

		strvec_ڈالو(&cmd.دلائل, "mktree");
		cmd.git_cmd = 1;
		cmd.اندر = fd;
		cmd.برآمد = -1;

		اگر (آغاز_command(&cmd)) {
			close(fd);
			واپس error(_("unable to spawn mktree"));
		}

		اگر (strbuf_read(&نتیجہ, cmd.برآمد, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.برآمد);
			واپس -1;
		}
		close(cmd.برآمد);

		اگر (اختتام_command(&cmd)) {
			strbuf_release(&نتیجہ);
			واپس error(_("mktree reported failure"));
		}
		اگر (get_oid_hex(نتیجہ.buf, oid) < 0) {
			strbuf_release(&نتیجہ);
			واپس error(_("mktree did not return an object name"));
		}

		strbuf_release(&نتیجہ);
	} ورنہ {
		ساخت stat st;
		صحیح flags = اشاریہ_FORMAT_CHECK | اشاریہ_WRITE_OBJECT;

		اگر (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			واپس -1;
		}
		اگر (اشاریہ_fd(the_repository->اشاریہ, oid, fd, &st, type, تہی, flags) < 0)
			واپس error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	واپس 0;
}

ساکن صحیح edit_and_replace(ثابت حرف *object_ref, صحیح force, صحیح raw)
{
	حرف *tmpfile;
	شمار object_type type;
	ساخت object_id old_oid, new_oid, پچھلا_2;
	ساخت strbuf ref = STRBUF_INIT;

	اگر (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		واپس error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, تہی);
	اگر (type < 0)
		واپس error(_("unable to get object type for %s"),
			     oid_مقصد_hex(&old_oid));

	اگر (check_ref_valid(&old_oid, &پچھلا_2, &ref, force)) {
		strbuf_release(&ref);
		واپس -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_راستہ(the_repository, "REPLACE_EDITOBJ");
	اگر (export_object(&old_oid, type, raw, tmpfile)) {
		آزاد(tmpfile);
		واپس -1;
	}
	اگر (launch_editor(tmpfile, تہی, تہی) < 0) {
		آزاد(tmpfile);
		واپس error(_("editing object file failed"));
	}
	اگر (import_object(&new_oid, type, raw, tmpfile)) {
		آزاد(tmpfile);
		واپس -1;
	}
	آزاد(tmpfile);

	اگر (oideq(&old_oid, &new_oid))
		واپس error(_("new object is the same as the old one: '%s'"), oid_مقصد_hex(&old_oid));

	واپس replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ساکن صحیح replace_parents(ساخت strbuf *buf, صحیح argc, ثابت حرف **argv)
{
	ساخت strbuf new_parents = STRBUF_INIT;
	ثابت حرف *والد_آغاز, *والد_انتہا;
	صحیح i;
	ثابت غیرعلامتی hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	والد_آغاز = buf->buf;
	والد_آغاز += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	والد_انتہا = والد_آغاز;

	جب (starts_with(والد_انتہا, "parent "))
		والد_انتہا += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	برائے (i = 0; i < argc; i++) {
		ساخت object_id oid;
		ساخت commit *commit;

		اگر (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			واپس error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		اگر (!commit) {
			strbuf_release(&new_parents);
			واپس error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_مقصد_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, والد_آغاز - buf->buf, والد_انتہا - والد_آغاز,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	واپس 0;
}

ساخت check_mergetag_ڈیٹا {
	صحیح argc;
	ثابت حرف **argv;
};

ساکن صحیح check_one_mergetag(ساخت commit *commit غیر_مستعمل,
			       ساخت commit_extra_سرخی *extra,
			       خالی *ڈیٹا)
{
	ساخت check_mergetag_ڈیٹا *mergetag_ڈیٹا = (ساخت check_mergetag_ڈیٹا *)ڈیٹا;
	ثابت حرف *ref = mergetag_ڈیٹا->argv[0];
	ساخت object_id لیبل_oid;
	ساخت لیبل *لیبل;
	صحیح i;

	hash_object_file(the_hash_algo, extra->قدر, extra->len,
			 OBJ_لیبل, &لیبل_oid);
	لیبل = lookup_لیبل(the_repository, &لیبل_oid);
	اگر (!لیبل)
		واپس error(_("bad mergetag in commit '%s'"), ref);
	اگر (parse_لیبل_بفر(the_repository, لیبل, extra->قدر, extra->len))
		واپس error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	برائے (i = 1; i < mergetag_ڈیٹا->argc; i++) {
		ساخت object_id oid;
		اگر (repo_get_oid(the_repository, mergetag_ڈیٹا->argv[i], &oid) < 0)
			واپس error(_("not a valid object name: '%s'"),
				     mergetag_ڈیٹا->argv[i]);
		اگر (oideq(get_tagged_oid(لیبل), &oid))
			واپس 0; /*ملا*/
	}

	واپس error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_مقصد_hex(&لیبل_oid));
}

ساکن صحیح check_mergetags(ساخت commit *commit, صحیح argc, ثابت حرف **argv)
{
	ساخت check_mergetag_ڈیٹا mergetag_ڈیٹا;

	mergetag_ڈیٹا.argc = argc;
	mergetag_ڈیٹا.argv = argv;
	واپس for_each_mergetag(check_one_mergetag, commit, &mergetag_ڈیٹا);
}

ساکن صحیح create_graft(صحیح argc, ثابت حرف **argv, صحیح force, صحیح gentle)
{
	ساخت object_id old_oid, new_oid;
	ثابت حرف *old_ref = argv[0];
	ساخت commit *commit;
	ساخت strbuf buf = STRBUF_INIT;
	ثابت حرف *بفر;
	غیرعلامتی طویل سائز;

	اگر (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		واپس error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	اگر (!commit)
		واپس error(_("could not parse %s"), old_ref);

	بفر = repo_get_commit_بفر(the_repository, commit, &سائز);
	strbuf_جوڑو(&buf, بفر, سائز);
	repo_unuse_commit_بفر(the_repository, commit, بفر);

	اگر (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		واپس -1;
	}

	اگر (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	اگر (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		واپس -1;
	}

	اگر (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		واپس error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	اگر (oideq(&commit->object.oid, &new_oid)) {
		اگر (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_مقصد_hex(&commit->object.oid));
			واپس 0;
		}
		واپس error(_("new commit is the same as the old one: '%s'"),
			     oid_مقصد_hex(&commit->object.oid));
	}

	واپس replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ساکن صحیح convert_graft_file(صحیح force)
{
	ثابت حرف *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ساخت strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ساخت strvec دلائل = STRVEC_INIT;

	اگر (!fp)
		واپس -1;

	no_graft_file_deprecated_advice = 1;
	جب (strbuf_getline(&buf, fp) != EOF) {
		اگر (*buf.buf == '#')
			جاری;

		strvec_بانٹو(&دلائل, buf.buf);
		اگر (دلائل.nr && create_graft(دلائل.nr, دلائل.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&دلائل);
	}
	fclose(fp);

	strbuf_release(&buf);

	اگر (!err.len)
		واپس unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	واپس -1;
}

صحیح cmd_replace(صحیح argc,
		ثابت حرف **argv,
		ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	صحیح force = 0;
	صحیح raw = 0;
	ثابت حرف *format = تہی;
	شمار {
		منوال_UNSPECIFIED = 0,
		منوال_LIST,
		منوال_DELETE,
		منوال_EDIT,
		منوال_GRAFT,
		منوال_CONVERT_GRAFT_FILE,
		منوال_REPLACE
	} cmdmode = منوال_UNSPECIFIED;
	ساخت option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list replace refs"), منوال_LIST),
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete replace refs"), منوال_DELETE),
		OPT_CMDMODE('e', "edit", &cmdmode, N_("edit existing object"), منوال_EDIT),
		OPT_CMDMODE('g', "graft", &cmdmode, N_("change a commit's parents"), منوال_GRAFT),
		OPT_CMDMODE(0, "convert-graft-file", &cmdmode, N_("convert existing graft file"), منوال_CONVERT_GRAFT_FILE),
		OPT_BOOL_F('f', "force", &force, N_("replace the ref if it exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "raw", &raw, N_("do not pretty-print contents for --edit")),
		OPT_STRING(0, "format", &format, N_("format"), N_("use this format")),
		OPT_انتہا()
	};

	disable_replace_refs();
	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options, git_replace_usage, 0);

	اگر (!cmdmode)
		cmdmode = argc ? منوال_REPLACE : منوال_LIST;

	اگر (format && cmdmode != منوال_LIST)
		usage_پیغام_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	اگر (force &&
	    cmdmode != منوال_REPLACE &&
	    cmdmode != منوال_EDIT &&
	    cmdmode != منوال_GRAFT &&
	    cmdmode != منوال_CONVERT_GRAFT_FILE)
		usage_پیغام_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	اگر (raw && cmdmode != منوال_EDIT)
		usage_پیغام_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	انتخاب (cmdmode) {
	حالت منوال_DELETE:
		اگر (argc < 1)
			usage_پیغام_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		واپس for_each_replace_name(argv, delete_replace_ref);

	حالت منوال_REPLACE:
		اگر (argc != 2)
			usage_پیغام_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		واپس replace_object(argv[0], argv[1], force);

	حالت منوال_EDIT:
		اگر (argc != 1)
			usage_پیغام_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		واپس edit_and_replace(argv[0], force, raw);

	حالت منوال_GRAFT:
		اگر (argc < 1)
			usage_پیغام_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		واپس create_graft(argc, argv, force, 0);

	حالت منوال_CONVERT_GRAFT_FILE:
		اگر (argc != 0)
			usage_پیغام_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		واپس !!convert_graft_file(force);

	حالت منوال_LIST:
		اگر (argc > 1)
			usage_پیغام_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		واپس list_replace_refs(argv[0], format);

	طےشدہ:
		BUG("invalid cmdmode %d", (صحیح)cmdmode);
	}
}
