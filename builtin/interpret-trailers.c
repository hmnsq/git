/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "string-list.h"
#అన్తర్భావ "tempfile.h"
#అన్తర్భావ "trailer.h"
#అన్తర్భావ "config.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	ఖాళీ
};

స్థిర గణన trailer_where where;
స్థిర గణన trailer_if_exists if_exists;
స్థిర గణన trailer_if_missing if_missing;

స్థిర పూర్ణాంకం option_parse_where(స్థిరరాశి నిర్మాణం option *opt,
			      స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset వాడని)
{
	/* unset implies NULL arg, which is handled in our helper */
	తిరిగి trailer_సమితి_where(opt->విలువ, arg);
}

స్థిర పూర్ణాంకం option_parse_if_exists(స్థిరరాశి నిర్మాణం option *opt,
				  స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset వాడని)
{
	/* unset implies NULL arg, which is handled in our helper */
	తిరిగి trailer_సమితి_if_exists(opt->విలువ, arg);
}

స్థిర పూర్ణాంకం option_parse_if_missing(స్థిరరాశి నిర్మాణం option *opt,
				   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset వాడని)
{
	/* unset implies NULL arg, which is handled in our helper */
	తిరిగి trailer_సమితి_if_missing(opt->విలువ, arg);
}

స్థిర శూన్య new_trailers_clear(నిర్మాణం list_తల *trailers)
{
	నిర్మాణం list_తల *pos, *tmp;
	నిర్మాణం new_trailer_అంశం *అంశం;

	list_for_each_సురక్షిత(pos, tmp, trailers) {
		అంశం = list_నమోదు(pos, నిర్మాణం new_trailer_అంశం, list);
		list_del(pos);
		విడుదల(అంశం);
	}
}

స్థిర పూర్ణాంకం option_parse_trailer(స్థిరరాశి నిర్మాణం option *opt,
				   స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం list_తల *trailers = opt->విలువ;
	నిర్మాణం new_trailer_అంశం *అంశం;

	ఒకవేళ (unset) {
		new_trailers_clear(trailers);
		తిరిగి 0;
	}

	ఒకవేళ (!arg)
		తిరిగి -1;

	అంశం = xmalloc(పరిమాణం(*అంశం));
	అంశం->పాఠ్యం = arg;
	అంశం->where = where;
	అంశం->if_exists = if_exists;
	అంశం->if_missing = if_missing;
	list_కూడు_తోక(&అంశం->list, trailers);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం parse_opt_parse(స్థిరరాశి నిర్మాణం option *opt, స్థిరరాశి అక్షరం *arg,
			   పూర్ణాంకం unset)
{
	నిర్మాణం process_trailer_options *v = opt->విలువ;
	v->only_trailers = 1;
	v->only_ఆదానం = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	తిరిగి 0;
}

స్థిర శూన్య read_ఆదానం_file(నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *file)
{
	ఒకవేళ (file) {
		ఒకవేళ (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} లేకపోతే {
		ఒకవేళ (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_పంక్తి(sb);
}

స్థిర శూన్య interpret_trailers(స్థిరరాశి నిర్మాణం process_trailer_options *opts,
			       నిర్మాణం list_తల *new_trailer_తల,
			       స్థిరరాశి అక్షరం *file)
{
	నిర్మాణం strbuf ఆదానం = STRBUF_INIT;
	నిర్మాణం strbuf బహిర్గతం = STRBUF_INIT;
	నిర్మాణం tempfile *tempfile = ఖాళీ;
	పూర్ణాంకం fd = 1;

	trailer_config_init();

	read_ఆదానం_file(&ఆదానం, file);

	ఒకవేళ (opts->లోపల_ఉంచు) {
		tempfile = trailer_create_లోపల_ఉంచు_tempfile(file);
		ఒకవేళ (!tempfile)
			die(ఖాళీ);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_తల, &ఆదానం, &బహిర్గతం);

	ఒకవేళ (write_లోపల_నిండిన(fd, బహిర్గతం.buf, బహిర్గతం.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	ఒకవేళ (opts->లోపల_ఉంచు)
		ఒకవేళ (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&ఆదానం);
	strbuf_release(&బహిర్గతం);
}

పూర్ణాంకం cmd_interpret_trailers(పూర్ణాంకం argc,
			   స్థిరరాశి అక్షరం **argv,
			   స్థిరరాశి అక్షరం *ఉపసర్గ,
			   నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_తల(trailers);

	నిర్మాణం option options[] = {
		OPT_BOOL(0, "in-place", &opts.లోపల_ఉంచు, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_ఖాళీలు, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_ఆదానం, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, ఖాళీ, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_చివర()
	};

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, options,
			     git_interpret_trailers_usage, 0);

	ఒకవేళ (opts.only_ఆదానం && !list_ఖాళీలు(&trailers))
		usage_సందేశం_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	ఒకవేళ (argc) {
		పూర్ణాంకం i;
		కొరకు (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} లేకపోతే {
		ఒకవేళ (opts.లోపల_ఉంచు)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, ఖాళీ);
	}

	new_trailers_clear(&trailers);

	తిరిగి 0;
}
