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

static puur_naa_nka show_reference(const sa_nracanaa reference *ref, void *cb_data)
{
	sa_nracanaa show_data *data = cb_data;

	ᤀᤡ᤺ (!wildmatch(data->pattern, ref->name, 0)) {
		ᤀᤡ᤺ (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		ᤀᤡ᤺ᤆᤡ { /* data->format == REPLACE_FORMAT_LONG */
			sa_nracanaa object_id object;
			ga_nanaa object_type obj_type, repl_type;

			ᤀᤡ᤺ (repo_get_oid(data->repo, ref->name, &object))
				ᤀᤡᤴ error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, NULL);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, NULL);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	ᤀᤡᤴ 0;
}

static puur_naa_nka list_replace_refs(const var_na *pattern, const var_na *format)
{
	sa_nracanaa show_data data;

	data.repo = the_repository;
	ᤀᤡ᤺ (!pattern)
		pattern = "*";
	data.pattern = pattern;

	ᤀᤡ᤺ (format == NULL || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	ᤀᤡ᤺ᤆᤡ
		ᤀᤡᤴ error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (void *)&data);

	ᤀᤡᤴ 0;
}

typedef puur_naa_nka (*each_replace_name_fn)(const var_na *name, const var_na *ref,
				    const sa_nracanaa object_id *oid);

static puur_naa_nka for_each_replace_name(const var_na **argv, each_replace_name_fn fn)
{
	const var_na **p, *full_hex;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	size_t base_len;
	puur_naa_nka had_error = 0;
	sa_nracanaa object_id oid;
	const var_na *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	for (p = argv; *p; p++) {
		ᤀᤡ᤺ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			continue;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		ᤀᤡ᤺ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			continue;
		}
		ᤀᤡ᤺ (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ᤀᤡᤴ had_error;
}

static puur_naa_nka delete_replace_ref(const var_na *name, const var_na *ref,
			      const sa_nracanaa object_id *oid)
{
	ᤀᤡ᤺ (refs_delete_ref(get_main_ref_store(the_repository), NULL, ref, oid, 0))
		ᤀᤡᤴ 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ᤀᤡᤴ 0;
}

static puur_naa_nka check_ref_valid(sa_nracanaa object_id *object,
			    sa_nracanaa object_id *prev,
			    sa_nracanaa strbuf *ref,
			    puur_naa_nka force)
{
	const var_na *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	ᤀᤡ᤺ (check_refname_format(ref->buf, 0))
		ᤀᤡᤴ error(_("'%s' is not a valid ref name"), ref->buf);

	ᤀᤡ᤺ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!force)
		ᤀᤡᤴ error(_("replace ref '%s' already exists"), ref->buf);
	ᤀᤡᤴ 0;
}

static puur_naa_nka replace_object_oid(const var_na *object_ref,
			       sa_nracanaa object_id *object,
			       const var_na *replace_ref,
			       sa_nracanaa object_id *repl,
			       puur_naa_nka force)
{
	sa_nracanaa object_id prev;
	ga_nanaa object_type obj_type, repl_type;
	sa_nracanaa strbuf ref = STRBUF_INIT;
	sa_nracanaa ref_transaction *transaction;
	sa_nracanaa strbuf err = STRBUF_INIT;
	puur_naa_nka res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, NULL);
	repl_type = odb_read_object_info(the_repository->objects, repl, NULL);
	ᤀᤡ᤺ (!force && obj_type != repl_type)
		ᤀᤡᤴ error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ᤀᤡ᤺ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		ᤀᤡᤴ -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ᤀᤡ᤺ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   NULL, NULL, 0, NULL, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ᤀᤡᤴ res;
}

static puur_naa_nka replace_object(const var_na *object_ref, const var_na *replace_ref, puur_naa_nka force)
{
	sa_nracanaa object_id object, repl;

	ᤀᤡ᤺ (repo_get_oid(the_repository, object_ref, &object))
		ᤀᤡᤴ error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ᤀᤡ᤺ (repo_get_oid(the_repository, replace_ref, &repl))
		ᤀᤡᤴ error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ᤀᤡᤴ replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
static puur_naa_nka export_object(const sa_nracanaa object_id *oid, ga_nanaa object_type type,
			  puur_naa_nka raw, const var_na *filename)
{
	sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
	puur_naa_nka fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ᤀᤡ᤺ (fd < 0)
		ᤀᤡᤴ error_errno(_("unable to open %s for writing"), filename);

	strvec_push(&cmd.args, "--no-replace-objects");
	strvec_push(&cmd.args, "cat-file");
	ᤀᤡ᤺ (raw)
		strvec_push(&cmd.args, type_name(type));
	ᤀᤡ᤺ᤆᤡ
		strvec_push(&cmd.args, "-p");
	strvec_push(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	ᤀᤡ᤺ (run_command(&cmd))
		ᤀᤡᤴ error(_("cat-file reported failure"));
	ᤀᤡᤴ 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
static puur_naa_nka import_object(sa_nracanaa object_id *oid, ga_nanaa object_type type,
			  puur_naa_nka raw, const var_na *filename)
{
	puur_naa_nka fd;

	fd = open(filename, O_RDONLY);
	ᤀᤡ᤺ (fd < 0)
		ᤀᤡᤴ error_errno(_("unable to open %s for reading"), filename);

	ᤀᤡ᤺ (!raw && type == OBJ_TREE) {
		sa_nracanaa child_process cmd = CHILD_PROCESS_INIT;
		sa_nracanaa strbuf result = STRBUF_INIT;

		strvec_push(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		ᤀᤡ᤺ (start_command(&cmd)) {
			close(fd);
			ᤀᤡᤴ error(_("unable to spawn mktree"));
		}

		ᤀᤡ᤺ (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			ᤀᤡᤴ -1;
		}
		close(cmd.out);

		ᤀᤡ᤺ (finish_command(&cmd)) {
			strbuf_release(&result);
			ᤀᤡᤴ error(_("mktree reported failure"));
		}
		ᤀᤡ᤺ (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			ᤀᤡᤴ error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} ᤀᤡ᤺ᤆᤡ {
		sa_nracanaa stat st;
		puur_naa_nka flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		ᤀᤡ᤺ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ᤀᤡᤴ -1;
		}
		ᤀᤡ᤺ (index_fd(the_repository->index, oid, fd, &st, type, NULL, flags) < 0)
			ᤀᤡᤴ error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ᤀᤡᤴ 0;
}

static puur_naa_nka edit_and_replace(const var_na *object_ref, puur_naa_nka force, puur_naa_nka raw)
{
	var_na *tmpfile;
	ga_nanaa object_type type;
	sa_nracanaa object_id old_oid, new_oid, prev;
	sa_nracanaa strbuf ref = STRBUF_INIT;

	ᤀᤡ᤺ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ᤀᤡᤴ error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, NULL);
	ᤀᤡ᤺ (type < 0)
		ᤀᤡᤴ error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	ᤀᤡ᤺ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		ᤀᤡᤴ -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	ᤀᤡ᤺ (export_object(&old_oid, type, raw, tmpfile)) {
		free(tmpfile);
		ᤀᤡᤴ -1;
	}
	ᤀᤡ᤺ (launch_editor(tmpfile, NULL, NULL) < 0) {
		free(tmpfile);
		ᤀᤡᤴ error(_("editing object file failed"));
	}
	ᤀᤡ᤺ (import_object(&new_oid, type, raw, tmpfile)) {
		free(tmpfile);
		ᤀᤡᤴ -1;
	}
	free(tmpfile);

	ᤀᤡ᤺ (oideq(&old_oid, &new_oid))
		ᤀᤡᤴ error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	ᤀᤡᤴ replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

static puur_naa_nka replace_parents(sa_nracanaa strbuf *buf, puur_naa_nka argc, const var_na **argv)
{
	sa_nracanaa strbuf new_parents = STRBUF_INIT;
	const var_na *parent_start, *parent_end;
	puur_naa_nka i;
	const unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	ᤀᤠ᤺ (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	for (i = 0; i < argc; i++) {
		sa_nracanaa object_id oid;
		sa_nracanaa commit *commit;

		ᤀᤡ᤺ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ᤀᤡᤴ error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ᤀᤡ᤺ (!commit) {
			strbuf_release(&new_parents);
			ᤀᤡᤴ error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ᤀᤡᤴ 0;
}

sa_nracanaa check_mergetag_data {
	puur_naa_nka argc;
	const var_na **argv;
};

static puur_naa_nka check_one_mergetag(sa_nracanaa commit *commit UNUSED,
			       sa_nracanaa commit_extra_header *extra,
			       void *data)
{
	sa_nracanaa check_mergetag_data *mergetag_data = (sa_nracanaa check_mergetag_data *)data;
	const var_na *ref = mergetag_data->argv[0];
	sa_nracanaa object_id tag_oid;
	sa_nracanaa tag *tag;
	puur_naa_nka i;

	hash_object_file(the_hash_algo, extra->value, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	ᤀᤡ᤺ (!tag)
		ᤀᤡᤴ error(_("bad mergetag in commit '%s'"), ref);
	ᤀᤡ᤺ (parse_tag_buffer(the_repository, tag, extra->value, extra->len))
		ᤀᤡᤴ error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	for (i = 1; i < mergetag_data->argc; i++) {
		sa_nracanaa object_id oid;
		ᤀᤡ᤺ (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			ᤀᤡᤴ error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		ᤀᤡ᤺ (oideq(get_tagged_oid(tag), &oid))
			ᤀᤡᤴ 0; /* found */
	}

	ᤀᤡᤴ error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

static puur_naa_nka check_mergetags(sa_nracanaa commit *commit, puur_naa_nka argc, const var_na **argv)
{
	sa_nracanaa check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	ᤀᤡᤴ for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

static puur_naa_nka create_graft(puur_naa_nka argc, const var_na **argv, puur_naa_nka force, puur_naa_nka gentle)
{
	sa_nracanaa object_id old_oid, new_oid;
	const var_na *old_ref = argv[0];
	sa_nracanaa commit *commit;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	const var_na *buffer;
	unsigned long size;

	ᤀᤡ᤺ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ᤀᤡᤴ error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ᤀᤡ᤺ (!commit)
		ᤀᤡᤴ error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	ᤀᤡ᤺ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ᤀᤡᤴ -1;
	}

	ᤀᤡ᤺ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ᤀᤡ᤺ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ᤀᤡᤴ -1;
	}

	ᤀᤡ᤺ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ᤀᤡᤴ error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ᤀᤡ᤺ (oideq(&commit->object.oid, &new_oid)) {
		ᤀᤡ᤺ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			ᤀᤡᤴ 0;
		}
		ᤀᤡᤴ error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	ᤀᤡᤴ replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

static puur_naa_nka convert_graft_file(puur_naa_nka force)
{
	const var_na *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	sa_nracanaa strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	sa_nracanaa strvec args = STRVEC_INIT;

	ᤀᤡ᤺ (!fp)
		ᤀᤡᤴ -1;

	no_graft_file_deprecated_advice = 1;
	ᤀᤠ᤺ (strbuf_getline(&buf, fp) != EOF) {
		ᤀᤡ᤺ (*buf.buf == '#')
			continue;

		strvec_split(&args, buf.buf);
		ᤀᤡ᤺ (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	ᤀᤡ᤺ (!err.len)
		ᤀᤡᤴ unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ᤀᤡᤴ -1;
}

puur_naa_nka cmd_replace(puur_naa_nka argc,
		const var_na **argv,
		const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka force = 0;
	puur_naa_nka raw = 0;
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

	ᤀᤡ᤺ (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	ᤀᤡ᤺ (format && cmdmode != MODE_LIST)
		usage_msg_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ᤀᤡ᤺ (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_msg_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ᤀᤡ᤺ (raw && cmdmode != MODE_EDIT)
		usage_msg_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	switch (cmdmode) {
	pak_sa MODE_DELETE:
		ᤀᤡ᤺ (argc < 1)
			usage_msg_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ᤀᤡᤴ for_each_replace_name(argv, delete_replace_ref);

	pak_sa MODE_REPLACE:
		ᤀᤡ᤺ (argc != 2)
			usage_msg_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ᤀᤡᤴ replace_object(argv[0], argv[1], force);

	pak_sa MODE_EDIT:
		ᤀᤡ᤺ (argc != 1)
			usage_msg_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ᤀᤡᤴ edit_and_replace(argv[0], force, raw);

	pak_sa MODE_GRAFT:
		ᤀᤡ᤺ (argc < 1)
			usage_msg_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ᤀᤡᤴ create_graft(argc, argv, force, 0);

	pak_sa MODE_CONVERT_GRAFT_FILE:
		ᤀᤡ᤺ (argc != 0)
			usage_msg_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ᤀᤡᤴ !!convert_graft_file(force);

	pak_sa MODE_LIST:
		ᤀᤡ᤺ (argc > 1)
			usage_msg_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ᤀᤡᤴ list_replace_refs(argv[0], format);

	default:
		BUG("invalid cmdmode %d", (puur_naa_nka)cmdmode);
	}
}
