/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "editor.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "hex.h"
#شامل "notes.h"
#شامل "object-file.h"
#شامل "object-name.h"
#شامل "odb.h"
#شامل "path.h"

#شامل "pretty.h"
#شامل "refs.h"
#شامل "exec-cmd.h"
#شامل "run-command.h"
#شامل "parse-options.h"
#شامل "string-list.h"
#شامل "notes-merge.h"
#شامل "notes-utils.h"
#شامل "worktree.h"
#شامل "write-or-die.h"

ساکن ثابت حرف *separator = "\n";
ساکن ثابت حرف * ثابت git_notes_usage[] = {
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
	تہی
};

ساکن ثابت حرف * ثابت git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_جوڑو_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_ملاؤ_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	تہی
};

ساکن ثابت حرف * ثابت git_notes_get_ref_usage[] = {
	"git notes get-ref",
	تہی
};

ساکن ثابت حرف note_template[] =
	N_("Write/edit the notes for the following object:");

شمار notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

ساخت note_پیغام {
	شمار notes_stripspace stripspace;
	ساخت strbuf buf;
};

ساخت note_ڈیٹا {
	صحیح use_editor;
	صحیح stripspace;
	حرف *edit_راستہ;
	ساخت strbuf buf;
	ساخت note_پیغام **messages;
	سائز_t پیغام_nr;
	سائز_t پیغام_alloc;
};

ساکن خالی free_note_ڈیٹا(ساخت note_ڈیٹا *d)
{
	اگر (d->edit_راستہ) {
		unlink_or_warn(d->edit_راستہ);
		آزاد(d->edit_راستہ);
	}
	strbuf_release(&d->buf);

	جب (d->پیغام_nr--) {
		strbuf_release(&d->messages[d->پیغام_nr]->buf);
		آزاد(d->messages[d->پیغام_nr]);
	}
	آزاد(d->messages);
}

ساکن صحیح list_each_note(ثابت ساخت object_id *object_oid,
			  ثابت ساخت object_id *note_oid,
			  حرف *note_راستہ غیر_مستعمل,
			  خالی *cb_ڈیٹا غیر_مستعمل)
{
	لکھو("%s %s\n", oid_مقصد_hex(note_oid), oid_مقصد_hex(object_oid));
	واپس 0;
}

ساکن خالی copy_obj_مقصد_fd(صحیح fd, ثابت ساخت object_id *oid)
{
	سائز_t سائز;
	شمار object_type type;
	حرف *buf = odb_read_object(the_repository->objects, oid, &type, &سائز);
	اگر (buf) {
		اگر (سائز)
			write_or_die(fd, buf, سائز);
		آزاد(buf);
	}
}

ساکن خالی write_commented_object(صحیح fd, ثابت ساخت object_id *object)
{
	ساخت بچہ_process show = بچہ_PROCESS_INIT;
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.دلائل, "show", "--stat", "--no-notes",
		     oid_مقصد_hex(object), تہی);
	show.no_stdin = 1;
	show.برآمد = -1;
	show.err = 0;
	show.git_cmd = 1;
	اگر (آغاز_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_مقصد_hex(object));

	اگر (strbuf_read(&buf, show.برآمد, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_جوڑو_commented_سطریں(&cbuf, buf.buf, buf.len, comment_سطر_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	اگر (اختتام_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_مقصد_hex(object));
}

ساکن خالی prepare_note_ڈیٹا(ثابت ساخت object_id *object, ساخت note_ڈیٹا *d,
		ثابت ساخت object_id *old_note)
{
	اگر (d->use_editor || !d->پیغام_nr) {
		صحیح fd;
		ساخت strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_راستہ = repo_git_راستہ(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_راستہ, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		اگر (d->پیغام_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		ورنہ اگر (old_note)
			copy_obj_مقصد_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_جوڑو_commented_سطریں(&buf, "\n", لڑی_لمبائی("\n"), comment_سطر_str);
		strbuf_جوڑو_commented_سطریں(&buf, _(note_template), لڑی_لمبائی(_(note_template)),
					   comment_سطر_str);
		strbuf_جوڑو_commented_سطریں(&buf, "\n", لڑی_لمبائی("\n"), comment_سطر_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		اگر (launch_editor(d->edit_راستہ, &d->buf, تہی)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		اگر (d->stripspace)
			strbuf_stripspace(&d->buf, comment_سطر_str);
	}
}

ساکن خالی write_note_ڈیٹا(ساخت note_ڈیٹا *d, ساخت object_id *oid)
{
	اگر (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		صحیح حالت_2 = die_پیغام(_("unable to write note object"));

		اگر (d->edit_راستہ)
			die_پیغام(_("the note contents have been left in %s"),
				    d->edit_راستہ);
		باہر_نکلو(حالت_2);
	}
}

ساکن خالی append_separator(ساخت strbuf *پیغام_2)
{
	سائز_t sep_len = 0;

	اگر (!separator)
		واپس;
	ورنہ اگر ((sep_len = لڑی_لمبائی(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(پیغام_2, separator);
	ورنہ
		strbuf_addf(پیغام_2, "%s%s", separator, "\n");
}

ساکن خالی concat_messages(ساخت note_ڈیٹا *d)
{
	ساخت strbuf پیغام = STRBUF_INIT;
	سائز_t i;

	برائے (i = 0; i < d->پیغام_nr ; i++) {
		اگر (d->buf.len)
			append_separator(&d->buf);
		strbuf_جوڑو(&پیغام, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &پیغام);
		اگر ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, تہی);
		strbuf_reset(&پیغام);
	}
	strbuf_release(&پیغام);
}

ساکن صحیح parse_پیغام_arg(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت note_ڈیٹا *d = opt->قدر;
	ساخت note_پیغام *پیغام = xmalloc(حجم(*پیغام));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&پیغام->buf, لڑی_لمبائی(arg));
	strbuf_addstr(&پیغام->buf, arg);
	ALLOC_GROW_BY(d->messages, d->پیغام_nr, 1, d->پیغام_alloc);
	d->messages[d->پیغام_nr - 1] = پیغام;
	پیغام->stripspace = STRIPSPACE;
	واپس 0;
}

ساکن صحیح parse_file_arg(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت note_ڈیٹا *d = opt->قدر;
	ساخت note_پیغام *پیغام = xmalloc(حجم(*پیغام));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&پیغام->buf , 0);
	اگر (!strcmp(arg, "-")) {
		اگر (strbuf_read(&پیغام->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} ورنہ اگر (strbuf_read_file(&پیغام->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->پیغام_nr, 1, d->پیغام_alloc);
	d->messages[d->پیغام_nr - 1] = پیغام;
	پیغام->stripspace = STRIPSPACE;
	واپس 0;
}

ساکن صحیح parse_reuse_arg(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت note_ڈیٹا *d = opt->قدر;
	ساخت note_پیغام *پیغام = xmalloc(حجم(*پیغام));
	حرف *قدر;
	ساخت object_id object;
	شمار object_type type;
	سائز_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&پیغام->buf, 0);
	اگر (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	اگر (!(قدر = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	اگر (type != OBJ_BLOB) {
		strbuf_release(&پیغام->buf);
		آزاد(قدر);
		آزاد(پیغام);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_جوڑو(&پیغام->buf, قدر, len);
	آزاد(قدر);

	پیغام->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->پیغام_nr, 1, d->پیغام_alloc);
	d->messages[d->پیغام_nr - 1] = پیغام;
	پیغام->stripspace = NO_STRIPSPACE;
	واپس 0;
}

ساکن صحیح parse_reedit_arg(ثابت ساخت option *opt, ثابت حرف *arg, صحیح unset)
{
	ساخت note_ڈیٹا *d = opt->قدر;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	واپس parse_reuse_arg(opt, arg, unset);
}

ساکن صحیح parse_separator_arg(ثابت ساخت option *opt, ثابت حرف *arg,
			       صحیح unset)
{
	اگر (unset)
		*(ثابت حرف **)opt->قدر = تہی;
	ورنہ
		*(ثابت حرف **)opt->قدر = arg ? arg : "\n";
	واپس 0;
}

ساکن صحیح notes_copy_from_stdin(صحیح force, ثابت حرف *rewrite_cmd)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت notes_rewrite_cfg *c = تہی;
	ساخت notes_درخت *t = تہی;
	صحیح ret = 0;
	ثابت حرف *پیغام = "Notes added by 'git notes copy'";

	اگر (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		اگر (!c)
			واپس 0;
	} ورنہ {
		init_notes(تہی, تہی, تہی, NOTES_INIT_WRITABLE);
		t = &default_notes_درخت;
	}

	جب (strbuf_getline_lf(&buf, stdin) != EOF) {
		ساخت object_id from_obj, مقصد_obj;
		ساخت string_list بانٹو = STRING_LIST_INIT_NODUP;
		صحیح err;

		string_list_بانٹو_اندر_رکھو_f(&بانٹو, buf.buf, " ", -1,
					     STRING_LIST_بانٹو_TRIM);
		اگر (بانٹو.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		اگر (repo_get_oid(the_repository, بانٹو.اشیاء[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    بانٹو.اشیاء[0].string);
		اگر (repo_get_oid(the_repository, بانٹو.اشیاء[1].string, &مقصد_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    بانٹو.اشیاء[1].string);

		اگر (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &مقصد_obj);
		ورنہ
			err = copy_note(t, &from_obj, &مقصد_obj, force,
					combine_notes_overwrite);

		اگر (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      بانٹو.اشیاء[0].string, بانٹو.اشیاء[1].string);
			ret = 1;
		}

		string_list_clear(&بانٹو, 0);
	}

	اگر (!rewrite_cmd) {
		commit_notes(the_repository, t, پیغام);
		free_notes(t);
	} ورنہ {
		اختتام_copy_notes_for_rewrite(the_repository, c, پیغام);
	}
	strbuf_release(&buf);
	واپس ret;
}

ساکن ساخت notes_درخت *init_notes_check(ثابت حرف *subcommand,
					   صحیح flags)
{
	ساخت notes_درخت *t;
	ثابت حرف *ref;
	init_notes(تہی, تہی, تہی, flags);
	t = &default_notes_درخت;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	اگر (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	واپس t;
}

ساکن صحیح list(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	ساخت notes_درخت *t;
	ساخت object_id object;
	ثابت ساخت object_id *note;
	صحیح retval = -1;
	ساخت option options[] = {
		OPT_انتہا()
	};

	اگر (argc)
		argc = parse_options(argc, argv, سابقہ, options,
				     git_notes_list_usage, 0);

	اگر (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	اگر (argc) {
		اگر (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		اگر (note) {
			puts(oid_مقصد_hex(note));
			retval = 0;
		} ورنہ
			retval = error(_("no note found for object %s."),
				       oid_مقصد_hex(&object));
	} ورنہ
		retval = for_each_note(t, 0, list_each_note, تہی);

	free_notes(t);
	واپس retval;
}

ساکن صحیح append_edit(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل);

ساکن صحیح جوڑو(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
	       ساخت repository *repo)
{
	صحیح force = 0, allow_خالی_خانے = 0;
	ثابت حرف *object_ref;
	ساخت notes_درخت *t;
	ساخت object_id object, new_note;
	ثابت ساخت object_id *note;
	ساخت note_ڈیٹا d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	ساخت option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_پیغام_arg),
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
		OPT_BOOL(0, "allow-empty", &allow_خالی_خانے,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, git_notes_جوڑو_usage,
			     PARSE_OPT_KEEP_ARGV0);

	اگر (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_جوڑو_usage, options);
	}

	اگر (d.پیغام_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	اگر (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	اگر (note) {
		اگر (!force) {
			free_notes(t);
			اگر (d.پیغام_nr) {
				free_note_ڈیٹا(&d);
				واپس error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_مقصد_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			واپس append_edit(argc, argv, سابقہ, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_مقصد_hex(&object));
	}

	prepare_note_ڈیٹا(&object, &d, note);
	اگر (d.buf.len || allow_خالی_خانے) {
		write_note_ڈیٹا(&d, &new_note);
		اگر (جوڑو_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} ورنہ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_مقصد_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_ڈیٹا(&d);
	free_notes(t);
	واپس 0;
}

ساکن صحیح copy(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	صحیح retval = 0, force = 0, from_stdin = 0;
	ثابت ساخت object_id *from_note, *note;
	ثابت حرف *object_ref;
	ساخت object_id object, from_obj;
	ساخت notes_درخت *t;
	ثابت حرف *rewrite_cmd = تہی;
	ساخت option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, git_notes_copy_usage,
			     0);

	اگر (from_stdin || rewrite_cmd) {
		اگر (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} ورنہ {
			واپس notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	اگر (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	اگر (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	اگر (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	اگر (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	اگر (note) {
		اگر (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_مقصد_hex(&object));
			جاؤ برآمد;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_مقصد_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	اگر (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_مقصد_hex(&from_obj));
		جاؤ برآمد;
	}

	اگر (جوڑو_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
برآمد:
	free_notes(t);
	واپس retval;
}

ساکن صحیح append_edit(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		       ساخت repository *repo غیر_مستعمل)
{
	صحیح allow_خالی_خانے = 0;
	ثابت حرف *object_ref;
	ساخت notes_درخت *t;
	ساخت object_id object, new_note;
	ثابت ساخت object_id *note;
	حرف *logmsg;
	ثابت حرف * ثابت *usage;
	ساخت note_ڈیٹا d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	ساخت option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_پیغام_arg),
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
		OPT_BOOL(0, "allow-empty", &allow_خالی_خانے,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_انتہا()
	};
	صحیح edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, سابقہ, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	اگر (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	اگر (d.پیغام_nr) {
		concat_messages(&d);
		اگر (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	اگر (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_ڈیٹا(&object, &d, edit && note ? note : تہی);

	اگر (note && !edit) {
		/* Append buf to previous note contents */
		سائز_t سائز;
		شمار object_type type;
		ساخت strbuf buf = STRBUF_INIT;
		حرف *پچھلا_buf = odb_read_object(the_repository->objects, note, &type, &سائز);

		اگر (!پچھلا_buf)
			die(_("unable to read %s"), oid_مقصد_hex(note));
		اگر (سائز)
			strbuf_جوڑو(&buf, پچھلا_buf, سائز);
		اگر (d.buf.len && سائز)
			append_separator(&buf);
		strbuf_داخل_کرو(&d.buf, 0, buf.buf, buf.len);

		آزاد(پچھلا_buf);
		strbuf_release(&buf);
	}

	اگر (d.buf.len || allow_خالی_خانے) {
		write_note_ڈیٹا(&d, &new_note);
		اگر (جوڑو_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} ورنہ {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_مقصد_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	آزاد(logmsg);
	free_note_ڈیٹا(&d);
	free_notes(t);
	واپس 0;
}

ساکن صحیح show(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		ساخت repository *repo غیر_مستعمل)
{
	ثابت حرف *object_ref;
	ساخت notes_درخت *t;
	ساخت object_id object;
	ثابت ساخت object_id *note;
	صحیح retval;
	ساخت option options[] = {
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, git_notes_show_usage,
			     0);

	اگر (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	اگر (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	اگر (!note)
		retval = error(_("no note found for object %s."),
			       oid_مقصد_hex(&object));
	ورنہ {
		ثابت حرف *show_دلائل[3] = {"show", oid_مقصد_hex(note), تہی};
		retval = execv_git_cmd(show_دلائل);
	}
	free_notes(t);
	واپس retval;
}

ساکن صحیح ملاؤ_abort(ساخت notes_ملاؤ_options *o)
{
	صحیح ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	اگر (refs_delete_ref(get_main_ref_store(the_repository), تہی, "NOTES_MERGE_PARTIAL", تہی, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	اگر (refs_delete_ref(get_main_ref_store(the_repository), تہی, "NOTES_MERGE_REF", تہی, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	اگر (notes_ملاؤ_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	واپس ret;
}

ساکن صحیح ملاؤ_commit(ساخت notes_ملاؤ_options *o)
{
	ساخت strbuf پیغام = STRBUF_INIT;
	ساخت object_id oid, والد_oid;
	ساخت notes_درخت t = {0};
	ساخت commit *partial;
	ساخت pretty_print_context pretty_ctx;
	خالی *local_ref_مقصد_free;
	صحیح ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	اگر (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	ورنہ اگر (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	ورنہ اگر (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	اگر (partial->parents)
		oidcpy(&والد_oid, &partial->parents->شے->object.oid);
	ورنہ
		oidclr(&والد_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_مقصد_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, تہی);
	اگر (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	اگر (notes_ملاؤ_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, حجم(pretty_ctx));
	repo_format_commit_پیغام(the_repository, partial, "%s", &پیغام,
				   &pretty_ctx);
	strbuf_trim(&پیغام);
	strbuf_insertstr(&پیغام, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
			o->local_ref, &oid,
			is_null_oid(&والد_oid) ? تہی : &والد_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&پیغام);
	ret = ملاؤ_abort(o);
	آزاد(local_ref_مقصد_free);
	واپس ret;
}

ساکن صحیح git_config_get_notes_strategy(ثابت حرف *کلید,
					 شمار notes_ملاؤ_strategy *strategy)
{
	حرف *قدر;

	اگر (repo_config_get_string(the_repository, کلید, &قدر))
		واپس 1;
	اگر (parse_notes_ملاؤ_strategy(قدر, strategy))
		git_die_config(the_repository, کلید, _("unknown notes merge strategy %s"), قدر);

	آزاد(قدر);
	واپس 0;
}

ساکن صحیح ملاؤ(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت strbuf remote_ref = STRBUF_INIT, پیغام = STRBUF_INIT;
	ساخت object_id نتیجہ_oid;
	ساخت notes_درخت *t;
	ساخت notes_ملاؤ_options o;
	صحیح do_ملاؤ = 0, do_commit = 0, do_abort = 0;
	صحیح verbosity = 0, نتیجہ;
	ثابت حرف *strategy = تہی;
	ساخت option options[] = {
		OPT_گروہ(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_گروہ(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_گروہ(N_("Committing unmerged notes")),
		OPT_سیٹ_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_گروہ(N_("Aborting notes merge resolution")),
		OPT_سیٹ_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_انتہا()
	};
	حرف *notes_ref;

	argc = parse_options(argc, argv, سابقہ, options,
			     git_notes_ملاؤ_usage, 0);

	اگر (strategy || do_commit + do_abort == 0)
		do_ملاؤ = 1;
	اگر (do_ملاؤ + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_ملاؤ_usage, options);
	}

	اگر (do_ملاؤ && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_ملاؤ_usage, options);
	} ورنہ اگر (!do_ملاؤ && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_ملاؤ_usage, options);
	}

	init_notes_ملاؤ_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_ملاؤ_VERBOSITY_DEFAULT;

	اگر (do_abort)
		واپس ملاؤ_abort(&o);
	اگر (do_commit)
		واپس ملاؤ_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	اگر (strategy) {
		اگر (parse_notes_ملاؤ_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_ملاؤ_usage, options);
		}
	} ورنہ {
		ساخت strbuf ملاؤ_کلید = STRBUF_INIT;
		ثابت حرف *short_ref = تہی;

		اگر (!skip_سابقہ(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&ملاؤ_کلید, "notes.%s.mergeStrategy", short_ref);

		اگر (git_config_get_notes_strategy(ملاؤ_کلید.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&ملاؤ_کلید);
	}

	strbuf_addf(&پیغام, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_جوڑو(&(o.commit_پیغام), پیغام.buf + 7, پیغام.len - 7); /* skip "notes: " */

	نتیجہ = notes_ملاؤ(&o, t, &نتیجہ_oid);

	اگر (نتیجہ >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				notes_ref, &نتیجہ_oid, تہی, 0,
				UPDATE_REFS_DIE_ON_ERR);
	ورنہ { /* Merge has unresolved conflicts */
		ساخت worktree **worktrees;
		ثابت ساخت worktree *wt;
		حرف *راستہ;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				"NOTES_MERGE_PARTIAL", &نتیجہ_oid, تہی,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = ڈھونڈو_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		اگر (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->راستہ);
		free_worktrees(worktrees);
		اگر (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, تہی))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		راستہ = repo_git_راستہ(the_repository, NOTES_ملاؤ_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			راستہ);
		آزاد(راستہ);
	}

	free_notes(t);
	آزاد(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&پیغام);
	واپس نتیجہ < 0; /* return non-zero on conflicts */
}

#تعریف_کرو IGNORE_MISSING 1

ساکن صحیح remove_one_note(ساخت notes_درخت *t, ثابت حرف *name, غیرعلامتی flag)
{
	صحیح حالت_2;
	ساخت object_id oid;
	اگر (repo_get_oid(the_repository, name, &oid))
		واپس error(_("Failed to resolve '%s' as a valid ref."), name);
	حالت_2 = remove_note(t, oid.hash);
	اگر (حالت_2)
		fprintf(stderr, _("Object %s has no note\n"), name);
	ورنہ
		fprintf(stderr, _("Removing note for object %s\n"), name);
	واپس (flag & IGNORE_MISSING) ? 0 : حالت_2;
}

ساکن صحیح remove_cmd(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		      ساخت repository *repo غیر_مستعمل)
{
	غیرعلامتی flag = 0;
	صحیح from_stdin = 0;
	ساخت option options[] = {
		OPT_ثنائی_ہندسہ(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_انتہا()
	};
	ساخت notes_درخت *t;
	صحیح retval = 0;

	argc = parse_options(argc, argv, سابقہ, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	اگر (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} ورنہ {
		جب (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	اگر (from_stdin) {
		ساخت strbuf sb = STRBUF_INIT;
		جب (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	اگر (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	واپس retval;
}

ساکن صحیح prune(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		 ساخت repository *repo غیر_مستعمل)
{
	ساخت notes_درخت *t;
	صحیح show_only = 0, verbose = 0;
	ساخت option options[] = {
		OPT_DRY_تسلسل(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_انتہا()
	};

	argc = parse_options(argc, argv, سابقہ, options, git_notes_prune_usage,
			     0);

	اگر (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	اگر (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	واپس 0;
}

ساکن صحیح get_ref(صحیح argc, ثابت حرف **argv, ثابت حرف *سابقہ,
		   ساخت repository *repo غیر_مستعمل)
{
	ساخت option options[] = { OPT_انتہا() };
	حرف *notes_ref;
	argc = parse_options(argc, argv, سابقہ, options,
			     git_notes_get_ref_usage, 0);

	اگر (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	آزاد(notes_ref);
	واپس 0;
}

صحیح cmd_notes(صحیح argc,
	      ثابت حرف **argv,
	      ثابت حرف *سابقہ,
	      ساخت repository *repo)
{
	ثابت حرف *override_notes_ref = تہی;
	parse_opt_subcommand_fn *fn = تہی;
	ساخت option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, جوڑو),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, ملاؤ),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_انتہا()
	};

	repo_config(the_repository, git_default_config, تہی);
	argc = parse_options(argc, argv, سابقہ, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	اگر (!fn) {
		اگر (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	اگر (override_notes_ref) {
		ساخت strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	واپس !!fn(argc, argv, سابقہ, repo);
}
