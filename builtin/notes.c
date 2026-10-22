/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "editor.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "hex.h"
#समावेश "notes.h"
#समावेश "object-file.h"
#समावेश "object-name.h"
#समावेश "odb.h"
#समावेश "path.h"

#समावेश "pretty.h"
#समावेश "refs.h"
#समावेश "exec-cmd.h"
#समावेश "run-command.h"
#समावेश "parse-options.h"
#समावेश "string-list.h"
#समावेश "notes-merge.h"
#समावेश "notes-utils.h"
#समावेश "worktree.h"
#समावेश "write-or-die.h"

अटल स्थिर अक्षर *separator = "\n";
अटल स्थिर अक्षर * स्थिर git_notes_usage[] = {
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
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	शून्य
};

अटल स्थिर अक्षर * स्थिर git_notes_get_ref_usage[] = {
	"git notes get-ref",
	शून्य
};

अटल स्थिर अक्षर note_template[] =
	N_("Write/edit the notes for the following object:");

क्रमागत notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

काष्ठा note_संदेश {
	क्रमागत notes_stripspace stripspace;
	काष्ठा strbuf buf;
};

काष्ठा note_तथ्य {
	पूर्णांक use_editor;
	पूर्णांक stripspace;
	अक्षर *edit_path;
	काष्ठा strbuf buf;
	काष्ठा note_संदेश **messages;
	माप_प्रकार संदेश_nr;
	माप_प्रकार संदेश_alloc;
};

अटल व्योम free_note_तथ्य(काष्ठा note_तथ्य *d)
{
	अगर (d->edit_path) {
		unlink_or_warn(d->edit_path);
		मुक्त(d->edit_path);
	}
	strbuf_release(&d->buf);

	जबतक (d->संदेश_nr--) {
		strbuf_release(&d->messages[d->संदेश_nr]->buf);
		मुक्त(d->messages[d->संदेश_nr]);
	}
	मुक्त(d->messages);
}

अटल पूर्णांक list_each_note(स्थिर काष्ठा object_id *object_oid,
			  स्थिर काष्ठा object_id *note_oid,
			  अक्षर *note_path UNUSED,
			  व्योम *cb_तथ्य UNUSED)
{
	म_लिखो("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	वापस 0;
}

अटल व्योम copy_obj_to_fd(पूर्णांक fd, स्थिर काष्ठा object_id *oid)
{
	माप_प्रकार आकार;
	क्रमागत object_type type;
	अक्षर *buf = odb_read_object(the_repository->objects, oid, &type, &आकार);
	अगर (buf) {
		अगर (आकार)
			write_or_die(fd, buf, आकार);
		मुक्त(buf);
	}
}

अटल व्योम write_commented_object(पूर्णांक fd, स्थिर काष्ठा object_id *object)
{
	काष्ठा child_process show = CHILD_PROCESS_INIT;
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), शून्य);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	अगर (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	अगर (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_पंक्तियाँ(&cbuf, buf.buf, buf.len, comment_पंक्ति_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	अगर (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

अटल व्योम prepare_note_तथ्य(स्थिर काष्ठा object_id *object, काष्ठा note_तथ्य *d,
		स्थिर काष्ठा object_id *old_note)
{
	अगर (d->use_editor || !d->संदेश_nr) {
		पूर्णांक fd;
		काष्ठा strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		अगर (d->संदेश_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		अन्यथा अगर (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_पंक्तियाँ(&buf, "\n", म_माप("\n"), comment_पंक्ति_str);
		strbuf_add_commented_पंक्तियाँ(&buf, _(note_template), म_माप(_(note_template)),
					   comment_पंक्ति_str);
		strbuf_add_commented_पंक्तियाँ(&buf, "\n", म_माप("\n"), comment_पंक्ति_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		बंद(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		अगर (launch_editor(d->edit_path, &d->buf, शून्य)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		अगर (d->stripspace)
			strbuf_stripspace(&d->buf, comment_पंक्ति_str);
	}
}

अटल व्योम write_note_तथ्य(काष्ठा note_तथ्य *d, काष्ठा object_id *oid)
{
	अगर (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		पूर्णांक status = die_message(_("unable to write note object"));

		अगर (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		निकास(status);
	}
}

अटल व्योम append_separator(काष्ठा strbuf *message)
{
	माप_प्रकार sep_len = 0;

	अगर (!separator)
		वापस;
	अन्यथा अगर ((sep_len = म_माप(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	अन्यथा
		strbuf_addf(message, "%s%s", separator, "\n");
}

अटल व्योम concat_messages(काष्ठा note_तथ्य *d)
{
	काष्ठा strbuf संदेश = STRBUF_INIT;
	माप_प्रकार i;

	क्रम (i = 0; i < d->संदेश_nr ; i++) {
		अगर (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&संदेश, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &संदेश);
		अगर ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, शून्य);
		strbuf_reset(&संदेश);
	}
	strbuf_release(&संदेश);
}

अटल पूर्णांक parse_संदेश_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा note_तथ्य *d = opt->मूल्य;
	काष्ठा note_संदेश *संदेश = xmalloc(माप(*संदेश));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&संदेश->buf, म_माप(arg));
	strbuf_addstr(&संदेश->buf, arg);
	ALLOC_GROW_BY(d->messages, d->संदेश_nr, 1, d->संदेश_alloc);
	d->messages[d->संदेश_nr - 1] = संदेश;
	संदेश->stripspace = STRIPSPACE;
	वापस 0;
}

अटल पूर्णांक parse_file_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा note_तथ्य *d = opt->मूल्य;
	काष्ठा note_संदेश *संदेश = xmalloc(माप(*संदेश));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&संदेश->buf , 0);
	अगर (!म_भेद(arg, "-")) {
		अगर (strbuf_read(&संदेश->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} अन्यथा अगर (strbuf_read_file(&संदेश->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->संदेश_nr, 1, d->संदेश_alloc);
	d->messages[d->संदेश_nr - 1] = संदेश;
	संदेश->stripspace = STRIPSPACE;
	वापस 0;
}

अटल पूर्णांक parse_reuse_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा note_तथ्य *d = opt->मूल्य;
	काष्ठा note_संदेश *संदेश = xmalloc(माप(*संदेश));
	अक्षर *मूल्य;
	काष्ठा object_id object;
	क्रमागत object_type type;
	माप_प्रकार len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&संदेश->buf, 0);
	अगर (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	अगर (!(मूल्य = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	अगर (type != OBJ_BLOB) {
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
	वापस 0;
}

अटल पूर्णांक parse_reedit_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा note_तथ्य *d = opt->मूल्य;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	वापस parse_reuse_arg(opt, arg, unset);
}

अटल पूर्णांक parse_separator_arg(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			       पूर्णांक unset)
{
	अगर (unset)
		*(स्थिर अक्षर **)opt->मूल्य = शून्य;
	अन्यथा
		*(स्थिर अक्षर **)opt->मूल्य = arg ? arg : "\n";
	वापस 0;
}

अटल पूर्णांक notes_copy_from_stdin(पूर्णांक force, स्थिर अक्षर *rewrite_cmd)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा notes_rewrite_cfg *c = शून्य;
	काष्ठा notes_वृक्ष *t = शून्य;
	पूर्णांक ret = 0;
	स्थिर अक्षर *संदेश = "Notes added by 'git notes copy'";

	अगर (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		अगर (!c)
			वापस 0;
	} अन्यथा {
		init_notes(शून्य, शून्य, शून्य, NOTES_INIT_WRITABLE);
		t = &default_notes_वृक्ष;
	}

	जबतक (strbuf_getline_lf(&buf, मानक_निवेश) != खातापूर्ण) {
		काष्ठा object_id from_obj, to_obj;
		काष्ठा string_list split = STRING_LIST_INIT_NODUP;
		पूर्णांक err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		अगर (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		अगर (repo_get_oid(the_repository, split.वस्तुएँ[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.वस्तुएँ[0].string);
		अगर (repo_get_oid(the_repository, split.वस्तुएँ[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.वस्तुएँ[1].string);

		अगर (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		अन्यथा
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		अगर (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.वस्तुएँ[0].string, split.वस्तुएँ[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	अगर (!rewrite_cmd) {
		commit_notes(the_repository, t, संदेश);
		free_notes(t);
	} अन्यथा {
		finish_copy_notes_for_rewrite(the_repository, c, संदेश);
	}
	strbuf_release(&buf);
	वापस ret;
}

अटल काष्ठा notes_वृक्ष *init_notes_check(स्थिर अक्षर *subcommand,
					   पूर्णांक flags)
{
	काष्ठा notes_वृक्ष *t;
	स्थिर अक्षर *ref;
	init_notes(शून्य, शून्य, शून्य, flags);
	t = &default_notes_वृक्ष;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	अगर (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	वापस t;
}

अटल पूर्णांक list(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	काष्ठा notes_वृक्ष *t;
	काष्ठा object_id object;
	स्थिर काष्ठा object_id *note;
	पूर्णांक retval = -1;
	काष्ठा option options[] = {
		OPT_END()
	};

	अगर (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	अगर (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	अगर (argc) {
		अगर (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		अगर (note) {
			माला_दो(oid_to_hex(note));
			retval = 0;
		} अन्यथा
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} अन्यथा
		retval = for_each_note(t, 0, list_each_note, शून्य);

	free_notes(t);
	वापस retval;
}

अटल पूर्णांक append_edit(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED);

अटल पूर्णांक add(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
	       काष्ठा repository *repo)
{
	पूर्णांक force = 0, allow_empty = 0;
	स्थिर अक्षर *object_ref;
	काष्ठा notes_वृक्ष *t;
	काष्ठा object_id object, new_note;
	स्थिर काष्ठा object_id *note;
	काष्ठा note_तथ्य d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	काष्ठा option options[] = {
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

	अगर (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	अगर (d.संदेश_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	अगर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	अगर (note) {
		अगर (!force) {
			free_notes(t);
			अगर (d.संदेश_nr) {
				free_note_तथ्य(&d);
				वापस error(_("Cannot add notes. "
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
			वापस append_edit(argc, argv, prefix, repo);
		}
		ख_लिखो(मानक_त्रुटि, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_तथ्य(&object, &d, note);
	अगर (d.buf.len || allow_empty) {
		write_note_तथ्य(&d, &new_note);
		अगर (add_note(t, &object, &new_note, combine_notes_overwrite))
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

	free_note_तथ्य(&d);
	free_notes(t);
	वापस 0;
}

अटल पूर्णांक copy(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	पूर्णांक retval = 0, force = 0, from_stdin = 0;
	स्थिर काष्ठा object_id *from_note, *note;
	स्थिर अक्षर *object_ref;
	काष्ठा object_id object, from_obj;
	काष्ठा notes_वृक्ष *t;
	स्थिर अक्षर *rewrite_cmd = शून्य;
	काष्ठा option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	अगर (from_stdin || rewrite_cmd) {
		अगर (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} अन्यथा {
			वापस notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	अगर (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	अगर (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	अगर (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	अगर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	अगर (note) {
		अगर (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			जाओ out;
		}
		ख_लिखो(मानक_त्रुटि, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	अगर (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		जाओ out;
	}

	अगर (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	वापस retval;
}

अटल पूर्णांक append_edit(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		       काष्ठा repository *repo UNUSED)
{
	पूर्णांक allow_empty = 0;
	स्थिर अक्षर *object_ref;
	काष्ठा notes_वृक्ष *t;
	काष्ठा object_id object, new_note;
	स्थिर काष्ठा object_id *note;
	अक्षर *logmsg;
	स्थिर अक्षर * स्थिर *usage;
	काष्ठा note_तथ्य d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	काष्ठा option options[] = {
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

	अगर (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	अगर (d.संदेश_nr) {
		concat_messages(&d);
		अगर (edit)
			ख_लिखो(मानक_त्रुटि, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	अगर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_तथ्य(&object, &d, edit && note ? note : शून्य);

	अगर (note && !edit) {
		/* Append buf to previous note contents */
		माप_प्रकार आकार;
		क्रमागत object_type type;
		काष्ठा strbuf buf = STRBUF_INIT;
		अक्षर *पूर्व_buf = odb_read_object(the_repository->objects, note, &type, &आकार);

		अगर (!पूर्व_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		अगर (आकार)
			strbuf_add(&buf, पूर्व_buf, आकार);
		अगर (d.buf.len && आकार)
			append_separator(&buf);
		strbuf_जोड़ो(&d.buf, 0, buf.buf, buf.len);

		मुक्त(पूर्व_buf);
		strbuf_release(&buf);
	}

	अगर (d.buf.len || allow_empty) {
		write_note_तथ्य(&d, &new_note);
		अगर (add_note(t, &object, &new_note, combine_notes_overwrite))
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
	free_note_तथ्य(&d);
	free_notes(t);
	वापस 0;
}

अटल पूर्णांक show(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		काष्ठा repository *repo UNUSED)
{
	स्थिर अक्षर *object_ref;
	काष्ठा notes_वृक्ष *t;
	काष्ठा object_id object;
	स्थिर काष्ठा object_id *note;
	पूर्णांक retval;
	काष्ठा option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	अगर (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	अगर (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	अगर (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	अन्यथा {
		स्थिर अक्षर *show_args[3] = {"show", oid_to_hex(note), शून्य};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	वापस retval;
}

अटल पूर्णांक merge_abort(काष्ठा notes_merge_options *o)
{
	पूर्णांक ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	अगर (refs_delete_ref(get_main_ref_store(the_repository), शून्य, "NOTES_MERGE_PARTIAL", शून्य, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	अगर (refs_delete_ref(get_main_ref_store(the_repository), शून्य, "NOTES_MERGE_REF", शून्य, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	अगर (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	वापस ret;
}

अटल पूर्णांक merge_commit(काष्ठा notes_merge_options *o)
{
	काष्ठा strbuf संदेश = STRBUF_INIT;
	काष्ठा object_id oid, parent_oid;
	काष्ठा notes_वृक्ष t = {0};
	काष्ठा commit *partial;
	काष्ठा pretty_print_context pretty_ctx;
	व्योम *local_ref_to_free;
	पूर्णांक ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	अगर (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	अन्यथा अगर (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	अन्यथा अगर (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	अगर (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	अन्यथा
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, शून्य);
	अगर (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	अगर (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	स_रखो(&pretty_ctx, 0, माप(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &संदेश,
				   &pretty_ctx);
	strbuf_trim(&संदेश);
	strbuf_insertstr(&संदेश, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? शून्य : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&संदेश);
	ret = merge_abort(o);
	मुक्त(local_ref_to_free);
	वापस ret;
}

अटल पूर्णांक git_config_get_notes_strategy(स्थिर अक्षर *कुंजी,
					 क्रमागत notes_merge_strategy *strategy)
{
	अक्षर *मूल्य;

	अगर (repo_config_get_string(the_repository, कुंजी, &मूल्य))
		वापस 1;
	अगर (parse_notes_merge_strategy(मूल्य, strategy))
		git_die_config(the_repository, कुंजी, _("unknown notes merge strategy %s"), मूल्य);

	मुक्त(मूल्य);
	वापस 0;
}

अटल पूर्णांक merge(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा strbuf remote_ref = STRBUF_INIT, संदेश = STRBUF_INIT;
	काष्ठा object_id परिणाम_oid;
	काष्ठा notes_वृक्ष *t;
	काष्ठा notes_merge_options o;
	पूर्णांक do_merge = 0, do_commit = 0, do_abort = 0;
	पूर्णांक verbosity = 0, परिणाम;
	स्थिर अक्षर *strategy = शून्य;
	काष्ठा option options[] = {
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

	अगर (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	अगर (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	अगर (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} अन्यथा अगर (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	अगर (do_abort)
		वापस merge_abort(&o);
	अगर (do_commit)
		वापस merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	अगर (strategy) {
		अगर (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} अन्यथा {
		काष्ठा strbuf merge_कुंजी = STRBUF_INIT;
		स्थिर अक्षर *short_ref = शून्य;

		अगर (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_कुंजी, "notes.%s.mergeStrategy", short_ref);

		अगर (git_config_get_notes_strategy(merge_कुंजी.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_कुंजी);
	}

	strbuf_addf(&संदेश, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_संदेश), संदेश.buf + 7, संदेश.len - 7); /* skip "notes: " */

	परिणाम = notes_merge(&o, t, &परिणाम_oid);

	अगर (परिणाम >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				notes_ref, &परिणाम_oid, शून्य, 0,
				UPDATE_REFS_DIE_ON_ERR);
	अन्यथा { /* Merge has unresolved conflicts */
		काष्ठा worktree **worktrees;
		स्थिर काष्ठा worktree *wt;
		अक्षर *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), संदेश.buf,
				"NOTES_MERGE_PARTIAL", &परिणाम_oid, शून्य,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		अगर (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		अगर (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, शून्य))
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
	वापस परिणाम < 0; /* return non-zero on conflicts */
}

#घोषणा IGNORE_MISSING 1

अटल पूर्णांक remove_one_note(काष्ठा notes_वृक्ष *t, स्थिर अक्षर *name, अचिन्हित flag)
{
	पूर्णांक status;
	काष्ठा object_id oid;
	अगर (repo_get_oid(the_repository, name, &oid))
		वापस error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	अगर (status)
		ख_लिखो(मानक_त्रुटि, _("Object %s has no note\n"), name);
	अन्यथा
		ख_लिखो(मानक_त्रुटि, _("Removing note for object %s\n"), name);
	वापस (flag & IGNORE_MISSING) ? 0 : status;
}

अटल पूर्णांक remove_cmd(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		      काष्ठा repository *repo UNUSED)
{
	अचिन्हित flag = 0;
	पूर्णांक from_stdin = 0;
	काष्ठा option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	काष्ठा notes_वृक्ष *t;
	पूर्णांक retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	अगर (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} अन्यथा {
		जबतक (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	अगर (from_stdin) {
		काष्ठा strbuf sb = STRBUF_INIT;
		जबतक (strbuf_getwholeline(&sb, मानक_निवेश, '\n') != खातापूर्ण) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	अगर (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	वापस retval;
}

अटल पूर्णांक prune(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		 काष्ठा repository *repo UNUSED)
{
	काष्ठा notes_वृक्ष *t;
	पूर्णांक show_only = 0, verbose = 0;
	काष्ठा option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	अगर (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	अगर (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	वापस 0;
}

अटल पूर्णांक get_ref(पूर्णांक argc, स्थिर अक्षर **argv, स्थिर अक्षर *prefix,
		   काष्ठा repository *repo UNUSED)
{
	काष्ठा option options[] = { OPT_END() };
	अक्षर *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	अगर (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	माला_दो(notes_ref);
	मुक्त(notes_ref);
	वापस 0;
}

पूर्णांक cmd_notes(पूर्णांक argc,
	      स्थिर अक्षर **argv,
	      स्थिर अक्षर *prefix,
	      काष्ठा repository *repo)
{
	स्थिर अक्षर *override_notes_ref = शून्य;
	parse_opt_subcommand_fn *fn = शून्य;
	काष्ठा option options[] = {
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

	repo_config(the_repository, git_default_config, शून्य);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	अगर (!fn) {
		अगर (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	अगर (override_notes_ref) {
		काष्ठा strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	वापस !!fn(argc, argv, prefix, repo);
}
