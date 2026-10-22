/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "editor.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "hex.h"
#ضمن "notes.h"
#ضمن "object-file.h"
#ضمن "object-name.h"
#ضمن "odb.h"
#ضمن "path.h"

#ضمن "pretty.h"
#ضمن "refs.h"
#ضمن "exec-cmd.h"
#ضمن "run-command.h"
#ضمن "parse-options.h"
#ضمن "string-list.h"
#ضمن "notes-merge.h"
#ضمن "notes-utils.h"
#ضمن "worktree.h"
#ضمن "write-or-die.h"

ساكن ثابت حرف *separator = "\n";
ساكن ثابت حرف * ثابت git_notes_usage[] = {
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
	عدم
};

ساكن ثابت حرف * ثابت git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	عدم
};

ساكن ثابت حرف * ثابت git_notes_get_ref_usage[] = {
	"git notes get-ref",
	عدم
};

ساكن ثابت حرف note_template[] =
	N_("Write/edit the notes for the following object:");

تعداد notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

بنية note_msg {
	تعداد notes_stripspace stripspace;
	بنية strbuf buf;
};

بنية note_data {
	صحيح use_editor;
	صحيح stripspace;
	حرف *edit_path;
	بنية strbuf buf;
	بنية note_msg **messages;
	size_t msg_nr;
	size_t msg_alloc;
};

ساكن فراغ free_note_data(بنية note_data *d)
{
	אם (d->edit_path) {
		unlink_or_warn(d->edit_path);
		حرر(d->edit_path);
	}
	strbuf_release(&d->buf);

	כלעוד (d->msg_nr--) {
		strbuf_release(&d->messages[d->msg_nr]->buf);
		حرر(d->messages[d->msg_nr]);
	}
	حرر(d->messages);
}

ساكن صحيح list_each_note(ثابت بنية object_id *object_oid,
			  ثابت بنية object_id *note_oid,
			  حرف *note_path UNUSED,
			  فراغ *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	החזר 0;
}

ساكن فراغ copy_obj_to_fd(صحيح fd, ثابت بنية object_id *oid)
{
	size_t size;
	تعداد object_type type;
	حرف *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	אם (buf) {
		אם (size)
			write_or_die(fd, buf, size);
		حرر(buf);
	}
}

ساكن فراغ write_commented_object(صحيح fd, ثابت بنية object_id *object)
{
	بنية child_process show = CHILD_PROCESS_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), عدم);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	אם (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	אם (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_lines(&cbuf, buf.buf, buf.len, comment_line_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	אם (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

ساكن فراغ prepare_note_data(ثابت بنية object_id *object, بنية note_data *d,
		ثابت بنية object_id *old_note)
{
	אם (d->use_editor || !d->msg_nr) {
		صحيح fd;
		بنية strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		אם (d->msg_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		אחרת אם (old_note)
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

		אם (launch_editor(d->edit_path, &d->buf, عدم)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		אם (d->stripspace)
			strbuf_stripspace(&d->buf, comment_line_str);
	}
}

ساكن فراغ write_note_data(بنية note_data *d, بنية object_id *oid)
{
	אם (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		صحيح status = die_message(_("unable to write note object"));

		אם (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

ساكن فراغ append_separator(بنية strbuf *message)
{
	size_t sep_len = 0;

	אם (!separator)
		החזר;
	אחרת אם ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	אחרת
		strbuf_addf(message, "%s%s", separator, "\n");
}

ساكن فراغ concat_messages(بنية note_data *d)
{
	بنية strbuf msg = STRBUF_INIT;
	size_t i;

	עבור (i = 0; i < d->msg_nr ; i++) {
		אם (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&msg, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &msg);
		אם ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, عدم);
		strbuf_reset(&msg);
	}
	strbuf_release(&msg);
}

ساكن صحيح parse_msg_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_data *d = opt->value;
	بنية note_msg *msg = xmalloc(حجم(*msg));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&msg->buf, strlen(arg));
	strbuf_addstr(&msg->buf, arg);
	ALLOC_GROW_BY(d->messages, d->msg_nr, 1, d->msg_alloc);
	d->messages[d->msg_nr - 1] = msg;
	msg->stripspace = STRIPSPACE;
	החזר 0;
}

ساكن صحيح parse_file_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_data *d = opt->value;
	بنية note_msg *msg = xmalloc(حجم(*msg));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&msg->buf , 0);
	אם (!strcmp(arg, "-")) {
		אם (strbuf_read(&msg->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} אחרת אם (strbuf_read_file(&msg->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->msg_nr, 1, d->msg_alloc);
	d->messages[d->msg_nr - 1] = msg;
	msg->stripspace = STRIPSPACE;
	החזר 0;
}

ساكن صحيح parse_reuse_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_data *d = opt->value;
	بنية note_msg *msg = xmalloc(حجم(*msg));
	حرف *value;
	بنية object_id object;
	تعداد object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&msg->buf, 0);
	אם (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	אם (!(value = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	אם (type != OBJ_BLOB) {
		strbuf_release(&msg->buf);
		حرر(value);
		حرر(msg);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&msg->buf, value, len);
	حرر(value);

	msg->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->msg_nr, 1, d->msg_alloc);
	d->messages[d->msg_nr - 1] = msg;
	msg->stripspace = NO_STRIPSPACE;
	החזר 0;
}

ساكن صحيح parse_reedit_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_data *d = opt->value;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	החזר parse_reuse_arg(opt, arg, unset);
}

ساكن صحيح parse_separator_arg(ثابت بنية option *opt, ثابت حرف *arg,
			       صحيح unset)
{
	אם (unset)
		*(ثابت حرف **)opt->value = عدم;
	אחרת
		*(ثابت حرف **)opt->value = arg ? arg : "\n";
	החזר 0;
}

ساكن صحيح notes_copy_from_stdin(صحيح force, ثابت حرف *rewrite_cmd)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية notes_rewrite_cfg *c = عدم;
	بنية notes_tree *t = عدم;
	صحيح ret = 0;
	ثابت حرف *msg = "Notes added by 'git notes copy'";

	אם (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		אם (!c)
			החזר 0;
	} אחרת {
		init_notes(عدم, عدم, عدم, NOTES_INIT_WRITABLE);
		t = &default_notes_tree;
	}

	כלעוד (strbuf_getline_lf(&buf, stdin) != EOF) {
		بنية object_id from_obj, to_obj;
		بنية string_list split = STRING_LIST_INIT_NODUP;
		صحيح err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		אם (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		אם (repo_get_oid(the_repository, split.items[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.items[0].string);
		אם (repo_get_oid(the_repository, split.items[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.items[1].string);

		אם (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		אחרת
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		אם (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.items[0].string, split.items[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	אם (!rewrite_cmd) {
		commit_notes(the_repository, t, msg);
		free_notes(t);
	} אחרת {
		finish_copy_notes_for_rewrite(the_repository, c, msg);
	}
	strbuf_release(&buf);
	החזר ret;
}

ساكن بنية notes_tree *init_notes_check(ثابت حرف *subcommand,
					   صحيح flags)
{
	بنية notes_tree *t;
	ثابت حرف *ref;
	init_notes(عدم, عدم, عدم, flags);
	t = &default_notes_tree;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	אם (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	החזר t;
}

ساكن صحيح list(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	بنية notes_tree *t;
	بنية object_id object;
	ثابت بنية object_id *note;
	صحيح retval = -1;
	بنية option options[] = {
		OPT_END()
	};

	אם (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	אם (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	אם (argc) {
		אם (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		אם (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} אחרת
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} אחרת
		retval = for_each_note(t, 0, list_each_note, عدم);

	free_notes(t);
	החזר retval;
}

ساكن صحيح append_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED);

ساكن صحيح add(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
	       بنية repository *repo)
{
	صحيح force = 0, allow_empty = 0;
	ثابت حرف *object_ref;
	بنية notes_tree *t;
	بنية object_id object, new_note;
	ثابت بنية object_id *note;
	بنية note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	بنية option options[] = {
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

	אם (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	אם (d.msg_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	אם (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	אם (note) {
		אם (!force) {
			free_notes(t);
			אם (d.msg_nr) {
				free_note_data(&d);
				החזר error(_("Cannot add notes. "
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
			החזר append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	אם (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		אם (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} אחרת {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	החזר 0;
}

ساكن صحيح copy(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	صحيح retval = 0, force = 0, from_stdin = 0;
	ثابت بنية object_id *from_note, *note;
	ثابت حرف *object_ref;
	بنية object_id object, from_obj;
	بنية notes_tree *t;
	ثابت حرف *rewrite_cmd = عدم;
	بنية option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	אם (from_stdin || rewrite_cmd) {
		אם (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} אחרת {
			החזר notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	אם (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	אם (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	אם (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	אם (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	אם (note) {
		אם (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			اذهب out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	אם (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		اذهب out;
	}

	אם (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	החזר retval;
}

ساكن صحيح append_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		       بنية repository *repo UNUSED)
{
	صحيح allow_empty = 0;
	ثابت حرف *object_ref;
	بنية notes_tree *t;
	بنية object_id object, new_note;
	ثابت بنية object_id *note;
	حرف *logmsg;
	ثابت حرف * ثابت *usage;
	بنية note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	بنية option options[] = {
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
	صحيح edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	אם (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	אם (d.msg_nr) {
		concat_messages(&d);
		אם (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	אם (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : عدم);

	אם (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		تعداد object_type type;
		بنية strbuf buf = STRBUF_INIT;
		حرف *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		אם (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		אם (size)
			strbuf_add(&buf, prev_buf, size);
		אם (d.buf.len && size)
			append_separator(&buf);
		strbuf_insert(&d.buf, 0, buf.buf, buf.len);

		حرر(prev_buf);
		strbuf_release(&buf);
	}

	אם (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		אם (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} אחרת {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	حرر(logmsg);
	free_note_data(&d);
	free_notes(t);
	החזר 0;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		بنية repository *repo UNUSED)
{
	ثابت حرف *object_ref;
	بنية notes_tree *t;
	بنية object_id object;
	ثابت بنية object_id *note;
	صحيح retval;
	بنية option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	אם (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	אם (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	אם (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	אחרת {
		ثابت حرف *show_args[3] = {"show", oid_to_hex(note), عدم};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	החזר retval;
}

ساكن صحيح merge_abort(بنية notes_merge_options *o)
{
	صحيح ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	אם (refs_delete_ref(get_main_ref_store(the_repository), عدم, "NOTES_MERGE_PARTIAL", عدم, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	אם (refs_delete_ref(get_main_ref_store(the_repository), عدم, "NOTES_MERGE_REF", عدم, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	אם (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	החזר ret;
}

ساكن صحيح merge_commit(بنية notes_merge_options *o)
{
	بنية strbuf msg = STRBUF_INIT;
	بنية object_id oid, parent_oid;
	بنية notes_tree t = {0};
	بنية commit *partial;
	بنية pretty_print_context pretty_ctx;
	فراغ *local_ref_to_free;
	صحيح ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	אם (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	אחרת אם (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	אחרת אם (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	אם (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	אחרת
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, عدم);
	אם (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	אם (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, حجم(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &msg,
				   &pretty_ctx);
	strbuf_trim(&msg);
	strbuf_insertstr(&msg, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), msg.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? عدم : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&msg);
	ret = merge_abort(o);
	حرر(local_ref_to_free);
	החזר ret;
}

ساكن صحيح git_config_get_notes_strategy(ثابت حرف *key,
					 تعداد notes_merge_strategy *strategy)
{
	حرف *value;

	אם (repo_config_get_string(the_repository, key, &value))
		החזר 1;
	אם (parse_notes_merge_strategy(value, strategy))
		git_die_config(the_repository, key, _("unknown notes merge strategy %s"), value);

	حرر(value);
	החזר 0;
}

ساكن صحيح merge(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	بنية strbuf remote_ref = STRBUF_INIT, msg = STRBUF_INIT;
	بنية object_id result_oid;
	بنية notes_tree *t;
	بنية notes_merge_options o;
	صحيح do_merge = 0, do_commit = 0, do_abort = 0;
	صحيح verbosity = 0, result;
	ثابت حرف *strategy = عدم;
	بنية option options[] = {
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
	حرف *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	אם (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	אם (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	אם (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} אחרת אם (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	אם (do_abort)
		החזר merge_abort(&o);
	אם (do_commit)
		החזר merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	אם (strategy) {
		אם (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} אחרת {
		بنية strbuf merge_key = STRBUF_INIT;
		ثابت حرف *short_ref = عدم;

		אם (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_key, "notes.%s.mergeStrategy", short_ref);

		אם (git_config_get_notes_strategy(merge_key.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_key);
	}

	strbuf_addf(&msg, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_msg), msg.buf + 7, msg.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	אם (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				notes_ref, &result_oid, عدم, 0,
				UPDATE_REFS_DIE_ON_ERR);
	אחרת { /* Merge has unresolved conflicts */
		بنية worktree **worktrees;
		ثابت بنية worktree *wt;
		حرف *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), msg.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, عدم,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		אם (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		אם (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, عدم))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		حرر(path);
	}

	free_notes(t);
	حرر(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&msg);
	החזר result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

ساكن صحيح remove_one_note(بنية notes_tree *t, ثابت حرف *name, unsigned flag)
{
	صحيح status;
	بنية object_id oid;
	אם (repo_get_oid(the_repository, name, &oid))
		החזר error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	אם (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	אחרת
		fprintf(stderr, _("Removing note for object %s\n"), name);
	החזר (flag & IGNORE_MISSING) ? 0 : status;
}

ساكن صحيح remove_cmd(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		      بنية repository *repo UNUSED)
{
	unsigned flag = 0;
	صحيح from_stdin = 0;
	بنية option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	بنية notes_tree *t;
	صحيح retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	אם (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} אחרת {
		כלעוד (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	אם (from_stdin) {
		بنية strbuf sb = STRBUF_INIT;
		כלעוד (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	אם (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	החזר retval;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		 بنية repository *repo UNUSED)
{
	بنية notes_tree *t;
	صحيح show_only = 0, verbose = 0;
	بنية option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	אם (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	אם (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	החזר 0;
}

ساكن صحيح get_ref(صحيح argc, ثابت حرف **argv, ثابت حرف *prefix,
		   بنية repository *repo UNUSED)
{
	بنية option options[] = { OPT_END() };
	حرف *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	אם (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	حرر(notes_ref);
	החזר 0;
}

صحيح cmd_notes(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *prefix,
	      بنية repository *repo)
{
	ثابت حرف *override_notes_ref = عدم;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
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

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	אם (!fn) {
		אם (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	אם (override_notes_ref) {
		بنية strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	החזר !!fn(argc, argv, prefix, repo);
}
