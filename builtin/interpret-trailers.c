/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "string-list.h"
#شامل "tempfile.h"
#شامل "trailer.h"
#شامل "config.h"

ایستا ثابت نویسه * ثابت git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	پوچ
};

ایستا شمارش trailer_where where;
ایستا شمارش trailer_if_exists if_exists;
ایستا شمارش trailer_if_missing if_missing;

ایستا صحیح option_parse_where(ثابت ساختار option *opt,
			      ثابت نویسه *arg, صحیح unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	بازگشت trailer_set_where(opt->مقدار, arg);
}

ایستا صحیح option_parse_if_exists(ثابت ساختار option *opt,
				  ثابت نویسه *arg, صحیح unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	بازگشت trailer_set_if_exists(opt->مقدار, arg);
}

ایستا صحیح option_parse_if_missing(ثابت ساختار option *opt,
				   ثابت نویسه *arg, صحیح unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	بازگشت trailer_set_if_missing(opt->مقدار, arg);
}

ایستا تهی new_trailers_clear(ساختار list_head *trailers)
{
	ساختار list_head *pos, *tmp;
	ساختار new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, ساختار new_trailer_item, list);
		list_del(pos);
		آزاد(item);
	}
}

ایستا صحیح option_parse_trailer(ثابت ساختار option *opt,
				   ثابت نویسه *arg, صحیح unset)
{
	ساختار list_head *trailers = opt->مقدار;
	ساختار new_trailer_item *item;

	اگر (unset) {
		new_trailers_clear(trailers);
		بازگشت 0;
	}

	اگر (!arg)
		بازگشت -1;

	item = xmalloc(اندازه(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	بازگشت 0;
}

ایستا صحیح parse_opt_parse(ثابت ساختار option *opt, ثابت نویسه *arg,
			   صحیح unset)
{
	ساختار process_trailer_options *v = opt->مقدار;
	v->only_trailers = 1;
	v->only_درون_داد = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	بازگشت 0;
}

ایستا تهی read_درون_داد_file(ساختار strbuf *sb, ثابت نویسه *file)
{
	اگر (file) {
		اگر (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} وگرنه {
		اگر (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_سطر(sb);
}

ایستا تهی interpret_trailers(ثابت ساختار process_trailer_options *opts,
			       ساختار list_head *new_trailer_head,
			       ثابت نویسه *file)
{
	ساختار strbuf درون_داد = STRBUF_INIT;
	ساختار strbuf out = STRBUF_INIT;
	ساختار tempfile *tempfile = پوچ;
	صحیح fd = 1;

	trailer_config_init();

	read_درون_داد_file(&درون_داد, file);

	اگر (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		اگر (!tempfile)
			die(پوچ);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &درون_داد, &out);

	اگر (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	اگر (opts->in_place)
		اگر (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&درون_داد);
	strbuf_release(&out);
}

صحیح cmd_interpret_trailers(صحیح argc,
			   ثابت نویسه **argv,
			   ثابت نویسه *prefix,
			   ساختار repository *repo UNUSED)
{
	ساختار process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	ساختار option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_درون_داد, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, پوچ, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	اگر (opts.only_درون_داد && !list_empty(&trailers))
		usage_پیام_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	اگر (argc) {
		صحیح i;
		برای (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} وگرنه {
		اگر (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, پوچ);
	}

	new_trailers_clear(&trailers);

	بازگشت 0;
}
