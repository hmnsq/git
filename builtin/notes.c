/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "editor.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "hex.h"
#ಅನ್ತರ್ಭಾವ "notes.h"
#ಅನ್ತರ್ಭಾವ "object-file.h"
#ಅನ್ತರ್ಭಾವ "object-name.h"
#ಅನ್ತರ್ಭಾವ "odb.h"
#ಅನ್ತರ್ಭಾವ "path.h"

#ಅನ್ತರ್ಭಾವ "pretty.h"
#ಅನ್ತರ್ಭಾವ "refs.h"
#ಅನ್ತರ್ಭಾವ "exec-cmd.h"
#ಅನ್ತರ್ಭಾವ "run-command.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "notes-merge.h"
#ಅನ್ತರ್ಭಾವ "notes-utils.h"
#ಅನ್ತರ್ಭಾವ "worktree.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *separator = "\n";
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_usage[] = {
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
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ note_template[] =
	N_("Write/edit the notes for the following object:");

ಗಣನಾ notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

ಸಂರಚನಾ note_ಸಂದೇಶ {
	ಗಣನಾ notes_stripspace stripspace;
	ಸಂರಚನಾ strbuf buf;
};

ಸಂರಚನಾ note_data {
	ಪೂರ್ಣಾಙ್ಕ use_editor;
	ಪೂರ್ಣಾಙ್ಕ stripspace;
	ವರ್ಣ *edit_path;
	ಸಂರಚನಾ strbuf buf;
	ಸಂರಚನಾ note_ಸಂದೇಶ **messages;
	size_t ಸಂದೇಶ_nr;
	size_t ಸಂದೇಶ_alloc;
};

ಸ್ಥಿರ ಶೂನ್ಯ free_note_data(ಸಂರಚನಾ note_data *d)
{
	ಯದಿ (d->edit_path) {
		unlink_or_warn(d->edit_path);
		ಮುಕ್ತ(d->edit_path);
	}
	strbuf_release(&d->buf);

	ಯಾವತ್ (d->ಸಂದೇಶ_nr--) {
		strbuf_release(&d->messages[d->ಸಂದೇಶ_nr]->buf);
		ಮುಕ್ತ(d->messages[d->ಸಂದೇಶ_nr]);
	}
	ಮುಕ್ತ(d->messages);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list_each_note(ಧ್ರುವ ಸಂರಚನಾ object_id *object_oid,
			  ಧ್ರುವ ಸಂರಚನಾ object_id *note_oid,
			  ವರ್ಣ *note_path UNUSED,
			  ಶೂನ್ಯ *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_obj_to_fd(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ object_id *oid)
{
	size_t size;
	ಗಣನಾ object_type type;
	ವರ್ಣ *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ಯದಿ (buf) {
		ಯದಿ (size)
			write_or_die(fd, buf, size);
		ಮುಕ್ತ(buf);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_commented_object(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ object_id *object)
{
	ಸಂರಚನಾ child_process show = CHILD_PROCESS_INIT;
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), ರಿಕ್ತ);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	ಯದಿ (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	ಯದಿ (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_ಸಾಲುಗಳು(&cbuf, buf.buf, buf.len, comment_ಸಾಲು_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	ಯದಿ (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_note_data(ಧ್ರುವ ಸಂರಚನಾ object_id *object, ಸಂರಚನಾ note_data *d,
		ಧ್ರುವ ಸಂರಚನಾ object_id *old_note)
{
	ಯದಿ (d->use_editor || !d->ಸಂದೇಶ_nr) {
		ಪೂರ್ಣಾಙ್ಕ fd;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ಯದಿ (d->ಸಂದೇಶ_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		ಅನ್ಯಥಾ ಯದಿ (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_ಸಾಲುಗಳು(&buf, "\n", strlen("\n"), comment_ಸಾಲು_str);
		strbuf_add_commented_ಸಾಲುಗಳು(&buf, _(note_template), strlen(_(note_template)),
					   comment_ಸಾಲು_str);
		strbuf_add_commented_ಸಾಲುಗಳು(&buf, "\n", strlen("\n"), comment_ಸಾಲು_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		ಯದಿ (launch_editor(d->edit_path, &d->buf, ರಿಕ್ತ)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		ಯದಿ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_ಸಾಲು_str);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_note_data(ಸಂರಚನಾ note_data *d, ಸಂರಚನಾ object_id *oid)
{
	ಯದಿ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		ಪೂರ್ಣಾಙ್ಕ status = die_message(_("unable to write note object"));

		ಯದಿ (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ append_separator(ಸಂರಚನಾ strbuf *message)
{
	size_t sep_len = 0;

	ಯದಿ (!separator)
		ಪ್ರತ್ಯಾಗಮ;
	ಅನ್ಯಥಾ ಯದಿ ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	ಅನ್ಯಥಾ
		strbuf_addf(message, "%s%s", separator, "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ concat_messages(ಸಂರಚನಾ note_data *d)
{
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	size_t i;

	ಪ್ರತಿ (i = 0; i < d->ಸಂದೇಶ_nr ; i++) {
		ಯದಿ (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&ಸಂದೇಶ, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &ಸಂದೇಶ);
		ಯದಿ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ರಿಕ್ತ);
		strbuf_reset(&ಸಂದೇಶ);
	}
	strbuf_release(&ಸಂದೇಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_ಸಂದೇಶ_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ note_data *d = opt->ಮೌಲ್ಯ;
	ಸಂರಚನಾ note_ಸಂದೇಶ *ಸಂದೇಶ = xmalloc(ಪರಿಮಾಣ(*ಸಂದೇಶ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ಸಂದೇಶ->buf, strlen(arg));
	strbuf_addstr(&ಸಂದೇಶ->buf, arg);
	ALLOC_GROW_BY(d->messages, d->ಸಂದೇಶ_nr, 1, d->ಸಂದೇಶ_alloc);
	d->messages[d->ಸಂದೇಶ_nr - 1] = ಸಂದೇಶ;
	ಸಂದೇಶ->stripspace = STRIPSPACE;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_file_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ note_data *d = opt->ಮೌಲ್ಯ;
	ಸಂರಚನಾ note_ಸಂದೇಶ *ಸಂದೇಶ = xmalloc(ಪರಿಮಾಣ(*ಸಂದೇಶ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ಸಂದೇಶ->buf , 0);
	ಯದಿ (!strcmp(arg, "-")) {
		ಯದಿ (strbuf_read(&ಸಂದೇಶ->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} ಅನ್ಯಥಾ ಯದಿ (strbuf_read_file(&ಸಂದೇಶ->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->ಸಂದೇಶ_nr, 1, d->ಸಂದೇಶ_alloc);
	d->messages[d->ಸಂದೇಶ_nr - 1] = ಸಂದೇಶ;
	ಸಂದೇಶ->stripspace = STRIPSPACE;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_reuse_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ note_data *d = opt->ಮೌಲ್ಯ;
	ಸಂರಚನಾ note_ಸಂದೇಶ *ಸಂದೇಶ = xmalloc(ಪರಿಮಾಣ(*ಸಂದೇಶ));
	ವರ್ಣ *ಮೌಲ್ಯ;
	ಸಂರಚನಾ object_id object;
	ಗಣನಾ object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ಸಂದೇಶ->buf, 0);
	ಯದಿ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	ಯದಿ (!(ಮೌಲ್ಯ = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	ಯದಿ (type != OBJ_BLOB) {
		strbuf_release(&ಸಂದೇಶ->buf);
		ಮುಕ್ತ(ಮೌಲ್ಯ);
		ಮುಕ್ತ(ಸಂದೇಶ);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&ಸಂದೇಶ->buf, ಮೌಲ್ಯ, len);
	ಮುಕ್ತ(ಮೌಲ್ಯ);

	ಸಂದೇಶ->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->ಸಂದೇಶ_nr, 1, d->ಸಂದೇಶ_alloc);
	d->messages[d->ಸಂದೇಶ_nr - 1] = ಸಂದೇಶ;
	ಸಂದೇಶ->stripspace = NO_STRIPSPACE;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_reedit_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಸಂರಚನಾ note_data *d = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	ಪ್ರತ್ಯಾಗಮ parse_reuse_arg(opt, arg, unset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ parse_separator_arg(ಧ್ರುವ ಸಂರಚನಾ option *opt, ಧ್ರುವ ವರ್ಣ *arg,
			       ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಯದಿ (unset)
		*(ಧ್ರುವ ವರ್ಣ **)opt->ಮೌಲ್ಯ = ರಿಕ್ತ;
	ಅನ್ಯಥಾ
		*(ಧ್ರುವ ವರ್ಣ **)opt->ಮೌಲ್ಯ = arg ? arg : "\n";
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ notes_copy_from_stdin(ಪೂರ್ಣಾಙ್ಕ force, ಧ್ರುವ ವರ್ಣ *rewrite_cmd)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ notes_rewrite_cfg *c = ರಿಕ್ತ;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = "Notes added by 'git notes copy'";

	ಯದಿ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		ಯದಿ (!c)
			ಪ್ರತ್ಯಾಗಮ 0;
	} ಅನ್ಯಥಾ {
		init_notes(ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, NOTES_INIT_WRITABLE);
		t = &default_notes_ವೃಕ್ಷ;
	}

	ಯಾವತ್ (strbuf_getline_lf(&buf, stdin) != EOF) {
		ಸಂರಚನಾ object_id from_obj, to_obj;
		ಸಂರಚನಾ string_list split = STRING_LIST_INIT_NODUP;
		ಪೂರ್ಣಾಙ್ಕ err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		ಯದಿ (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		ಯದಿ (repo_get_oid(the_repository, split.ವಸ್ತುಗಳು[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.ವಸ್ತುಗಳು[0].string);
		ಯದಿ (repo_get_oid(the_repository, split.ವಸ್ತುಗಳು[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.ವಸ್ತುಗಳು[1].string);

		ಯದಿ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		ಅನ್ಯಥಾ
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		ಯದಿ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.ವಸ್ತುಗಳು[0].string, split.ವಸ್ತುಗಳು[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	ಯದಿ (!rewrite_cmd) {
		commit_notes(the_repository, t, ಸಂದೇಶ);
		free_notes(t);
	} ಅನ್ಯಥಾ {
		finish_copy_notes_for_rewrite(the_repository, c, ಸಂದೇಶ);
	}
	strbuf_release(&buf);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಸಂರಚನಾ notes_ವೃಕ್ಷ *init_notes_check(ಧ್ರುವ ವರ್ಣ *subcommand,
					   ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಧ್ರುವ ವರ್ಣ *ref;
	init_notes(ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, flags);
	t = &default_notes_ವೃಕ್ಷ;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	ಯದಿ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	ಪ್ರತ್ಯಾಗಮ t;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ list(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಸಂರಚನಾ object_id object;
	ಧ್ರುವ ಸಂರಚನಾ object_id *note;
	ಪೂರ್ಣಾಙ್ಕ retval = -1;
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	ಯದಿ (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	ಯದಿ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	ಯದಿ (argc) {
		ಯದಿ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		ಯದಿ (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} ಅನ್ಯಥಾ
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} ಅನ್ಯಥಾ
		retval = for_each_note(t, 0, list_each_note, ರಿಕ್ತ);

	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_edit(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ add(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
	       ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ force = 0, allow_empty = 0;
	ಧ್ರುವ ವರ್ಣ *object_ref;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಸಂರಚನಾ object_id object, new_note;
	ಧ್ರುವ ಸಂರಚನಾ object_id *note;
	ಸಂರಚನಾ note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_ಸಂದೇಶ_arg),
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

	ಯದಿ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	ಯದಿ (d.ಸಂದೇಶ_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	ಯದಿ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ಯದಿ (note) {
		ಯದಿ (!force) {
			free_notes(t);
			ಯದಿ (d.ಸಂದೇಶ_nr) {
				free_note_data(&d);
				ಪ್ರತ್ಯಾಗಮ error(_("Cannot add notes. "
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
			ಪ್ರತ್ಯಾಗಮ append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	ಯದಿ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		ಯದಿ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} ಅನ್ಯಥಾ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ copy(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ retval = 0, force = 0, from_stdin = 0;
	ಧ್ರುವ ಸಂರಚನಾ object_id *from_note, *note;
	ಧ್ರುವ ವರ್ಣ *object_ref;
	ಸಂರಚನಾ object_id object, from_obj;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಧ್ರುವ ವರ್ಣ *rewrite_cmd = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	ಯದಿ (from_stdin || rewrite_cmd) {
		ಯದಿ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} ಅನ್ಯಥಾ {
			ಪ್ರತ್ಯಾಗಮ notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	ಯದಿ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	ಯದಿ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	ಯದಿ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ಯದಿ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ಯದಿ (note) {
		ಯದಿ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			ಗಚ್ಛ out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	ಯದಿ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		ಗಚ್ಛ out;
	}

	ಯದಿ (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ append_edit(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ allow_empty = 0;
	ಧ್ರುವ ವರ್ಣ *object_ref;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಸಂರಚನಾ object_id object, new_note;
	ಧ್ರುವ ಸಂರಚನಾ object_id *note;
	ವರ್ಣ *logmsg;
	ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ *usage;
	ಸಂರಚನಾ note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	ಸಂರಚನಾ option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_ಸಂದೇಶ_arg),
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
	ಪೂರ್ಣಾಙ್ಕ edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	ಯದಿ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	ಯದಿ (d.ಸಂದೇಶ_nr) {
		concat_messages(&d);
		ಯದಿ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ಯದಿ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : ರಿಕ್ತ);

	ಯದಿ (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		ಗಣನಾ object_type type;
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ವರ್ಣ *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		ಯದಿ (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		ಯದಿ (size)
			strbuf_add(&buf, prev_buf, size);
		ಯದಿ (d.buf.len && size)
			append_separator(&buf);
		strbuf_ಸೇರಿಸು(&d.buf, 0, buf.buf, buf.len);

		ಮುಕ್ತ(prev_buf);
		strbuf_release(&buf);
	}

	ಯದಿ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		ಯದಿ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} ಅನ್ಯಥಾ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	ಮುಕ್ತ(logmsg);
	free_note_data(&d);
	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ show(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		ಸಂರಚನಾ repository *repo UNUSED)
{
	ಧ್ರುವ ವರ್ಣ *object_ref;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಸಂರಚನಾ object_id object;
	ಧ್ರುವ ಸಂರಚನಾ object_id *note;
	ಪೂರ್ಣಾಙ್ಕ retval;
	ಸಂರಚನಾ option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	ಯದಿ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	ಯದಿ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	ಯದಿ (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	ಅನ್ಯಥಾ {
		ಧ್ರುವ ವರ್ಣ *show_args[3] = {"show", oid_to_hex(note), ರಿಕ್ತ};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ merge_abort(ಸಂರಚನಾ notes_merge_options *o)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	ಯದಿ (refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ, "NOTES_MERGE_PARTIAL", ರಿಕ್ತ, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	ಯದಿ (refs_delete_ref(get_main_ref_store(the_repository), ರಿಕ್ತ, "NOTES_MERGE_REF", ರಿಕ್ತ, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	ಯದಿ (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ merge_commit(ಸಂರಚನಾ notes_merge_options *o)
{
	ಸಂರಚನಾ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ object_id oid, parent_oid;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ t = {0};
	ಸಂರಚನಾ commit *partial;
	ಸಂರಚನಾ pretty_print_context pretty_ctx;
	ಶೂನ್ಯ *local_ref_to_free;
	ಪೂರ್ಣಾಙ್ಕ ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	ಯದಿ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	ಅನ್ಯಥಾ ಯದಿ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	ಅನ್ಯಥಾ ಯದಿ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	ಯದಿ (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	ಅನ್ಯಥಾ
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ರಿಕ್ತ);
	ಯದಿ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	ಯದಿ (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, ಪರಿಮಾಣ(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &ಸಂದೇಶ,
				   &pretty_ctx);
	strbuf_trim(&ಸಂದೇಶ);
	strbuf_insertstr(&ಸಂದೇಶ, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? ರಿಕ್ತ : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&ಸಂದೇಶ);
	ret = merge_abort(o);
	ಮುಕ್ತ(local_ref_to_free);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ git_config_get_notes_strategy(ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
					 ಗಣನಾ notes_merge_strategy *strategy)
{
	ವರ್ಣ *ಮೌಲ್ಯ;

	ಯದಿ (repo_config_get_string(the_repository, ಕೀಲಿ, &ಮೌಲ್ಯ))
		ಪ್ರತ್ಯಾಗಮ 1;
	ಯದಿ (parse_notes_merge_strategy(ಮೌಲ್ಯ, strategy))
		git_die_config(the_repository, ಕೀಲಿ, _("unknown notes merge strategy %s"), ಮೌಲ್ಯ);

	ಮುಕ್ತ(ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ merge(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ strbuf remote_ref = STRBUF_INIT, ಸಂದೇಶ = STRBUF_INIT;
	ಸಂರಚನಾ object_id result_oid;
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಸಂರಚನಾ notes_merge_options o;
	ಪೂರ್ಣಾಙ್ಕ do_merge = 0, do_commit = 0, do_abort = 0;
	ಪೂರ್ಣಾಙ್ಕ verbosity = 0, result;
	ಧ್ರುವ ವರ್ಣ *strategy = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
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
	ವರ್ಣ *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	ಯದಿ (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	ಯದಿ (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	ಯದಿ (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} ಅನ್ಯಥಾ ಯದಿ (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	ಯದಿ (do_abort)
		ಪ್ರತ್ಯಾಗಮ merge_abort(&o);
	ಯದಿ (do_commit)
		ಪ್ರತ್ಯಾಗಮ merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	ಯದಿ (strategy) {
		ಯದಿ (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} ಅನ್ಯಥಾ {
		ಸಂರಚನಾ strbuf merge_ಕೀಲಿ = STRBUF_INIT;
		ಧ್ರುವ ವರ್ಣ *short_ref = ರಿಕ್ತ;

		ಯದಿ (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_ಕೀಲಿ, "notes.%s.mergeStrategy", short_ref);

		ಯದಿ (git_config_get_notes_strategy(merge_ಕೀಲಿ.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_ಕೀಲಿ);
	}

	strbuf_addf(&ಸಂದೇಶ, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_ಸಂದೇಶ), ಸಂದೇಶ.buf + 7, ಸಂದೇಶ.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	ಯದಿ (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				notes_ref, &result_oid, ರಿಕ್ತ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ಅನ್ಯಥಾ { /* Merge has unresolved conflicts */
		ಸಂರಚನಾ worktree **worktrees;
		ಧ್ರುವ ಸಂರಚನಾ worktree *wt;
		ವರ್ಣ *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, ರಿಕ್ತ,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		ಯದಿ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		ಯದಿ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ರಿಕ್ತ))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		ಮುಕ್ತ(path);
	}

	free_notes(t);
	ಮುಕ್ತ(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&ಸಂದೇಶ);
	ಪ್ರತ್ಯಾಗಮ result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_one_note(ಸಂರಚನಾ notes_ವೃಕ್ಷ *t, ಧ್ರುವ ವರ್ಣ *name, unsigned flag)
{
	ಪೂರ್ಣಾಙ್ಕ status;
	ಸಂರಚನಾ object_id oid;
	ಯದಿ (repo_get_oid(the_repository, name, &oid))
		ಪ್ರತ್ಯಾಗಮ error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	ಯದಿ (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	ಅನ್ಯಥಾ
		fprintf(stderr, _("Removing note for object %s\n"), name);
	ಪ್ರತ್ಯಾಗಮ (flag & IGNORE_MISSING) ? 0 : status;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ remove_cmd(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		      ಸಂರಚನಾ repository *repo UNUSED)
{
	unsigned flag = 0;
	ಪೂರ್ಣಾಙ್ಕ from_stdin = 0;
	ಸಂರಚನಾ option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಪೂರ್ಣಾಙ್ಕ retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	ಯದಿ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} ಅನ್ಯಥಾ {
		ಯಾವತ್ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	ಯದಿ (from_stdin) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		ಯಾವತ್ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	ಯದಿ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ prune(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		 ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ notes_ವೃಕ್ಷ *t;
	ಪೂರ್ಣಾಙ್ಕ show_only = 0, verbose = 0;
	ಸಂರಚನಾ option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	ಯದಿ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	ಯದಿ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_ref(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv, ಧ್ರುವ ವರ್ಣ *prefix,
		   ಸಂರಚನಾ repository *repo UNUSED)
{
	ಸಂರಚನಾ option options[] = { OPT_END() };
	ವರ್ಣ *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	ಯದಿ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	ಮುಕ್ತ(notes_ref);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_notes(ಪೂರ್ಣಾಙ್ಕ argc,
	      ಧ್ರುವ ವರ್ಣ **argv,
	      ಧ್ರುವ ವರ್ಣ *prefix,
	      ಸಂರಚನಾ repository *repo)
{
	ಧ್ರುವ ವರ್ಣ *override_notes_ref = ರಿಕ್ತ;
	parse_opt_subcommand_fn *fn = ರಿಕ್ತ;
	ಸಂರಚನಾ option options[] = {
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

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	ಯದಿ (!fn) {
		ಯದಿ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	ಯದಿ (override_notes_ref) {
		ಸಂರಚನಾ strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	ಪ್ರತ್ಯಾಗಮ !!fn(argc, argv, prefix, repo);
}
