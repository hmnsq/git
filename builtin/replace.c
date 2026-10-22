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
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "path.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "replace-object.h"
#અન્તર્ભાવ "tag.h"
#અન્તર્ભાવ "wildmatch.h"

સ્થિર અચલ અક્ષર * અચલ git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ખાલી
};

ગણના replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

સંરચના show_data {
	સંરચના repository *repo;
	અચલ અક્ષર *pattern;
	ગણના replace_format format;
};

સ્થિર પૂર્ણાંક show_reference(અચલ સંરચના reference *ref, શૂન્ય *cb_data)
{
	સંરચના show_data *data = cb_data;

	જો (!wildmatch(data->pattern, ref->name, 0)) {
		જો (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		નહીંતર જો (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		નહીંતર { /* data->format == REPLACE_FORMAT_LONG */
			સંરચના object_id object;
			ગણના object_type obj_type, repl_type;

			જો (repo_get_oid(data->repo, ref->name, &object))
				પાછા error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, ખાલી);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, ખાલી);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	પાછા 0;
}

સ્થિર પૂર્ણાંક list_replace_refs(અચલ અક્ષર *pattern, અચલ અક્ષર *format)
{
	સંરચના show_data data;

	data.repo = the_repository;
	જો (!pattern)
		pattern = "*";
	data.pattern = pattern;

	જો (format == ખાલી || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	નહીંતર જો (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	નહીંતર જો (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	નહીંતર
		પાછા error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (શૂન્ય *)&data);

	પાછા 0;
}

પ્રકારવ્યાખ્યા પૂર્ણાંક (*each_replace_name_fn)(અચલ અક્ષર *name, અચલ અક્ષર *ref,
				    અચલ સંરચના object_id *oid);

સ્થિર પૂર્ણાંક for_each_replace_name(અચલ અક્ષર **argv, each_replace_name_fn fn)
{
	અચલ અક્ષર **p, *full_hex;
	સંરચના strbuf ref = STRBUF_INIT;
	size_t base_len;
	પૂર્ણાંક had_error = 0;
	સંરચના object_id oid;
	અચલ અક્ષર *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	માટે (p = argv; *p; p++) {
		જો (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			ચાલુ;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		જો (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			ચાલુ;
		}
		જો (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	પાછા had_error;
}

સ્થિર પૂર્ણાંક delete_replace_ref(અચલ અક્ષર *name, અચલ અક્ષર *ref,
			      અચલ સંરચના object_id *oid)
{
	જો (refs_delete_ref(get_main_ref_store(the_repository), ખાલી, ref, oid, 0))
		પાછા 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	પાછા 0;
}

સ્થિર પૂર્ણાંક check_ref_valid(સંરચના object_id *object,
			    સંરચના object_id *prev,
			    સંરચના strbuf *ref,
			    પૂર્ણાંક force)
{
	અચલ અક્ષર *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	જો (check_refname_format(ref->buf, 0))
		પાછા error(_("'%s' is not a valid ref name"), ref->buf);

	જો (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	નહીંતર જો (!force)
		પાછા error(_("replace ref '%s' already exists"), ref->buf);
	પાછા 0;
}

સ્થિર પૂર્ણાંક replace_object_oid(અચલ અક્ષર *object_ref,
			       સંરચના object_id *object,
			       અચલ અક્ષર *replace_ref,
			       સંરચના object_id *repl,
			       પૂર્ણાંક force)
{
	સંરચના object_id prev;
	ગણના object_type obj_type, repl_type;
	સંરચના strbuf ref = STRBUF_INIT;
	સંરચના ref_transaction *transaction;
	સંરચના strbuf err = STRBUF_INIT;
	પૂર્ણાંક res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ખાલી);
	repl_type = odb_read_object_info(the_repository->objects, repl, ખાલી);
	જો (!force && obj_type != repl_type)
		પાછા error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	જો (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		પાછા -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	જો (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ખાલી, ખાલી, 0, ખાલી, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	પાછા res;
}

સ્થિર પૂર્ણાંક replace_object(અચલ અક્ષર *object_ref, અચલ અક્ષર *replace_ref, પૂર્ણાંક force)
{
	સંરચના object_id object, repl;

	જો (repo_get_oid(the_repository, object_ref, &object))
		પાછા error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	જો (repo_get_oid(the_repository, replace_ref, &repl))
		પાછા error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	પાછા replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
સ્થિર પૂર્ણાંક export_object(અચલ સંરચના object_id *oid, ગણના object_type type,
			  પૂર્ણાંક raw, અચલ અક્ષર *filename)
{
	સંરચના child_process cmd = CHILD_PROCESS_INIT;
	પૂર્ણાંક fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	જો (fd < 0)
		પાછા error_errno(_("unable to open %s for writing"), filename);

	strvec_ધકેલો(&cmd.args, "--no-replace-objects");
	strvec_ધકેલો(&cmd.args, "cat-file");
	જો (raw)
		strvec_ધકેલો(&cmd.args, type_name(type));
	નહીંતર
		strvec_ધકેલો(&cmd.args, "-p");
	strvec_ધકેલો(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	જો (run_command(&cmd))
		પાછા error(_("cat-file reported failure"));
	પાછા 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
સ્થિર પૂર્ણાંક import_object(સંરચના object_id *oid, ગણના object_type type,
			  પૂર્ણાંક raw, અચલ અક્ષર *filename)
{
	પૂર્ણાંક fd;

	fd = open(filename, O_RDONLY);
	જો (fd < 0)
		પાછા error_errno(_("unable to open %s for reading"), filename);

	જો (!raw && type == OBJ_વૃક્ષ) {
		સંરચના child_process cmd = CHILD_PROCESS_INIT;
		સંરચના strbuf result = STRBUF_INIT;

		strvec_ધકેલો(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		જો (start_command(&cmd)) {
			close(fd);
			પાછા error(_("unable to spawn mktree"));
		}

		જો (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			પાછા -1;
		}
		close(cmd.out);

		જો (finish_command(&cmd)) {
			strbuf_release(&result);
			પાછા error(_("mktree reported failure"));
		}
		જો (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			પાછા error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} નહીંતર {
		સંરચના stat st;
		પૂર્ણાંક flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		જો (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			પાછા -1;
		}
		જો (index_fd(the_repository->index, oid, fd, &st, type, ખાલી, flags) < 0)
			પાછા error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	પાછા 0;
}

સ્થિર પૂર્ણાંક edit_and_replace(અચલ અક્ષર *object_ref, પૂર્ણાંક force, પૂર્ણાંક raw)
{
	અક્ષર *tmpfile;
	ગણના object_type type;
	સંરચના object_id old_oid, new_oid, prev;
	સંરચના strbuf ref = STRBUF_INIT;

	જો (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		પાછા error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ખાલી);
	જો (type < 0)
		પાછા error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	જો (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		પાછા -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	જો (export_object(&old_oid, type, raw, tmpfile)) {
		મુક્ત(tmpfile);
		પાછા -1;
	}
	જો (launch_editor(tmpfile, ખાલી, ખાલી) < 0) {
		મુક્ત(tmpfile);
		પાછા error(_("editing object file failed"));
	}
	જો (import_object(&new_oid, type, raw, tmpfile)) {
		મુક્ત(tmpfile);
		પાછા -1;
	}
	મુક્ત(tmpfile);

	જો (oideq(&old_oid, &new_oid))
		પાછા error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	પાછા replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

સ્થિર પૂર્ણાંક replace_parents(સંરચના strbuf *buf, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના strbuf new_parents = STRBUF_INIT;
	અચલ અક્ષર *parent_start, *parent_end;
	પૂર્ણાંક i;
	અચલ unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	જ્યારે (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	માટે (i = 0; i < argc; i++) {
		સંરચના object_id oid;
		સંરચના commit *commit;

		જો (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			પાછા error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		જો (!commit) {
			strbuf_release(&new_parents);
			પાછા error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	પાછા 0;
}

સંરચના check_mergetag_data {
	પૂર્ણાંક argc;
	અચલ અક્ષર **argv;
};

સ્થિર પૂર્ણાંક check_one_mergetag(સંરચના commit *commit UNUSED,
			       સંરચના commit_extra_header *extra,
			       શૂન્ય *data)
{
	સંરચના check_mergetag_data *mergetag_data = (સંરચના check_mergetag_data *)data;
	અચલ અક્ષર *ref = mergetag_data->argv[0];
	સંરચના object_id tag_oid;
	સંરચના tag *tag;
	પૂર્ણાંક i;

	hash_object_file(the_hash_algo, extra->કિંમત, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	જો (!tag)
		પાછા error(_("bad mergetag in commit '%s'"), ref);
	જો (parse_tag_buffer(the_repository, tag, extra->કિંમત, extra->len))
		પાછા error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	માટે (i = 1; i < mergetag_data->argc; i++) {
		સંરચના object_id oid;
		જો (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			પાછા error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		જો (oideq(get_tagged_oid(tag), &oid))
			પાછા 0; /* found */
	}

	પાછા error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

સ્થિર પૂર્ણાંક check_mergetags(સંરચના commit *commit, પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
	સંરચના check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	પાછા for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

સ્થિર પૂર્ણાંક create_graft(પૂર્ણાંક argc, અચલ અક્ષર **argv, પૂર્ણાંક force, પૂર્ણાંક gentle)
{
	સંરચના object_id old_oid, new_oid;
	અચલ અક્ષર *old_ref = argv[0];
	સંરચના commit *commit;
	સંરચના strbuf buf = STRBUF_INIT;
	અચલ અક્ષર *buffer;
	unsigned long size;

	જો (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		પાછા error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	જો (!commit)
		પાછા error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	જો (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		પાછા -1;
	}

	જો (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	જો (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		પાછા -1;
	}

	જો (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		પાછા error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	જો (oideq(&commit->object.oid, &new_oid)) {
		જો (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			પાછા 0;
		}
		પાછા error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	પાછા replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

સ્થિર પૂર્ણાંક convert_graft_file(પૂર્ણાંક force)
{
	અચલ અક્ષર *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	સંરચના strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	સંરચના strvec args = STRVEC_INIT;

	જો (!fp)
		પાછા -1;

	no_graft_file_deprecated_advice = 1;
	જ્યારે (strbuf_getline(&buf, fp) != EOF) {
		જો (*buf.buf == '#')
			ચાલુ;

		strvec_split(&args, buf.buf);
		જો (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	જો (!err.len)
		પાછા unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	પાછા -1;
}

પૂર્ણાંક cmd_replace(પૂર્ણાંક argc,
		અચલ અક્ષર **argv,
		અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક force = 0;
	પૂર્ણાંક raw = 0;
	અચલ અક્ષર *format = ખાલી;
	ગણના {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	સંરચના option options[] = {
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
	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	જો (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	જો (format && cmdmode != MODE_LIST)
		usage_સંદેશ_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	જો (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_સંદેશ_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	જો (raw && cmdmode != MODE_EDIT)
		usage_સંદેશ_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	પસંદગી (cmdmode) {
	સ્થિતિ MODE_DELETE:
		જો (argc < 1)
			usage_સંદેશ_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		પાછા for_each_replace_name(argv, delete_replace_ref);

	સ્થિતિ MODE_REPLACE:
		જો (argc != 2)
			usage_સંદેશ_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		પાછા replace_object(argv[0], argv[1], force);

	સ્થિતિ MODE_EDIT:
		જો (argc != 1)
			usage_સંદેશ_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		પાછા edit_and_replace(argv[0], force, raw);

	સ્થિતિ MODE_GRAFT:
		જો (argc < 1)
			usage_સંદેશ_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		પાછા create_graft(argc, argv, force, 0);

	સ્થિતિ MODE_CONVERT_GRAFT_FILE:
		જો (argc != 0)
			usage_સંદેશ_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		પાછા !!convert_graft_file(force);

	સ્થિતિ MODE_LIST:
		જો (argc > 1)
			usage_સંદેશ_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		પાછા list_replace_refs(argv[0], format);

	મૂળભૂત:
		BUG("invalid cmdmode %d", (પૂર્ણાંક)cmdmode);
	}
}
