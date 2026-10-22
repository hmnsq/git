/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "editor.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "hex.h"
#അന്തര്ഭാവ "notes.h"
#അന്തര്ഭാവ "object-file.h"
#അന്തര്ഭാവ "object-name.h"
#അന്തര്ഭാവ "odb.h"
#അന്തര്ഭാവ "path.h"

#അന്തര്ഭാവ "pretty.h"
#അന്തര്ഭാവ "refs.h"
#അന്തര്ഭാവ "exec-cmd.h"
#അന്തര്ഭാവ "run-command.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "notes-merge.h"
#അന്തര്ഭാവ "notes-utils.h"
#അന്തര്ഭാവ "worktree.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം *separator = "\n";
സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_usage[] = {
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
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ശൂന്യ
};

സ്ഥിതം സ്ഥിരം അക്ഷരം note_template[] =
	N_("Write/edit the notes for the following object:");

എണ്ണൽ notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

ഘടന note_സന്ദേശം {
	എണ്ണൽ notes_stripspace stripspace;
	ഘടന strbuf buf;
};

ഘടന note_data {
	പൂർണ്ണസംഖ്യ use_editor;
	പൂർണ്ണസംഖ്യ stripspace;
	അക്ഷരം *edit_path;
	ഘടന strbuf buf;
	ഘടന note_സന്ദേശം **messages;
	size_t സന്ദേശം_nr;
	size_t സന്ദേശം_alloc;
};

സ്ഥിതം ശൂന്യം free_note_data(ഘടന note_data *d)
{
	എങ്കിൽ (d->edit_path) {
		unlink_or_warn(d->edit_path);
		സ്വതന്ത്രം(d->edit_path);
	}
	strbuf_release(&d->buf);

	ആകുമ്പോൾ (d->സന്ദേശം_nr--) {
		strbuf_release(&d->messages[d->സന്ദേശം_nr]->buf);
		സ്വതന്ത്രം(d->messages[d->സന്ദേശം_nr]);
	}
	സ്വതന്ത്രം(d->messages);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ list_each_note(സ്ഥിരം ഘടന object_id *object_oid,
			  സ്ഥിരം ഘടന object_id *note_oid,
			  അക്ഷരം *note_path UNUSED,
			  ശൂന്യം *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം copy_obj_to_fd(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന object_id *oid)
{
	size_t size;
	എണ്ണൽ object_type type;
	അക്ഷരം *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	എങ്കിൽ (buf) {
		എങ്കിൽ (size)
			write_or_die(fd, buf, size);
		സ്വതന്ത്രം(buf);
	}
}

സ്ഥിതം ശൂന്യം write_commented_object(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന object_id *object)
{
	ഘടന child_process show = CHILD_PROCESS_INIT;
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), ശൂന്യ);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	എങ്കിൽ (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	എങ്കിൽ (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_വരികൾ(&cbuf, buf.buf, buf.len, comment_വരി_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	എങ്കിൽ (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

സ്ഥിതം ശൂന്യം prepare_note_data(സ്ഥിരം ഘടന object_id *object, ഘടന note_data *d,
		സ്ഥിരം ഘടന object_id *old_note)
{
	എങ്കിൽ (d->use_editor || !d->സന്ദേശം_nr) {
		പൂർണ്ണസംഖ്യ fd;
		ഘടന strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		എങ്കിൽ (d->സന്ദേശം_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		അല്ലെങ്കിൽ എങ്കിൽ (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_വരികൾ(&buf, "\n", strlen("\n"), comment_വരി_str);
		strbuf_add_commented_വരികൾ(&buf, _(note_template), strlen(_(note_template)),
					   comment_വരി_str);
		strbuf_add_commented_വരികൾ(&buf, "\n", strlen("\n"), comment_വരി_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		എങ്കിൽ (launch_editor(d->edit_path, &d->buf, ശൂന്യ)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		എങ്കിൽ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_വരി_str);
	}
}

സ്ഥിതം ശൂന്യം write_note_data(ഘടന note_data *d, ഘടന object_id *oid)
{
	എങ്കിൽ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		പൂർണ്ണസംഖ്യ status = die_message(_("unable to write note object"));

		എങ്കിൽ (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

സ്ഥിതം ശൂന്യം append_separator(ഘടന strbuf *message)
{
	size_t sep_len = 0;

	എങ്കിൽ (!separator)
		തിരികെ;
	അല്ലെങ്കിൽ എങ്കിൽ ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	അല്ലെങ്കിൽ
		strbuf_addf(message, "%s%s", separator, "\n");
}

സ്ഥിതം ശൂന്യം concat_messages(ഘടന note_data *d)
{
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	size_t i;

	വേണ്ടി (i = 0; i < d->സന്ദേശം_nr ; i++) {
		എങ്കിൽ (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&സന്ദേശം, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &സന്ദേശം);
		എങ്കിൽ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ശൂന്യ);
		strbuf_reset(&സന്ദേശം);
	}
	strbuf_release(&സന്ദേശം);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_സന്ദേശം_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന note_data *d = opt->മൂല്യം;
	ഘടന note_സന്ദേശം *സന്ദേശം = xmalloc(വലുപ്പം(*സന്ദേശം));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&സന്ദേശം->buf, strlen(arg));
	strbuf_addstr(&സന്ദേശം->buf, arg);
	ALLOC_GROW_BY(d->messages, d->സന്ദേശം_nr, 1, d->സന്ദേശം_alloc);
	d->messages[d->സന്ദേശം_nr - 1] = സന്ദേശം;
	സന്ദേശം->stripspace = STRIPSPACE;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_file_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന note_data *d = opt->മൂല്യം;
	ഘടന note_സന്ദേശം *സന്ദേശം = xmalloc(വലുപ്പം(*സന്ദേശം));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&സന്ദേശം->buf , 0);
	എങ്കിൽ (!strcmp(arg, "-")) {
		എങ്കിൽ (strbuf_read(&സന്ദേശം->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} അല്ലെങ്കിൽ എങ്കിൽ (strbuf_read_file(&സന്ദേശം->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->സന്ദേശം_nr, 1, d->സന്ദേശം_alloc);
	d->messages[d->സന്ദേശം_nr - 1] = സന്ദേശം;
	സന്ദേശം->stripspace = STRIPSPACE;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_reuse_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന note_data *d = opt->മൂല്യം;
	ഘടന note_സന്ദേശം *സന്ദേശം = xmalloc(വലുപ്പം(*സന്ദേശം));
	അക്ഷരം *മൂല്യം;
	ഘടന object_id object;
	എണ്ണൽ object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&സന്ദേശം->buf, 0);
	എങ്കിൽ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	എങ്കിൽ (!(മൂല്യം = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	എങ്കിൽ (type != OBJ_BLOB) {
		strbuf_release(&സന്ദേശം->buf);
		സ്വതന്ത്രം(മൂല്യം);
		സ്വതന്ത്രം(സന്ദേശം);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&സന്ദേശം->buf, മൂല്യം, len);
	സ്വതന്ത്രം(മൂല്യം);

	സന്ദേശം->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->സന്ദേശം_nr, 1, d->സന്ദേശം_alloc);
	d->messages[d->സന്ദേശം_nr - 1] = സന്ദേശം;
	സന്ദേശം->stripspace = NO_STRIPSPACE;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_reedit_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന note_data *d = opt->മൂല്യം;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	തിരികെ parse_reuse_arg(opt, arg, unset);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_separator_arg(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			       പൂർണ്ണസംഖ്യ unset)
{
	എങ്കിൽ (unset)
		*(സ്ഥിരം അക്ഷരം **)opt->മൂല്യം = ശൂന്യ;
	അല്ലെങ്കിൽ
		*(സ്ഥിരം അക്ഷരം **)opt->മൂല്യം = arg ? arg : "\n";
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ notes_copy_from_stdin(പൂർണ്ണസംഖ്യ force, സ്ഥിരം അക്ഷരം *rewrite_cmd)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന notes_rewrite_cfg *c = ശൂന്യ;
	ഘടന notes_വൃക്ഷം *t = ശൂന്യ;
	പൂർണ്ണസംഖ്യ ret = 0;
	സ്ഥിരം അക്ഷരം *സന്ദേശം = "Notes added by 'git notes copy'";

	എങ്കിൽ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		എങ്കിൽ (!c)
			തിരികെ 0;
	} അല്ലെങ്കിൽ {
		init_notes(ശൂന്യ, ശൂന്യ, ശൂന്യ, NOTES_INIT_WRITABLE);
		t = &default_notes_വൃക്ഷം;
	}

	ആകുമ്പോൾ (strbuf_getline_lf(&buf, stdin) != EOF) {
		ഘടന object_id from_obj, to_obj;
		ഘടന string_list split = STRING_LIST_INIT_NODUP;
		പൂർണ്ണസംഖ്യ err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		എങ്കിൽ (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		എങ്കിൽ (repo_get_oid(the_repository, split.ഇനങ്ങൾ[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.ഇനങ്ങൾ[0].string);
		എങ്കിൽ (repo_get_oid(the_repository, split.ഇനങ്ങൾ[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.ഇനങ്ങൾ[1].string);

		എങ്കിൽ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		അല്ലെങ്കിൽ
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		എങ്കിൽ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.ഇനങ്ങൾ[0].string, split.ഇനങ്ങൾ[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	എങ്കിൽ (!rewrite_cmd) {
		commit_notes(the_repository, t, സന്ദേശം);
		free_notes(t);
	} അല്ലെങ്കിൽ {
		finish_copy_notes_for_rewrite(the_repository, c, സന്ദേശം);
	}
	strbuf_release(&buf);
	തിരികെ ret;
}

സ്ഥിതം ഘടന notes_വൃക്ഷം *init_notes_check(സ്ഥിരം അക്ഷരം *subcommand,
					   പൂർണ്ണസംഖ്യ flags)
{
	ഘടന notes_വൃക്ഷം *t;
	സ്ഥിരം അക്ഷരം *ref;
	init_notes(ശൂന്യ, ശൂന്യ, ശൂന്യ, flags);
	t = &default_notes_വൃക്ഷം;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	എങ്കിൽ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	തിരികെ t;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ list(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	ഘടന notes_വൃക്ഷം *t;
	ഘടന object_id object;
	സ്ഥിരം ഘടന object_id *note;
	പൂർണ്ണസംഖ്യ retval = -1;
	ഘടന option options[] = {
		OPT_END()
	};

	എങ്കിൽ (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	എങ്കിൽ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	എങ്കിൽ (argc) {
		എങ്കിൽ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		എങ്കിൽ (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} അല്ലെങ്കിൽ
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} അല്ലെങ്കിൽ
		retval = for_each_note(t, 0, list_each_note, ശൂന്യ);

	free_notes(t);
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_edit(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED);

സ്ഥിതം പൂർണ്ണസംഖ്യ add(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
	       ഘടന repository *repo)
{
	പൂർണ്ണസംഖ്യ force = 0, allow_empty = 0;
	സ്ഥിരം അക്ഷരം *object_ref;
	ഘടന notes_വൃക്ഷം *t;
	ഘടന object_id object, new_note;
	സ്ഥിരം ഘടന object_id *note;
	ഘടന note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	ഘടന option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_സന്ദേശം_arg),
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

	എങ്കിൽ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	എങ്കിൽ (d.സന്ദേശം_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	എങ്കിൽ (note) {
		എങ്കിൽ (!force) {
			free_notes(t);
			എങ്കിൽ (d.സന്ദേശം_nr) {
				free_note_data(&d);
				തിരികെ error(_("Cannot add notes. "
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
			തിരികെ append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	എങ്കിൽ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		എങ്കിൽ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} അല്ലെങ്കിൽ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ copy(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ retval = 0, force = 0, from_stdin = 0;
	സ്ഥിരം ഘടന object_id *from_note, *note;
	സ്ഥിരം അക്ഷരം *object_ref;
	ഘടന object_id object, from_obj;
	ഘടന notes_വൃക്ഷം *t;
	സ്ഥിരം അക്ഷരം *rewrite_cmd = ശൂന്യ;
	ഘടന option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	എങ്കിൽ (from_stdin || rewrite_cmd) {
		എങ്കിൽ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} അല്ലെങ്കിൽ {
			തിരികെ notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	എങ്കിൽ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	എങ്കിൽ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	എങ്കിൽ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	എങ്കിൽ (note) {
		എങ്കിൽ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			പോകുക out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	എങ്കിൽ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		പോകുക out;
	}

	എങ്കിൽ (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ append_edit(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		       ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ allow_empty = 0;
	സ്ഥിരം അക്ഷരം *object_ref;
	ഘടന notes_വൃക്ഷം *t;
	ഘടന object_id object, new_note;
	സ്ഥിരം ഘടന object_id *note;
	അക്ഷരം *logmsg;
	സ്ഥിരം അക്ഷരം * സ്ഥിരം *usage;
	ഘടന note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	ഘടന option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_സന്ദേശം_arg),
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
	പൂർണ്ണസംഖ്യ edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	എങ്കിൽ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	എങ്കിൽ (d.സന്ദേശം_nr) {
		concat_messages(&d);
		എങ്കിൽ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : ശൂന്യ);

	എങ്കിൽ (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		എണ്ണൽ object_type type;
		ഘടന strbuf buf = STRBUF_INIT;
		അക്ഷരം *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		എങ്കിൽ (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		എങ്കിൽ (size)
			strbuf_add(&buf, prev_buf, size);
		എങ്കിൽ (d.buf.len && size)
			append_separator(&buf);
		strbuf_തിരുകുക(&d.buf, 0, buf.buf, buf.len);

		സ്വതന്ത്രം(prev_buf);
		strbuf_release(&buf);
	}

	എങ്കിൽ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		എങ്കിൽ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} അല്ലെങ്കിൽ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	സ്വതന്ത്രം(logmsg);
	free_note_data(&d);
	free_notes(t);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ show(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		ഘടന repository *repo UNUSED)
{
	സ്ഥിരം അക്ഷരം *object_ref;
	ഘടന notes_വൃക്ഷം *t;
	ഘടന object_id object;
	സ്ഥിരം ഘടന object_id *note;
	പൂർണ്ണസംഖ്യ retval;
	ഘടന option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	എങ്കിൽ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	എങ്കിൽ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	എങ്കിൽ (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	അല്ലെങ്കിൽ {
		സ്ഥിരം അക്ഷരം *show_args[3] = {"show", oid_to_hex(note), ശൂന്യ};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ merge_abort(ഘടന notes_merge_options *o)
{
	പൂർണ്ണസംഖ്യ ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	എങ്കിൽ (refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ, "NOTES_MERGE_PARTIAL", ശൂന്യ, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	എങ്കിൽ (refs_delete_ref(get_main_ref_store(the_repository), ശൂന്യ, "NOTES_MERGE_REF", ശൂന്യ, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	എങ്കിൽ (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ merge_commit(ഘടന notes_merge_options *o)
{
	ഘടന strbuf സന്ദേശം = STRBUF_INIT;
	ഘടന object_id oid, parent_oid;
	ഘടന notes_വൃക്ഷം t = {0};
	ഘടന commit *partial;
	ഘടന pretty_print_context pretty_ctx;
	ശൂന്യം *local_ref_to_free;
	പൂർണ്ണസംഖ്യ ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	എങ്കിൽ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	അല്ലെങ്കിൽ എങ്കിൽ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	അല്ലെങ്കിൽ എങ്കിൽ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	എങ്കിൽ (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	അല്ലെങ്കിൽ
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ശൂന്യ);
	എങ്കിൽ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	എങ്കിൽ (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, വലുപ്പം(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &സന്ദേശം,
				   &pretty_ctx);
	strbuf_trim(&സന്ദേശം);
	strbuf_insertstr(&സന്ദേശം, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? ശൂന്യ : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&സന്ദേശം);
	ret = merge_abort(o);
	സ്വതന്ത്രം(local_ref_to_free);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ git_config_get_notes_strategy(സ്ഥിരം അക്ഷരം *താക്കോൽ,
					 എണ്ണൽ notes_merge_strategy *strategy)
{
	അക്ഷരം *മൂല്യം;

	എങ്കിൽ (repo_config_get_string(the_repository, താക്കോൽ, &മൂല്യം))
		തിരികെ 1;
	എങ്കിൽ (parse_notes_merge_strategy(മൂല്യം, strategy))
		git_die_config(the_repository, താക്കോൽ, _("unknown notes merge strategy %s"), മൂല്യം);

	സ്വതന്ത്രം(മൂല്യം);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ merge(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന strbuf remote_ref = STRBUF_INIT, സന്ദേശം = STRBUF_INIT;
	ഘടന object_id result_oid;
	ഘടന notes_വൃക്ഷം *t;
	ഘടന notes_merge_options o;
	പൂർണ്ണസംഖ്യ do_merge = 0, do_commit = 0, do_abort = 0;
	പൂർണ്ണസംഖ്യ verbosity = 0, result;
	സ്ഥിരം അക്ഷരം *strategy = ശൂന്യ;
	ഘടന option options[] = {
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
	അക്ഷരം *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	എങ്കിൽ (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	എങ്കിൽ (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	എങ്കിൽ (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} അല്ലെങ്കിൽ എങ്കിൽ (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	എങ്കിൽ (do_abort)
		തിരികെ merge_abort(&o);
	എങ്കിൽ (do_commit)
		തിരികെ merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	എങ്കിൽ (strategy) {
		എങ്കിൽ (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} അല്ലെങ്കിൽ {
		ഘടന strbuf merge_താക്കോൽ = STRBUF_INIT;
		സ്ഥിരം അക്ഷരം *short_ref = ശൂന്യ;

		എങ്കിൽ (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_താക്കോൽ, "notes.%s.mergeStrategy", short_ref);

		എങ്കിൽ (git_config_get_notes_strategy(merge_താക്കോൽ.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_താക്കോൽ);
	}

	strbuf_addf(&സന്ദേശം, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_സന്ദേശം), സന്ദേശം.buf + 7, സന്ദേശം.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	എങ്കിൽ (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം.buf,
				notes_ref, &result_oid, ശൂന്യ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	അല്ലെങ്കിൽ { /* Merge has unresolved conflicts */
		ഘടന worktree **worktrees;
		സ്ഥിരം ഘടന worktree *wt;
		അക്ഷരം *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), സന്ദേശം.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, ശൂന്യ,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		എങ്കിൽ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		എങ്കിൽ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ശൂന്യ))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		സ്വതന്ത്രം(path);
	}

	free_notes(t);
	സ്വതന്ത്രം(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&സന്ദേശം);
	തിരികെ result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

സ്ഥിതം പൂർണ്ണസംഖ്യ remove_one_note(ഘടന notes_വൃക്ഷം *t, സ്ഥിരം അക്ഷരം *name, unsigned flag)
{
	പൂർണ്ണസംഖ്യ status;
	ഘടന object_id oid;
	എങ്കിൽ (repo_get_oid(the_repository, name, &oid))
		തിരികെ error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	എങ്കിൽ (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	അല്ലെങ്കിൽ
		fprintf(stderr, _("Removing note for object %s\n"), name);
	തിരികെ (flag & IGNORE_MISSING) ? 0 : status;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ remove_cmd(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		      ഘടന repository *repo UNUSED)
{
	unsigned flag = 0;
	പൂർണ്ണസംഖ്യ from_stdin = 0;
	ഘടന option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	ഘടന notes_വൃക്ഷം *t;
	പൂർണ്ണസംഖ്യ retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	എങ്കിൽ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} അല്ലെങ്കിൽ {
		ആകുമ്പോൾ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	എങ്കിൽ (from_stdin) {
		ഘടന strbuf sb = STRBUF_INIT;
		ആകുമ്പോൾ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	എങ്കിൽ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	തിരികെ retval;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ prune(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		 ഘടന repository *repo UNUSED)
{
	ഘടന notes_വൃക്ഷം *t;
	പൂർണ്ണസംഖ്യ show_only = 0, verbose = 0;
	ഘടന option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	എങ്കിൽ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	എങ്കിൽ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_ref(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv, സ്ഥിരം അക്ഷരം *prefix,
		   ഘടന repository *repo UNUSED)
{
	ഘടന option options[] = { OPT_END() };
	അക്ഷരം *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	എങ്കിൽ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	സ്വതന്ത്രം(notes_ref);
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ cmd_notes(പൂർണ്ണസംഖ്യ argc,
	      സ്ഥിരം അക്ഷരം **argv,
	      സ്ഥിരം അക്ഷരം *prefix,
	      ഘടന repository *repo)
{
	സ്ഥിരം അക്ഷരം *override_notes_ref = ശൂന്യ;
	parse_opt_subcommand_fn *fn = ശൂന്യ;
	ഘടന option options[] = {
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

	repo_config(the_repository, git_default_config, ശൂന്യ);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	എങ്കിൽ (!fn) {
		എങ്കിൽ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	എങ്കിൽ (override_notes_ref) {
		ഘടന strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	തിരികെ !!fn(argc, argv, prefix, repo);
}
