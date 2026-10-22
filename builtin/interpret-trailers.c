/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "parse-options.h"
#شامل "string-list.h"
#شامل "tempfile.h"
#شامل "trailer.h"
#شامل "config.h"

ساکن ثابت حرف * ثابت git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	تہی
};

ساکن شمار trailer_where where;
ساکن شمار trailer_if_exists if_exists;
ساکن شمار trailer_if_missing if_missing;

ساکن صحیح option_parse_where(ثابت ساخت option *opt,
			      ثابت حرف *arg, صحیح unset غیر_مستعمل)
{
	/* unset implies NULL arg, which is handled in our helper */
	واپس trailer_سیٹ_where(opt->قدر, arg);
}

ساکن صحیح option_parse_if_exists(ثابت ساخت option *opt,
				  ثابت حرف *arg, صحیح unset غیر_مستعمل)
{
	/* unset implies NULL arg, which is handled in our helper */
	واپس trailer_سیٹ_if_exists(opt->قدر, arg);
}

ساکن صحیح option_parse_if_missing(ثابت ساخت option *opt,
				   ثابت حرف *arg, صحیح unset غیر_مستعمل)
{
	/* unset implies NULL arg, which is handled in our helper */
	واپس trailer_سیٹ_if_missing(opt->قدر, arg);
}

ساکن خالی new_trailers_clear(ساخت list_سر *trailers)
{
	ساخت list_سر *pos, *tmp;
	ساخت new_trailer_شے *شے;

	list_for_each_محفوظ(pos, tmp, trailers) {
		شے = list_اندراج(pos, ساخت new_trailer_شے, list);
		list_del(pos);
		آزاد(شے);
	}
}

ساکن صحیح option_parse_trailer(ثابت ساخت option *opt,
				   ثابت حرف *arg, صحیح unset)
{
	ساخت list_سر *trailers = opt->قدر;
	ساخت new_trailer_شے *شے;

	اگر (unset) {
		new_trailers_clear(trailers);
		واپس 0;
	}

	اگر (!arg)
		واپس -1;

	شے = xmalloc(حجم(*شے));
	شے->متن = arg;
	شے->where = where;
	شے->if_exists = if_exists;
	شے->if_missing = if_missing;
	list_جوڑو_دم(&شے->list, trailers);
	واپس 0;
}

ساکن صحیح parse_opt_parse(ثابت ساخت option *opt, ثابت حرف *arg,
			   صحیح unset)
{
	ساخت process_trailer_options *v = opt->قدر;
	v->only_trailers = 1;
	v->only_داخلہ = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	واپس 0;
}

ساکن خالی read_داخلہ_file(ساخت strbuf *sb, ثابت حرف *file)
{
	اگر (file) {
		اگر (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} ورنہ {
		اگر (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_سطر(sb);
}

ساکن خالی interpret_trailers(ثابت ساخت process_trailer_options *opts,
			       ساخت list_سر *new_trailer_سر,
			       ثابت حرف *file)
{
	ساخت strbuf داخلہ = STRBUF_INIT;
	ساخت strbuf برآمد = STRBUF_INIT;
	ساخت tempfile *tempfile = تہی;
	صحیح fd = 1;

	trailer_config_init();

	read_داخلہ_file(&داخلہ, file);

	اگر (opts->اندر_رکھو) {
		tempfile = trailer_create_اندر_رکھو_tempfile(file);
		اگر (!tempfile)
			die(تہی);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_سر, &داخلہ, &برآمد);

	اگر (write_اندر_بھرا(fd, برآمد.buf, برآمد.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	اگر (opts->اندر_رکھو)
		اگر (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&داخلہ);
	strbuf_release(&برآمد);
}

صحیح cmd_interpret_trailers(صحیح argc,
			   ثابت حرف **argv,
			   ثابت حرف *سابقہ,
			   ساخت repository *repo غیر_مستعمل)
{
	ساخت process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_سر(trailers);

	ساخت option options[] = {
		OPT_BOOL(0, "in-place", &opts.اندر_رکھو, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_خالی_خانے, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_داخلہ, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, تہی, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_انتہا()
	};

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, options,
			     git_interpret_trailers_usage, 0);

	اگر (opts.only_داخلہ && !list_خالی_خانے(&trailers))
		usage_پیغام_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	اگر (argc) {
		صحیح i;
		برائے (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} ورنہ {
		اگر (opts.اندر_رکھو)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, تہی);
	}

	new_trailers_clear(&trailers);

	واپس 0;
}
