/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "editor.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "hex.h"
#ଅନ୍ତର୍ଭାଵ "notes.h"
#ଅନ୍ତର୍ଭାଵ "object-file.h"
#ଅନ୍ତର୍ଭାଵ "object-name.h"
#ଅନ୍ତର୍ଭାଵ "odb.h"
#ଅନ୍ତର୍ଭାଵ "path.h"

#ଅନ୍ତର୍ଭାଵ "pretty.h"
#ଅନ୍ତର୍ଭାଵ "refs.h"
#ଅନ୍ତର୍ଭାଵ "exec-cmd.h"
#ଅନ୍ତର୍ଭାଵ "run-command.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "string-list.h"
#ଅନ୍ତର୍ଭାଵ "notes-merge.h"
#ଅନ୍ତର୍ଭାଵ "notes-utils.h"
#ଅନ୍ତର୍ଭାଵ "worktree.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *separator = "\n";
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_usage[] = {
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
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ git_notes_get_ref_usage[] = {
	"git notes get-ref",
	ଖାଲି
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର note_template[] =
	N_("Write/edit the notes for the following object:");

ଗଣନା notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

ସଂରଚନା note_ସନ୍ଦେଶ {
	ଗଣନା notes_stripspace stripspace;
	ସଂରଚନା strbuf buf;
};

ସଂରଚନା note_data {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା use_editor;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା stripspace;
	ଅକ୍ଷର *edit_path;
	ସଂରଚନା strbuf buf;
	ସଂରଚନା note_ସନ୍ଦେଶ **messages;
	size_t ସନ୍ଦେଶ_nr;
	size_t ସନ୍ଦେଶ_alloc;
};

ସ୍ଥିର ଶୂନ୍ୟ free_note_data(ସଂରଚନା note_data *d)
{
	ଯଦି (d->edit_path) {
		unlink_or_warn(d->edit_path);
		ମୁକ୍ତ(d->edit_path);
	}
	strbuf_release(&d->buf);

	ଯେତେବେଳେ (d->ସନ୍ଦେଶ_nr--) {
		strbuf_release(&d->messages[d->ସନ୍ଦେଶ_nr]->buf);
		ମୁକ୍ତ(d->messages[d->ସନ୍ଦେଶ_nr]);
	}
	ମୁକ୍ତ(d->messages);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list_each_note(ଅଚଳ ସଂରଚନା object_id *object_oid,
			  ଅଚଳ ସଂରଚନା object_id *note_oid,
			  ଅକ୍ଷର *note_path UNUSED,
			  ଶୂନ୍ୟ *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ copy_obj_to_fd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା object_id *oid)
{
	size_t size;
	ଗଣନା object_type type;
	ଅକ୍ଷର *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ଯଦି (buf) {
		ଯଦି (size)
			write_or_die(fd, buf, size);
		ମୁକ୍ତ(buf);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ write_commented_object(ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ସଂରଚନା object_id *object)
{
	ସଂରଚନା child_process show = CHILD_PROCESS_INIT;
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), ଖାଲି);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	ଯଦି (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	ଯଦି (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_ଧାଡ଼ିଗୁଡ଼ିକ(&cbuf, buf.buf, buf.len, comment_ଧାଡ଼ି_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	ଯଦି (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

ସ୍ଥିର ଶୂନ୍ୟ prepare_note_data(ଅଚଳ ସଂରଚନା object_id *object, ସଂରଚନା note_data *d,
		ଅଚଳ ସଂରଚନା object_id *old_note)
{
	ଯଦି (d->use_editor || !d->ସନ୍ଦେଶ_nr) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
		ସଂରଚନା strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ଯଦି (d->ସନ୍ଦେଶ_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		ଅନ୍ୟଥା ଯଦି (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_ଧାଡ଼ିଗୁଡ଼ିକ(&buf, "\n", strlen("\n"), comment_ଧାଡ଼ି_str);
		strbuf_add_commented_ଧାଡ଼ିଗୁଡ଼ିକ(&buf, _(note_template), strlen(_(note_template)),
					   comment_ଧାଡ଼ି_str);
		strbuf_add_commented_ଧାଡ଼ିଗୁଡ଼ିକ(&buf, "\n", strlen("\n"), comment_ଧାଡ଼ି_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		ଯଦି (launch_editor(d->edit_path, &d->buf, ଖାଲି)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		ଯଦି (d->stripspace)
			strbuf_stripspace(&d->buf, comment_ଧାଡ଼ି_str);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ write_note_data(ସଂରଚନା note_data *d, ସଂରଚନା object_id *oid)
{
	ଯଦି (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = die_message(_("unable to write note object"));

		ଯଦି (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ append_separator(ସଂରଚନା strbuf *message)
{
	size_t sep_len = 0;

	ଯଦି (!separator)
		ଫେରନ୍ତୁ;
	ଅନ୍ୟଥା ଯଦି ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	ଅନ୍ୟଥା
		strbuf_addf(message, "%s%s", separator, "\n");
}

ସ୍ଥିର ଶୂନ୍ୟ concat_messages(ସଂରଚନା note_data *d)
{
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	size_t i;

	ପାଇଁ (i = 0; i < d->ସନ୍ଦେଶ_nr ; i++) {
		ଯଦି (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&ସନ୍ଦେଶ, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &ସନ୍ଦେଶ);
		ଯଦି ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, ଖାଲି);
		strbuf_reset(&ସନ୍ଦେଶ);
	}
	strbuf_release(&ସନ୍ଦେଶ);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_ସନ୍ଦେଶ_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା note_data *d = opt->ମୂଲ୍ୟ;
	ସଂରଚନା note_ସନ୍ଦେଶ *ସନ୍ଦେଶ = xmalloc(ଆକାର(*ସନ୍ଦେଶ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ସନ୍ଦେଶ->buf, strlen(arg));
	strbuf_addstr(&ସନ୍ଦେଶ->buf, arg);
	ALLOC_GROW_BY(d->messages, d->ସନ୍ଦେଶ_nr, 1, d->ସନ୍ଦେଶ_alloc);
	d->messages[d->ସନ୍ଦେଶ_nr - 1] = ସନ୍ଦେଶ;
	ସନ୍ଦେଶ->stripspace = STRIPSPACE;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_file_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା note_data *d = opt->ମୂଲ୍ୟ;
	ସଂରଚନା note_ସନ୍ଦେଶ *ସନ୍ଦେଶ = xmalloc(ଆକାର(*ସନ୍ଦେଶ));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ସନ୍ଦେଶ->buf , 0);
	ଯଦି (!strcmp(arg, "-")) {
		ଯଦି (strbuf_read(&ସନ୍ଦେଶ->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} ଅନ୍ୟଥା ଯଦି (strbuf_read_file(&ସନ୍ଦେଶ->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->ସନ୍ଦେଶ_nr, 1, d->ସନ୍ଦେଶ_alloc);
	d->messages[d->ସନ୍ଦେଶ_nr - 1] = ସନ୍ଦେଶ;
	ସନ୍ଦେଶ->stripspace = STRIPSPACE;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_reuse_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା note_data *d = opt->ମୂଲ୍ୟ;
	ସଂରଚନା note_ସନ୍ଦେଶ *ସନ୍ଦେଶ = xmalloc(ଆକାର(*ସନ୍ଦେଶ));
	ଅକ୍ଷର *ମୂଲ୍ୟ;
	ସଂରଚନା object_id object;
	ଗଣନା object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&ସନ୍ଦେଶ->buf, 0);
	ଯଦି (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	ଯଦି (!(ମୂଲ୍ୟ = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	ଯଦି (type != OBJ_BLOB) {
		strbuf_release(&ସନ୍ଦେଶ->buf);
		ମୁକ୍ତ(ମୂଲ୍ୟ);
		ମୁକ୍ତ(ସନ୍ଦେଶ);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&ସନ୍ଦେଶ->buf, ମୂଲ୍ୟ, len);
	ମୁକ୍ତ(ମୂଲ୍ୟ);

	ସନ୍ଦେଶ->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->ସନ୍ଦେଶ_nr, 1, d->ସନ୍ଦେଶ_alloc);
	d->messages[d->ସନ୍ଦେଶ_nr - 1] = ସନ୍ଦେଶ;
	ସନ୍ଦେଶ->stripspace = NO_STRIPSPACE;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_reedit_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ସଂରଚନା note_data *d = opt->ମୂଲ୍ୟ;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	ଫେରନ୍ତୁ parse_reuse_arg(opt, arg, unset);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_separator_arg(ଅଚଳ ସଂରଚନା option *opt, ଅଚଳ ଅକ୍ଷର *arg,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା unset)
{
	ଯଦି (unset)
		*(ଅଚଳ ଅକ୍ଷର **)opt->ମୂଲ୍ୟ = ଖାଲି;
	ଅନ୍ୟଥା
		*(ଅଚଳ ଅକ୍ଷର **)opt->ମୂଲ୍ୟ = arg ? arg : "\n";
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା notes_copy_from_stdin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା force, ଅଚଳ ଅକ୍ଷର *rewrite_cmd)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା notes_rewrite_cfg *c = ଖାଲି;
	ସଂରଚନା notes_ବୃକ୍ଷ *t = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = "Notes added by 'git notes copy'";

	ଯଦି (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		ଯଦି (!c)
			ଫେରନ୍ତୁ 0;
	} ଅନ୍ୟଥା {
		init_notes(ଖାଲି, ଖାଲି, ଖାଲି, NOTES_INIT_WRITABLE);
		t = &default_notes_ବୃକ୍ଷ;
	}

	ଯେତେବେଳେ (strbuf_getline_lf(&buf, stdin) != EOF) {
		ସଂରଚନା object_id from_obj, to_obj;
		ସଂରଚନା string_list split = STRING_LIST_INIT_NODUP;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		ଯଦି (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		ଯଦି (repo_get_oid(the_repository, split.ବସ୍ତୁଗୁଡ଼ିକ[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.ବସ୍ତୁଗୁଡ଼ିକ[0].string);
		ଯଦି (repo_get_oid(the_repository, split.ବସ୍ତୁଗୁଡ଼ିକ[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.ବସ୍ତୁଗୁଡ଼ିକ[1].string);

		ଯଦି (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		ଅନ୍ୟଥା
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		ଯଦି (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.ବସ୍ତୁଗୁଡ଼ିକ[0].string, split.ବସ୍ତୁଗୁଡ଼ିକ[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	ଯଦି (!rewrite_cmd) {
		commit_notes(the_repository, t, ସନ୍ଦେଶ);
		free_notes(t);
	} ଅନ୍ୟଥା {
		finish_copy_notes_for_rewrite(the_repository, c, ସନ୍ଦେଶ);
	}
	strbuf_release(&buf);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ସଂରଚନା notes_ବୃକ୍ଷ *init_notes_check(ଅଚଳ ଅକ୍ଷର *subcommand,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ଅଚଳ ଅକ୍ଷର *ref;
	init_notes(ଖାଲି, ଖାଲି, ଖାଲି, flags);
	t = &default_notes_ବୃକ୍ଷ;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	ଯଦି (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	ଫେରନ୍ତୁ t;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା list(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ସଂରଚନା object_id object;
	ଅଚଳ ସଂରଚନା object_id *note;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = -1;
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	ଯଦି (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	ଯଦି (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	ଯଦି (argc) {
		ଯଦି (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		ଯଦି (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} ଅନ୍ୟଥା
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} ଅନ୍ୟଥା
		retval = for_each_note(t, 0, list_each_note, ଖାଲି);

	free_notes(t);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_edit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା add(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
	       ସଂରଚନା repository *repo)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା force = 0, allow_empty = 0;
	ଅଚଳ ଅକ୍ଷର *object_ref;
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ସଂରଚନା object_id object, new_note;
	ଅଚଳ ସଂରଚନା object_id *note;
	ସଂରଚନା note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_ସନ୍ଦେଶ_arg),
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

	ଯଦି (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	ଯଦି (d.ସନ୍ଦେଶ_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	ଯଦି (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ଯଦି (note) {
		ଯଦି (!force) {
			free_notes(t);
			ଯଦି (d.ସନ୍ଦେଶ_nr) {
				free_note_data(&d);
				ଫେରନ୍ତୁ error(_("Cannot add notes. "
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
			ଫେରନ୍ତୁ append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	ଯଦି (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		ଯଦି (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} ଅନ୍ୟଥା {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0, force = 0, from_stdin = 0;
	ଅଚଳ ସଂରଚନା object_id *from_note, *note;
	ଅଚଳ ଅକ୍ଷର *object_ref;
	ସଂରଚନା object_id object, from_obj;
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ଅଚଳ ଅକ୍ଷର *rewrite_cmd = ଖାଲି;
	ସଂରଚନା option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	ଯଦି (from_stdin || rewrite_cmd) {
		ଯଦି (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} ଅନ୍ୟଥା {
			ଫେରନ୍ତୁ notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	ଯଦି (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	ଯଦି (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	ଯଦି (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ଯଦି (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ଯଦି (note) {
		ଯଦି (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			ଯାଅ out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	ଯଦି (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		ଯାଅ out;
	}

	ଯଦି (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା append_edit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		       ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା allow_empty = 0;
	ଅଚଳ ଅକ୍ଷର *object_ref;
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ସଂରଚନା object_id object, new_note;
	ଅଚଳ ସଂରଚନା object_id *note;
	ଅକ୍ଷର *logmsg;
	ଅଚଳ ଅକ୍ଷର * ଅଚଳ *usage;
	ସଂରଚନା note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	ସଂରଚନା option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_ସନ୍ଦେଶ_arg),
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	ଯଦି (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	ଯଦି (d.ସନ୍ଦେଶ_nr) {
		concat_messages(&d);
		ଯଦି (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ଯଦି (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : ଖାଲି);

	ଯଦି (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		ଗଣନା object_type type;
		ସଂରଚନା strbuf buf = STRBUF_INIT;
		ଅକ୍ଷର *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		ଯଦି (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		ଯଦି (size)
			strbuf_add(&buf, prev_buf, size);
		ଯଦି (d.buf.len && size)
			append_separator(&buf);
		strbuf_ଭର୍ତ୍ତି(&d.buf, 0, buf.buf, buf.len);

		ମୁକ୍ତ(prev_buf);
		strbuf_release(&buf);
	}

	ଯଦି (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		ଯଦି (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} ଅନ୍ୟଥା {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	ମୁକ୍ତ(logmsg);
	free_note_data(&d);
	free_notes(t);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା show(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		ସଂରଚନା repository *repo UNUSED)
{
	ଅଚଳ ଅକ୍ଷର *object_ref;
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ସଂରଚନା object_id object;
	ଅଚଳ ସଂରଚନା object_id *note;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;
	ସଂରଚନା option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	ଯଦି (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	ଯଦି (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	ଯଦି (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	ଅନ୍ୟଥା {
		ଅଚଳ ଅକ୍ଷର *show_args[3] = {"show", oid_to_hex(note), ଖାଲି};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_abort(ସଂରଚନା notes_merge_options *o)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	ଯଦି (refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି, "NOTES_MERGE_PARTIAL", ଖାଲି, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	ଯଦି (refs_delete_ref(get_main_ref_store(the_repository), ଖାଲି, "NOTES_MERGE_REF", ଖାଲି, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	ଯଦି (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge_commit(ସଂରଚନା notes_merge_options *o)
{
	ସଂରଚନା strbuf ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା object_id oid, parent_oid;
	ସଂରଚନା notes_ବୃକ୍ଷ t = {0};
	ସଂରଚନା commit *partial;
	ସଂରଚନା pretty_print_context pretty_ctx;
	ଶୂନ୍ୟ *local_ref_to_free;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	ଯଦି (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	ଅନ୍ୟଥା ଯଦି (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	ଅନ୍ୟଥା ଯଦି (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	ଯଦି (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	ଅନ୍ୟଥା
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, ଖାଲି);
	ଯଦି (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	ଯଦି (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, ଆକାର(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &ସନ୍ଦେଶ,
				   &pretty_ctx);
	strbuf_trim(&ସନ୍ଦେଶ);
	strbuf_insertstr(&ସନ୍ଦେଶ, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? ଖାଲି : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&ସନ୍ଦେଶ);
	ret = merge_abort(o);
	ମୁକ୍ତ(local_ref_to_free);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା git_config_get_notes_strategy(ଅଚଳ ଅକ୍ଷର *ଚାବି,
					 ଗଣନା notes_merge_strategy *strategy)
{
	ଅକ୍ଷର *ମୂଲ୍ୟ;

	ଯଦି (repo_config_get_string(the_repository, ଚାବି, &ମୂଲ୍ୟ))
		ଫେରନ୍ତୁ 1;
	ଯଦି (parse_notes_merge_strategy(ମୂଲ୍ୟ, strategy))
		git_die_config(the_repository, ଚାବି, _("unknown notes merge strategy %s"), ମୂଲ୍ୟ);

	ମୁକ୍ତ(ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା merge(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା strbuf remote_ref = STRBUF_INIT, ସନ୍ଦେଶ = STRBUF_INIT;
	ସଂରଚନା object_id result_oid;
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ସଂରଚନା notes_merge_options o;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_merge = 0, do_commit = 0, do_abort = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity = 0, result;
	ଅଚଳ ଅକ୍ଷର *strategy = ଖାଲି;
	ସଂରଚନା option options[] = {
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
	ଅକ୍ଷର *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	ଯଦି (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	ଯଦି (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	ଯଦି (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} ଅନ୍ୟଥା ଯଦି (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	ଯଦି (do_abort)
		ଫେରନ୍ତୁ merge_abort(&o);
	ଯଦି (do_commit)
		ଫେରନ୍ତୁ merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	ଯଦି (strategy) {
		ଯଦି (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} ଅନ୍ୟଥା {
		ସଂରଚନା strbuf merge_ଚାବି = STRBUF_INIT;
		ଅଚଳ ଅକ୍ଷର *short_ref = ଖାଲି;

		ଯଦି (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_ଚାବି, "notes.%s.mergeStrategy", short_ref);

		ଯଦି (git_config_get_notes_strategy(merge_ଚାବି.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_ଚାବି);
	}

	strbuf_addf(&ସନ୍ଦେଶ, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_ସନ୍ଦେଶ), ସନ୍ଦେଶ.buf + 7, ସନ୍ଦେଶ.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	ଯଦି (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ.buf,
				notes_ref, &result_oid, ଖାଲି, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ଅନ୍ୟଥା { /* Merge has unresolved conflicts */
		ସଂରଚନା worktree **worktrees;
		ଅଚଳ ସଂରଚନା worktree *wt;
		ଅକ୍ଷର *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), ସନ୍ଦେଶ.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, ଖାଲି,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		ଯଦି (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		ଯଦି (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, ଖାଲି))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		ମୁକ୍ତ(path);
	}

	free_notes(t);
	ମୁକ୍ତ(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&ସନ୍ଦେଶ);
	ଫେରନ୍ତୁ result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_one_note(ସଂରଚନା notes_ବୃକ୍ଷ *t, ଅଚଳ ଅକ୍ଷର *name, unsigned flag)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
	ସଂରଚନା object_id oid;
	ଯଦି (repo_get_oid(the_repository, name, &oid))
		ଫେରନ୍ତୁ error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	ଯଦି (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	ଅନ୍ୟଥା
		fprintf(stderr, _("Removing note for object %s\n"), name);
	ଫେରନ୍ତୁ (flag & IGNORE_MISSING) ? 0 : status;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା remove_cmd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		      ସଂରଚନା repository *repo UNUSED)
{
	unsigned flag = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା from_stdin = 0;
	ସଂରଚନା option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	ଯଦି (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} ଅନ୍ୟଥା {
		ଯେତେବେଳେ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	ଯଦି (from_stdin) {
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		ଯେତେବେଳେ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	ଯଦି (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା prune(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		 ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା notes_ବୃକ୍ଷ *t;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_only = 0, verbose = 0;
	ସଂରଚନା option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	ଯଦି (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	ଯଦି (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ref(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv, ଅଚଳ ଅକ୍ଷର *prefix,
		   ସଂରଚନା repository *repo UNUSED)
{
	ସଂରଚନା option options[] = { OPT_END() };
	ଅକ୍ଷର *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	ଯଦି (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	ମୁକ୍ତ(notes_ref);
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_notes(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
	      ଅଚଳ ଅକ୍ଷର **argv,
	      ଅଚଳ ଅକ୍ଷର *prefix,
	      ସଂରଚନା repository *repo)
{
	ଅଚଳ ଅକ୍ଷର *override_notes_ref = ଖାଲି;
	parse_opt_subcommand_fn *fn = ଖାଲି;
	ସଂରଚନା option options[] = {
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

	repo_config(the_repository, git_default_config, ଖାଲି);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	ଯଦି (!fn) {
		ଯଦି (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	ଯଦି (override_notes_ref) {
		ସଂରଚନା strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	ଫେରନ୍ତୁ !!fn(argc, argv, prefix, repo);
}
