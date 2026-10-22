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

static const char * const git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	NULL
};

enum replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

struct show_data {
	struct repository *repo;
	const char *pattern;
	enum replace_format format;
};

static 정수 show_reference(const struct reference *ref, void *cb_data)
{
	struct show_data *data = cb_data;

	만약 (!wildmatch(data->pattern, ref->name, 0)) {
		만약 (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		아니면 만약 (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		아니면 { /* data->format == REPLACE_FORMAT_LONG */
			struct object_id object;
			enum object_type obj_type, repl_type;

			만약 (repo_get_oid(data->repo, ref->name, &object))
				반환 error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, NULL);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, NULL);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	반환 0;
}

static 정수 list_replace_refs(const char *pattern, const char *format)
{
	struct show_data data;

	data.repo = the_repository;
	만약 (!pattern)
		pattern = "*";
	data.pattern = pattern;

	만약 (format == NULL || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	아니면 만약 (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	아니면 만약 (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	아니면
		반환 error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (void *)&data);

	반환 0;
}

typedef 정수 (*each_replace_name_fn)(const char *name, const char *ref,
				    const struct object_id *oid);

static 정수 for_each_replace_name(const char **argv, each_replace_name_fn fn)
{
	const char **p, *full_hex;
	struct strbuf ref = STRBUF_INIT;
	size_t base_len;
	정수 had_error = 0;
	struct object_id oid;
	const char *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	위해 (p = argv; *p; p++) {
		만약 (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			continue;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		만약 (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			continue;
		}
		만약 (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	반환 had_error;
}

static 정수 delete_replace_ref(const char *name, const char *ref,
			      const struct object_id *oid)
{
	만약 (refs_delete_ref(get_main_ref_store(the_repository), NULL, ref, oid, 0))
		반환 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	반환 0;
}

static 정수 check_ref_valid(struct object_id *object,
			    struct object_id *prev,
			    struct strbuf *ref,
			    정수 force)
{
	const char *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	만약 (check_refname_format(ref->buf, 0))
		반환 error(_("'%s' is not a valid ref name"), ref->buf);

	만약 (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	아니면 만약 (!force)
		반환 error(_("replace ref '%s' already exists"), ref->buf);
	반환 0;
}

static 정수 replace_object_oid(const char *object_ref,
			       struct object_id *object,
			       const char *replace_ref,
			       struct object_id *repl,
			       정수 force)
{
	struct object_id prev;
	enum object_type obj_type, repl_type;
	struct strbuf ref = STRBUF_INIT;
	struct ref_transaction *transaction;
	struct strbuf err = STRBUF_INIT;
	정수 res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, NULL);
	repl_type = odb_read_object_info(the_repository->objects, repl, NULL);
	만약 (!force && obj_type != repl_type)
		반환 error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	만약 (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		반환 -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	만약 (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   NULL, NULL, 0, NULL, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	반환 res;
}

static 정수 replace_object(const char *object_ref, const char *replace_ref, 정수 force)
{
	struct object_id object, repl;

	만약 (repo_get_oid(the_repository, object_ref, &object))
		반환 error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	만약 (repo_get_oid(the_repository, replace_ref, &repl))
		반환 error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	반환 replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
static 정수 export_object(const struct object_id *oid, enum object_type type,
			  정수 raw, const char *filename)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	정수 fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	만약 (fd < 0)
		반환 error_errno(_("unable to open %s for writing"), filename);

	strvec_push(&cmd.args, "--no-replace-objects");
	strvec_push(&cmd.args, "cat-file");
	만약 (raw)
		strvec_push(&cmd.args, type_name(type));
	아니면
		strvec_push(&cmd.args, "-p");
	strvec_push(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	만약 (run_command(&cmd))
		반환 error(_("cat-file reported failure"));
	반환 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
static 정수 import_object(struct object_id *oid, enum object_type type,
			  정수 raw, const char *filename)
{
	정수 fd;

	fd = open(filename, O_RDONLY);
	만약 (fd < 0)
		반환 error_errno(_("unable to open %s for reading"), filename);

	만약 (!raw && type == OBJ_TREE) {
		struct child_process cmd = CHILD_PROCESS_INIT;
		struct strbuf result = STRBUF_INIT;

		strvec_push(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		만약 (start_command(&cmd)) {
			close(fd);
			반환 error(_("unable to spawn mktree"));
		}

		만약 (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			반환 -1;
		}
		close(cmd.out);

		만약 (finish_command(&cmd)) {
			strbuf_release(&result);
			반환 error(_("mktree reported failure"));
		}
		만약 (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			반환 error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} 아니면 {
		struct stat st;
		정수 flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		만약 (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			반환 -1;
		}
		만약 (index_fd(the_repository->index, oid, fd, &st, type, NULL, flags) < 0)
			반환 error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	반환 0;
}

static 정수 edit_and_replace(const char *object_ref, 정수 force, 정수 raw)
{
	char *tmpfile;
	enum object_type type;
	struct object_id old_oid, new_oid, prev;
	struct strbuf ref = STRBUF_INIT;

	만약 (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		반환 error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, NULL);
	만약 (type < 0)
		반환 error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	만약 (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		반환 -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	만약 (export_object(&old_oid, type, raw, tmpfile)) {
		free(tmpfile);
		반환 -1;
	}
	만약 (launch_editor(tmpfile, NULL, NULL) < 0) {
		free(tmpfile);
		반환 error(_("editing object file failed"));
	}
	만약 (import_object(&new_oid, type, raw, tmpfile)) {
		free(tmpfile);
		반환 -1;
	}
	free(tmpfile);

	만약 (oideq(&old_oid, &new_oid))
		반환 error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	반환 replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

static 정수 replace_parents(struct strbuf *buf, 정수 argc, const char **argv)
{
	struct strbuf new_parents = STRBUF_INIT;
	const char *parent_start, *parent_end;
	정수 i;
	const unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	동안 (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	위해 (i = 0; i < argc; i++) {
		struct object_id oid;
		struct commit *commit;

		만약 (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			반환 error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		만약 (!commit) {
			strbuf_release(&new_parents);
			반환 error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	반환 0;
}

struct check_mergetag_data {
	정수 argc;
	const char **argv;
};

static 정수 check_one_mergetag(struct commit *commit UNUSED,
			       struct commit_extra_header *extra,
			       void *data)
{
	struct check_mergetag_data *mergetag_data = (struct check_mergetag_data *)data;
	const char *ref = mergetag_data->argv[0];
	struct object_id tag_oid;
	struct tag *tag;
	정수 i;

	hash_object_file(the_hash_algo, extra->value, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	만약 (!tag)
		반환 error(_("bad mergetag in commit '%s'"), ref);
	만약 (parse_tag_buffer(the_repository, tag, extra->value, extra->len))
		반환 error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	위해 (i = 1; i < mergetag_data->argc; i++) {
		struct object_id oid;
		만약 (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			반환 error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		만약 (oideq(get_tagged_oid(tag), &oid))
			반환 0; /* found */
	}

	반환 error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

static 정수 check_mergetags(struct commit *commit, 정수 argc, const char **argv)
{
	struct check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	반환 for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

static 정수 create_graft(정수 argc, const char **argv, 정수 force, 정수 gentle)
{
	struct object_id old_oid, new_oid;
	const char *old_ref = argv[0];
	struct commit *commit;
	struct strbuf buf = STRBUF_INIT;
	const char *buffer;
	unsigned long size;

	만약 (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		반환 error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	만약 (!commit)
		반환 error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	만약 (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		반환 -1;
	}

	만약 (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	만약 (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		반환 -1;
	}

	만약 (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		반환 error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	만약 (oideq(&commit->object.oid, &new_oid)) {
		만약 (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			반환 0;
		}
		반환 error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	반환 replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

static 정수 convert_graft_file(정수 force)
{
	const char *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	struct strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	struct strvec args = STRVEC_INIT;

	만약 (!fp)
		반환 -1;

	no_graft_file_deprecated_advice = 1;
	동안 (strbuf_getline(&buf, fp) != EOF) {
		만약 (*buf.buf == '#')
			continue;

		strvec_split(&args, buf.buf);
		만약 (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	만약 (!err.len)
		반환 unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	반환 -1;
}

정수 cmd_replace(정수 argc,
		const char **argv,
		const char *prefix,
		struct repository *repo UNUSED)
{
	정수 force = 0;
	정수 raw = 0;
	const char *format = NULL;
	enum {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	struct option options[] = {
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

	만약 (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	만약 (format && cmdmode != MODE_LIST)
		usage_msg_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	만약 (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_msg_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	만약 (raw && cmdmode != MODE_EDIT)
		usage_msg_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	switch (cmdmode) {
	case MODE_DELETE:
		만약 (argc < 1)
			usage_msg_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		반환 for_each_replace_name(argv, delete_replace_ref);

	case MODE_REPLACE:
		만약 (argc != 2)
			usage_msg_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		반환 replace_object(argv[0], argv[1], force);

	case MODE_EDIT:
		만약 (argc != 1)
			usage_msg_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		반환 edit_and_replace(argv[0], force, raw);

	case MODE_GRAFT:
		만약 (argc < 1)
			usage_msg_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		반환 create_graft(argc, argv, force, 0);

	case MODE_CONVERT_GRAFT_FILE:
		만약 (argc != 0)
			usage_msg_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		반환 !!convert_graft_file(force);

	case MODE_LIST:
		만약 (argc > 1)
			usage_msg_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		반환 list_replace_refs(argv[0], format);

	default:
		BUG("invalid cmdmode %d", (정수)cmdmode);
	}
}
