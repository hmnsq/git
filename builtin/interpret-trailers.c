/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "parse-options.h"
#समावेश "string-list.h"
#समावेश "tempfile.h"
#समावेश "trailer.h"
#समावेश "config.h"

अटल स्थिर अक्षर * स्थिर git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	शून्य
};

अटल क्रमागत trailer_where where;
अटल क्रमागत trailer_if_exists if_exists;
अटल क्रमागत trailer_if_missing if_missing;

अटल पूर्णांक option_parse_where(स्थिर काष्ठा option *opt,
			      स्थिर अक्षर *arg, पूर्णांक unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	वापस trailer_set_where(opt->मूल्य, arg);
}

अटल पूर्णांक option_parse_if_exists(स्थिर काष्ठा option *opt,
				  स्थिर अक्षर *arg, पूर्णांक unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	वापस trailer_set_if_exists(opt->मूल्य, arg);
}

अटल पूर्णांक option_parse_if_missing(स्थिर काष्ठा option *opt,
				   स्थिर अक्षर *arg, पूर्णांक unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	वापस trailer_set_if_missing(opt->मूल्य, arg);
}

अटल व्योम new_trailers_clear(काष्ठा list_शीर्ष *trailers)
{
	काष्ठा list_शीर्ष *pos, *tmp;
	काष्ठा new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, काष्ठा new_trailer_item, list);
		list_del(pos);
		मुक्त(item);
	}
}

अटल पूर्णांक option_parse_trailer(स्थिर काष्ठा option *opt,
				   स्थिर अक्षर *arg, पूर्णांक unset)
{
	काष्ठा list_शीर्ष *trailers = opt->मूल्य;
	काष्ठा new_trailer_item *item;

	अगर (unset) {
		new_trailers_clear(trailers);
		वापस 0;
	}

	अगर (!arg)
		वापस -1;

	item = xmalloc(माप(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_पुच्छ(&item->list, trailers);
	वापस 0;
}

अटल पूर्णांक parse_opt_parse(स्थिर काष्ठा option *opt, स्थिर अक्षर *arg,
			   पूर्णांक unset)
{
	काष्ठा process_trailer_options *v = opt->मूल्य;
	v->only_trailers = 1;
	v->only_निवेश = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	वापस 0;
}

अटल व्योम read_निवेश_file(काष्ठा strbuf *sb, स्थिर अक्षर *file)
{
	अगर (file) {
		अगर (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} अन्यथा {
		अगर (strbuf_read(sb, fileno(मानक_निवेश), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_पंक्ति(sb);
}

अटल व्योम interpret_trailers(स्थिर काष्ठा process_trailer_options *opts,
			       काष्ठा list_शीर्ष *new_trailer_शीर्ष,
			       स्थिर अक्षर *file)
{
	काष्ठा strbuf निवेश = STRBUF_INIT;
	काष्ठा strbuf out = STRBUF_INIT;
	काष्ठा tempfile *tempfile = शून्य;
	पूर्णांक fd = 1;

	trailer_config_init();

	read_निवेश_file(&निवेश, file);

	अगर (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		अगर (!tempfile)
			die(शून्य);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_शीर्ष, &निवेश, &out);

	अगर (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	अगर (opts->in_place)
		अगर (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&निवेश);
	strbuf_release(&out);
}

पूर्णांक cmd_interpret_trailers(पूर्णांक argc,
			   स्थिर अक्षर **argv,
			   स्थिर अक्षर *prefix,
			   काष्ठा repository *repo UNUSED)
{
	काष्ठा process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_शीर्ष(trailers);

	काष्ठा option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_निवेश, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, शून्य, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	अगर (opts.only_निवेश && !list_empty(&trailers))
		usage_संदेश_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	अगर (argc) {
		पूर्णांक i;
		क्रम (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} अन्यथा {
		अगर (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, शून्य);
	}

	new_trailers_clear(&trailers);

	वापस 0;
}
