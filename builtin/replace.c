/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "refs.h"
#समावेश "parse-options.h"
#समावेश "path.h"
#समावेश "run-command.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "replace-object.h"
#समावेश "tag.h"
#समावेश "wildmatch.h"

अटल स्थिर अक्षर * स्थिर git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	शून्य
};

क्रमागत replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

काष्ठा show_तथ्य {
	काष्ठा repository *repo;
	स्थिर अक्षर *pattern;
	क्रमागत replace_format format;
};

अटल पूर्णांक show_reference(स्थिर काष्ठा reference *ref, व्योम *cb_तथ्य)
{
	काष्ठा show_तथ्य *तथ्य = cb_तथ्य;

	अगर (!wildmatch(तथ्य->pattern, ref->name, 0)) {
		अगर (तथ्य->format == REPLACE_FORMAT_SHORT)
			म_लिखो("%s\n", ref->name);
		अन्यथा अगर (तथ्य->format == REPLACE_FORMAT_MEDIUM)
			म_लिखो("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		अन्यथा { /* data->format == REPLACE_FORMAT_LONG */
			काष्ठा object_id object;
			क्रमागत object_type obj_type, repl_type;

			अगर (repo_get_oid(तथ्य->repo, ref->name, &object))
				वापस error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(तथ्य->repo->objects, &object, शून्य);
			repl_type = odb_read_object_info(तथ्य->repo->objects, ref->oid, शून्य);

			म_लिखो("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	वापस 0;
}

अटल पूर्णांक list_replace_refs(स्थिर अक्षर *pattern, स्थिर अक्षर *format)
{
	काष्ठा show_तथ्य तथ्य;

	तथ्य.repo = the_repository;
	अगर (!pattern)
		pattern = "*";
	तथ्य.pattern = pattern;

	अगर (format == शून्य || *format == '\0' || !म_भेद(format, "short"))
		तथ्य.format = REPLACE_FORMAT_SHORT;
	अन्यथा अगर (!म_भेद(format, "medium"))
		तथ्य.format = REPLACE_FORMAT_MEDIUM;
	अन्यथा अगर (!म_भेद(format, "long"))
		तथ्य.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	अन्यथा
		वापस error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (व्योम *)&तथ्य);

	वापस 0;
}

प्रकार पूर्णांक (*each_replace_name_fn)(स्थिर अक्षर *name, स्थिर अक्षर *ref,
				    स्थिर काष्ठा object_id *oid);

अटल पूर्णांक for_each_replace_name(स्थिर अक्षर **argv, each_replace_name_fn fn)
{
	स्थिर अक्षर **p, *full_hex;
	काष्ठा strbuf ref = STRBUF_INIT;
	माप_प्रकार base_len;
	पूर्णांक had_error = 0;
	काष्ठा object_id oid;
	स्थिर अक्षर *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	क्रम (p = argv; *p; p++) {
		अगर (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			जारी;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		अगर (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			जारी;
		}
		अगर (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	वापस had_error;
}

अटल पूर्णांक delete_replace_ref(स्थिर अक्षर *name, स्थिर अक्षर *ref,
			      स्थिर काष्ठा object_id *oid)
{
	अगर (refs_delete_ref(get_main_ref_store(the_repository), शून्य, ref, oid, 0))
		वापस 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	वापस 0;
}

अटल पूर्णांक check_ref_valid(काष्ठा object_id *object,
			    काष्ठा object_id *पूर्व,
			    काष्ठा strbuf *ref,
			    पूर्णांक force)
{
	स्थिर अक्षर *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	अगर (check_refname_format(ref->buf, 0))
		वापस error(_("'%s' is not a valid ref name"), ref->buf);

	अगर (refs_read_ref(get_main_ref_store(the_repository), ref->buf, पूर्व))
		oidclr(पूर्व, the_repository->hash_algo);
	अन्यथा अगर (!force)
		वापस error(_("replace ref '%s' already exists"), ref->buf);
	वापस 0;
}

अटल पूर्णांक replace_object_oid(स्थिर अक्षर *object_ref,
			       काष्ठा object_id *object,
			       स्थिर अक्षर *replace_ref,
			       काष्ठा object_id *repl,
			       पूर्णांक force)
{
	काष्ठा object_id पूर्व;
	क्रमागत object_type obj_type, repl_type;
	काष्ठा strbuf ref = STRBUF_INIT;
	काष्ठा ref_transaction *transaction;
	काष्ठा strbuf err = STRBUF_INIT;
	पूर्णांक res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, शून्य);
	repl_type = odb_read_object_info(the_repository->objects, repl, शून्य);
	अगर (!force && obj_type != repl_type)
		वापस error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	अगर (check_ref_valid(object, &पूर्व, &ref, force)) {
		strbuf_release(&ref);
		वापस -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	अगर (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &पूर्व,
				   शून्य, शून्य, 0, शून्य, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	वापस res;
}

अटल पूर्णांक replace_object(स्थिर अक्षर *object_ref, स्थिर अक्षर *replace_ref, पूर्णांक force)
{
	काष्ठा object_id object, repl;

	अगर (repo_get_oid(the_repository, object_ref, &object))
		वापस error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	अगर (repo_get_oid(the_repository, replace_ref, &repl))
		वापस error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	वापस replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
अटल पूर्णांक export_object(स्थिर काष्ठा object_id *oid, क्रमागत object_type type,
			  पूर्णांक raw, स्थिर अक्षर *filename)
{
	काष्ठा child_process cmd = CHILD_PROCESS_INIT;
	पूर्णांक fd;

	fd = खोलो(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	अगर (fd < 0)
		वापस error_errno(_("unable to open %s for writing"), filename);

	strvec_धकेलो(&cmd.args, "--no-replace-objects");
	strvec_धकेलो(&cmd.args, "cat-file");
	अगर (raw)
		strvec_धकेलो(&cmd.args, type_name(type));
	अन्यथा
		strvec_धकेलो(&cmd.args, "-p");
	strvec_धकेलो(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	अगर (run_command(&cmd))
		वापस error(_("cat-file reported failure"));
	वापस 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
अटल पूर्णांक import_object(काष्ठा object_id *oid, क्रमागत object_type type,
			  पूर्णांक raw, स्थिर अक्षर *filename)
{
	पूर्णांक fd;

	fd = खोलो(filename, O_RDONLY);
	अगर (fd < 0)
		वापस error_errno(_("unable to open %s for reading"), filename);

	अगर (!raw && type == OBJ_वृक्ष) {
		काष्ठा child_process cmd = CHILD_PROCESS_INIT;
		काष्ठा strbuf परिणाम = STRBUF_INIT;

		strvec_धकेलो(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		अगर (start_command(&cmd)) {
			बंद(fd);
			वापस error(_("unable to spawn mktree"));
		}

		अगर (strbuf_read(&परिणाम, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			बंद(fd);
			बंद(cmd.out);
			वापस -1;
		}
		बंद(cmd.out);

		अगर (finish_command(&cmd)) {
			strbuf_release(&परिणाम);
			वापस error(_("mktree reported failure"));
		}
		अगर (get_oid_hex(परिणाम.buf, oid) < 0) {
			strbuf_release(&परिणाम);
			वापस error(_("mktree did not return an object name"));
		}

		strbuf_release(&परिणाम);
	} अन्यथा {
		काष्ठा stat st;
		पूर्णांक flags = सूचकांक_FORMAT_CHECK | सूचकांक_WRITE_OBJECT;

		अगर (ख_स्थिति(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			बंद(fd);
			वापस -1;
		}
		अगर (सूचकांक_fd(the_repository->सूचकांक, oid, fd, &st, type, शून्य, flags) < 0)
			वापस error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	वापस 0;
}

अटल पूर्णांक edit_and_replace(स्थिर अक्षर *object_ref, पूर्णांक force, पूर्णांक raw)
{
	अक्षर *क्षणिक_ख;
	क्रमागत object_type type;
	काष्ठा object_id old_oid, new_oid, पूर्व;
	काष्ठा strbuf ref = STRBUF_INIT;

	अगर (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		वापस error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, शून्य);
	अगर (type < 0)
		वापस error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	अगर (check_ref_valid(&old_oid, &पूर्व, &ref, force)) {
		strbuf_release(&ref);
		वापस -1;
	}
	strbuf_release(&ref);

	क्षणिक_ख = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	अगर (export_object(&old_oid, type, raw, क्षणिक_ख)) {
		मुक्त(क्षणिक_ख);
		वापस -1;
	}
	अगर (launch_editor(क्षणिक_ख, शून्य, शून्य) < 0) {
		मुक्त(क्षणिक_ख);
		वापस error(_("editing object file failed"));
	}
	अगर (import_object(&new_oid, type, raw, क्षणिक_ख)) {
		मुक्त(क्षणिक_ख);
		वापस -1;
	}
	मुक्त(क्षणिक_ख);

	अगर (oideq(&old_oid, &new_oid))
		वापस error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	वापस replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

अटल पूर्णांक replace_parents(काष्ठा strbuf *buf, पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा strbuf new_parents = STRBUF_INIT;
	स्थिर अक्षर *parent_start, *parent_end;
	पूर्णांक i;
	स्थिर अचिन्हित hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	जबतक (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	क्रम (i = 0; i < argc; i++) {
		काष्ठा object_id oid;
		काष्ठा commit *commit;

		अगर (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			वापस error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		अगर (!commit) {
			strbuf_release(&new_parents);
			वापस error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	वापस 0;
}

काष्ठा check_mergetag_तथ्य {
	पूर्णांक argc;
	स्थिर अक्षर **argv;
};

अटल पूर्णांक check_one_mergetag(काष्ठा commit *commit UNUSED,
			       काष्ठा commit_extra_header *extra,
			       व्योम *तथ्य)
{
	काष्ठा check_mergetag_तथ्य *mergetag_तथ्य = (काष्ठा check_mergetag_तथ्य *)तथ्य;
	स्थिर अक्षर *ref = mergetag_तथ्य->argv[0];
	काष्ठा object_id tag_oid;
	काष्ठा tag *tag;
	पूर्णांक i;

	hash_object_file(the_hash_algo, extra->मूल्य, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	अगर (!tag)
		वापस error(_("bad mergetag in commit '%s'"), ref);
	अगर (parse_tag_बफर(the_repository, tag, extra->मूल्य, extra->len))
		वापस error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	क्रम (i = 1; i < mergetag_तथ्य->argc; i++) {
		काष्ठा object_id oid;
		अगर (repo_get_oid(the_repository, mergetag_तथ्य->argv[i], &oid) < 0)
			वापस error(_("not a valid object name: '%s'"),
				     mergetag_तथ्य->argv[i]);
		अगर (oideq(get_tagged_oid(tag), &oid))
			वापस 0; /* found */
	}

	वापस error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

अटल पूर्णांक check_mergetags(काष्ठा commit *commit, पूर्णांक argc, स्थिर अक्षर **argv)
{
	काष्ठा check_mergetag_तथ्य mergetag_तथ्य;

	mergetag_तथ्य.argc = argc;
	mergetag_तथ्य.argv = argv;
	वापस for_each_mergetag(check_one_mergetag, commit, &mergetag_तथ्य);
}

अटल पूर्णांक create_graft(पूर्णांक argc, स्थिर अक्षर **argv, पूर्णांक force, पूर्णांक gentle)
{
	काष्ठा object_id old_oid, new_oid;
	स्थिर अक्षर *old_ref = argv[0];
	काष्ठा commit *commit;
	काष्ठा strbuf buf = STRBUF_INIT;
	स्थिर अक्षर *बफर;
	अचिन्हित दीर्घ आकार;

	अगर (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		वापस error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	अगर (!commit)
		वापस error(_("could not parse %s"), old_ref);

	बफर = repo_get_commit_बफर(the_repository, commit, &आकार);
	strbuf_add(&buf, बफर, आकार);
	repo_unuse_commit_बफर(the_repository, commit, बफर);

	अगर (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		वापस -1;
	}

	अगर (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	अगर (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		वापस -1;
	}

	अगर (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		वापस error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	अगर (oideq(&commit->object.oid, &new_oid)) {
		अगर (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			वापस 0;
		}
		वापस error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	वापस replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

अटल पूर्णांक convert_graft_file(पूर्णांक force)
{
	स्थिर अक्षर *graft_file = repo_get_graft_file(the_repository);
	खाता *fp = fopen_or_warn(graft_file, "r");
	काष्ठा strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	काष्ठा strvec args = STRVEC_INIT;

	अगर (!fp)
		वापस -1;

	no_graft_file_deprecated_advice = 1;
	जबतक (strbuf_getline(&buf, fp) != खातापूर्ण) {
		अगर (*buf.buf == '#')
			जारी;

		strvec_split(&args, buf.buf);
		अगर (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	ख_बंद(fp);

	strbuf_release(&buf);

	अगर (!err.len)
		वापस unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	वापस -1;
}

पूर्णांक cmd_replace(पूर्णांक argc,
		स्थिर अक्षर **argv,
		स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	पूर्णांक force = 0;
	पूर्णांक raw = 0;
	स्थिर अक्षर *format = शून्य;
	क्रमागत {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	काष्ठा option options[] = {
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
	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	अगर (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	अगर (format && cmdmode != MODE_LIST)
		usage_संदेश_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	अगर (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_संदेश_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	अगर (raw && cmdmode != MODE_EDIT)
		usage_संदेश_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	चयन (cmdmode) {
	हाल MODE_DELETE:
		अगर (argc < 1)
			usage_संदेश_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		वापस for_each_replace_name(argv, delete_replace_ref);

	हाल MODE_REPLACE:
		अगर (argc != 2)
			usage_संदेश_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		वापस replace_object(argv[0], argv[1], force);

	हाल MODE_EDIT:
		अगर (argc != 1)
			usage_संदेश_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		वापस edit_and_replace(argv[0], force, raw);

	हाल MODE_GRAFT:
		अगर (argc < 1)
			usage_संदेश_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		वापस create_graft(argc, argv, force, 0);

	हाल MODE_CONVERT_GRAFT_FILE:
		अगर (argc != 0)
			usage_संदेश_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		वापस !!convert_graft_file(force);

	हाल MODE_LIST:
		अगर (argc > 1)
			usage_संदेश_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		वापस list_replace_refs(argv[0], format);

	शेष:
		BUG("invalid cmdmode %d", (पूर्णांक)cmdmode);
	}
}
