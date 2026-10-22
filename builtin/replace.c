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
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "path.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "replace-object.h"
#ଅନ୍ତର୍ଭାଵ "tag.h"
#ଅନ୍ତର୍ଭାଵ "wildmatch.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ଖାଲି
};

ଗଣନା replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

ସଂରଚନା show_data {
	ସଂରଚନା repository *repo;
	ଅଚଳ ଅକ୍ଷର *pattern;
	ଗଣନା replace_format format;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_reference(ଅଚଳ ସଂରଚନା reference *ref, ଶୂନ୍ୟ *cb_data)
{
	ସଂରଚନା show_data *data = cb_data;

	ଯଦି (!wildmatch(data->pattern, ref->name, 0)) {
		ଯଦି (data->format == REPLACE_FORMAT_SHORT)
			printf("%s\n", ref->name);
		ଅନ୍ୟଥା ଯଦି (data->format == REPLACE_FORMAT_MEDIUM)
			printf("%s -> %s\n", ref->name, oid_to_hex(ref->oid));
		ଅନ୍ୟଥା { /* data->format == REPLACE_FORMAT_LONG */
			ସଂରଚନା object_id object;
			ଗଣନା object_type obj_type, repl_type;

			ଯଦି (repo_get_oid(data->repo, ref->name, &object))
				ଫେରନ୍ତୁ error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(data->repo->objects, &object, ଖାଲି);
			repl_type = odb_read_object_info(data->repo->objects, ref->oid, ଖାଲି);

			printf("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_to_hex(ref->oid), type_name(repl_type));
		}
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_replace_refs(ଅଚଳ ଅକ୍ଷର *pattern, ଅଚଳ ଅକ୍ଷର *format)
{
	ସଂରଚନା show_data data;

	data.repo = the_repository;
	ଯଦି (!pattern)
		pattern = "*";
	data.pattern = pattern;

	ଯଦି (format == ଖାଲି || *format == '\0' || !strcmp(format, "short"))
		data.format = REPLACE_FORMAT_SHORT;
	ଅନ୍ୟଥା ଯଦି (!strcmp(format, "medium"))
		data.format = REPLACE_FORMAT_MEDIUM;
	ଅନ୍ୟଥା ଯଦି (!strcmp(format, "long"))
		data.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (ଶୂନ୍ୟ *)&data);

	ଫେରନ୍ତୁ 0;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*each_replace_name_fn)(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *ref,
				    ଅଚଳ ସଂରଚନା object_id *oid);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା for_each_replace_name(ଅଚଳ ଅକ୍ଷର **argv, each_replace_name_fn fn)
{
	ଅଚଳ ଅକ୍ଷର **p, *full_hex;
	ସଂରଚନା strbuf ref = STRBUF_INIT;
	size_t base_len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା had_error = 0;
	ସଂରଚନା object_id oid;
	ଅଚଳ ଅକ୍ଷର *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_base);
	base_len = ref.len;

	ପାଇଁ (p = argv; *p; p++) {
		ଯଦି (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			ଜାରି;
		}

		strbuf_setlen(&ref, base_len);
		strbuf_add_oid_hex(&ref, &oid);
		full_hex = ref.buf + base_len;

		ଯଦି (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), full_hex);
			had_error = 1;
			ଜାରି;
		}
		ଯଦି (fn(full_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	ଫେରନ୍ତୁ had_error;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା delete_replace_ref(ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *ref,
			      ଅଚଳ ସଂରଚନା object_id *oid)
{
	ଯଦି (refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି, ref, oid, 0))
		ଫେରନ୍ତୁ 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_ref_valid(ସଂରଚନା object_id *object,
			    ସଂରଚନା object_id *prev,
			    ସଂରଚନା strbuf *ref,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା force)
{
	ଅଚଳ ଅକ୍ଷର *git_replace_ref_base = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_base, oid_to_hex(object));
	ଯଦି (check_refname_format(ref->buf, 0))
		ଫେରନ୍ତୁ error(_("'%s' is not a valid ref name"), ref->buf);

	ଯଦି (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	ଅନ୍ୟଥା ଯଦି (!force)
		ଫେରନ୍ତୁ error(_("replace ref '%s' already exists"), ref->buf);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା replace_object_oid(ଅଚଳ ଅକ୍ଷର *object_ref,
			       ସଂରଚନା object_id *object,
			       ଅଚଳ ଅକ୍ଷର *replace_ref,
			       ସଂରଚନା object_id *repl,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା force)
{
	ସଂରଚନା object_id prev;
	ଗଣନା object_type obj_type, repl_type;
	ସଂରଚନା strbuf ref = STRBUF_INIT;
	ସଂରଚନା ref_transaction *transaction;
	ସଂରଚନା strbuf err = STRBUF_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ଖାଲି);
	repl_type = odb_read_object_info(the_repository->objects, repl, ଖାଲି);
	ଯଦି (!force && obj_type != repl_type)
		ଫେରନ୍ତୁ error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ଯଦି (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		ଫେରନ୍ତୁ -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ଯଦି (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ଖାଲି, ଖାଲି, 0, ଖାଲି, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	ଫେରନ୍ତୁ res;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା replace_object(ଅଚଳ ଅକ୍ଷର *object_ref, ଅଚଳ ଅକ୍ଷର *replace_ref, ପୂର୍ଣ୍ଣସଂଖ୍ୟା force)
{
	ସଂରଚନା object_id object, repl;

	ଯଦି (repo_get_oid(the_repository, object_ref, &object))
		ଫେରନ୍ତୁ error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ଯଦି (repo_get_oid(the_repository, replace_ref, &repl))
		ଫେରନ୍ତୁ error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	ଫେରନ୍ତୁ replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା export_object(ଅଚଳ ସଂରଚନା object_id *oid, ଗଣନା object_type type,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା raw, ଅଚଳ ଅକ୍ଷର *filename)
{
	ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ error_errno(_("unable to open %s for writing"), filename);

	strvec_ଠେଲ(&cmd.args, "--no-replace-objects");
	strvec_ଠେଲ(&cmd.args, "cat-file");
	ଯଦି (raw)
		strvec_ଠେଲ(&cmd.args, type_name(type));
	ଅନ୍ୟଥା
		strvec_ଠେଲ(&cmd.args, "-p");
	strvec_ଠେଲ(&cmd.args, oid_to_hex(oid));
	cmd.git_cmd = 1;
	cmd.out = fd;

	ଯଦି (run_command(&cmd))
		ଫେରନ୍ତୁ error(_("cat-file reported failure"));
	ଫେରନ୍ତୁ 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା import_object(ସଂରଚନା object_id *oid, ଗଣନା object_type type,
			  ପୂର୍ଣ୍ଣସଂଖ୍ୟା raw, ଅଚଳ ଅକ୍ଷର *filename)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

	fd = open(filename, O_RDONLY);
	ଯଦି (fd < 0)
		ଫେରନ୍ତୁ error_errno(_("unable to open %s for reading"), filename);

	ଯଦି (!raw && type == OBJ_ବୃକ୍ଷ) {
		ସଂରଚନା child_process cmd = CHILD_PROCESS_INIT;
		ସଂରଚନା strbuf result = STRBUF_INIT;

		strvec_ଠେଲ(&cmd.args, "mktree");
		cmd.git_cmd = 1;
		cmd.in = fd;
		cmd.out = -1;

		ଯଦି (start_command(&cmd)) {
			close(fd);
			ଫେରନ୍ତୁ error(_("unable to spawn mktree"));
		}

		ଯଦି (strbuf_read(&result, cmd.out, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.out);
			ଫେରନ୍ତୁ -1;
		}
		close(cmd.out);

		ଯଦି (finish_command(&cmd)) {
			strbuf_release(&result);
			ଫେରନ୍ତୁ error(_("mktree reported failure"));
		}
		ଯଦି (get_oid_hex(result.buf, oid) < 0) {
			strbuf_release(&result);
			ଫେରନ୍ତୁ error(_("mktree did not return an object name"));
		}

		strbuf_release(&result);
	} ଅନ୍ୟଥା {
		ସଂରଚନା stat st;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = INDEX_FORMAT_CHECK | INDEX_WRITE_OBJECT;

		ଯଦି (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			ଫେରନ୍ତୁ -1;
		}
		ଯଦି (index_fd(the_repository->index, oid, fd, &st, type, ଖାଲି, flags) < 0)
			ଫେରନ୍ତୁ error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit_and_replace(ଅଚଳ ଅକ୍ଷର *object_ref, ପୂର୍ଣ୍ଣସଂଖ୍ୟା force, ପୂର୍ଣ୍ଣସଂଖ୍ୟା raw)
{
	ଅକ୍ଷର *tmpfile;
	ଗଣନା object_type type;
	ସଂରଚନା object_id old_oid, new_oid, prev;
	ସଂରଚନା strbuf ref = STRBUF_INIT;

	ଯଦି (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		ଫେରନ୍ତୁ error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ଖାଲି);
	ଯଦି (type < 0)
		ଫେରନ୍ତୁ error(_("unable to get object type for %s"),
			     oid_to_hex(&old_oid));

	ଯଦି (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		ଫେରନ୍ତୁ -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_path(the_repository, "REPLACE_EDITOBJ");
	ଯଦି (export_object(&old_oid, type, raw, tmpfile)) {
		ମୁକ୍ତ(tmpfile);
		ଫେରନ୍ତୁ -1;
	}
	ଯଦି (launch_editor(tmpfile, ଖାଲି, ଖାଲି) < 0) {
		ମୁକ୍ତ(tmpfile);
		ଫେରନ୍ତୁ error(_("editing object file failed"));
	}
	ଯଦି (import_object(&new_oid, type, raw, tmpfile)) {
		ମୁକ୍ତ(tmpfile);
		ଫେରନ୍ତୁ -1;
	}
	ମୁକ୍ତ(tmpfile);

	ଯଦି (oideq(&old_oid, &new_oid))
		ଫେରନ୍ତୁ error(_("new object is the same as the old one: '%s'"), oid_to_hex(&old_oid));

	ଫେରନ୍ତୁ replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା replace_parents(ସଂରଚନା strbuf *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା strbuf new_parents = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *parent_start, *parent_end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଅଚଳ unsigned hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	parent_start = buf->buf;
	parent_start += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	parent_end = parent_start;

	ଯେତେବେଳେ (starts_with(parent_end, "parent "))
		parent_end += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	ପାଇଁ (i = 0; i < argc; i++) {
		ସଂରଚନା object_id oid;
		ସଂରଚନା commit *commit;

		ଯଦି (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			ଫେରନ୍ତୁ error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ଯଦି (!commit) {
			strbuf_release(&new_parents);
			ଫେରନ୍ତୁ error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_to_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, parent_start - buf->buf, parent_end - parent_start,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା check_mergetag_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;
	ଅଚଳ ଅକ୍ଷର **argv;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_one_mergetag(ସଂରଚନା commit *commit UNUSED,
			       ସଂରଚନା commit_extra_header *extra,
			       ଶୂନ୍ୟ *data)
{
	ସଂରଚନା check_mergetag_data *mergetag_data = (ସଂରଚନା check_mergetag_data *)data;
	ଅଚଳ ଅକ୍ଷର *ref = mergetag_data->argv[0];
	ସଂରଚନା object_id tag_oid;
	ସଂରଚନା tag *tag;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	hash_object_file(the_hash_algo, extra->ମୂଲ୍ୟ, extra->len,
			 OBJ_TAG, &tag_oid);
	tag = lookup_tag(the_repository, &tag_oid);
	ଯଦି (!tag)
		ଫେରନ୍ତୁ error(_("bad mergetag in commit '%s'"), ref);
	ଯଦି (parse_tag_buffer(the_repository, tag, extra->ମୂଲ୍ୟ, extra->len))
		ଫେରନ୍ତୁ error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	ପାଇଁ (i = 1; i < mergetag_data->argc; i++) {
		ସଂରଚନା object_id oid;
		ଯଦି (repo_get_oid(the_repository, mergetag_data->argv[i], &oid) < 0)
			ଫେରନ୍ତୁ error(_("not a valid object name: '%s'"),
				     mergetag_data->argv[i]);
		ଯଦି (oideq(get_tagged_oid(tag), &oid))
			ଫେରନ୍ତୁ 0; /* found */
	}

	ଫେରନ୍ତୁ error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_to_hex(&tag_oid));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_mergetags(ସଂରଚନା commit *commit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ସଂରଚନା check_mergetag_data mergetag_data;

	mergetag_data.argc = argc;
	mergetag_data.argv = argv;
	ଫେରନ୍ତୁ for_each_mergetag(check_one_mergetag, commit, &mergetag_data);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା create_graft(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା force, ପୂର୍ଣ୍ଣସଂଖ୍ୟା gentle)
{
	ସଂରଚନା object_id old_oid, new_oid;
	ଅଚଳ ଅକ୍ଷର *old_ref = argv[0];
	ସଂରଚନା commit *commit;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ଅଚଳ ଅକ୍ଷର *buffer;
	unsigned long size;

	ଯଦି (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		ଫେରନ୍ତୁ error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ଯଦି (!commit)
		ଫେରନ୍ତୁ error(_("could not parse %s"), old_ref);

	buffer = repo_get_commit_buffer(the_repository, commit, &size);
	strbuf_add(&buf, buffer, size);
	repo_unuse_commit_buffer(the_repository, commit, buffer);

	ଯଦି (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ଯଦି (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		ଫେରନ୍ତୁ -1;
	}

	ଯଦି (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		ଫେରନ୍ତୁ error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ଯଦି (oideq(&commit->object.oid, &new_oid)) {
		ଯଦି (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_to_hex(&commit->object.oid));
			ଫେରନ୍ତୁ 0;
		}
		ଫେରନ୍ତୁ error(_("new commit is the same as the old one: '%s'"),
			     oid_to_hex(&commit->object.oid));
	}

	ଫେରନ୍ତୁ replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା convert_graft_file(ପୂର୍ଣ୍ଣସଂଖ୍ୟା force)
{
	ଅଚଳ ଅକ୍ଷର *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	ସଂରଚନା strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	ସଂରଚନା strvec args = STRVEC_INIT;

	ଯଦି (!fp)
		ଫେରନ୍ତୁ -1;

	no_graft_file_deprecated_advice = 1;
	ଯେତେବେଳେ (strbuf_getline(&buf, fp) != EOF) {
		ଯଦି (*buf.buf == '#')
			ଜାରି;

		strvec_split(&args, buf.buf);
		ଯଦି (args.nr && create_graft(args.nr, args.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&args);
	}
	fclose(fp);

	strbuf_release(&buf);

	ଯଦି (!err.len)
		ଫେରନ୍ତୁ unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_replace(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		ଅଚଳ ଅକ୍ଷର **argv,
		ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା raw = 0;
	ଅଚଳ ଅକ୍ଷର *format = ଖାଲି;
	ଗଣନା {
		MODE_UNSPECIFIED = 0,
		MODE_LIST,
		MODE_DELETE,
		MODE_EDIT,
		MODE_GRAFT,
		MODE_CONVERT_GRAFT_FILE,
		MODE_REPLACE
	} cmdmode = MODE_UNSPECIFIED;
	ସଂରଚନା option options[] = {
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
	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, options, git_replace_usage, 0);

	ଯଦି (!cmdmode)
		cmdmode = argc ? MODE_REPLACE : MODE_LIST;

	ଯଦି (format && cmdmode != MODE_LIST)
		usage_ସନ୍ଦେଶ_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ଯଦି (force &&
	    cmdmode != MODE_REPLACE &&
	    cmdmode != MODE_EDIT &&
	    cmdmode != MODE_GRAFT &&
	    cmdmode != MODE_CONVERT_GRAFT_FILE)
		usage_ସନ୍ଦେଶ_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ଯଦି (raw && cmdmode != MODE_EDIT)
		usage_ସନ୍ଦେଶ_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	ବାଛ (cmdmode) {
	ସ୍ଥିତି MODE_DELETE:
		ଯଦି (argc < 1)
			usage_ସନ୍ଦେଶ_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		ଫେରନ୍ତୁ for_each_replace_name(argv, delete_replace_ref);

	ସ୍ଥିତି MODE_REPLACE:
		ଯଦି (argc != 2)
			usage_ସନ୍ଦେଶ_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		ଫେରନ୍ତୁ replace_object(argv[0], argv[1], force);

	ସ୍ଥିତି MODE_EDIT:
		ଯଦି (argc != 1)
			usage_ସନ୍ଦେଶ_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		ଫେରନ୍ତୁ edit_and_replace(argv[0], force, raw);

	ସ୍ଥିତି MODE_GRAFT:
		ଯଦି (argc < 1)
			usage_ସନ୍ଦେଶ_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		ଫେରନ୍ତୁ create_graft(argc, argv, force, 0);

	ସ୍ଥିତି MODE_CONVERT_GRAFT_FILE:
		ଯଦି (argc != 0)
			usage_ସନ୍ଦେଶ_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		ଫେରନ୍ତୁ !!convert_graft_file(force);

	ସ୍ଥିତି MODE_LIST:
		ଯଦି (argc > 1)
			usage_ସନ୍ଦେଶ_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		ଫେରନ୍ତୁ list_replace_refs(argv[0], format);

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		BUG("invalid cmdmode %d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cmdmode);
	}
}
