/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#परिभाषय USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "editor.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "hex.h"
#अन्तर्भाव "notes.h"
#अन्तर्भाव "object-file.h"
#अन्तर्भाव "object-name.h"
#अन्तर्भाव "odb.h"
#अन्तर्भाव "path.h"

#अन्तर्भाव "pretty.h"
#अन्तर्भाव "refs.h"
#अन्तर्भाव "exec-cmd.h"
#अन्तर्भाव "run-command.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "notes-merge.h"
#अन्तर्भाव "notes-utils.h"
#अन्तर्भाव "worktree.h"
#अन्तर्भाव "write-or-die.h"

स्थिर ध्रुव वर्ण *separator = "\n";
स्थिर ध्रुव वर्ण * ध्रुव git_notes_usage[] = {
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
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_सङ्कलय_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	रिक्त
};

स्थिर ध्रुव वर्ण * ध्रुव git_notes_get_ref_usage[] = {
	"git notes get-ref",
	रिक्त
};

स्थिर ध्रुव वर्ण note_template[] =
	N_("Write/edit the notes for the following object:");

गणना notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

संरचना note_सन्देशः {
	गणना notes_stripspace stripspace;
	संरचना strbuf buf;
};

संरचना note_दत्त {
	पूर्णाङ्क use_editor;
	पूर्णाङ्क stripspace;
	वर्ण *edit_मार्गः;
	संरचना strbuf buf;
	संरचना note_सन्देशः **messages;
	आकार_t सन्देशः_nr;
	आकार_t सन्देशः_alloc;
};

स्थिर शून्य free_note_दत्त(संरचना note_दत्त *d)
{
	यदि (d->edit_मार्गः) {
		unlink_or_warn(d->edit_मार्गः);
		मुक्त(d->edit_मार्गः);
	}
	strbuf_release(&d->buf);

	यावत् (d->सन्देशः_nr--) {
		strbuf_release(&d->messages[d->सन्देशः_nr]->buf);
		मुक्त(d->messages[d->सन्देशः_nr]);
	}
	मुक्त(d->messages);
}

स्थिर पूर्णाङ्क list_each_note(ध्रुव संरचना object_id *object_oid,
			  ध्रुव संरचना object_id *note_oid,
			  वर्ण *note_मार्गः अप्रयुक्तम्,
			  शून्य *cb_दत्त अप्रयुक्तम्)
{
	प्रारूप_लिख("%s %s\n", oid_गन्तव्यम्_hex(note_oid), oid_गन्तव्यम्_hex(object_oid));
	प्रत्यागम 0;
}

स्थिर शून्य copy_obj_गन्तव्यम्_fd(पूर्णाङ्क fd, ध्रुव संरचना object_id *oid)
{
	आकार_t आकार;
	गणना object_type type;
	वर्ण *buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	यदि (buf) {
		यदि (आकार)
			write_or_die(fd, buf, आकार);
		मुक्त(buf);
	}
}

स्थिर शून्य write_commented_object(पूर्णाङ्क fd, ध्रुव संरचना object_id *object)
{
	संरचना child_process show = CHILD_PROCESS_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.तर्काः, "show", "--stat", "--no-notes",
		     oid_गन्तव्यम्_hex(object), रिक्त);
	show.no_stdin = 1;
	show.बहिर्गमनम् = -1;
	show.err = 0;
	show.git_cmd = 1;
	यदि (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_गन्तव्यम्_hex(object));

	यदि (strbuf_read(&buf, show.बहिर्गमनम्, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_सङ्कलय_commented_पङ्क्तयः(&cbuf, buf.buf, buf.len, comment_पङ्क्तिः_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	यदि (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_गन्तव्यम्_hex(object));
}

स्थिर शून्य prepare_note_दत्त(ध्रुव संरचना object_id *object, संरचना note_दत्त *d,
		ध्रुव संरचना object_id *old_note)
{
	यदि (d->use_editor || !d->सन्देशः_nr) {
		पूर्णाङ्क fd;
		संरचना strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_मार्गः = repo_git_मार्गः(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_मार्गः, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		यदि (d->सन्देशः_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		अन्यथा यदि (old_note)
			copy_obj_गन्तव्यम्_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_सङ्कलय_commented_पङ्क्तयः(&buf, "\n", वाक्यदैर्घ्यम्("\n"), comment_पङ्क्तिः_str);
		strbuf_सङ्कलय_commented_पङ्क्तयः(&buf, _(note_template), वाक्यदैर्घ्यम्(_(note_template)),
					   comment_पङ्क्तिः_str);
		strbuf_सङ्कलय_commented_पङ्क्तयः(&buf, "\n", वाक्यदैर्घ्यम्("\n"), comment_पङ्क्तिः_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		यदि (launch_editor(d->edit_मार्गः, &d->buf, रिक्त)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		यदि (d->stripspace)
			strbuf_stripspace(&d->buf, comment_पङ्क्तिः_str);
	}
}

स्थिर शून्य write_note_दत्त(संरचना note_दत्त *d, संरचना object_id *oid)
{
	यदि (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		पूर्णाङ्क स्थितिः = die_सन्देशः(_("unable to write note object"));

		यदि (d->edit_मार्गः)
			die_सन्देशः(_("the note contents have been left in %s"),
				    d->edit_मार्गः);
		निर्गच्छ(स्थितिः);
	}
}

स्थिर शून्य append_separator(संरचना strbuf *सन्देशः_2)
{
	आकार_t sep_len = 0;

	यदि (!separator)
		प्रत्यागम;
	अन्यथा यदि ((sep_len = वाक्यदैर्घ्यम्(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(सन्देशः_2, separator);
	अन्यथा
		strbuf_addf(सन्देशः_2, "%s%s", separator, "\n");
}

स्थिर शून्य concat_messages(संरचना note_दत्त *d)
{
	संरचना strbuf सन्देशः = STRBUF_INIT;
	आकार_t i;

	प्रति (i = 0; i < d->सन्देशः_nr ; i++) {
		यदि (d->buf.len)
			append_separator(&d->buf);
		strbuf_सङ्कलय(&सन्देशः, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &सन्देशः);
		यदि ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, रिक्त);
		strbuf_reset(&सन्देशः);
	}
	strbuf_release(&सन्देशः);
}

स्थिर पूर्णाङ्क parse_सन्देशः_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_दत्त *d = opt->मानम्;
	संरचना note_सन्देशः *सन्देशः = xmalloc(परिमाण(*सन्देशः));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&सन्देशः->buf, वाक्यदैर्घ्यम्(arg));
	strbuf_addstr(&सन्देशः->buf, arg);
	ALLOC_GROW_BY(d->messages, d->सन्देशः_nr, 1, d->सन्देशः_alloc);
	d->messages[d->सन्देशः_nr - 1] = सन्देशः;
	सन्देशः->stripspace = STRIPSPACE;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_file_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_दत्त *d = opt->मानम्;
	संरचना note_सन्देशः *सन्देशः = xmalloc(परिमाण(*सन्देशः));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&सन्देशः->buf , 0);
	यदि (!वाक्यं_तुलय(arg, "-")) {
		यदि (strbuf_read(&सन्देशः->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} अन्यथा यदि (strbuf_read_file(&सन्देशः->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->सन्देशः_nr, 1, d->सन्देशः_alloc);
	d->messages[d->सन्देशः_nr - 1] = सन्देशः;
	सन्देशः->stripspace = STRIPSPACE;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_reuse_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_दत्त *d = opt->मानम्;
	संरचना note_सन्देशः *सन्देशः = xmalloc(परिमाण(*सन्देशः));
	वर्ण *मानम्;
	संरचना object_id object;
	गणना object_type type;
	आकार_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&सन्देशः->buf, 0);
	यदि (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	यदि (!(मानम् = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	यदि (type != OBJ_BLOB) {
		strbuf_release(&सन्देशः->buf);
		मुक्त(मानम्);
		मुक्त(सन्देशः);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_सङ्कलय(&सन्देशः->buf, मानम्, len);
	मुक्त(मानम्);

	सन्देशः->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->सन्देशः_nr, 1, d->सन्देशः_alloc);
	d->messages[d->सन्देशः_nr - 1] = सन्देशः;
	सन्देशः->stripspace = NO_STRIPSPACE;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_reedit_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_दत्त *d = opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	प्रत्यागम parse_reuse_arg(opt, arg, unset);
}

स्थिर पूर्णाङ्क parse_separator_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			       पूर्णाङ्क unset)
{
	यदि (unset)
		*(ध्रुव वर्ण **)opt->मानम् = रिक्त;
	अन्यथा
		*(ध्रुव वर्ण **)opt->मानम् = arg ? arg : "\n";
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क notes_copy_from_stdin(पूर्णाङ्क force, ध्रुव वर्ण *rewrite_cmd)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना notes_rewrite_cfg *c = रिक्त;
	संरचना notes_वृक्षः *t = रिक्त;
	पूर्णाङ्क ret = 0;
	ध्रुव वर्ण *सन्देशः = "Notes added by 'git notes copy'";

	यदि (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		यदि (!c)
			प्रत्यागम 0;
	} अन्यथा {
		init_notes(रिक्त, रिक्त, रिक्त, NOTES_INIT_WRITABLE);
		t = &default_notes_वृक्षः;
	}

	यावत् (strbuf_getline_lf(&buf, stdin) != EOF) {
		संरचना object_id from_obj, गन्तव्यम्_obj;
		संरचना string_list विभज = STRING_LIST_INIT_NODUP;
		पूर्णाङ्क err;

		string_list_विभज_अन्तर्_place_f(&विभज, buf.buf, " ", -1,
					     STRING_LIST_विभज_TRIM);
		यदि (विभज.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		यदि (repo_get_oid(the_repository, विभज.वस्तूनि[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    विभज.वस्तूनि[0].string);
		यदि (repo_get_oid(the_repository, विभज.वस्तूनि[1].string, &गन्तव्यम्_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    विभज.वस्तूनि[1].string);

		यदि (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &गन्तव्यम्_obj);
		अन्यथा
			err = copy_note(t, &from_obj, &गन्तव्यम्_obj, force,
					combine_notes_overwrite);

		यदि (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      विभज.वस्तूनि[0].string, विभज.वस्तूनि[1].string);
			ret = 1;
		}

		string_list_clear(&विभज, 0);
	}

	यदि (!rewrite_cmd) {
		commit_notes(the_repository, t, सन्देशः);
		free_notes(t);
	} अन्यथा {
		finish_copy_notes_for_rewrite(the_repository, c, सन्देशः);
	}
	strbuf_release(&buf);
	प्रत्यागम ret;
}

स्थिर संरचना notes_वृक्षः *init_notes_check(ध्रुव वर्ण *subcommand,
					   पूर्णाङ्क flags)
{
	संरचना notes_वृक्षः *t;
	ध्रुव वर्ण *ref;
	init_notes(रिक्त, रिक्त, रिक्त, flags);
	t = &default_notes_वृक्षः;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	यदि (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	प्रत्यागम t;
}

स्थिर पूर्णाङ्क list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	संरचना notes_वृक्षः *t;
	संरचना object_id object;
	ध्रुव संरचना object_id *note;
	पूर्णाङ्क retval = -1;
	संरचना option options[] = {
		OPT_अन्तः()
	};

	यदि (argc)
		argc = parse_options(argc, argv, उपसर्गः, options,
				     git_notes_list_usage, 0);

	यदि (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	यदि (argc) {
		यदि (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		यदि (note) {
			पङ्क्तिं_लिख(oid_गन्तव्यम्_hex(note));
			retval = 0;
		} अन्यथा
			retval = error(_("no note found for object %s."),
				       oid_गन्तव्यम्_hex(&object));
	} अन्यथा
		retval = for_each_note(t, 0, list_each_note, रिक्त);

	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क append_edit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्);

स्थिर पूर्णाङ्क सङ्कलय(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
	       संरचना repository *repo)
{
	पूर्णाङ्क force = 0, allow_रिक्तानि = 0;
	ध्रुव वर्ण *object_ref;
	संरचना notes_वृक्षः *t;
	संरचना object_id object, new_note;
	ध्रुव संरचना object_id *note;
	संरचना note_दत्त d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	संरचना option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_सन्देशः_arg),
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
		OPT_BOOL(0, "allow-empty", &allow_रिक्तानि,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, git_notes_सङ्कलय_usage,
			     PARSE_OPT_KEEP_ARGV0);

	यदि (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_सङ्कलय_usage, options);
	}

	यदि (d.सन्देशः_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	यदि (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	यदि (note) {
		यदि (!force) {
			free_notes(t);
			यदि (d.सन्देशः_nr) {
				free_note_दत्त(&d);
				प्रत्यागम error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_गन्तव्यम्_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			प्रत्यागम append_edit(argc, argv, उपसर्गः, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_गन्तव्यम्_hex(&object));
	}

	prepare_note_दत्त(&object, &d, note);
	यदि (d.buf.len || allow_रिक्तानि) {
		write_note_दत्त(&d, &new_note);
		यदि (सङ्कलय_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} अन्यथा {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_गन्तव्यम्_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_दत्त(&d);
	free_notes(t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क copy(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क retval = 0, force = 0, from_stdin = 0;
	ध्रुव संरचना object_id *from_note, *note;
	ध्रुव वर्ण *object_ref;
	संरचना object_id object, from_obj;
	संरचना notes_वृक्षः *t;
	ध्रुव वर्ण *rewrite_cmd = रिक्त;
	संरचना option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, git_notes_copy_usage,
			     0);

	यदि (from_stdin || rewrite_cmd) {
		यदि (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} अन्यथा {
			प्रत्यागम notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	यदि (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	यदि (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	यदि (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	यदि (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	यदि (note) {
		यदि (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_गन्तव्यम्_hex(&object));
			गच्छ बहिर्गमनम्;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_गन्तव्यम्_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	यदि (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_गन्तव्यम्_hex(&from_obj));
		गच्छ बहिर्गमनम्;
	}

	यदि (सङ्कलय_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
बहिर्गमनम्:
	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क append_edit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		       संरचना repository *repo अप्रयुक्तम्)
{
	पूर्णाङ्क allow_रिक्तानि = 0;
	ध्रुव वर्ण *object_ref;
	संरचना notes_वृक्षः *t;
	संरचना object_id object, new_note;
	ध्रुव संरचना object_id *note;
	वर्ण *logmsg;
	ध्रुव वर्ण * ध्रुव *usage;
	संरचना note_दत्त d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	संरचना option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_सन्देशः_arg),
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
		OPT_BOOL(0, "allow-empty", &allow_रिक्तानि,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_अन्तः()
	};
	पूर्णाङ्क edit = !वाक्यं_तुलय(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, उपसर्गः, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	यदि (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	यदि (d.सन्देशः_nr) {
		concat_messages(&d);
		यदि (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	यदि (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_दत्त(&object, &d, edit && note ? note : रिक्त);

	यदि (note && !edit) {
		/* Append buf to previous note contents */
		आकार_t आकार;
		गणना object_type type;
		संरचना strbuf buf = STRBUF_INIT;
		वर्ण *पूर्व_buf = odb_read_object(the_repository->objects, note, &type, &आकार);

		यदि (!पूर्व_buf)
			die(_("unable to read %s"), oid_गन्तव्यम्_hex(note));
		यदि (आकार)
			strbuf_सङ्कलय(&buf, पूर्व_buf, आकार);
		यदि (d.buf.len && आकार)
			append_separator(&buf);
		strbuf_निवेशय(&d.buf, 0, buf.buf, buf.len);

		मुक्त(पूर्व_buf);
		strbuf_release(&buf);
	}

	यदि (d.buf.len || allow_रिक्तानि) {
		write_note_दत्त(&d, &new_note);
		यदि (सङ्कलय_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} अन्यथा {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_गन्तव्यम्_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	मुक्त(logmsg);
	free_note_दत्त(&d);
	free_notes(t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		संरचना repository *repo अप्रयुक्तम्)
{
	ध्रुव वर्ण *object_ref;
	संरचना notes_वृक्षः *t;
	संरचना object_id object;
	ध्रुव संरचना object_id *note;
	पूर्णाङ्क retval;
	संरचना option options[] = {
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, git_notes_show_usage,
			     0);

	यदि (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	यदि (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	यदि (!note)
		retval = error(_("no note found for object %s."),
			       oid_गन्तव्यम्_hex(&object));
	अन्यथा {
		ध्रुव वर्ण *show_तर्काः[3] = {"show", oid_गन्तव्यम्_hex(note), रिक्त};
		retval = execv_git_cmd(show_तर्काः);
	}
	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क merge_abort(संरचना notes_merge_options *o)
{
	पूर्णाङ्क ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	यदि (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, "NOTES_MERGE_PARTIAL", रिक्त, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	यदि (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, "NOTES_MERGE_REF", रिक्त, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	यदि (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क merge_commit(संरचना notes_merge_options *o)
{
	संरचना strbuf सन्देशः = STRBUF_INIT;
	संरचना object_id oid, parent_oid;
	संरचना notes_वृक्षः t = {0};
	संरचना commit *partial;
	संरचना pretty_print_context pretty_ctx;
	शून्य *local_ref_गन्तव्यम्_free;
	पूर्णाङ्क ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	यदि (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	अन्यथा यदि (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	अन्यथा यदि (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	यदि (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	अन्यथा
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_गन्तव्यम्_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, रिक्त);
	यदि (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	यदि (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	स्मृतिं_पूरय(&pretty_ctx, 0, परिमाण(pretty_ctx));
	repo_format_commit_सन्देशः(the_repository, partial, "%s", &सन्देशः,
				   &pretty_ctx);
	strbuf_trim(&सन्देशः);
	strbuf_insertstr(&सन्देशः, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), सन्देशः.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? रिक्त : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&सन्देशः);
	ret = merge_abort(o);
	मुक्त(local_ref_गन्तव्यम्_free);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क git_config_get_notes_strategy(ध्रुव वर्ण *कुञ्चिका,
					 गणना notes_merge_strategy *strategy)
{
	वर्ण *मानम्;

	यदि (repo_config_get_string(the_repository, कुञ्चिका, &मानम्))
		प्रत्यागम 1;
	यदि (parse_notes_merge_strategy(मानम्, strategy))
		git_die_config(the_repository, कुञ्चिका, _("unknown notes merge strategy %s"), मानम्);

	मुक्त(मानम्);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क merge(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना strbuf remote_ref = STRBUF_INIT, सन्देशः = STRBUF_INIT;
	संरचना object_id परिणाम_oid;
	संरचना notes_वृक्षः *t;
	संरचना notes_merge_options o;
	पूर्णाङ्क do_merge = 0, do_commit = 0, do_abort = 0;
	पूर्णाङ्क verbosity = 0, परिणाम;
	ध्रुव वर्ण *strategy = रिक्त;
	संरचना option options[] = {
		OPT_गणः(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_गणः(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_गणः(N_("Committing unmerged notes")),
		OPT_समुच्चयः_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_गणः(N_("Aborting notes merge resolution")),
		OPT_समुच्चयः_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_अन्तः()
	};
	वर्ण *notes_ref;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_notes_merge_usage, 0);

	यदि (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	यदि (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	यदि (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} अन्यथा यदि (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	यदि (do_abort)
		प्रत्यागम merge_abort(&o);
	यदि (do_commit)
		प्रत्यागम merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	यदि (strategy) {
		यदि (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} अन्यथा {
		संरचना strbuf merge_कुञ्चिका = STRBUF_INIT;
		ध्रुव वर्ण *short_ref = रिक्त;

		यदि (!skip_उपसर्गः(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_कुञ्चिका, "notes.%s.mergeStrategy", short_ref);

		यदि (git_config_get_notes_strategy(merge_कुञ्चिका.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_कुञ्चिका);
	}

	strbuf_addf(&सन्देशः, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_सङ्कलय(&(o.commit_सन्देशः), सन्देशः.buf + 7, सन्देशः.len - 7); /* skip "notes: " */

	परिणाम = notes_merge(&o, t, &परिणाम_oid);

	यदि (परिणाम >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः.buf,
				notes_ref, &परिणाम_oid, रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अन्यथा { /* Merge has unresolved conflicts */
		संरचना worktree **worktrees;
		ध्रुव संरचना worktree *wt;
		वर्ण *मार्गः;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः.buf,
				"NOTES_MERGE_PARTIAL", &परिणाम_oid, रिक्त,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		यदि (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->मार्गः);
		free_worktrees(worktrees);
		यदि (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, रिक्त))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		मार्गः = repo_git_मार्गः(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			मार्गः);
		मुक्त(मार्गः);
	}

	free_notes(t);
	मुक्त(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&सन्देशः);
	प्रत्यागम परिणाम < 0; /* return non-zero on conflicts */
}

#परिभाषय IGNORE_MISSING 1

स्थिर पूर्णाङ्क remove_one_note(संरचना notes_वृक्षः *t, ध्रुव वर्ण *name, अचिह्नित flag)
{
	पूर्णाङ्क स्थितिः;
	संरचना object_id oid;
	यदि (repo_get_oid(the_repository, name, &oid))
		प्रत्यागम error(_("Failed to resolve '%s' as a valid ref."), name);
	स्थितिः = remove_note(t, oid.hash);
	यदि (स्थितिः)
		fprintf(stderr, _("Object %s has no note\n"), name);
	अन्यथा
		fprintf(stderr, _("Removing note for object %s\n"), name);
	प्रत्यागम (flag & IGNORE_MISSING) ? 0 : स्थितिः;
}

स्थिर पूर्णाङ्क remove_cmd(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		      संरचना repository *repo अप्रयुक्तम्)
{
	अचिह्नित flag = 0;
	पूर्णाङ्क from_stdin = 0;
	संरचना option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_अन्तः()
	};
	संरचना notes_वृक्षः *t;
	पूर्णाङ्क retval = 0;

	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	यदि (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} अन्यथा {
		यावत् (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	यदि (from_stdin) {
		संरचना strbuf sb = STRBUF_INIT;
		यावत् (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	यदि (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क prune(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		 संरचना repository *repo अप्रयुक्तम्)
{
	संरचना notes_वृक्षः *t;
	पूर्णाङ्क show_only = 0, verbose = 0;
	संरचना option options[] = {
		OPT_DRY_सततम्(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_अन्तः()
	};

	argc = parse_options(argc, argv, उपसर्गः, options, git_notes_prune_usage,
			     0);

	यदि (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	यदि (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_ref(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *उपसर्गः,
		   संरचना repository *repo अप्रयुक्तम्)
{
	संरचना option options[] = { OPT_अन्तः() };
	वर्ण *notes_ref;
	argc = parse_options(argc, argv, उपसर्गः, options,
			     git_notes_get_ref_usage, 0);

	यदि (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	पङ्क्तिं_लिख(notes_ref);
	मुक्त(notes_ref);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_notes(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *उपसर्गः,
	      संरचना repository *repo)
{
	ध्रुव वर्ण *override_notes_ref = रिक्त;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, सङ्कलय),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, merge),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_अन्तः()
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, उपसर्गः, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	यदि (!fn) {
		यदि (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	यदि (override_notes_ref) {
		संरचना strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	प्रत्यागम !!fn(argc, argv, उपसर्गः, repo);
}
