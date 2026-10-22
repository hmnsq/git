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

स्थिर स्थिरांक अक्षर *separator = "\n";
स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_usage[] = {
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

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	रिक्त
};

स्थिर स्थिरांक अक्षर * स्थिरांक git_notes_get_ref_usage[] = {
	"git notes get-ref",
	रिक्त
};

स्थिर स्थिरांक अक्षर note_template[] =
	N_("Write/edit the notes for the following object:");

गणना notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

संरचना note_संदेश {
	गणना notes_stripspace stripspace;
	संरचना strbuf buf;
};

संरचना note_data {
	पूर्णांक use_editor;
	पूर्णांक stripspace;
	अक्षर *edit_path;
	संरचना strbuf buf;
	संरचना note_संदेश **messages;
	माप_प्रकार संदेश_nr;
	माप_प्रकार संदेश_alloc;
};

स्थिर शून्य free_note_data(संरचना note_data *d)
{
	जर (d->edit_path) {
		unlink_or_warn(d->edit_path);
		मुक्त(d->edit_path);
	}
	strbuf_release(&d->buf);

	पर्यंत (d->संदेश_nr--) {
		strbuf_release(&d->messages[d->संदेश_nr]->buf);
		मुक्त(d->messages[d->संदेश_nr]);
	}
	मुक्त(d->messages);
}

स्थिर पूर्णांक list_each_note(स्थिरांक संरचना object_id *object_oid,
			  स्थिरांक संरचना object_id *note_oid,
			  अक्षर *note_path UNUSED,
			  शून्य *cb_data UNUSED)
{
	म_लिखो("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	परत 0;
}

स्थिर शून्य copy_obj_to_fd(पूर्णांक fd, स्थिरांक संरचना object_id *oid)
{
	माप_प्रकार size;
	गणना object_type type;
	अक्षर *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	जर (buf) {
		जर (size)
			write_or_die(fd, buf, size);
		मुक्त(buf);
	}
}

स्थिर शून्य write_commented_object(पूर्णांक fd, स्थिरांक संरचना object_id *object)
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
	जर (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	जर (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_ओळी(&cbuf, buf.buf, buf.len, comment_ओळ_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	जर (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

स्थिर शून्य prepare_note_data(स्थिरांक संरचना object_id *object, संरचना note_data *d,
		स्थिरांक संरचना object_id *old_note)
{
	जर (d->use_editor || !d->संदेश_nr) {
		पूर्णांक fd;
		संरचना strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		जर (d->संदेश_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		अन्यथा जर (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_ओळी(&buf, "\n", म_माप("\n"), comment_ओळ_str);
		strbuf_add_commented_ओळी(&buf, _(note_template), म_माप(_(note_template)),
					   comment_ओळ_str);
		strbuf_add_commented_ओळी(&buf, "\n", म_माप("\n"), comment_ओळ_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		बंद(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		जर (launch_editor(d->edit_path, &d->buf, रिक्त)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		जर (d->stripspace)
			strbuf_stripspace(&d->buf, comment_ओळ_str);
	}
}

स्थिर शून्य write_note_data(संरचना note_data *d, संरचना object_id *oid)
{
	जर (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		पूर्णांक status = die_message(_("unable to write note object"));

		जर (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		निकास(status);
	}
}

स्थिर शून्य append_separator(संरचना strbuf *message)
{
	माप_प्रकार sep_len = 0;

	जर (!separator)
		परत;
	अन्यथा जर ((sep_len = म_माप(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	अन्यथा
		strbuf_addf(message, "%s%s", separator, "\n");
}

स्थिर शून्य concat_messages(संरचना note_data *d)
{
	संरचना strbuf संदेश = STRBUF_INIT;
	माप_प्रकार i;

	साठी (i = 0; i < d->संदेश_nr ; i++) {
		जर (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&संदेश, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &संदेश);
		जर ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, रिक्त);
		strbuf_reset(&संदेश);
	}
	strbuf_release(&संदेश);
}

स्थिर पूर्णांक parse_संदेश_arg(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना note_data *d = opt->मूल्य;
	संरचना note_संदेश *संदेश = xmalloc(आकार(*संदेश));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&संदेश->buf, म_माप(arg));
	strbuf_addstr(&संदेश->buf, arg);
	ALLOC_GROW_BY(d->messages, d->संदेश_nr, 1, d->संदेश_alloc);
	d->messages[d->संदेश_nr - 1] = संदेश;
	संदेश->stripspace = STRIPSPACE;
	परत 0;
}

स्थिर पूर्णांक parse_file_arg(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना note_data *d = opt->मूल्य;
	संरचना note_संदेश *संदेश = xmalloc(आकार(*संदेश));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&संदेश->buf , 0);
	जर (!म_भेद(arg, "-")) {
		जर (strbuf_read(&संदेश->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} अन्यथा जर (strbuf_read_file(&संदेश->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->संदेश_nr, 1, d->संदेश_alloc);
	d->messages[d->संदेश_nr - 1] = संदेश;
	संदेश->stripspace = STRIPSPACE;
	परत 0;
}

स्थिर पूर्णांक parse_reuse_arg(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना note_data *d = opt->मूल्य;
	संरचना note_संदेश *संदेश = xmalloc(आकार(*संदेश));
	अक्षर *मूल्य;
	संरचना object_id object;
	गणना object_type type;
	माप_प्रकार len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&संदेश->buf, 0);
	जर (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	जर (!(मूल्य = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	जर (type != OBJ_BLOB) {
		strbuf_release(&संदेश->buf);
		मुक्त(मूल्य);
		मुक्त(संदेश);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&संदेश->buf, मूल्य, len);
	मुक्त(मूल्य);

	संदेश->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->संदेश_nr, 1, d->संदेश_alloc);
	d->messages[d->संदेश_nr - 1] = संदेश;
	संदेश->stripspace = NO_STRIPSPACE;
	परत 0;
}

स्थिर पूर्णांक parse_reedit_arg(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना note_data *d = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	परत parse_reuse_arg(opt, arg, unset);
}

स्थिर पूर्णांक parse_separator_arg(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
			       पूर्णांक unset)
{
	जर (unset)
		*(स्थिरांक अक्षर **)opt->मूल्य = रिक्त;
	अन्यथा
		*(स्थिरांक अक्षर **)opt->मूल्य = arg ? arg : "\n";
	परत 0;
}

स्थिर पूर्णांक notes_copy_from_stdin(पूर्णांक force, स्थिरांक अक्षर *rewrite_cmd)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना notes_rewrite_cfg *c = रिक्त;
	संरचना notes_वृक्ष *t = रिक्त;
	पूर्णांक ret = 0;
	स्थिरांक अक्षर *संदेश = "Notes added by 'git notes copy'";

	जर (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		जर (!c)
			परत 0;
	} अन्यथा {
		init_notes(रिक्त, रिक्त, रिक्त, NOTES_INIT_WRITABLE);
		t = &default_notes_वृक्ष;
	}

	पर्यंत (strbuf_getline_lf(&buf, मानक_निवेश) != खातापूर्ण) {
		संरचना object_id from_obj, to_obj;
		संरचना string_list split = STRING_LIST_INIT_NODUP;
		पूर्णांक err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		जर (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		जर (repo_get_oid(the_repository, split.वस्तुसंच[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.वस्तुसंच[0].string);
		जर (repo_get_oid(the_repository, split.वस्तुसंच[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.वस्तुसंच[1].string);

		जर (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		अन्यथा
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		जर (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.वस्तुसंच[0].string, split.वस्तुसंच[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	जर (!rewrite_cmd) {
		commit_notes(the_repository, t, संदेश);
		free_notes(t);
	} अन्यथा {
		finish_copy_notes_for_rewrite(the_repository, c, संदेश);
	}
	strbuf_release(&buf);
	परत ret;
}

स्थिर संरचना notes_वृक्ष *init_notes_check(स्थिरांक अक्षर *subcommand,
					   पूर्णांक flags)
{
	संरचना notes_वृक्ष *t;
	स्थिरांक अक्षर *ref;
	init_notes(रिक्त, रिक्त, रिक्त, flags);
	t = &default_notes_वृक्ष;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	जर (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	परत t;
}

स्थिर पूर्णांक list(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	संरचना notes_वृक्ष *t;
	संरचना object_id object;
	स्थिरांक संरचना object_id *note;
	पूर्णांक retval = -1;
	संरचना option options[] = {
		OPT_END()
	};

	जर (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	जर (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	जर (argc) {
		जर (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		जर (note) {
			माला_दो(oid_to_hex(note));
			retval = 0;
		} अन्यथा
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} अन्यथा
		retval = for_each_note(t, 0, list_each_note, रिक्त);

	free_notes(t);
	परत retval;
}

स्थिर पूर्णांक append_edit(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		       संरचना repository *repo UNUSED);

स्थिर पूर्णांक add(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
	       संरचना repository *repo)
{
	पूर्णांक force = 0, allow_empty = 0;
	स्थिरांक अक्षर *object_ref;
	संरचना notes_वृक्ष *t;
	संरचना object_id object, new_note;
	स्थिरांक संरचना object_id *note;
	संरचना note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	संरचना option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_संदेश_arg),
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

	जर (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	जर (d.संदेश_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	जर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	जर (note) {
		जर (!force) {
			free_notes(t);
			जर (d.संदेश_nr) {
				free_note_data(&d);
				परत error(_("Cannot add notes. "
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
			परत append_edit(argc, argv, prefix, repo);
		}
		ख_लिखो(मानक_त्रुटि, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	जर (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		जर (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} अन्यथा {
		ख_लिखो(मानक_त्रुटि, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	परत 0;
}

स्थिर पूर्णांक copy(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	पूर्णांक retval = 0, force = 0, from_stdin = 0;
	स्थिरांक संरचना object_id *from_note, *note;
	स्थिरांक अक्षर *object_ref;
	संरचना object_id object, from_obj;
	संरचना notes_वृक्ष *t;
	स्थिरांक अक्षर *rewrite_cmd = रिक्त;
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

	जर (from_stdin || rewrite_cmd) {
		जर (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} अन्यथा {
			परत notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	जर (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	जर (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	जर (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	जर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	जर (note) {
		जर (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			जा out;
		}
		ख_लिखो(मानक_त्रुटि, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	जर (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		जा out;
	}

	जर (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	परत retval;
}

स्थिर पूर्णांक append_edit(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		       संरचना repository *repo UNUSED)
{
	पूर्णांक allow_empty = 0;
	स्थिरांक अक्षर *object_ref;
	संरचना notes_वृक्ष *t;
	संरचना object_id object, new_note;
	स्थिरांक संरचना object_id *note;
	अक्षर *logmsg;
	स्थिरांक अक्षर * स्थिरांक *usage;
	संरचना note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	संरचना option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_संदेश_arg),
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
	पूर्णांक edit = !म_भेद(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	जर (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	जर (d.संदेश_nr) {
		concat_messages(&d);
		जर (edit)
			ख_लिखो(मानक_त्रुटि, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	जर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : रिक्त);

	जर (note && !edit) {
		/* Append buf to previous note contents */
		माप_प्रकार size;
		गणना object_type type;
		संरचना strbuf buf = STRBUF_INIT;
		अक्षर *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		जर (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		जर (size)
			strbuf_add(&buf, prev_buf, size);
		जर (d.buf.len && size)
			append_separator(&buf);
		strbuf_घाला(&d.buf, 0, buf.buf, buf.len);

		मुक्त(prev_buf);
		strbuf_release(&buf);
	}

	जर (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		जर (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} अन्यथा {
		ख_लिखो(मानक_त्रुटि, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	मुक्त(logmsg);
	free_note_data(&d);
	free_notes(t);
	परत 0;
}

स्थिर पूर्णांक show(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		संरचना repository *repo UNUSED)
{
	स्थिरांक अक्षर *object_ref;
	संरचना notes_वृक्ष *t;
	संरचना object_id object;
	स्थिरांक संरचना object_id *note;
	पूर्णांक retval;
	संरचना option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	जर (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	जर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	जर (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	अन्यथा {
		स्थिरांक अक्षर *show_args[3] = {"show", oid_to_hex(note), रिक्त};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	परत retval;
}

स्थिर पूर्णांक merge_abort(संरचना notes_merge_options *o)
{
	पूर्णांक ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	जर (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, "NOTES_MERGE_PARTIAL", रिक्त, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	जर (refs_delete_ref(get_main_ref_store(the_repository), रिक्त, "NOTES_MERGE_REF", रिक्त, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	जर (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	परत ret;
}

स्थिर पूर्णांक merge_commit(संरचना notes_merge_options *o)
{
	संरचना strbuf संदेश = STRBUF_INIT;
	संरचना object_id oid, parent_oid;
	संरचना notes_वृक्ष t = {0};
	संरचना commit *partial;
	संरचना pretty_print_context pretty_ctx;
	शून्य *local_ref_to_free;
	पूर्णांक ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	जर (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	अन्यथा जर (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	अन्यथा जर (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	जर (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	अन्यथा
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, रिक्त);
	जर (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	जर (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	स_रखो(&pretty_ctx, 0, आकार(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &संदेश,
				   &pretty_ctx);
	strbuf_trim(&संदेश);
	strbuf_insertstr(&संदेश, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? रिक्त : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&संदेश);
	ret = merge_abort(o);
	मुक्त(local_ref_to_free);
	परत ret;
}

स्थिर पूर्णांक git_config_get_notes_strategy(स्थिरांक अक्षर *किल्ली,
					 गणना notes_merge_strategy *strategy)
{
	अक्षर *मूल्य;

	जर (repo_config_get_string(the_repository, किल्ली, &मूल्य))
		परत 1;
	जर (parse_notes_merge_strategy(मूल्य, strategy))
		git_die_config(the_repository, किल्ली, _("unknown notes merge strategy %s"), मूल्य);

	मुक्त(मूल्य);
	परत 0;
}

स्थिर पूर्णांक merge(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना strbuf remote_ref = STRBUF_INIT, संदेश = STRBUF_INIT;
	संरचना object_id result_oid;
	संरचना notes_वृक्ष *t;
	संरचना notes_merge_options o;
	पूर्णांक do_merge = 0, do_commit = 0, do_abort = 0;
	पूर्णांक verbosity = 0, result;
	स्थिरांक अक्षर *strategy = रिक्त;
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
	अक्षर *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	जर (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	जर (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	जर (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} अन्यथा जर (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	जर (do_abort)
		परत merge_abort(&o);
	जर (do_commit)
		परत merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	जर (strategy) {
		जर (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} अन्यथा {
		संरचना strbuf merge_किल्ली = STRBUF_INIT;
		स्थिरांक अक्षर *short_ref = रिक्त;

		जर (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_किल्ली, "notes.%s.mergeStrategy", short_ref);

		जर (git_config_get_notes_strategy(merge_किल्ली.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_किल्ली);
	}

	strbuf_addf(&संदेश, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_संदेश), संदेश.buf + 7, संदेश.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	जर (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				notes_ref, &result_oid, रिक्त, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अन्यथा { /* Merge has unresolved conflicts */
		संरचना worktree **worktrees;
		स्थिरांक संरचना worktree *wt;
		अक्षर *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, रिक्त,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		जर (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		जर (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, रिक्त))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		ख_लिखो(मानक_त्रुटि, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		मुक्त(path);
	}

	free_notes(t);
	मुक्त(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&संदेश);
	परत result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

स्थिर पूर्णांक remove_one_note(संरचना notes_वृक्ष *t, स्थिरांक अक्षर *name, अचिन्हित flag)
{
	पूर्णांक status;
	संरचना object_id oid;
	जर (repo_get_oid(the_repository, name, &oid))
		परत error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	जर (status)
		ख_लिखो(मानक_त्रुटि, _("Object %s has no note\n"), name);
	अन्यथा
		ख_लिखो(मानक_त्रुटि, _("Removing note for object %s\n"), name);
	परत (flag & IGNORE_MISSING) ? 0 : status;
}

स्थिर पूर्णांक remove_cmd(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		      संरचना repository *repo UNUSED)
{
	अचिन्हित flag = 0;
	पूर्णांक from_stdin = 0;
	संरचना option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	संरचना notes_वृक्ष *t;
	पूर्णांक retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	जर (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} अन्यथा {
		पर्यंत (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	जर (from_stdin) {
		संरचना strbuf sb = STRBUF_INIT;
		पर्यंत (strbuf_getwholeline(&sb, मानक_निवेश, '\n') != खातापूर्ण) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	जर (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	परत retval;
}

स्थिर पूर्णांक prune(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		 संरचना repository *repo UNUSED)
{
	संरचना notes_वृक्ष *t;
	पूर्णांक show_only = 0, verbose = 0;
	संरचना option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	जर (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	जर (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	परत 0;
}

स्थिर पूर्णांक get_ref(पूर्णांक argc, स्थिरांक अक्षर **argv, स्थिरांक अक्षर *prefix,
		   संरचना repository *repo UNUSED)
{
	संरचना option options[] = { OPT_END() };
	अक्षर *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	जर (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	माला_दो(notes_ref);
	मुक्त(notes_ref);
	परत 0;
}

पूर्णांक cmd_notes(पूर्णांक argc,
	      स्थिरांक अक्षर **argv,
	      स्थिरांक अक्षर *prefix,
	      संरचना repository *repo)
{
	स्थिरांक अक्षर *override_notes_ref = रिक्त;
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
	जर (!fn) {
		जर (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	जर (override_notes_ref) {
		संरचना strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	परत !!fn(argc, argv, prefix, repo);
}
