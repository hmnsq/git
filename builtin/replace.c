/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
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

بنية show_معلومات {
	بنية repository *repo;
	ثابت حرف *نمونو;
	تعداد replace_format format;
};

ساكن صحيح show_reference(ثابت بنية reference *ref, فراغ *cb_معلومات)
{
	بنية show_معلومات *معلومات = cb_معلومات;

	إذا (!wildmatch(معلومات->نمونو, ref->name, 0)) {
		إذا (معلومات->format == REPLACE_FORMAT_SHORT)
			لکو("%s\n", ref->name);
		وإلا إذا (معلومات->format == REPLACE_FORMAT_MEDIUM)
			لکو("%s -> %s\n", ref->name, oid_منزل_hex(ref->oid));
		وإلا { /* data->format == REPLACE_FORMAT_LONG */
			بنية object_id object;
			تعداد object_type obj_type, repl_type;

			إذا (repo_get_oid(معلومات->repo, ref->name, &object))
				أرجع error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(معلومات->repo->objects, &object, عدم);
			repl_type = odb_read_object_info(معلومات->repo->objects, ref->oid, عدم);

			لکو("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_منزل_hex(ref->oid), type_name(repl_type));
		}
	}

	أرجع 0;
}

ساكن صحيح list_replace_refs(ثابت حرف *نمونو, ثابت حرف *format)
{
	بنية show_معلومات معلومات;

	معلومات.repo = the_repository;
	إذا (!نمونو)
		نمونو = "*";
	معلومات.نمونو = نمونو;

	إذا (format == عدم || *format == '\0' || !strcmp(format, "short"))
		معلومات.format = REPLACE_FORMAT_SHORT;
	وإلا إذا (!strcmp(format, "medium"))
		معلومات.format = REPLACE_FORMAT_MEDIUM;
	وإلا إذا (!strcmp(format, "long"))
		معلومات.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	وإلا
		أرجع error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (فراغ *)&معلومات);

	أرجع 0;
}

تعريفنوع صحيح (*each_replace_name_fn)(ثابت حرف *name, ثابت حرف *ref,
				    ثابت بنية object_id *oid);

ساكن صحيح for_each_replace_name(ثابت حرف **argv, each_replace_name_fn fn)
{
	ثابت حرف **p, *ڀريل_hex;
	بنية strbuf ref = STRBUF_INIT;
	سائيز_t بنياد_len;
	صحيح had_error = 0;
	بنية object_id oid;
	ثابت حرف *git_replace_ref_بنياد = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_بنياد);
	بنياد_len = ref.len;

	لكل (p = argv; *p; p++) {
		إذا (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			استمر;
		}

		strbuf_setlen(&ref, بنياد_len);
		strbuf_جوڙيو_oid_hex(&ref, &oid);
		ڀريل_hex = ref.buf + بنياد_len;

		إذا (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), ڀريل_hex);
			had_error = 1;
			استمر;
		}
		إذا (fn(ڀريل_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	أرجع had_error;
}

ساكن صحيح delete_replace_ref(ثابت حرف *name, ثابت حرف *ref,
			      ثابت بنية object_id *oid)
{
	إذا (refs_delete_ref(get_main_ref_store(the_repository), عدم, ref, oid, 0))
		أرجع 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	أرجع 0;
}

ساكن صحيح check_ref_valid(بنية object_id *object,
			    بنية object_id *prev,
			    بنية strbuf *ref,
			    صحيح force)
{
	ثابت حرف *git_replace_ref_بنياد = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_بنياد, oid_منزل_hex(object));
	إذا (check_refname_format(ref->buf, 0))
		أرجع error(_("'%s' is not a valid ref name"), ref->buf);

	إذا (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	وإلا إذا (!force)
		أرجع error(_("replace ref '%s' already exists"), ref->buf);
	أرجع 0;
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
	إذا (!force && obj_type != repl_type)
		أرجع error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	إذا (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		أرجع -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	إذا (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   عدم, عدم, 0, عدم, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	أرجع res;
}

ساكن صحيح replace_object(ثابت حرف *object_ref, ثابت حرف *replace_ref, صحيح force)
{
	بنية object_id object, repl;

	إذا (repo_get_oid(the_repository, object_ref, &object))
		أرجع error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	إذا (repo_get_oid(the_repository, replace_ref, &repl))
		أرجع error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	أرجع replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ساكن صحيح export_object(ثابت بنية object_id *oid, تعداد object_type type,
			  صحيح raw, ثابت حرف *filename)
{
	بنية ٻار_process cmd = ٻار_PROCESS_INIT;
	صحيح fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	إذا (fd < 0)
		أرجع error_errno(_("unable to open %s for writing"), filename);

	strvec_ڌڪو(&cmd.دليلون, "--no-replace-objects");
	strvec_ڌڪو(&cmd.دليلون, "cat-file");
	إذا (raw)
		strvec_ڌڪو(&cmd.دليلون, type_name(type));
	وإلا
		strvec_ڌڪو(&cmd.دليلون, "-p");
	strvec_ڌڪو(&cmd.دليلون, oid_منزل_hex(oid));
	cmd.git_cmd = 1;
	cmd.ٻاهريون = fd;

	إذا (لاڳيتو_command(&cmd))
		أرجع error(_("cat-file reported failure"));
	أرجع 0;
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
	إذا (fd < 0)
		أرجع error_errno(_("unable to open %s for reading"), filename);

	إذا (!raw && type == OBJ_وڻ) {
		بنية ٻار_process cmd = ٻار_PROCESS_INIT;
		بنية strbuf نتيجو = STRBUF_INIT;

		strvec_ڌڪو(&cmd.دليلون, "mktree");
		cmd.git_cmd = 1;
		cmd.اندريون = fd;
		cmd.ٻاهريون = -1;

		إذا (شروعات_command(&cmd)) {
			close(fd);
			أرجع error(_("unable to spawn mktree"));
		}

		إذا (strbuf_read(&نتيجو, cmd.ٻاهريون, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.ٻاهريون);
			أرجع -1;
		}
		close(cmd.ٻاهريون);

		إذا (پڄاڻي_command(&cmd)) {
			strbuf_release(&نتيجو);
			أرجع error(_("mktree reported failure"));
		}
		إذا (get_oid_hex(نتيجو.buf, oid) < 0) {
			strbuf_release(&نتيجو);
			أرجع error(_("mktree did not return an object name"));
		}

		strbuf_release(&نتيجو);
	} وإلا {
		بنية stat st;
		صحيح flags = اشاريو_FORMAT_CHECK | اشاريو_WRITE_OBJECT;

		إذا (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			أرجع -1;
		}
		إذا (اشاريو_fd(the_repository->اشاريو, oid, fd, &st, type, عدم, flags) < 0)
			أرجع error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	أرجع 0;
}

ساكن صحيح edit_and_replace(ثابت حرف *object_ref, صحيح force, صحيح raw)
{
	حرف *tmpfile;
	تعداد object_type type;
	بنية object_id old_oid, new_oid, prev;
	بنية strbuf ref = STRBUF_INIT;

	إذا (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		أرجع error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, عدم);
	إذا (type < 0)
		أرجع error(_("unable to get object type for %s"),
			     oid_منزل_hex(&old_oid));

	إذا (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		أرجع -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_رستو(the_repository, "REPLACE_EDITOBJ");
	إذا (export_object(&old_oid, type, raw, tmpfile)) {
		حرر(tmpfile);
		أرجع -1;
	}
	إذا (launch_editor(tmpfile, عدم, عدم) < 0) {
		حرر(tmpfile);
		أرجع error(_("editing object file failed"));
	}
	إذا (import_object(&new_oid, type, raw, tmpfile)) {
		حرر(tmpfile);
		أرجع -1;
	}
	حرر(tmpfile);

	إذا (oideq(&old_oid, &new_oid))
		أرجع error(_("new object is the same as the old one: '%s'"), oid_منزل_hex(&old_oid));

	أرجع replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ساكن صحيح replace_parents(بنية strbuf *buf, صحيح argc, ثابت حرف **argv)
{
	بنية strbuf new_parents = STRBUF_INIT;
	ثابت حرف *والد_شروعات, *والد_پڇاڙي;
	صحيح i;
	ثابت بنانشان hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	والد_شروعات = buf->buf;
	والد_شروعات += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	والد_پڇاڙي = والد_شروعات;

	طالما (starts_with(والد_پڇاڙي, "parent "))
		والد_پڇاڙي += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	لكل (i = 0; i < argc; i++) {
		بنية object_id oid;
		بنية commit *commit;

		إذا (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			أرجع error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		إذا (!commit) {
			strbuf_release(&new_parents);
			أرجع error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_منزل_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, والد_شروعات - buf->buf, والد_پڇاڙي - والد_شروعات,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	أرجع 0;
}

بنية check_mergetag_معلومات {
	صحيح argc;
	ثابت حرف **argv;
};

ساكن صحيح check_one_mergetag(بنية commit *commit اڻ_استعمال_ٿيل,
			       بنية commit_extra_سرو *extra,
			       فراغ *معلومات)
{
	بنية check_mergetag_معلومات *mergetag_معلومات = (بنية check_mergetag_معلومات *)معلومات;
	ثابت حرف *ref = mergetag_معلومات->argv[0];
	بنية object_id ٽيگ_oid;
	بنية ٽيگ *ٽيگ;
	صحيح i;

	hash_object_file(the_hash_algo, extra->قدر, extra->len,
			 OBJ_ٽيگ, &ٽيگ_oid);
	ٽيگ = lookup_ٽيگ(the_repository, &ٽيگ_oid);
	إذا (!ٽيگ)
		أرجع error(_("bad mergetag in commit '%s'"), ref);
	إذا (parse_ٽيگ_بفر(the_repository, ٽيگ, extra->قدر, extra->len))
		أرجع error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	لكل (i = 1; i < mergetag_معلومات->argc; i++) {
		بنية object_id oid;
		إذا (repo_get_oid(the_repository, mergetag_معلومات->argv[i], &oid) < 0)
			أرجع error(_("not a valid object name: '%s'"),
				     mergetag_معلومات->argv[i]);
		إذا (oideq(get_tagged_oid(ٽيگ), &oid))
			أرجع 0; /*مليو*/
	}

	أرجع error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_منزل_hex(&ٽيگ_oid));
}

ساكن صحيح check_mergetags(بنية commit *commit, صحيح argc, ثابت حرف **argv)
{
	بنية check_mergetag_معلومات mergetag_معلومات;

	mergetag_معلومات.argc = argc;
	mergetag_معلومات.argv = argv;
	أرجع for_each_mergetag(check_one_mergetag, commit, &mergetag_معلومات);
}

ساكن صحيح create_graft(صحيح argc, ثابت حرف **argv, صحيح force, صحيح gentle)
{
	بنية object_id old_oid, new_oid;
	ثابت حرف *old_ref = argv[0];
	بنية commit *commit;
	بنية strbuf buf = STRBUF_INIT;
	ثابت حرف *بفر;
	بنانشان ڊگهو سائيز;

	إذا (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		أرجع error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	إذا (!commit)
		أرجع error(_("could not parse %s"), old_ref);

	بفر = repo_get_commit_بفر(the_repository, commit, &سائيز);
	strbuf_جوڙيو(&buf, بفر, سائيز);
	repo_unuse_commit_بفر(the_repository, commit, بفر);

	إذا (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		أرجع -1;
	}

	إذا (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	إذا (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		أرجع -1;
	}

	إذا (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		أرجع error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	إذا (oideq(&commit->object.oid, &new_oid)) {
		إذا (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_منزل_hex(&commit->object.oid));
			أرجع 0;
		}
		أرجع error(_("new commit is the same as the old one: '%s'"),
			     oid_منزل_hex(&commit->object.oid));
	}

	أرجع replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ساكن صحيح convert_graft_file(صحيح force)
{
	ثابت حرف *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	بنية strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	بنية strvec دليلون = STRVEC_INIT;

	إذا (!fp)
		أرجع -1;

	no_graft_file_deprecated_advice = 1;
	طالما (strbuf_getline(&buf, fp) != EOF) {
		إذا (*buf.buf == '#')
			استمر;

		strvec_ورهايو(&دليلون, buf.buf);
		إذا (دليلون.nr && create_graft(دليلون.nr, دليلون.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&دليلون);
	}
	fclose(fp);

	strbuf_release(&buf);

	إذا (!err.len)
		أرجع unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	أرجع -1;
}

صحيح cmd_replace(صحيح argc,
		ثابت حرف **argv,
		ثابت حرف *اڳياڙي,
		بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح force = 0;
	صحيح raw = 0;
	ثابت حرف *format = عدم;
	تعداد {
		گهڻائي_UNSPECIFIED = 0,
		گهڻائي_LIST,
		گهڻائي_DELETE,
		گهڻائي_EDIT,
		گهڻائي_GRAFT,
		گهڻائي_CONVERT_GRAFT_FILE,
		گهڻائي_REPLACE
	} cmdmode = گهڻائي_UNSPECIFIED;
	بنية option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list replace refs"), گهڻائي_LIST),
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete replace refs"), گهڻائي_DELETE),
		OPT_CMDMODE('e', "edit", &cmdmode, N_("edit existing object"), گهڻائي_EDIT),
		OPT_CMDMODE('g', "graft", &cmdmode, N_("change a commit's parents"), گهڻائي_GRAFT),
		OPT_CMDMODE(0, "convert-graft-file", &cmdmode, N_("convert existing graft file"), گهڻائي_CONVERT_GRAFT_FILE),
		OPT_BOOL_F('f', "force", &force, N_("replace the ref if it exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "raw", &raw, N_("do not pretty-print contents for --edit")),
		OPT_STRING(0, "format", &format, N_("format"), N_("use this format")),
		OPT_پڇاڙي()
	};

	disable_replace_refs();
	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, options, git_replace_usage, 0);

	إذا (!cmdmode)
		cmdmode = argc ? گهڻائي_REPLACE : گهڻائي_LIST;

	إذا (format && cmdmode != گهڻائي_LIST)
		usage_نياپو_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	إذا (force &&
	    cmdmode != گهڻائي_REPLACE &&
	    cmdmode != گهڻائي_EDIT &&
	    cmdmode != گهڻائي_GRAFT &&
	    cmdmode != گهڻائي_CONVERT_GRAFT_FILE)
		usage_نياپو_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	إذا (raw && cmdmode != گهڻائي_EDIT)
		usage_نياپو_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	بدل (cmdmode) {
	حالة گهڻائي_DELETE:
		إذا (argc < 1)
			usage_نياپو_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		أرجع for_each_replace_name(argv, delete_replace_ref);

	حالة گهڻائي_REPLACE:
		إذا (argc != 2)
			usage_نياپو_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		أرجع replace_object(argv[0], argv[1], force);

	حالة گهڻائي_EDIT:
		إذا (argc != 1)
			usage_نياپو_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		أرجع edit_and_replace(argv[0], force, raw);

	حالة گهڻائي_GRAFT:
		إذا (argc < 1)
			usage_نياپو_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		أرجع create_graft(argc, argv, force, 0);

	حالة گهڻائي_CONVERT_GRAFT_FILE:
		إذا (argc != 0)
			usage_نياپو_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		أرجع !!convert_graft_file(force);

	حالة گهڻائي_LIST:
		إذا (argc > 1)
			usage_نياپو_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		أرجع list_replace_refs(argv[0], format);

	افتراضي:
		BUG("invalid cmdmode %d", (صحيح)cmdmode);
	}
}
