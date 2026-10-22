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
#include "builtin.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "refs.h"
#include "parse-options.h"
#include "path.h"
#include "run-command.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "replace-object.h"
#include "tag.h"
#include "wildmatch.h"

static const var_na * const git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	NULL
};

ga_nanaa replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

sa_nracanaa show_data {
	sa_nracanaa repository *repo;
	const var_na *pattern;
	ga_nanaa replace_format format;
};

static จำนวนเต็ม show_reference(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa show_data *data = cb_data;

	ถ้า (!wildmatch(data->pattern, ref->name, 0)) {
		ถ้า (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		มิฉะนั้น ถ้า (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		มิฉะนั้น { /* data->format == REPLACE_FORMAT_LONG */
			sa_nracanaa object_id object;
			ga_nanaa object_type obj_type, repl_type;

			ถ้า (repo_get_oid(data->repo, ref->name, &object))
				คืน error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, NULL);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, NULL);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	คืน 0;
}

static จำนวนเต็ม list_replace_refs(const var_na *pattern, const var_na *format)
{
	sa_nracanaa show_data data;

	data.repo = the_repository;
	ถ้า (!pattern)
		pattern = "*";
	data.pattern = pattern;

	ถ้า (format == NULL || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	มิฉะนั้น ถ้า (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	มิฉะนั้น ถ้า (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	มิฉะนั้น
		คืน error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (void *)&data);

	คืน 0;
}

typedef จำนวนเต็ม (*each_replace_name_fn)(const var_na *name, const var_na *ref,
				    const sa_nracanaa object_id *oid);

static จำนวนเต็ม for_each_replace_name(const var_na **argv, each_replace_name_fn fn)
{
	const var_na **p, *full_hex;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	size_t base_len;
	จำนวนเต็ม had_error = 0;
	sa_nracanaa object_id oid;
	const var_na *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	สำหรับ (p = argv; *p; p++) {
		ถ้า (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			continue;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		ถ้า (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			continue;
		}
		ถ้า (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	คืน had_error;
}

static จำนวนเต็ม delete_replace_ref(const var_na *name, const var_na *ref,
			      const sa_nracanaa object_id *oid)
{
	ถ้า (refs_delete_ref(get_main_ref_store(the_repository), NULL, ref, oid, 0))
		คืน 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	คืน 0;
}

static จำนวนเต็ม check_ref_valid(sa_nracanaa object_id *object,
			    sa_nracanaa object_id *prev,
			    sa_nracanaa strbuf *ref,
			    จำนวนเต็ม force)
{
	const var_na *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	ถ้า (check_refname_format(ref->buf, 0))
		คืน error(_("'%s' is not a valid ref name"), ref->buf);

	ถ้า (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	มิฉะนั้น ถ้า (!force)
		คืน error(_("replace ref '%s' already exists"), ref->buf);
	คืน 0;
}

static จำนวนเต็ม replace_object_oid(const var_na *object_ref,
			       sa_nracanaa object_id *object,
			       const var_na *replace_ref,
			       sa_nracanaa object_id *repl,
			       จำนวนเต็ม force)
{
	sa_nracanaa object_id prev;
	ga_nanaa object_type obj_type, repl_type;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	sa_nracanaa ref_transaction *transaction;
	sa_nracanaa strbuf err = STRBUF_INIT;
	จำนวนเต็ม res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, NULL);
	repl_type = odb_read_object_info(the_repository->objects, repl, NULL);
	ถ้า (!force && obj_type != repl_type)
		คืน error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ถ้า (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		คืน -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ถ้า (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   NULL, NULL, 0, NULL, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	คืน res;
}

static จำนวนเต็ม replace_object(const var_na *object_ref, const var_na *replace_ref, จำนวนเต็ม force)
{
	sa_nracanaa object_id object, repl;

	ถ้า (repo_get_oid(the_repository, object_ref, &object))
		คืน error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ถ้า (repo_get_oid(the_repository, replace_ref, &repl))
		คืน error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	คืน replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
static จำนวนเต็ม export_object(const sa_nracanaa object_id *oid, ga_nanaa object_type type,
			  จำนวนเต็ม raw, const var_na *filename)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	จำนวนเต็ม fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ถ้า (fd < 0)
		คืน error_errno(_("unable to open %s for writing"), filename);

	strvec_push(&cmd.args, "--no-replace-objects");
	strvec_push(&cmd.args, "cat-file");
	ถ้า (raw)
		strvec_push(&cmd.args, type_name(type));
	มิฉะนั้น
		strvec_push(&cmd.args, "-p");
	strvec_push(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	ถ้า (run_command(&cmd))
		คืน error(_("cat-file reported failure"));
	คืน 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
static จำนวนเต็ม import_object(sa_nracanaa object_id *oid, ga_nanaa object_type type,
			  จำนวนเต็ม raw, const var_na *filename)
{
	จำนวนเต็ม fd;

	fd = open(filename, O_RDONLY);
	ถ้า (fd < 0)
		คืน error_errno(_("unable to open %s for reading"), filename);

	ถ้า (!raw && type == OBJ_TREE) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
		sa_nracanaa strbuf result = STRBUF_INIT;

		strvec_push(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		ถ้า (start_command(&cmd)) {
			close(fd);
			คืน error(_("unable to spawn mktree"));
		}

		ถ้า (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			คืน -1;
		}
		close(cmd.out);

		ถ้า (finish_command(&cmd)) {
			strbuf_release(&result);
			คืน error(_("mktree reported failure"));
		}
		ถ้า (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			คืน error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} มิฉะนั้น {
		sa_nracanaa stat st;
		จำนวนเต็ม flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		ถ้า (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			คืน -1;
		}
		ถ้า (index_fd(the_repository->index, oid, fd, &st, type, NULL, flags) < 0)
			คืน error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	คืน 0;
}

static จำนวนเต็ม edit_and_replace(const var_na *object_ref, จำนวนเต็ม force, จำนวนเต็ม raw)
{
	var_na *tmpfile;
	ga_nanaa object_type type;
	sa_nracanaa object_id old_oid, new_oid, prev;
	sa_nracanaa strbuf ref = STRBUF_INIT;

	ถ้า (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		คืน error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, NULL);
	ถ้า (type < 0)
		คืน error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	ถ้า (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		คืน -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	ถ้า (export_object(&old_oid, type, raw, tmpfile)) {
		free(tmpfile);
		คืน -1;
	}
	ถ้า (launch_editor(tmpfile, NULL, NULL) < 0) {
		free(tmpfile);
		คืน error(_("editing object file failed"));
	}
	ถ้า (import_object(&new_oid, type, raw, tmpfile)) {
		free(tmpfile);
		คืน -1;
	}
	free(tmpfile);

	ถ้า (oideq(&old_oid, &new_oid))
		คืน error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	คืน replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

static จำนวนเต็ม replace_parents(sa_nracanaa strbuf *buf, จำนวนเต็ม argc, const var_na **argv)
{
	sa_nracanaa strbuf new_parents = STRBUF_INIT;
	const var_na *parent_start, *parent_end;
	จำนวนเต็ม i;
	const unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	ขณะ (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	สำหรับ (i = 0; i < argc; i++) {
		sa_nracanaa object_id oid;
		sa_nracanaa commit *commit;

		ถ้า (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			คืน error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ถ้า (!commit) {
			strbuf_release(&new_parents);
			คืน error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	คืน 0;
}

sa_nracanaa check_mergetag_data {
	จำนวนเต็ม argc;
	const var_na **argv;
};

static จำนวนเต็ม check_one_mergetag(sa_nracanaa commit *commit UNUSED,
			       sa_nracanaa commit_extra_header *extra,
			       void *data)
{
	sa_nracanaa check_mergetag_data *mergetag_data = (sa_nracanaa check_mergetag_data *)data;
	const var_na *ref = mergetag_data->argv[0];
	sa_nracanaa object_id tag_oid;
	sa_nracanaa tag *tag;
	จำนวนเต็ม i;

	hash_object_file(the_hash_algo, extra->value, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	ถ้า (!tag)
		คืน error(_("bad mergetag in commit '%s'"), ref);
	ถ้า (parse_tag_buffer(the_repository, tag, extra->value, extra->len))
		คืน error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	สำหรับ (i = 1; i < mergetag_data->argc; i++) {
		sa_nracanaa object_id oid;
		ถ้า (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			คืน error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		ถ้า (oideq(get_tagged_oid(tag), &oid))
			คืน 0; /* found */
	}

	คืน error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

static จำนวนเต็ม check_mergetags(sa_nracanaa commit *commit, จำนวนเต็ม argc, const var_na **argv)
{
	sa_nracanaa check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	คืน for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

static จำนวนเต็ม create_graft(จำนวนเต็ม argc, const var_na **argv, จำนวนเต็ม force, จำนวนเต็ม gentle)
{
	sa_nracanaa object_id old_oid, new_oid;
	const var_na *old_ref = argv[0];
	sa_nracanaa commit *commit;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *buffer;
	unsigned long size;

	ถ้า (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		คืน error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ถ้า (!commit)
		คืน error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	ถ้า (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		คืน -1;
	}

	ถ้า (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ถ้า (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		คืน -1;
	}

	ถ้า (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		คืน error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ถ้า (oideq(&commit->object.oid, &new_oid)) {
		ถ้า (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			คืน 0;
		}
		คืน error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	คืน replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

static จำนวนเต็ม convert_graft_file(จำนวนเต็ม force)
{
	const var_na *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	sa_nracanaa strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	sa_nracanaa strvec args = STRVEC_INIT;

	ถ้า (!fp)
		คืน -1;

	no_graft_file_deprecated_advice = 1;
	ขณะ (strbuf_getline(&buf, fp) != EOF) {
		ถ้า (*buf.buf == '#')
			continue;

		strvec_split(&args, buf.buf);
		ถ้า (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	ถ้า (!err.len)
		คืน unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	คืน -1;
}

จำนวนเต็ม cmd_replace(จำนวนเต็ม argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	จำนวนเต็ม force = 0;
	จำนวนเต็ม raw = 0;
	const var_na *format = NULL;
	ga_nanaa {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	sa_nracanaa option options[] = {
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
	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	ถ้า (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	ถ้า (format && cmdmode != MODE_LIST)
		usage_msg_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ถ้า (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_msg_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ถ้า (raw && cmdmode != MODE_EDIT)
		usage_msg_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	switch (cmdmode) {
	pak_sa MODE_DELETE:
		ถ้า (argc < 1)
			usage_msg_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		คืน for_each_replace_name(argv, delete_replace_ref);

	pak_sa MODE_REPLACE:
		ถ้า (argc != 2)
			usage_msg_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		คืน replace_object(argv[0], argv[1], force);

	pak_sa MODE_EDIT:
		ถ้า (argc != 1)
			usage_msg_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		คืน edit_and_replace(argv[0], force, raw);

	pak_sa MODE_GRAFT:
		ถ้า (argc < 1)
			usage_msg_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		คืน create_graft(argc, argv, force, 0);

	pak_sa MODE_CONVERT_GRAFT_FILE:
		ถ้า (argc != 0)
			usage_msg_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		คืน !!convert_graft_file(force);

	pak_sa MODE_LIST:
		ถ้า (argc > 1)
			usage_msg_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		คืน list_replace_refs(argv[0], format);

	default:
		BUG("invalid cmdmode %d", (จำนวนเต็ม)cmdmode);
	}
}
