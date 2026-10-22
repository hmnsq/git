/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "parse-options.h"
#ضمن "string-list.h"
#ضمن "tempfile.h"
#ضمن "trailer.h"
#ضمن "config.h"

ساكن ثابت حرف * ثابت git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	عدم
};

ساكن تعداد trailer_where where;
ساكن تعداد trailer_if_exists if_exists;
ساكن تعداد trailer_if_missing if_missing;

ساكن صحيح option_parse_where(ثابت بنية option *opt,
			      ثابت حرف *arg, صحيح unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	החזר trailer_set_where(opt->value, arg);
}

ساكن صحيح option_parse_if_exists(ثابت بنية option *opt,
				  ثابت حرف *arg, صحيح unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	החזר trailer_set_if_exists(opt->value, arg);
}

ساكن صحيح option_parse_if_missing(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	החזר trailer_set_if_missing(opt->value, arg);
}

ساكن فراغ new_trailers_clear(بنية list_head *trailers)
{
	بنية list_head *pos, *tmp;
	بنية new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, بنية new_trailer_item, list);
		list_del(pos);
		حرر(item);
	}
}

ساكن صحيح option_parse_trailer(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية list_head *trailers = opt->value;
	بنية new_trailer_item *item;

	אם (unset) {
		new_trailers_clear(trailers);
		החזר 0;
	}

	אם (!arg)
		החזר -1;

	item = xmalloc(حجم(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	החזר 0;
}

ساكن صحيح parse_opt_parse(ثابت بنية option *opt, ثابت حرف *arg,
			   صحيح unset)
{
	بنية process_trailer_options *v = opt->value;
	v->only_trailers = 1;
	v->only_input = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	החזר 0;
}

ساكن فراغ read_input_file(بنية strbuf *sb, ثابت حرف *file)
{
	אם (file) {
		אם (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} אחרת {
		אם (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_line(sb);
}

ساكن فراغ interpret_trailers(ثابت بنية process_trailer_options *opts,
			       بنية list_head *new_trailer_head,
			       ثابت حرف *file)
{
	بنية strbuf input = STRBUF_INIT;
	بنية strbuf out = STRBUF_INIT;
	بنية tempfile *tempfile = عدم;
	صحيح fd = 1;

	trailer_config_init();

	read_input_file(&input, file);

	אם (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		אם (!tempfile)
			die(عدم);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &input, &out);

	אם (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	אם (opts->in_place)
		אם (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&input);
	strbuf_release(&out);
}

صحيح cmd_interpret_trailers(صحيح argc,
			   ثابت حرف **argv,
			   ثابت حرف *prefix,
			   بنية repository *repo UNUSED)
{
	بنية process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	بنية option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_input, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, عدم, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	אם (opts.only_input && !list_empty(&trailers))
		usage_msg_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	אם (argc) {
		صحيح i;
		עבור (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} אחרת {
		אם (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, عدم);
	}

	new_trailers_clear(&trailers);

	החזר 0;
}
