/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "editor.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "notes.h"
#అన్తర్భావ "object-file.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "path.h"

#అన్తర్భావ "pretty.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "exec-cmd.h"
#అన్తర్భావ "run-command.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "notes-merge.h"
#అన్తర్భావ "notes-utils.h"
#అన్తర్భావ "worktree.h"
#అన్తర్భావ "write-or-die.h"

స్థిర స్థిరరాశి అక్షరం *separator = "\n";
స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_usage[] = {
	N_("git notes [--ref <notes-ref>] [list [<object>]]"),
	N_("git notes [--ref <notes-ref>] add [-f] [--allow-empty] [--[no-]separator|--separator=<paragraph-break>] [--[no-]stripspace] [-m <msg> | -F <file> | (-c | -C) <object>] [<object>] [-e]"),
	N_("git notes [--ref <notes-ref>] copy [-f] <from-object> <to-object>"),
	N_("git notes [--ref <notes-ref>] append [--allow-empty] [--[no-]separator|--separator=<paragraph-break>] [--[no-]stripspace] [-m <msg> | -F <file> | (-c | -C) <object>] [<object>] [-e]"),
	N_("git notes [--ref <notes-ref>] edit [--allow-empty] [<object>]"),
	N_("git notes [--ref <notes-ref>] show [<object>]"),
	N_("git notes [--ref <notes-ref>] merge [-v | -q] [-s <strategy>] <notes-ref>"),
	"git notes merge --commit [-v | -q]",
	"git notes merge --abort [-v | -q]",
	N_("git notes [--ref <notes-ref>] remove [<object>...]"),
	N_("git notes [--ref <notes-ref>] prune [-n] [-v]"),
	N_("git notes [--ref <notes-ref>] get-ref"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_కూడు_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_విలీనం_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ఖాళీ
};

స్థిర స్థిరరాశి అక్షరం note_template[] =
	N_("Write/edit the notes for the following object:");

గణన notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

నిర్మాణం note_సందేశం {
	గణన notes_stripspace stripspace;
	నిర్మాణం strbuf buf;
};

నిర్మాణం note_దత్తాంశం {
	పూర్ణాంకం use_editor;
	పూర్ణాంకం stripspace;
	అక్షరం *edit_మార్గం;
	నిర్మాణం strbuf buf;
	నిర్మాణం note_సందేశం **messages;
	కొలత_t సందేశం_nr;
	కొలత_t సందేశం_alloc;
};

స్థిర శూన్య free_note_దత్తాంశం(నిర్మాణం note_దత్తాంశం *d)
{
	ఒకవేళ (d->edit_మార్గం) {
		unlink_or_warn(d->edit_మార్గం);
		విడుదల(d->edit_మార్గం);
	}
	strbuf_release(&d->buf);

	అయితే (d->సందేశం_nr--) {
		strbuf_release(&d->messages[d->సందేశం_nr]->buf);
		విడుదల(d->messages[d->సందేశం_nr]);
	}
	విడుదల(d->messages);
}

స్థిర పూర్ణాంకం list_each_note(స్థిరరాశి నిర్మాణం object_id *object_oid,
			  స్థిరరాశి నిర్మాణం object_id *note_oid,
			  అక్షరం *note_మార్గం వాడని,
			  శూన్య *cb_దత్తాంశం వాడని)
{
	రాయి("%s %s\n", oid_గమ్యం_hex(note_oid), oid_గమ్యం_hex(object_oid));
	తిరిగి 0;
}

స్థిర శూన్య copy_obj_గమ్యం_fd(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం object_id *oid)
{
	కొలత_t కొలత;
	గణన object_type type;
	అక్షరం *buf = odb_read_object(the_repository->objects, oid, &type, &కొలత);
	ఒకవేళ (buf) {
		ఒకవేళ (కొలత)
			write_or_die(fd, buf, కొలత);
		విడుదల(buf);
	}
}

స్థిర శూన్య write_commented_object(పూర్ణాంకం fd, స్థిరరాశి నిర్మాణం object_id *object)
{
	నిర్మాణం బిడ్డ_process show = బిడ్డ_PROCESS_INIT;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.వాదనలు, "show", "--stat", "--no-notes",
		     oid_గమ్యం_hex(object), ఖాళీ);
	show.no_stdin = 1;
	show.బహిర్గతం = -1;
	show.err = 0;
	show.git_cmd = 1;
	ఒకవేళ (ప్రారంభం_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_గమ్యం_hex(object));

	ఒకవేళ (strbuf_read(&buf, show.బహిర్గతం, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_కూడు_commented_పంక్తులు(&cbuf, buf.buf, buf.len, comment_పంక్తి_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	ఒకవేళ (ముగింపు_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_గమ్యం_hex(object));
}

స్థిర శూన్య prepare_note_దత్తాంశం(స్థిరరాశి నిర్మాణం object_id *object, నిర్మాణం note_దత్తాంశం *d,
		స్థిరరాశి నిర్మాణం object_id *old_note)
{
	ఒకవేళ (d->use_editor || !d->సందేశం_nr) {
		పూర్ణాంకం fd;
		నిర్మాణం strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_మార్గం = repo_git_మార్గం(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_మార్గం, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ఒకవేళ (d->సందేశం_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		లేకపోతే ఒకవేళ (old_note)
			copy_obj_గమ్యం_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_కూడు_commented_పంక్తులు(&buf, "\n", అక్షరశ్రేణి_పొడవు("\n"), comment_పంక్తి_str);
		strbuf_కూడు_commented_పంక్తులు(&buf, _(note_template), అక్షరశ్రేణి_పొడవు(_(note_template)),
					   comment_పంక్తి_str);
		strbuf_కూడు_commented_పంక్తులు(&buf, "\n", అక్షరశ్రేణి_పొడవు("\n"), comment_పంక్తి_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		ఒకవేళ (launch_editor(d->edit_మార్గం, &d->buf, ఖాళీ)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		ఒకవేళ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_పంక్తి_str);
	}
}

స్థిర శూన్య write_note_దత్తాంశం(నిర్మాణం note_దత్తాంశం *d, నిర్మాణం object_id *oid)
{
	ఒకవేళ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		పూర్ణాంకం స్థితి_2 = die_సందేశం(_("unable to write note object"));

		ఒకవేళ (d->edit_మార్గం)
			die_సందేశం(_("the note contents have been left in %s"),
				    d->edit_మార్గం);
		నిష్క్రమించు(స్థితి_2);
	}
}

స్థిర శూన్య append_separator(నిర్మాణం strbuf *సందేశం_2)
{
	కొలత_t sep_len = 0;

	ఒకవేళ (!separator)
		తిరిగి;
	లేకపోతే ఒకవేళ ((sep_len = అక్షరశ్రేణి_పొడవు(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(సందేశం_2, separator);
	లేకపోతే
		strbuf_addf(సందేశం_2, "%s%s", separator, "\n");
}

స్థిర శూన్య concat_messages(నిర్మాణం note_దత్తాంశం *d)
{
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	కొలత_t i;

	కొరకు (i = 0; i < d->సందేశం_nr ; i++) {
		ఒకవేళ (d->buf.len)
			append_separator(&d->buf);
		strbuf_కూడు(&సందేశం, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &సందేశం);
		ఒకవేళ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ఖాళీ);
		strbuf_reset(&సందేశం);
	}
	strbuf_release(&సందేశం);
}

స్థిర పూర్ణాంకం parse_సందేశం_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం note_దత్తాంశం *d = opt->విలువ;
	నిర్మాణం note_సందేశం *సందేశం = xmalloc(పరిమాణం(*సందేశం));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&సందేశం->buf, అక్షరశ్రేణి_పొడవు(arg));
	strbuf_addstr(&సందేశం->buf, arg);
	ALLOC_GROW_BY(d->messages, d->సందేశం_nr, 1, d->సందేశం_alloc);
	d->messages[d->సందేశం_nr - 1] = సందేశం;
	సందేశం->stripspace = STRIPSPACE;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_file_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం note_దత్తాంశం *d = opt->విలువ;
	నిర్మాణం note_సందేశం *సందేశం = xmalloc(పరిమాణం(*సందేశం));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&సందేశం->buf , 0);
	ఒకవేళ (!strcmp(arg, "-")) {
		ఒకవేళ (strbuf_read(&సందేశం->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} లేకపోతే ఒకవేళ (strbuf_read_file(&సందేశం->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->సందేశం_nr, 1, d->సందేశం_alloc);
	d->messages[d->సందేశం_nr - 1] = సందేశం;
	సందేశం->stripspace = STRIPSPACE;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_reuse_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం note_దత్తాంశం *d = opt->విలువ;
	నిర్మాణం note_సందేశం *సందేశం = xmalloc(పరిమాణం(*సందేశం));
	అక్షరం *విలువ;
	నిర్మాణం object_id object;
	గణన object_type type;
	కొలత_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&సందేశం->buf, 0);
	ఒకవేళ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	ఒకవేళ (!(విలువ = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	ఒకవేళ (type != OBJ_BLOB) {
		strbuf_release(&సందేశం->buf);
		విడుదల(విలువ);
		విడుదల(సందేశం);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_కూడు(&సందేశం->buf, విలువ, len);
	విడుదల(విలువ);

	సందేశం->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->సందేశం_nr, 1, d->సందేశం_alloc);
	d->messages[d->సందేశం_nr - 1] = సందేశం;
	సందేశం->stripspace = NO_STRIPSPACE;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_reedit_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం note_దత్తాంశం *d = opt->విలువ;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	తిరిగి parse_reuse_arg(opt, arg, unset);
}

స్థిర పూర్ణాంకం parse_separator_arg(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			       పూర్ణాంకం unset)
{
	ఒకవేళ (unset)
		*(స్థిరరాశి అక్షరం **)opt->విలువ = ఖాళీ;
	లేకపోతే
		*(స్థిరరాశి అక్షరం **)opt->విలువ = arg ? arg : "\n";
	తిరిగి 0;
}

స్థిర పూర్ణాంకం notes_copy_from_stdin(పూర్ణాంకం force, స్థిరరాశి అక్షరం *rewrite_cmd)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం notes_rewrite_cfg *c = ఖాళీ;
	నిర్మాణం notes_వృక్షం *t = ఖాళీ;
	పూర్ణాంకం ret = 0;
	స్థిరరాశి అక్షరం *సందేశం = "Notes added by 'git notes copy'";

	ఒకవేళ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		ఒకవేళ (!c)
			తిరిగి 0;
	} లేకపోతే {
		init_notes(ఖాళీ, ఖాళీ, ఖాళీ, NOTES_INIT_WRITABLE);
		t = &default_notes_వృక్షం;
	}

	అయితే (strbuf_getline_lf(&buf, stdin) != EOF) {
		నిర్మాణం object_id from_obj, గమ్యం_obj;
		నిర్మాణం string_list విభజించు = STRING_LIST_INIT_NODUP;
		పూర్ణాంకం err;

		string_list_విభజించు_లోపల_ఉంచు_f(&విభజించు, buf.buf, " ", -1,
					     STRING_LIST_విభజించు_TRIM);
		ఒకవేళ (విభజించు.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		ఒకవేళ (repo_get_oid(the_repository, విభజించు.వస్తువులు[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    విభజించు.వస్తువులు[0].string);
		ఒకవేళ (repo_get_oid(the_repository, విభజించు.వస్తువులు[1].string, &గమ్యం_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    విభజించు.వస్తువులు[1].string);

		ఒకవేళ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &గమ్యం_obj);
		లేకపోతే
			err = copy_note(t, &from_obj, &గమ్యం_obj, force,
					combine_notes_overwrite);

		ఒకవేళ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      విభజించు.వస్తువులు[0].string, విభజించు.వస్తువులు[1].string);
			ret = 1;
		}

		string_list_clear(&విభజించు, 0);
	}

	ఒకవేళ (!rewrite_cmd) {
		commit_notes(the_repository, t, సందేశం);
		free_notes(t);
	} లేకపోతే {
		ముగింపు_copy_notes_for_rewrite(the_repository, c, సందేశం);
	}
	strbuf_release(&buf);
	తిరిగి ret;
}

స్థిర నిర్మాణం notes_వృక్షం *init_notes_check(స్థిరరాశి అక్షరం *subcommand,
					   పూర్ణాంకం flags)
{
	నిర్మాణం notes_వృక్షం *t;
	స్థిరరాశి అక్షరం *ref;
	init_notes(ఖాళీ, ఖాళీ, ఖాళీ, flags);
	t = &default_notes_వృక్షం;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	ఒకవేళ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	తిరిగి t;
}

స్థిర పూర్ణాంకం list(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం notes_వృక్షం *t;
	నిర్మాణం object_id object;
	స్థిరరాశి నిర్మాణం object_id *note;
	పూర్ణాంకం retval = -1;
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	ఒకవేళ (argc)
		argc = parse_options(argc, argv, ఉపసర్గ, options,
				     git_notes_list_usage, 0);

	ఒకవేళ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	ఒకవేళ (argc) {
		ఒకవేళ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		ఒకవేళ (note) {
			puts(oid_గమ్యం_hex(note));
			retval = 0;
		} లేకపోతే
			retval = error(_("no note found for object %s."),
				       oid_గమ్యం_hex(&object));
	} లేకపోతే
		retval = for_each_note(t, 0, list_each_note, ఖాళీ);

	free_notes(t);
	తిరిగి retval;
}

స్థిర పూర్ణాంకం append_edit(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని);

స్థిర పూర్ణాంకం కూడు(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
	       నిర్మాణం repository *repo)
{
	పూర్ణాంకం force = 0, allow_ఖాళీలు = 0;
	స్థిరరాశి అక్షరం *object_ref;
	నిర్మాణం notes_వృక్షం *t;
	నిర్మాణం object_id object, new_note;
	స్థిరరాశి నిర్మాణం object_id *note;
	నిర్మాణం note_దత్తాంశం d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	నిర్మాణం option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_సందేశం_arg),
		OPT_CALLBACK_F('F', "file", &d, N_("file"),
			N_("note contents in a file"), PARSE_OPT_NONEG,
			parse_file_arg),
		OPT_CALLBACK_F('c', "reedit-message", &d, N_("object"),
			N_("reuse and edit specified note object"), PARSE_OPT_NONEG,
			parse_reedit_arg),
		OPT_BOOL('e', "edit", &d.use_editor,
			N_("edit note message in editor")),
		OPT_CALLBACK_F('C', "reuse-message", &d, N_("object"),
			N_("reuse specified note object"), PARSE_OPT_NONEG,
			parse_reuse_arg),
		OPT_BOOL(0, "allow-empty", &allow_ఖాళీలు,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_notes_కూడు_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ఒకవేళ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_కూడు_usage, options);
	}

	ఒకవేళ (d.సందేశం_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ఒకవేళ (note) {
		ఒకవేళ (!force) {
			free_notes(t);
			ఒకవేళ (d.సందేశం_nr) {
				free_note_దత్తాంశం(&d);
				తిరిగి error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_గమ్యం_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			తిరిగి append_edit(argc, argv, ఉపసర్గ, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_గమ్యం_hex(&object));
	}

	prepare_note_దత్తాంశం(&object, &d, note);
	ఒకవేళ (d.buf.len || allow_ఖాళీలు) {
		write_note_దత్తాంశం(&d, &new_note);
		ఒకవేళ (కూడు_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} లేకపోతే {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_గమ్యం_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_దత్తాంశం(&d);
	free_notes(t);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం copy(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం retval = 0, force = 0, from_stdin = 0;
	స్థిరరాశి నిర్మాణం object_id *from_note, *note;
	స్థిరరాశి అక్షరం *object_ref;
	నిర్మాణం object_id object, from_obj;
	నిర్మాణం notes_వృక్షం *t;
	స్థిరరాశి అక్షరం *rewrite_cmd = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_notes_copy_usage,
			     0);

	ఒకవేళ (from_stdin || rewrite_cmd) {
		ఒకవేళ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} లేకపోతే {
			తిరిగి notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	ఒకవేళ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	ఒకవేళ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	ఒకవేళ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ఒకవేళ (note) {
		ఒకవేళ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_గమ్యం_hex(&object));
			వెళ్ళు బహిర్గతం;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_గమ్యం_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	ఒకవేళ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_గమ్యం_hex(&from_obj));
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (కూడు_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
బహిర్గతం:
	free_notes(t);
	తిరిగి retval;
}

స్థిర పూర్ణాంకం append_edit(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం allow_ఖాళీలు = 0;
	స్థిరరాశి అక్షరం *object_ref;
	నిర్మాణం notes_వృక్షం *t;
	నిర్మాణం object_id object, new_note;
	స్థిరరాశి నిర్మాణం object_id *note;
	అక్షరం *logmsg;
	స్థిరరాశి అక్షరం * స్థిరరాశి *usage;
	నిర్మాణం note_దత్తాంశం d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	నిర్మాణం option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_సందేశం_arg),
		OPT_CALLBACK_F('F', "file", &d, N_("file"),
			N_("note contents in a file"), PARSE_OPT_NONEG,
			parse_file_arg),
		OPT_CALLBACK_F('c', "reedit-message", &d, N_("object"),
			N_("reuse and edit specified note object"), PARSE_OPT_NONEG,
			parse_reedit_arg),
		OPT_CALLBACK_F('C', "reuse-message", &d, N_("object"),
			N_("reuse specified note object"), PARSE_OPT_NONEG,
			parse_reuse_arg),
		OPT_BOOL('e', "edit", &d.use_editor,
			N_("edit note message in editor")),
		OPT_BOOL(0, "allow-empty", &allow_ఖాళీలు,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_చివర()
	};
	పూర్ణాంకం edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, ఉపసర్గ, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	ఒకవేళ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	ఒకవేళ (d.సందేశం_nr) {
		concat_messages(&d);
		ఒకవేళ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_దత్తాంశం(&object, &d, edit && note ? note : ఖాళీ);

	ఒకవేళ (note && !edit) {
		/* Append buf to previous note contents */
		కొలత_t కొలత;
		గణన object_type type;
		నిర్మాణం strbuf buf = STRBUF_INIT;
		అక్షరం *prev_buf = odb_read_object(the_repository->objects, note, &type, &కొలత);

		ఒకవేళ (!prev_buf)
			die(_("unable to read %s"), oid_గమ్యం_hex(note));
		ఒకవేళ (కొలత)
			strbuf_కూడు(&buf, prev_buf, కొలత);
		ఒకవేళ (d.buf.len && కొలత)
			append_separator(&buf);
		strbuf_చొప్పించు(&d.buf, 0, buf.buf, buf.len);

		విడుదల(prev_buf);
		strbuf_release(&buf);
	}

	ఒకవేళ (d.buf.len || allow_ఖాళీలు) {
		write_note_దత్తాంశం(&d, &new_note);
		ఒకవేళ (కూడు_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} లేకపోతే {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_గమ్యం_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	విడుదల(logmsg);
	free_note_దత్తాంశం(&d);
	free_notes(t);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	స్థిరరాశి అక్షరం *object_ref;
	నిర్మాణం notes_వృక్షం *t;
	నిర్మాణం object_id object;
	స్థిరరాశి నిర్మాణం object_id *note;
	పూర్ణాంకం retval;
	నిర్మాణం option options[] = {
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_notes_show_usage,
			     0);

	ఒకవేళ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	ఒకవేళ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	ఒకవేళ (!note)
		retval = error(_("no note found for object %s."),
			       oid_గమ్యం_hex(&object));
	లేకపోతే {
		స్థిరరాశి అక్షరం *show_వాదనలు[3] = {"show", oid_గమ్యం_hex(note), ఖాళీ};
		retval = execv_git_cmd(show_వాదనలు);
	}
	free_notes(t);
	తిరిగి retval;
}

స్థిర పూర్ణాంకం విలీనం_abort(నిర్మాణం notes_విలీనం_options *o)
{
	పూర్ణాంకం ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	ఒకవేళ (refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ, "NOTES_MERGE_PARTIAL", ఖాళీ, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	ఒకవేళ (refs_delete_ref(get_main_ref_store(the_repository), ఖాళీ, "NOTES_MERGE_REF", ఖాళీ, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	ఒకవేళ (notes_విలీనం_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	తిరిగి ret;
}

స్థిర పూర్ణాంకం విలీనం_commit(నిర్మాణం notes_విలీనం_options *o)
{
	నిర్మాణం strbuf సందేశం = STRBUF_INIT;
	నిర్మాణం object_id oid, మాతృ_oid;
	నిర్మాణం notes_వృక్షం t = {0};
	నిర్మాణం commit *partial;
	నిర్మాణం pretty_print_context pretty_ctx;
	శూన్య *local_ref_గమ్యం_free;
	పూర్ణాంకం ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	ఒకవేళ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	లేకపోతే ఒకవేళ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	లేకపోతే ఒకవేళ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	ఒకవేళ (partial->parents)
		oidcpy(&మాతృ_oid, &partial->parents->అంశం->object.oid);
	లేకపోతే
		oidclr(&మాతృ_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_గమ్యం_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ఖాళీ);
	ఒకవేళ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	ఒకవేళ (notes_విలీనం_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, పరిమాణం(pretty_ctx));
	repo_format_commit_సందేశం(the_repository, partial, "%s", &సందేశం,
				   &pretty_ctx);
	strbuf_trim(&సందేశం);
	strbuf_insertstr(&సందేశం, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), సందేశం.buf,
			o->local_ref, &oid,
			is_null_oid(&మాతృ_oid) ? ఖాళీ : &మాతృ_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&సందేశం);
	ret = విలీనం_abort(o);
	విడుదల(local_ref_గమ్యం_free);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం git_config_get_notes_strategy(స్థిరరాశి అక్షరం *కీలకం,
					 గణన notes_విలీనం_strategy *strategy)
{
	అక్షరం *విలువ;

	ఒకవేళ (repo_config_get_string(the_repository, కీలకం, &విలువ))
		తిరిగి 1;
	ఒకవేళ (parse_notes_విలీనం_strategy(విలువ, strategy))
		git_die_config(the_repository, కీలకం, _("unknown notes merge strategy %s"), విలువ);

	విడుదల(విలువ);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం విలీనం(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం strbuf remote_ref = STRBUF_INIT, సందేశం = STRBUF_INIT;
	నిర్మాణం object_id ఫలితం_oid;
	నిర్మాణం notes_వృక్షం *t;
	నిర్మాణం notes_విలీనం_options o;
	పూర్ణాంకం do_విలీనం = 0, do_commit = 0, do_abort = 0;
	పూర్ణాంకం verbosity = 0, ఫలితం;
	స్థిరరాశి అక్షరం *strategy = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_సమూహం(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_సమూహం(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_సమూహం(N_("Committing unmerged notes")),
		OPT_సమితి_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_సమూహం(N_("Aborting notes merge resolution")),
		OPT_సమితి_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_చివర()
	};
	అక్షరం *notes_ref;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_notes_విలీనం_usage, 0);

	ఒకవేళ (strategy || do_commit + do_abort == 0)
		do_విలీనం = 1;
	ఒకవేళ (do_విలీనం + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_విలీనం_usage, options);
	}

	ఒకవేళ (do_విలీనం && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_విలీనం_usage, options);
	} లేకపోతే ఒకవేళ (!do_విలీనం && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_విలీనం_usage, options);
	}

	init_notes_విలీనం_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_విలీనం_VERBOSITY_DEFAULT;

	ఒకవేళ (do_abort)
		తిరిగి విలీనం_abort(&o);
	ఒకవేళ (do_commit)
		తిరిగి విలీనం_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	ఒకవేళ (strategy) {
		ఒకవేళ (parse_notes_విలీనం_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_విలీనం_usage, options);
		}
	} లేకపోతే {
		నిర్మాణం strbuf విలీనం_కీలకం = STRBUF_INIT;
		స్థిరరాశి అక్షరం *short_ref = ఖాళీ;

		ఒకవేళ (!skip_ఉపసర్గ(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&విలీనం_కీలకం, "notes.%s.mergeStrategy", short_ref);

		ఒకవేళ (git_config_get_notes_strategy(విలీనం_కీలకం.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&విలీనం_కీలకం);
	}

	strbuf_addf(&సందేశం, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_కూడు(&(o.commit_సందేశం), సందేశం.buf + 7, సందేశం.len - 7); /* skip "notes: " */

	ఫలితం = notes_విలీనం(&o, t, &ఫలితం_oid);

	ఒకవేళ (ఫలితం >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), సందేశం.buf,
				notes_ref, &ఫలితం_oid, ఖాళీ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	లేకపోతే { /* Merge has unresolved conflicts */
		నిర్మాణం worktree **worktrees;
		స్థిరరాశి నిర్మాణం worktree *wt;
		అక్షరం *మార్గం;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), సందేశం.buf,
				"NOTES_MERGE_PARTIAL", &ఫలితం_oid, ఖాళీ,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = కనుగొను_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		ఒకవేళ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->మార్గం);
		free_worktrees(worktrees);
		ఒకవేళ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ఖాళీ))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		మార్గం = repo_git_మార్గం(the_repository, NOTES_విలీనం_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			మార్గం);
		విడుదల(మార్గం);
	}

	free_notes(t);
	విడుదల(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&సందేశం);
	తిరిగి ఫలితం < 0; /* return non-zero on conflicts */
}

#నిర్వచించు IGNORE_MISSING 1

స్థిర పూర్ణాంకం remove_one_note(నిర్మాణం notes_వృక్షం *t, స్థిరరాశి అక్షరం *name, గుర్తులేని flag)
{
	పూర్ణాంకం స్థితి_2;
	నిర్మాణం object_id oid;
	ఒకవేళ (repo_get_oid(the_repository, name, &oid))
		తిరిగి error(_("Failed to resolve '%s' as a valid ref."), name);
	స్థితి_2 = remove_note(t, oid.hash);
	ఒకవేళ (స్థితి_2)
		fprintf(stderr, _("Object %s has no note\n"), name);
	లేకపోతే
		fprintf(stderr, _("Removing note for object %s\n"), name);
	తిరిగి (flag & IGNORE_MISSING) ? 0 : స్థితి_2;
}

స్థిర పూర్ణాంకం remove_cmd(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		      నిర్మాణం repository *repo వాడని)
{
	గుర్తులేని flag = 0;
	పూర్ణాంకం from_stdin = 0;
	నిర్మాణం option options[] = {
		OPT_బిట్టు(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_చివర()
	};
	నిర్మాణం notes_వృక్షం *t;
	పూర్ణాంకం retval = 0;

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	ఒకవేళ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} లేకపోతే {
		అయితే (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	ఒకవేళ (from_stdin) {
		నిర్మాణం strbuf sb = STRBUF_INIT;
		అయితే (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	ఒకవేళ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	తిరిగి retval;
}

స్థిర పూర్ణాంకం prune(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		 నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం notes_వృక్షం *t;
	పూర్ణాంకం show_only = 0, verbose = 0;
	నిర్మాణం option options[] = {
		OPT_DRY_నిరంతరం(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_చివర()
	};

	argc = parse_options(argc, argv, ఉపసర్గ, options, git_notes_prune_usage,
			     0);

	ఒకవేళ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	ఒకవేళ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం get_ref(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ,
		   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం option options[] = { OPT_చివర() };
	అక్షరం *notes_ref;
	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_notes_get_ref_usage, 0);

	ఒకవేళ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	విడుదల(notes_ref);
	తిరిగి 0;
}

పూర్ణాంకం cmd_notes(పూర్ణాంకం argc,
	      స్థిరరాశి అక్షరం **argv,
	      స్థిరరాశి అక్షరం *ఉపసర్గ,
	      నిర్మాణం repository *repo)
{
	స్థిరరాశి అక్షరం *override_notes_ref = ఖాళీ;
	parse_opt_subcommand_fn *fn = ఖాళీ;
	నిర్మాణం option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, కూడు),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, విలీనం),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_చివర()
	};

	repo_config(the_repository, git_default_config, ఖాళీ);
	argc = parse_options(argc, argv, ఉపసర్గ, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	ఒకవేళ (!fn) {
		ఒకవేళ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	ఒకవేళ (override_notes_ref) {
		నిర్మాణం strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	తిరిగి !!fn(argc, argv, ఉపసర్గ, repo);
}
