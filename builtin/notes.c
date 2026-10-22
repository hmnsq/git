/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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

ساكن ثابت حرف * ثابت git_notes_جمع_کړه_usage[] = {
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

ساكن ثابت حرف * ثابت git_notes_یوځای_usage[] = {
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

بنية note_پیغام {
	تعداد notes_stripspace stripspace;
	بنية strbuf buf;
};

بنية note_معلومات {
	صحيح use_editor;
	صحيح stripspace;
	حرف *edit_لار;
	بنية strbuf buf;
	بنية note_پیغام **messages;
	اندازه_t پیغام_nr;
	اندازه_t پیغام_alloc;
};

ساكن فراغ free_note_معلومات(بنية note_معلومات *d)
{
	إذا (d->edit_لار) {
		unlink_or_warn(d->edit_لار);
		حرر(d->edit_لار);
	}
	strbuf_release(&d->buf);

	طالما (d->پیغام_nr--) {
		strbuf_release(&d->messages[d->پیغام_nr]->buf);
		حرر(d->messages[d->پیغام_nr]);
	}
	حرر(d->messages);
}

ساكن صحيح list_each_note(ثابت بنية object_id *object_oid,
			  ثابت بنية object_id *note_oid,
			  حرف *note_لار ناکارول_شوی,
			  فراغ *cb_معلومات ناکارول_شوی)
{
	چاپ_کړه("%s %s\n", oid_منزل_hex(note_oid), oid_منزل_hex(object_oid));
	أرجع 0;
}

ساكن فراغ copy_obj_منزل_fd(صحيح fd, ثابت بنية object_id *oid)
{
	اندازه_t اندازه;
	تعداد object_type type;
	حرف *buf = odb_read_object(the_repository->objects, oid, &type, &اندازه);
	إذا (buf) {
		إذا (اندازه)
			write_or_die(fd, buf, اندازه);
		حرر(buf);
	}
}

ساكن فراغ write_commented_object(صحيح fd, ثابت بنية object_id *object)
{
	بنية ماشوم_process show = ماشوم_PROCESS_INIT;
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.دلیلونه, "show", "--stat", "--no-notes",
		     oid_منزل_hex(object), عدم);
	show.no_stdin = 1;
	show.بهر = -1;
	show.err = 0;
	show.git_cmd = 1;
	إذا (پیل_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_منزل_hex(object));

	إذا (strbuf_read(&buf, show.بهر, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_جمع_کړه_commented_کرښې(&cbuf, buf.buf, buf.len, comment_کرښه_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	إذا (پای_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_منزل_hex(object));
}

ساكن فراغ prepare_note_معلومات(ثابت بنية object_id *object, بنية note_معلومات *d,
		ثابت بنية object_id *old_note)
{
	إذا (d->use_editor || !d->پیغام_nr) {
		صحيح fd;
		بنية strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_لار = repo_git_لار(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_لار, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		إذا (d->پیغام_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		وإلا إذا (old_note)
			copy_obj_منزل_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_جمع_کړه_commented_کرښې(&buf, "\n", مزي_اوږدوالی("\n"), comment_کرښه_str);
		strbuf_جمع_کړه_commented_کرښې(&buf, _(note_template), مزي_اوږدوالی(_(note_template)),
					   comment_کرښه_str);
		strbuf_جمع_کړه_commented_کرښې(&buf, "\n", مزي_اوږدوالی("\n"), comment_کرښه_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		إذا (launch_editor(d->edit_لار, &d->buf, عدم)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		إذا (d->stripspace)
			strbuf_stripspace(&d->buf, comment_کرښه_str);
	}
}

ساكن فراغ write_note_معلومات(بنية note_معلومات *d, بنية object_id *oid)
{
	إذا (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		صحيح حالت = die_پیغام(_("unable to write note object"));

		إذا (d->edit_لار)
			die_پیغام(_("the note contents have been left in %s"),
				    d->edit_لار);
		وتل(حالت);
	}
}

ساكن فراغ append_separator(بنية strbuf *پیغام_2)
{
	اندازه_t sep_len = 0;

	إذا (!separator)
		أرجع;
	وإلا إذا ((sep_len = مزي_اوږدوالی(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(پیغام_2, separator);
	وإلا
		strbuf_addf(پیغام_2, "%s%s", separator, "\n");
}

ساكن فراغ concat_messages(بنية note_معلومات *d)
{
	بنية strbuf پیغام = STRBUF_INIT;
	اندازه_t i;

	لكل (i = 0; i < d->پیغام_nr ; i++) {
		إذا (d->buf.len)
			append_separator(&d->buf);
		strbuf_جمع_کړه(&پیغام, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &پیغام);
		إذا ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, عدم);
		strbuf_reset(&پیغام);
	}
	strbuf_release(&پیغام);
}

ساكن صحيح parse_پیغام_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_معلومات *d = opt->ارزښت;
	بنية note_پیغام *پیغام = xmalloc(حجم(*پیغام));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&پیغام->buf, مزي_اوږدوالی(arg));
	strbuf_addstr(&پیغام->buf, arg);
	ALLOC_GROW_BY(d->messages, d->پیغام_nr, 1, d->پیغام_alloc);
	d->messages[d->پیغام_nr - 1] = پیغام;
	پیغام->stripspace = STRIPSPACE;
	أرجع 0;
}

ساكن صحيح parse_file_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_معلومات *d = opt->ارزښت;
	بنية note_پیغام *پیغام = xmalloc(حجم(*پیغام));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&پیغام->buf , 0);
	إذا (!strcmp(arg, "-")) {
		إذا (strbuf_read(&پیغام->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} وإلا إذا (strbuf_read_file(&پیغام->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->پیغام_nr, 1, d->پیغام_alloc);
	d->messages[d->پیغام_nr - 1] = پیغام;
	پیغام->stripspace = STRIPSPACE;
	أرجع 0;
}

ساكن صحيح parse_reuse_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_معلومات *d = opt->ارزښت;
	بنية note_پیغام *پیغام = xmalloc(حجم(*پیغام));
	حرف *ارزښت;
	بنية object_id object;
	تعداد object_type type;
	اندازه_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&پیغام->buf, 0);
	إذا (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	إذا (!(ارزښت = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	إذا (type != OBJ_BLOB) {
		strbuf_release(&پیغام->buf);
		حرر(ارزښت);
		حرر(پیغام);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_جمع_کړه(&پیغام->buf, ارزښت, len);
	حرر(ارزښت);

	پیغام->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->پیغام_nr, 1, d->پیغام_alloc);
	d->messages[d->پیغام_nr - 1] = پیغام;
	پیغام->stripspace = NO_STRIPSPACE;
	أرجع 0;
}

ساكن صحيح parse_reedit_arg(ثابت بنية option *opt, ثابت حرف *arg, صحيح unset)
{
	بنية note_معلومات *d = opt->ارزښت;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	أرجع parse_reuse_arg(opt, arg, unset);
}

ساكن صحيح parse_separator_arg(ثابت بنية option *opt, ثابت حرف *arg,
			       صحيح unset)
{
	إذا (unset)
		*(ثابت حرف **)opt->ارزښت = عدم;
	وإلا
		*(ثابت حرف **)opt->ارزښت = arg ? arg : "\n";
	أرجع 0;
}

ساكن صحيح notes_copy_from_stdin(صحيح force, ثابت حرف *rewrite_cmd)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية notes_rewrite_cfg *c = عدم;
	بنية notes_ونه *t = عدم;
	صحيح ret = 0;
	ثابت حرف *پیغام = "Notes added by 'git notes copy'";

	إذا (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		إذا (!c)
			أرجع 0;
	} وإلا {
		init_notes(عدم, عدم, عدم, NOTES_INIT_WRITABLE);
		t = &default_notes_ونه;
	}

	طالما (strbuf_getline_lf(&buf, stdin) != EOF) {
		بنية object_id from_obj, منزل_obj;
		بنية string_list وېشل = STRING_LIST_INIT_NODUP;
		صحيح err;

		string_list_وېشل_دننه_ځای_پرځای_کړه_f(&وېشل, buf.buf, " ", -1,
					     STRING_LIST_وېشل_TRIM);
		إذا (وېشل.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		إذا (repo_get_oid(the_repository, وېشل.توکي[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    وېشل.توکي[0].string);
		إذا (repo_get_oid(the_repository, وېشل.توکي[1].string, &منزل_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    وېشل.توکي[1].string);

		إذا (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &منزل_obj);
		وإلا
			err = copy_note(t, &from_obj, &منزل_obj, force,
					combine_notes_overwrite);

		إذا (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      وېشل.توکي[0].string, وېشل.توکي[1].string);
			ret = 1;
		}

		string_list_clear(&وېشل, 0);
	}

	إذا (!rewrite_cmd) {
		commit_notes(the_repository, t, پیغام);
		free_notes(t);
	} وإلا {
		پای_copy_notes_for_rewrite(the_repository, c, پیغام);
	}
	strbuf_release(&buf);
	أرجع ret;
}

ساكن بنية notes_ونه *init_notes_check(ثابت حرف *subcommand,
					   صحيح flags)
{
	بنية notes_ونه *t;
	ثابت حرف *ref;
	init_notes(عدم, عدم, عدم, flags);
	t = &default_notes_ونه;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	إذا (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	أرجع t;
}

ساكن صحيح list(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		بنية repository *repo ناکارول_شوی)
{
	بنية notes_ونه *t;
	بنية object_id object;
	ثابت بنية object_id *note;
	صحيح retval = -1;
	بنية option options[] = {
		OPT_پای_ټکی()
	};

	إذا (argc)
		argc = parse_options(argc, argv, مختاړی, options,
				     git_notes_list_usage, 0);

	إذا (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	إذا (argc) {
		إذا (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		إذا (note) {
			puts(oid_منزل_hex(note));
			retval = 0;
		} وإلا
			retval = error(_("no note found for object %s."),
				       oid_منزل_hex(&object));
	} وإلا
		retval = for_each_note(t, 0, list_each_note, عدم);

	free_notes(t);
	أرجع retval;
}

ساكن صحيح append_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		       بنية repository *repo ناکارول_شوی);

ساكن صحيح جمع_کړه(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
	       بنية repository *repo)
{
	صحيح force = 0, allow_تشه = 0;
	ثابت حرف *object_ref;
	بنية notes_ونه *t;
	بنية object_id object, new_note;
	ثابت بنية object_id *note;
	بنية note_معلومات d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	بنية option options[] = {
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
		OPT_BOOL(0, "allow-empty", &allow_تشه,
			N_("allow storing empty note")),
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, git_notes_جمع_کړه_usage,
			     PARSE_OPT_KEEP_ARGV0);

	إذا (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_جمع_کړه_usage, options);
	}

	إذا (d.پیغام_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	إذا (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	إذا (note) {
		إذا (!force) {
			free_notes(t);
			إذا (d.پیغام_nr) {
				free_note_معلومات(&d);
				أرجع error(_("Cannot add notes. "
					"Found existing notes for object %s. "
					"Use '-f' to overwrite existing notes"),
					oid_منزل_hex(&object));
			}
			/*
			 * Redirect to "edit" subcommand.
			 *
			 * We only end up here if none of -m/-F/-c/-C or -f are
			 * given. The original args are therefore still in
			 * argv[0-1].
			 */
			argv[0] = "edit";
			أرجع append_edit(argc, argv, مختاړی, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_منزل_hex(&object));
	}

	prepare_note_معلومات(&object, &d, note);
	إذا (d.buf.len || allow_تشه) {
		write_note_معلومات(&d, &new_note);
		إذا (جمع_کړه_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} وإلا {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_منزل_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_معلومات(&d);
	free_notes(t);
	أرجع 0;
}

ساكن صحيح copy(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		بنية repository *repo ناکارول_شوی)
{
	صحيح retval = 0, force = 0, from_stdin = 0;
	ثابت بنية object_id *from_note, *note;
	ثابت حرف *object_ref;
	بنية object_id object, from_obj;
	بنية notes_ونه *t;
	ثابت حرف *rewrite_cmd = عدم;
	بنية option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, git_notes_copy_usage,
			     0);

	إذا (from_stdin || rewrite_cmd) {
		إذا (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} وإلا {
			أرجع notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	إذا (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	إذا (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	إذا (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	إذا (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	إذا (note) {
		إذا (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_منزل_hex(&object));
			اذهب بهر;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_منزل_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	إذا (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_منزل_hex(&from_obj));
		اذهب بهر;
	}

	إذا (جمع_کړه_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
بهر:
	free_notes(t);
	أرجع retval;
}

ساكن صحيح append_edit(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		       بنية repository *repo ناکارول_شوی)
{
	صحيح allow_تشه = 0;
	ثابت حرف *object_ref;
	بنية notes_ونه *t;
	بنية object_id object, new_note;
	ثابت بنية object_id *note;
	حرف *logmsg;
	ثابت حرف * ثابت *usage;
	بنية note_معلومات d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	بنية option options[] = {
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
		OPT_BOOL(0, "allow-empty", &allow_تشه,
			N_("allow storing empty note")),
		OPT_CALLBACK_F(0, "separator", &separator,
			N_("<paragraph-break>"),
			N_("insert <paragraph-break> between paragraphs"),
			PARSE_OPT_OPTARG, parse_separator_arg),
		OPT_BOOL(0, "stripspace", &d.stripspace,
			N_("remove unnecessary whitespace")),
		OPT_پای_ټکی()
	};
	صحيح edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, مختاړی, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	إذا (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	إذا (d.پیغام_nr) {
		concat_messages(&d);
		إذا (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	إذا (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_معلومات(&object, &d, edit && note ? note : عدم);

	إذا (note && !edit) {
		/* Append buf to previous note contents */
		اندازه_t اندازه;
		تعداد object_type type;
		بنية strbuf buf = STRBUF_INIT;
		حرف *prev_buf = odb_read_object(the_repository->objects, note, &type, &اندازه);

		إذا (!prev_buf)
			die(_("unable to read %s"), oid_منزل_hex(note));
		إذا (اندازه)
			strbuf_جمع_کړه(&buf, prev_buf, اندازه);
		إذا (d.buf.len && اندازه)
			append_separator(&buf);
		strbuf_ورننباسه(&d.buf, 0, buf.buf, buf.len);

		حرر(prev_buf);
		strbuf_release(&buf);
	}

	إذا (d.buf.len || allow_تشه) {
		write_note_معلومات(&d, &new_note);
		إذا (جمع_کړه_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} وإلا {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_منزل_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	حرر(logmsg);
	free_note_معلومات(&d);
	free_notes(t);
	أرجع 0;
}

ساكن صحيح show(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		بنية repository *repo ناکارول_شوی)
{
	ثابت حرف *object_ref;
	بنية notes_ونه *t;
	بنية object_id object;
	ثابت بنية object_id *note;
	صحيح retval;
	بنية option options[] = {
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, git_notes_show_usage,
			     0);

	إذا (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	إذا (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	إذا (!note)
		retval = error(_("no note found for object %s."),
			       oid_منزل_hex(&object));
	وإلا {
		ثابت حرف *show_دلیلونه[3] = {"show", oid_منزل_hex(note), عدم};
		retval = execv_git_cmd(show_دلیلونه);
	}
	free_notes(t);
	أرجع retval;
}

ساكن صحيح یوځای_abort(بنية notes_یوځای_options *o)
{
	صحيح ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	إذا (refs_delete_ref(get_main_ref_store(the_repository), عدم, "NOTES_MERGE_PARTIAL", عدم, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	إذا (refs_delete_ref(get_main_ref_store(the_repository), عدم, "NOTES_MERGE_REF", عدم, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	إذا (notes_یوځای_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	أرجع ret;
}

ساكن صحيح یوځای_commit(بنية notes_یوځای_options *o)
{
	بنية strbuf پیغام = STRBUF_INIT;
	بنية object_id oid, پلار_oid;
	بنية notes_ونه t = {0};
	بنية commit *partial;
	بنية pretty_print_context pretty_ctx;
	فراغ *local_ref_منزل_free;
	صحيح ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	إذا (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	وإلا إذا (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	وإلا إذا (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	إذا (partial->parents)
		oidcpy(&پلار_oid, &partial->parents->توکی->object.oid);
	وإلا
		oidclr(&پلار_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_منزل_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, عدم);
	إذا (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	إذا (notes_یوځای_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, حجم(pretty_ctx));
	repo_format_commit_پیغام(the_repository, partial, "%s", &پیغام,
				   &pretty_ctx);
	strbuf_trim(&پیغام);
	strbuf_insertstr(&پیغام, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
			o->local_ref, &oid,
			is_null_oid(&پلار_oid) ? عدم : &پلار_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&پیغام);
	ret = یوځای_abort(o);
	حرر(local_ref_منزل_free);
	أرجع ret;
}

ساكن صحيح git_config_get_notes_strategy(ثابت حرف *کلی,
					 تعداد notes_یوځای_strategy *strategy)
{
	حرف *ارزښت;

	إذا (repo_config_get_string(the_repository, کلی, &ارزښت))
		أرجع 1;
	إذا (parse_notes_یوځای_strategy(ارزښت, strategy))
		git_die_config(the_repository, کلی, _("unknown notes merge strategy %s"), ارزښت);

	حرر(ارزښت);
	أرجع 0;
}

ساكن صحيح یوځای(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		 بنية repository *repo ناکارول_شوی)
{
	بنية strbuf remote_ref = STRBUF_INIT, پیغام = STRBUF_INIT;
	بنية object_id پایله_oid;
	بنية notes_ونه *t;
	بنية notes_یوځای_options o;
	صحيح do_یوځای = 0, do_commit = 0, do_abort = 0;
	صحيح verbosity = 0, پایله;
	ثابت حرف *strategy = عدم;
	بنية option options[] = {
		OPT_ډله(N_("General options")),
		OPT__VERBOSITY(&verbosity),
		OPT_ډله(N_("Merge options")),
		OPT_STRING('s', "strategy", &strategy, N_("strategy"),
			   N_("resolve notes conflicts using the given strategy "
			      "(manual/ours/theirs/union/cat_sort_uniq)")),
		OPT_ډله(N_("Committing unmerged notes")),
		OPT_سېټ_INT_F(0, "commit", &do_commit,
			      N_("finalize notes merge by committing unmerged notes"),
			      1, PARSE_OPT_NONEG),
		OPT_ډله(N_("Aborting notes merge resolution")),
		OPT_سېټ_INT_F(0, "abort", &do_abort,
			      N_("abort notes merge"),
			      1, PARSE_OPT_NONEG),
		OPT_پای_ټکی()
	};
	حرف *notes_ref;

	argc = parse_options(argc, argv, مختاړی, options,
			     git_notes_یوځای_usage, 0);

	إذا (strategy || do_commit + do_abort == 0)
		do_یوځای = 1;
	إذا (do_یوځای + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_یوځای_usage, options);
	}

	إذا (do_یوځای && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_یوځای_usage, options);
	} وإلا إذا (!do_یوځای && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_یوځای_usage, options);
	}

	init_notes_یوځای_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_یوځای_VERBOSITY_DEFAULT;

	إذا (do_abort)
		أرجع یوځای_abort(&o);
	إذا (do_commit)
		أرجع یوځای_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	إذا (strategy) {
		إذا (parse_notes_یوځای_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_یوځای_usage, options);
		}
	} وإلا {
		بنية strbuf یوځای_کلی = STRBUF_INIT;
		ثابت حرف *short_ref = عدم;

		إذا (!skip_مختاړی(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&یوځای_کلی, "notes.%s.mergeStrategy", short_ref);

		إذا (git_config_get_notes_strategy(یوځای_کلی.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&یوځای_کلی);
	}

	strbuf_addf(&پیغام, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_جمع_کړه(&(o.commit_پیغام), پیغام.buf + 7, پیغام.len - 7); /* skip "notes: " */

	پایله = notes_یوځای(&o, t, &پایله_oid);

	إذا (پایله >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				notes_ref, &پایله_oid, عدم, 0,
				UPDATE_REFS_DIE_ON_ERR);
	وإلا { /* Merge has unresolved conflicts */
		بنية worktree **worktrees;
		ثابت بنية worktree *wt;
		حرف *لار;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), پیغام.buf,
				"NOTES_MERGE_PARTIAL", &پایله_oid, عدم,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = پیدا_کړه_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		إذا (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->لار);
		free_worktrees(worktrees);
		إذا (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, عدم))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		لار = repo_git_لار(the_repository, NOTES_یوځای_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			لار);
		حرر(لار);
	}

	free_notes(t);
	حرر(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&پیغام);
	أرجع پایله < 0; /* return non-zero on conflicts */
}

#تعریف_کړه IGNORE_MISSING 1

ساكن صحيح remove_one_note(بنية notes_ونه *t, ثابت حرف *name, بینښه flag)
{
	صحيح حالت;
	بنية object_id oid;
	إذا (repo_get_oid(the_repository, name, &oid))
		أرجع error(_("Failed to resolve '%s' as a valid ref."), name);
	حالت = remove_note(t, oid.hash);
	إذا (حالت)
		fprintf(stderr, _("Object %s has no note\n"), name);
	وإلا
		fprintf(stderr, _("Removing note for object %s\n"), name);
	أرجع (flag & IGNORE_MISSING) ? 0 : حالت;
}

ساكن صحيح remove_cmd(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		      بنية repository *repo ناکارول_شوی)
{
	بینښه flag = 0;
	صحيح from_stdin = 0;
	بنية option options[] = {
		OPT_بټ(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_پای_ټکی()
	};
	بنية notes_ونه *t;
	صحيح retval = 0;

	argc = parse_options(argc, argv, مختاړی, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	إذا (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} وإلا {
		طالما (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	إذا (from_stdin) {
		بنية strbuf sb = STRBUF_INIT;
		طالما (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	إذا (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	أرجع retval;
}

ساكن صحيح prune(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		 بنية repository *repo ناکارول_شوی)
{
	بنية notes_ونه *t;
	صحيح show_only = 0, verbose = 0;
	بنية option options[] = {
		OPT_DRY_پرلپسې(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_پای_ټکی()
	};

	argc = parse_options(argc, argv, مختاړی, options, git_notes_prune_usage,
			     0);

	إذا (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	إذا (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	أرجع 0;
}

ساكن صحيح get_ref(صحيح argc, ثابت حرف **argv, ثابت حرف *مختاړی,
		   بنية repository *repo ناکارول_شوی)
{
	بنية option options[] = { OPT_پای_ټکی() };
	حرف *notes_ref;
	argc = parse_options(argc, argv, مختاړی, options,
			     git_notes_get_ref_usage, 0);

	إذا (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	حرر(notes_ref);
	أرجع 0;
}

صحيح cmd_notes(صحيح argc,
	      ثابت حرف **argv,
	      ثابت حرف *مختاړی,
	      بنية repository *repo)
{
	ثابت حرف *override_notes_ref = عدم;
	parse_opt_subcommand_fn *fn = عدم;
	بنية option options[] = {
		OPT_STRING(0, "ref", &override_notes_ref, N_("notes-ref"),
			   N_("use notes from <notes-ref>")),
		OPT_SUBCOMMAND("list", &fn, list),
		OPT_SUBCOMMAND("add", &fn, جمع_کړه),
		OPT_SUBCOMMAND("copy", &fn, copy),
		OPT_SUBCOMMAND("append", &fn, append_edit),
		OPT_SUBCOMMAND("edit", &fn, append_edit),
		OPT_SUBCOMMAND("show", &fn, show),
		OPT_SUBCOMMAND("merge", &fn, یوځای),
		OPT_SUBCOMMAND("remove", &fn, remove_cmd),
		OPT_SUBCOMMAND("prune", &fn, prune),
		OPT_SUBCOMMAND("get-ref", &fn, get_ref),
		OPT_پای_ټکی()
	};

	repo_config(the_repository, git_default_config, عدم);
	argc = parse_options(argc, argv, مختاړی, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	إذا (!fn) {
		إذا (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	إذا (override_notes_ref) {
		بنية strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	أرجع !!fn(argc, argv, مختاړی, repo);
}
