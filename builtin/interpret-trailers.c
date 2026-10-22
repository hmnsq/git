/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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
			      ثابت حرف *arg, صحيح unset ناکارول_شوی)
{
	/* unset implies NULL arg, which is handled in our helper */
	أرجع trailer_سېټ_where(opt->ارزښت, arg);
}

ساكن صحيح option_parse_if_exists(ثابت بنية option *opt,
				  ثابت حرف *arg, صحيح unset ناکارول_شوی)
{
	/* unset implies NULL arg, which is handled in our helper */
	أرجع trailer_سېټ_if_exists(opt->ارزښت, arg);
}

ساكن صحيح option_parse_if_missing(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset ناکارول_شوی)
{
	/* unset implies NULL arg, which is handled in our helper */
	أرجع trailer_سېټ_if_missing(opt->ارزښت, arg);
}

ساكن فراغ new_trailers_clear(بنية list_سر *trailers)
{
	بنية list_سر *pos, *tmp;
	بنية new_trailer_توکی *توکی;

	list_for_each_خوندي(pos, tmp, trailers) {
		توکی = list_ثبت(pos, بنية new_trailer_توکی, list);
		list_del(pos);
		حرر(توکی);
	}
}

ساكن صحيح option_parse_trailer(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية list_سر *trailers = opt->ارزښت;
	بنية new_trailer_توکی *توکی;

	إذا (unset) {
		new_trailers_clear(trailers);
		أرجع 0;
	}

	إذا (!arg)
		أرجع -1;

	توکی = xmalloc(حجم(*توکی));
	توکی->متن = arg;
	توکی->where = where;
	توکی->if_exists = if_exists;
	توکی->if_missing = if_missing;
	list_جمع_کړه_لکۍ(&توکی->list, trailers);
	أرجع 0;
}

ساكن صحيح parse_opt_parse(ثابت بنية option *opt, ثابت حرف *arg,
			   صحيح unset)
{
	بنية process_trailer_options *v = opt->ارزښت;
	v->only_trailers = 1;
	v->only_ننوت = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع 0;
}

ساكن فراغ read_ننوت_file(بنية strbuf *sb, ثابت حرف *file)
{
	إذا (file) {
		إذا (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} وإلا {
		إذا (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_کرښه(sb);
}

ساكن فراغ interpret_trailers(ثابت بنية process_trailer_options *opts,
			       بنية list_سر *new_trailer_سر,
			       ثابت حرف *file)
{
	بنية strbuf ننوت = STRBUF_INIT;
	بنية strbuf بهر = STRBUF_INIT;
	بنية tempfile *tempfile = عدم;
	صحيح fd = 1;

	trailer_config_init();

	read_ننوت_file(&ننوت, file);

	إذا (opts->دننه_ځای_پرځای_کړه) {
		tempfile = trailer_create_دننه_ځای_پرځای_کړه_tempfile(file);
		إذا (!tempfile)
			die(عدم);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_سر, &ننوت, &بهر);

	إذا (write_دننه_ډک(fd, بهر.buf, بهر.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	إذا (opts->دننه_ځای_پرځای_کړه)
		إذا (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&ننوت);
	strbuf_release(&بهر);
}

صحيح cmd_interpret_trailers(صحيح argc,
			   ثابت حرف **argv,
			   ثابت حرف *مختاړی,
			   بنية repository *repo ناکارول_شوی)
{
	بنية process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_سر(trailers);

	بنية option options[] = {
		OPT_BOOL(0, "in-place", &opts.دننه_ځای_پرځای_کړه, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_تشه, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_ننوت, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, عدم, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_پای_ټکی()
	};

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, مختاړی, options,
			     git_interpret_trailers_usage, 0);

	إذا (opts.only_ننوت && !list_تشه(&trailers))
		usage_پیغام_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	إذا (argc) {
		صحيح i;
		لكل (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} وإلا {
		إذا (opts.دننه_ځای_پرځای_کړه)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, عدم);
	}

	new_trailers_clear(&trailers);

	أرجع 0;
}
