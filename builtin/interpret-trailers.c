/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "string-list.h"
#ಅನ್ತರ್ಭಾವ "tempfile.h"
#ಅನ್ತರ್ಭಾವ "trailer.h"
#ಅನ್ತರ್ಭಾವ "config.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಎಣಿಕೆ trailer_where where;
ಸ್ಥಿರ ಎಣಿಕೆ trailer_if_exists if_exists;
ಸ್ಥಿರ ಎಣಿಕೆ trailer_if_missing if_missing;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_where(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset ಬಳಸದ)
{
	/* unset implies NULL arg, which is handled in our helper */
	ಹಿಂತಿರುಗಿ trailer_ಗಣ_where(opt->ಮೌಲ್ಯ, arg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_if_exists(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset ಬಳಸದ)
{
	/* unset implies NULL arg, which is handled in our helper */
	ಹಿಂತಿರುಗಿ trailer_ಗಣ_if_exists(opt->ಮೌಲ್ಯ, arg);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_if_missing(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset ಬಳಸದ)
{
	/* unset implies NULL arg, which is handled in our helper */
	ಹಿಂತಿರುಗಿ trailer_ಗಣ_if_missing(opt->ಮೌಲ್ಯ, arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ new_trailers_clear(ರಚನೆ list_ತಲೆ *trailers)
{
	ರಚನೆ list_ತಲೆ *pos, *tmp;
	ರಚನೆ new_trailer_ಅಂಶ *ಅಂಶ;

	list_for_each_ಸುರಕ್ಷಿತ(pos, tmp, trailers) {
		ಅಂಶ = list_ನಮೂದು(pos, ರಚನೆ new_trailer_ಅಂಶ, list);
		list_del(pos);
		ಬಿಡುಗಡೆ(ಅಂಶ);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_trailer(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ list_ತಲೆ *trailers = opt->ಮೌಲ್ಯ;
	ರಚನೆ new_trailer_ಅಂಶ *ಅಂಶ;

	ಆದರೆ (unset) {
		new_trailers_clear(trailers);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!arg)
		ಹಿಂತಿರುಗಿ -1;

	ಅಂಶ = xmalloc(ಗಾತ್ರ(*ಅಂಶ));
	ಅಂಶ->ಪಠ್ಯ = arg;
	ಅಂಶ->where = where;
	ಅಂಶ->if_exists = if_exists;
	ಅಂಶ->if_missing = if_missing;
	list_ಕೂಡಿಸು_ಬಾಲ(&ಅಂಶ->list, trailers);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ parse_opt_parse(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg,
			   ಪೂರ್ಣಾಂಕ unset)
{
	ರಚನೆ process_trailer_options *v = opt->ಮೌಲ್ಯ;
	v->only_trailers = 1;
	v->only_ಒಳಹರಿವು = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ read_ಒಳಹರಿವು_file(ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file)
{
	ಆದರೆ (file) {
		ಆದರೆ (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_ಸಾಲು(sb);
}

ಸ್ಥಿರ ಶೂನ್ಯ interpret_trailers(ಸ್ಥಿರಾಂಕ ರಚನೆ process_trailer_options *opts,
			       ರಚನೆ list_ತಲೆ *new_trailer_ತಲೆ,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file)
{
	ರಚನೆ strbuf ಒಳಹರಿವು = STRBUF_INIT;
	ರಚನೆ strbuf ಹೊರಹೋಗು = STRBUF_INIT;
	ರಚನೆ tempfile *tempfile = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ fd = 1;

	trailer_config_init();

	read_ಒಳಹರಿವು_file(&ಒಳಹರಿವು, file);

	ಆದರೆ (opts->ಒಳ_ಇರಿಸು) {
		tempfile = trailer_create_ಒಳ_ಇರಿಸು_tempfile(file);
		ಆದರೆ (!tempfile)
			die(ಖಾಲಿ);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_ತಲೆ, &ಒಳಹರಿವು, &ಹೊರಹೋಗು);

	ಆದರೆ (write_ಒಳ_ತುಂಬಿದ(fd, ಹೊರಹೋಗು.buf, ಹೊರಹೋಗು.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	ಆದರೆ (opts->ಒಳ_ಇರಿಸು)
		ಆದರೆ (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&ಒಳಹರಿವು);
	strbuf_release(&ಹೊರಹೋಗು);
}

ಪೂರ್ಣಾಂಕ cmd_interpret_trailers(ಪೂರ್ಣಾಂಕ argc,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			   ರಚನೆ repository *repo ಬಳಸದ)
{
	ರಚನೆ process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_ತಲೆ(trailers);

	ರಚನೆ option options[] = {
		OPT_BOOL(0, "in-place", &opts.ಒಳ_ಇರಿಸು, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_ಖಾಲಿಗಳು, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_ಒಳಹರಿವು, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, ಖಾಲಿ, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_ಕೊನೆ()
	};

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, options,
			     git_interpret_trailers_usage, 0);

	ಆದರೆ (opts.only_ಒಳಹರಿವು && !list_ಖಾಲಿಗಳು(&trailers))
		usage_ಸಂದೇಶ_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	ಆದರೆ (argc) {
		ಪೂರ್ಣಾಂಕ i;
		ಗಾಗಿ (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (opts.ಒಳ_ಇರಿಸು)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, ಖಾಲಿ);
	}

	new_trailers_clear(&trailers);

	ಹಿಂತಿರುಗಿ 0;
}
