/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#include "builtin.h"
#include "environment.h"
#include "gettext.h"
#include "parse-options.h"
#include "string-list.h"
#include "tempfile.h"
#include "trailer.h"
#include "config.h"

static const var_na * const git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	NULL
};

static ga_nanaa trailer_where where;
static ga_nanaa trailer_if_exists if_exists;
static ga_nanaa trailer_if_missing if_missing;

static puur_naa_nka option_parse_where(const sa_nracanaa option *opt,
			      const var_na *arg, puur_naa_nka unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	ᰀᰧᰌ trailer_set_where(opt->value, arg);
}

static puur_naa_nka option_parse_if_exists(const sa_nracanaa option *opt,
				  const var_na *arg, puur_naa_nka unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	ᰀᰧᰌ trailer_set_if_exists(opt->value, arg);
}

static puur_naa_nka option_parse_if_missing(const sa_nracanaa option *opt,
				   const var_na *arg, puur_naa_nka unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	ᰀᰧᰌ trailer_set_if_missing(opt->value, arg);
}

static void new_trailers_clear(sa_nracanaa list_head *trailers)
{
	sa_nracanaa list_head *pos, *tmp;
	sa_nracanaa new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, sa_nracanaa new_trailer_item, list);
		list_del(pos);
		free(item);
	}
}

static puur_naa_nka option_parse_trailer(const sa_nracanaa option *opt,
				   const var_na *arg, puur_naa_nka unset)
{
	sa_nracanaa list_head *trailers = opt->value;
	sa_nracanaa new_trailer_item *item;

	ᰀᰧ (unset) {
		new_trailers_clear(trailers);
		ᰀᰧᰌ 0;
	}

	ᰀᰧ (!arg)
		ᰀᰧᰌ -1;

	item = xmalloc(parimaa_na(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	ᰀᰧᰌ 0;
}

static puur_naa_nka parse_opt_parse(const sa_nracanaa option *opt, const var_na *arg,
			   puur_naa_nka unset)
{
	sa_nracanaa process_trailer_options *v = opt->value;
	v->only_trailers = 1;
	v->only_input = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	ᰀᰧᰌ 0;
}

static void read_input_file(sa_nracanaa strbuf *sb, const var_na *file)
{
	ᰀᰧ (file) {
		ᰀᰧ (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} ᰀᰧᰀ {
		ᰀᰧ (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_line(sb);
}

static void interpret_trailers(const sa_nracanaa process_trailer_options *opts,
			       sa_nracanaa list_head *new_trailer_head,
			       const var_na *file)
{
	sa_nracanaa strbuf input = STRBUF_INIT;
	sa_nracanaa strbuf out = STRBUF_INIT;
	sa_nracanaa tempfile *tempfile = NULL;
	puur_naa_nka fd = 1;

	trailer_config_init();

	read_input_file(&input, file);

	ᰀᰧ (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		ᰀᰧ (!tempfile)
			die(NULL);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &input, &out);

	ᰀᰧ (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	ᰀᰧ (opts->in_place)
		ᰀᰧ (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&input);
	strbuf_release(&out);
}

puur_naa_nka cmd_interpret_trailers(puur_naa_nka argc,
			   const var_na **argv,
			   const var_na *prefix,
			   sa_nracanaa repository *repo UNUSED)
{
	sa_nracanaa process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	sa_nracanaa option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_input, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, NULL, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, NULL);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	ᰀᰧ (opts.only_input && !list_empty(&trailers))
		usage_msg_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	ᰀᰧ (argc) {
		puur_naa_nka i;
		for (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} ᰀᰧᰀ {
		ᰀᰧ (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, NULL);
	}

	new_trailers_clear(&trailers);

	ᰀᰧᰌ 0;
}
