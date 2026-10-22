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
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "path.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "replace-object.h"
#அந்தர்பாவ "tag.h"
#அந்தர்பாவ "wildmatch.h"

நிலையான மாறிலி எழுத்து * மாறிலி git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	வெற்று
};

எண்ணிக்கை replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

கட்டமைப்பு show_data {
	கட்டமைப்பு repository *repo;
	மாறிலி எழுத்து *pattern;
	எண்ணிக்கை replace_format format;
};

நிலையான முழு show_reference(மாறிலி கட்டமைப்பு reference *ref, வெறுமை *cb_data)
{
	கட்டமைப்பு show_data *data = cb_data;

	எனில் (!wildmatch(data->pattern, ref->name, 0)) {
		எனில் (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		இல்லையெனில் எனில் (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		இல்லையெனில் { /* data->format == REPLACE_FORMAT_LONG */
			கட்டமைப்பு object_id object;
			எண்ணிக்கை object_type obj_type, repl_type;

			எனில் (repo_get_oid(data->repo, ref->name, &object))
				திரும்பு error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, வெற்று);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, வெற்று);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	திரும்பு 0;
}

நிலையான முழு list_replace_refs(மாறிலி எழுத்து *pattern, மாறிலி எழுத்து *format)
{
	கட்டமைப்பு show_data data;

	data.repo = the_repository;
	எனில் (!pattern)
		pattern = "*";
	data.pattern = pattern;

	எனில் (format == வெற்று || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	இல்லையெனில் எனில் (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	இல்லையெனில் எனில் (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	இல்லையெனில்
		திரும்பு error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (வெறுமை *)&data);

	திரும்பு 0;
}

வகைவரை முழு (*each_replace_name_fn)(மாறிலி எழுத்து *name, மாறிலி எழுத்து *ref,
				    மாறிலி கட்டமைப்பு object_id *oid);

நிலையான முழு for_each_replace_name(மாறிலி எழுத்து **argv, each_replace_name_fn fn)
{
	மாறிலி எழுத்து **p, *full_hex;
	கட்டமைப்பு strbuf ref = STRBUF_INIT;
	size_t base_len;
	முழு had_error = 0;
	கட்டமைப்பு object_id oid;
	மாறிலி எழுத்து *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	க்கு (p = argv; *p; p++) {
		எனில் (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			தொடர்;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		எனில் (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			தொடர்;
		}
		எனில் (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	திரும்பு had_error;
}

நிலையான முழு delete_replace_ref(மாறிலி எழுத்து *name, மாறிலி எழுத்து *ref,
			      மாறிலி கட்டமைப்பு object_id *oid)
{
	எனில் (refs_delete_ref(get_main_ref_store(the_repository), வெற்று, ref, oid, 0))
		திரும்பு 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	திரும்பு 0;
}

நிலையான முழு check_ref_valid(கட்டமைப்பு object_id *object,
			    கட்டமைப்பு object_id *prev,
			    கட்டமைப்பு strbuf *ref,
			    முழு force)
{
	மாறிலி எழுத்து *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	எனில் (check_refname_format(ref->buf, 0))
		திரும்பு error(_("'%s' is not a valid ref name"), ref->buf);

	எனில் (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	இல்லையெனில் எனில் (!force)
		திரும்பு error(_("replace ref '%s' already exists"), ref->buf);
	திரும்பு 0;
}

நிலையான முழு replace_object_oid(மாறிலி எழுத்து *object_ref,
			       கட்டமைப்பு object_id *object,
			       மாறிலி எழுத்து *replace_ref,
			       கட்டமைப்பு object_id *repl,
			       முழு force)
{
	கட்டமைப்பு object_id prev;
	எண்ணிக்கை object_type obj_type, repl_type;
	கட்டமைப்பு strbuf ref = STRBUF_INIT;
	கட்டமைப்பு ref_transaction *transaction;
	கட்டமைப்பு strbuf err = STRBUF_INIT;
	முழு res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, வெற்று);
	repl_type = odb_read_object_info(the_repository->objects, repl, வெற்று);
	எனில் (!force && obj_type != repl_type)
		திரும்பு error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	எனில் (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		திரும்பு -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	எனில் (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   வெற்று, வெற்று, 0, வெற்று, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	திரும்பு res;
}

நிலையான முழு replace_object(மாறிலி எழுத்து *object_ref, மாறிலி எழுத்து *replace_ref, முழு force)
{
	கட்டமைப்பு object_id object, repl;

	எனில் (repo_get_oid(the_repository, object_ref, &object))
		திரும்பு error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	எனில் (repo_get_oid(the_repository, replace_ref, &repl))
		திரும்பு error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	திரும்பு replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
நிலையான முழு export_object(மாறிலி கட்டமைப்பு object_id *oid, எண்ணிக்கை object_type type,
			  முழு raw, மாறிலி எழுத்து *filename)
{
	கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
	முழு fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	எனில் (fd < 0)
		திரும்பு error_errno(_("unable to open %s for writing"), filename);

	strvec_தள்ளு(&cmd.args, "--no-replace-objects");
	strvec_தள்ளு(&cmd.args, "cat-file");
	எனில் (raw)
		strvec_தள்ளு(&cmd.args, type_name(type));
	இல்லையெனில்
		strvec_தள்ளு(&cmd.args, "-p");
	strvec_தள்ளு(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	எனில் (run_command(&cmd))
		திரும்பு error(_("cat-file reported failure"));
	திரும்பு 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
நிலையான முழு import_object(கட்டமைப்பு object_id *oid, எண்ணிக்கை object_type type,
			  முழு raw, மாறிலி எழுத்து *filename)
{
	முழு fd;

	fd = open(filename, O_RDONLY);
	எனில் (fd < 0)
		திரும்பு error_errno(_("unable to open %s for reading"), filename);

	எனில் (!raw && type == OBJ_மரம்) {
		கட்டமைப்பு child_process cmd = CHILD_PROCESS_INIT;
		கட்டமைப்பு strbuf result = STRBUF_INIT;

		strvec_தள்ளு(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		எனில் (start_command(&cmd)) {
			close(fd);
			திரும்பு error(_("unable to spawn mktree"));
		}

		எனில் (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			திரும்பு -1;
		}
		close(cmd.out);

		எனில் (finish_command(&cmd)) {
			strbuf_release(&result);
			திரும்பு error(_("mktree reported failure"));
		}
		எனில் (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			திரும்பு error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} இல்லையெனில் {
		கட்டமைப்பு stat st;
		முழு flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		எனில் (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			திரும்பு -1;
		}
		எனில் (index_fd(the_repository->index, oid, fd, &st, type, வெற்று, flags) < 0)
			திரும்பு error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	திரும்பு 0;
}

நிலையான முழு edit_and_replace(மாறிலி எழுத்து *object_ref, முழு force, முழு raw)
{
	எழுத்து *tmpfile;
	எண்ணிக்கை object_type type;
	கட்டமைப்பு object_id old_oid, new_oid, prev;
	கட்டமைப்பு strbuf ref = STRBUF_INIT;

	எனில் (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		திரும்பு error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, வெற்று);
	எனில் (type < 0)
		திரும்பு error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	எனில் (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		திரும்பு -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	எனில் (export_object(&old_oid, type, raw, tmpfile)) {
		விடுவி(tmpfile);
		திரும்பு -1;
	}
	எனில் (launch_editor(tmpfile, வெற்று, வெற்று) < 0) {
		விடுவி(tmpfile);
		திரும்பு error(_("editing object file failed"));
	}
	எனில் (import_object(&new_oid, type, raw, tmpfile)) {
		விடுவி(tmpfile);
		திரும்பு -1;
	}
	விடுவி(tmpfile);

	எனில் (oideq(&old_oid, &new_oid))
		திரும்பு error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	திரும்பு replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

நிலையான முழு replace_parents(கட்டமைப்பு strbuf *buf, முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு strbuf new_parents = STRBUF_INIT;
	மாறிலி எழுத்து *parent_start, *parent_end;
	முழு i;
	மாறிலி unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	போது (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	க்கு (i = 0; i < argc; i++) {
		கட்டமைப்பு object_id oid;
		கட்டமைப்பு commit *commit;

		எனில் (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			திரும்பு error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		எனில் (!commit) {
			strbuf_release(&new_parents);
			திரும்பு error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	திரும்பு 0;
}

கட்டமைப்பு check_mergetag_data {
	முழு argc;
	மாறிலி எழுத்து **argv;
};

நிலையான முழு check_one_mergetag(கட்டமைப்பு commit *commit UNUSED,
			       கட்டமைப்பு commit_extra_header *extra,
			       வெறுமை *data)
{
	கட்டமைப்பு check_mergetag_data *mergetag_data = (கட்டமைப்பு check_mergetag_data *)data;
	மாறிலி எழுத்து *ref = mergetag_data->argv[0];
	கட்டமைப்பு object_id tag_oid;
	கட்டமைப்பு tag *tag;
	முழு i;

	hash_object_file(the_hash_algo, extra->மதிப்பு, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	எனில் (!tag)
		திரும்பு error(_("bad mergetag in commit '%s'"), ref);
	எனில் (parse_tag_buffer(the_repository, tag, extra->மதிப்பு, extra->len))
		திரும்பு error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	க்கு (i = 1; i < mergetag_data->argc; i++) {
		கட்டமைப்பு object_id oid;
		எனில் (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			திரும்பு error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		எனில் (oideq(get_tagged_oid(tag), &oid))
			திரும்பு 0; /* found */
	}

	திரும்பு error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

நிலையான முழு check_mergetags(கட்டமைப்பு commit *commit, முழு argc, மாறிலி எழுத்து **argv)
{
	கட்டமைப்பு check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	திரும்பு for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

நிலையான முழு create_graft(முழு argc, மாறிலி எழுத்து **argv, முழு force, முழு gentle)
{
	கட்டமைப்பு object_id old_oid, new_oid;
	மாறிலி எழுத்து *old_ref = argv[0];
	கட்டமைப்பு commit *commit;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	மாறிலி எழுத்து *buffer;
	unsigned long size;

	எனில் (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		திரும்பு error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	எனில் (!commit)
		திரும்பு error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	எனில் (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		திரும்பு -1;
	}

	எனில் (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	எனில் (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		திரும்பு -1;
	}

	எனில் (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		திரும்பு error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	எனில் (oideq(&commit->object.oid, &new_oid)) {
		எனில் (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			திரும்பு 0;
		}
		திரும்பு error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	திரும்பு replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

நிலையான முழு convert_graft_file(முழு force)
{
	மாறிலி எழுத்து *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	கட்டமைப்பு strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	கட்டமைப்பு strvec args = STRVEC_INIT;

	எனில் (!fp)
		திரும்பு -1;

	no_graft_file_deprecated_advice = 1;
	போது (strbuf_getline(&buf, fp) != EOF) {
		எனில் (*buf.buf == '#')
			தொடர்;

		strvec_split(&args, buf.buf);
		எனில் (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	எனில் (!err.len)
		திரும்பு unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	திரும்பு -1;
}

முழு cmd_replace(முழு argc,
		மாறிலி எழுத்து **argv,
		மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	முழு force = 0;
	முழு raw = 0;
	மாறிலி எழுத்து *format = வெற்று;
	எண்ணிக்கை {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	கட்டமைப்பு option options[] = {
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
	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	எனில் (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	எனில் (format && cmdmode != MODE_LIST)
		usage_செய்தி_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	எனில் (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_செய்தி_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	எனில் (raw && cmdmode != MODE_EDIT)
		usage_செய்தி_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	தேர்வு (cmdmode) {
	நிலை MODE_DELETE:
		எனில் (argc < 1)
			usage_செய்தி_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		திரும்பு for_each_replace_name(argv, delete_replace_ref);

	நிலை MODE_REPLACE:
		எனில் (argc != 2)
			usage_செய்தி_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		திரும்பு replace_object(argv[0], argv[1], force);

	நிலை MODE_EDIT:
		எனில் (argc != 1)
			usage_செய்தி_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		திரும்பு edit_and_replace(argv[0], force, raw);

	நிலை MODE_GRAFT:
		எனில் (argc < 1)
			usage_செய்தி_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		திரும்பு create_graft(argc, argv, force, 0);

	நிலை MODE_CONVERT_GRAFT_FILE:
		எனில் (argc != 0)
			usage_செய்தி_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		திரும்பு !!convert_graft_file(force);

	நிலை MODE_LIST:
		எனில் (argc > 1)
			usage_செய்தி_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		திரும்பு list_replace_refs(argv[0], format);

	இயல்பு:
		BUG("invalid cmdmode %d", (முழு)cmdmode);
	}
}
