/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "string-list.h"
#अन्तर्भाव "tempfile.h"
#अन्तर्भाव "trailer.h"
#अन्तर्भाव "config.h"

स्थिर स्थिरांक अक्षर * स्थिरांक git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	रिक्त
};

स्थिर गणना trailer_where where;
स्थिर गणना trailer_if_exists if_exists;
स्थिर गणना trailer_if_missing if_missing;

स्थिर पूर्णांक option_parse_where(स्थिरांक संरचना option *opt,
			      स्थिरांक अक्षर *arg, पूर्णांक unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	परत trailer_set_where(opt->मूल्य, arg);
}

स्थिर पूर्णांक option_parse_if_exists(स्थिरांक संरचना option *opt,
				  स्थिरांक अक्षर *arg, पूर्णांक unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	परत trailer_set_if_exists(opt->मूल्य, arg);
}

स्थिर पूर्णांक option_parse_if_missing(स्थिरांक संरचना option *opt,
				   स्थिरांक अक्षर *arg, पूर्णांक unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	परत trailer_set_if_missing(opt->मूल्य, arg);
}

स्थिर शून्य new_trailers_clear(संरचना list_head *trailers)
{
	संरचना list_head *pos, *tmp;
	संरचना new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, संरचना new_trailer_item, list);
		list_del(pos);
		मुक्त(item);
	}
}

स्थिर पूर्णांक option_parse_trailer(स्थिरांक संरचना option *opt,
				   स्थिरांक अक्षर *arg, पूर्णांक unset)
{
	संरचना list_head *trailers = opt->मूल्य;
	संरचना new_trailer_item *item;

	जर (unset) {
		new_trailers_clear(trailers);
		परत 0;
	}

	जर (!arg)
		परत -1;

	item = xmalloc(आकार(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	परत 0;
}

स्थिर पूर्णांक parse_opt_parse(स्थिरांक संरचना option *opt, स्थिरांक अक्षर *arg,
			   पूर्णांक unset)
{
	संरचना process_trailer_options *v = opt->मूल्य;
	v->only_trailers = 1;
	v->only_आदान = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	परत 0;
}

स्थिर शून्य read_आदान_file(संरचना strbuf *sb, स्थिरांक अक्षर *file)
{
	जर (file) {
		जर (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} अन्यथा {
		जर (strbuf_read(sb, fileno(मानक_निवेश), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_ओळ(sb);
}

स्थिर शून्य interpret_trailers(स्थिरांक संरचना process_trailer_options *opts,
			       संरचना list_head *new_trailer_head,
			       स्थिरांक अक्षर *file)
{
	संरचना strbuf आदान = STRBUF_INIT;
	संरचना strbuf out = STRBUF_INIT;
	संरचना tempfile *tempfile = रिक्त;
	पूर्णांक fd = 1;

	trailer_config_init();

	read_आदान_file(&आदान, file);

	जर (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		जर (!tempfile)
			die(रिक्त);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &आदान, &out);

	जर (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	जर (opts->in_place)
		जर (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&आदान);
	strbuf_release(&out);
}

पूर्णांक cmd_interpret_trailers(पूर्णांक argc,
			   स्थिरांक अक्षर **argv,
			   स्थिरांक अक्षर *prefix,
			   संरचना repository *repo UNUSED)
{
	संरचना process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	संरचना option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_आदान, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, रिक्त, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	जर (opts.only_आदान && !list_empty(&trailers))
		usage_संदेश_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	जर (argc) {
		पूर्णांक i;
		साठी (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} अन्यथा {
		जर (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, रिक्त);
	}

	new_trailers_clear(&trailers);

	परत 0;
}
