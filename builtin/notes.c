/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "config.h"
#include "editor.h"
#include "environment.h"
#include "gettext.h"
#include "hex.h"
#include "notes.h"
#include "object-file.h"
#include "object-name.h"
#include "odb.h"
#include "path.h"

#include "pretty.h"
#include "refs.h"
#include "exec-cmd.h"
#include "run-command.h"
#include "parse-options.h"
#include "string-list.h"
#include "notes-merge.h"
#include "notes-utils.h"
#include "worktree.h"
#include "write-or-die.h"

static const var_na *separator = "\n";
static const var_na * const git_notes_usage[] = {
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
	NULL
};

static const var_na * const git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	NULL
};

static const var_na * const git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	NULL
};

static const var_na * const git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	NULL
};

static const var_na * const git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	NULL
};

static const var_na * const git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	NULL
};

static const var_na * const git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	NULL
};

static const var_na * const git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	NULL
};

static const var_na * const git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	NULL
};

static const var_na * const git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	NULL
};

static const var_na * const git_notes_get_ref_usage[] = {
	"git notes get-ref",
	NULL
};

static const var_na note_template[] =
	N_("Write/edit the notes for the following object:");

ga_nanaa notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

sa_nracanaa note_msg {
	ga_nanaa notes_stripspace stripspace;
	sa_nracanaa strbuf buf;
};

sa_nracanaa note_data {
	puur_naa_nka use_editor;
	puur_naa_nka stripspace;
	var_na *edit_path;
	sa_nracanaa strbuf buf;
	sa_nracanaa note_msg **messages;
	size_t msg_nr;
	size_t msg_alloc;
};

static void free_note_data(sa_nracanaa note_data *d)
{
	ᱡᱩᱫᱤ (d->edit_path) {
		unlink_or_warn(d->edit_path);
		free(d->edit_path);
	}
	strbuf_release(&d->buf);

	ᱡᱚᱠᱷᱚᱱ (d->msg_nr--) {
		strbuf_release(&d->messages[d->msg_nr]->buf);
		free(d->messages[d->msg_nr]);
	}
	free(d->messages);
}

static puur_naa_nka list_each_note(const sa_nracanaa object_id *object_oid,
			  const sa_nracanaa object_id *note_oid,
			  var_na *note_path UNUSED,
			  void *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	ᱨᱩᱣᱟᱹᱲ 0;
}

static void copy_obj_to_fd(puur_naa_nka fd, const sa_nracanaa object_id *oid)
{
	size_t size;
	ga_nanaa object_type type;
	var_na *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	ᱡᱩᱫᱤ (buf) {
		ᱡᱩᱫᱤ (size)
			write_or_die(fd, buf, size);
		free(buf);
	}
}

static void write_commented_object(puur_naa_nka fd, const sa_nracanaa object_id *object)
{
	sa_nracanaa child_process show = CHILD_PROCESS_INIT;
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), NULL);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	ᱡᱩᱫᱤ (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	ᱡᱩᱫᱤ (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_lines(&cbuf, buf.buf, buf.len, comment_line_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	ᱡᱩᱫᱤ (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

static void prepare_note_data(const sa_nracanaa object_id *object, sa_nracanaa note_data *d,
		const sa_nracanaa object_id *old_note)
{
	ᱡᱩᱫᱤ (d->use_editor || !d->msg_nr) {
		puur_naa_nka fd;
		sa_nracanaa strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		ᱡᱩᱫᱤ (d->msg_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		ᱵᱟᱝ ᱡᱩᱫᱤ (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_lines(&buf, "\n", strlen("\n"), comment_line_str);
		strbuf_add_commented_lines(&buf, _(note_template), strlen(_(note_template)),
					   comment_line_str);
		strbuf_add_commented_lines(&buf, "\n", strlen("\n"), comment_line_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		ᱡᱩᱫᱤ (launch_editor(d->edit_path, &d->buf, NULL)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		ᱡᱩᱫᱤ (d->stripspace)
			strbuf_stripspace(&d->buf, comment_line_str);
	}
}

static void write_note_data(sa_nracanaa note_data *d, sa_nracanaa object_id *oid)
{
	ᱡᱩᱫᱤ (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		puur_naa_nka status = die_message(_("unable to write note object"));

		ᱡᱩᱫᱤ (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

static void append_separator(sa_nracanaa strbuf *message)
{
	size_t sep_len = 0;

	ᱡᱩᱫᱤ (!separator)
		ᱨᱩᱣᱟᱹᱲ;
	ᱵᱟᱝ ᱡᱩᱫᱤ ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	ᱵᱟᱝ
		strbuf_addf(message, "%s%s", separator, "\n");
}

static void concat_messages(sa_nracanaa note_data *d)
{
	sa_nracanaa strbuf msg = STRBUF_INIT;
	size_t i;

	for (i = 0; i < d->msg_nr ; i++) {
		ᱡᱩᱫᱤ (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&msg, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &msg);
		ᱡᱩᱫᱤ ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, NULL);
		strbuf_reset(&msg);
	}
	strbuf_release(&msg);
}

static puur_naa_nka parse_msg_arg(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa note_data *d = opt->value;
	sa_nracanaa note_msg *msg = xmalloc(parimaa_na(*msg));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&msg->buf, strlen(arg));
	strbuf_addstr(&msg->buf, arg);
	ALLOC_GROW_BY(d->messages, d->msg_nr, 1, d->msg_alloc);
	d->messages[d->msg_nr - 1] = msg;
	msg->stripspace = STRIPSPACE;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka parse_file_arg(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa note_data *d = opt->value;
	sa_nracanaa note_msg *msg = xmalloc(parimaa_na(*msg));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&msg->buf , 0);
	ᱡᱩᱫᱤ (!strcmp(arg, "-")) {
		ᱡᱩᱫᱤ (strbuf_read(&msg->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (strbuf_read_file(&msg->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->msg_nr, 1, d->msg_alloc);
	d->messages[d->msg_nr - 1] = msg;
	msg->stripspace = STRIPSPACE;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka parse_reuse_arg(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa note_data *d = opt->value;
	sa_nracanaa note_msg *msg = xmalloc(parimaa_na(*msg));
	var_na *value;
	sa_nracanaa object_id object;
	ga_nanaa object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&msg->buf, 0);
	ᱡᱩᱫᱤ (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	ᱡᱩᱫᱤ (!(value = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	ᱡᱩᱫᱤ (type != OBJ_BLOB) {
		strbuf_release(&msg->buf);
		free(value);
		free(msg);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&msg->buf, value, len);
	free(value);

	msg->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->msg_nr, 1, d->msg_alloc);
	d->messages[d->msg_nr - 1] = msg;
	msg->stripspace = NO_STRIPSPACE;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka parse_reedit_arg(const sa_nracanaa option *opt, const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa note_data *d = opt->value;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	ᱨᱩᱣᱟᱹᱲ parse_reuse_arg(opt, arg, unset);
}

static puur_naa_nka parse_separator_arg(const sa_nracanaa option *opt, const var_na *arg,
			       puur_naa_nka unset)
{
	ᱡᱩᱫᱤ (unset)
		*(const var_na **)opt->value = NULL;
	ᱵᱟᱝ
		*(const var_na **)opt->value = arg ? arg : "\n";
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka notes_copy_from_stdin(puur_naa_nka force, const var_na *rewrite_cmd)
{
	sa_nracanaa strbuf buf = STRBUF_INIT;
	sa_nracanaa notes_rewrite_cfg *c = NULL;
	sa_nracanaa notes_tree *t = NULL;
	puur_naa_nka ret = 0;
	const var_na *msg = "Notes added by 'git notes copy'";

	ᱡᱩᱫᱤ (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		ᱡᱩᱫᱤ (!c)
			ᱨᱩᱣᱟᱹᱲ 0;
	} ᱵᱟᱝ {
		init_notes(NULL, NULL, NULL, NOTES_INIT_WRITABLE);
		t = &default_notes_tree;
	}

	ᱡᱚᱠᱷᱚᱱ (strbuf_getline_lf(&buf, stdin) != EOF) {
		sa_nracanaa object_id from_obj, to_obj;
		sa_nracanaa string_list split = STRING_LIST_INIT_NODUP;
		puur_naa_nka err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		ᱡᱩᱫᱤ (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		ᱡᱩᱫᱤ (repo_get_oid(the_repository, split.items[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.items[0].string);
		ᱡᱩᱫᱤ (repo_get_oid(the_repository, split.items[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.items[1].string);

		ᱡᱩᱫᱤ (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		ᱵᱟᱝ
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		ᱡᱩᱫᱤ (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.items[0].string, split.items[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	ᱡᱩᱫᱤ (!rewrite_cmd) {
		commit_notes(the_repository, t, msg);
		free_notes(t);
	} ᱵᱟᱝ {
		finish_copy_notes_for_rewrite(the_repository, c, msg);
	}
	strbuf_release(&buf);
	ᱨᱩᱣᱟᱹᱲ ret;
}

static sa_nracanaa notes_tree *init_notes_check(const var_na *subcommand,
					   puur_naa_nka flags)
{
	sa_nracanaa notes_tree *t;
	const var_na *ref;
	init_notes(NULL, NULL, NULL, flags);
	t = &default_notes_tree;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	ᱡᱩᱫᱤ (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	ᱨᱩᱣᱟᱹᱲ t;
}

static puur_naa_nka list(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa notes_tree *t;
	sa_nracanaa object_id object;
	const sa_nracanaa object_id *note;
	puur_naa_nka retval = -1;
	sa_nracanaa option options[] = {
		OPT_END()
	};

	ᱡᱩᱫᱤ (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	ᱡᱩᱫᱤ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	ᱡᱩᱫᱤ (argc) {
		ᱡᱩᱫᱤ (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		ᱡᱩᱫᱤ (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} ᱵᱟᱝ
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} ᱵᱟᱝ
		retval = for_each_note(t, 0, list_each_note, NULL);

	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static puur_naa_nka append_edit(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED);

static puur_naa_nka add(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
	       sa_nracanaa repository *repo)
{
	puur_naa_nka force = 0, allow_empty = 0;
	const var_na *object_ref;
	sa_nracanaa notes_tree *t;
	sa_nracanaa object_id object, new_note;
	const sa_nracanaa object_id *note;
	sa_nracanaa note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	sa_nracanaa option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_msg_arg),
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

	ᱡᱩᱫᱤ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	ᱡᱩᱫᱤ (d.msg_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ᱡᱩᱫᱤ (note) {
		ᱡᱩᱫᱤ (!force) {
			free_notes(t);
			ᱡᱩᱫᱤ (d.msg_nr) {
				free_note_data(&d);
				ᱨᱩᱣᱟᱹᱲ error(_("Cannot add notes. "
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
			ᱨᱩᱣᱟᱹᱲ append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	ᱡᱩᱫᱤ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		ᱡᱩᱫᱤ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} ᱵᱟᱝ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka copy(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka retval = 0, force = 0, from_stdin = 0;
	const sa_nracanaa object_id *from_note, *note;
	const var_na *object_ref;
	sa_nracanaa object_id object, from_obj;
	sa_nracanaa notes_tree *t;
	const var_na *rewrite_cmd = NULL;
	sa_nracanaa option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	ᱡᱩᱫᱤ (from_stdin || rewrite_cmd) {
		ᱡᱩᱫᱤ (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} ᱵᱟᱝ {
			ᱨᱩᱣᱟᱹᱲ notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	ᱡᱩᱫᱤ (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	ᱡᱩᱫᱤ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	ᱡᱩᱫᱤ (note) {
		ᱡᱩᱫᱤ (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			goto out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	ᱡᱩᱫᱤ (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		goto out;
	}

	ᱡᱩᱫᱤ (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static puur_naa_nka append_edit(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		       sa_nracanaa repository *repo UNUSED)
{
	puur_naa_nka allow_empty = 0;
	const var_na *object_ref;
	sa_nracanaa notes_tree *t;
	sa_nracanaa object_id object, new_note;
	const sa_nracanaa object_id *note;
	var_na *logmsg;
	const var_na * const *usage;
	sa_nracanaa note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	sa_nracanaa option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_msg_arg),
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
	puur_naa_nka edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	ᱡᱩᱫᱤ (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	ᱡᱩᱫᱤ (d.msg_nr) {
		concat_messages(&d);
		ᱡᱩᱫᱤ (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : NULL);

	ᱡᱩᱫᱤ (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		ga_nanaa object_type type;
		sa_nracanaa strbuf buf = STRBUF_INIT;
		var_na *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		ᱡᱩᱫᱤ (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		ᱡᱩᱫᱤ (size)
			strbuf_add(&buf, prev_buf, size);
		ᱡᱩᱫᱤ (d.buf.len && size)
			append_separator(&buf);
		strbuf_insert(&d.buf, 0, buf.buf, buf.len);

		free(prev_buf);
		strbuf_release(&buf);
	}

	ᱡᱩᱫᱤ (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		ᱡᱩᱫᱤ (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} ᱵᱟᱝ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	free(logmsg);
	free_note_data(&d);
	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka show(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		sa_nracanaa repository *repo UNUSED)
{
	const var_na *object_ref;
	sa_nracanaa notes_tree *t;
	sa_nracanaa object_id object;
	const sa_nracanaa object_id *note;
	puur_naa_nka retval;
	sa_nracanaa option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	ᱡᱩᱫᱤ (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	ᱡᱩᱫᱤ (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	ᱵᱟᱝ {
		const var_na *show_args[3] = {"show", oid_to_hex(note), NULL};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static puur_naa_nka merge_abort(sa_nracanaa notes_merge_options *o)
{
	puur_naa_nka ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	ᱡᱩᱫᱤ (refs_delete_ref(get_main_ref_store(the_repository), NULL, "NOTES_MERGE_PARTIAL", NULL, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	ᱡᱩᱫᱤ (refs_delete_ref(get_main_ref_store(the_repository), NULL, "NOTES_MERGE_REF", NULL, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	ᱡᱩᱫᱤ (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka merge_commit(sa_nracanaa notes_merge_options *o)
{
	sa_nracanaa strbuf msg = STRBUF_INIT;
	sa_nracanaa object_id oid, parent_oid;
	sa_nracanaa notes_tree t = {0};
	sa_nracanaa commit *partial;
	sa_nracanaa pretty_print_context pretty_ctx;
	void *local_ref_to_free;
	puur_naa_nka ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	ᱡᱩᱫᱤ (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	ᱵᱟᱝ ᱡᱩᱫᱤ (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	ᱵᱟᱝ ᱡᱩᱫᱤ (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	ᱡᱩᱫᱤ (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	ᱵᱟᱝ
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, NULL);
	ᱡᱩᱫᱤ (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	ᱡᱩᱫᱤ (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, parimaa_na(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &msg,
				   &pretty_ctx);
	strbuf_trim(&msg);
	strbuf_insertstr(&msg, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), msg.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? NULL : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&msg);
	ret = merge_abort(o);
	free(local_ref_to_free);
	ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka git_config_get_notes_strategy(const var_na *key,
					 ga_nanaa notes_merge_strategy *strategy)
{
	var_na *value;

	ᱡᱩᱫᱤ (repo_config_get_string(the_repository, key, &value))
		ᱨᱩᱣᱟᱹᱲ 1;
	ᱡᱩᱫᱤ (parse_notes_merge_strategy(value, strategy))
		git_die_config(the_repository, key, _("unknown notes merge strategy %s"), value);

	free(value);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka merge(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa strbuf remote_ref = STRBUF_INIT, msg = STRBUF_INIT;
	sa_nracanaa object_id result_oid;
	sa_nracanaa notes_tree *t;
	sa_nracanaa notes_merge_options o;
	puur_naa_nka do_merge = 0, do_commit = 0, do_abort = 0;
	puur_naa_nka verbosity = 0, result;
	const var_na *strategy = NULL;
	sa_nracanaa option options[] = {
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
	var_na *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	ᱡᱩᱫᱤ (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	ᱡᱩᱫᱤ (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	ᱡᱩᱫᱤ (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} ᱵᱟᱝ ᱡᱩᱫᱤ (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	ᱡᱩᱫᱤ (do_abort)
		ᱨᱩᱣᱟᱹᱲ merge_abort(&o);
	ᱡᱩᱫᱤ (do_commit)
		ᱨᱩᱣᱟᱹᱲ merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	ᱡᱩᱫᱤ (strategy) {
		ᱡᱩᱫᱤ (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} ᱵᱟᱝ {
		sa_nracanaa strbuf merge_key = STRBUF_INIT;
		const var_na *short_ref = NULL;

		ᱡᱩᱫᱤ (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_key, "notes.%s.mergeStrategy", short_ref);

		ᱡᱩᱫᱤ (git_config_get_notes_strategy(merge_key.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_key);
	}

	strbuf_addf(&msg, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_msg), msg.buf + 7, msg.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	ᱡᱩᱫᱤ (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				notes_ref, &result_oid, NULL, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ᱵᱟᱝ { /* Merge has unresolved conflicts */
		sa_nracanaa worktree **worktrees;
		const sa_nracanaa worktree *wt;
		var_na *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, NULL,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		ᱡᱩᱫᱤ (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		ᱡᱩᱫᱤ (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, NULL))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		free(path);
	}

	free_notes(t);
	free(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&msg);
	ᱨᱩᱣᱟᱹᱲ result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

static puur_naa_nka remove_one_note(sa_nracanaa notes_tree *t, const var_na *name, unsigned flag)
{
	puur_naa_nka status;
	sa_nracanaa object_id oid;
	ᱡᱩᱫᱤ (repo_get_oid(the_repository, name, &oid))
		ᱨᱩᱣᱟᱹᱲ error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	ᱡᱩᱫᱤ (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	ᱵᱟᱝ
		fprintf(stderr, _("Removing note for object %s\n"), name);
	ᱨᱩᱣᱟᱹᱲ (flag & IGNORE_MISSING) ? 0 : status;
}

static puur_naa_nka remove_cmd(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		      sa_nracanaa repository *repo UNUSED)
{
	unsigned flag = 0;
	puur_naa_nka from_stdin = 0;
	sa_nracanaa option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	sa_nracanaa notes_tree *t;
	puur_naa_nka retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	ᱡᱩᱫᱤ (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} ᱵᱟᱝ {
		ᱡᱚᱠᱷᱚᱱ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	ᱡᱩᱫᱤ (from_stdin) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		ᱡᱚᱠᱷᱚᱱ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	ᱡᱩᱫᱤ (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static puur_naa_nka prune(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		 sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa notes_tree *t;
	puur_naa_nka show_only = 0, verbose = 0;
	sa_nracanaa option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	ᱡᱩᱫᱤ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	ᱡᱩᱫᱤ (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka get_ref(puur_naa_nka argc, const var_na **argv, const var_na *prefix,
		   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa option options[] = { OPT_END() };
	var_na *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	ᱡᱩᱫᱤ (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	free(notes_ref);
	ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka cmd_notes(puur_naa_nka argc,
	      const var_na **argv,
	      const var_na *prefix,
	      sa_nracanaa repository *repo)
{
	const var_na *override_notes_ref = NULL;
	parse_opt_subcommand_fn *fn = NULL;
	sa_nracanaa option options[] = {
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

	repo_config(the_repository, git_default_config, NULL);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	ᱡᱩᱫᱤ (!fn) {
		ᱡᱩᱫᱤ (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	ᱡᱩᱫᱤ (override_notes_ref) {
		sa_nracanaa strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	ᱨᱩᱣᱟᱹᱲ !!fn(argc, argv, prefix, repo);
}
