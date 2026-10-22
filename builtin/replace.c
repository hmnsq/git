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

स्थिर ध्रुव वर्ण * ध्रुव git_replace_usage[] = {
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
	ध्रुव वर्ण *pattern;
	गणना replace_format format;
};

स्थिर पूर्णाङ्क show_reference(ध्रुव संरचना reference *ref, शून्य *cb_data)
{
	संरचना show_data *data = cb_data;

	यदि (!wildmatch(data->pattern, ref->name, 0)) {
		यदि (data->format == REPLACE_FORMAT_SHORT)
			म_लिखो("%s\n", ref->name);
		अन्यथा यदि (data->format == REPLACE_FORMAT_MEDIUM)
			म_लिखो("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		अन्यथा { /* data->format == REPLACE_FORMAT_LONG */
			संरचना object_id object;
			गणना object_type obj_type, repl_type;

			यदि (repo_get_oid(data->repo, ref->name, &object))
				प्रत्यागम error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, रिक्त);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, रिक्त);

			म_लिखो("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क list_replace_refs(ध्रुव वर्ण *pattern, ध्रुव वर्ण *format)
{
	संरचना show_data data;

	data.repo = the_repository;
	यदि (!pattern)
		pattern = "*";
	data.pattern = pattern;

	यदि (format == रिक्त || *format == '\0' || !म_भेद(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	अन्यथा यदि (!म_भेद(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	अन्यथा यदि (!म_भेद(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	अन्यथा
		प्रत्यागम error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (शून्य *)&data);

	प्रत्यागम 0;
}

प्रकारनिर्देश पूर्णाङ्क (*each_replace_name_fn)(ध्रुव वर्ण *name, ध्रुव वर्ण *ref,
				    ध्रुव संरचना object_id *oid);

स्थिर पूर्णाङ्क for_each_replace_name(ध्रुव वर्ण **argv, each_replace_name_fn fn)
{
	ध्रुव वर्ण **p, *full_hex;
	संरचना strbuf ref = STRBUF_INIT;
	माप_प्रकार base_len;
	पूर्णाङ्क had_error = 0;
	संरचना object_id oid;
	ध्रुव वर्ण *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	प्रति (p = argv; *p; p++) {
		यदि (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			अनुवर्त;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		यदि (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			अनुवर्त;
		}
		यदि (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	प्रत्यागम had_error;
}

स्थिर पूर्णाङ्क delete_replace_ref(ध्रुव वर्ण *name, ध्रुव वर्ण *ref,
			      ध्रुव संरचना object_id *oid)
{
	यदि (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, ref, oid, 0))
		प्रत्यागम 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क check_ref_valid(संरचना object_id *object,
			    संरचना object_id *prev,
			    संरचना strbuf *ref,
			    पूर्णाङ्क force)
{
	ध्रुव वर्ण *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	यदि (check_refname_format(ref->buf, 0))
		प्रत्यागम error(_("'%s' is not a valid ref name"), ref->buf);

	यदि (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	अन्यथा यदि (!force)
		प्रत्यागम error(_("replace ref '%s' already exists"), ref->buf);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क replace_object_oid(ध्रुव वर्ण *object_ref,
			       संरचना object_id *object,
			       ध्रुव वर्ण *replace_ref,
			       संरचना object_id *repl,
			       पूर्णाङ्क force)
{
	संरचना object_id prev;
	गणना object_type obj_type, repl_type;
	संरचना strbuf ref = STRBUF_INIT;
	संरचना ref_transaction *transaction;
	संरचना strbuf err = STRBUF_INIT;
	पूर्णाङ्क res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, रिक्त);
	repl_type = odb_read_object_info(the_repository->objects, repl, रिक्त);
	यदि (!force && obj_type != repl_type)
		प्रत्यागम error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	यदि (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		प्रत्यागम -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	यदि (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   रिक्त, रिक्त, 0, रिक्त, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	प्रत्यागम res;
}

स्थिर पूर्णाङ्क replace_object(ध्रुव वर्ण *object_ref, ध्रुव वर्ण *replace_ref, पूर्णाङ्क force)
{
	संरचना object_id object, repl;

	यदि (repo_get_oid(the_repository, object_ref, &object))
		प्रत्यागम error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	यदि (repo_get_oid(the_repository, replace_ref, &repl))
		प्रत्यागम error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	प्रत्यागम replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
स्थिर पूर्णाङ्क export_object(ध्रुव संरचना object_id *oid, गणना object_type type,
			  पूर्णाङ्क raw, ध्रुव वर्ण *filename)
{
	संरचना child_process cmd = CHILD_PROCESS_INIT;
	पूर्णाङ्क fd;

	fd = खोलो(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	यदि (fd < 0)
		प्रत्यागम error_errno(_("unable to open %s for writing"), filename);

	strvec_धकेलो(&cmd.args, "--no-replace-objects");
	strvec_धकेलो(&cmd.args, "cat-file");
	यदि (raw)
		strvec_धकेलो(&cmd.args, type_name(type));
	अन्यथा
		strvec_धकेलो(&cmd.args, "-p");
	strvec_धकेलो(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	यदि (run_command(&cmd))
		प्रत्यागम error(_("cat-file reported failure"));
	प्रत्यागम 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
स्थिर पूर्णाङ्क import_object(संरचना object_id *oid, गणना object_type type,
			  पूर्णाङ्क raw, ध्रुव वर्ण *filename)
{
	पूर्णाङ्क fd;

	fd = खोलो(filename, O_RDONLY);
	यदि (fd < 0)
		प्रत्यागम error_errno(_("unable to open %s for reading"), filename);

	यदि (!raw && type == OBJ_वृक्ष) {
		संरचना child_process cmd = CHILD_PROCESS_INIT;
		संरचना strbuf result = STRBUF_INIT;

		strvec_धकेलो(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		यदि (start_command(&cmd)) {
			बंद(fd);
			प्रत्यागम error(_("unable to spawn mktree"));
		}

		यदि (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			बंद(fd);
			बंद(cmd.out);
			प्रत्यागम -1;
		}
		बंद(cmd.out);

		यदि (finish_command(&cmd)) {
			strbuf_release(&result);
			प्रत्यागम error(_("mktree reported failure"));
		}
		यदि (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			प्रत्यागम error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} अन्यथा {
		संरचना stat st;
		पूर्णाङ्क flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		यदि (ख_स्थिति(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			बंद(fd);
			प्रत्यागम -1;
		}
		यदि (index_fd(the_repository->index, oid, fd, &st, type, रिक्त, flags) < 0)
			प्रत्यागम error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क edit_and_replace(ध्रुव वर्ण *object_ref, पूर्णाङ्क force, पूर्णाङ्क raw)
{
	वर्ण *क्षणिक_ख;
	गणना object_type type;
	संरचना object_id old_oid, new_oid, prev;
	संरचना strbuf ref = STRBUF_INIT;

	यदि (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		प्रत्यागम error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, रिक्त);
	यदि (type < 0)
		प्रत्यागम error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	यदि (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		प्रत्यागम -1;
	}
	strbuf_release(&ref);

	क्षणिक_ख = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	यदि (export_object(&old_oid, type, raw, क्षणिक_ख)) {
		मुक्त(क्षणिक_ख);
		प्रत्यागम -1;
	}
	यदि (launch_editor(क्षणिक_ख, रिक्त, रिक्त) < 0) {
		मुक्त(क्षणिक_ख);
		प्रत्यागम error(_("editing object file failed"));
	}
	यदि (import_object(&new_oid, type, raw, क्षणिक_ख)) {
		मुक्त(क्षणिक_ख);
		प्रत्यागम -1;
	}
	मुक्त(क्षणिक_ख);

	यदि (oideq(&old_oid, &new_oid))
		प्रत्यागम error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	प्रत्यागम replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

स्थिर पूर्णाङ्क replace_parents(संरचना strbuf *buf, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना strbuf new_parents = STRBUF_INIT;
	ध्रुव वर्ण *parent_start, *parent_end;
	पूर्णाङ्क i;
	ध्रुव अचिन्हित hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	यावत् (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	प्रति (i = 0; i < argc; i++) {
		संरचना object_id oid;
		संरचना commit *commit;

		यदि (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			प्रत्यागम error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		यदि (!commit) {
			strbuf_release(&new_parents);
			प्रत्यागम error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	प्रत्यागम 0;
}

संरचना check_mergetag_data {
	पूर्णाङ्क argc;
	ध्रुव वर्ण **argv;
};

स्थिर पूर्णाङ्क check_one_mergetag(संरचना commit *commit UNUSED,
			       संरचना commit_extra_header *extra,
			       शून्य *data)
{
	संरचना check_mergetag_data *mergetag_data = (संरचना check_mergetag_data *)data;
	ध्रुव वर्ण *ref = mergetag_data->argv[0];
	संरचना object_id tag_oid;
	संरचना tag *tag;
	पूर्णाङ्क i;

	hash_object_file(the_hash_algo, extra->मूल्य, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	यदि (!tag)
		प्रत्यागम error(_("bad mergetag in commit '%s'"), ref);
	यदि (parse_tag_buffer(the_repository, tag, extra->मूल्य, extra->len))
		प्रत्यागम error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	प्रति (i = 1; i < mergetag_data->argc; i++) {
		संरचना object_id oid;
		यदि (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			प्रत्यागम error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		यदि (oideq(get_tagged_oid(tag), &oid))
			प्रत्यागम 0; /* found */
	}

	प्रत्यागम error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

स्थिर पूर्णाङ्क check_mergetags(संरचना commit *commit, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	संरचना check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	प्रत्यागम for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

स्थिर पूर्णाङ्क create_graft(पूर्णाङ्क argc, ध्रुव वर्ण **argv, पूर्णाङ्क force, पूर्णाङ्क gentle)
{
	संरचना object_id old_oid, new_oid;
	ध्रुव वर्ण *old_ref = argv[0];
	संरचना commit *commit;
	संरचना strbuf buf = STRBUF_INIT;
	ध्रुव वर्ण *buffer;
	अचिन्हित दीर्घ size;

	यदि (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		प्रत्यागम error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	यदि (!commit)
		प्रत्यागम error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	यदि (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		प्रत्यागम -1;
	}

	यदि (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	यदि (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		प्रत्यागम -1;
	}

	यदि (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		प्रत्यागम error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	यदि (oideq(&commit->object.oid, &new_oid)) {
		यदि (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			प्रत्यागम 0;
		}
		प्रत्यागम error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	प्रत्यागम replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

स्थिर पूर्णाङ्क convert_graft_file(पूर्णाङ्क force)
{
	ध्रुव वर्ण *graft_file = repo_get_graft_file(the_repository);
	खाता *fp = fopen_or_warn(graft_file, "r");
	संरचना strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	संरचना strvec args = STRVEC_INIT;

	यदि (!fp)
		प्रत्यागम -1;

	no_graft_file_deprecated_advice = 1;
	यावत् (strbuf_getline(&buf, fp) != खातापूर्ण) {
		यदि (*buf.buf == '#')
			अनुवर्त;

		strvec_split(&args, buf.buf);
		यदि (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	ख_बंद(fp);

	strbuf_release(&buf);

	यदि (!err.len)
		प्रत्यागम unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	प्रत्यागम -1;
}

पूर्णाङ्क cmd_replace(पूर्णाङ्क argc,
		ध्रुव वर्ण **argv,
		ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	पूर्णाङ्क force = 0;
	पूर्णाङ्क raw = 0;
	ध्रुव वर्ण *format = रिक्त;
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

	यदि (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	यदि (format && cmdmode != MODE_LIST)
		usage_संदेश_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	यदि (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_संदेश_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	यदि (raw && cmdmode != MODE_EDIT)
		usage_संदेश_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	विकल्प (cmdmode) {
	पक्ष MODE_DELETE:
		यदि (argc < 1)
			usage_संदेश_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		प्रत्यागम for_each_replace_name(argv, delete_replace_ref);

	पक्ष MODE_REPLACE:
		यदि (argc != 2)
			usage_संदेश_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		प्रत्यागम replace_object(argv[0], argv[1], force);

	पक्ष MODE_EDIT:
		यदि (argc != 1)
			usage_संदेश_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		प्रत्यागम edit_and_replace(argv[0], force, raw);

	पक्ष MODE_GRAFT:
		यदि (argc < 1)
			usage_संदेश_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		प्रत्यागम create_graft(argc, argv, force, 0);

	पक्ष MODE_CONVERT_GRAFT_FILE:
		यदि (argc != 0)
			usage_संदेश_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		प्रत्यागम !!convert_graft_file(force);

	पक्ष MODE_LIST:
		यदि (argc > 1)
			usage_संदेश_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		प्रत्यागम list_replace_refs(argv[0], format);

	स्वभाव:
		BUG("invalid cmdmode %d", (पूर्णाङ्क)cmdmode);
	}
}
