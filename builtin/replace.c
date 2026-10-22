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

static ကိန်းပြည့် show_reference(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa show_data *data = cb_data;

	အကယ်၍ (!wildmatch(data->pattern, ref->name, 0)) {
		အကယ်၍ (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		မဟုတ်ပါက အကယ်၍ (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		မဟုတ်ပါက { /* data->format == REPLACE_FORMAT_LONG */
			sa_nracanaa object_id object;
			ga_nanaa object_type obj_type, repl_type;

			အကယ်၍ (repo_get_oid(data->repo, ref->name, &object))
				ပြန်ပေး error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, NULL);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, NULL);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	ပြန်ပေး 0;
}

static ကိန်းပြည့် list_replace_refs(const var_na *pattern, const var_na *format)
{
	sa_nracanaa show_data data;

	data.repo = the_repository;
	အကယ်၍ (!pattern)
		pattern = "*";
	data.pattern = pattern;

	အကယ်၍ (format == NULL || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	မဟုတ်ပါက အကယ်၍ (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	မဟုတ်ပါက အကယ်၍ (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	မဟုတ်ပါက
		ပြန်ပေး error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (void *)&data);

	ပြန်ပေး 0;
}

typedef ကိန်းပြည့် (*each_replace_name_fn)(const var_na *name, const var_na *ref,
				    const sa_nracanaa object_id *oid);

static ကိန်းပြည့် for_each_replace_name(const var_na **argv, each_replace_name_fn fn)
{
	const var_na **p, *full_hex;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	size_t base_len;
	ကိန်းပြည့် had_error = 0;
	sa_nracanaa object_id oid;
	const var_na *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	အတွက် (p = argv; *p; p++) {
		အကယ်၍ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			continue;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		အကယ်၍ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			continue;
		}
		အကယ်၍ (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ပြန်ပေး had_error;
}

static ကိန်းပြည့် delete_replace_ref(const var_na *name, const var_na *ref,
			      const sa_nracanaa object_id *oid)
{
	အကယ်၍ (refs_delete_ref(get_main_ref_store(the_repository), NULL, ref, oid, 0))
		ပြန်ပေး 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် check_ref_valid(sa_nracanaa object_id *object,
			    sa_nracanaa object_id *prev,
			    sa_nracanaa strbuf *ref,
			    ကိန်းပြည့် force)
{
	const var_na *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	အကယ်၍ (check_refname_format(ref->buf, 0))
		ပြန်ပေး error(_("'%s' is not a valid ref name"), ref->buf);

	အကယ်၍ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	မဟုတ်ပါက အကယ်၍ (!force)
		ပြန်ပေး error(_("replace ref '%s' already exists"), ref->buf);
	ပြန်ပေး 0;
}

static ကိန်းပြည့် replace_object_oid(const var_na *object_ref,
			       sa_nracanaa object_id *object,
			       const var_na *replace_ref,
			       sa_nracanaa object_id *repl,
			       ကိန်းပြည့် force)
{
	sa_nracanaa object_id prev;
	ga_nanaa object_type obj_type, repl_type;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	sa_nracanaa ref_transaction *transaction;
	sa_nracanaa strbuf err = STRBUF_INIT;
	ကိန်းပြည့် res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, NULL);
	repl_type = odb_read_object_info(the_repository->objects, repl, NULL);
	အကယ်၍ (!force && obj_type != repl_type)
		ပြန်ပေး error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	အကယ်၍ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		ပြန်ပေး -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	အကယ်၍ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   NULL, NULL, 0, NULL, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ပြန်ပေး res;
}

static ကိန်းပြည့် replace_object(const var_na *object_ref, const var_na *replace_ref, ကိန်းပြည့် force)
{
	sa_nracanaa object_id object, repl;

	အကယ်၍ (repo_get_oid(the_repository, object_ref, &object))
		ပြန်ပေး error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	အကယ်၍ (repo_get_oid(the_repository, replace_ref, &repl))
		ပြန်ပေး error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ပြန်ပေး replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
static ကိန်းပြည့် export_object(const sa_nracanaa object_id *oid, ga_nanaa object_type type,
			  ကိန်းပြည့် raw, const var_na *filename)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	ကိန်းပြည့် fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	အကယ်၍ (fd < 0)
		ပြန်ပေး error_errno(_("unable to open %s for writing"), filename);

	strvec_push(&cmd.args, "--no-replace-objects");
	strvec_push(&cmd.args, "cat-file");
	အကယ်၍ (raw)
		strvec_push(&cmd.args, type_name(type));
	မဟုတ်ပါက
		strvec_push(&cmd.args, "-p");
	strvec_push(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	အကယ်၍ (run_command(&cmd))
		ပြန်ပေး error(_("cat-file reported failure"));
	ပြန်ပေး 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
static ကိန်းပြည့် import_object(sa_nracanaa object_id *oid, ga_nanaa object_type type,
			  ကိန်းပြည့် raw, const var_na *filename)
{
	ကိန်းပြည့် fd;

	fd = open(filename, O_RDONLY);
	အကယ်၍ (fd < 0)
		ပြန်ပေး error_errno(_("unable to open %s for reading"), filename);

	အကယ်၍ (!raw && type == OBJ_TREE) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
		sa_nracanaa strbuf result = STRBUF_INIT;

		strvec_push(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		အကယ်၍ (start_command(&cmd)) {
			close(fd);
			ပြန်ပေး error(_("unable to spawn mktree"));
		}

		အကယ်၍ (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			ပြန်ပေး -1;
		}
		close(cmd.out);

		အကယ်၍ (finish_command(&cmd)) {
			strbuf_release(&result);
			ပြန်ပေး error(_("mktree reported failure"));
		}
		အကယ်၍ (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			ပြန်ပေး error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} မဟုတ်ပါက {
		sa_nracanaa stat st;
		ကိန်းပြည့် flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		အကယ်၍ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ပြန်ပေး -1;
		}
		အကယ်၍ (index_fd(the_repository->index, oid, fd, &st, type, NULL, flags) < 0)
			ပြန်ပေး error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ပြန်ပေး 0;
}

static ကိန်းပြည့် edit_and_replace(const var_na *object_ref, ကိန်းပြည့် force, ကိန်းပြည့် raw)
{
	var_na *tmpfile;
	ga_nanaa object_type type;
	sa_nracanaa object_id old_oid, new_oid, prev;
	sa_nracanaa strbuf ref = STRBUF_INIT;

	အကယ်၍ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ပြန်ပေး error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, NULL);
	အကယ်၍ (type < 0)
		ပြန်ပေး error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	အကယ်၍ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		ပြန်ပေး -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	အကယ်၍ (export_object(&old_oid, type, raw, tmpfile)) {
		free(tmpfile);
		ပြန်ပေး -1;
	}
	အကယ်၍ (launch_editor(tmpfile, NULL, NULL) < 0) {
		free(tmpfile);
		ပြန်ပေး error(_("editing object file failed"));
	}
	အကယ်၍ (import_object(&new_oid, type, raw, tmpfile)) {
		free(tmpfile);
		ပြန်ပေး -1;
	}
	free(tmpfile);

	အကယ်၍ (oideq(&old_oid, &new_oid))
		ပြန်ပေး error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	ပြန်ပေး replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

static ကိန်းပြည့် replace_parents(sa_nracanaa strbuf *buf, ကိန်းပြည့် argc, const var_na **argv)
{
	sa_nracanaa strbuf new_parents = STRBUF_INIT;
	const var_na *parent_start, *parent_end;
	ကိန်းပြည့် i;
	const unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	နေစဉ် (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	အတွက် (i = 0; i < argc; i++) {
		sa_nracanaa object_id oid;
		sa_nracanaa commit *commit;

		အကယ်၍ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ပြန်ပေး error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		အကယ်၍ (!commit) {
			strbuf_release(&new_parents);
			ပြန်ပေး error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ပြန်ပေး 0;
}

sa_nracanaa check_mergetag_data {
	ကိန်းပြည့် argc;
	const var_na **argv;
};

static ကိန်းပြည့် check_one_mergetag(sa_nracanaa commit *commit UNUSED,
			       sa_nracanaa commit_extra_header *extra,
			       void *data)
{
	sa_nracanaa check_mergetag_data *mergetag_data = (sa_nracanaa check_mergetag_data *)data;
	const var_na *ref = mergetag_data->argv[0];
	sa_nracanaa object_id tag_oid;
	sa_nracanaa tag *tag;
	ကိန်းပြည့် i;

	hash_object_file(the_hash_algo, extra->value, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	အကယ်၍ (!tag)
		ပြန်ပေး error(_("bad mergetag in commit '%s'"), ref);
	အကယ်၍ (parse_tag_buffer(the_repository, tag, extra->value, extra->len))
		ပြန်ပေး error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	အတွက် (i = 1; i < mergetag_data->argc; i++) {
		sa_nracanaa object_id oid;
		အကယ်၍ (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			ပြန်ပေး error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		အကယ်၍ (oideq(get_tagged_oid(tag), &oid))
			ပြန်ပေး 0; /* found */
	}

	ပြန်ပေး error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

static ကိန်းပြည့် check_mergetags(sa_nracanaa commit *commit, ကိန်းပြည့် argc, const var_na **argv)
{
	sa_nracanaa check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	ပြန်ပေး for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

static ကိန်းပြည့် create_graft(ကိန်းပြည့် argc, const var_na **argv, ကိန်းပြည့် force, ကိန်းပြည့် gentle)
{
	sa_nracanaa object_id old_oid, new_oid;
	const var_na *old_ref = argv[0];
	sa_nracanaa commit *commit;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *buffer;
	unsigned long size;

	အကယ်၍ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ပြန်ပေး error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	အကယ်၍ (!commit)
		ပြန်ပေး error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	အကယ်၍ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ပြန်ပေး -1;
	}

	အကယ်၍ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	အကယ်၍ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ပြန်ပေး -1;
	}

	အကယ်၍ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ပြန်ပေး error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	အကယ်၍ (oideq(&commit->object.oid, &new_oid)) {
		အကယ်၍ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			ပြန်ပေး 0;
		}
		ပြန်ပေး error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	ပြန်ပေး replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

static ကိန်းပြည့် convert_graft_file(ကိန်းပြည့် force)
{
	const var_na *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	sa_nracanaa strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	sa_nracanaa strvec args = STRVEC_INIT;

	အကယ်၍ (!fp)
		ပြန်ပေး -1;

	no_graft_file_deprecated_advice = 1;
	နေစဉ် (strbuf_getline(&buf, fp) != EOF) {
		အကယ်၍ (*buf.buf == '#')
			continue;

		strvec_split(&args, buf.buf);
		အကယ်၍ (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	အကယ်၍ (!err.len)
		ပြန်ပေး unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ပြန်ပေး -1;
}

ကိန်းပြည့် cmd_replace(ကိန်းပြည့် argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	ကိန်းပြည့် force = 0;
	ကိန်းပြည့် raw = 0;
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

	အကယ်၍ (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	အကယ်၍ (format && cmdmode != MODE_LIST)
		usage_msg_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	အကယ်၍ (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_msg_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	အကယ်၍ (raw && cmdmode != MODE_EDIT)
		usage_msg_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	switch (cmdmode) {
	pak_sa MODE_DELETE:
		အကယ်၍ (argc < 1)
			usage_msg_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ပြန်ပေး for_each_replace_name(argv, delete_replace_ref);

	pak_sa MODE_REPLACE:
		အကယ်၍ (argc != 2)
			usage_msg_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ပြန်ပေး replace_object(argv[0], argv[1], force);

	pak_sa MODE_EDIT:
		အကယ်၍ (argc != 1)
			usage_msg_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ပြန်ပေး edit_and_replace(argv[0], force, raw);

	pak_sa MODE_GRAFT:
		အကယ်၍ (argc < 1)
			usage_msg_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ပြန်ပေး create_graft(argc, argv, force, 0);

	pak_sa MODE_CONVERT_GRAFT_FILE:
		အကယ်၍ (argc != 0)
			usage_msg_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ပြန်ပေး !!convert_graft_file(force);

	pak_sa MODE_LIST:
		အကယ်၍ (argc > 1)
			usage_msg_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ပြန်ပေး list_replace_refs(argv[0], format);

	default:
		BUG("invalid cmdmode %d", (ကိန်းပြည့်)cmdmode);
	}
}
