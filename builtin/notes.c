/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *separator = "\n";
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_usage[] = {
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
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_ಕೂಡಿಸು_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_ವಿಲೀನ_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ಖಾಲಿ
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ note_template[] =
	N_("Write/edit the notes for the following object:");

ಎಣಿಕೆ notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

ರಚನೆ note_ಸಂದೇಶ {
	ಎಣಿಕೆ notes_stripspace stripspace;
	ರಚನೆ strbuf buf;
};

ರಚನೆ note_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ use_editor;
	ಪೂರ್ಣಾಂಕ stripspace;
	ಅಕ್ಷರ *edit_ಮಾರ್ಗ;
	ರಚನೆ strbuf buf;
	ರಚನೆ note_ಸಂದೇಶ **messages;
	ಗಾತ್ರ_t ಸಂದೇಶ_nr;
	ಗಾತ್ರ_t ಸಂದೇಶ_alloc;
};

ಸ್ಥಿರ ಶೂನ್ಯ free_note_ದತ್ತಾಂಶ(ರಚನೆ note_ದತ್ತಾಂಶ *d)
{
	ಆದರೆ (d->edit_ಮಾರ್ಗ) {
		unlink_or_warn(d->edit_ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(d->edit_ಮಾರ್ಗ);
	}
	strbuf_release(&d->buf);

	ಆಗ (d->ಸಂದೇಶ_nr--) {
		strbuf_release(&d->messages[d->ಸಂದೇಶ_nr]->buf);
		ಬಿಡುಗಡೆ(d->messages[d->ಸಂದೇಶ_nr]);
	}
	ಬಿಡುಗಡೆ(d->messages);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list_each_note(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *object_oid,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *note_oid,
			  ಅಕ್ಷರ *note_ಮಾರ್ಗ ಬಳಸದ,
			  ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ ಬಳಸದ)
{
	ಬರೆ("%s %s\n", oid_ಗಮ್ಯ_hex(note_oid), oid_ಗಮ್ಯ_hex(object_oid));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_obj_ಗಮ್ಯ_fd(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *oid)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2;
	ಎಣಿಕೆ object_type type;
	ಅಕ್ಷರ *buf = odb_read_object(the_repository->objects, oid, &type, &ಗಾತ್ರ_2);
	ಆದರೆ (buf) {
		ಆದರೆ (ಗಾತ್ರ_2)
			write_or_die(fd, buf, ಗಾತ್ರ_2);
		ಬಿಡುಗಡೆ(buf);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_commented_object(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *object)
{
	ರಚನೆ ಮಗು_process show = ಮಗು_PROCESS_INIT;
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.ವಾದಗಳು, "show", "--stat", "--no-notes",
		     oid_ಗಮ್ಯ_hex(object), ಖಾಲಿ);
	show.no_stdin = 1;
	show.ಹೊರಹೋಗು = -1;
	show.err = 0;
	show.git_cmd = 1;
	ಆದರೆ (ಆರಂಭ_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_ಗಮ್ಯ_hex(object));

	ಆದರೆ (strbuf_read(&buf, show.ಹೊರಹೋಗು, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_ಕೂಡಿಸು_commented_ಸಾಲುಗಳು(&cbuf, buf.buf, buf.len, comment_ಸಾಲು_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	ಆದರೆ (ಮುಕ್ತಾಯ_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_ಗಮ್ಯ_hex(object));
}

ಸ್ಥಿರ ಶೂನ್ಯ prepare_note_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *object, ರಚನೆ note_ದತ್ತಾಂಶ *d,
		ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *old_note)
{
	ಆದರೆ (d->use_editor || !d->ಸಂದೇಶ_nr) {
		ಪೂರ್ಣಾಂಕ fd;
		ರಚನೆ strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_ಮಾರ್ಗ, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ಆದರೆ (d->ಸಂದೇಶ_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (old_note)
			copy_obj_ಗಮ್ಯ_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_ಕೂಡಿಸು_commented_ಸಾಲುಗಳು(&buf, "\n", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("\n"), comment_ಸಾಲು_str);
		strbuf_ಕೂಡಿಸು_commented_ಸಾಲುಗಳು(&buf, _(note_template), ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(_(note_template)),
					   comment_ಸಾಲು_str);
		strbuf_ಕೂಡಿಸು_commented_ಸಾಲುಗಳು(&buf, "\n", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("\n"), comment_ಸಾಲು_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		ಆದರೆ (launch_editor(d->edit_ಮಾರ್ಗ, &d->buf, ಖಾಲಿ)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		ಆದರೆ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_ಸಾಲು_str);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ write_note_ದತ್ತಾಂಶ(ರಚನೆ note_ದತ್ತಾಂಶ *d, ರಚನೆ object_id *oid)
{
	ಆದರೆ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = die_ಸಂದೇಶ(_("unable to write note object"));

		ಆದರೆ (d->edit_ಮಾರ್ಗ)
			die_ಸಂದೇಶ(_("the note contents have been left in %s"),
				    d->edit_ಮಾರ್ಗ);
		ನಿರ್ಗಮಿಸು(ಸ್ಥಿತಿ_2);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ append_separator(ರಚನೆ strbuf *ಸಂದೇಶ_2)
{
	ಗಾತ್ರ_t sep_len = 0;

	ಆದರೆ (!separator)
		ಹಿಂತಿರುಗಿ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((sep_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(ಸಂದೇಶ_2, separator);
	ಇಲ್ಲದಿದ್ದರೆ
		strbuf_addf(ಸಂದೇಶ_2, "%s%s", separator, "\n");
}

ಸ್ಥಿರ ಶೂನ್ಯ concat_messages(ರಚನೆ note_ದತ್ತಾಂಶ *d)
{
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ಗಾತ್ರ_t i;

	ಗಾಗಿ (i = 0; i < d->ಸಂದೇಶ_nr ; i++) {
		ಆದರೆ (d->buf.len)
			append_separator(&d->buf);
		strbuf_ಕೂಡಿಸು(&ಸಂದೇಶ, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &ಸಂದೇಶ);
		ಆದರೆ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ಖಾಲಿ);
		strbuf_reset(&ಸಂದೇಶ);
	}
	strbuf_release(&ಸಂದೇಶ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_ಸಂದೇಶ_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ note_ದತ್ತಾಂಶ *d = opt->ಮೌಲ್ಯ;
	ರಚನೆ note_ಸಂದೇಶ *ಸಂದೇಶ = xmalloc(ಗಾತ್ರ(*ಸಂದೇಶ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ಸಂದೇಶ->buf, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(arg));
	strbuf_addstr(&ಸಂದೇಶ->buf, arg);
	ALLOC_GROW_BY(d->messages, d->ಸಂದೇಶ_nr, 1, d->ಸಂದೇಶ_alloc);
	d->messages[d->ಸಂದೇಶ_nr - 1] = ಸಂದೇಶ;
	ಸಂದೇಶ->stripspace = STRIPSPACE;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_file_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ note_ದತ್ತಾಂಶ *d = opt->ಮೌಲ್ಯ;
	ರಚನೆ note_ಸಂದೇಶ *ಸಂದೇಶ = xmalloc(ಗಾತ್ರ(*ಸಂದೇಶ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ಸಂದೇಶ->buf , 0);
	ಆದರೆ (!strcmp(arg, "-")) {
		ಆದರೆ (strbuf_read(&ಸಂದೇಶ->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strbuf_read_file(&ಸಂದೇಶ->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->ಸಂದೇಶ_nr, 1, d->ಸಂದೇಶ_alloc);
	d->messages[d->ಸಂದೇಶ_nr - 1] = ಸಂದೇಶ;
	ಸಂದೇಶ->stripspace = STRIPSPACE;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_reuse_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ note_ದತ್ತಾಂಶ *d = opt->ಮೌಲ್ಯ;
	ರಚನೆ note_ಸಂದೇಶ *ಸಂದೇಶ = xmalloc(ಗಾತ್ರ(*ಸಂದೇಶ));
	ಅಕ್ಷರ *ಮೌಲ್ಯ;
	ರಚನೆ object_id object;
	ಎಣಿಕೆ object_type type;
	ಗಾತ್ರ_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ಸಂದೇಶ->buf, 0);
	ಆದರೆ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	ಆದರೆ (!(ಮೌಲ್ಯ = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	ಆದರೆ (type != OBJ_BLOB) {
		strbuf_release(&ಸಂದೇಶ->buf);
		ಬಿಡುಗಡೆ(ಮೌಲ್ಯ);
		ಬಿಡುಗಡೆ(ಸಂದೇಶ);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_ಕೂಡಿಸು(&ಸಂದೇಶ->buf, ಮೌಲ್ಯ, len);
	ಬಿಡುಗಡೆ(ಮೌಲ್ಯ);

	ಸಂದೇಶ->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->ಸಂದೇಶ_nr, 1, d->ಸಂದೇಶ_alloc);
	d->messages[d->ಸಂದೇಶ_nr - 1] = ಸಂದೇಶ;
	ಸಂದೇಶ->stripspace = NO_STRIPSPACE;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_reedit_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ note_ದತ್ತಾಂಶ *d = opt->ಮೌಲ್ಯ;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	ಹಿಂತಿರುಗಿ parse_reuse_arg(opt, arg, unset);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_separator_arg(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			       ಪೂರ್ಣಾಂಕ unset)
{
	ಆದರೆ (unset)
		*(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)opt->ಮೌಲ್ಯ = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		*(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)opt->ಮೌಲ್ಯ = arg ? arg : "\n";
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ notes_copy_from_stdin(ಪೂರ್ಣಾಂಕ force, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rewrite_cmd)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ notes_rewrite_cfg *c = ಖಾಲಿ;
	ರಚನೆ notes_ವೃಕ್ಷ *t = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = "Notes added by 'git notes copy'";

	ಆದರೆ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		ಆದರೆ (!c)
			ಹಿಂತಿರುಗಿ 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		init_notes(ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, NOTES_INIT_WRITABLE);
		t = &default_notes_ವೃಕ್ಷ;
	}

	ಆಗ (strbuf_getline_lf(&buf, stdin) != EOF) {
		ರಚನೆ object_id from_obj, ಗಮ್ಯ_obj;
		ರಚನೆ string_list ವಿಭಜಿಸು = STRING_LIST_INIT_NODUP;
		ಪೂರ್ಣಾಂಕ err;

		string_list_ವಿಭಜಿಸು_ಒಳ_ಇರಿಸು_f(&ವಿಭಜಿಸು, buf.buf, " ", -1,
					     STRING_LIST_ವಿಭಜಿಸು_TRIM);
		ಆದರೆ (ವಿಭಜಿಸು.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		ಆದರೆ (repo_get_oid(the_repository, ವಿಭಜಿಸು.ವಸ್ತುಗಳು[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    ವಿಭಜಿಸು.ವಸ್ತುಗಳು[0].string);
		ಆದರೆ (repo_get_oid(the_repository, ವಿಭಜಿಸು.ವಸ್ತುಗಳು[1].string, &ಗಮ್ಯ_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    ವಿಭಜಿಸು.ವಸ್ತುಗಳು[1].string);

		ಆದರೆ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &ಗಮ್ಯ_obj);
		ಇಲ್ಲದಿದ್ದರೆ
			err = copy_note(t, &from_obj, &ಗಮ್ಯ_obj, force,
					combine_notes_overwrite);

		ಆದರೆ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      ವಿಭಜಿಸು.ವಸ್ತುಗಳು[0].string, ವಿಭಜಿಸು.ವಸ್ತುಗಳು[1].string);
			ret = 1;
		}

		string_list_clear(&ವಿಭಜಿಸು, 0);
	}

	ಆದರೆ (!rewrite_cmd) {
		commit_notes(the_repository, t, ಸಂದೇಶ);
		free_notes(t);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಮುಕ್ತಾಯ_copy_notes_for_rewrite(the_repository, c, ಸಂದೇಶ);
	}
	strbuf_release(&buf);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ರಚನೆ notes_ವೃಕ್ಷ *init_notes_check(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subcommand,
					   ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ref;
	init_notes(ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, flags);
	t = &default_notes_ವೃಕ್ಷ;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	ಆದರೆ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	ಹಿಂತಿರುಗಿ t;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ list(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ರಚನೆ object_id object;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *note;
	ಪೂರ್ಣಾಂಕ retval = -1;
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	ಆದರೆ (argc)
		argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
				     git_notes_list_usage, 0);

	ಆದರೆ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	ಆದರೆ (argc) {
		ಆದರೆ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		ಆದರೆ (note) {
			puts(oid_ಗಮ್ಯ_hex(note));
			retval = 0;
		} ಇಲ್ಲದಿದ್ದರೆ
			retval = error(_("no note found for object %s."),
				       oid_ಗಮ್ಯ_hex(&object));
	} ಇಲ್ಲದಿದ್ದರೆ
		retval = for_each_note(t, 0, list_each_note, ಖಾಲಿ);

	free_notes(t);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_edit(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	       ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ force = 0, allow_ಖಾಲಿಗಳು = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref;
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ರಚನೆ object_id object, new_note;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *note;
	ರಚನೆ note_ದತ್ತಾಂಶ d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	ರಚನೆ option options[] = {
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
		OPT_BOOL(0, "allow-empty", &allow_ಖಾಲಿಗಳು,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_notes_ಕೂಡಿಸು_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ಆದರೆ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_ಕೂಡಿಸು_usage, options);
	}

	ಆದರೆ (d.ಸಂದೇಶ_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ಆದರೆ (note) {
		ಆದರೆ (!force) {
			free_notes(t);
			ಆದರೆ (d.ಸಂದೇಶ_nr) {
				free_note_ದತ್ತಾಂಶ(&d);
				ಹಿಂತಿರುಗಿ error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_ಗಮ್ಯ_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			ಹಿಂತಿರುಗಿ append_edit(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_ಗಮ್ಯ_hex(&object));
	}

	prepare_note_ದತ್ತಾಂಶ(&object, &d, note);
	ಆದರೆ (d.buf.len || allow_ಖಾಲಿಗಳು) {
		write_note_ದತ್ತಾಂಶ(&d, &new_note);
		ಆದರೆ (ಕೂಡಿಸು_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} ಇಲ್ಲದಿದ್ದರೆ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_ಗಮ್ಯ_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_ದತ್ತಾಂಶ(&d);
	free_notes(t);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ copy(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ retval = 0, force = 0, from_stdin = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *from_note, *note;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref;
	ರಚನೆ object_id object, from_obj;
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rewrite_cmd = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_notes_copy_usage,
			     0);

	ಆದರೆ (from_stdin || rewrite_cmd) {
		ಆದರೆ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಹಿಂತಿರುಗಿ notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	ಆದರೆ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	ಆದರೆ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	ಆದರೆ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ಆದರೆ (note) {
		ಆದರೆ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_ಗಮ್ಯ_hex(&object));
			ಹೋಗು ಹೊರಹೋಗು;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_ಗಮ್ಯ_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	ಆದರೆ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_ಗಮ್ಯ_hex(&from_obj));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (ಕೂಡಿಸು_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
ಹೊರಹೋಗು:
	free_notes(t);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ append_edit(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ allow_ಖಾಲಿಗಳು = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref;
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ರಚನೆ object_id object, new_note;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *note;
	ಅಕ್ಷರ *logmsg;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *usage;
	ರಚನೆ note_ದತ್ತಾಂಶ d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	ರಚನೆ option options[] = {
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
		OPT_BOOL(0, "allow-empty", &allow_ಖಾಲಿಗಳು,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_ಕೊನೆ()
	};
	ಪೂರ್ಣಾಂಕ edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	ಆದರೆ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	ಆದರೆ (d.ಸಂದೇಶ_nr) {
		concat_messages(&d);
		ಆದರೆ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_ದತ್ತಾಂಶ(&object, &d, edit && note ? note : ಖಾಲಿ);

	ಆದರೆ (note && !edit) {
		/* Append buf to previous note contents */
		ಗಾತ್ರ_t ಗಾತ್ರ_2;
		ಎಣಿಕೆ object_type type;
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ಅಕ್ಷರ *prev_buf = odb_read_object(the_repository->objects, note, &type, &ಗಾತ್ರ_2);

		ಆದರೆ (!prev_buf)
			die(_("unable to read %s"), oid_ಗಮ್ಯ_hex(note));
		ಆದರೆ (ಗಾತ್ರ_2)
			strbuf_ಕೂಡಿಸು(&buf, prev_buf, ಗಾತ್ರ_2);
		ಆದರೆ (d.buf.len && ಗಾತ್ರ_2)
			append_separator(&buf);
		strbuf_ಸೇರಿಸು(&d.buf, 0, buf.buf, buf.len);

		ಬಿಡುಗಡೆ(prev_buf);
		strbuf_release(&buf);
	}

	ಆದರೆ (d.buf.len || allow_ಖಾಲಿಗಳು) {
		write_note_ದತ್ತಾಂಶ(&d, &new_note);
		ಆದರೆ (ಕೂಡಿಸು_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} ಇಲ್ಲದಿದ್ದರೆ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_ಗಮ್ಯ_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	ಬಿಡುಗಡೆ(logmsg);
	free_note_ದತ್ತಾಂಶ(&d);
	free_notes(t);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ show(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		ರಚನೆ repository *repo ಬಳಸದ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *object_ref;
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ರಚನೆ object_id object;
	ಸ್ಥಿರಾಂಕ ರಚನೆ object_id *note;
	ಪೂರ್ಣಾಂಕ retval;
	ರಚನೆ option options[] = {
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_notes_show_usage,
			     0);

	ಆದರೆ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	ಆದರೆ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	ಆದರೆ (!note)
		retval = error(_("no note found for object %s."),
			       oid_ಗಮ್ಯ_hex(&object));
	ಇಲ್ಲದಿದ್ದರೆ {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *show_ವಾದಗಳು[3] = {"show", oid_ಗಮ್ಯ_hex(note), ಖಾಲಿ};
		retval = execv_git_cmd(show_ವಾದಗಳು);
	}
	free_notes(t);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_abort(ರಚನೆ notes_ವಿಲೀನ_options *o)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	ಆದರೆ (refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ, "NOTES_MERGE_PARTIAL", ಖಾಲಿ, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	ಆದರೆ (refs_delete_ref(get_main_ref_store(the_repository), ಖಾಲಿ, "NOTES_MERGE_REF", ಖಾಲಿ, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	ಆದರೆ (notes_ವಿಲೀನ_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ_commit(ರಚನೆ notes_ವಿಲೀನ_options *o)
{
	ರಚನೆ strbuf ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ object_id oid, ಪೋಷಕ_oid;
	ರಚನೆ notes_ವೃಕ್ಷ t = {0};
	ರಚನೆ commit *partial;
	ರಚನೆ pretty_print_context pretty_ctx;
	ಶೂನ್ಯ *local_ref_ಗಮ್ಯ_free;
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	ಆದರೆ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	ಆದರೆ (partial->parents)
		oidcpy(&ಪೋಷಕ_oid, &partial->parents->ಅಂಶ->object.oid);
	ಇಲ್ಲದಿದ್ದರೆ
		oidclr(&ಪೋಷಕ_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_ಗಮ್ಯ_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ಖಾಲಿ);
	ಆದರೆ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	ಆದರೆ (notes_ವಿಲೀನ_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, ಗಾತ್ರ(pretty_ctx));
	repo_format_commit_ಸಂದೇಶ(the_repository, partial, "%s", &ಸಂದೇಶ,
				   &pretty_ctx);
	strbuf_trim(&ಸಂದೇಶ);
	strbuf_insertstr(&ಸಂದೇಶ, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
			o->local_ref, &oid,
			is_null_oid(&ಪೋಷಕ_oid) ? ಖಾಲಿ : &ಪೋಷಕ_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&ಸಂದೇಶ);
	ret = ವಿಲೀನ_abort(o);
	ಬಿಡುಗಡೆ(local_ref_ಗಮ್ಯ_free);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ git_config_get_notes_strategy(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
					 ಎಣಿಕೆ notes_ವಿಲೀನ_strategy *strategy)
{
	ಅಕ್ಷರ *ಮೌಲ್ಯ;

	ಆದರೆ (repo_config_get_string(the_repository, ಕೀಲಿ, &ಮೌಲ್ಯ))
		ಹಿಂತಿರುಗಿ 1;
	ಆದರೆ (parse_notes_ವಿಲೀನ_strategy(ಮೌಲ್ಯ, strategy))
		git_die_config(the_repository, ಕೀಲಿ, _("unknown notes merge strategy %s"), ಮೌಲ್ಯ);

	ಬಿಡುಗಡೆ(ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ವಿಲೀನ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ strbuf remote_ref = STRBUF_INIT, ಸಂದೇಶ = STRBUF_INIT;
	ರಚನೆ object_id ಫಲಿತಾಂಶ_oid;
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ರಚನೆ notes_ವಿಲೀನ_options o;
	ಪೂರ್ಣಾಂಕ do_ವಿಲೀನ = 0, do_commit = 0, do_abort = 0;
	ಪೂರ್ಣಾಂಕ verbosity = 0, ಫಲಿತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *strategy = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_ಗುಂಪು(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_ಗುಂಪು(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_ಗುಂಪು(N_("Committing unmerged notes")),
		OPT_ಗಣ_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_ಗುಂಪು(N_("Aborting notes merge resolution")),
		OPT_ಗಣ_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_ಕೊನೆ()
	};
	ಅಕ್ಷರ *notes_ref;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_notes_ವಿಲೀನ_usage, 0);

	ಆದರೆ (strategy || do_commit + do_abort == 0)
		do_ವಿಲೀನ = 1;
	ಆದರೆ (do_ವಿಲೀನ + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_ವಿಲೀನ_usage, options);
	}

	ಆದರೆ (do_ವಿಲೀನ && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_ವಿಲೀನ_usage, options);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!do_ವಿಲೀನ && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_ವಿಲೀನ_usage, options);
	}

	init_notes_ವಿಲೀನ_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_ವಿಲೀನ_VERBOSITY_DEFAULT;

	ಆದರೆ (do_abort)
		ಹಿಂತಿರುಗಿ ವಿಲೀನ_abort(&o);
	ಆದರೆ (do_commit)
		ಹಿಂತಿರುಗಿ ವಿಲೀನ_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	ಆದರೆ (strategy) {
		ಆದರೆ (parse_notes_ವಿಲೀನ_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_ವಿಲೀನ_usage, options);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ strbuf ವಿಲೀನ_ಕೀಲಿ = STRBUF_INIT;
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *short_ref = ಖಾಲಿ;

		ಆದರೆ (!skip_ಪೂರ್ವಪ್ರತ್ಯಯ(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&ವಿಲೀನ_ಕೀಲಿ, "notes.%s.mergeStrategy", short_ref);

		ಆದರೆ (git_config_get_notes_strategy(ವಿಲೀನ_ಕೀಲಿ.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&ವಿಲೀನ_ಕೀಲಿ);
	}

	strbuf_addf(&ಸಂದೇಶ, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_ಕೂಡಿಸು(&(o.commit_ಸಂದೇಶ), ಸಂದೇಶ.buf + 7, ಸಂದೇಶ.len - 7); /* skip "notes: " */

	ಫಲಿತಾಂಶ = notes_ವಿಲೀನ(&o, t, &ಫಲಿತಾಂಶ_oid);

	ಆದರೆ (ಫಲಿತಾಂಶ >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				notes_ref, &ಫಲಿತಾಂಶ_oid, ಖಾಲಿ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ಇಲ್ಲದಿದ್ದರೆ { /* Merge has unresolved conflicts */
		ರಚನೆ worktree **worktrees;
		ಸ್ಥಿರಾಂಕ ರಚನೆ worktree *wt;
		ಅಕ್ಷರ *ಮಾರ್ಗ;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), ಸಂದೇಶ.buf,
				"NOTES_MERGE_PARTIAL", &ಫಲಿತಾಂಶ_oid, ಖಾಲಿ,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = ಕಂಡುಹಿಡಿ_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		ಆದರೆ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->ಮಾರ್ಗ);
		free_worktrees(worktrees);
		ಆದರೆ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ಖಾಲಿ))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		ಮಾರ್ಗ = repo_git_ಮಾರ್ಗ(the_repository, NOTES_ವಿಲೀನ_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			ಮಾರ್ಗ);
		ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
	}

	free_notes(t);
	ಬಿಡುಗಡೆ(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&ಸಂದೇಶ);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ < 0; /* return non-zero on conflicts */
}

#ವ್ಯಾಖ್ಯಾನಿಸು IGNORE_MISSING 1

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_one_note(ರಚನೆ notes_ವೃಕ್ಷ *t, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಚಿಹ್ನೆರಹಿತ flag)
{
	ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
	ರಚನೆ object_id oid;
	ಆದರೆ (repo_get_oid(the_repository, name, &oid))
		ಹಿಂತಿರುಗಿ error(_("Failed to resolve '%s' as a valid ref."), name);
	ಸ್ಥಿತಿ_2 = remove_note(t, oid.hash);
	ಆದರೆ (ಸ್ಥಿತಿ_2)
		fprintf(stderr, _("Object %s has no note\n"), name);
	ಇಲ್ಲದಿದ್ದರೆ
		fprintf(stderr, _("Removing note for object %s\n"), name);
	ಹಿಂತಿರುಗಿ (flag & IGNORE_MISSING) ? 0 : ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_cmd(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		      ರಚನೆ repository *repo ಬಳಸದ)
{
	ಚಿಹ್ನೆರಹಿತ flag = 0;
	ಪೂರ್ಣಾಂಕ from_stdin = 0;
	ರಚನೆ option options[] = {
		OPT_ಬಿಟ್(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_ಕೊನೆ()
	};
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ಪೂರ್ಣಾಂಕ retval = 0;

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	ಆದರೆ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆಗ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	ಆದರೆ (from_stdin) {
		ರಚನೆ strbuf sb = STRBUF_INIT;
		ಆಗ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	ಆದರೆ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ prune(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		 ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ notes_ವೃಕ್ಷ *t;
	ಪೂರ್ಣಾಂಕ show_only = 0, verbose = 0;
	ರಚನೆ option options[] = {
		OPT_DRY_ಸತತ(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_ಕೊನೆ()
	};

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_notes_prune_usage,
			     0);

	ಆದರೆ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	ಆದರೆ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_ref(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ option options[] = { OPT_ಕೊನೆ() };
	ಅಕ್ಷರ *notes_ref;
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_notes_get_ref_usage, 0);

	ಆದರೆ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	ಬಿಡುಗಡೆ(notes_ref);
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_notes(ಪೂರ್ಣಾಂಕ argc,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
	      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
	      ರಚನೆ repository *repo)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *override_notes_ref = ಖಾಲಿ;
	parse_opt_subcommand_fn *fn = ಖಾಲಿ;
	ರಚನೆ option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, ಕೂಡಿಸು),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, ವಿಲೀನ),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);
	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	ಆದರೆ (!fn) {
		ಆದರೆ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	ಆದರೆ (override_notes_ref) {
		ರಚನೆ strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	ಹಿಂತಿರುಗಿ !!fn(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, repo);
}
