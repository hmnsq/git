/*
 * GIT - The information manager from hell
 *
 * Copyright (C) Linus Torvalds, 2005
 */
#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"

#అన్తర్భావ "config.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "odb.h"
#అన్తర్భావ "tree.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "pathspec.h"

స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి ls_వృక్షం_usage[] = {
	N_("git ls-tree [<options>] <tree-ish> [<path>...]"),
	ఖాళీ
};

స్థిర శూన్య expand_objectsize(నిర్మాణం strbuf *పంక్తి, స్థిరరాశి నిర్మాణం object_id *oid,
			      స్థిరరాశి గణన object_type type, గుర్తులేని పూర్ణాంకం padded)
{
	స్థిర స్థిరరాశి అక్షరం padding[] = "       ";
	కొలత_t min_len = padded ? అక్షరశ్రేణి_పొడవు(padding) : 0;
	కొలత_t orig_len = పంక్తి->len;
	కొలత_t len;

	ఒకవేళ (type == OBJ_BLOB) {
		కొలత_t కొలత;
		ఒకవేళ (odb_read_object_info(the_repository->objects, oid, &కొలత) < 0)
			die(_("could not get object info about '%s'"),
			    oid_గమ్యం_hex(oid));
		strbuf_కూడు_uint(పంక్తి, కొలత);
	} లేకపోతే {
		strbuf_addstr(పంక్తి, "-");
	}
	len = పంక్తి->len - orig_len;
	ఒకవేళ (len < min_len)
		strbuf_చొప్పించు(పంక్తి, orig_len, padding, min_len - len);
}

నిర్మాణం ls_వృక్షం_options {
	గుర్తులేని null_termination:1;
	పూర్ణాంకం abbrev;
	గణన ls_వృక్షం_మార్గం_options {
		LS_RECURSIVE = 1 << 0,
		LS_వృక్షం_ONLY = 1 << 1,
		LS_SHOW_TREES = 1 << 2,
	} ls_options;
	నిర్మాణం pathspec pathspec;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	స్థిరరాశి అక్షరం *format;
};

స్థిర పూర్ణాంకం show_recursive(నిర్మాణం ls_వృక్షం_options *options, స్థిరరాశి అక్షరం *పునాది,
			  కొలత_t baselen, స్థిరరాశి అక్షరం *pathname)
{
	పూర్ణాంకం i;

	ఒకవేళ (options->ls_options & LS_RECURSIVE)
		తిరిగి 1;

	ఒకవేళ (!options->pathspec.nr)
		తిరిగి 0;

	కొరకు (i = 0; i < options->pathspec.nr; i++) {
		స్థిరరాశి అక్షరం *spec = options->pathspec.వస్తువులు[i].సరిపోలిక;
		కొలత_t len, speclen;

		ఒకవేళ (అక్షరశ్రేణి_పోలిక(పునాది, spec, baselen))
			కొనసాగు;
		len = అక్షరశ్రేణి_పొడవు(pathname);
		spec += baselen;
		speclen = అక్షరశ్రేణి_పొడవు(spec);
		ఒకవేళ (speclen <= len)
			కొనసాగు;
		ఒకవేళ (spec[len] && spec[len] != '/')
			కొనసాగు;
		ఒకవేళ (memcmp(pathname, spec, len))
			కొనసాగు;
		తిరిగి 1;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_వృక్షం_fmt(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *పునాది,
			 స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం, శూన్య *context)
{
	నిర్మాణం ls_వృక్షం_options *options = context;
	పూర్ణాంకం recurse = 0;
	నిర్మాణం strbuf sb = STRBUF_INIT;
	గణన object_type type = object_type(బహుళకం);
	స్థిరరాశి అక్షరం *format = options->format;

	ఒకవేళ (type == OBJ_వృక్షం && show_recursive(options, పునాది->buf, పునాది->len, pathname))
		recurse = READ_వృక్షం_RECURSIVE;
	ఒకవేళ (type == OBJ_వృక్షం && recurse && !(options->ls_options & LS_SHOW_TREES))
		తిరిగి recurse;
	ఒకవేళ (type == OBJ_BLOB && (options->ls_options & LS_వృక్షం_ONLY))
		తిరిగి 0;

	అయితే (strbuf_expand_అడుగు(&sb, &format)) {
		కొలత_t len;

		ఒకవేళ (skip_ఉపసర్గ(format, "%", &format))
			strbuf_addch(&sb, '%');
		లేకపోతే ఒకవేళ ((len = strbuf_expand_literal(&sb, format)))
			format += len;
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectmode)", &format))
			strbuf_addf(&sb, "%06o", బహుళకం);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objecttype)", &format))
			strbuf_addstr(&sb, type_name(type));
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectsize:padded)", &format))
			expand_objectsize(&sb, oid, type, 1);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectsize)", &format))
			expand_objectsize(&sb, oid, type, 0);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(objectname)", &format))
			strbuf_కూడు_unique_abbrev(&sb, oid, options->abbrev);
		లేకపోతే ఒకవేళ (skip_ఉపసర్గ(format, "(path)", &format)) {
			స్థిరరాశి అక్షరం *name;
			స్థిరరాశి అక్షరం *ఉపసర్గ = options->ఉపసర్గ;
			నిర్మాణం strbuf sbuf = STRBUF_INIT;
			కొలత_t baselen = పునాది->len;

			strbuf_addstr(పునాది, pathname);
			name = relative_మార్గం(పునాది->buf, ఉపసర్గ, &sbuf);
			quote_c_style(name, &sb, ఖాళీ, 0);
			strbuf_setlen(పునాది, baselen);
			strbuf_release(&sbuf);
		} లేకపోతే
			strbuf_expand_bad_format(format, "ls-tree");
	}
	strbuf_addch(&sb, options->null_termination ? '\0' : '\n');
	fwrite(sb.buf, sb.len, 1, stdout);
	strbuf_release(&sb);
	తిరిగి recurse;
}

స్థిర పూర్ణాంకం show_వృక్షం_common(నిర్మాణం ls_వృక్షం_options *options, పూర్ణాంకం *recurse,
			    నిర్మాణం strbuf *పునాది, స్థిరరాశి అక్షరం *pathname,
			    గణన object_type type)
{
	పూర్ణాంకం ret = -1;
	*recurse = 0;

	ఒకవేళ (type == OBJ_BLOB) {
		ఒకవేళ (options->ls_options & LS_వృక్షం_ONLY)
			ret = 0;
	} లేకపోతే ఒకవేళ (type == OBJ_వృక్షం &&
		   show_recursive(options, పునాది->buf, పునాది->len, pathname)) {
		*recurse = READ_వృక్షం_RECURSIVE;
		ఒకవేళ (!(options->ls_options & LS_SHOW_TREES))
			ret = *recurse;
	}

	తిరిగి ret;
}

స్థిర శూన్య show_వృక్షం_common_default_long(నిర్మాణం ls_వృక్షం_options *options,
					  నిర్మాణం strbuf *పునాది,
					  స్థిరరాశి అక్షరం *pathname,
					  స్థిరరాశి కొలత_t baselen)
{
	స్థిరరాశి అక్షరం *ఉపసర్గ = options->ఉపసర్గ;

	strbuf_addstr(పునాది, pathname);

	ఒకవేళ (options->null_termination) {
		నిర్మాణం strbuf sb = STRBUF_INIT;
		స్థిరరాశి అక్షరం *name = relative_మార్గం(పునాది->buf, ఉపసర్గ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} లేకపోతే {
		write_name_quoted_relative(పునాది->buf, ఉపసర్గ, stdout, '\n');
	}

	strbuf_setlen(పునాది, baselen);
}

స్థిర పూర్ణాంకం show_వృక్షం_default(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *పునాది,
			     స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం,
			     శూన్య *context)
{
	నిర్మాణం ls_వృక్షం_options *options = context;
	పూర్ణాంకం early;
	పూర్ణాంకం recurse;
	గణన object_type type = object_type(బహుళకం);

	early = show_వృక్షం_common(options, &recurse, పునాది, pathname, type);
	ఒకవేళ (early >= 0)
		తిరిగి early;

	రాయి("%06o %s %s\t", బహుళకం, type_name(object_type(బహుళకం)),
	       repo_కనుగొను_unique_abbrev(the_repository, oid, options->abbrev));
	show_వృక్షం_common_default_long(options, పునాది, pathname, పునాది->len);
	తిరిగి recurse;
}

స్థిర పూర్ణాంకం show_వృక్షం_long(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *పునాది,
			  స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం,
			  శూన్య *context)
{
	నిర్మాణం ls_వృక్షం_options *options = context;
	పూర్ణాంకం early;
	పూర్ణాంకం recurse;
	అక్షరం కొలత_పాఠ్యం[24];
	గణన object_type type = object_type(బహుళకం);

	early = show_వృక్షం_common(options, &recurse, పునాది, pathname, type);
	ఒకవేళ (early >= 0)
		తిరిగి early;

	ఒకవేళ (type == OBJ_BLOB) {
		కొలత_t కొలత;
		ఒకవేళ (odb_read_object_info(the_repository->objects, oid, &కొలత) == OBJ_BAD)
			xsnprintf(కొలత_పాఠ్యం, పరిమాణం(కొలత_పాఠ్యం), "BAD");
		లేకపోతే
			xsnprintf(కొలత_పాఠ్యం, పరిమాణం(కొలత_పాఠ్యం),
				  "%" PRIuMAX, (uintmax_t)కొలత);
	} లేకపోతే {
		xsnprintf(కొలత_పాఠ్యం, పరిమాణం(కొలత_పాఠ్యం), "-");
	}

	రాయి("%06o %s %s %7s\t", బహుళకం, type_name(type),
	       repo_కనుగొను_unique_abbrev(the_repository, oid, options->abbrev),
	       కొలత_పాఠ్యం);
	show_వృక్షం_common_default_long(options, పునాది, pathname, పునాది->len);
	తిరిగి recurse;
}

స్థిర పూర్ణాంకం show_వృక్షం_name_only(స్థిరరాశి నిర్మాణం object_id *oid వాడని,
			       నిర్మాణం strbuf *పునాది,
			       స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం,
			       శూన్య *context)
{
	నిర్మాణం ls_వృక్షం_options *options = context;
	పూర్ణాంకం early;
	పూర్ణాంకం recurse;
	స్థిరరాశి కొలత_t baselen = పునాది->len;
	గణన object_type type = object_type(బహుళకం);
	స్థిరరాశి అక్షరం *ఉపసర్గ;

	early = show_వృక్షం_common(options, &recurse, పునాది, pathname, type);
	ఒకవేళ (early >= 0)
		తిరిగి early;

	ఉపసర్గ = options->ఉపసర్గ;
	strbuf_addstr(పునాది, pathname);
	ఒకవేళ (options->null_termination) {
		నిర్మాణం strbuf sb = STRBUF_INIT;
		స్థిరరాశి అక్షరం *name = relative_మార్గం(పునాది->buf, ఉపసర్గ, &sb);

		fputs(name, stdout);
		fputc('\0', stdout);

		strbuf_release(&sb);
	} లేకపోతే {
		write_name_quoted_relative(పునాది->buf, ఉపసర్గ, stdout, '\n');
	}
	strbuf_setlen(పునాది, baselen);
	తిరిగి recurse;
}

స్థిర పూర్ణాంకం show_వృక్షం_object(స్థిరరాశి నిర్మాణం object_id *oid, నిర్మాణం strbuf *పునాది,
			    స్థిరరాశి అక్షరం *pathname, గుర్తులేని బహుళకం,
			    శూన్య *context)
{
	నిర్మాణం ls_వృక్షం_options *options = context;
	పూర్ణాంకం early;
	పూర్ణాంకం recurse;
	గణన object_type type = object_type(బహుళకం);
	స్థిరరాశి అక్షరం *str;

	early = show_వృక్షం_common(options, &recurse, పునాది, pathname, type);
	ఒకవేళ (early >= 0)
		తిరిగి early;

	str = repo_కనుగొను_unique_abbrev(the_repository, oid, options->abbrev);
	ఒకవేళ (options->null_termination) {
		fputs(str, stdout);
		fputc('\0', stdout);
	} లేకపోతే  {
		puts(str);
	}
	తిరిగి recurse;
}

గణన ls_వృక్షం_cmdmode {
	బహుళకం_DEFAULT = 0,
	బహుళకం_LONG,
	బహుళకం_NAME_ONLY,
	బహుళకం_NAME_స్థితి,
	బహుళకం_OBJECT_ONLY,
};

నిర్మాణం ls_వృక్షం_cmdmode_గమ్యం_fmt {
	గణన ls_వృక్షం_cmdmode బహుళకం;
	స్థిరరాశి అక్షరం *స్థిరరాశి fmt;
	read_వృక్షం_fn_t fn;
};

స్థిర నిర్మాణం ls_వృక్షం_cmdmode_గమ్యం_fmt ls_వృక్షం_cmdmode_format[] = {
	{
		.బహుళకం = బహుళకం_DEFAULT,
		.fmt = "%(objectmode) %(objecttype) %(objectname)%x09%(path)",
		.fn = show_వృక్షం_default,
	},
	{
		.బహుళకం = బహుళకం_LONG,
		.fmt = "%(objectmode) %(objecttype) %(objectname) %(objectsize:padded)%x09%(path)",
		.fn = show_వృక్షం_long,
	},
	{
		.బహుళకం = బహుళకం_NAME_ONLY, /* And MODE_NAME_STATUS */
		.fmt = "%(path)",
		.fn = show_వృక్షం_name_only,
	},
	{
		.బహుళకం = బహుళకం_OBJECT_ONLY,
		.fmt = "%(objectname)",
		.fn = show_వృక్షం_object
	},
	{
		/* fallback */
		.fn = show_వృక్షం_default,
	},
};

పూర్ణాంకం cmd_ls_వృక్షం(పూర్ణాంకం argc,
		స్థిరరాశి అక్షరం **argv,
		స్థిరరాశి అక్షరం *ఉపసర్గ,
		నిర్మాణం repository *repo వాడని)
{
	నిర్మాణం object_id oid;
	నిర్మాణం వృక్షం *వృక్షం;
	పూర్ణాంకం i, నిండిన_వృక్షం = 0;
	పూర్ణాంకం నిండిన_name = !ఉపసర్గ || !*ఉపసర్గ;
	read_వృక్షం_fn_t fn = ఖాళీ;
	గణన ls_వృక్షం_cmdmode cmdmode = బహుళకం_DEFAULT;
	పూర్ణాంకం null_termination = 0;
	నిర్మాణం ls_వృక్షం_options options = { 0 };
	స్థిరరాశి నిర్మాణం option ls_వృక్షం_options[] = {
		OPT_బిట్టు('d', ఖాళీ, &options.ls_options, N_("only show trees"),
			LS_వృక్షం_ONLY),
		OPT_బిట్టు('r', ఖాళీ, &options.ls_options, N_("recurse into subtrees"),
			LS_RECURSIVE),
		OPT_బిట్టు('t', ఖాళీ, &options.ls_options, N_("show trees when recursing"),
			LS_SHOW_TREES),
		OPT_BOOL('z', ఖాళీ, &null_termination,
			 N_("terminate entries with NUL byte")),
		OPT_CMDMODE('l', "long", &cmdmode, N_("include object size"),
			    బహుళకం_LONG),
		OPT_CMDMODE(0, "name-only", &cmdmode, N_("list only filenames"),
			    బహుళకం_NAME_ONLY),
		OPT_CMDMODE(0, "name-status", &cmdmode, N_("list only filenames"),
			    బహుళకం_NAME_స్థితి),
		OPT_CMDMODE(0, "object-only", &cmdmode, N_("list only objects"),
			    బహుళకం_OBJECT_ONLY),
		OPT_BOOL(0, "full-name", &నిండిన_name, N_("use full path names")),
		OPT_BOOL(0, "full-tree", &నిండిన_వృక్షం,
			 N_("list entire tree; not just current directory "
			    "(implies --full-name)")),
		OPT_STRING_F(0, "format", &options.format, N_("format"),
					 N_("format to use for the output"),
					 PARSE_OPT_NONEG),
		OPT__ABBREV(&options.abbrev),
		OPT_చివర()
	};
	నిర్మాణం ls_వృక్షం_cmdmode_గమ్యం_fmt *m2f = ls_వృక్షం_cmdmode_format;
	పూర్ణాంకం ret;

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, ls_వృక్షం_options,
			     ls_వృక్షం_usage, 0);
	options.null_termination = null_termination;

	ఒకవేళ (నిండిన_వృక్షం)
		ఉపసర్గ = ఖాళీ;
	options.ఉపసర్గ = నిండిన_name ? ఖాళీ : ఉపసర్గ;

	/*
	 * We wanted to detect conflicts between --name-only and
	 * --name-status, but once we're done with that subsequent
	 * code should only need to check the primary name.
	 */
	ఒకవేళ (cmdmode == బహుళకం_NAME_స్థితి)
		cmdmode = బహుళకం_NAME_ONLY;

	/* -d -r should imply -t, but -d by itself should not have to. */
	ఒకవేళ ( (LS_వృక్షం_ONLY|LS_RECURSIVE) ==
	    ((LS_వృక్షం_ONLY|LS_RECURSIVE) & options.ls_options))
		options.ls_options |= LS_SHOW_TREES;

	ఒకవేళ (options.format && cmdmode)
		usage_సందేశం_opt(
			_("--format can't be combined with other format-altering options"),
			ls_వృక్షం_usage, ls_వృక్షం_options);
	ఒకవేళ (argc < 1)
		usage_with_options(ls_వృక్షం_usage, ls_వృక్షం_options);
	ఒకవేళ (repo_get_oid_with_flags(the_repository, argv[0], &oid,
				    GET_OID_HASH_ANY))
		die("Not a valid object name %s", argv[0]);

	/*
	 * show_recursive() rolls its own matching code and is
	 * generally ignorant of 'struct pathspec'. The magic mask
	 * cannot be lifted until it is converted to use
	 * match_pathspec() or tree_entry_interesting()
	 */
	parse_pathspec(&options.pathspec, PATHSPEC_ALL_MAGIC &
		       ~(PATHSPEC_FROMTOP | PATHSPEC_LITERAL),
		       PATHSPEC_PREFER_CWD,
		       ఉపసర్గ, argv + 1);
	కొరకు (i = 0; i < options.pathspec.nr; i++)
		options.pathspec.వస్తువులు[i].nowildcard_len = options.pathspec.వస్తువులు[i].len;
	options.pathspec.has_wildcard = 0;
	వృక్షం = repo_parse_వృక్షం_పరోక్ష(the_repository, &oid);
	ఒకవేళ (!వృక్షం)
		die("not a tree object");
	/*
	 * The generic show_tree_fmt() is slower than show_tree(), so
	 * take the fast path if possible.
	 */
	అయితే (m2f) {
		ఒకవేళ (!m2f->fmt) {
			fn = options.format ? show_వృక్షం_fmt : show_వృక్షం_default;
		} లేకపోతే ఒకవేళ (options.format && !strcmp(options.format, m2f->fmt)) {
			cmdmode = m2f->బహుళకం;
			fn = m2f->fn;
		} లేకపోతే ఒకవేళ (!options.format && cmdmode == m2f->బహుళకం) {
			fn = m2f->fn;
		} లేకపోతే {
			m2f++;
			కొనసాగు;
		}
		ఆపు;
	}

	ret = !!read_వృక్షం(the_repository, వృక్షం, &options.pathspec, fn, &options);
	clear_pathspec(&options.pathspec);
	తిరిగి ret;
}
