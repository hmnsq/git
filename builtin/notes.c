/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "editor.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "hex.h"
#অন্তর্ভাব "notes.h"
#অন্তর্ভাব "object-file.h"
#অন্তর্ভাব "object-name.h"
#অন্তর্ভাব "odb.h"
#অন্তর্ভাব "path.h"

#অন্তর্ভাব "pretty.h"
#অন্তর্ভাব "refs.h"
#অন্তর্ভাব "exec-cmd.h"
#অন্তর্ভাব "run-command.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "string-list.h"
#অন্তর্ভাব "notes-merge.h"
#অন্তর্ভাব "notes-utils.h"
#অন্তর্ভাব "worktree.h"
#অন্তর্ভাব "write-or-die.h"

স্থির ধ্রুব বর্ণ *separator = "\n";
স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_usage[] = {
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
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	রিক্ত
};

স্থির ধ্রুব বর্ণ * ধ্রুব git_notes_get_ref_usage[] = {
	"git notes get-ref",
	রিক্ত
};

স্থির ধ্রুব বর্ণ note_template[] =
	N_("Write/edit the notes for the following object:");

গণনা notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

সংরচনা note_বার্তা {
	গণনা notes_stripspace stripspace;
	সংরচনা strbuf buf;
};

সংরচনা note_data {
	পূর্ণাঙ্ক use_editor;
	পূর্ণাঙ্ক stripspace;
	বর্ণ *edit_path;
	সংরচনা strbuf buf;
	সংরচনা note_বার্তা **messages;
	size_t বার্তা_nr;
	size_t বার্তা_alloc;
};

স্থির শূন্য free_note_data(সংরচনা note_data *d)
{
	যদি (d->edit_path) {
		unlink_or_warn(d->edit_path);
		মুক্ত(d->edit_path);
	}
	strbuf_release(&d->buf);

	যাবত্ (d->বার্তা_nr--) {
		strbuf_release(&d->messages[d->বার্তা_nr]->buf);
		মুক্ত(d->messages[d->বার্তা_nr]);
	}
	মুক্ত(d->messages);
}

স্থির পূর্ণাঙ্ক list_each_note(ধ্রুব সংরচনা object_id *object_oid,
			  ধ্রুব সংরচনা object_id *note_oid,
			  বর্ণ *note_path UNUSED,
			  শূন্য *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	প্রত্যাগম 0;
}

স্থির শূন্য copy_obj_to_fd(পূর্ণাঙ্ক fd, ধ্রুব সংরচনা object_id *oid)
{
	size_t size;
	গণনা object_type type;
	বর্ণ *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	যদি (buf) {
		যদি (size)
			write_or_die(fd, buf, size);
		মুক্ত(buf);
	}
}

স্থির শূন্য write_commented_object(পূর্ণাঙ্ক fd, ধ্রুব সংরচনা object_id *object)
{
	সংরচনা child_process show = CHILD_PROCESS_INIT;
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), রিক্ত);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	যদি (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	যদি (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_সারিগুলি(&cbuf, buf.buf, buf.len, comment_সারি_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	যদি (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

স্থির শূন্য prepare_note_data(ধ্রুব সংরচনা object_id *object, সংরচনা note_data *d,
		ধ্রুব সংরচনা object_id *old_note)
{
	যদি (d->use_editor || !d->বার্তা_nr) {
		পূর্ণাঙ্ক fd;
		সংরচনা strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		যদি (d->বার্তা_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		অন্যথা যদি (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_সারিগুলি(&buf, "\n", strlen("\n"), comment_সারি_str);
		strbuf_add_commented_সারিগুলি(&buf, _(note_template), strlen(_(note_template)),
					   comment_সারি_str);
		strbuf_add_commented_সারিগুলি(&buf, "\n", strlen("\n"), comment_সারি_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		যদি (launch_editor(d->edit_path, &d->buf, রিক্ত)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		যদি (d->stripspace)
			strbuf_stripspace(&d->buf, comment_সারি_str);
	}
}

স্থির শূন্য write_note_data(সংরচনা note_data *d, সংরচনা object_id *oid)
{
	যদি (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		পূর্ণাঙ্ক status = die_message(_("unable to write note object"));

		যদি (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

স্থির শূন্য append_separator(সংরচনা strbuf *message)
{
	size_t sep_len = 0;

	যদি (!separator)
		প্রত্যাগম;
	অন্যথা যদি ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	অন্যথা
		strbuf_addf(message, "%s%s", separator, "\n");
}

স্থির শূন্য concat_messages(সংরচনা note_data *d)
{
	সংরচনা strbuf বার্তা = STRBUF_INIT;
	size_t i;

	প্রতি (i = 0; i < d->বার্তা_nr ; i++) {
		যদি (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&বার্তা, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &বার্তা);
		যদি ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, রিক্ত);
		strbuf_reset(&বার্তা);
	}
	strbuf_release(&বার্তা);
}

স্থির পূর্ণাঙ্ক parse_বার্তা_arg(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা note_data *d = opt->মান;
	সংরচনা note_বার্তা *বার্তা = xmalloc(পরিমাণ(*বার্তা));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&বার্তা->buf, strlen(arg));
	strbuf_addstr(&বার্তা->buf, arg);
	ALLOC_GROW_BY(d->messages, d->বার্তা_nr, 1, d->বার্তা_alloc);
	d->messages[d->বার্তা_nr - 1] = বার্তা;
	বার্তা->stripspace = STRIPSPACE;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_file_arg(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা note_data *d = opt->মান;
	সংরচনা note_বার্তা *বার্তা = xmalloc(পরিমাণ(*বার্তা));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&বার্তা->buf , 0);
	যদি (!strcmp(arg, "-")) {
		যদি (strbuf_read(&বার্তা->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} অন্যথা যদি (strbuf_read_file(&বার্তা->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->বার্তা_nr, 1, d->বার্তা_alloc);
	d->messages[d->বার্তা_nr - 1] = বার্তা;
	বার্তা->stripspace = STRIPSPACE;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_reuse_arg(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা note_data *d = opt->মান;
	সংরচনা note_বার্তা *বার্তা = xmalloc(পরিমাণ(*বার্তা));
	বর্ণ *মান;
	সংরচনা object_id object;
	গণনা object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&বার্তা->buf, 0);
	যদি (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	যদি (!(মান = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	যদি (type != OBJ_BLOB) {
		strbuf_release(&বার্তা->buf);
		মুক্ত(মান);
		মুক্ত(বার্তা);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&বার্তা->buf, মান, len);
	মুক্ত(মান);

	বার্তা->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->বার্তা_nr, 1, d->বার্তা_alloc);
	d->messages[d->বার্তা_nr - 1] = বার্তা;
	বার্তা->stripspace = NO_STRIPSPACE;
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক parse_reedit_arg(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg, পূর্ণাঙ্ক unset)
{
	সংরচনা note_data *d = opt->মান;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	প্রত্যাগম parse_reuse_arg(opt, arg, unset);
}

স্থির পূর্ণাঙ্ক parse_separator_arg(ধ্রুব সংরচনা option *opt, ধ্রুব বর্ণ *arg,
			       পূর্ণাঙ্ক unset)
{
	যদি (unset)
		*(ধ্রুব বর্ণ **)opt->মান = রিক্ত;
	অন্যথা
		*(ধ্রুব বর্ণ **)opt->মান = arg ? arg : "\n";
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক notes_copy_from_stdin(পূর্ণাঙ্ক force, ধ্রুব বর্ণ *rewrite_cmd)
{
	সংরচনা strbuf buf = STRBUF_INIT;
	সংরচনা notes_rewrite_cfg *c = রিক্ত;
	সংরচনা notes_বৃক্ষ *t = রিক্ত;
	পূর্ণাঙ্ক ret = 0;
	ধ্রুব বর্ণ *বার্তা = "Notes added by 'git notes copy'";

	যদি (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		যদি (!c)
			প্রত্যাগম 0;
	} অন্যথা {
		init_notes(রিক্ত, রিক্ত, রিক্ত, NOTES_INIT_WRITABLE);
		t = &default_notes_বৃক্ষ;
	}

	যাবত্ (strbuf_getline_lf(&buf, stdin) != EOF) {
		সংরচনা object_id from_obj, to_obj;
		সংরচনা string_list split = STRING_LIST_INIT_NODUP;
		পূর্ণাঙ্ক err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		যদি (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		যদি (repo_get_oid(the_repository, split.বস্তুসমূহ[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.বস্তুসমূহ[0].string);
		যদি (repo_get_oid(the_repository, split.বস্তুসমূহ[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.বস্তুসমূহ[1].string);

		যদি (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		অন্যথা
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		যদি (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.বস্তুসমূহ[0].string, split.বস্তুসমূহ[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	যদি (!rewrite_cmd) {
		commit_notes(the_repository, t, বার্তা);
		free_notes(t);
	} অন্যথা {
		finish_copy_notes_for_rewrite(the_repository, c, বার্তা);
	}
	strbuf_release(&buf);
	প্রত্যাগম ret;
}

স্থির সংরচনা notes_বৃক্ষ *init_notes_check(ধ্রুব বর্ণ *subcommand,
					   পূর্ণাঙ্ক flags)
{
	সংরচনা notes_বৃক্ষ *t;
	ধ্রুব বর্ণ *ref;
	init_notes(রিক্ত, রিক্ত, রিক্ত, flags);
	t = &default_notes_বৃক্ষ;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	যদি (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	প্রত্যাগম t;
}

স্থির পূর্ণাঙ্ক list(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	সংরচনা notes_বৃক্ষ *t;
	সংরচনা object_id object;
	ধ্রুব সংরচনা object_id *note;
	পূর্ণাঙ্ক retval = -1;
	সংরচনা option options[] = {
		OPT_END()
	};

	যদি (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	যদি (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	যদি (argc) {
		যদি (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		যদি (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} অন্যথা
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} অন্যথা
		retval = for_each_note(t, 0, list_each_note, রিক্ত);

	free_notes(t);
	প্রত্যাগম retval;
}

স্থির পূর্ণাঙ্ক append_edit(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		       সংরচনা repository *repo UNUSED);

স্থির পূর্ণাঙ্ক add(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
	       সংরচনা repository *repo)
{
	পূর্ণাঙ্ক force = 0, allow_empty = 0;
	ধ্রুব বর্ণ *object_ref;
	সংরচনা notes_বৃক্ষ *t;
	সংরচনা object_id object, new_note;
	ধ্রুব সংরচনা object_id *note;
	সংরচনা note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	সংরচনা option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_বার্তা_arg),
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

	যদি (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	যদি (d.বার্তা_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	যদি (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	যদি (note) {
		যদি (!force) {
			free_notes(t);
			যদি (d.বার্তা_nr) {
				free_note_data(&d);
				প্রত্যাগম error(_("Cannot add notes. "
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
			প্রত্যাগম append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	যদি (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		যদি (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} অন্যথা {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক copy(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক retval = 0, force = 0, from_stdin = 0;
	ধ্রুব সংরচনা object_id *from_note, *note;
	ধ্রুব বর্ণ *object_ref;
	সংরচনা object_id object, from_obj;
	সংরচনা notes_বৃক্ষ *t;
	ধ্রুব বর্ণ *rewrite_cmd = রিক্ত;
	সংরচনা option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	যদি (from_stdin || rewrite_cmd) {
		যদি (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} অন্যথা {
			প্রত্যাগম notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	যদি (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	যদি (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	যদি (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	যদি (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	যদি (note) {
		যদি (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			গচ্ছ out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	যদি (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		গচ্ছ out;
	}

	যদি (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	প্রত্যাগম retval;
}

স্থির পূর্ণাঙ্ক append_edit(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		       সংরচনা repository *repo UNUSED)
{
	পূর্ণাঙ্ক allow_empty = 0;
	ধ্রুব বর্ণ *object_ref;
	সংরচনা notes_বৃক্ষ *t;
	সংরচনা object_id object, new_note;
	ধ্রুব সংরচনা object_id *note;
	বর্ণ *logmsg;
	ধ্রুব বর্ণ * ধ্রুব *usage;
	সংরচনা note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	সংরচনা option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_বার্তা_arg),
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
	পূর্ণাঙ্ক edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	যদি (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	যদি (d.বার্তা_nr) {
		concat_messages(&d);
		যদি (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	যদি (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : রিক্ত);

	যদি (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		গণনা object_type type;
		সংরচনা strbuf buf = STRBUF_INIT;
		বর্ণ *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		যদি (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		যদি (size)
			strbuf_add(&buf, prev_buf, size);
		যদি (d.buf.len && size)
			append_separator(&buf);
		strbuf_ঢোকাও(&d.buf, 0, buf.buf, buf.len);

		মুক্ত(prev_buf);
		strbuf_release(&buf);
	}

	যদি (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		যদি (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} অন্যথা {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	মুক্ত(logmsg);
	free_note_data(&d);
	free_notes(t);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক show(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		সংরচনা repository *repo UNUSED)
{
	ধ্রুব বর্ণ *object_ref;
	সংরচনা notes_বৃক্ষ *t;
	সংরচনা object_id object;
	ধ্রুব সংরচনা object_id *note;
	পূর্ণাঙ্ক retval;
	সংরচনা option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	যদি (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	যদি (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	যদি (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	অন্যথা {
		ধ্রুব বর্ণ *show_args[3] = {"show", oid_to_hex(note), রিক্ত};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	প্রত্যাগম retval;
}

স্থির পূর্ণাঙ্ক merge_abort(সংরচনা notes_merge_options *o)
{
	পূর্ণাঙ্ক ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	যদি (refs_delete_ref(get_main_ref_store(the_repository), রিক্ত, "NOTES_MERGE_PARTIAL", রিক্ত, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	যদি (refs_delete_ref(get_main_ref_store(the_repository), রিক্ত, "NOTES_MERGE_REF", রিক্ত, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	যদি (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক merge_commit(সংরচনা notes_merge_options *o)
{
	সংরচনা strbuf বার্তা = STRBUF_INIT;
	সংরচনা object_id oid, parent_oid;
	সংরচনা notes_বৃক্ষ t = {0};
	সংরচনা commit *partial;
	সংরচনা pretty_print_context pretty_ctx;
	শূন্য *local_ref_to_free;
	পূর্ণাঙ্ক ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	যদি (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	অন্যথা যদি (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	অন্যথা যদি (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	যদি (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	অন্যথা
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, রিক্ত);
	যদি (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	যদি (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, পরিমাণ(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &বার্তা,
				   &pretty_ctx);
	strbuf_trim(&বার্তা);
	strbuf_insertstr(&বার্তা, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), বার্তা.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? রিক্ত : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&বার্তা);
	ret = merge_abort(o);
	মুক্ত(local_ref_to_free);
	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক git_config_get_notes_strategy(ধ্রুব বর্ণ *চাবি,
					 গণনা notes_merge_strategy *strategy)
{
	বর্ণ *মান;

	যদি (repo_config_get_string(the_repository, চাবি, &মান))
		প্রত্যাগম 1;
	যদি (parse_notes_merge_strategy(মান, strategy))
		git_die_config(the_repository, চাবি, _("unknown notes merge strategy %s"), মান);

	মুক্ত(মান);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক merge(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	সংরচনা strbuf remote_ref = STRBUF_INIT, বার্তা = STRBUF_INIT;
	সংরচনা object_id result_oid;
	সংরচনা notes_বৃক্ষ *t;
	সংরচনা notes_merge_options o;
	পূর্ণাঙ্ক do_merge = 0, do_commit = 0, do_abort = 0;
	পূর্ণাঙ্ক verbosity = 0, result;
	ধ্রুব বর্ণ *strategy = রিক্ত;
	সংরচনা option options[] = {
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
	বর্ণ *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	যদি (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	যদি (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	যদি (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} অন্যথা যদি (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	যদি (do_abort)
		প্রত্যাগম merge_abort(&o);
	যদি (do_commit)
		প্রত্যাগম merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	যদি (strategy) {
		যদি (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} অন্যথা {
		সংরচনা strbuf merge_চাবি = STRBUF_INIT;
		ধ্রুব বর্ণ *short_ref = রিক্ত;

		যদি (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_চাবি, "notes.%s.mergeStrategy", short_ref);

		যদি (git_config_get_notes_strategy(merge_চাবি.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_চাবি);
	}

	strbuf_addf(&বার্তা, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_বার্তা), বার্তা.buf + 7, বার্তা.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	যদি (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), বার্তা.buf,
				notes_ref, &result_oid, রিক্ত, 0,
				UPDATE_REFS_DIE_ON_ERR);
	অন্যথা { /* Merge has unresolved conflicts */
		সংরচনা worktree **worktrees;
		ধ্রুব সংরচনা worktree *wt;
		বর্ণ *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), বার্তা.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, রিক্ত,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		যদি (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		যদি (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, রিক্ত))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		মুক্ত(path);
	}

	free_notes(t);
	মুক্ত(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&বার্তা);
	প্রত্যাগম result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

স্থির পূর্ণাঙ্ক remove_one_note(সংরচনা notes_বৃক্ষ *t, ধ্রুব বর্ণ *name, unsigned flag)
{
	পূর্ণাঙ্ক status;
	সংরচনা object_id oid;
	যদি (repo_get_oid(the_repository, name, &oid))
		প্রত্যাগম error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	যদি (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	অন্যথা
		fprintf(stderr, _("Removing note for object %s\n"), name);
	প্রত্যাগম (flag & IGNORE_MISSING) ? 0 : status;
}

স্থির পূর্ণাঙ্ক remove_cmd(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		      সংরচনা repository *repo UNUSED)
{
	unsigned flag = 0;
	পূর্ণাঙ্ক from_stdin = 0;
	সংরচনা option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	সংরচনা notes_বৃক্ষ *t;
	পূর্ণাঙ্ক retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	যদি (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} অন্যথা {
		যাবত্ (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	যদি (from_stdin) {
		সংরচনা strbuf sb = STRBUF_INIT;
		যাবত্ (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	যদি (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	প্রত্যাগম retval;
}

স্থির পূর্ণাঙ্ক prune(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		 সংরচনা repository *repo UNUSED)
{
	সংরচনা notes_বৃক্ষ *t;
	পূর্ণাঙ্ক show_only = 0, verbose = 0;
	সংরচনা option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	যদি (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	যদি (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক get_ref(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ **argv, ধ্রুব বর্ণ *prefix,
		   সংরচনা repository *repo UNUSED)
{
	সংরচনা option options[] = { OPT_END() };
	বর্ণ *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	যদি (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	মুক্ত(notes_ref);
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক cmd_notes(পূর্ণাঙ্ক argc,
	      ধ্রুব বর্ণ **argv,
	      ধ্রুব বর্ণ *prefix,
	      সংরচনা repository *repo)
{
	ধ্রুব বর্ণ *override_notes_ref = রিক্ত;
	parse_opt_subcommand_fn *fn = রিক্ত;
	সংরচনা option options[] = {
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

	repo_config(the_repository, git_default_config, রিক্ত);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	যদি (!fn) {
		যদি (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	যদি (override_notes_ref) {
		সংরচনা strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	প্রত্যাগম !!fn(argc, argv, prefix, repo);
}
