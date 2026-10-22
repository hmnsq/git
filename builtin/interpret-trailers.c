/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "string-list.h"
#അന്തര്ഭാവ "tempfile.h"
#അന്തര്ഭാവ "trailer.h"
#അന്തര്ഭാവ "config.h"

സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	ശൂന്യ
};

സ്ഥിതം എണ്ണൽ trailer_where where;
സ്ഥിതം എണ്ണൽ trailer_if_exists if_exists;
സ്ഥിതം എണ്ണൽ trailer_if_missing if_missing;

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_where(സ്ഥിരം ഘടന option *opt,
			      സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	തിരികെ trailer_set_where(opt->മൂല്യം, arg);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_if_exists(സ്ഥിരം ഘടന option *opt,
				  സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	തിരികെ trailer_set_if_exists(opt->മൂല്യം, arg);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_if_missing(സ്ഥിരം ഘടന option *opt,
				   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	തിരികെ trailer_set_if_missing(opt->മൂല്യം, arg);
}

സ്ഥിതം ശൂന്യം new_trailers_clear(ഘടന list_head *trailers)
{
	ഘടന list_head *pos, *tmp;
	ഘടന new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, ഘടന new_trailer_item, list);
		list_del(pos);
		സ്വതന്ത്രം(item);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ option_parse_trailer(സ്ഥിരം ഘടന option *opt,
				   സ്ഥിരം അക്ഷരം *arg, പൂർണ്ണസംഖ്യ unset)
{
	ഘടന list_head *trailers = opt->മൂല്യം;
	ഘടന new_trailer_item *item;

	എങ്കിൽ (unset) {
		new_trailers_clear(trailers);
		തിരികെ 0;
	}

	എങ്കിൽ (!arg)
		തിരികെ -1;

	item = xmalloc(വലുപ്പം(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ parse_opt_parse(സ്ഥിരം ഘടന option *opt, സ്ഥിരം അക്ഷരം *arg,
			   പൂർണ്ണസംഖ്യ unset)
{
	ഘടന process_trailer_options *v = opt->മൂല്യം;
	v->only_trailers = 1;
	v->only_നിവേശം = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം read_നിവേശം_file(ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *file)
{
	എങ്കിൽ (file) {
		എങ്കിൽ (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_വരി(sb);
}

സ്ഥിതം ശൂന്യം interpret_trailers(സ്ഥിരം ഘടന process_trailer_options *opts,
			       ഘടന list_head *new_trailer_head,
			       സ്ഥിരം അക്ഷരം *file)
{
	ഘടന strbuf നിവേശം = STRBUF_INIT;
	ഘടന strbuf out = STRBUF_INIT;
	ഘടന tempfile *tempfile = ശൂന്യ;
	പൂർണ്ണസംഖ്യ fd = 1;

	trailer_config_init();

	read_നിവേശം_file(&നിവേശം, file);

	എങ്കിൽ (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		എങ്കിൽ (!tempfile)
			die(ശൂന്യ);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &നിവേശം, &out);

	എങ്കിൽ (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	എങ്കിൽ (opts->in_place)
		എങ്കിൽ (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&നിവേശം);
	strbuf_release(&out);
}

പൂർണ്ണസംഖ്യ cmd_interpret_trailers(പൂർണ്ണസംഖ്യ argc,
			   സ്ഥിരം അക്ഷരം **argv,
			   സ്ഥിരം അക്ഷരം *prefix,
			   ഘടന repository *repo UNUSED)
{
	ഘടന process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	ഘടന option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_നിവേശം, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, ശൂന്യ, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	എങ്കിൽ (opts.only_നിവേശം && !list_empty(&trailers))
		usage_സന്ദേശം_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	എങ്കിൽ (argc) {
		പൂർണ്ണസംഖ്യ i;
		വേണ്ടി (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, ശൂന്യ);
	}

	new_trailers_clear(&trailers);

	തിരികെ 0;
}
