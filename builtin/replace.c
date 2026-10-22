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

static цяло show_reference(const struct reference *ref, void *cb_data)
{
	struct show_data *data = cb_data;

	ако (!wildmatch(data->pattern, ref->name, 0)) {
		ако (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		иначе ако (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		иначе { /* data->format == REPLACE_FORMAT_LONG */
			struct object_id object;
			enum object_type obj_type, repl_type;

			ако (repo_get_oid(data->repo, ref->name, &object))
				върни error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, NULL);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, NULL);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	върни 0;
}

static цяло list_replace_refs(const char *pattern, const char *format)
{
	struct show_data data;

	data.repo = the_repository;
	ако (!pattern)
		pattern = "*";
	data.pattern = pattern;

	ако (format == NULL || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	иначе ако (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	иначе ако (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	иначе
		върни error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (void *)&data);

	върни 0;
}

typedef цяло (*each_replace_name_fn)(const char *name, const char *ref,
				    const struct object_id *oid);

static цяло for_each_replace_name(const char **argv, each_replace_name_fn fn)
{
	const char **p, *full_hex;
	struct strbuf ref = STRBUF_INIT;
	size_t base_len;
	цяло had_error = 0;
	struct object_id oid;
	const char *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	за (p = argv; *p; p++) {
		ако (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			continue;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		ако (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			continue;
		}
		ако (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	върни had_error;
}

static цяло delete_replace_ref(const char *name, const char *ref,
			      const struct object_id *oid)
{
	ако (refs_delete_ref(get_main_ref_store(the_repository), NULL, ref, oid, 0))
		върни 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	върни 0;
}

static цяло check_ref_valid(struct object_id *object,
			    struct object_id *prev,
			    struct strbuf *ref,
			    цяло force)
{
	const char *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	ако (check_refname_format(ref->buf, 0))
		върни error(_("'%s' is not a valid ref name"), ref->buf);

	ако (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	иначе ако (!force)
		върни error(_("replace ref '%s' already exists"), ref->buf);
	върни 0;
}

static цяло replace_object_oid(const char *object_ref,
			       struct object_id *object,
			       const char *replace_ref,
			       struct object_id *repl,
			       цяло force)
{
	struct object_id prev;
	enum object_type obj_type, repl_type;
	struct strbuf ref = STRBUF_INIT;
	struct ref_transaction *transaction;
	struct strbuf err = STRBUF_INIT;
	цяло res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, NULL);
	repl_type = odb_read_object_info(the_repository->objects, repl, NULL);
	ако (!force && obj_type != repl_type)
		върни error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ако (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		върни -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ако (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   NULL, NULL, 0, NULL, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	върни res;
}

static цяло replace_object(const char *object_ref, const char *replace_ref, цяло force)
{
	struct object_id object, repl;

	ако (repo_get_oid(the_repository, object_ref, &object))
		върни error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ако (repo_get_oid(the_repository, replace_ref, &repl))
		върни error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	върни replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
static цяло export_object(const struct object_id *oid, enum object_type type,
			  цяло raw, const char *filename)
{
	struct child_process cmd = CHILD_PROCESS_INIT;
	цяло fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ако (fd < 0)
		върни error_errno(_("unable to open %s for writing"), filename);

	strvec_push(&cmd.args, "--no-replace-objects");
	strvec_push(&cmd.args, "cat-file");
	ако (raw)
		strvec_push(&cmd.args, type_name(type));
	иначе
		strvec_push(&cmd.args, "-p");
	strvec_push(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	ако (run_command(&cmd))
		върни error(_("cat-file reported failure"));
	върни 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
static цяло import_object(struct object_id *oid, enum object_type type,
			  цяло raw, const char *filename)
{
	цяло fd;

	fd = open(filename, O_RDONLY);
	ако (fd < 0)
		върни error_errno(_("unable to open %s for reading"), filename);

	ако (!raw && type == OBJ_TREE) {
		struct child_process cmd = CHILD_PROCESS_INIT;
		struct strbuf result = STRBUF_INIT;

		strvec_push(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		ако (start_command(&cmd)) {
			close(fd);
			върни error(_("unable to spawn mktree"));
		}

		ако (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			върни -1;
		}
		close(cmd.out);

		ако (finish_command(&cmd)) {
			strbuf_release(&result);
			върни error(_("mktree reported failure"));
		}
		ако (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			върни error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} иначе {
		struct stat st;
		цяло flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		ако (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			върни -1;
		}
		ако (index_fd(the_repository->index, oid, fd, &st, type, NULL, flags) < 0)
			върни error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	върни 0;
}

static цяло edit_and_replace(const char *object_ref, цяло force, цяло raw)
{
	char *tmpfile;
	enum object_type type;
	struct object_id old_oid, new_oid, prev;
	struct strbuf ref = STRBUF_INIT;

	ако (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		върни error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, NULL);
	ако (type < 0)
		върни error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	ако (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		върни -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	ако (export_object(&old_oid, type, raw, tmpfile)) {
		free(tmpfile);
		върни -1;
	}
	ако (launch_editor(tmpfile, NULL, NULL) < 0) {
		free(tmpfile);
		върни error(_("editing object file failed"));
	}
	ако (import_object(&new_oid, type, raw, tmpfile)) {
		free(tmpfile);
		върни -1;
	}
	free(tmpfile);

	ако (oideq(&old_oid, &new_oid))
		върни error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	върни replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

static цяло replace_parents(struct strbuf *buf, цяло argc, const char **argv)
{
	struct strbuf new_parents = STRBUF_INIT;
	const char *parent_start, *parent_end;
	цяло i;
	const unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	докато (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	за (i = 0; i < argc; i++) {
		struct object_id oid;
		struct commit *commit;

		ако (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			върни error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ако (!commit) {
			strbuf_release(&new_parents);
			върни error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	върни 0;
}

struct check_mergetag_data {
	цяло argc;
	const char **argv;
};

static цяло check_one_mergetag(struct commit *commit UNUSED,
			       struct commit_extra_header *extra,
			       void *data)
{
	struct check_mergetag_data *mergetag_data = (struct check_mergetag_data *)data;
	const char *ref = mergetag_data->argv[0];
	struct object_id tag_oid;
	struct tag *tag;
	цяло i;

	hash_object_file(the_hash_algo, extra->value, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	ако (!tag)
		върни error(_("bad mergetag in commit '%s'"), ref);
	ако (parse_tag_buffer(the_repository, tag, extra->value, extra->len))
		върни error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	за (i = 1; i < mergetag_data->argc; i++) {
		struct object_id oid;
		ако (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			върни error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		ако (oideq(get_tagged_oid(tag), &oid))
			върни 0; /* found */
	}

	върни error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

static цяло check_mergetags(struct commit *commit, цяло argc, const char **argv)
{
	struct check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	върни for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

static цяло create_graft(цяло argc, const char **argv, цяло force, цяло gentle)
{
	struct object_id old_oid, new_oid;
	const char *old_ref = argv[0];
	struct commit *commit;
	struct strbuf buf = STRBUF_INIT;
	const char *buffer;
	unsigned long size;

	ако (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		върни error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ако (!commit)
		върни error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	ако (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		върни -1;
	}

	ако (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ако (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		върни -1;
	}

	ако (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		върни error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ако (oideq(&commit->object.oid, &new_oid)) {
		ако (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			върни 0;
		}
		върни error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	върни replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

static цяло convert_graft_file(цяло force)
{
	const char *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	struct strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	struct strvec args = STRVEC_INIT;

	ако (!fp)
		върни -1;

	no_graft_file_deprecated_advice = 1;
	докато (strbuf_getline(&buf, fp) != EOF) {
		ако (*buf.buf == '#')
			continue;

		strvec_split(&args, buf.buf);
		ако (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	ако (!err.len)
		върни unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	върни -1;
}

цяло cmd_replace(цяло argc,
		const char **argv,
		const char *prefix,
		struct repository *repo UNUSED)
{
	цяло force = 0;
	цяло raw = 0;
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

	ако (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	ако (format && cmdmode != MODE_LIST)
		usage_msg_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ако (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_msg_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ако (raw && cmdmode != MODE_EDIT)
		usage_msg_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	switch (cmdmode) {
	case MODE_DELETE:
		ако (argc < 1)
			usage_msg_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		върни for_each_replace_name(argv, delete_replace_ref);

	case MODE_REPLACE:
		ако (argc != 2)
			usage_msg_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		върни replace_object(argv[0], argv[1], force);

	case MODE_EDIT:
		ако (argc != 1)
			usage_msg_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		върни edit_and_replace(argv[0], force, raw);

	case MODE_GRAFT:
		ако (argc < 1)
			usage_msg_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		върни create_graft(argc, argv, force, 0);

	case MODE_CONVERT_GRAFT_FILE:
		ако (argc != 0)
			usage_msg_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		върни !!convert_graft_file(force);

	case MODE_LIST:
		ако (argc > 1)
			usage_msg_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		върни list_replace_refs(argv[0], format);

	default:
		BUG("invalid cmdmode %d", (цяло)cmdmode);
	}
}
