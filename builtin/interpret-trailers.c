/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tempfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "trailer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ trailer_where where;
ਸਥਿਰ ਗਿਣਤੀ trailer_if_exists if_exists;
ਸਥਿਰ ਗਿਣਤੀ trailer_if_missing if_missing;

ਸਥਿਰ ਪੂਰਨ option_parse_where(ਅਚਲ ਬਣਤਰ option *opt,
			      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset ਅਣਵਰਤਿਆ)
{
	/* unset implies NULL arg, which is handled in our helper */
	ਵਾਪਸ trailer_ਸੈੱਟ_where(opt->ਮੁੱਲ, arg);
}

ਸਥਿਰ ਪੂਰਨ option_parse_if_exists(ਅਚਲ ਬਣਤਰ option *opt,
				  ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset ਅਣਵਰਤਿਆ)
{
	/* unset implies NULL arg, which is handled in our helper */
	ਵਾਪਸ trailer_ਸੈੱਟ_if_exists(opt->ਮੁੱਲ, arg);
}

ਸਥਿਰ ਪੂਰਨ option_parse_if_missing(ਅਚਲ ਬਣਤਰ option *opt,
				   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset ਅਣਵਰਤਿਆ)
{
	/* unset implies NULL arg, which is handled in our helper */
	ਵਾਪਸ trailer_ਸੈੱਟ_if_missing(opt->ਮੁੱਲ, arg);
}

ਸਥਿਰ ਖਾਲੀ new_trailers_clear(ਬਣਤਰ list_ਸਿਰ *trailers)
{
	ਬਣਤਰ list_ਸਿਰ *pos, *tmp;
	ਬਣਤਰ new_trailer_ਵਸਤੂ *ਵਸਤੂ;

	list_for_each_ਸੁਰੱਖਿਅਤ(pos, tmp, trailers) {
		ਵਸਤੂ = list_ਇੰਦਰਾਜ(pos, ਬਣਤਰ new_trailer_ਵਸਤੂ, list);
		list_del(pos);
		ਮੁਕਤ(ਵਸਤੂ);
	}
}

ਸਥਿਰ ਪੂਰਨ option_parse_trailer(ਅਚਲ ਬਣਤਰ option *opt,
				   ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਬਣਤਰ list_ਸਿਰ *trailers = opt->ਮੁੱਲ;
	ਬਣਤਰ new_trailer_ਵਸਤੂ *ਵਸਤੂ;

	ਜੇ (unset) {
		new_trailers_clear(trailers);
		ਵਾਪਸ 0;
	}

	ਜੇ (!arg)
		ਵਾਪਸ -1;

	ਵਸਤੂ = xmalloc(ਆਕਾਰ(*ਵਸਤੂ));
	ਵਸਤੂ->ਪਾਠ = arg;
	ਵਸਤੂ->where = where;
	ਵਸਤੂ->if_exists = if_exists;
	ਵਸਤੂ->if_missing = if_missing;
	list_ਜੋੜੋ_ਪੂਛ(&ਵਸਤੂ->list, trailers);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ parse_opt_parse(ਅਚਲ ਬਣਤਰ option *opt, ਅਚਲ ਅੱਖਰ *arg,
			   ਪੂਰਨ unset)
{
	ਬਣਤਰ process_trailer_options *v = opt->ਮੁੱਲ;
	v->only_trailers = 1;
	v->only_ਨਿਵੇਸ਼ = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ read_ਨਿਵੇਸ਼_file(ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *file)
{
	ਜੇ (file) {
		ਜੇ (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_ਸਤਰ(sb);
}

ਸਥਿਰ ਖਾਲੀ interpret_trailers(ਅਚਲ ਬਣਤਰ process_trailer_options *opts,
			       ਬਣਤਰ list_ਸਿਰ *new_trailer_ਸਿਰ,
			       ਅਚਲ ਅੱਖਰ *file)
{
	ਬਣਤਰ strbuf ਨਿਵੇਸ਼ = STRBUF_INIT;
	ਬਣਤਰ strbuf ਬਾਹਰਲਾ = STRBUF_INIT;
	ਬਣਤਰ tempfile *tempfile = ਨਿਰਾ;
	ਪੂਰਨ fd = 1;

	trailer_config_init();

	read_ਨਿਵੇਸ਼_file(&ਨਿਵੇਸ਼, file);

	ਜੇ (opts->ਵਿੱਚ_ਰੱਖੋ) {
		tempfile = trailer_create_ਵਿੱਚ_ਰੱਖੋ_tempfile(file);
		ਜੇ (!tempfile)
			die(ਨਿਰਾ);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_ਸਿਰ, &ਨਿਵੇਸ਼, &ਬਾਹਰਲਾ);

	ਜੇ (write_ਵਿੱਚ_ਭਰਿਆ(fd, ਬਾਹਰਲਾ.buf, ਬਾਹਰਲਾ.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	ਜੇ (opts->ਵਿੱਚ_ਰੱਖੋ)
		ਜੇ (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&ਨਿਵੇਸ਼);
	strbuf_release(&ਬਾਹਰਲਾ);
}

ਪੂਰਨ cmd_interpret_trailers(ਪੂਰਨ argc,
			   ਅਚਲ ਅੱਖਰ **argv,
			   ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			   ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਬਣਤਰ process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_ਸਿਰ(trailers);

	ਬਣਤਰ option options[] = {
		OPT_BOOL(0, "in-place", &opts.ਵਿੱਚ_ਰੱਖੋ, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_ਖਾਲੀ_ਥਾਵਾਂ, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_ਨਿਵੇਸ਼, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, ਨਿਰਾ, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_ਸਿਰਾ()
	};

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, options,
			     git_interpret_trailers_usage, 0);

	ਜੇ (opts.only_ਨਿਵੇਸ਼ && !list_ਖਾਲੀ_ਥਾਵਾਂ(&trailers))
		usage_ਸੁਨੇਹਾ_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	ਜੇ (argc) {
		ਪੂਰਨ i;
		ਲਈ (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (opts.ਵਿੱਚ_ਰੱਖੋ)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, ਨਿਰਾ);
	}

	new_trailers_clear(&trailers);

	ਵਾਪਸ 0;
}
