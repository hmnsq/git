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

स्थिर ध्रुव वर्ण * ध्रुव git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	रिक्त
};

स्थिर गणना trailer_where where;
स्थिर गणना trailer_if_exists if_exists;
स्थिर गणना trailer_if_missing if_missing;

स्थिर पूर्णाङ्क option_parse_where(ध्रुव संरचना option *opt,
			      ध्रुव वर्ण *arg, पूर्णाङ्क unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	प्रत्यागम trailer_set_where(opt->मूल्य, arg);
}

स्थिर पूर्णाङ्क option_parse_if_exists(ध्रुव संरचना option *opt,
				  ध्रुव वर्ण *arg, पूर्णाङ्क unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	प्रत्यागम trailer_set_if_exists(opt->मूल्य, arg);
}

स्थिर पूर्णाङ्क option_parse_if_missing(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	प्रत्यागम trailer_set_if_missing(opt->मूल्य, arg);
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

स्थिर पूर्णाङ्क option_parse_trailer(ध्रुव संरचना option *opt,
				   ध्रुव वर्ण *arg, पूर्णाङ्क unset)
{
	संरचना list_head *trailers = opt->मूल्य;
	संरचना new_trailer_item *item;

	यदि (unset) {
		new_trailers_clear(trailers);
		प्रत्यागम 0;
	}

	यदि (!arg)
		प्रत्यागम -1;

	item = xmalloc(परिमाण(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क parse_opt_parse(ध्रुव संरचना option *opt, ध्रुव वर्ण *arg,
			   पूर्णाङ्क unset)
{
	संरचना process_trailer_options *v = opt->मूल्य;
	v->only_trailers = 1;
	v->only_आदान = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	प्रत्यागम 0;
}

स्थिर शून्य read_आदान_file(संरचना strbuf *sb, ध्रुव वर्ण *file)
{
	यदि (file) {
		यदि (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} अन्यथा {
		यदि (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_ओळ(sb);
}

स्थिर शून्य interpret_trailers(ध्रुव संरचना process_trailer_options *opts,
			       संरचना list_head *new_trailer_head,
			       ध्रुव वर्ण *file)
{
	संरचना strbuf आदान = STRBUF_INIT;
	संरचना strbuf out = STRBUF_INIT;
	संरचना tempfile *tempfile = रिक्त;
	पूर्णाङ्क fd = 1;

	trailer_config_init();

	read_आदान_file(&आदान, file);

	यदि (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		यदि (!tempfile)
			die(रिक्त);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &आदान, &out);

	यदि (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	यदि (opts->in_place)
		यदि (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&आदान);
	strbuf_release(&out);
}

पूर्णाङ्क cmd_interpret_trailers(पूर्णाङ्क argc,
			   ध्रुव वर्ण **argv,
			   ध्रुव वर्ण *prefix,
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

	यदि (opts.only_आदान && !list_empty(&trailers))
		usage_संदेश_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	यदि (argc) {
		पूर्णाङ्क i;
		प्रति (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} अन्यथा {
		यदि (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, रिक्त);
	}

	new_trailers_clear(&trailers);

	प्रत्यागम 0;
}
