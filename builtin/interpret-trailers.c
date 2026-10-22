/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#અન્તર્ભાવ "builtin.h"
#અન્તર્ભાવ "environment.h"
#અન્તર્ભાવ "gettext.h"
#અન્તર્ભાવ "parse-options.h"
#અન્તર્ભાવ "string-list.h"
#અન્તર્ભાવ "tempfile.h"
#અન્તર્ભાવ "trailer.h"
#અન્તર્ભાવ "config.h"

સ્થિર અચલ અક્ષર * અચલ git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	ખાલી
};

સ્થિર ગણના trailer_where where;
સ્થિર ગણના trailer_if_exists if_exists;
સ્થિર ગણના trailer_if_missing if_missing;

સ્થિર પૂર્ણાંક option_parse_where(અચલ સંરચના option *opt,
			      અચલ અક્ષર *arg, પૂર્ણાંક unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	પાછા trailer_set_where(opt->કિંમત, arg);
}

સ્થિર પૂર્ણાંક option_parse_if_exists(અચલ સંરચના option *opt,
				  અચલ અક્ષર *arg, પૂર્ણાંક unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	પાછા trailer_set_if_exists(opt->કિંમત, arg);
}

સ્થિર પૂર્ણાંક option_parse_if_missing(અચલ સંરચના option *opt,
				   અચલ અક્ષર *arg, પૂર્ણાંક unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	પાછા trailer_set_if_missing(opt->કિંમત, arg);
}

સ્થિર શૂન્ય new_trailers_clear(સંરચના list_head *trailers)
{
	સંરચના list_head *pos, *tmp;
	સંરચના new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, સંરચના new_trailer_item, list);
		list_del(pos);
		મુક્ત(item);
	}
}

સ્થિર પૂર્ણાંક option_parse_trailer(અચલ સંરચના option *opt,
				   અચલ અક્ષર *arg, પૂર્ણાંક unset)
{
	સંરચના list_head *trailers = opt->કિંમત;
	સંરચના new_trailer_item *item;

	જો (unset) {
		new_trailers_clear(trailers);
		પાછા 0;
	}

	જો (!arg)
		પાછા -1;

	item = xmalloc(કદ(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	પાછા 0;
}

સ્થિર પૂર્ણાંક parse_opt_parse(અચલ સંરચના option *opt, અચલ અક્ષર *arg,
			   પૂર્ણાંક unset)
{
	સંરચના process_trailer_options *v = opt->કિંમત;
	v->only_trailers = 1;
	v->only_આગત = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	પાછા 0;
}

સ્થિર શૂન્ય read_આગત_file(સંરચના strbuf *sb, અચલ અક્ષર *file)
{
	જો (file) {
		જો (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} નહીંતર {
		જો (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_લીટી(sb);
}

સ્થિર શૂન્ય interpret_trailers(અચલ સંરચના process_trailer_options *opts,
			       સંરચના list_head *new_trailer_head,
			       અચલ અક્ષર *file)
{
	સંરચના strbuf આગત = STRBUF_INIT;
	સંરચના strbuf out = STRBUF_INIT;
	સંરચના tempfile *tempfile = ખાલી;
	પૂર્ણાંક fd = 1;

	trailer_config_init();

	read_આગત_file(&આગત, file);

	જો (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		જો (!tempfile)
			die(ખાલી);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &આગત, &out);

	જો (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	જો (opts->in_place)
		જો (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&આગત);
	strbuf_release(&out);
}

પૂર્ણાંક cmd_interpret_trailers(પૂર્ણાંક argc,
			   અચલ અક્ષર **argv,
			   અચલ અક્ષર *prefix,
			   સંરચના repository *repo UNUSED)
{
	સંરચના process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	સંરચના option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_આગત, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, ખાલી, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, ખાલી);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	જો (opts.only_આગત && !list_empty(&trailers))
		usage_સંદેશ_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	જો (argc) {
		પૂર્ણાંક i;
		માટે (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} નહીંતર {
		જો (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, ખાલી);
	}

	new_trailers_clear(&trailers);

	પાછા 0;
}
