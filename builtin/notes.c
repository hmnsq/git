/*
 * Builtin "git notes"
 *
 * Copyright (c) 2010 Johan Herland <johan@herland.net>
 *
 * Based on git-notes.sh by Johannes Schindelin,
 * and builtin/tag.c by Kristian Høgsberg and Carlos Rica.
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "editor.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "hex.h"
#அந்தர்பாவ "notes.h"
#அந்தர்பாவ "object-file.h"
#அந்தர்பாவ "object-name.h"
#அந்தர்பாவ "odb.h"
#அந்தர்பாவ "path.h"

#அந்தர்பாவ "pretty.h"
#அந்தர்பாவ "refs.h"
#அந்தர்பாவ "exec-cmd.h"
#அந்தர்பாவ "run-command.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "notes-merge.h"
#அந்தர்பாவ "notes-utils.h"
#அந்தர்பாவ "worktree.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான மாறிலி எழுத்து *separator = "\n";
நிலையான மாறிலி எழுத்து * மாறிலி git_notes_usage[] = {
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
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_list_usage[] = {
	N_("git notes [list [<object>]]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_add_usage[] = {
	N_("git notes add [<options>] [<object>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_copy_usage[] = {
	N_("git notes copy [<options>] <from-object> <to-object>"),
	N_("git notes copy --stdin [<from-object> <to-object>]..."),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_append_usage[] = {
	N_("git notes append [<options>] [<object>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_edit_usage[] = {
	N_("git notes edit [<object>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_show_usage[] = {
	N_("git notes show [<object>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_merge_usage[] = {
	N_("git notes merge [<options>] <notes-ref>"),
	N_("git notes merge --commit [<options>]"),
	N_("git notes merge --abort [<options>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_remove_usage[] = {
	N_("git notes remove [<object>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_prune_usage[] = {
	N_("git notes prune [<options>]"),
	வெற்று
};

நிலையான மாறிலி எழுத்து * மாறிலி git_notes_get_ref_usage[] = {
	"git notes get-ref",
	வெற்று
};

நிலையான மாறிலி எழுத்து note_template[] =
	N_("Write/edit the notes for the following object:");

எண்ணிக்கை notes_stripspace {
	UNSPECIFIED = -1,
	NO_STRIPSPACE = 0,
	STRIPSPACE = 1,
};

கட்டமைப்பு note_செய்தி {
	எண்ணிக்கை notes_stripspace stripspace;
	கட்டமைப்பு strbuf buf;
};

கட்டமைப்பு note_data {
	முழு use_editor;
	முழு stripspace;
	எழுத்து *edit_path;
	கட்டமைப்பு strbuf buf;
	கட்டமைப்பு note_செய்தி **messages;
	size_t செய்தி_nr;
	size_t செய்தி_alloc;
};

நிலையான வெறுமை free_note_data(கட்டமைப்பு note_data *d)
{
	எனில் (d->edit_path) {
		unlink_or_warn(d->edit_path);
		விடுவி(d->edit_path);
	}
	strbuf_release(&d->buf);

	போது (d->செய்தி_nr--) {
		strbuf_release(&d->messages[d->செய்தி_nr]->buf);
		விடுவி(d->messages[d->செய்தி_nr]);
	}
	விடுவி(d->messages);
}

நிலையான முழு list_each_note(மாறிலி கட்டமைப்பு object_id *object_oid,
			  மாறிலி கட்டமைப்பு object_id *note_oid,
			  எழுத்து *note_path UNUSED,
			  வெறுமை *cb_data UNUSED)
{
	printf("%s %s\n", oid_to_hex(note_oid), oid_to_hex(object_oid));
	திரும்பு 0;
}

நிலையான வெறுமை copy_obj_to_fd(முழு fd, மாறிலி கட்டமைப்பு object_id *oid)
{
	size_t size;
	எண்ணிக்கை object_type type;
	எழுத்து *buf = odb_read_object(the_repository->objects, oid, &type, &size);
	எனில் (buf) {
		எனில் (size)
			write_or_die(fd, buf, size);
		விடுவி(buf);
	}
}

நிலையான வெறுமை write_commented_object(முழு fd, மாறிலி கட்டமைப்பு object_id *object)
{
	கட்டமைப்பு child_process show = CHILD_PROCESS_INIT;
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf cbuf = STRBUF_INIT;

	/* Invoke "git show --stat --no-notes $object" */
	strvec_pushl(&show.args, "show", "--stat", "--no-notes",
		     oid_to_hex(object), வெற்று);
	show.no_stdin = 1;
	show.out = -1;
	show.err = 0;
	show.git_cmd = 1;
	எனில் (start_command(&show))
		die(_("unable to start 'show' for object '%s'"),
		    oid_to_hex(object));

	எனில் (strbuf_read(&buf, show.out, 0) < 0)
		die_errno(_("could not read 'show' output"));
	strbuf_add_commented_வரிகள்(&cbuf, buf.buf, buf.len, comment_வரி_str);
	write_or_die(fd, cbuf.buf, cbuf.len);

	strbuf_release(&cbuf);
	strbuf_release(&buf);

	எனில் (finish_command(&show))
		die(_("failed to finish 'show' for object '%s'"),
		    oid_to_hex(object));
}

நிலையான வெறுமை prepare_note_data(மாறிலி கட்டமைப்பு object_id *object, கட்டமைப்பு note_data *d,
		மாறிலி கட்டமைப்பு object_id *old_note)
{
	எனில் (d->use_editor || !d->செய்தி_nr) {
		முழு fd;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;

		/* write the template message before editing: */
		d->edit_path = repo_git_path(the_repository, "NOTES_EDITMSG");
		fd = xopen(d->edit_path, O_CREAT | O_TRUNC | O_WRONLY, 0600);

		எனில் (d->செய்தி_nr)
			write_or_die(fd, d->buf.buf, d->buf.len);
		இல்லையெனில் எனில் (old_note)
			copy_obj_to_fd(fd, old_note);

		strbuf_addch(&buf, '\n');
		strbuf_add_commented_வரிகள்(&buf, "\n", strlen("\n"), comment_வரி_str);
		strbuf_add_commented_வரிகள்(&buf, _(note_template), strlen(_(note_template)),
					   comment_வரி_str);
		strbuf_add_commented_வரிகள்(&buf, "\n", strlen("\n"), comment_வரி_str);
		write_or_die(fd, buf.buf, buf.len);

		write_commented_object(fd, object);

		close(fd);
		strbuf_release(&buf);
		strbuf_reset(&d->buf);

		எனில் (launch_editor(d->edit_path, &d->buf, வெற்று)) {
			die(_("please supply the note contents using either -m or -F option"));
		}
		எனில் (d->stripspace)
			strbuf_stripspace(&d->buf, comment_வரி_str);
	}
}

நிலையான வெறுமை write_note_data(கட்டமைப்பு note_data *d, கட்டமைப்பு object_id *oid)
{
	எனில் (odb_write_object(the_repository->objects, d->buf.buf,
			     d->buf.len, OBJ_BLOB, oid)) {
		முழு status = die_message(_("unable to write note object"));

		எனில் (d->edit_path)
			die_message(_("the note contents have been left in %s"),
				    d->edit_path);
		exit(status);
	}
}

நிலையான வெறுமை append_separator(கட்டமைப்பு strbuf *message)
{
	size_t sep_len = 0;

	எனில் (!separator)
		திரும்பு;
	இல்லையெனில் எனில் ((sep_len = strlen(separator)) && separator[sep_len - 1] == '\n')
		strbuf_addstr(message, separator);
	இல்லையெனில்
		strbuf_addf(message, "%s%s", separator, "\n");
}

நிலையான வெறுமை concat_messages(கட்டமைப்பு note_data *d)
{
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	size_t i;

	க்கு (i = 0; i < d->செய்தி_nr ; i++) {
		எனில் (d->buf.len)
			append_separator(&d->buf);
		strbuf_add(&செய்தி, d->messages[i]->buf.buf, d->messages[i]->buf.len);
		strbuf_addbuf(&d->buf, &செய்தி);
		எனில் ((d->stripspace == UNSPECIFIED &&
		     d->messages[i]->stripspace == STRIPSPACE) ||
		    d->stripspace == STRIPSPACE)
			strbuf_stripspace(&d->buf, வெற்று);
		strbuf_reset(&செய்தி);
	}
	strbuf_release(&செய்தி);
}

நிலையான முழு parse_செய்தி_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு note_data *d = opt->மதிப்பு;
	கட்டமைப்பு note_செய்தி *செய்தி = xmalloc(அளவு(*செய்தி));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&செய்தி->buf, strlen(arg));
	strbuf_addstr(&செய்தி->buf, arg);
	ALLOC_GROW_BY(d->messages, d->செய்தி_nr, 1, d->செய்தி_alloc);
	d->messages[d->செய்தி_nr - 1] = செய்தி;
	செய்தி->stripspace = STRIPSPACE;
	திரும்பு 0;
}

நிலையான முழு parse_file_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு note_data *d = opt->மதிப்பு;
	கட்டமைப்பு note_செய்தி *செய்தி = xmalloc(அளவு(*செய்தி));

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&செய்தி->buf , 0);
	எனில் (!strcmp(arg, "-")) {
		எனில் (strbuf_read(&செய்தி->buf, 0, 1024) < 0)
			die_errno(_("cannot read '%s'"), arg);
	} இல்லையெனில் எனில் (strbuf_read_file(&செய்தி->buf, arg, 1024) < 0)
		die_errno(_("could not open or read '%s'"), arg);

	ALLOC_GROW_BY(d->messages, d->செய்தி_nr, 1, d->செய்தி_alloc);
	d->messages[d->செய்தி_nr - 1] = செய்தி;
	செய்தி->stripspace = STRIPSPACE;
	திரும்பு 0;
}

நிலையான முழு parse_reuse_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு note_data *d = opt->மதிப்பு;
	கட்டமைப்பு note_செய்தி *செய்தி = xmalloc(அளவு(*செய்தி));
	எழுத்து *மதிப்பு;
	கட்டமைப்பு object_id object;
	எண்ணிக்கை object_type type;
	size_t len;

	BUG_ON_OPT_NEG(unset);

	strbuf_init(&செய்தி->buf, 0);
	எனில் (repo_get_oid(the_repository, arg, &object))
		die(_("failed to resolve '%s' as a valid ref."), arg);
	எனில் (!(மதிப்பு = odb_read_object(the_repository->objects, &object, &type, &len)))
		die(_("failed to read object '%s'."), arg);
	எனில் (type != OBJ_BLOB) {
		strbuf_release(&செய்தி->buf);
		விடுவி(மதிப்பு);
		விடுவி(செய்தி);
		die(_("cannot read note data from non-blob object '%s'."), arg);
	}

	strbuf_add(&செய்தி->buf, மதிப்பு, len);
	விடுவி(மதிப்பு);

	செய்தி->buf.len = len;
	ALLOC_GROW_BY(d->messages, d->செய்தி_nr, 1, d->செய்தி_alloc);
	d->messages[d->செய்தி_nr - 1] = செய்தி;
	செய்தி->stripspace = NO_STRIPSPACE;
	திரும்பு 0;
}

நிலையான முழு parse_reedit_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு note_data *d = opt->மதிப்பு;
	BUG_ON_OPT_NEG(unset);
	d->use_editor = 1;
	திரும்பு parse_reuse_arg(opt, arg, unset);
}

நிலையான முழு parse_separator_arg(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			       முழு unset)
{
	எனில் (unset)
		*(மாறிலி எழுத்து **)opt->மதிப்பு = வெற்று;
	இல்லையெனில்
		*(மாறிலி எழுத்து **)opt->மதிப்பு = arg ? arg : "\n";
	திரும்பு 0;
}

நிலையான முழு notes_copy_from_stdin(முழு force, மாறிலி எழுத்து *rewrite_cmd)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு notes_rewrite_cfg *c = வெற்று;
	கட்டமைப்பு notes_மரம் *t = வெற்று;
	முழு ret = 0;
	மாறிலி எழுத்து *செய்தி = "Notes added by 'git notes copy'";

	எனில் (rewrite_cmd) {
		c = init_copy_notes_for_rewrite(rewrite_cmd);
		எனில் (!c)
			திரும்பு 0;
	} இல்லையெனில் {
		init_notes(வெற்று, வெற்று, வெற்று, NOTES_INIT_WRITABLE);
		t = &default_notes_மரம்;
	}

	போது (strbuf_getline_lf(&buf, stdin) != EOF) {
		கட்டமைப்பு object_id from_obj, to_obj;
		கட்டமைப்பு string_list split = STRING_LIST_INIT_NODUP;
		முழு err;

		string_list_split_in_place_f(&split, buf.buf, " ", -1,
					     STRING_LIST_SPLIT_TRIM);
		எனில் (split.nr < 2)
			die(_("malformed input line: '%s'."), buf.buf);
		எனில் (repo_get_oid(the_repository, split.பொருட்கள்[0].string, &from_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.பொருட்கள்[0].string);
		எனில் (repo_get_oid(the_repository, split.பொருட்கள்[1].string, &to_obj))
			die(_("failed to resolve '%s' as a valid ref."),
			    split.பொருட்கள்[1].string);

		எனில் (rewrite_cmd)
			err = copy_note_for_rewrite(c, &from_obj, &to_obj);
		இல்லையெனில்
			err = copy_note(t, &from_obj, &to_obj, force,
					combine_notes_overwrite);

		எனில் (err) {
			error(_("failed to copy notes from '%s' to '%s'"),
			      split.பொருட்கள்[0].string, split.பொருட்கள்[1].string);
			ret = 1;
		}

		string_list_clear(&split, 0);
	}

	எனில் (!rewrite_cmd) {
		commit_notes(the_repository, t, செய்தி);
		free_notes(t);
	} இல்லையெனில் {
		finish_copy_notes_for_rewrite(the_repository, c, செய்தி);
	}
	strbuf_release(&buf);
	திரும்பு ret;
}

நிலையான கட்டமைப்பு notes_மரம் *init_notes_check(மாறிலி எழுத்து *subcommand,
					   முழு flags)
{
	கட்டமைப்பு notes_மரம் *t;
	மாறிலி எழுத்து *ref;
	init_notes(வெற்று, வெற்று, வெற்று, flags);
	t = &default_notes_மரம்;

	ref = (flags & NOTES_INIT_WRITABLE) ? t->update_ref : t->ref;
	எனில் (!starts_with(ref, "refs/notes/"))
		/*
		 * TRANSLATORS: the first %s will be replaced by a git
		 * notes command: 'add', 'merge', 'remove', etc.
		 */
		die(_("refusing to %s notes in %s (outside of refs/notes/)"),
		    subcommand, ref);
	திரும்பு t;
}

நிலையான முழு list(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு notes_மரம் *t;
	கட்டமைப்பு object_id object;
	மாறிலி கட்டமைப்பு object_id *note;
	முழு retval = -1;
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	எனில் (argc)
		argc = parse_options(argc, argv, prefix, options,
				     git_notes_list_usage, 0);

	எனில் (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_list_usage, options);
	}

	t = init_notes_check("list", 0);
	எனில் (argc) {
		எனில் (repo_get_oid(the_repository, argv[0], &object))
			die(_("failed to resolve '%s' as a valid ref."), argv[0]);
		note = get_note(t, &object);
		எனில் (note) {
			puts(oid_to_hex(note));
			retval = 0;
		} இல்லையெனில்
			retval = error(_("no note found for object %s."),
				       oid_to_hex(&object));
	} இல்லையெனில்
		retval = for_each_note(t, 0, list_each_note, வெற்று);

	free_notes(t);
	திரும்பு retval;
}

நிலையான முழு append_edit(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED);

நிலையான முழு add(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
	       கட்டமைப்பு repository *repo)
{
	முழு force = 0, allow_empty = 0;
	மாறிலி எழுத்து *object_ref;
	கட்டமைப்பு notes_மரம் *t;
	கட்டமைப்பு object_id object, new_note;
	மாறிலி கட்டமைப்பு object_id *note;
	கட்டமைப்பு note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };

	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_செய்தி_arg),
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

	எனில் (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_add_usage, options);
	}

	எனில் (d.செய்தி_nr)
		concat_messages(&d);

	object_ref = argc > 1 ? argv[1] : "HEAD";

	எனில் (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("add", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	எனில் (note) {
		எனில் (!force) {
			free_notes(t);
			எனில் (d.செய்தி_nr) {
				free_note_data(&d);
				திரும்பு error(_("Cannot add notes. "
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
			திரும்பு append_edit(argc, argv, prefix, repo);
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	prepare_note_data(&object, &d, note);
	எனில் (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		எனில் (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		commit_notes(the_repository, t,
			     "Notes added by 'git notes add'");
	} இல்லையெனில் {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes add'");
	}

	free_note_data(&d);
	free_notes(t);
	திரும்பு 0;
}

நிலையான முழு copy(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	முழு retval = 0, force = 0, from_stdin = 0;
	மாறிலி கட்டமைப்பு object_id *from_note, *note;
	மாறிலி எழுத்து *object_ref;
	கட்டமைப்பு object_id object, from_obj;
	கட்டமைப்பு notes_மரம் *t;
	மாறிலி எழுத்து *rewrite_cmd = வெற்று;
	கட்டமைப்பு option options[] = {
		OPT__FORCE(&force, N_("replace existing notes"), PARSE_OPT_NOCOMPLETE),
		OPT_BOOL(0, "stdin", &from_stdin, N_("read objects from stdin")),
		OPT_STRING(0, "for-rewrite", &rewrite_cmd, N_("command"),
			   N_("load rewriting config for <command> (implies "
			      "--stdin)")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_copy_usage,
			     0);

	எனில் (from_stdin || rewrite_cmd) {
		எனில் (argc) {
			error(_("too many arguments"));
			usage_with_options(git_notes_copy_usage, options);
		} இல்லையெனில் {
			திரும்பு notes_copy_from_stdin(force, rewrite_cmd);
		}
	}

	எனில் (argc < 1) {
		error(_("too few arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}
	எனில் (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_copy_usage, options);
	}

	எனில் (repo_get_oid(the_repository, argv[0], &from_obj))
		die(_("failed to resolve '%s' as a valid ref."), argv[0]);

	object_ref = 1 < argc ? argv[1] : "HEAD";

	எனில் (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("copy", NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	எனில் (note) {
		எனில் (!force) {
			retval = error(_("Cannot copy notes. Found existing "
				       "notes for object %s. Use '-f' to "
				       "overwrite existing notes"),
				       oid_to_hex(&object));
			செல் out;
		}
		fprintf(stderr, _("Overwriting existing notes for object %s\n"),
			oid_to_hex(&object));
	}

	from_note = get_note(t, &from_obj);
	எனில் (!from_note) {
		retval = error(_("missing notes on source object %s. Cannot "
			       "copy."), oid_to_hex(&from_obj));
		செல் out;
	}

	எனில் (add_note(t, &object, from_note, combine_notes_overwrite))
		BUG("combine_notes_overwrite failed");
	commit_notes(the_repository, t,
		     "Notes added by 'git notes copy'");
out:
	free_notes(t);
	திரும்பு retval;
}

நிலையான முழு append_edit(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		       கட்டமைப்பு repository *repo UNUSED)
{
	முழு allow_empty = 0;
	மாறிலி எழுத்து *object_ref;
	கட்டமைப்பு notes_மரம் *t;
	கட்டமைப்பு object_id object, new_note;
	மாறிலி கட்டமைப்பு object_id *note;
	எழுத்து *logmsg;
	மாறிலி எழுத்து * மாறிலி *usage;
	கட்டமைப்பு note_data d = { .buf = STRBUF_INIT, .stripspace = UNSPECIFIED };
	கட்டமைப்பு option options[] = {
		OPT_CALLBACK_F('m', "message", &d, N_("message"),
			N_("note contents as a string"), PARSE_OPT_NONEG,
			parse_செய்தி_arg),
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
	முழு edit = !strcmp(argv[0], "edit");

	usage = edit ? git_notes_edit_usage : git_notes_append_usage;
	argc = parse_options(argc, argv, prefix, options, usage,
			     PARSE_OPT_KEEP_ARGV0);

	எனில் (2 < argc) {
		error(_("too many arguments"));
		usage_with_options(usage, options);
	}

	எனில் (d.செய்தி_nr) {
		concat_messages(&d);
		எனில் (edit)
			fprintf(stderr, _("The -m/-F/-c/-C options have been "
				"deprecated for the 'edit' subcommand.\n"
				"Please use 'git notes add -f -m/-F/-c/-C' "
				"instead.\n"));
	}

	object_ref = 1 < argc ? argv[1] : "HEAD";

	எனில் (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check(argv[0], NOTES_INIT_WRITABLE);
	note = get_note(t, &object);

	prepare_note_data(&object, &d, edit && note ? note : வெற்று);

	எனில் (note && !edit) {
		/* Append buf to previous note contents */
		size_t size;
		எண்ணிக்கை object_type type;
		கட்டமைப்பு strbuf buf = STRBUF_INIT;
		எழுத்து *prev_buf = odb_read_object(the_repository->objects, note, &type, &size);

		எனில் (!prev_buf)
			die(_("unable to read %s"), oid_to_hex(note));
		எனில் (size)
			strbuf_add(&buf, prev_buf, size);
		எனில் (d.buf.len && size)
			append_separator(&buf);
		strbuf_செருகு(&d.buf, 0, buf.buf, buf.len);

		விடுவி(prev_buf);
		strbuf_release(&buf);
	}

	எனில் (d.buf.len || allow_empty) {
		write_note_data(&d, &new_note);
		எனில் (add_note(t, &object, &new_note, combine_notes_overwrite))
			BUG("combine_notes_overwrite failed");
		logmsg = xstrfmt("Notes added by 'git notes %s'", argv[0]);
	} இல்லையெனில் {
		fprintf(stderr, _("Removing note for object %s\n"),
			oid_to_hex(&object));
		remove_note(t, object.hash);
		logmsg = xstrfmt("Notes removed by 'git notes %s'", argv[0]);
	}
	commit_notes(the_repository, t, logmsg);

	விடுவி(logmsg);
	free_note_data(&d);
	free_notes(t);
	திரும்பு 0;
}

நிலையான முழு show(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		கட்டமைப்பு repository *repo UNUSED)
{
	மாறிலி எழுத்து *object_ref;
	கட்டமைப்பு notes_மரம் *t;
	கட்டமைப்பு object_id object;
	மாறிலி கட்டமைப்பு object_id *note;
	முழு retval;
	கட்டமைப்பு option options[] = {
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_show_usage,
			     0);

	எனில் (1 < argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_show_usage, options);
	}

	object_ref = argc ? argv[0] : "HEAD";

	எனில் (repo_get_oid(the_repository, object_ref, &object))
		die(_("failed to resolve '%s' as a valid ref."), object_ref);

	t = init_notes_check("show", 0);
	note = get_note(t, &object);

	எனில் (!note)
		retval = error(_("no note found for object %s."),
			       oid_to_hex(&object));
	இல்லையெனில் {
		மாறிலி எழுத்து *show_args[3] = {"show", oid_to_hex(note), வெற்று};
		retval = execv_git_cmd(show_args);
	}
	free_notes(t);
	திரும்பு retval;
}

நிலையான முழு merge_abort(கட்டமைப்பு notes_merge_options *o)
{
	முழு ret = 0;

	/*
	 * Remove .git/NOTES_MERGE_PARTIAL and .git/NOTES_MERGE_REF, and call
	 * notes_merge_abort() to remove .git/NOTES_MERGE_WORKTREE.
	 */

	எனில் (refs_delete_ref(get_main_ref_store(the_repository), வெற்று, "NOTES_MERGE_PARTIAL", வெற்று, 0))
		ret += error(_("failed to delete ref NOTES_MERGE_PARTIAL"));
	எனில் (refs_delete_ref(get_main_ref_store(the_repository), வெற்று, "NOTES_MERGE_REF", வெற்று, REF_NO_DEREF))
		ret += error(_("failed to delete ref NOTES_MERGE_REF"));
	எனில் (notes_merge_abort(o))
		ret += error(_("failed to remove 'git notes merge' worktree"));
	திரும்பு ret;
}

நிலையான முழு merge_commit(கட்டமைப்பு notes_merge_options *o)
{
	கட்டமைப்பு strbuf செய்தி = STRBUF_INIT;
	கட்டமைப்பு object_id oid, parent_oid;
	கட்டமைப்பு notes_மரம் t = {0};
	கட்டமைப்பு commit *partial;
	கட்டமைப்பு pretty_print_context pretty_ctx;
	வெறுமை *local_ref_to_free;
	முழு ret;

	/*
	 * Read partial merge result from .git/NOTES_MERGE_PARTIAL,
	 * and target notes ref from .git/NOTES_MERGE_REF.
	 */

	எனில் (repo_get_oid(the_repository, "NOTES_MERGE_PARTIAL", &oid))
		die(_("failed to read ref NOTES_MERGE_PARTIAL"));
	இல்லையெனில் எனில் (!(partial = lookup_commit_reference(the_repository, &oid)))
		die(_("could not find commit from NOTES_MERGE_PARTIAL."));
	இல்லையெனில் எனில் (repo_parse_commit(the_repository, partial))
		die(_("could not parse commit from NOTES_MERGE_PARTIAL."));

	எனில் (partial->parents)
		oidcpy(&parent_oid, &partial->parents->item->object.oid);
	இல்லையெனில்
		oidclr(&parent_oid, the_repository->hash_algo);

	init_notes(&t, "NOTES_MERGE_PARTIAL", combine_notes_overwrite, 0);

	o->local_ref = local_ref_to_free =
		refs_resolve_refdup(get_main_ref_store(the_repository),
				    "NOTES_MERGE_REF", 0, &oid, வெற்று);
	எனில் (!o->local_ref)
		die(_("failed to resolve NOTES_MERGE_REF"));

	எனில் (notes_merge_commit(o, &t, partial, &oid))
		die(_("failed to finalize notes merge"));

	/* Reuse existing commit message in reflog message */
	memset(&pretty_ctx, 0, அளவு(pretty_ctx));
	repo_format_commit_message(the_repository, partial, "%s", &செய்தி,
				   &pretty_ctx);
	strbuf_trim(&செய்தி);
	strbuf_insertstr(&செய்தி, 0, "notes: ");
	refs_update_ref(get_main_ref_store(the_repository), செய்தி.buf,
			o->local_ref, &oid,
			is_null_oid(&parent_oid) ? வெற்று : &parent_oid,
			0, UPDATE_REFS_DIE_ON_ERR);

	free_notes(&t);
	strbuf_release(&செய்தி);
	ret = merge_abort(o);
	விடுவி(local_ref_to_free);
	திரும்பு ret;
}

நிலையான முழு git_config_get_notes_strategy(மாறிலி எழுத்து *சாவி,
					 எண்ணிக்கை notes_merge_strategy *strategy)
{
	எழுத்து *மதிப்பு;

	எனில் (repo_config_get_string(the_repository, சாவி, &மதிப்பு))
		திரும்பு 1;
	எனில் (parse_notes_merge_strategy(மதிப்பு, strategy))
		git_die_config(the_repository, சாவி, _("unknown notes merge strategy %s"), மதிப்பு);

	விடுவி(மதிப்பு);
	திரும்பு 0;
}

நிலையான முழு merge(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு strbuf remote_ref = STRBUF_INIT, செய்தி = STRBUF_INIT;
	கட்டமைப்பு object_id result_oid;
	கட்டமைப்பு notes_மரம் *t;
	கட்டமைப்பு notes_merge_options o;
	முழு do_merge = 0, do_commit = 0, do_abort = 0;
	முழு verbosity = 0, result;
	மாறிலி எழுத்து *strategy = வெற்று;
	கட்டமைப்பு option options[] = {
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
	எழுத்து *notes_ref;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_merge_usage, 0);

	எனில் (strategy || do_commit + do_abort == 0)
		do_merge = 1;
	எனில் (do_merge + do_commit + do_abort != 1) {
		error(_("cannot mix --commit, --abort or -s/--strategy"));
		usage_with_options(git_notes_merge_usage, options);
	}

	எனில் (do_merge && argc != 1) {
		error(_("must specify a notes ref to merge"));
		usage_with_options(git_notes_merge_usage, options);
	} இல்லையெனில் எனில் (!do_merge && argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_merge_usage, options);
	}

	init_notes_merge_options(the_repository, &o);
	o.verbosity = verbosity + NOTES_MERGE_VERBOSITY_DEFAULT;

	எனில் (do_abort)
		திரும்பு merge_abort(&o);
	எனில் (do_commit)
		திரும்பு merge_commit(&o);

	notes_ref = default_notes_ref(the_repository);
	o.local_ref = notes_ref;
	strbuf_addstr(&remote_ref, argv[0]);
	expand_loose_notes_ref(&remote_ref);
	o.remote_ref = remote_ref.buf;

	t = init_notes_check("merge", NOTES_INIT_WRITABLE);

	எனில் (strategy) {
		எனில் (parse_notes_merge_strategy(strategy, &o.strategy)) {
			error(_("unknown -s/--strategy: %s"), strategy);
			usage_with_options(git_notes_merge_usage, options);
		}
	} இல்லையெனில் {
		கட்டமைப்பு strbuf merge_சாவி = STRBUF_INIT;
		மாறிலி எழுத்து *short_ref = வெற்று;

		எனில் (!skip_prefix(o.local_ref, "refs/notes/", &short_ref))
			BUG("local ref %s is outside of refs/notes/",
			    o.local_ref);

		strbuf_addf(&merge_சாவி, "notes.%s.mergeStrategy", short_ref);

		எனில் (git_config_get_notes_strategy(merge_சாவி.buf, &o.strategy))
			git_config_get_notes_strategy("notes.mergeStrategy", &o.strategy);

		strbuf_release(&merge_சாவி);
	}

	strbuf_addf(&செய்தி, "notes: Merged notes from %s into %s",
		    remote_ref.buf, notes_ref);
	strbuf_add(&(o.commit_செய்தி), செய்தி.buf + 7, செய்தி.len - 7); /* skip "notes: " */

	result = notes_merge(&o, t, &result_oid);

	எனில் (result >= 0) /* Merge resulted (trivially) in result_oid */
		/* Update default notes ref with new commit */
		refs_update_ref(get_main_ref_store(the_repository), செய்தி.buf,
				notes_ref, &result_oid, வெற்று, 0,
				UPDATE_REFS_DIE_ON_ERR);
	இல்லையெனில் { /* Merge has unresolved conflicts */
		கட்டமைப்பு worktree **worktrees;
		மாறிலி கட்டமைப்பு worktree *wt;
		எழுத்து *path;

		/* Update .git/NOTES_MERGE_PARTIAL with partial merge result */
		refs_update_ref(get_main_ref_store(the_repository), செய்தி.buf,
				"NOTES_MERGE_PARTIAL", &result_oid, வெற்று,
				0, UPDATE_REFS_DIE_ON_ERR);
		/* Store ref-to-be-updated into .git/NOTES_MERGE_REF */
		worktrees = get_worktrees(the_repository);
		wt = find_shared_symref(worktrees, "NOTES_MERGE_REF",
					notes_ref);
		எனில் (wt)
			die(_("a notes merge into %s is already in-progress at %s"),
			    notes_ref, wt->path);
		free_worktrees(worktrees);
		எனில் (refs_update_symref(get_main_ref_store(the_repository), "NOTES_MERGE_REF", notes_ref, வெற்று))
			die(_("failed to store link to current notes ref (%s)"),
			    notes_ref);

		path = repo_git_path(the_repository, NOTES_MERGE_WORKTREE);
		fprintf(stderr, _("Automatic notes merge failed. Fix conflicts in %s "
				  "and commit the result with 'git notes merge --commit', "
				  "or abort the merge with 'git notes merge --abort'.\n"),
			path);
		விடுவி(path);
	}

	free_notes(t);
	விடுவி(notes_ref);
	strbuf_release(&remote_ref);
	strbuf_release(&செய்தி);
	திரும்பு result < 0; /* return non-zero on conflicts */
}

#define IGNORE_MISSING 1

நிலையான முழு remove_one_note(கட்டமைப்பு notes_மரம் *t, மாறிலி எழுத்து *name, unsigned flag)
{
	முழு status;
	கட்டமைப்பு object_id oid;
	எனில் (repo_get_oid(the_repository, name, &oid))
		திரும்பு error(_("Failed to resolve '%s' as a valid ref."), name);
	status = remove_note(t, oid.hash);
	எனில் (status)
		fprintf(stderr, _("Object %s has no note\n"), name);
	இல்லையெனில்
		fprintf(stderr, _("Removing note for object %s\n"), name);
	திரும்பு (flag & IGNORE_MISSING) ? 0 : status;
}

நிலையான முழு remove_cmd(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		      கட்டமைப்பு repository *repo UNUSED)
{
	unsigned flag = 0;
	முழு from_stdin = 0;
	கட்டமைப்பு option options[] = {
		OPT_BIT(0, "ignore-missing", &flag,
			N_("attempt to remove non-existent note is not an error"),
			IGNORE_MISSING),
		OPT_BOOL(0, "stdin", &from_stdin,
			    N_("read object names from the standard input")),
		OPT_END()
	};
	கட்டமைப்பு notes_மரம் *t;
	முழு retval = 0;

	argc = parse_options(argc, argv, prefix, options,
			     git_notes_remove_usage, 0);

	t = init_notes_check("remove", NOTES_INIT_WRITABLE);

	எனில் (!argc && !from_stdin) {
		retval = remove_one_note(t, "HEAD", flag);
	} இல்லையெனில் {
		போது (*argv) {
			retval |= remove_one_note(t, *argv, flag);
			argv++;
		}
	}
	எனில் (from_stdin) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		போது (strbuf_getwholeline(&sb, stdin, '\n') != EOF) {
			strbuf_rtrim(&sb);
			retval |= remove_one_note(t, sb.buf, flag);
		}
		strbuf_release(&sb);
	}
	எனில் (!retval)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes remove'");
	free_notes(t);
	திரும்பு retval;
}

நிலையான முழு prune(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		 கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு notes_மரம் *t;
	முழு show_only = 0, verbose = 0;
	கட்டமைப்பு option options[] = {
		OPT__DRY_RUN(&show_only, N_("do not remove, show only")),
		OPT__VERBOSE(&verbose, N_("report pruned notes")),
		OPT_END()
	};

	argc = parse_options(argc, argv, prefix, options, git_notes_prune_usage,
			     0);

	எனில் (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_prune_usage, options);
	}

	t = init_notes_check("prune", NOTES_INIT_WRITABLE);

	prune_notes(t, (verbose ? NOTES_PRUNE_VERBOSE : 0) |
		(show_only ? NOTES_PRUNE_VERBOSE|NOTES_PRUNE_DRYRUN : 0) );
	எனில் (!show_only)
		commit_notes(the_repository, t,
			     "Notes removed by 'git notes prune'");
	free_notes(t);
	திரும்பு 0;
}

நிலையான முழு get_ref(முழு argc, மாறிலி எழுத்து **argv, மாறிலி எழுத்து *prefix,
		   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு option options[] = { OPT_END() };
	எழுத்து *notes_ref;
	argc = parse_options(argc, argv, prefix, options,
			     git_notes_get_ref_usage, 0);

	எனில் (argc) {
		error(_("too many arguments"));
		usage_with_options(git_notes_get_ref_usage, options);
	}

	notes_ref = default_notes_ref(the_repository);
	puts(notes_ref);
	விடுவி(notes_ref);
	திரும்பு 0;
}

முழு cmd_notes(முழு argc,
	      மாறிலி எழுத்து **argv,
	      மாறிலி எழுத்து *prefix,
	      கட்டமைப்பு repository *repo)
{
	மாறிலி எழுத்து *override_notes_ref = வெற்று;
	parse_opt_subcommand_fn *fn = வெற்று;
	கட்டமைப்பு option options[] = {
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

	repo_config(the_repository, git_default_config, வெற்று);
	argc = parse_options(argc, argv, prefix, options, git_notes_usage,
			     PARSE_OPT_SUBCOMMAND_OPTIONAL);
	எனில் (!fn) {
		எனில் (argc) {
			error(_("unknown subcommand: `%s'"), argv[0]);
			usage_with_options(git_notes_usage, options);
		}
		fn = list;
	}

	எனில் (override_notes_ref) {
		கட்டமைப்பு strbuf sb = STRBUF_INIT;
		strbuf_addstr(&sb, override_notes_ref);
		expand_notes_ref(&sb);
		setenv("GIT_NOTES_REF", sb.buf, 1);
		strbuf_release(&sb);
	}

	திரும்பு !!fn(argc, argv, prefix, repo);
}
