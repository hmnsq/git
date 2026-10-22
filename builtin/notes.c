/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
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

स्थिर ध्रुव वर्ण * ध्रुव git_notes_add_usage[] = {
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

संरचना note_data {
	पूर्णाङ्क use_editor;
	पूर्णाङ्क stripspace;
	वर्ण *edit_path;
	संरचना strbuf buf;
	संरचना note_सन्देशः **messages;
	size_t सन्देशः_nr;
	size_t सन्देशः_alloc;
};

स्थिर शून्य free_note_data(संरचना note_data *d)
{
	जइ (d->edit_path) {
		unlink_or_warn(d->edit_path);
		मुक्त(d->edit_path);
	}
	strbuf_release(&d->buf);

	जाव (d->सन्देशः_nr--) {
		strbuf_release(&d->messages[d->सन्देशः_nr]->buf);
		मुक्त(d->messages[d->सन्देशः_nr]);
	}
	मुक्त(d->messages);
}

स्थिर पूर्णाङ्क list_each_note(ध्रुव संरचना object_id *object_oid,
			  ध्रुव संरचना object_id *note_oid,
			  वर्ण *note_path UNUSED,
			  शून्य *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	प्रत्यागम 0;
}

स्थिर शून्य copy_obj_to_fd(पूर्णाङ्क fd, ध्रुव संरचना object_id *oid)
{
	size_t size;
	गणना object_type type;
	वर्ण *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	जइ (buf) {
		जइ (size)
			write_or_die(fd, buf, size);
		मुक्त(buf);
	}
}

स्थिर शून्य write_commented_object(पूर्णाङ्क fd, ध्रुव संरचना object_id *object)
{
	संरचना child_process show = CHILD_PROCESS_INIT;
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), रिक्त);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	जइ (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	जइ (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_पङ्क्तयः(&cbuf, buf.buf, buf.len, comment_पङ्क्तिः_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	जइ (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

स्थिर शून्य prepare_note_data(ध्रुव संरचना object_id *object, संरचना note_data *d,
		ध्रुव संरचना object_id *old_note)
{
	जइ (d->use_editor || !d->सन्देशः_nr) {
		पूर्णाङ्क fd;
		संरचना strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		जइ (d->सन्देशः_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		अण्णहा जइ (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_पङ्क्तयः(&buf, "\n", strlen("\n"), comment_पङ्क्तिः_str);
		strbuf_add_commented_पङ्क्तयः(&buf, _(note_template), strlen(_(note_template)),
					   comment_पङ्क्तिः_str);
		strbuf_add_commented_पङ्क्तयः(&buf, "\n", strlen("\n"), comment_पङ्क्तिः_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		जइ (launch_editor(d->edit_path, &d->buf, रिक्त)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		जइ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_पङ्क्तिः_str);
	}
}

स्थिर शून्य write_note_data(संरचना note_data *d, संरचना object_id *oid)
{
	जइ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		पूर्णाङ्क status = die_message(_("unable to write note object"));

		जइ (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

स्थिर शून्य append_separator(संरचना strbuf *message)
{
	size_t sep_len = 0;

	जइ (!separator)
		प्रत्यागम;
	अण्णहा जइ ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	अण्णहा
		strbuf_addf(message, "%s%s", separator, "\n");
}

स्थिर शून्य concat_messages(संरचना note_data *d)
{
	संरचना strbuf सन्देशः = STRBUF_INIT;
	size_t i;

	प्रति (i = 0; i < d->सन्देशः_nr ; i++) {
		जइ (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&सन्देशः, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &सन्देशः);
		जइ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, रिक्त);
		strbuf_reset(&सन्देशः);
	}
	strbuf_release(&सन्देशः);
}

स्थिर पूर्णाङ्क parse_सन्देशः_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_data *d = opt->मानम्;
	संरचना note_सन्देशः *सन्देशः = xmalloc(परिमाण(*सन्देशः));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&सन्देशः->buf, strlen(arg));
	strbuf_addstr(&सन्देशः->buf, arg);
	ALLOC_GROW_BY(d->messages, d->सन्देशः_nr, 1, d->सन्देशः_alloc);
	d->messages[d->सन्देशः_nr - 1] = सन्देशः;
	सन्देशः->stripspace = STRIPSPACE;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_file_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_data *d = opt->मानम्;
	संरचना note_सन्देशः *सन्देशः = xmalloc(परिमाण(*सन्देशः));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&सन्देशः->buf , 0);
	जइ (!strcmp(arg, "-")) {
		जइ (strbuf_read(&सन्देशः->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} अण्णहा जइ (strbuf_read_file(&सन्देशः->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->सन्देशः_nr, 1, d->सन्देशः_alloc);
	d->messages[d->सन्देशः_nr - 1] = सन्देशः;
	सन्देशः->stripspace = STRIPSPACE;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_reuse_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_data *d = opt->मानम्;
	संरचना note_सन्देशः *सन्देशः = xmalloc(परिमाण(*सन्देशः));
	वर्ण *मानम्;
	संरचना object_id object;
	गणना object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&सन्देशः->buf, 0);
	जइ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	जइ (!(मानम् = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	जइ (type != OBJ_BLOB) {
		strbuf_release(&सन्देशः->buf);
		मुक्त(मानम्);
		मुक्त(सन्देशः);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&सन्देशः->buf, मानम्, len);
	मुक्त(मानम्);

	सन्देशः->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->सन्देशः_nr, 1, d->सन्देशः_alloc);
	d->messages[d->सन्देशः_nr - 1] = सन्देशः;
	सन्देशः->stripspace = NO_STRIPSPACE;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_reedit_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना note_data *d = opt->मानम्;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	प्रत्यागम parse_reuse_arg(opt, arg, unset);
}

स्थिर पूर्णाङ्क parse_separator_arg(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			       पूर्णाङ्क unset)
{
	जइ (unset)
		*(ध्रुव वर्ण **)opt->मानम् = रिक्त;
	अण्णहा
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

	जइ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		जइ (!c)
			प्रत्यागम 0;
	} अण्णहा {
		init_notes(रिक्त, रिक्त, रिक्त, NOTES_INIT_WRITABLE);
		t = &default_notes_वृक्षः;
	}

	जाव (strbuf_getline_lf(&buf, stdin) != EOF) {
		संरचना object_id from_obj, to_obj;
		संरचना string_list split = STRING_LIST_INIT_NODUP;
		पूर्णाङ्क err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		जइ (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		जइ (repo_get_oid(the_repository, split.वस्तूनि[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.वस्तूनि[0].string);
		जइ (repo_get_oid(the_repository, split.वस्तूनि[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.वस्तूनि[1].string);

		जइ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		अण्णहा
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		जइ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.वस्तूनि[0].string, split.वस्तूनि[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	जइ (!rewrite_cmd) {
		commit_notes(the_repository, t, सन्देशः);
		free_notes(t);
	} अण्णहा {
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
	जइ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	प्रत्यागम t;
}

स्थिर पूर्णाङ्क list(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना notes_वृक्षः *t;
	संरचना object_id object;
	ध्रुव संरचना object_id *note;
	पूर्णाङ्क retval = -1;
	संरचना option options[] = {
		OPT_END()
	};

	जइ (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	जइ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	जइ (argc) {
		जइ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		जइ (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} अण्णहा
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} अण्णहा
		retval = for_each_note(t, 0, list_each_note, रिक्त);

	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क append_edit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		       संरचना repository *repo UNUSED);

स्थिर पूर्णाङ्क add(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
	       संरचना repository *repo)
{
	पूर्णाङ्क force = 0, allow_empty = 0;
	ध्रुव वर्ण *object_ref;
	संरचना notes_वृक्षः *t;
	संरचना object_id object, new_note;
	ध्रुव संरचना object_id *note;
	संरचना note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

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
		OPT_BOOL(0, "allow-empty", &allow_empty,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_add_usage,
			     PARSE_OPT_KEEP_ARGV0);

	जइ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	जइ (d.सन्देशः_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	जइ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	जइ (note) {
		जइ (!force) {
			free_notes(t);
			जइ (d.सन्देशः_nr) {
				free_note_data(&d);
				प्रत्यागम error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_to_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			प्रत्यागम append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	जइ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		जइ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} अण्णहा {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क copy(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
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
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	जइ (from_stdin || rewrite_cmd) {
		जइ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} अण्णहा {
			प्रत्यागम notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	जइ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	जइ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	जइ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	जइ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	जइ (note) {
		जइ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			गच्छ out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	जइ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		गच्छ out;
	}

	जइ (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क append_edit(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		       संरचना repository *repo UNUSED)
{
	पूर्णाङ्क allow_empty = 0;
	ध्रुव वर्ण *object_ref;
	संरचना notes_वृक्षः *t;
	संरचना object_id object, new_note;
	ध्रुव संरचना object_id *note;
	वर्ण *logmsg;
	ध्रुव वर्ण * ध्रुव *usage;
	संरचना note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
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
		OPT_BOOL(0, "allow-empty", &allow_empty,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_END()
	};
	पूर्णाङ्क edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	जइ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	जइ (d.सन्देशः_nr) {
		concat_messages(&d);
		जइ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	जइ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : रिक्त);

	जइ (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		गणना object_type type;
		संरचना strbuf buf = STRBUF_INIT;
		वर्ण *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		जइ (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		जइ (size)
			strbuf_add(&buf, prev_buf, size);
		जइ (d.buf.len && size)
			append_separator(&buf);
		strbuf_निवेशय(&d.buf, 0, buf.buf, buf.len);

		मुक्त(prev_buf);
		strbuf_release(&buf);
	}

	जइ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		जइ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} अण्णहा {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	मुक्त(logmsg);
	free_note_data(&d);
	free_notes(t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क show(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		संरचना repository *repo UNUSED)
{
	ध्रुव वर्ण *object_ref;
	संरचना notes_वृक्षः *t;
	संरचना object_id object;
	ध्रुव संरचना object_id *note;
	पूर्णाङ्क retval;
	संरचना option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	जइ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	जइ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	जइ (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	अण्णहा {
		ध्रुव वर्ण *show_args[3] = {"show", oid_to_hex(note), रिक्त};
		retval = execv_git_cmd(show_args);
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

	जइ (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, "NOTES_MERGE_PARTIAL", रिक्त, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	जइ (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, "NOTES_MERGE_REF", रिक्त, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	जइ (notes_merge_abort(o))
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
	शून्य *local_ref_to_free;
	पूर्णाङ्क ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	जइ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	अण्णहा जइ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	अण्णहा जइ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	जइ (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	अण्णहा
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, रिक्त);
	जइ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	जइ (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, परिमाण(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &सन्देशः,
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
	मुक्त(local_ref_to_free);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क git_config_get_notes_strategy(ध्रुव वर्ण *कुञ्चिका,
					 गणना notes_merge_strategy *strategy)
{
	वर्ण *मानम्;

	जइ (repo_config_get_string(the_repository, कुञ्चिका, &मानम्))
		प्रत्यागम 1;
	जइ (parse_notes_merge_strategy(मानम्, strategy))
		git_die_config(the_repository, कुञ्चिका, _("unknown notes merge strategy %s"), मानम्);

	मुक्त(मानम्);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क merge(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना strbuf remote_ref = STRBUF_INIT, सन्देशः = STRBUF_INIT;
	संरचना object_id result_oid;
	संरचना notes_वृक्षः *t;
	संरचना notes_merge_options o;
	पूर्णाङ्क do_merge = 0, do_commit = 0, do_abort = 0;
	पूर्णाङ्क verbosity = 0, result;
	ध्रुव वर्ण *strategy = रिक्त;
	संरचना option options[] = {
		OPT_GROUP(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_GROUP(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_GROUP(N_("Committing unmerged notes")),
		OPT_SET_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_GROUP(N_("Aborting notes merge resolution")),
		OPT_SET_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_END()
	};
	वर्ण *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	जइ (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	जइ (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	जइ (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} अण्णहा जइ (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	जइ (do_abort)
		प्रत्यागम merge_abort(&o);
	जइ (do_commit)
		प्रत्यागम merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	जइ (strategy) {
		जइ (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} अण्णहा {
		संरचना strbuf merge_कुञ्चिका = STRBUF_INIT;
		ध्रुव वर्ण *short_ref = रिक्त;

		जइ (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_कुञ्चिका, "notes.%s.mergeStrategy", short_ref);

		जइ (git_config_get_notes_strategy(merge_कुञ्चिका.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_कुञ्चिका);
	}

	strbuf_addf(&सन्देशः, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_सन्देशः), सन्देशः.buf + 7, सन्देशः.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	जइ (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः.buf,
				notes_ref, &result_oid, रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अण्णहा { /* Merge has unresolved conflicts */
		संरचना worktree **worktrees;
		ध्रुव संरचना worktree *wt;
		वर्ण *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), सन्देशः.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, रिक्त,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		जइ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		जइ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, रिक्त))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		मुक्त(path);
	}

	free_notes(t);
	मुक्त(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&सन्देशः);
	प्रत्यागम result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

स्थिर पूर्णाङ्क remove_one_note(संरचना notes_वृक्षः *t, ध्रुव वर्ण *name, unsigned flag)
{
	पूर्णाङ्क status;
	संरचना object_id oid;
	जइ (repo_get_oid(the_repository, name, &oid))
		प्रत्यागम error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	जइ (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	अण्णहा
		fprintf(stderr, _("Removing note for object %s\n"), name);
	प्रत्यागम (flag & IGNORE_MISSING) ? 0 : status;
}

स्थिर पूर्णाङ्क remove_cmd(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		      संरचना repository *repo UNUSED)
{
	unsigned flag = 0;
	पूर्णाङ्क from_stdin = 0;
	संरचना option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	संरचना notes_वृक्षः *t;
	पूर्णाङ्क retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	जइ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} अण्णहा {
		जाव (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	जइ (from_stdin) {
		संरचना strbuf sb = STRBUF_INIT;
		जाव (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	जइ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क prune(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना notes_वृक्षः *t;
	पूर्णाङ्क show_only = 0, verbose = 0;
	संरचना option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	जइ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	जइ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क get_ref(पूर्णाङ्क argc, ध्रुव वर्ण **argv, ध्रुव वर्ण *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना option options[] = { OPT_END() };
	वर्ण *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	जइ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	मुक्त(notes_ref);
	प्रत्यागम 0;
}

पूर्णाङ्क cmd_notes(पूर्णाङ्क argc,
	      ध्रुव वर्ण **argv,
	      ध्रुव वर्ण *prefix,
	      संरचना repository *repo)
{
	ध्रुव वर्ण *override_notes_ref = रिक्त;
	parse_opt_subcommand_fn *fn = रिक्त;
	संरचना option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, add),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, merge),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, रिक्त);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	जइ (!fn) {
		जइ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	जइ (override_notes_ref) {
		संरचना strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	प्रत्यागम !!fn(argc, argv, prefix, repo);
}
