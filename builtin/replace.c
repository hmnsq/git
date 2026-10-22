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
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "path.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "replace-object.h"
#अन्तर्भाव "tag.h"
#अन्तर्भाव "wildmatch.h"

स्थिर स्थिरांक अक्षर * स्थिरांक git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	रिक्त
};

गणना replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

संरचना show_data {
	संरचना repository *repo;
	स्थिरांक अक्षर *pattern;
	गणना replace_format format;
};

स्थिर पूर्णांक show_reference(स्थिरांक संरचना reference *ref, शून्य *cb_data)
{
	संरचना show_data *data = cb_data;

	जर (!wildmatch(data->pattern, ref->name, 0)) {
		जर (data->format == REPLACE_FORMAT_SHORT)
			म_लिखो("%s\n", ref->name);
		अन्यथा जर (data->format == REPLACE_FORMAT_MEDIUM)
			म_लिखो("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		अन्यथा { /* data->format == REPLACE_FORMAT_LONG */
			संरचना object_id object;
			गणना object_type obj_type, repl_type;

			जर (repo_get_oid(data->repo, ref->name, &object))
				परत error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, रिक्त);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, रिक्त);

			म_लिखो("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	परत 0;
}

स्थिर पूर्णांक list_replace_refs(स्थिरांक अक्षर *pattern, स्थिरांक अक्षर *format)
{
	संरचना show_data data;

	data.repo = the_repository;
	जर (!pattern)
		pattern = "*";
	data.pattern = pattern;

	जर (format == रिक्त || *format == '\0' || !म_भेद(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	अन्यथा जर (!म_भेद(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	अन्यथा जर (!म_भेद(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	अन्यथा
		परत error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (शून्य *)&data);

	परत 0;
}

प्रकारव्याख्या पूर्णांक (*each_replace_name_fn)(स्थिरांक अक्षर *name, स्थिरांक अक्षर *ref,
				    स्थिरांक संरचना object_id *oid);

स्थिर पूर्णांक for_each_replace_name(स्थिरांक अक्षर **argv, each_replace_name_fn fn)
{
	स्थिरांक अक्षर **p, *full_hex;
	संरचना strbuf ref = STRBUF_INIT;
	माप_प्रकार base_len;
	पूर्णांक had_error = 0;
	संरचना object_id oid;
	स्थिरांक अक्षर *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	साठी (p = argv; *p; p++) {
		जर (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			सुरूठेवा;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		जर (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			सुरूठेवा;
		}
		जर (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	परत had_error;
}

स्थिर पूर्णांक delete_replace_ref(स्थिरांक अक्षर *name, स्थिरांक अक्षर *ref,
			      स्थिरांक संरचना object_id *oid)
{
	जर (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, ref, oid, 0))
		परत 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	परत 0;
}

स्थिर पूर्णांक check_ref_valid(संरचना object_id *object,
			    संरचना object_id *prev,
			    संरचना strbuf *ref,
			    पूर्णांक force)
{
	स्थिरांक अक्षर *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	जर (check_refname_format(ref->buf, 0))
		परत error(_("'%s' is not a valid ref name"), ref->buf);

	जर (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	अन्यथा जर (!force)
		परत error(_("replace ref '%s' already exists"), ref->buf);
	परत 0;
}

स्थिर पूर्णांक replace_object_oid(स्थिरांक अक्षर *object_ref,
			       संरचना object_id *object,
			       स्थिरांक अक्षर *replace_ref,
			       संरचना object_id *repl,
			       पूर्णांक force)
{
	संरचना object_id prev;
	गणना object_type obj_type, repl_type;
	संरचना strbuf ref = STRBUF_INIT;
	संरचना ref_transaction *transaction;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णांक res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, रिक्त);
	repl_type = odb_read_object_info(the_repository->objects, repl, रिक्त);
	जर (!force && obj_type != repl_type)
		परत error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	जर (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		परत -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	जर (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   रिक्त, रिक्त, 0, रिक्त, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	परत res;
}

स्थिर पूर्णांक replace_object(स्थिरांक अक्षर *object_ref, स्थिरांक अक्षर *replace_ref, पूर्णांक force)
{
	संरचना object_id object, repl;

	जर (repo_get_oid(the_repository, object_ref, &object))
		परत error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	जर (repo_get_oid(the_repository, replace_ref, &repl))
		परत error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	परत replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
स्थिर पूर्णांक export_object(स्थिरांक संरचना object_id *oid, गणना object_type type,
			  पूर्णांक raw, स्थिरांक अक्षर *filename)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णांक fd;

	fd = खोलो(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	जर (fd < 0)
		परत error_errno(_("unable to open %s for writing"), filename);

	strvec_ढकला(&cmd.args, "--no-replace-objects");
	strvec_ढकला(&cmd.args, "cat-file");
	जर (raw)
		strvec_ढकला(&cmd.args, type_name(type));
	अन्यथा
		strvec_ढकला(&cmd.args, "-p");
	strvec_ढकला(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	जर (run_command(&cmd))
		परत error(_("cat-file reported failure"));
	परत 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
स्थिर पूर्णांक import_object(संरचना object_id *oid, गणना object_type type,
			  पूर्णांक raw, स्थिरांक अक्षर *filename)
{
	पूर्णांक fd;

	fd = खोलो(filename, O_RDONLY);
	जर (fd < 0)
		परत error_errno(_("unable to open %s for reading"), filename);

	जर (!raw && type == OBJ_वृक्ष) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		संरचना strbuf result = STRBUF_INIT;

		strvec_ढकला(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		जर (start_command(&cmd)) {
			बंद(fd);
			परत error(_("unable to spawn mktree"));
		}

		जर (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			बंद(fd);
			बंद(cmd.out);
			परत -1;
		}
		बंद(cmd.out);

		जर (finish_command(&cmd)) {
			strbuf_release(&result);
			परत error(_("mktree reported failure"));
		}
		जर (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			परत error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} अन्यथा {
		संरचना stat st;
		पूर्णांक flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		जर (ख_स्थिति(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			बंद(fd);
			परत -1;
		}
		जर (index_fd(the_repository->index, oid, fd, &st, type, रिक्त, flags) < 0)
			परत error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	परत 0;
}

स्थिर पूर्णांक edit_and_replace(स्थिरांक अक्षर *object_ref, पूर्णांक force, पूर्णांक raw)
{
	अक्षर *क्षणिक_ख;
	गणना object_type type;
	संरचना object_id old_oid, new_oid, prev;
	संरचना strbuf ref = STRBUF_INIT;

	जर (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		परत error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, रिक्त);
	जर (type < 0)
		परत error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	जर (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		परत -1;
	}
	strbuf_release(&ref);

	क्षणिक_ख = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	जर (export_object(&old_oid, type, raw, क्षणिक_ख)) {
		मुक्त(क्षणिक_ख);
		परत -1;
	}
	जर (launch_editor(क्षणिक_ख, रिक्त, रिक्त) < 0) {
		मुक्त(क्षणिक_ख);
		परत error(_("editing object file failed"));
	}
	जर (import_object(&new_oid, type, raw, क्षणिक_ख)) {
		मुक्त(क्षणिक_ख);
		परत -1;
	}
	मुक्त(क्षणिक_ख);

	जर (oideq(&old_oid, &new_oid))
		परत error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	परत replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

स्थिर पूर्णांक replace_parents(संरचना strbuf *buf, पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना strbuf new_parents = STRBUF_INIT;
	स्थिरांक अक्षर *parent_start, *parent_end;
	पूर्णांक i;
	स्थिरांक अचिन्हित hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	पर्यंत (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	साठी (i = 0; i < argc; i++) {
		संरचना object_id oid;
		संरचना commit *commit;

		जर (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			परत error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		जर (!commit) {
			strbuf_release(&new_parents);
			परत error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	परत 0;
}

संरचना check_mergetag_data {
	पूर्णांक argc;
	स्थिरांक अक्षर **argv;
};

स्थिर पूर्णांक check_one_mergetag(संरचना commit *commit UNUSED,
			       संरचना commit_extra_header *extra,
			       शून्य *data)
{
	संरचना check_mergetag_data *mergetag_data = (संरचना check_mergetag_data *)data;
	स्थिरांक अक्षर *ref = mergetag_data->argv[0];
	संरचना object_id tag_oid;
	संरचना tag *tag;
	पूर्णांक i;

	hash_object_file(the_hash_algo, extra->मूल्य, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	जर (!tag)
		परत error(_("bad mergetag in commit '%s'"), ref);
	जर (parse_tag_buffer(the_repository, tag, extra->मूल्य, extra->len))
		परत error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	साठी (i = 1; i < mergetag_data->argc; i++) {
		संरचना object_id oid;
		जर (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			परत error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		जर (oideq(get_tagged_oid(tag), &oid))
			परत 0; /* found */
	}

	परत error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

स्थिर पूर्णांक check_mergetags(संरचना commit *commit, पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	संरचना check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	परत for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

स्थिर पूर्णांक create_graft(पूर्णांक argc, स्थिरांक अक्षर **argv, पूर्णांक force, पूर्णांक gentle)
{
	संरचना object_id old_oid, new_oid;
	स्थिरांक अक्षर *old_ref = argv[0];
	संरचना commit *commit;
	संरचना strbuf buf = STRBUF_INIT;
	स्थिरांक अक्षर *buffer;
	अचिन्हित दीर्घ size;

	जर (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		परत error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	जर (!commit)
		परत error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	जर (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		परत -1;
	}

	जर (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	जर (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		परत -1;
	}

	जर (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		परत error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	जर (oideq(&commit->object.oid, &new_oid)) {
		जर (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			परत 0;
		}
		परत error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	परत replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

स्थिर पूर्णांक convert_graft_file(पूर्णांक force)
{
	स्थिरांक अक्षर *graft_file = repo_get_graft_file(the_repository);
	खाता *fp = fopen_or_warn(graft_file, "r");
	संरचना strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	संरचना strvec args = STRVEC_INIT;

	जर (!fp)
		परत -1;

	no_graft_file_deprecated_advice = 1;
	पर्यंत (strbuf_getline(&buf, fp) != खातापूर्ण) {
		जर (*buf.buf == '#')
			सुरूठेवा;

		strvec_split(&args, buf.buf);
		जर (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	ख_बंद(fp);

	strbuf_release(&buf);

	जर (!err.len)
		परत unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	परत -1;
}

पूर्णांक cmd_replace(पूर्णांक argc,
		स्थिरांक अक्षर **argv,
		स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	पूर्णांक force = 0;
	पूर्णांक raw = 0;
	स्थिरांक अक्षर *format = रिक्त;
	गणना {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	संरचना option options[] = {
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
	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	जर (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	जर (format && cmdmode != MODE_LIST)
		usage_संदेश_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	जर (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_संदेश_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	जर (raw && cmdmode != MODE_EDIT)
		usage_संदेश_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	निवड (cmdmode) {
	स्थिती MODE_DELETE:
		जर (argc < 1)
			usage_संदेश_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		परत for_each_replace_name(argv, delete_replace_ref);

	स्थिती MODE_REPLACE:
		जर (argc != 2)
			usage_संदेश_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		परत replace_object(argv[0], argv[1], force);

	स्थिती MODE_EDIT:
		जर (argc != 1)
			usage_संदेश_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		परत edit_and_replace(argv[0], force, raw);

	स्थिती MODE_GRAFT:
		जर (argc < 1)
			usage_संदेश_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		परत create_graft(argc, argv, force, 0);

	स्थिती MODE_CONVERT_GRAFT_FILE:
		जर (argc != 0)
			usage_संदेश_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		परत !!convert_graft_file(force);

	स्थिती MODE_LIST:
		जर (argc > 1)
			usage_संदेश_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		परत list_replace_refs(argv[0], format);

	पूर्वनिर्धारित:
		BUG("invalid cmdmode %d", (पूर्णांक)cmdmode);
	}
}
