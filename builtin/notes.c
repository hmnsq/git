/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "editor.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hex.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "notes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "object-name.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "odb.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "path.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "pretty.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "refs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exec-cmd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run-command.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "notes-merge.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "notes-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "worktree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *separator = "\n";
ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_usage[] = {
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
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_ਜੋੜੋ_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_ਮਿਲਾਓ_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ਨਿਰਾ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ note_template[] =
	N_("Write/edit the notes for the following object:");

ਗਿਣਤੀ notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

ਬਣਤਰ note_ਸੁਨੇਹਾ {
	ਗਿਣਤੀ notes_stripspace stripspace;
	ਬਣਤਰ strbuf buf;
};

ਬਣਤਰ note_ਅੰਕੜੇ {
	ਪੂਰਨ use_editor;
	ਪੂਰਨ stripspace;
	ਅੱਖਰ *edit_ਰਸਤਾ;
	ਬਣਤਰ strbuf buf;
	ਬਣਤਰ note_ਸੁਨੇਹਾ **messages;
	ਮਾਪ_t ਸੁਨੇਹਾ_nr;
	ਮਾਪ_t ਸੁਨੇਹਾ_alloc;
};

ਸਥਿਰ ਖਾਲੀ free_note_ਅੰਕੜੇ(ਬਣਤਰ note_ਅੰਕੜੇ *d)
{
	ਜੇ (d->edit_ਰਸਤਾ) {
		unlink_or_warn(d->edit_ਰਸਤਾ);
		ਮੁਕਤ(d->edit_ਰਸਤਾ);
	}
	strbuf_release(&d->buf);

	ਜਦੋਂ (d->ਸੁਨੇਹਾ_nr--) {
		strbuf_release(&d->messages[d->ਸੁਨੇਹਾ_nr]->buf);
		ਮੁਕਤ(d->messages[d->ਸੁਨੇਹਾ_nr]);
	}
	ਮੁਕਤ(d->messages);
}

ਸਥਿਰ ਪੂਰਨ list_each_note(ਅਚਲ ਬਣਤਰ object_id *object_oid,
			  ਅਚਲ ਬਣਤਰ object_id *note_oid,
			  ਅੱਖਰ *note_ਰਸਤਾ ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *cb_ਅੰਕੜੇ ਅਣਵਰਤਿਆ)
{
	ਲਿਖੋ("%s %s\n", oid_ਮੰਜ਼ਿਲ_hex(note_oid), oid_ਮੰਜ਼ਿਲ_hex(object_oid));
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ copy_obj_ਮੰਜ਼ਿਲ_fd(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ object_id *oid)
{
	ਮਾਪ_t ਮਾਪ;
	ਗਿਣਤੀ object_type type;
	ਅੱਖਰ *buf = odb_read_object(the_repository->objects, oid, &type, &ਮਾਪ);
	ਜੇ (buf) {
		ਜੇ (ਮਾਪ)
			write_or_die(fd, buf, ਮਾਪ);
		ਮੁਕਤ(buf);
	}
}

ਸਥਿਰ ਖਾਲੀ write_commented_object(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ object_id *object)
{
	ਬਣਤਰ ਬੱਚਾ_process show = ਬੱਚਾ_PROCESS_INIT;
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.ਦਲੀਲਾਂ, "show", "--stat", "--no-notes",
		     oid_ਮੰਜ਼ਿਲ_hex(object), ਨਿਰਾ);
	show.no_stdin = 1;
	show.ਬਾਹਰਲਾ = -1;
	show.err = 0;
	show.git_cmd = 1;
	ਜੇ (ਸ਼ੁਰੂ_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_ਮੰਜ਼ਿਲ_hex(object));

	ਜੇ (strbuf_read(&buf, show.ਬਾਹਰਲਾ, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_ਜੋੜੋ_commented_ਸਤਰਾਂ(&cbuf, buf.buf, buf.len, comment_ਸਤਰ_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	ਜੇ (ਅੰਤ_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_ਮੰਜ਼ਿਲ_hex(object));
}

ਸਥਿਰ ਖਾਲੀ prepare_note_ਅੰਕੜੇ(ਅਚਲ ਬਣਤਰ object_id *object, ਬਣਤਰ note_ਅੰਕੜੇ *d,
		ਅਚਲ ਬਣਤਰ object_id *old_note)
{
	ਜੇ (d->use_editor || !d->ਸੁਨੇਹਾ_nr) {
		ਪੂਰਨ fd;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_ਰਸਤਾ, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ਜੇ (d->ਸੁਨੇਹਾ_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		ਨਹੀਂਤਾਂ ਜੇ (old_note)
			copy_obj_ਮੰਜ਼ਿਲ_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_ਜੋੜੋ_commented_ਸਤਰਾਂ(&buf, "\n", ਲੜੀ_ਲੰਬਾਈ("\n"), comment_ਸਤਰ_str);
		strbuf_ਜੋੜੋ_commented_ਸਤਰਾਂ(&buf, _(note_template), ਲੜੀ_ਲੰਬਾਈ(_(note_template)),
					   comment_ਸਤਰ_str);
		strbuf_ਜੋੜੋ_commented_ਸਤਰਾਂ(&buf, "\n", ਲੜੀ_ਲੰਬਾਈ("\n"), comment_ਸਤਰ_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		ਜੇ (launch_editor(d->edit_ਰਸਤਾ, &d->buf, ਨਿਰਾ)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		ਜੇ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_ਸਤਰ_str);
	}
}

ਸਥਿਰ ਖਾਲੀ write_note_ਅੰਕੜੇ(ਬਣਤਰ note_ਅੰਕੜੇ *d, ਬਣਤਰ object_id *oid)
{
	ਜੇ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		ਪੂਰਨ ਸਥਿਤੀ = die_ਸੁਨੇਹਾ(_("unable to write note object"));

		ਜੇ (d->edit_ਰਸਤਾ)
			die_ਸੁਨੇਹਾ(_("the note contents have been left in %s"),
				    d->edit_ਰਸਤਾ);
		ਬਾਹਰ_ਨਿਕਲੋ(ਸਥਿਤੀ);
	}
}

ਸਥਿਰ ਖਾਲੀ append_separator(ਬਣਤਰ strbuf *ਸੁਨੇਹਾ_2)
{
	ਮਾਪ_t sep_len = 0;

	ਜੇ (!separator)
		ਵਾਪਸ;
	ਨਹੀਂਤਾਂ ਜੇ ((sep_len = ਲੜੀ_ਲੰਬਾਈ(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(ਸੁਨੇਹਾ_2, separator);
	ਨਹੀਂਤਾਂ
		strbuf_addf(ਸੁਨੇਹਾ_2, "%s%s", separator, "\n");
}

ਸਥਿਰ ਖਾਲੀ concat_messages(ਬਣਤਰ note_ਅੰਕੜੇ *d)
{
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਮਾਪ_t i;

	ਲਈ (i = 0; i < d->ਸੁਨੇਹਾ_nr ; i++) {
		ਜੇ (d->buf.len)
			append_separator(&d->buf);
		strbuf_ਜੋੜੋ(&ਸੁਨੇਹਾ, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &ਸੁਨੇਹਾ);
		ਜੇ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ਨਿਰਾ);
		strbuf_reset(&ਸੁਨੇਹਾ);
	}
	strbuf_release(&ਸੁਨੇਹਾ);
}

ਸਥਿਰ ਪੂਰਨ parse_ਸੁਨੇਹਾ_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ note_ਅੰਕੜੇ *d = opt->ਮੁੱਲ;
	ਬਣਤਰ note_ਸੁਨੇਹਾ *ਸੁਨੇਹਾ = xmalloc(ਆਕਾਰ(*ਸੁਨੇਹਾ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ਸੁਨੇਹਾ->buf, ਲੜੀ_ਲੰਬਾਈ(arg));
	strbuf_addstr(&ਸੁਨੇਹਾ->buf, arg);
	ALLOC_GROW_BY(d->messages, d->ਸੁਨੇਹਾ_nr, 1, d->ਸੁਨੇਹਾ_alloc);
	d->messages[d->ਸੁਨੇਹਾ_nr - 1] = ਸੁਨੇਹਾ;
	ਸੁਨੇਹਾ->stripspace = STRIPSPACE;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_file_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ note_ਅੰਕੜੇ *d = opt->ਮੁੱਲ;
	ਬਣਤਰ note_ਸੁਨੇਹਾ *ਸੁਨੇਹਾ = xmalloc(ਆਕਾਰ(*ਸੁਨੇਹਾ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ਸੁਨੇਹਾ->buf , 0);
	ਜੇ (!strcmp(arg, "-")) {
		ਜੇ (strbuf_read(&ਸੁਨੇਹਾ->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} ਨਹੀਂਤਾਂ ਜੇ (strbuf_read_file(&ਸੁਨੇਹਾ->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->ਸੁਨੇਹਾ_nr, 1, d->ਸੁਨੇਹਾ_alloc);
	d->messages[d->ਸੁਨੇਹਾ_nr - 1] = ਸੁਨੇਹਾ;
	ਸੁਨੇਹਾ->stripspace = STRIPSPACE;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_reuse_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ note_ਅੰਕੜੇ *d = opt->ਮੁੱਲ;
	ਬਣਤਰ note_ਸੁਨੇਹਾ *ਸੁਨੇਹਾ = xmalloc(ਆਕਾਰ(*ਸੁਨੇਹਾ));
	ਅੱਖਰ *ਮੁੱਲ;
	ਬਣਤਰ object_id object;
	ਗਿਣਤੀ object_type type;
	ਮਾਪ_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ਸੁਨੇਹਾ->buf, 0);
	ਜੇ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	ਜੇ (!(ਮੁੱਲ = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	ਜੇ (type != OBJ_BLOB) {
		strbuf_release(&ਸੁਨੇਹਾ->buf);
		ਮੁਕਤ(ਮੁੱਲ);
		ਮੁਕਤ(ਸੁਨੇਹਾ);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_ਜੋੜੋ(&ਸੁਨੇਹਾ->buf, ਮੁੱਲ, len);
	ਮੁਕਤ(ਮੁੱਲ);

	ਸੁਨੇਹਾ->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->ਸੁਨੇਹਾ_nr, 1, d->ਸੁਨੇਹਾ_alloc);
	d->messages[d->ਸੁਨੇਹਾ_nr - 1] = ਸੁਨੇਹਾ;
	ਸੁਨੇਹਾ->stripspace = NO_STRIPSPACE;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_reedit_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ note_ਅੰਕੜੇ *d = opt->ਮੁੱਲ;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	ਵਾਪਸ parse_reuse_arg(opt, arg, unset);
}

ਸਥਿਰ ਪੂਰਨ parse_separator_arg(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			       ਪੂਰਨ unset)
{
	ਜੇ (unset)
		*(ਅਚਲ ਅੱਖਰ **)opt->ਮੁੱਲ = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ
		*(ਅਚਲ ਅੱਖਰ **)opt->ਮੁੱਲ = arg ? arg : "\n";
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ notes_copy_from_stdin(ਪੂਰਨ force, ਅਚਲ ਅੱਖਰ *rewrite_cmd)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ notes_rewrite_cfg *c = ਨਿਰਾ;
	ਬਣਤਰ notes_ਰੁੱਖ *t = ਨਿਰਾ;
	ਪੂਰਨ ret = 0;
	ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = "Notes added by 'git notes copy'";

	ਜੇ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		ਜੇ (!c)
			ਵਾਪਸ 0;
	} ਨਹੀਂਤਾਂ {
		init_notes(ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, NOTES_INIT_WRITABLE);
		t = &default_notes_ਰੁੱਖ;
	}

	ਜਦੋਂ (strbuf_getline_lf(&buf, stdin) != EOF) {
		ਬਣਤਰ object_id from_obj, ਮੰਜ਼ਿਲ_obj;
		ਬਣਤਰ string_list ਵੰਡੋ = STRING_LIST_INIT_NODUP;
		ਪੂਰਨ err;

		string_list_ਵੰਡੋ_ਵਿੱਚ_ਰੱਖੋ_f(&ਵੰਡੋ, buf.buf, " ", -1,
					     STRING_LIST_ਵੰਡੋ_TRIM);
		ਜੇ (ਵੰਡੋ.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		ਜੇ (repo_get_oid(the_repository, ਵੰਡੋ.ਚੀਜ਼ਾਂ[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    ਵੰਡੋ.ਚੀਜ਼ਾਂ[0].string);
		ਜੇ (repo_get_oid(the_repository, ਵੰਡੋ.ਚੀਜ਼ਾਂ[1].string, &ਮੰਜ਼ਿਲ_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    ਵੰਡੋ.ਚੀਜ਼ਾਂ[1].string);

		ਜੇ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &ਮੰਜ਼ਿਲ_obj);
		ਨਹੀਂਤਾਂ
			err = copy_note(t, &from_obj, &ਮੰਜ਼ਿਲ_obj, force,
					combine_notes_overwrite);

		ਜੇ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      ਵੰਡੋ.ਚੀਜ਼ਾਂ[0].string, ਵੰਡੋ.ਚੀਜ਼ਾਂ[1].string);
			ret = 1;
		}

		string_list_clear(&ਵੰਡੋ, 0);
	}

	ਜੇ (!rewrite_cmd) {
		commit_notes(the_repository, t, ਸੁਨੇਹਾ);
		free_notes(t);
	} ਨਹੀਂਤਾਂ {
		ਅੰਤ_copy_notes_for_rewrite(the_repository, c, ਸੁਨੇਹਾ);
	}
	strbuf_release(&buf);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਬਣਤਰ notes_ਰੁੱਖ *init_notes_check(ਅਚਲ ਅੱਖਰ *subcommand,
					   ਪੂਰਨ flags)
{
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਅਚਲ ਅੱਖਰ *ref;
	init_notes(ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, flags);
	t = &default_notes_ਰੁੱਖ;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	ਜੇ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	ਵਾਪਸ t;
}

ਸਥਿਰ ਪੂਰਨ list(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਬਣਤਰ object_id object;
	ਅਚਲ ਬਣਤਰ object_id *note;
	ਪੂਰਨ retval = -1;
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	ਜੇ (argc)
		argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
				     git_notes_list_usage, 0);

	ਜੇ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	ਜੇ (argc) {
		ਜੇ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		ਜੇ (note) {
			puts(oid_ਮੰਜ਼ਿਲ_hex(note));
			retval = 0;
		} ਨਹੀਂਤਾਂ
			retval = error(_("no note found for object %s."),
				       oid_ਮੰਜ਼ਿਲ_hex(&object));
	} ਨਹੀਂਤਾਂ
		retval = for_each_note(t, 0, list_each_note, ਨਿਰਾ);

	free_notes(t);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ append_edit(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ);

ਸਥਿਰ ਪੂਰਨ ਜੋੜੋ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	       ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ force = 0, allow_ਖਾਲੀ_ਥਾਵਾਂ = 0;
	ਅਚਲ ਅੱਖਰ *object_ref;
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਬਣਤਰ object_id object, new_note;
	ਅਚਲ ਬਣਤਰ object_id *note;
	ਬਣਤਰ note_ਅੰਕੜੇ d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_ਸੁਨੇਹਾ_arg),
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
		OPT_BOOL(0, "allow-empty", &allow_ਖਾਲੀ_ਥਾਵਾਂ,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_notes_ਜੋੜੋ_usage,
			     PARSE_OPT_KEEP_ARGV0);

	ਜੇ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_ਜੋੜੋ_usage, options);
	}

	ਜੇ (d.ਸੁਨੇਹਾ_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	ਜੇ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ਜੇ (note) {
		ਜੇ (!force) {
			free_notes(t);
			ਜੇ (d.ਸੁਨੇਹਾ_nr) {
				free_note_ਅੰਕੜੇ(&d);
				ਵਾਪਸ error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_ਮੰਜ਼ਿਲ_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			ਵਾਪਸ append_edit(argc, argv, ਅਗੇਤਰ, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_ਮੰਜ਼ਿਲ_hex(&object));
	}

	prepare_note_ਅੰਕੜੇ(&object, &d, note);
	ਜੇ (d.buf.len || allow_ਖਾਲੀ_ਥਾਵਾਂ) {
		write_note_ਅੰਕੜੇ(&d, &new_note);
		ਜੇ (ਜੋੜੋ_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} ਨਹੀਂਤਾਂ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_ਮੰਜ਼ਿਲ_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_ਅੰਕੜੇ(&d);
	free_notes(t);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ copy(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ retval = 0, force = 0, from_stdin = 0;
	ਅਚਲ ਬਣਤਰ object_id *from_note, *note;
	ਅਚਲ ਅੱਖਰ *object_ref;
	ਬਣਤਰ object_id object, from_obj;
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਅਚਲ ਅੱਖਰ *rewrite_cmd = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_notes_copy_usage,
			     0);

	ਜੇ (from_stdin || rewrite_cmd) {
		ਜੇ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} ਨਹੀਂਤਾਂ {
			ਵਾਪਸ notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	ਜੇ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	ਜੇ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	ਜੇ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ਜੇ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ਜੇ (note) {
		ਜੇ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_ਮੰਜ਼ਿਲ_hex(&object));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_ਮੰਜ਼ਿਲ_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	ਜੇ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_ਮੰਜ਼ਿਲ_hex(&from_obj));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (ਜੋੜੋ_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
ਬਾਹਰਲਾ:
	free_notes(t);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ append_edit(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ allow_ਖਾਲੀ_ਥਾਵਾਂ = 0;
	ਅਚਲ ਅੱਖਰ *object_ref;
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਬਣਤਰ object_id object, new_note;
	ਅਚਲ ਬਣਤਰ object_id *note;
	ਅੱਖਰ *logmsg;
	ਅਚਲ ਅੱਖਰ * ਅਚਲ *usage;
	ਬਣਤਰ note_ਅੰਕੜੇ d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	ਬਣਤਰ option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_ਸੁਨੇਹਾ_arg),
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
		OPT_BOOL(0, "allow-empty", &allow_ਖਾਲੀ_ਥਾਵਾਂ,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_ਸਿਰਾ()
	};
	ਪੂਰਨ edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	ਜੇ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	ਜੇ (d.ਸੁਨੇਹਾ_nr) {
		concat_messages(&d);
		ਜੇ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ਜੇ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_ਅੰਕੜੇ(&object, &d, edit && note ? note : ਨਿਰਾ);

	ਜੇ (note && !edit) {
		/* Append buf to previous note contents */
		ਮਾਪ_t ਮਾਪ;
		ਗਿਣਤੀ object_type type;
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਅੱਖਰ *prev_buf = odb_read_object(the_repository->objects, note, &type, &ਮਾਪ);

		ਜੇ (!prev_buf)
			die(_("unable to read %s"), oid_ਮੰਜ਼ਿਲ_hex(note));
		ਜੇ (ਮਾਪ)
			strbuf_ਜੋੜੋ(&buf, prev_buf, ਮਾਪ);
		ਜੇ (d.buf.len && ਮਾਪ)
			append_separator(&buf);
		strbuf_ਪਾਓ(&d.buf, 0, buf.buf, buf.len);

		ਮੁਕਤ(prev_buf);
		strbuf_release(&buf);
	}

	ਜੇ (d.buf.len || allow_ਖਾਲੀ_ਥਾਵਾਂ) {
		write_note_ਅੰਕੜੇ(&d, &new_note);
		ਜੇ (ਜੋੜੋ_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} ਨਹੀਂਤਾਂ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_ਮੰਜ਼ਿਲ_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	ਮੁਕਤ(logmsg);
	free_note_ਅੰਕੜੇ(&d);
	free_notes(t);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ show(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਅਚਲ ਅੱਖਰ *object_ref;
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਬਣਤਰ object_id object;
	ਅਚਲ ਬਣਤਰ object_id *note;
	ਪੂਰਨ retval;
	ਬਣਤਰ option options[] = {
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_notes_show_usage,
			     0);

	ਜੇ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	ਜੇ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	ਜੇ (!note)
		retval = error(_("no note found for object %s."),
			       oid_ਮੰਜ਼ਿਲ_hex(&object));
	ਨਹੀਂਤਾਂ {
		ਅਚਲ ਅੱਖਰ *show_ਦਲੀਲਾਂ[3] = {"show", oid_ਮੰਜ਼ਿਲ_hex(note), ਨਿਰਾ};
		retval = execv_git_cmd(show_ਦਲੀਲਾਂ);
	}
	free_notes(t);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ_abort(ਬਣਤਰ notes_ਮਿਲਾਓ_options *o)
{
	ਪੂਰਨ ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	ਜੇ (refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ, "NOTES_MERGE_PARTIAL", ਨਿਰਾ, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	ਜੇ (refs_delete_ref(get_main_ref_store(the_repository), ਨਿਰਾ, "NOTES_MERGE_REF", ਨਿਰਾ, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	ਜੇ (notes_ਮਿਲਾਓ_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ_commit(ਬਣਤਰ notes_ਮਿਲਾਓ_options *o)
{
	ਬਣਤਰ strbuf ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ object_id oid, ਮਾਪਾ_oid;
	ਬਣਤਰ notes_ਰੁੱਖ t = {0};
	ਬਣਤਰ commit *partial;
	ਬਣਤਰ pretty_print_context pretty_ctx;
	ਖਾਲੀ *local_ref_ਮੰਜ਼ਿਲ_free;
	ਪੂਰਨ ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	ਜੇ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	ਨਹੀਂਤਾਂ ਜੇ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	ਨਹੀਂਤਾਂ ਜੇ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	ਜੇ (partial->parents)
		oidcpy(&ਮਾਪਾ_oid, &partial->parents->ਵਸਤੂ->object.oid);
	ਨਹੀਂਤਾਂ
		oidclr(&ਮਾਪਾ_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_ਮੰਜ਼ਿਲ_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ਨਿਰਾ);
	ਜੇ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	ਜੇ (notes_ਮਿਲਾਓ_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, ਆਕਾਰ(pretty_ctx));
	repo_format_commit_ਸੁਨੇਹਾ(the_repository, partial, "%s", &ਸੁਨੇਹਾ,
				   &pretty_ctx);
	strbuf_trim(&ਸੁਨੇਹਾ);
	strbuf_insertstr(&ਸੁਨੇਹਾ, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ.buf,
			o->local_ref, &oid,
			is_null_oid(&ਮਾਪਾ_oid) ? ਨਿਰਾ : &ਮਾਪਾ_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&ਸੁਨੇਹਾ);
	ret = ਮਿਲਾਓ_abort(o);
	ਮੁਕਤ(local_ref_ਮੰਜ਼ਿਲ_free);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ git_config_get_notes_strategy(ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
					 ਗਿਣਤੀ notes_ਮਿਲਾਓ_strategy *strategy)
{
	ਅੱਖਰ *ਮੁੱਲ;

	ਜੇ (repo_config_get_string(the_repository, ਕੁੰਜੀ, &ਮੁੱਲ))
		ਵਾਪਸ 1;
	ਜੇ (parse_notes_ਮਿਲਾਓ_strategy(ਮੁੱਲ, strategy))
		git_die_config(the_repository, ਕੁੰਜੀ, _("unknown notes merge strategy %s"), ਮੁੱਲ);

	ਮੁਕਤ(ਮੁੱਲ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਮਿਲਾਓ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ strbuf remote_ref = STRBUF_INIT, ਸੁਨੇਹਾ = STRBUF_INIT;
	ਬਣਤਰ object_id ਨਤੀਜਾ_oid;
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਬਣਤਰ notes_ਮਿਲਾਓ_options o;
	ਪੂਰਨ do_ਮਿਲਾਓ = 0, do_commit = 0, do_abort = 0;
	ਪੂਰਨ verbosity = 0, ਨਤੀਜਾ;
	ਅਚਲ ਅੱਖਰ *strategy = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_ਸਮੂਹ(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_ਸਮੂਹ(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_ਸਮੂਹ(N_("Committing unmerged notes")),
		OPT_ਸੈੱਟ_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸਮੂਹ(N_("Aborting notes merge resolution")),
		OPT_ਸੈੱਟ_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_ਸਿਰਾ()
	};
	ਅੱਖਰ *notes_ref;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_notes_ਮਿਲਾਓ_usage, 0);

	ਜੇ (strategy || do_commit + do_abort == 0)
		do_ਮਿਲਾਓ = 1;
	ਜੇ (do_ਮਿਲਾਓ + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_ਮਿਲਾਓ_usage, options);
	}

	ਜੇ (do_ਮਿਲਾਓ && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_ਮਿਲਾਓ_usage, options);
	} ਨਹੀਂਤਾਂ ਜੇ (!do_ਮਿਲਾਓ && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_ਮਿਲਾਓ_usage, options);
	}

	init_notes_ਮਿਲਾਓ_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_ਮਿਲਾਓ_VERBOSITY_DEFAULT;

	ਜੇ (do_abort)
		ਵਾਪਸ ਮਿਲਾਓ_abort(&o);
	ਜੇ (do_commit)
		ਵਾਪਸ ਮਿਲਾਓ_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	ਜੇ (strategy) {
		ਜੇ (parse_notes_ਮਿਲਾਓ_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_ਮਿਲਾਓ_usage, options);
		}
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ strbuf ਮਿਲਾਓ_ਕੁੰਜੀ = STRBUF_INIT;
		ਅਚਲ ਅੱਖਰ *short_ref = ਨਿਰਾ;

		ਜੇ (!skip_ਅਗੇਤਰ(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&ਮਿਲਾਓ_ਕੁੰਜੀ, "notes.%s.mergeStrategy", short_ref);

		ਜੇ (git_config_get_notes_strategy(ਮਿਲਾਓ_ਕੁੰਜੀ.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&ਮਿਲਾਓ_ਕੁੰਜੀ);
	}

	strbuf_addf(&ਸੁਨੇਹਾ, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_ਜੋੜੋ(&(o.commit_ਸੁਨੇਹਾ), ਸੁਨੇਹਾ.buf + 7, ਸੁਨੇਹਾ.len - 7); /* skip "notes: " */

	ਨਤੀਜਾ = notes_ਮਿਲਾਓ(&o, t, &ਨਤੀਜਾ_oid);

	ਜੇ (ਨਤੀਜਾ >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ.buf,
				notes_ref, &ਨਤੀਜਾ_oid, ਨਿਰਾ, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ਨਹੀਂਤਾਂ { /* Merge has unresolved conflicts */
		ਬਣਤਰ worktree **worktrees;
		ਅਚਲ ਬਣਤਰ worktree *wt;
		ਅੱਖਰ *ਰਸਤਾ;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), ਸੁਨੇਹਾ.buf,
				"NOTES_MERGE_PARTIAL", &ਨਤੀਜਾ_oid, ਨਿਰਾ,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = ਲੱਭੋ_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		ਜੇ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->ਰਸਤਾ);
		free_worktrees(worktrees);
		ਜੇ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ਨਿਰਾ))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		ਰਸਤਾ = repo_git_ਰਸਤਾ(the_repository, NOTES_ਮਿਲਾਓ_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			ਰਸਤਾ);
		ਮੁਕਤ(ਰਸਤਾ);
	}

	free_notes(t);
	ਮੁਕਤ(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&ਸੁਨੇਹਾ);
	ਵਾਪਸ ਨਤੀਜਾ < 0; /* return non-zero on conflicts */
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IGNORE_MISSING 1

ਸਥਿਰ ਪੂਰਨ remove_one_note(ਬਣਤਰ notes_ਰੁੱਖ *t, ਅਚਲ ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ flag)
{
	ਪੂਰਨ ਸਥਿਤੀ;
	ਬਣਤਰ object_id oid;
	ਜੇ (repo_get_oid(the_repository, name, &oid))
		ਵਾਪਸ error(_("Failed to resolve '%s' as a valid ref."), name);
	ਸਥਿਤੀ = remove_note(t, oid.hash);
	ਜੇ (ਸਥਿਤੀ)
		fprintf(stderr, _("Object %s has no note\n"), name);
	ਨਹੀਂਤਾਂ
		fprintf(stderr, _("Removing note for object %s\n"), name);
	ਵਾਪਸ (flag & IGNORE_MISSING) ? 0 : ਸਥਿਤੀ;
}

ਸਥਿਰ ਪੂਰਨ remove_cmd(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		      ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ flag = 0;
	ਪੂਰਨ from_stdin = 0;
	ਬਣਤਰ option options[] = {
		OPT_ਦੋਆਧਾਰੀ_ਅੰਕ(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_ਸਿਰਾ()
	};
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਪੂਰਨ retval = 0;

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	ਜੇ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} ਨਹੀਂਤਾਂ {
		ਜਦੋਂ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	ਜੇ (from_stdin) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		ਜਦੋਂ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	ਜੇ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ prune(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		 ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ notes_ਰੁੱਖ *t;
	ਪੂਰਨ show_only = 0, verbose = 0;
	ਬਣਤਰ option options[] = {
		OPT_DRY_ਲੜੀਵਾਰ(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_ਸਿਰਾ()
	};

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_notes_prune_usage,
			     0);

	ਜੇ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	ਜੇ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ get_ref(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ option options[] = { OPT_ਸਿਰਾ() };
	ਅੱਖਰ *notes_ref;
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_notes_get_ref_usage, 0);

	ਜੇ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	ਮੁਕਤ(notes_ref);
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_notes(ਪੂਰਨ argc,
	      ਅਚਲ ਅੱਖਰ **argv,
	      ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
	      ਬਣਤਰ repository *repo)
{
	ਅਚਲ ਅੱਖਰ *override_notes_ref = ਨਿਰਾ;
	parse_opt_subcommand_fn *fn = ਨਿਰਾ;
	ਬਣਤਰ option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, ਜੋੜੋ),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, ਮਿਲਾਓ),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);
	argc = parse_options(argc, argv, ਅਗੇਤਰ, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	ਜੇ (!fn) {
		ਜੇ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	ਜੇ (override_notes_ref) {
		ਬਣਤਰ strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	ਵਾਪਸ !!fn(argc, argv, ਅਗੇਤਰ, repo);
}
