/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
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
			      ثابت حرف *arg, صحيح unset اڻ_استعمال_ٿيل)
{
	/* unset implies NULL arg, which is handled in our helper */
	أرجع trailer_سيٽ_where(opt->قدر, arg);
}

ساكن صحيح option_parse_if_exists(ثابت بنية option *opt,
				  ثابت حرف *arg, صحيح unset اڻ_استعمال_ٿيل)
{
	/* unset implies NULL arg, which is handled in our helper */
	أرجع trailer_سيٽ_if_exists(opt->قدر, arg);
}

ساكن صحيح option_parse_if_missing(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset اڻ_استعمال_ٿيل)
{
	/* unset implies NULL arg, which is handled in our helper */
	أرجع trailer_سيٽ_if_missing(opt->قدر, arg);
}

ساكن فراغ new_trailers_clear(بنية list_سر *trailers)
{
	بنية list_سر *pos, *tmp;
	بنية new_trailer_شي *شي;

	list_for_each_محفوظ(pos, tmp, trailers) {
		شي = list_داخلا(pos, بنية new_trailer_شي, list);
		list_del(pos);
		حرر(شي);
	}
}

ساكن صحيح option_parse_trailer(ثابت بنية option *opt,
				   ثابت حرف *arg, صحيح unset)
{
	بنية list_سر *trailers = opt->قدر;
	بنية new_trailer_شي *شي;

	إذا (unset) {
		new_trailers_clear(trailers);
		أرجع 0;
	}

	إذا (!arg)
		أرجع -1;

	شي = xmalloc(حجم(*شي));
	شي->متن = arg;
	شي->where = where;
	شي->if_exists = if_exists;
	شي->if_missing = if_missing;
	list_جوڙيو_پڇ(&شي->list, trailers);
	أرجع 0;
}

ساكن صحيح parse_opt_parse(ثابت بنية option *opt, ثابت حرف *arg,
			   صحيح unset)
{
	بنية process_trailer_options *v = opt->قدر;
	v->only_trailers = 1;
	v->only_داخلا = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	أرجع 0;
}

ساكن فراغ read_داخلا_file(بنية strbuf *sb, ثابت حرف *file)
{
	إذا (file) {
		إذا (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} وإلا {
		إذا (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_سٽ(sb);
}

ساكن فراغ interpret_trailers(ثابت بنية process_trailer_options *opts,
			       بنية list_سر *new_trailer_سر,
			       ثابت حرف *file)
{
	بنية strbuf داخلا_2 = STRBUF_INIT;
	بنية strbuf ٻاهريون = STRBUF_INIT;
	بنية tempfile *tempfile = عدم;
	صحيح fd = 1;

	trailer_config_init();

	read_داخلا_file(&داخلا_2, file);

	إذا (opts->اندريون_رکو) {
		tempfile = trailer_create_اندريون_رکو_tempfile(file);
		إذا (!tempfile)
			die(عدم);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_سر, &داخلا_2, &ٻاهريون);

	إذا (write_اندريون_ڀريل(fd, ٻاهريون.buf, ٻاهريون.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	إذا (opts->اندريون_رکو)
		إذا (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&داخلا_2);
	strbuf_release(&ٻاهريون);
}

صحيح cmd_interpret_trailers(صحيح argc,
			   ثابت حرف **argv,
			   ثابت حرف *اڳياڙي,
			   بنية repository *repo اڻ_استعمال_ٿيل)
{
	بنية process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_سر(trailers);

	بنية option options[] = {
		OPT_BOOL(0, "in-place", &opts.اندريون_رکو, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_خالي_جايون, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_داخلا, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, عدم, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_پڇاڙي()
	};

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, options,
			     git_interpret_trailers_usage, 0);

	إذا (opts.only_داخلا && !list_خالي_جايون(&trailers))
		usage_نياپو_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	إذا (argc) {
		صحيح i;
		لكل (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} وإلا {
		إذا (opts.اندريون_رکو)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, عدم);
	}

	new_trailers_clear(&trailers);

	أرجع 0;
}
