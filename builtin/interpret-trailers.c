/*
 * Builtin "git interpret-trailers"
 *
 * Copyright (c) 2013, 2014 Christian Couder <chriscool@tuxfamily.org>
 *
 */
#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "string-list.h"
#அந்தர்பாவ "tempfile.h"
#அந்தர்பாவ "trailer.h"
#அந்தர்பாவ "config.h"

நிலையான மாறிலி எழுத்து * மாறிலி git_interpret_trailers_usage[] = {
	N_("git interpret-trailers [--in-place] [--trim-empty]\n"
	   "                       [(--trailer (<key>|<key-alias>)[(=|:)<value>])...]\n"
	   "                       [--parse] [<file>...]"),
	வெற்று
};

நிலையான எண்ணிக்கை trailer_where where;
நிலையான எண்ணிக்கை trailer_if_exists if_exists;
நிலையான எண்ணிக்கை trailer_if_missing if_missing;

நிலையான முழு option_parse_where(மாறிலி கட்டமைப்பு option *opt,
			      மாறிலி எழுத்து *arg, முழு unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	திரும்பு trailer_set_where(opt->மதிப்பு, arg);
}

நிலையான முழு option_parse_if_exists(மாறிலி கட்டமைப்பு option *opt,
				  மாறிலி எழுத்து *arg, முழு unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	திரும்பு trailer_set_if_exists(opt->மதிப்பு, arg);
}

நிலையான முழு option_parse_if_missing(மாறிலி கட்டமைப்பு option *opt,
				   மாறிலி எழுத்து *arg, முழு unset UNUSED)
{
	/* unset implies NULL arg, which is handled in our helper */
	திரும்பு trailer_set_if_missing(opt->மதிப்பு, arg);
}

நிலையான வெறுமை new_trailers_clear(கட்டமைப்பு list_head *trailers)
{
	கட்டமைப்பு list_head *pos, *tmp;
	கட்டமைப்பு new_trailer_item *item;

	list_for_each_safe(pos, tmp, trailers) {
		item = list_entry(pos, கட்டமைப்பு new_trailer_item, list);
		list_del(pos);
		விடுவி(item);
	}
}

நிலையான முழு option_parse_trailer(மாறிலி கட்டமைப்பு option *opt,
				   மாறிலி எழுத்து *arg, முழு unset)
{
	கட்டமைப்பு list_head *trailers = opt->மதிப்பு;
	கட்டமைப்பு new_trailer_item *item;

	எனில் (unset) {
		new_trailers_clear(trailers);
		திரும்பு 0;
	}

	எனில் (!arg)
		திரும்பு -1;

	item = xmalloc(அளவு(*item));
	item->text = arg;
	item->where = where;
	item->if_exists = if_exists;
	item->if_missing = if_missing;
	list_add_tail(&item->list, trailers);
	திரும்பு 0;
}

நிலையான முழு parse_opt_parse(மாறிலி கட்டமைப்பு option *opt, மாறிலி எழுத்து *arg,
			   முழு unset)
{
	கட்டமைப்பு process_trailer_options *v = opt->மதிப்பு;
	v->only_trailers = 1;
	v->only_உள்ளீடு = 1;
	v->unfold = 1;
	BUG_ON_OPT_NEG(unset);
	BUG_ON_OPT_ARG(arg);
	திரும்பு 0;
}

நிலையான வெறுமை read_உள்ளீடு_file(கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *file)
{
	எனில் (file) {
		எனில் (strbuf_read_file(sb, file, 0) < 0)
			die_errno(_("could not read input file '%s'"), file);
	} இல்லையெனில் {
		எனில் (strbuf_read(sb, fileno(stdin), 0) < 0)
			die_errno(_("could not read from stdin"));
	}
	strbuf_complete_வரி(sb);
}

நிலையான வெறுமை interpret_trailers(மாறிலி கட்டமைப்பு process_trailer_options *opts,
			       கட்டமைப்பு list_head *new_trailer_head,
			       மாறிலி எழுத்து *file)
{
	கட்டமைப்பு strbuf உள்ளீடு = STRBUF_INIT;
	கட்டமைப்பு strbuf out = STRBUF_INIT;
	கட்டமைப்பு tempfile *tempfile = வெற்று;
	முழு fd = 1;

	trailer_config_init();

	read_உள்ளீடு_file(&உள்ளீடு, file);

	எனில் (opts->in_place) {
		tempfile = trailer_create_in_place_tempfile(file);
		எனில் (!tempfile)
			die(வெற்று);
		fd = tempfile->fd;
	}
	process_trailers(opts, new_trailer_head, &உள்ளீடு, &out);

	எனில் (write_in_full(fd, out.buf, out.len) < 0)
		die_errno(_("could not write to temporary file '%s'"), file);
	எனில் (opts->in_place)
		எனில் (rename_tempfile(&tempfile, file))
			die_errno(_("could not rename temporary file to %s"), file);

	strbuf_release(&உள்ளீடு);
	strbuf_release(&out);
}

முழு cmd_interpret_trailers(முழு argc,
			   மாறிலி எழுத்து **argv,
			   மாறிலி எழுத்து *prefix,
			   கட்டமைப்பு repository *repo UNUSED)
{
	கட்டமைப்பு process_trailer_options opts = PROCESS_TRAILER_OPTIONS_INIT;
	LIST_HEAD(trailers);

	கட்டமைப்பு option options[] = {
		OPT_BOOL(0, "in-place", &opts.in_place, N_("edit files in place")),
		OPT_BOOL(0, "trim-empty", &opts.trim_empty, N_("trim empty trailers")),

		OPT_CALLBACK(0, "where", &where, N_("placement"),
			     N_("where to place the new trailer"), option_parse_where),
		OPT_CALLBACK(0, "if-exists", &if_exists, N_("action"),
			     N_("action if trailer already exists"), option_parse_if_exists),
		OPT_CALLBACK(0, "if-missing", &if_missing, N_("action"),
			     N_("action if trailer is missing"), option_parse_if_missing),

		OPT_BOOL(0, "only-trailers", &opts.only_trailers, N_("output only the trailers")),
		OPT_BOOL(0, "only-input", &opts.only_உள்ளீடு, N_("do not apply trailer.<key-alias> configuration variables")),
		OPT_BOOL(0, "unfold", &opts.unfold, N_("reformat multiline trailer values as single-line values")),
		OPT_CALLBACK_F(0, "parse", &opts, வெற்று, N_("alias for --only-trailers --only-input --unfold"),
			PARSE_OPT_NOARG | PARSE_OPT_NONEG, parse_opt_parse),
		OPT_BOOL(0, "no-divider", &opts.no_divider, N_("do not treat \"---\" as the end of input")),
		OPT_CALLBACK(0, "trailer", &trailers, N_("trailer"),
				N_("trailer(s) to add"), option_parse_trailer),
		OPT_END()
	};

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, options,
			     git_interpret_trailers_usage, 0);

	எனில் (opts.only_உள்ளீடு && !list_empty(&trailers))
		usage_செய்தி_opt(
			_("--trailer with --only-input does not make sense"),
			git_interpret_trailers_usage,
			options);

	எனில் (argc) {
		முழு i;
		க்கு (i = 0; i < argc; i++)
			interpret_trailers(&opts, &trailers, argv[i]);
	} இல்லையெனில் {
		எனில் (opts.in_place)
			die(_("no input file given for in-place editing"));
		interpret_trailers(&opts, &trailers, வெற்று);
	}

	new_trailers_clear(&trailers);

	திரும்பு 0;
}
