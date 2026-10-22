/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "editor.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "hex.h"
#અન્તર્ભાવ "notes.h"
#અન્તર્ભાવ "object-file.h"
#અન્તર્ભાવ "object-name.h"
#અન્તર્ભાવ "odb.h"
#અન્તર્ભાવ "path.h"

#અન્તર્ભાવ "pretty.h"
#અન્તર્ભાવ "refs.h"
#અન્તર્ભાવ "exec-cmd.h"
#અન્તર્ભાવ "run-command.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "notes-merge.h"
#અન્તર્ભાવ "notes-utils.h"
#અન્તર્ભાવ "worktree.h"
#અન્તર્ભાવ "write-or-die.h"

સ્થિર અચલ અક્ષર *separator = "\n";
સ્થિર અચલ અક્ષર * અચલ git_notes_usage[] = {
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
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ખાલી
};

સ્થિર અચલ અક્ષર * અચલ git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ખાલી
};

સ્થિર અચલ અક્ષર note_template[] =
	N_("Write/edit the notes for the following object:");

ગણના notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

સંરચના note_સંદેશ {
	ગણના notes_stripspace stripspace;
	સંરચના strbuf buf;
};

સંરચના note_data {
	પૂર્ણાંક use_editor;
	પૂર્ણાંક stripspace;
	અક્ષર *edit_path;
	સંરચના strbuf buf;
	સંરચના note_સંદેશ **messages;
	size_t સંદેશ_nr;
	size_t સંદેશ_alloc;
};

સ્થિર શૂન્ય free_note_data(સંરચના note_data *d)
{
	જો (d->edit_path) {
		unlink_or_warn(d->edit_path);
		મુક્ત(d->edit_path);
	}
	strbuf_release(&d->buf);

	જ્યારે (d->સંદેશ_nr--) {
		strbuf_release(&d->messages[d->સંદેશ_nr]->buf);
		મુક્ત(d->messages[d->સંદેશ_nr]);
	}
	મુક્ત(d->messages);
}

સ્થિર પૂર્ણાંક list_each_note(અચલ સંરચના object_id *object_oid,
			  અચલ સંરચના object_id *note_oid,
			  અક્ષર *note_path UNUSED,
			  શૂન્ય *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	પાછા 0;
}

સ્થિર શૂન્ય copy_obj_to_fd(પૂર્ણાંક fd, અચલ સંરચના object_id *oid)
{
	size_t size;
	ગણના object_type type;
	અક્ષર *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	જો (buf) {
		જો (size)
			write_or_die(fd, buf, size);
		મુક્ત(buf);
	}
}

સ્થિર શૂન્ય write_commented_object(પૂર્ણાંક fd, અચલ સંરચના object_id *object)
{
	સંરચના child_process show = CHILD_PROCESS_INIT;
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), ખાલી);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	જો (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	જો (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_લીટીઓ(&cbuf, buf.buf, buf.len, comment_લીટી_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	જો (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

સ્થિર શૂન્ય prepare_note_data(અચલ સંરચના object_id *object, સંરચના note_data *d,
		અચલ સંરચના object_id *old_note)
{
	જો (d->use_editor || !d->સંદેશ_nr) {
		પૂર્ણાંક fd;
		સંરચના strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		જો (d->સંદેશ_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		નહીંતર જો (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_લીટીઓ(&buf, "\n", strlen("\n"), comment_લીટી_str);
		strbuf_add_commented_લીટીઓ(&buf, _(note_template), strlen(_(note_template)),
					   comment_લીટી_str);
		strbuf_add_commented_લીટીઓ(&buf, "\n", strlen("\n"), comment_લીટી_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		જો (launch_editor(d->edit_path, &d->buf, ખાલી)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		જો (d->stripspace)
			strbuf_stripspace(&d->buf, comment_લીટી_str);
	}
}

સ્થિર શૂન્ય write_note_data(સંરચના note_data *d, સંરચના object_id *oid)
{
	જો (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		પૂર્ણાંક status = die_message(_("unable to write note object"));

		જો (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

સ્થિર શૂન્ય append_separator(સંરચના strbuf *message)
{
	size_t sep_len = 0;

	જો (!separator)
		પાછા;
	નહીંતર જો ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	નહીંતર
		strbuf_addf(message, "%s%s", separator, "\n");
}

સ્થિર શૂન્ય concat_messages(સંરચના note_data *d)
{
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	size_t i;

	માટે (i = 0; i < d->સંદેશ_nr ; i++) {
		જો (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&સંદેશ, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &સંદેશ);
		જો ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ખાલી);
		strbuf_reset(&સંદેશ);
	}
	strbuf_release(&સંદેશ);
}

સ્થિર પૂર્ણાંક parse_સંદેશ_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના note_data *d = opt->કિંમત;
	સંરચના note_સંદેશ *સંદેશ = xmalloc(કદ(*સંદેશ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&સંદેશ->buf, strlen(arg));
	strbuf_addstr(&સંદેશ->buf, arg);
	ALLOC_GROW_BY(d->messages, d->સંદેશ_nr, 1, d->સંદેશ_alloc);
	d->messages[d->સંદેશ_nr - 1] = સંદેશ;
	સંદેશ->stripspace = STRIPSPACE;
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_file_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના note_data *d = opt->કિંમત;
	સંરચના note_સંદેશ *સંદેશ = xmalloc(કદ(*સંદેશ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&સંદેશ->buf , 0);
	જો (!strcmp(arg, "-")) {
		જો (strbuf_read(&સંદેશ->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} નહીંતર જો (strbuf_read_file(&સંદેશ->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->સંદેશ_nr, 1, d->સંદેશ_alloc);
	d->messages[d->સંદેશ_nr - 1] = સંદેશ;
	સંદેશ->stripspace = STRIPSPACE;
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_reuse_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના note_data *d = opt->કિંમત;
	સંરચના note_સંદેશ *સંદેશ = xmalloc(કદ(*સંદેશ));
	અક્ષર *કિંમત;
	સંરચના object_id object;
	ગણના object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&સંદેશ->buf, 0);
	જો (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	જો (!(કિંમત = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	જો (type != OBJ_BLOB) {
		strbuf_release(&સંદેશ->buf);
		મુક્ત(કિંમત);
		મુક્ત(સંદેશ);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&સંદેશ->buf, કિંમત, len);
	મુક્ત(કિંમત);

	સંદેશ->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->સંદેશ_nr, 1, d->સંદેશ_alloc);
	d->messages[d->સંદેશ_nr - 1] = સંદેશ;
	સંદેશ->stripspace = NO_STRIPSPACE;
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_reedit_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના note_data *d = opt->કિંમત;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	પાછા parse_reuse_arg(opt, arg, unset);
}

સ્થિર પૂર્ણાંક parse_separator_arg(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			       પૂર્ણાંક unset)
{
	જો (unset)
		*(અચલ અક્ષર **)opt->કિંમત = ખાલી;
	નહીંતર
		*(અચલ અક્ષર **)opt->કિંમત = arg ? arg : "\n";
	પાછા 0;
}

સ્થિર પૂર્ણાંક notes_copy_from_stdin(પૂર્ણાંક force, અચલ અક્ષર *rewrite_cmd)
{
	સંરચના strbuf buf = STRBUF_INIT;
	સંરચના notes_rewrite_cfg *c = ખાલી;
	સંરચના notes_વૃક્ષ *t = ખાલી;
	પૂર્ણાંક ret = 0;
	અચલ અક્ષર *સંદેશ = "Notes added by 'git notes copy'";

	જો (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		જો (!c)
			પાછા 0;
	} નહીંતર {
		init_notes(ખાલી, ખાલી, ખાલી, NOTES_INIT_WRITABLE);
		t = &default_notes_વૃક્ષ;
	}

	જ્યારે (strbuf_getline_lf(&buf, stdin) != EOF) {
		સંરચના object_id from_obj, to_obj;
		સંરચના string_list split = STRING_LIST_INIT_NODUP;
		પૂર્ણાંક err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		જો (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		જો (repo_get_oid(the_repository, split.વસ્તુઓ[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.વસ્તુઓ[0].string);
		જો (repo_get_oid(the_repository, split.વસ્તુઓ[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.વસ્તુઓ[1].string);

		જો (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		નહીંતર
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		જો (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.વસ્તુઓ[0].string, split.વસ્તુઓ[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	જો (!rewrite_cmd) {
		commit_notes(the_repository, t, સંદેશ);
		free_notes(t);
	} નહીંતર {
		finish_copy_notes_for_rewrite(the_repository, c, સંદેશ);
	}
	strbuf_release(&buf);
	પાછા ret;
}

સ્થિર સંરચના notes_વૃક્ષ *init_notes_check(અચલ અક્ષર *subcommand,
					   પૂર્ણાંક flags)
{
	સંરચના notes_વૃક્ષ *t;
	અચલ અક્ષર *ref;
	init_notes(ખાલી, ખાલી, ખાલી, flags);
	t = &default_notes_વૃક્ષ;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	જો (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	પાછા t;
}

સ્થિર પૂર્ણાંક list(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	સંરચના notes_વૃક્ષ *t;
	સંરચના object_id object;
	અચલ સંરચના object_id *note;
	પૂર્ણાંક retval = -1;
	સંરચના option options[] = {
		OPT_END()
	};

	જો (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	જો (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	જો (argc) {
		જો (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		જો (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} નહીંતર
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} નહીંતર
		retval = for_each_note(t, 0, list_each_note, ખાલી);

	free_notes(t);
	પાછા retval;
}

સ્થિર પૂર્ણાંક append_edit(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED);

સ્થિર પૂર્ણાંક add(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
	       સંરચના repository *repo)
{
	પૂર્ણાંક force = 0, allow_empty = 0;
	અચલ અક્ષર *object_ref;
	સંરચના notes_વૃક્ષ *t;
	સંરચના object_id object, new_note;
	અચલ સંરચના object_id *note;
	સંરચના note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	સંરચના option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_સંદેશ_arg),
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

	જો (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	જો (d.સંદેશ_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	જો (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	જો (note) {
		જો (!force) {
			free_notes(t);
			જો (d.સંદેશ_nr) {
				free_note_data(&d);
				પાછા error(_("Cannot add notes. "
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
			પાછા append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	જો (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		જો (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} નહીંતર {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	પાછા 0;
}

સ્થિર પૂર્ણાંક copy(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક retval = 0, force = 0, from_stdin = 0;
	અચલ સંરચના object_id *from_note, *note;
	અચલ અક્ષર *object_ref;
	સંરચના object_id object, from_obj;
	સંરચના notes_વૃક્ષ *t;
	અચલ અક્ષર *rewrite_cmd = ખાલી;
	સંરચના option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	જો (from_stdin || rewrite_cmd) {
		જો (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} નહીંતર {
			પાછા notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	જો (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	જો (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	જો (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	જો (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	જો (note) {
		જો (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			જાઓ out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	જો (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		જાઓ out;
	}

	જો (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	પાછા retval;
}

સ્થિર પૂર્ણાંક append_edit(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		       સંરચના repository *repo UNUSED)
{
	પૂર્ણાંક allow_empty = 0;
	અચલ અક્ષર *object_ref;
	સંરચના notes_વૃક્ષ *t;
	સંરચના object_id object, new_note;
	અચલ સંરચના object_id *note;
	અક્ષર *logmsg;
	અચલ અક્ષર * અચલ *usage;
	સંરચના note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	સંરચના option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_સંદેશ_arg),
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
	પૂર્ણાંક edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	જો (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	જો (d.સંદેશ_nr) {
		concat_messages(&d);
		જો (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	જો (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : ખાલી);

	જો (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		ગણના object_type type;
		સંરચના strbuf buf = STRBUF_INIT;
		અક્ષર *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		જો (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		જો (size)
			strbuf_add(&buf, prev_buf, size);
		જો (d.buf.len && size)
			append_separator(&buf);
		strbuf_ઉમેરો(&d.buf, 0, buf.buf, buf.len);

		મુક્ત(prev_buf);
		strbuf_release(&buf);
	}

	જો (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		જો (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} નહીંતર {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	મુક્ત(logmsg);
	free_note_data(&d);
	free_notes(t);
	પાછા 0;
}

સ્થિર પૂર્ણાંક show(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		સંરચના repository *repo UNUSED)
{
	અચલ અક્ષર *object_ref;
	સંરચના notes_વૃક્ષ *t;
	સંરચના object_id object;
	અચલ સંરચના object_id *note;
	પૂર્ણાંક retval;
	સંરચના option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	જો (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	જો (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	જો (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	નહીંતર {
		અચલ અક્ષર *show_args[3] = {"show", oid_to_hex(note), ખાલી};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	પાછા retval;
}

સ્થિર પૂર્ણાંક merge_abort(સંરચના notes_merge_options *o)
{
	પૂર્ણાંક ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	જો (refs_delete_ref(get_main_ref_store(the_repository), ખાલી, "NOTES_MERGE_PARTIAL", ખાલી, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	જો (refs_delete_ref(get_main_ref_store(the_repository), ખાલી, "NOTES_MERGE_REF", ખાલી, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	જો (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	પાછા ret;
}

સ્થિર પૂર્ણાંક merge_commit(સંરચના notes_merge_options *o)
{
	સંરચના strbuf સંદેશ = STRBUF_INIT;
	સંરચના object_id oid, parent_oid;
	સંરચના notes_વૃક્ષ t = {0};
	સંરચના commit *partial;
	સંરચના pretty_print_context pretty_ctx;
	શૂન્ય *local_ref_to_free;
	પૂર્ણાંક ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	જો (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	નહીંતર જો (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	નહીંતર જો (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	જો (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	નહીંતર
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ખાલી);
	જો (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	જો (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, કદ(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &સંદેશ,
				   &pretty_ctx);
	strbuf_trim(&સંદેશ);
	strbuf_insertstr(&સંદેશ, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), સંદેશ.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? ખાલી : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&સંદેશ);
	ret = merge_abort(o);
	મુક્ત(local_ref_to_free);
	પાછા ret;
}

સ્થિર પૂર્ણાંક git_config_get_notes_strategy(અચલ અક્ષર *ચાવી,
					 ગણના notes_merge_strategy *strategy)
{
	અક્ષર *કિંમત;

	જો (repo_config_get_string(the_repository, ચાવી, &કિંમત))
		પાછા 1;
	જો (parse_notes_merge_strategy(કિંમત, strategy))
		git_die_config(the_repository, ચાવી, _("unknown notes merge strategy %s"), કિંમત);

	મુક્ત(કિંમત);
	પાછા 0;
}

સ્થિર પૂર્ણાંક merge(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	સંરચના strbuf remote_ref = STRBUF_INIT, સંદેશ = STRBUF_INIT;
	સંરચના object_id result_oid;
	સંરચના notes_વૃક્ષ *t;
	સંરચના notes_merge_options o;
	પૂર્ણાંક do_merge = 0, do_commit = 0, do_abort = 0;
	પૂર્ણાંક verbosity = 0, result;
	અચલ અક્ષર *strategy = ખાલી;
	સંરચના option options[] = {
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
	અક્ષર *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	જો (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	જો (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	જો (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} નહીંતર જો (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	જો (do_abort)
		પાછા merge_abort(&o);
	જો (do_commit)
		પાછા merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	જો (strategy) {
		જો (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} નહીંતર {
		સંરચના strbuf merge_ચાવી = STRBUF_INIT;
		અચલ અક્ષર *short_ref = ખાલી;

		જો (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_ચાવી, "notes.%s.mergeStrategy", short_ref);

		જો (git_config_get_notes_strategy(merge_ચાવી.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_ચાવી);
	}

	strbuf_addf(&સંદેશ, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_સંદેશ), સંદેશ.buf + 7, સંદેશ.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	જો (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ.buf,
				notes_ref, &result_oid, ખાલી, 0,
				UPDATE_REFS_DIE_ON_ERR);
	નહીંતર { /* Merge has unresolved conflicts */
		સંરચના worktree **worktrees;
		અચલ સંરચના worktree *wt;
		અક્ષર *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), સંદેશ.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, ખાલી,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		જો (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		જો (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ખાલી))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		મુક્ત(path);
	}

	free_notes(t);
	મુક્ત(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&સંદેશ);
	પાછા result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

સ્થિર પૂર્ણાંક remove_one_note(સંરચના notes_વૃક્ષ *t, અચલ અક્ષર *name, unsigned flag)
{
	પૂર્ણાંક status;
	સંરચના object_id oid;
	જો (repo_get_oid(the_repository, name, &oid))
		પાછા error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	જો (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	નહીંતર
		fprintf(stderr, _("Removing note for object %s\n"), name);
	પાછા (flag & IGNORE_MISSING) ? 0 : status;
}

સ્થિર પૂર્ણાંક remove_cmd(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		      સંરચના repository *repo UNUSED)
{
	unsigned flag = 0;
	પૂર્ણાંક from_stdin = 0;
	સંરચના option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	સંરચના notes_વૃક્ષ *t;
	પૂર્ણાંક retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	જો (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} નહીંતર {
		જ્યારે (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	જો (from_stdin) {
		સંરચના strbuf sb = STRBUF_INIT;
		જ્યારે (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	જો (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	પાછા retval;
}

સ્થિર પૂર્ણાંક prune(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		 સંરચના repository *repo UNUSED)
{
	સંરચના notes_વૃક્ષ *t;
	પૂર્ણાંક show_only = 0, verbose = 0;
	સંરચના option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	જો (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	જો (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	પાછા 0;
}

સ્થિર પૂર્ણાંક get_ref(પૂર્ણાંક argc, અચલ અક્ષર **argv, અચલ અક્ષર *prefix,
		   સંરચના repository *repo UNUSED)
{
	સંરચના option options[] = { OPT_END() };
	અક્ષર *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	જો (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	મુક્ત(notes_ref);
	પાછા 0;
}

પૂર્ણાંક cmd_notes(પૂર્ણાંક argc,
	      અચલ અક્ષર **argv,
	      અચલ અક્ષર *prefix,
	      સંરચના repository *repo)
{
	અચલ અક્ષર *override_notes_ref = ખાલી;
	parse_opt_subcommand_fn *fn = ખાલી;
	સંરચના option options[] = {
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

	repo_config(the_repository, git_default_config, ખાલી);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	જો (!fn) {
		જો (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	જો (override_notes_ref) {
		સંરચના strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	પાછા !!fn(argc, argv, prefix, repo);
}
