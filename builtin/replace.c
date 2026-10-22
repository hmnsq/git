/*
 * Builtin "git replace"
 *
 * Copyright (c) 2008 Christian Couder <chriscool@tuxfamily.org>
 *
 * Based on builtin/tag.c by Kristian Høgsberg <krh@redhat.com>
 * and Carlos Rica <jasampler@gmail.com> that was itself based on
 * git-tag.sh and mktag.c by Linus Torvalds.
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "replace-object.h"
#అన్తర్భావ "tag.h"
#అన్తర్భావ "wildmatch.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_replace_usage[] = {
	N_("git replace [-f] <object> <replacement>"),
	N_("git replace [-f] --edit <object>"),
	N_("git replace [-f] --graft <commit> [<parent>...]"),
	"git replace [-f] --convert-graft-file",
	N_("git replace -d <object>..."),
	N_("git replace [--format=<format>] [-l [<pattern>]]"),
	ఖాళీ
};

గణన replace_format {
	REPLACE_FORMAT_SHORT,
	REPLACE_FORMAT_MEDIUM,
	REPLACE_FORMAT_LONG
};

నిర్మాణం show_దత్తాంశం {
	నిర్మాణం repository *repo;
	స్థిరరాశి అక్షరం *నమూనా;
	గణన replace_format format;
};

స్థిర పూర్ణాంకం show_reference(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం)
{
	నిర్మాణం show_దత్తాంశం *దత్తాంశం = cb_దత్తాంశం;

	ఒకవేళ (!wildmatch(దత్తాంశం->నమూనా, ref->name, 0)) {
		ఒకవేళ (దత్తాంశం->format == REPLACE_FORMAT_SHORT)
			రాయి("%s\n", ref->name);
		లేకపోతే ఒకవేళ (దత్తాంశం->format == REPLACE_FORMAT_MEDIUM)
			రాయి("%s -> %s\n", ref->name, oid_గమ్యం_hex(ref->oid));
		లేకపోతే { /* data->format == REPLACE_FORMAT_LONG */
			నిర్మాణం object_id object;
			గణన object_type obj_type, repl_type;

			ఒకవేళ (repo_get_oid(దత్తాంశం->repo, ref->name, &object))
				తిరిగి error(_("failed to resolve '%s' as a valid ref"), ref->name);

			obj_type = odb_read_object_info(దత్తాంశం->repo->objects, &object, ఖాళీ);
			repl_type = odb_read_object_info(దత్తాంశం->repo->objects, ref->oid, ఖాళీ);

			రాయి("%s (%s) -> %s (%s)\n", ref->name, type_name(obj_type),
			       oid_గమ్యం_hex(ref->oid), type_name(repl_type));
		}
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం list_replace_refs(స్థిరరాశి అక్షరం *నమూనా, స్థిరరాశి అక్షరం *format)
{
	నిర్మాణం show_దత్తాంశం దత్తాంశం;

	దత్తాంశం.repo = the_repository;
	ఒకవేళ (!నమూనా)
		నమూనా = "*";
	దత్తాంశం.నమూనా = నమూనా;

	ఒకవేళ (format == ఖాళీ || *format == '\0' || !strcmp(format, "short"))
		దత్తాంశం.format = REPLACE_FORMAT_SHORT;
	లేకపోతే ఒకవేళ (!strcmp(format, "medium"))
		దత్తాంశం.format = REPLACE_FORMAT_MEDIUM;
	లేకపోతే ఒకవేళ (!strcmp(format, "long"))
		దత్తాంశం.format = REPLACE_FORMAT_LONG;
	/*
	 * Please update _git_replace() in git-completion.bash when
	 * you add new format
	 */
	లేకపోతే
		తిరిగి error(_("invalid replace format '%s'\n"
			       "valid formats are 'short', 'medium' and 'long'"),
			     format);

	refs_for_each_replace_ref(get_main_ref_store(the_repository),
				  show_reference, (శూన్య *)&దత్తాంశం);

	తిరిగి 0;
}

రకనిర్వచనం పూర్ణాంకం (*each_replace_name_fn)(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ref,
				    స్థిరరాశి నిర్మాణం object_id *oid);

స్థిర పూర్ణాంకం for_each_replace_name(స్థిరరాశి అక్షరం **argv, each_replace_name_fn fn)
{
	స్థిరరాశి అక్షరం **p, *నిండిన_hex;
	నిర్మాణం strbuf ref = STRBUF_INIT;
	కొలత_t పునాది_len;
	పూర్ణాంకం had_error = 0;
	నిర్మాణం object_id oid;
	స్థిరరాశి అక్షరం *git_replace_ref_పునాది = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_addstr(&ref, git_replace_ref_పునాది);
	పునాది_len = ref.len;

	కొరకు (p = argv; *p; p++) {
		ఒకవేళ (repo_get_oid(the_repository, *p, &oid)) {
			error("failed to resolve '%s' as a valid ref", *p);
			had_error = 1;
			కొనసాగు;
		}

		strbuf_setlen(&ref, పునాది_len);
		strbuf_కూడు_oid_hex(&ref, &oid);
		నిండిన_hex = ref.buf + పునాది_len;

		ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), ref.buf, &oid)) {
			error(_("replace ref '%s' not found"), నిండిన_hex);
			had_error = 1;
			కొనసాగు;
		}
		ఒకవేళ (fn(నిండిన_hex, ref.buf, &oid))
			had_error = 1;
	}
	strbuf_release(&ref);
	తిరిగి had_error;
}

స్థిర పూర్ణాంకం delete_replace_ref(స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *ref,
			      స్థిరరాశి నిర్మాణం object_id *oid)
{
	ఒకవేళ (refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ, ref, oid, 0))
		తిరిగి 1;
	printf_ln(_("Deleted replace ref '%s'"), name);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం check_ref_valid(నిర్మాణం object_id *object,
			    నిర్మాణం object_id *prev,
			    నిర్మాణం strbuf *ref,
			    పూర్ణాంకం force)
{
	స్థిరరాశి అక్షరం *git_replace_ref_పునాది = ref_namespace[NAMESPACE_REPLACE].ref;

	strbuf_reset(ref);
	strbuf_addf(ref, "%s%s", git_replace_ref_పునాది, oid_గమ్యం_hex(object));
	ఒకవేళ (check_refname_format(ref->buf, 0))
		తిరిగి error(_("'%s' is not a valid ref name"), ref->buf);

	ఒకవేళ (refs_read_ref(get_main_ref_store(the_repository), ref->buf, prev))
		oidclr(prev, the_repository->hash_algo);
	లేకపోతే ఒకవేళ (!force)
		తిరిగి error(_("replace ref '%s' already exists"), ref->buf);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం replace_object_oid(స్థిరరాశి అక్షరం *object_ref,
			       నిర్మాణం object_id *object,
			       స్థిరరాశి అక్షరం *replace_ref,
			       నిర్మాణం object_id *repl,
			       పూర్ణాంకం force)
{
	నిర్మాణం object_id prev;
	గణన object_type obj_type, repl_type;
	నిర్మాణం strbuf ref = STRBUF_INIT;
	నిర్మాణం ref_transaction *transaction;
	నిర్మాణం strbuf err = STRBUF_INIT;
	పూర్ణాంకం res = 0;

	obj_type = odb_read_object_info(the_repository->objects, object, ఖాళీ);
	repl_type = odb_read_object_info(the_repository->objects, repl, ఖాళీ);
	ఒకవేళ (!force && obj_type != repl_type)
		తిరిగి error(_("Objects must be of the same type.\n"
			       "'%s' points to a replaced object of type '%s'\n"
			       "while '%s' points to a replacement object of "
			       "type '%s'."),
			     object_ref, type_name(obj_type),
			     replace_ref, type_name(repl_type));

	ఒకవేళ (check_ref_valid(object, &prev, &ref, force)) {
		strbuf_release(&ref);
		తిరిగి -1;
	}

	transaction = ref_store_transaction_begin(get_main_ref_store(the_repository),
						  0, &err);
	ఒకవేళ (!transaction ||
	    ref_transaction_update(transaction, ref.buf, repl, &prev,
				   ఖాళీ, ఖాళీ, 0, ఖాళీ, &err) ||
	    ref_transaction_commit(transaction, &err))
		res = error("%s", err.buf);

	ref_transaction_free(transaction);
	strbuf_release(&ref);
	తిరిగి res;
}

స్థిర పూర్ణాంకం replace_object(స్థిరరాశి అక్షరం *object_ref, స్థిరరాశి అక్షరం *replace_ref, పూర్ణాంకం force)
{
	నిర్మాణం object_id object, repl;

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &object))
		తిరిగి error(_("failed to resolve '%s' as a valid ref"),
			     object_ref);
	ఒకవేళ (repo_get_oid(the_repository, replace_ref, &repl))
		తిరిగి error(_("failed to resolve '%s' as a valid ref"),
			     replace_ref);

	తిరిగి replace_object_oid(object_ref, &object, replace_ref, &repl, force);
}

/*
 * Write the contents of the object named by "sha1" to the file "filename".
 * If "raw" is true, then the object's raw contents are printed according to
 * "type". Otherwise, we pretty-print the contents for human editing.
 */
స్థిర పూర్ణాంకం export_object(స్థిరరాశి నిర్మాణం object_id *oid, గణన object_type type,
			  పూర్ణాంకం raw, స్థిరరాశి అక్షరం *filename)
{
	నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
	పూర్ణాంకం fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	ఒకవేళ (fd < 0)
		తిరిగి error_errno(_("unable to open %s for writing"), filename);

	strvec_నెట్టు(&cmd.వాదనలు, "--no-replace-objects");
	strvec_నెట్టు(&cmd.వాదనలు, "cat-file");
	ఒకవేళ (raw)
		strvec_నెట్టు(&cmd.వాదనలు, type_name(type));
	లేకపోతే
		strvec_నెట్టు(&cmd.వాదనలు, "-p");
	strvec_నెట్టు(&cmd.వాదనలు, oid_గమ్యం_hex(oid));
	cmd.git_cmd = 1;
	cmd.బహిర్గతం = fd;

	ఒకవేళ (నిరంతరం_command(&cmd))
		తిరిగి error(_("cat-file reported failure"));
	తిరిగి 0;
}

/*
 * Read a previously-exported (and possibly edited) object back from "filename",
 * interpreting it as "type", and writing the result to the object database.
 * The sha1 of the written object is returned via sha1.
 */
స్థిర పూర్ణాంకం import_object(నిర్మాణం object_id *oid, గణన object_type type,
			  పూర్ణాంకం raw, స్థిరరాశి అక్షరం *filename)
{
	పూర్ణాంకం fd;

	fd = open(filename, O_RDONLY);
	ఒకవేళ (fd < 0)
		తిరిగి error_errno(_("unable to open %s for reading"), filename);

	ఒకవేళ (!raw && type == OBJ_వృక్షం) {
		నిర్మాణం బిడ్డ_process cmd = బిడ్డ_PROCESS_INIT;
		నిర్మాణం strbuf ఫలితం = STRBUF_INIT;

		strvec_నెట్టు(&cmd.వాదనలు, "mktree");
		cmd.git_cmd = 1;
		cmd.లోపల = fd;
		cmd.బహిర్గతం = -1;

		ఒకవేళ (ప్రారంభం_command(&cmd)) {
			close(fd);
			తిరిగి error(_("unable to spawn mktree"));
		}

		ఒకవేళ (strbuf_read(&ఫలితం, cmd.బహిర్గతం, the_hash_algo->hexsz + 1) < 0) {
			error_errno(_("unable to read from mktree"));
			close(fd);
			close(cmd.బహిర్గతం);
			తిరిగి -1;
		}
		close(cmd.బహిర్గతం);

		ఒకవేళ (ముగింపు_command(&cmd)) {
			strbuf_release(&ఫలితం);
			తిరిగి error(_("mktree reported failure"));
		}
		ఒకవేళ (get_oid_hex(ఫలితం.buf, oid) < 0) {
			strbuf_release(&ఫలితం);
			తిరిగి error(_("mktree did not return an object name"));
		}

		strbuf_release(&ఫలితం);
	} లేకపోతే {
		నిర్మాణం stat st;
		పూర్ణాంకం flags = సూచీ_FORMAT_CHECK | సూచీ_WRITE_OBJECT;

		ఒకవేళ (fstat(fd, &st) < 0) {
			error_errno(_("unable to fstat %s"), filename);
			close(fd);
			తిరిగి -1;
		}
		ఒకవేళ (సూచీ_fd(the_repository->సూచీ, oid, fd, &st, type, ఖాళీ, flags) < 0)
			తిరిగి error(_("unable to write object to database"));
		/* index_fd close()s fd for us */
	}

	/*
	 * No need to close(fd) here; both run-command and index-fd
	 * will have done it for us.
	 */
	తిరిగి 0;
}

స్థిర పూర్ణాంకం edit_and_replace(స్థిరరాశి అక్షరం *object_ref, పూర్ణాంకం force, పూర్ణాంకం raw)
{
	అక్షరం *tmpfile;
	గణన object_type type;
	నిర్మాణం object_id old_oid, new_oid, prev;
	నిర్మాణం strbuf ref = STRBUF_INIT;

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &old_oid) < 0)
		తిరిగి error(_("not a valid object name: '%s'"), object_ref);

	type = odb_read_object_info(the_repository->objects, &old_oid, ఖాళీ);
	ఒకవేళ (type < 0)
		తిరిగి error(_("unable to get object type for %s"),
			     oid_గమ్యం_hex(&old_oid));

	ఒకవేళ (check_ref_valid(&old_oid, &prev, &ref, force)) {
		strbuf_release(&ref);
		తిరిగి -1;
	}
	strbuf_release(&ref);

	tmpfile = repo_git_మార్గం(the_repository, "REPLACE_EDITOBJ");
	ఒకవేళ (export_object(&old_oid, type, raw, tmpfile)) {
		విడుదల(tmpfile);
		తిరిగి -1;
	}
	ఒకవేళ (launch_editor(tmpfile, ఖాళీ, ఖాళీ) < 0) {
		విడుదల(tmpfile);
		తిరిగి error(_("editing object file failed"));
	}
	ఒకవేళ (import_object(&new_oid, type, raw, tmpfile)) {
		విడుదల(tmpfile);
		తిరిగి -1;
	}
	విడుదల(tmpfile);

	ఒకవేళ (oideq(&old_oid, &new_oid))
		తిరిగి error(_("new object is the same as the old one: '%s'"), oid_గమ్యం_hex(&old_oid));

	తిరిగి replace_object_oid(object_ref, &old_oid, "replacement", &new_oid, force);
}

స్థిర పూర్ణాంకం replace_parents(నిర్మాణం strbuf *buf, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం strbuf new_parents = STRBUF_INIT;
	స్థిరరాశి అక్షరం *మాతృ_ప్రారంభం, *మాతృ_చివర;
	పూర్ణాంకం i;
	స్థిరరాశి గుర్తులేని hexsz = the_hash_algo->hexsz;

	/* find existing parents */
	మాతృ_ప్రారంభం = buf->buf;
	మాతృ_ప్రారంభం += hexsz + 6; /* "tree " + "hex sha1" + "\n" */
	మాతృ_చివర = మాతృ_ప్రారంభం;

	అయితే (starts_with(మాతృ_చివర, "parent "))
		మాతృ_చివర += hexsz + 8; /* "parent " + "hex sha1" + "\n" */

	/* prepare new parents */
	కొరకు (i = 0; i < argc; i++) {
		నిర్మాణం object_id oid;
		నిర్మాణం commit *commit;

		ఒకవేళ (repo_get_oid(the_repository, argv[i], &oid) < 0) {
			strbuf_release(&new_parents);
			తిరిగి error(_("not a valid object name: '%s'"),
				     argv[i]);
		}
		commit = lookup_commit_reference(the_repository, &oid);
		ఒకవేళ (!commit) {
			strbuf_release(&new_parents);
			తిరిగి error(_("could not parse %s as a commit"), argv[i]);
		}
		strbuf_addf(&new_parents, "parent %s\n", oid_గమ్యం_hex(&commit->object.oid));
	}

	/* replace existing parents with new ones */
	strbuf_splice(buf, మాతృ_ప్రారంభం - buf->buf, మాతృ_చివర - మాతృ_ప్రారంభం,
		      new_parents.buf, new_parents.len);

	strbuf_release(&new_parents);
	తిరిగి 0;
}

నిర్మాణం check_mergetag_దత్తాంశం {
	పూర్ణాంకం argc;
	స్థిరరాశి అక్షరం **argv;
};

స్థిర పూర్ణాంకం check_one_mergetag(నిర్మాణం commit *commit వాడని,
			       నిర్మాణం commit_extra_శీర్షిక *extra,
			       శూన్య *దత్తాంశం)
{
	నిర్మాణం check_mergetag_దత్తాంశం *mergetag_దత్తాంశం = (నిర్మాణం check_mergetag_దత్తాంశం *)దత్తాంశం;
	స్థిరరాశి అక్షరం *ref = mergetag_దత్తాంశం->argv[0];
	నిర్మాణం object_id ట్యాగ్_oid;
	నిర్మాణం ట్యాగ్ *ట్యాగ్;
	పూర్ణాంకం i;

	hash_object_file(the_hash_algo, extra->విలువ, extra->len,
			 OBJ_ట్యాగ్, &ట్యాగ్_oid);
	ట్యాగ్ = lookup_ట్యాగ్(the_repository, &ట్యాగ్_oid);
	ఒకవేళ (!ట్యాగ్)
		తిరిగి error(_("bad mergetag in commit '%s'"), ref);
	ఒకవేళ (parse_ట్యాగ్_బఫరు(the_repository, ట్యాగ్, extra->విలువ, extra->len))
		తిరిగి error(_("malformed mergetag in commit '%s'"), ref);

	/* iterate over new parents */
	కొరకు (i = 1; i < mergetag_దత్తాంశం->argc; i++) {
		నిర్మాణం object_id oid;
		ఒకవేళ (repo_get_oid(the_repository, mergetag_దత్తాంశం->argv[i], &oid) < 0)
			తిరిగి error(_("not a valid object name: '%s'"),
				     mergetag_దత్తాంశం->argv[i]);
		ఒకవేళ (oideq(get_tagged_oid(ట్యాగ్), &oid))
			తిరిగి 0; /*దొరికింది*/
	}

	తిరిగి error(_("original commit '%s' contains mergetag '%s' that is "
		       "discarded; use --edit instead of --graft"), ref,
		     oid_గమ్యం_hex(&ట్యాగ్_oid));
}

స్థిర పూర్ణాంకం check_mergetags(నిర్మాణం commit *commit, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం check_mergetag_దత్తాంశం mergetag_దత్తాంశం;

	mergetag_దత్తాంశం.argc = argc;
	mergetag_దత్తాంశం.argv = argv;
	తిరిగి for_each_mergetag(check_one_mergetag, commit, &mergetag_దత్తాంశం);
}

స్థిర పూర్ణాంకం create_graft(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, పూర్ణాంకం force, పూర్ణాంకం gentle)
{
	నిర్మాణం object_id old_oid, new_oid;
	స్థిరరాశి అక్షరం *old_ref = argv[0];
	నిర్మాణం commit *commit;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	స్థిరరాశి అక్షరం *బఫరు;
	గుర్తులేని దీర్ఘ కొలత;

	ఒకవేళ (repo_get_oid(the_repository, old_ref, &old_oid) < 0)
		తిరిగి error(_("not a valid object name: '%s'"), old_ref);
	commit = lookup_commit_reference(the_repository, &old_oid);
	ఒకవేళ (!commit)
		తిరిగి error(_("could not parse %s"), old_ref);

	బఫరు = repo_get_commit_బఫరు(the_repository, commit, &కొలత);
	strbuf_కూడు(&buf, బఫరు, కొలత);
	repo_unuse_commit_బఫరు(the_repository, commit, బఫరు);

	ఒకవేళ (replace_parents(&buf, argc - 1, &argv[1]) < 0) {
		strbuf_release(&buf);
		తిరిగి -1;
	}

	ఒకవేళ (remove_signature(&buf)) {
		warning(_("the original commit '%s' has a gpg signature"), old_ref);
		warning(_("the signature will be removed in the replacement commit!"));
	}

	ఒకవేళ (check_mergetags(commit, argc, argv)) {
		strbuf_release(&buf);
		తిరిగి -1;
	}

	ఒకవేళ (odb_write_object(the_repository->objects, buf.buf,
			     buf.len, OBJ_COMMIT, &new_oid)) {
		strbuf_release(&buf);
		తిరిగి error(_("could not write replacement commit for: '%s'"),
			     old_ref);
	}

	strbuf_release(&buf);

	ఒకవేళ (oideq(&commit->object.oid, &new_oid)) {
		ఒకవేళ (gentle) {
			warning(_("graft for '%s' unnecessary"),
				oid_గమ్యం_hex(&commit->object.oid));
			తిరిగి 0;
		}
		తిరిగి error(_("new commit is the same as the old one: '%s'"),
			     oid_గమ్యం_hex(&commit->object.oid));
	}

	తిరిగి replace_object_oid(old_ref, &commit->object.oid,
				  "replacement", &new_oid, force);
}

స్థిర పూర్ణాంకం convert_graft_file(పూర్ణాంకం force)
{
	స్థిరరాశి అక్షరం *graft_file = repo_get_graft_file(the_repository);
	FILE *fp = fopen_or_warn(graft_file, "r");
	నిర్మాణం strbuf buf = STRBUF_INIT, err = STRBUF_INIT;
	నిర్మాణం strvec వాదనలు = STRVEC_INIT;

	ఒకవేళ (!fp)
		తిరిగి -1;

	no_graft_file_deprecated_advice = 1;
	అయితే (strbuf_getline(&buf, fp) != EOF) {
		ఒకవేళ (*buf.buf == '#')
			కొనసాగు;

		strvec_విభజించు(&వాదనలు, buf.buf);
		ఒకవేళ (వాదనలు.nr && create_graft(వాదనలు.nr, వాదనలు.v, force, 1))
			strbuf_addf(&err, "\n\t%s", buf.buf);
		strvec_clear(&వాదనలు);
	}
	fclose(fp);

	strbuf_release(&buf);

	ఒకవేళ (!err.len)
		తిరిగి unlink_or_warn(graft_file);

	warning(_("could not convert the following graft(s):\n%s"), err.buf);
	strbuf_release(&err);

	తిరిగి -1;
}

పూర్ణాంకం cmd_replace(పూర్ణాంకం argc,
		స్థిరరాశి అక్షరం **argv,
		స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం force = 0;
	పూర్ణాంకం raw = 0;
	స్థిరరాశి అక్షరం *format = ఖాళీ;
	గణన {
		బహుళకం_UNSPECIFIED = 0,
		బహుళకం_LIST,
		బహుళకం_DELETE,
		బహుళకం_EDIT,
		బహుళకం_GRAFT,
		బహుళకం_CONVERT_GRAFT_FILE,
		బహుళకం_REPLACE
	} cmdmode = బహుళకం_UNSPECIFIED;
	నిర్మాణం option options[] = {
		OPT_CMDMODE('l', "list", &cmdmode, N_("list replace refs"), బహుళకం_LIST),
		OPT_CMDMODE('d', "delete", &cmdmode, N_("delete replace refs"), బహుళకం_DELETE),
		OPT_CMDMODE('e', "edit", &cmdmode, N_("edit existing object"), బహుళకం_EDIT),
		OPT_CMDMODE('g', "graft", &cmdmode, N_("change a commit's parents"), బహుళకం_GRAFT),
		OPT_CMDMODE(0, "convert-graft-file", &cmdmode, N_("convert existing graft file"), బహుళకం_CONVERT_GRAFT_FILE),
		OPT_BOOL_F('f', "force", &force, N_("replace the ref if it exists"),
			   PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "raw", &raw, N_("do not pretty-print contents for --edit")),
		OPT_STRING(0, "format", &format, N_("format"), N_("use this format")),
		OPT_చివర()
	};

	disable_replace_refs();
	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_replace_usage, 0);

	ఒకవేళ (!cmdmode)
		cmdmode = argc ? బహుళకం_REPLACE : బహుళకం_LIST;

	ఒకవేళ (format && cmdmode != బహుళకం_LIST)
		usage_సందేశం_opt(_("--format cannot be used when not listing"),
			      git_replace_usage, options);

	ఒకవేళ (force &&
	    cmdmode != బహుళకం_REPLACE &&
	    cmdmode != బహుళకం_EDIT &&
	    cmdmode != బహుళకం_GRAFT &&
	    cmdmode != బహుళకం_CONVERT_GRAFT_FILE)
		usage_సందేశం_opt(_("-f only makes sense when writing a replacement"),
			      git_replace_usage, options);

	ఒకవేళ (raw && cmdmode != బహుళకం_EDIT)
		usage_సందేశం_opt(_("--raw only makes sense with --edit"),
			      git_replace_usage, options);

	ఎంపిక (cmdmode) {
	స్థితి బహుళకం_DELETE:
		ఒకవేళ (argc < 1)
			usage_సందేశం_opt(_("-d needs at least one argument"),
				      git_replace_usage, options);
		తిరిగి for_each_replace_name(argv, delete_replace_ref);

	స్థితి బహుళకం_REPLACE:
		ఒకవేళ (argc != 2)
			usage_సందేశం_opt(_("bad number of arguments"),
				      git_replace_usage, options);
		తిరిగి replace_object(argv[0], argv[1], force);

	స్థితి బహుళకం_EDIT:
		ఒకవేళ (argc != 1)
			usage_సందేశం_opt(_("-e needs exactly one argument"),
				      git_replace_usage, options);
		తిరిగి edit_and_replace(argv[0], force, raw);

	స్థితి బహుళకం_GRAFT:
		ఒకవేళ (argc < 1)
			usage_సందేశం_opt(_("-g needs at least one argument"),
				      git_replace_usage, options);
		తిరిగి create_graft(argc, argv, force, 0);

	స్థితి బహుళకం_CONVERT_GRAFT_FILE:
		ఒకవేళ (argc != 0)
			usage_సందేశం_opt(_("--convert-graft-file takes no argument"),
				      git_replace_usage, options);
		తిరిగి !!convert_graft_file(force);

	స్థితి బహుళకం_LIST:
		ఒకవేళ (argc > 1)
			usage_సందేశం_opt(_("only one pattern can be given with -l"),
				      git_replace_usage, options);
		తిరిగి list_replace_refs(argv[0], format);

	అప్రమేయ:
		BUG("invalid cmdmode %d", (పూర్ణాంకం)cmdmode);
	}
}
