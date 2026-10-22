/*
 * rev-parse.c
 *
 * Copyright (C) Linus Torvalds, 2005
 */

#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#నిర్వచించు DISABLE_SIGN_COMPARE_WARNINGS

#అన్తర్భావ "builtin.h"

#అన్తర్భావ "abspath.h"
#అన్తర్భావ "bisect.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "commit.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "hex.h"
#అన్తర్భావ "refs.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "object-name.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "path.h"
#అన్తర్భావ "diff.h"
#అన్తర్భావ "read-cache-ll.h"
#అన్తర్భావ "repo-settings.h"
#అన్తర్భావ "repository.h"
#అన్తర్భావ "revision.h"
#అన్తర్భావ "setup.h"
#అన్తర్భావ "split-index.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "commit-reach.h"
#అన్తర్భావ "shallow.h"
#అన్తర్భావ "object-file-convert.h"

#నిర్వచించు DO_REVS		1
#నిర్వచించు DO_NOREV	2
#నిర్వచించు DO_FLAGS	4
#నిర్వచించు DO_NONFLAGS	8
స్థిర పూర్ణాంకం filter = ~0;

స్థిర స్థిరరాశి అక్షరం *def;

#నిర్వచించు సాధారణ 0
#నిర్వచించు REVERSED 1
స్థిర పూర్ణాంకం show_type = సాధారణ;

#నిర్వచించు SHOW_SYMBOLIC_ASIS 1
#నిర్వచించు SHOW_SYMBOLIC_నిండిన 2
స్థిర పూర్ణాంకం symbolic;
స్థిర పూర్ణాంకం abbrev;
స్థిర పూర్ణాంకం abbrev_ref;
స్థిర పూర్ణాంకం abbrev_ref_strict;
స్థిర పూర్ణాంకం output_sq;

స్థిర పూర్ణాంకం stuck_long;
స్థిర నిర్మాణం ref_exclusions ref_excludes = REF_EXCLUSIONS_INIT;

/*
 * Some arguments are relevant "revision" arguments,
 * others are about output format or other details.
 * This sorts it all out.
 */
స్థిర పూర్ణాంకం is_rev_వాదం(స్థిరరాశి అక్షరం *arg)
{
	స్థిర స్థిరరాశి అక్షరం *rev_వాదనలు[] = {
		"--all",
		"--bisect",
		"--dense",
		"--branches=",
		"--branches",
		"--header",
		"--ignore-missing",
		"--max-age=",
		"--max-count=",
		"--min-age=",
		"--no-merges",
		"--min-parents=",
		"--no-min-parents",
		"--max-parents=",
		"--no-max-parents",
		"--objects",
		"--objects-edge",
		"--parents",
		"--pretty",
		"--remotes=",
		"--remotes",
		"--glob=",
		"--sparse",
		"--tags=",
		"--tags",
		"--topo-order",
		"--date-order",
		"--unpacked",
		ఖాళీ
	};
	స్థిరరాశి అక్షరం **p = rev_వాదనలు;

	/* accept -<digit>, like traditional "head" */
	ఒకవేళ ((*arg == '-') && అంకెనా(arg[1]))
		తిరిగి 1;

	కొరకు (;;) {
		స్థిరరాశి అక్షరం *str = *p++;
		పూర్ణాంకం len;
		ఒకవేళ (!str)
			తిరిగి 0;
		len = అక్షరశ్రేణి_పొడవు(str);
		ఒకవేళ (!strcmp(arg, str) ||
		    (str[len-1] == '=' && !అక్షరశ్రేణి_పోలిక(arg, str, len)))
			తిరిగి 1;
	}
}

/* Output argument as a string, either SQ or normal */
స్థిర శూన్య show(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (output_sq) {
		పూర్ణాంకం sq = '\'', ch;

		అక్షరం_రాయి(sq);
		అయితే ((ch = *arg++)) {
			ఒకవేళ (ch == sq)
				fputs("'\\'", stdout);
			అక్షరం_రాయి(ch);
		}
		అక్షరం_రాయి(sq);
		అక్షరం_రాయి(' ');
	}
	లేకపోతే
		puts(arg);
}

/* Like show(), but with a negation prefix according to type */
స్థిర శూన్య show_with_type(పూర్ణాంకం type, స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (type != show_type)
		అక్షరం_రాయి('^');
	show(arg);
}

/* Output a revision, only if filter allows it */
స్థిర శూన్య show_rev(పూర్ణాంకం type, స్థిరరాశి నిర్మాణం object_id *oid, స్థిరరాశి అక్షరం *name)
{
	ఒకవేళ (!(filter & DO_REVS))
		తిరిగి;
	def = ఖాళీ;

	ఒకవేళ ((symbolic || abbrev_ref) && name) {
		ఒకవేళ (symbolic == SHOW_SYMBOLIC_నిండిన || abbrev_ref) {
			నిర్మాణం object_id discard;
			అక్షరం *నిండిన;

			ఎంపిక (repo_dwim_ref(the_repository, name,
					      అక్షరశ్రేణి_పొడవు(name), &discard, &నిండిన,
					      0)) {
			స్థితి 0:
				/*
				 * Not found -- not a ref.  We could
				 * emit "name" here, but symbolic-full
				 * users are interested in finding the
				 * refs spelled in full, and they would
				 * need to filter non-refs if we did so.
				 */
				ఆపు;
			స్థితి 1: /* happy */
				ఒకవేళ (abbrev_ref) {
					అక్షరం *old = నిండిన;
					నిండిన = refs_shorten_unambiguous_ref(get_main_ref_store(the_repository),
									    నిండిన,
									    abbrev_ref_strict);
					విడుదల(old);
				}
				show_with_type(type, నిండిన);
				ఆపు;
			అప్రమేయ: /* ambiguous */
				error("refname '%s' is ambiguous", name);
				ఆపు;
			}
			విడుదల(నిండిన);
		} లేకపోతే {
			show_with_type(type, name);
		}
	}
	లేకపోతే ఒకవేళ (abbrev)
		show_with_type(type,
			       repo_కనుగొను_unique_abbrev(the_repository, oid, abbrev));
	లేకపోతే
		show_with_type(type, oid_గమ్యం_hex(oid));
}

/* Output a flag, only if filter allows it. */
స్థిర పూర్ణాంకం show_flag(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ (!(filter & DO_FLAGS))
		తిరిగి 0;
	ఒకవేళ (filter & (is_rev_వాదం(arg) ? DO_REVS : DO_NOREV)) {
		show(arg);
		తిరిగి 1;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_default(శూన్య)
{
	స్థిరరాశి అక్షరం *s = def;

	ఒకవేళ (s) {
		నిర్మాణం object_id oid;

		def = ఖాళీ;
		ఒకవేళ (!repo_get_oid(the_repository, s, &oid)) {
			show_rev(సాధారణ, &oid, s);
			తిరిగి 1;
		}
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_reference(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	ఒకవేళ (ref_excluded(&ref_excludes, ref->name))
		తిరిగి 0;
	show_rev(సాధారణ, ref->oid, ref->name);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం anti_reference(స్థిరరాశి నిర్మాణం reference *ref, శూన్య *cb_దత్తాంశం వాడని)
{
	show_rev(REVERSED, ref->oid, ref->name);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం show_abbrev(స్థిరరాశి నిర్మాణం object_id *oid, శూన్య *cb_దత్తాంశం వాడని)
{
	show_rev(సాధారణ, oid, ఖాళీ);
	తిరిగి 0;
}

స్థిర శూన్య show_datestring(స్థిరరాశి అక్షరం *flag, స్థిరరాశి అక్షరం *datestr)
{
	అక్షరం *బఫరు;

	/* date handling requires both flags and revs */
	ఒకవేళ ((filter & (DO_FLAGS | DO_REVS)) != (DO_FLAGS | DO_REVS))
		తిరిగి;
	బఫరు = xstrfmt("%s%"PRItime, flag, approxidate(datestr));
	show(బఫరు);
	విడుదల(బఫరు);
}

స్థిర పూర్ణాంకం show_file(స్థిరరాశి అక్షరం *arg, పూర్ణాంకం output_ఉపసర్గ)
{
	show_default();
	ఒకవేళ ((filter & (DO_NONFLAGS|DO_NOREV)) == (DO_NONFLAGS|DO_NOREV)) {
		ఒకవేళ (output_ఉపసర్గ) {
			స్థిరరాశి అక్షరం *ఉపసర్గ = the_repository->ఉపసర్గ;
			అక్షరం *fname = ఉపసర్గ_filename(ఉపసర్గ, arg);
			show(fname);
			విడుదల(fname);
		} లేకపోతే
			show(arg);
		తిరిగి 1;
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం try_difference(స్థిరరాశి అక్షరం *arg)
{
	స్థిరరాశి అక్షరం *dotdot;
	నిర్మాణం object_id ప్రారంభం_oid;
	నిర్మాణం object_id చివర_oid;
	స్థిరరాశి అక్షరం *చివర;
	స్థిరరాశి అక్షరం *ప్రారంభం;
	అక్షరం *గమ్యం_free;
	పూర్ణాంకం సౌష్ఠవ;
	స్థిర స్థిరరాశి అక్షరం తల_by_default[] = "HEAD";

	ఒకవేళ (!(dotdot = అక్షరశ్రేణి_శోధన(arg, "..")))
		తిరిగి 0;
	ప్రారంభం = గమ్యం_free = xmemdupz(arg, dotdot - arg);
	చివర = dotdot + 2;
	సౌష్ఠవ = (*చివర == '.');
	చివర += సౌష్ఠవ;

	ఒకవేళ (!*చివర)
		చివర = తల_by_default;
	ఒకవేళ (dotdot == arg)
		ప్రారంభం = తల_by_default;

	ఒకవేళ (ప్రారంభం == తల_by_default && చివర == తల_by_default &&
	    !సౌష్ఠవ) {
		/*
		 * Just ".."?  That is not a range but the
		 * pathspec for the parent directory.
		 */
		విడుదల(గమ్యం_free);
		తిరిగి 0;
	}

	ఒకవేళ (!repo_get_oid_committish(the_repository, ప్రారంభం, &ప్రారంభం_oid) && !repo_get_oid_committish(the_repository, చివర, &చివర_oid)) {
		show_rev(సాధారణ, &చివర_oid, చివర);
		show_rev(సౌష్ఠవ ? సాధారణ : REVERSED, &ప్రారంభం_oid, ప్రారంభం);
		ఒకవేళ (సౌష్ఠవ) {
			నిర్మాణం commit_list *exclude = ఖాళీ;
			నిర్మాణం commit *a, *b;
			a = lookup_commit_reference(the_repository, &ప్రారంభం_oid);
			b = lookup_commit_reference(the_repository, &చివర_oid);
			ఒకవేళ (!a || !b) {
				విడుదల(గమ్యం_free);
				తిరిగి 0;
			}
			ఒకవేళ (repo_get_విలీనం_bases(the_repository, a, b, &exclude) < 0)
				నిష్క్రమించు(128);
			అయితే (exclude) {
				నిర్మాణం commit *commit = తీయి_commit(&exclude);
				show_rev(REVERSED, &commit->object.oid, ఖాళీ);
			}
		}
		విడుదల(గమ్యం_free);
		తిరిగి 1;
	}
	విడుదల(గమ్యం_free);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం try_మాతృ_shorthands(స్థిరరాశి అక్షరం *arg)
{
	స్థిరరాశి అక్షరం *mark;
	నిర్మాణం object_id oid;
	నిర్మాణం commit *commit;
	నిర్మాణం commit_list *parents;
	పూర్ణాంకం మాతృ_సంఖ్య;
	పూర్ణాంకం include_rev = 0;
	పూర్ణాంకం include_parents = 0;
	పూర్ణాంకం exclude_మాతృ = 0;
	అక్షరం *గమ్యం_free;

	ఒకవేళ ((mark = అక్షరశ్రేణి_శోధన(arg, "^!"))) {
		include_rev = 1;
		ఒకవేళ (mark[2])
			తిరిగి 0;
	} లేకపోతే ఒకవేళ ((mark = అక్షరశ్రేణి_శోధన(arg, "^@"))) {
		include_parents = 1;
		ఒకవేళ (mark[2])
			తిరిగి 0;
	} లేకపోతే ఒకవేళ ((mark = అక్షరశ్రేణి_శోధన(arg, "^-"))) {
		include_rev = 1;
		exclude_మాతృ = 1;

		ఒకవేళ (mark[2]) {
			అక్షరం *చివర;
			exclude_మాతృ = strtoul(mark + 2, &చివర, 10);
			ఒకవేళ (*చివర != '\0' || !exclude_మాతృ)
				తిరిగి 0;
		}
	} లేకపోతే
		తిరిగి 0;

	arg = గమ్యం_free = xmemdupz(arg, mark - arg);
	ఒకవేళ (repo_get_oid_committish(the_repository, arg, &oid) ||
	    !(commit = lookup_commit_reference(the_repository, &oid))) {
		విడుదల(గమ్యం_free);
		తిరిగి 0;
	}

	ఒకవేళ (exclude_మాతృ &&
	    exclude_మాతృ > commit_list_లెక్క(commit->parents)) {
		విడుదల(గమ్యం_free);
		తిరిగి 0;
	}

	ఒకవేళ (include_rev)
		show_rev(సాధారణ, &oid, arg);
	కొరకు (parents = commit->parents, మాతృ_సంఖ్య = 1;
	     parents;
	     parents = parents->తదుపరి, మాతృ_సంఖ్య++) {
		అక్షరం *name = ఖాళీ;

		ఒకవేళ (exclude_మాతృ && మాతృ_సంఖ్య != exclude_మాతృ)
			కొనసాగు;

		ఒకవేళ (symbolic)
			name = xstrfmt("%s^%d", arg, మాతృ_సంఖ్య);
		show_rev(include_parents ? సాధారణ : REVERSED,
			 &parents->అంశం->object.oid, name);
		విడుదల(name);
	}

	విడుదల(గమ్యం_free);
	తిరిగి 1;
}

స్థిర పూర్ణాంకం parseopt_dump(స్థిరరాశి నిర్మాణం option *o, స్థిరరాశి అక్షరం *arg, పూర్ణాంకం unset)
{
	నిర్మాణం strbuf *parsed = o->విలువ;
	ఒకవేళ (unset)
		strbuf_addf(parsed, " --no-%s", o->long_name);
	లేకపోతే ఒకవేళ (o->short_name && (o->long_name == ఖాళీ || !stuck_long))
		strbuf_addf(parsed, " -%c", o->short_name);
	లేకపోతే
		strbuf_addf(parsed, " --%s", o->long_name);
	ఒకవేళ (arg) {
		ఒకవేళ (!stuck_long)
			strbuf_addch(parsed, ' ');
		లేకపోతే ఒకవేళ (o->long_name)
			strbuf_addch(parsed, '=');
		sq_quote_buf(parsed, arg);
	}
	తిరిగి 0;
}

స్థిర స్థిరరాశి అక్షరం *skipspaces(స్థిరరాశి అక్షరం *s)
{
	అయితే (ఖాళీనా(*s))
		s++;
	తిరిగి s;
}

స్థిర అక్షరం *findspace(స్థిరరాశి అక్షరం *s)
{
	కొరకు (; *s; s++)
		ఒకవేళ (ఖాళీనా(*s))
			తిరిగి (అక్షరం*)s;
	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం cmd_parseopt(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	పూర్ణాంకం keep_dashdash = 0, stop_at_non_option = 0;
	అక్షరం స్థిరరాశి * స్థిరరాశి parseopt_usage[] = {
		N_("git rev-parse --parseopt [<options>] -- [<args>...]"),
		ఖాళీ
	};
	నిర్మాణం option parseopt_opts[] = {
		OPT_BOOL(0, "keep-dashdash", &keep_dashdash,
					N_("keep the `--` passed as an arg")),
		OPT_BOOL(0, "stop-at-non-option", &stop_at_non_option,
					N_("stop parsing after the "
					   "first non-option argument")),
		OPT_BOOL(0, "stuck-long", &stuck_long,
					N_("output in stuck long form")),
		OPT_చివర(),
	};
	నిర్మాణం strbuf sb = STRBUF_INIT, parsed = STRBUF_INIT;
	నిర్మాణం strvec longnames = STRVEC_INIT;
	నిర్మాణం strvec usage = STRVEC_INIT;
	నిర్మాణం option *opts = ఖాళీ;
	కొలత_t opts_nr = 0, opts_alloc = 0;

	strbuf_addstr(&parsed, "set --");
	argc = parse_options(argc, argv, ఉపసర్గ, parseopt_opts, parseopt_usage,
	                     PARSE_OPT_KEEP_DASHDASH);
	ఒకవేళ (argc < 1 || strcmp(argv[0], "--"))
		usage_with_options(parseopt_usage, parseopt_opts);

	/* get the usage up to the first line with a -- on it */
	కొరకు (;;) {
		strbuf_reset(&sb);
		ఒకవేళ (strbuf_getline(&sb, stdin) == EOF)
			die(_("premature end of input"));
		ఒకవేళ (!strcmp("--", sb.buf)) {
			ఒకవేళ (!usage.nr)
				die(_("no usage string given before the `--' separator"));
			ఆపు;
		}

		strvec_నెట్టు(&usage, sb.buf);
	}

	/* parse: (<short>|<short>,<long>|<long>)[*=?!]*<arghint>? SP+ <help> */
	అయితే (strbuf_getline(&sb, stdin) != EOF) {
		స్థిరరాశి అక్షరం *s;
		అక్షరం *help;
		నిర్మాణం option *o;

		ఒకవేళ (!sb.len)
			కొనసాగు;

		ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
		memset(opts + opts_nr, 0, పరిమాణం(*opts));

		o = &opts[opts_nr++];
		help = findspace(sb.buf);
		ఒకవేళ (!help || sb.buf == help) {
			o->type = OPTION_సమూహం;
			o->help = xstrdup(skipspaces(sb.buf));
			కొనసాగు;
		}

		*help = '\0';

		o->type = OPTION_CALLBACK;
		o->help = xstrdup(skipspaces(help+1));
		o->విలువ = &parsed;
		o->flags = PARSE_OPT_NOARG;
		o->callback = &parseopt_dump;

		/* name(s) */
		s = strpbrk(sb.buf, "*=?!");
		ఒకవేళ (!s)
			s = help;

		ఒకవేళ (s == sb.buf)
			die(_("missing opt-spec before option flags"));

		ఒకవేళ (s - sb.buf == 1) { /* short option only */
			o->short_name = *sb.buf;
		} లేకపోతే ఒకవేళ (sb.buf[1] != ',') { /* long option only */
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (పూర్ణాంకం)(s - sb.buf), sb.buf);
		} లేకపోతే {
			o->short_name = *sb.buf;
			o->long_name = strvec_pushf(&longnames, "%.*s",
						    (పూర్ణాంకం)(s - sb.buf - 2), sb.buf + 2);
		}

		/* flags */
		అయితే (s < help) {
			ఎంపిక (*s++) {
			స్థితి '=':
				o->flags &= ~PARSE_OPT_NOARG;
				కొనసాగు;
			స్థితి '?':
				o->flags &= ~PARSE_OPT_NOARG;
				o->flags |= PARSE_OPT_OPTARG;
				కొనసాగు;
			స్థితి '!':
				o->flags |= PARSE_OPT_NONEG;
				కొనసాగు;
			స్థితి '*':
				o->flags |= PARSE_OPT_HIDDEN;
				కొనసాగు;
			}
			s--;
			ఆపు;
		}

		ఒకవేళ (s < help)
			o->argh = xmemdupz(s, help - s);
	}
	strbuf_release(&sb);

	/* put an OPT_END() */
	ALLOC_GROW(opts, opts_nr + 1, opts_alloc);
	memset(opts + opts_nr, 0, పరిమాణం(*opts));
	argc = parse_options(argc, argv, ఉపసర్గ, opts, usage.v,
			(keep_dashdash ? PARSE_OPT_KEEP_DASHDASH : 0) |
			(stop_at_non_option ? PARSE_OPT_STOP_AT_NON_OPTION : 0) |
			PARSE_OPT_SHELL_EVAL);

	strbuf_addstr(&parsed, " --");
	sq_quote_argv(&parsed, argv);
	puts(parsed.buf);

	strbuf_release(&parsed);
	strbuf_release(&sb);
	strvec_clear(&longnames);
	strvec_clear(&usage);
	కొరకు (కొలత_t i = 0; i < opts_nr; i++) {
		విడుదల((అక్షరం *) opts[i].help);
		విడుదల((అక్షరం *) opts[i].argh);
	}
	విడుదల(opts);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cmd_sq_quote(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;

	ఒకవేళ (argc)
		sq_quote_argv(&buf, argv);
	రాయి("%s\n", buf.buf);
	strbuf_release(&buf);

	తిరిగి 0;
}

స్థిర శూన్య die_no_single_rev(పూర్ణాంకం quiet)
{
	ఒకవేళ (quiet)
		నిష్క్రమించు(1);
	లేకపోతే
		die(_("Needed a single revision"));
}

స్థిర స్థిరరాశి అక్షరం builtin_rev_parse_usage[] =
N_("git rev-parse --parseopt [<options>] -- [<args>...]\n"
   "   or: git rev-parse --sq-quote [<arg>...]\n"
   "   or: git rev-parse [<options>] [<arg>...]\n"
   "\n"
   "Run \"git rev-parse --parseopt -h\" for more information on the first usage.");

/*
 * Parse "opt" or "opt=<value>", setting value respectively to either
 * NULL or the string after "=".
 */
స్థిర పూర్ణాంకం opt_with_విలువ(స్థిరరాశి అక్షరం *arg, స్థిరరాశి అక్షరం *opt, స్థిరరాశి అక్షరం **విలువ)
{
	ఒకవేళ (skip_ఉపసర్గ(arg, opt, &arg)) {
		ఒకవేళ (!*arg) {
			*విలువ = ఖాళీ;
			తిరిగి 1;
		}
		ఒకవేళ (*arg++ == '=') {
			*విలువ = arg;
			తిరిగి 1;
		}
	}
	తిరిగి 0;
}

స్థిర శూన్య handle_ref_opt(స్థిరరాశి అక్షరం *నమూనా, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	ఒకవేళ (నమూనా) {
		నిర్మాణం refs_for_each_ref_options opts = {
			.నమూనా = నమూనా,
			.ఉపసర్గ = ఉపసర్గ,
			.trim_ఉపసర్గ = ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0,
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ఖాళీ, &opts);
	} లేకపోతే {
		నిర్మాణం refs_for_each_ref_options opts = {
			.ఉపసర్గ = ఉపసర్గ,
			.trim_ఉపసర్గ = అక్షరశ్రేణి_పొడవు(ఉపసర్గ),
		};
		refs_for_each_ref_ext(get_main_ref_store(the_repository),
				      show_reference, ఖాళీ, &opts);
	}
	clear_ref_exclusions(&ref_excludes);
}

గణన format_type {
	/* We would like a relative path. */
	FORMAT_RELATIVE,
	/* We would like a canonical absolute path. */
	FORMAT_CANONICAL,
	/* We would like the default behavior. */
	FORMAT_DEFAULT,
};

గణన default_type {
	/* Our default is a relative path. */
	DEFAULT_RELATIVE,
	/* Our default is a relative path if there's a shared root. */
	DEFAULT_RELATIVE_IF_SHARED,
	/* Our default is a canonical absolute path. */
	DEFAULT_CANONICAL,
	/* Our default is not to modify the item. */
	DEFAULT_UNMODIFIED,
};

స్థిర శూన్య print_మార్గం(స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *ఉపసర్గ,
		       గణన format_type format, గణన default_type def)
{
	నిర్మాణం strbuf sb = STRBUF_INIT;
	గణన మార్గం_format fmt;

	ఒకవేళ (format == FORMAT_DEFAULT) {
		ఎంపిక (def) {
		స్థితి DEFAULT_RELATIVE:
			fmt = మార్గం_FORMAT_RELATIVE;
			ఆపు;
		స్థితి DEFAULT_RELATIVE_IF_SHARED:
			fmt = మార్గం_FORMAT_RELATIVE_IF_SHARED;
			ఆపు;
		స్థితి DEFAULT_CANONICAL:
			fmt = మార్గం_FORMAT_CANONICAL;
			ఆపు;
		స్థితి DEFAULT_UNMODIFIED:
		అప్రమేయ:
			fmt = మార్గం_FORMAT_UNMODIFIED;
			ఆపు;
		}
	} లేకపోతే {
		ఎంపిక (format) {
		స్థితి FORMAT_RELATIVE:
			fmt = మార్గం_FORMAT_RELATIVE;
			ఆపు;
		స్థితి FORMAT_CANONICAL:
			fmt = మార్గం_FORMAT_CANONICAL;
			ఆపు;
		అప్రమేయ:
			fmt = మార్గం_FORMAT_UNMODIFIED;
			ఆపు;
		}
	}

	format_మార్గం(&sb, మార్గం, ఉపసర్గ, fmt);
	puts(sb.buf);

	strbuf_release(&sb);
}

పూర్ణాంకం cmd_rev_parse(పూర్ణాంకం argc,
		  స్థిరరాశి అక్షరం **argv,
		  స్థిరరాశి అక్షరం *ఉపసర్గ,
		  నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం i, as_is = 0, verify = 0, quiet = 0, revs_లెక్క = 0, type = 0;
	స్థిరరాశి నిర్మాణం git_hash_algo *output_algo = ఖాళీ;
	స్థిరరాశి నిర్మాణం git_hash_algo *compat = ఖాళీ;
	పూర్ణాంకం did_repo_అమరిక = 0;
	పూర్ణాంకం has_dashdash = 0;
	పూర్ణాంకం output_ఉపసర్గ = 0;
	నిర్మాణం object_id oid;
	గుర్తులేని పూర్ణాంకం flags = 0;
	స్థిరరాశి అక్షరం *name = ఖాళీ;
	నిర్మాణం strbuf buf = STRBUF_INIT;
	పూర్ణాంకం చూసిన_చివర_యొక్క_options = 0;
	గణన format_type format = FORMAT_DEFAULT;

	show_usage_if_asked(argc, argv, builtin_rev_parse_usage);

	ఒకవేళ (argc > 1 && !strcmp("--parseopt", argv[1]))
		తిరిగి cmd_parseopt(argc - 1, argv + 1, ఉపసర్గ);

	ఒకవేళ (argc > 1 && !strcmp("--sq-quote", argv[1]))
		తిరిగి cmd_sq_quote(argc - 2, argv + 2);

	ఒకవేళ (argc > 1 && !strcmp("-h", argv[1]))
		usage(builtin_rev_parse_usage);

	కొరకు (i = 1; i < argc; i++) {
		ఒకవేళ (!strcmp(argv[i], "--")) {
			has_dashdash = 1;
			ఆపు;
		}
	}

	/* No options; just report on whether we're in a git repo or not. */
	ఒకవేళ (argc == 1) {
		అమరిక_git_directory(the_repository);
		repo_config(the_repository, git_default_config, ఖాళీ);
		తిరిగి 0;
	}

	కొరకు (i = 1; i < argc; i++) {
		స్థిరరాశి అక్షరం *arg = argv[i];

		ఒకవేళ (as_is) {
			ఒకవేళ (show_file(arg, output_ఉపసర్గ) && as_is < 2)
				verify_filename(the_repository, ఉపసర్గ, arg, 0);
			కొనసాగు;
		}

		ఒకవేళ (!చూసిన_చివర_యొక్క_options) {
			ఒకవేళ (!strcmp(arg, "--local-env-vars")) {
				పూర్ణాంకం i;
				కొరకు (i = 0; local_repo_env[i]; i++)
					రాయి("%s\n", local_repo_env[i]);
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--resolve-git-dir")) {
				స్థిరరాశి అక్షరం *gitdir = argv[++i];
				ఒకవేళ (!gitdir)
					die(_("--resolve-git-dir requires an argument"));
				gitdir = resolve_gitdir(gitdir);
				ఒకవేళ (!gitdir)
					die(_("not a gitdir '%s'"), argv[i]);
				puts(gitdir);
				కొనసాగు;
			}
		}

		/* The rest of the options require a git repository. */
		ఒకవేళ (!did_repo_అమరిక) {
			ఉపసర్గ = అమరిక_git_directory(the_repository);
			repo_config(the_repository, git_default_config, ఖాళీ);
			did_repo_అమరిక = 1;

			prepare_repo_settings(the_repository);
			the_repository->settings.command_requires_నిండిన_సూచీ = 0;
			compat = the_repository->compat_hash_algo;
		}

		ఒకవేళ (!strcmp(arg, "--")) {
			as_is = 2;
			/* Pass on the "--" if we show anything but files.. */
			ఒకవేళ (filter & (DO_FLAGS | DO_REVS))
				show_file(arg, 0);
			కొనసాగు;
		}

		ఒకవేళ (!చూసిన_చివర_యొక్క_options && *arg == '-') {
			ఒకవేళ (!strcmp(arg, "--git-path")) {
				ఒకవేళ (!argv[i + 1])
					die(_("--git-path requires an argument"));
				print_మార్గం(repo_git_మార్గం_replace(the_repository, &buf,
								 "%s", argv[i + 1]), ఉపసర్గ,
						format,
						DEFAULT_RELATIVE_IF_SHARED);
				i++;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg,"-n")) {
				ఒకవేళ (++i >= argc)
					die(_("-n requires an argument"));
				ఒకవేళ ((filter & DO_FLAGS) && (filter & DO_REVS)) {
					show(arg);
					show(argv[i]);
				}
				కొనసాగు;
			}
			ఒకవేళ (starts_with(arg, "-n")) {
				ఒకవేళ ((filter & DO_FLAGS) && (filter & DO_REVS))
					show(arg);
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--path-format", &arg)) {
				ఒకవేళ (!arg)
					die(_("--path-format requires an argument"));
				ఒకవేళ (!strcmp(arg, "absolute")) {
					format = FORMAT_CANONICAL;
				} లేకపోతే ఒకవేళ (!strcmp(arg, "relative")) {
					format = FORMAT_RELATIVE;
				} లేకపోతే {
					die(_("unknown argument to --path-format: %s"), arg);
				}
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--default")) {
				def = argv[++i];
				ఒకవేళ (!def)
					die(_("--default requires an argument"));
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--prefix")) {
				ఉపసర్గ = argv[++i];
				ఒకవేళ (!ఉపసర్గ)
					die(_("--prefix requires an argument"));
				FREE_AND_NULL(the_repository->ఉపసర్గ);
				the_repository->ఉపసర్గ = xstrdup(ఉపసర్గ);
				output_ఉపసర్గ = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--revs-only")) {
				filter &= ~DO_NOREV;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--no-revs")) {
				filter &= ~DO_REVS;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--flags")) {
				filter &= ~DO_NONFLAGS;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--no-flags")) {
				filter &= ~DO_FLAGS;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--verify")) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--quiet") || !strcmp(arg, "-q")) {
				quiet = 1;
				flags |= GET_OID_QUIETLY;
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--output-object-format", &arg)) {
				ఒకవేళ (!arg)
					die(_("no object format specified"));
				ఒకవేళ (!strcmp(arg, the_hash_algo->name) ||
				    !strcmp(arg, "storage")) {
					flags |= GET_OID_HASH_ANY;
					output_algo = the_hash_algo;
					కొనసాగు;
				}
				లేకపోతే ఒకవేళ (compat && !strcmp(arg, compat->name)) {
					flags |= GET_OID_HASH_ANY;
					output_algo = compat;
					కొనసాగు;
				}
				లేకపోతే die(_("unsupported object format: %s"), arg);
			}
			ఒకవేళ (opt_with_విలువ(arg, "--short", &arg)) {
				filter &= ~(DO_FLAGS|DO_NOREV);
				verify = 1;
				abbrev = DEFAULT_ABBREV;
				ఒకవేళ (!arg)
					కొనసాగు;
				abbrev = strtoul(arg, ఖాళీ, 10);
				ఒకవేళ (abbrev < MINIMUM_ABBREV)
					abbrev = MINIMUM_ABBREV;
				లేకపోతే ఒకవేళ ((పూర్ణాంకం)the_hash_algo->hexsz <= abbrev)
					abbrev = the_hash_algo->hexsz;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--sq")) {
				output_sq = 1;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--not")) {
				show_type ^= REVERSED;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--symbolic")) {
				symbolic = SHOW_SYMBOLIC_ASIS;
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--symbolic-full-name")) {
				symbolic = SHOW_SYMBOLIC_నిండిన;
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--abbrev-ref", &arg)) {
				abbrev_ref = 1;
				abbrev_ref_strict =
					repo_settings_get_warn_ambiguous_refs(the_repository);
				ఒకవేళ (arg) {
					ఒకవేళ (!strcmp(arg, "strict"))
						abbrev_ref_strict = 1;
					లేకపోతే ఒకవేళ (!strcmp(arg, "loose"))
						abbrev_ref_strict = 0;
					లేకపోతే
						die(_("unknown mode for --abbrev-ref: %s"),
						    arg);
				}
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--all")) {
				refs_for_each_ref(get_main_ref_store(the_repository),
						  show_reference, ఖాళీ);
				clear_ref_exclusions(&ref_excludes);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--disambiguate=", &arg)) {
				repo_for_each_abbrev(the_repository, arg, the_hash_algo,
						     show_abbrev, ఖాళీ);
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--bisect")) {
				అక్షరం *ఉపసర్గ;
				అక్షరం *పదం_bad = ఖాళీ;
				అక్షరం *పదం_good = ఖాళీ;
				నిర్మాణం refs_for_each_ref_options opts = { 0 };
				read_bisect_terms(&పదం_bad, &పదం_good);
				ఉపసర్గ = xstrfmt("refs/bisect/%s", పదం_bad);
				opts.ఉపసర్గ = ఉపసర్గ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      show_reference, ఖాళీ, &opts);
				విడుదల(ఉపసర్గ);
				ఉపసర్గ = xstrfmt("refs/bisect/%s", పదం_good);
				opts.ఉపసర్గ = ఉపసర్గ;
				refs_for_each_ref_ext(get_main_ref_store(the_repository),
						      anti_reference, ఖాళీ, &opts);
				విడుదల(ఉపసర్గ);
				విడుదల(పదం_good);
				విడుదల(పదం_bad);
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--branches", &arg)) {
				ఒకవేళ (ref_excludes.hidden_refs_configured)
					తిరిగి error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--branches");
				handle_ref_opt(arg, "refs/heads/");
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--tags", &arg)) {
				ఒకవేళ (ref_excludes.hidden_refs_configured)
					తిరిగి error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--tags");
				handle_ref_opt(arg, "refs/tags/");
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--glob=", &arg)) {
				handle_ref_opt(arg, ఖాళీ);
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--remotes", &arg)) {
				ఒకవేళ (ref_excludes.hidden_refs_configured)
					తిరిగి error(_("options '%s' and '%s' cannot be used together"),
						     "--exclude-hidden", "--remotes");
				handle_ref_opt(arg, "refs/remotes/");
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--exclude=", &arg)) {
				కూడు_ref_exclusion(&ref_excludes, arg);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--exclude-hidden=", &arg)) {
				exclude_hidden_refs(&ref_excludes, arg);
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--show-toplevel")) {
				స్థిరరాశి అక్షరం *work_వృక్షం = repo_get_work_వృక్షం(the_repository);
				ఒకవేళ (work_వృక్షం)
					print_మార్గం(work_వృక్షం, ఉపసర్గ, format, DEFAULT_UNMODIFIED);
				లేకపోతే
					die(_("this operation must be run in a work tree"));
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--show-superproject-working-tree")) {
				నిర్మాణం strbuf superproject = STRBUF_INIT;
				ఒకవేళ (get_superproject_working_వృక్షం(&superproject))
					print_మార్గం(superproject.buf, ఉపసర్గ, format, DEFAULT_UNMODIFIED);
				strbuf_release(&superproject);
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--show-prefix")) {
				ఒకవేళ (ఉపసర్గ)
					puts(ఉపసర్గ);
				లేకపోతే
					అక్షరం_రాయి('\n');
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--show-cdup")) {
				స్థిరరాశి అక్షరం *pfx = ఉపసర్గ;
				ఒకవేళ (!is_inside_work_వృక్షం(the_repository)) {
					స్థిరరాశి అక్షరం *work_వృక్షం =
						repo_get_work_వృక్షం(the_repository);
					ఒకవేళ (work_వృక్షం)
						రాయి("%s\n", work_వృక్షం);
					కొనసాగు;
				}
				అయితే (pfx) {
					pfx = strchr(pfx, '/');
					ఒకవేళ (pfx) {
						pfx++;
						రాయి("../");
					}
				}
				అక్షరం_రాయి('\n');
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--git-dir") ||
			    !strcmp(arg, "--absolute-git-dir")) {
				స్థిరరాశి అక్షరం *gitdir = getenv(GIT_DIR_ENVIRONMENT);
				అక్షరం *cwd;
				పూర్ణాంకం len;
				గణన format_type wanted = format;
				ఒకవేళ (arg[2] == 'g') {	/* --git-dir */
					ఒకవేళ (gitdir) {
						print_మార్గం(gitdir, ఉపసర్గ, format, DEFAULT_UNMODIFIED);
						కొనసాగు;
					}
					ఒకవేళ (!ఉపసర్గ) {
						print_మార్గం(".git", ఉపసర్గ, format, DEFAULT_UNMODIFIED);
						కొనసాగు;
					}
				} లేకపోతే {		/* --absolute-git-dir */
					wanted = FORMAT_CANONICAL;
					ఒకవేళ (!gitdir && !ఉపసర్గ)
						gitdir = ".git";
					ఒకవేళ (gitdir) {
						నిర్మాణం strbuf realpath = STRBUF_INIT;
						strbuf_realpath(&realpath, gitdir, 1);
						puts(realpath.buf);
						strbuf_release(&realpath);
						కొనసాగు;
					}
				}
				cwd = xgetcwd();
				len = అక్షరశ్రేణి_పొడవు(cwd);
				strbuf_reset(&buf);
				strbuf_addf(&buf, "%s%s.git", cwd, len && cwd[len-1] != '/' ? "/" : "");
				విడుదల(cwd);
				print_మార్గం(buf.buf, ఉపసర్గ, wanted, DEFAULT_CANONICAL);
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--git-common-dir")) {
				print_మార్గం(repo_get_common_dir(the_repository), ఉపసర్గ, format, DEFAULT_RELATIVE_IF_SHARED);
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--is-inside-git-dir")) {
				రాయి("%s\n", is_inside_git_dir(the_repository) ? "true"
						: "false");
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--is-inside-work-tree")) {
				రాయి("%s\n", is_inside_work_వృక్షం(the_repository) ? "true"
						: "false");
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--is-bare-repository")) {
				రాయి("%s\n", is_bare_repository(the_repository) ? "true"
						: "false");
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--is-shallow-repository")) {
				రాయి("%s\n",
						is_repository_shallow(the_repository) ? "true"
						: "false");
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--shared-index-path")) {
				ఒకవేళ (repo_read_సూచీ(the_repository) < 0)
					die(_("Could not read the index"));
				ఒకవేళ (the_repository->సూచీ->విభజించు_సూచీ) {
					స్థిరరాశి నిర్మాణం object_id *oid = &the_repository->సూచీ->విభజించు_సూచీ->పునాది_oid;
					స్థిరరాశి అక్షరం *మార్గం = repo_git_మార్గం_replace(the_repository, &buf, "sharedindex.%s", oid_గమ్యం_hex(oid));
					print_మార్గం(మార్గం, ఉపసర్గ, format, DEFAULT_RELATIVE);
				}
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--since=", &arg)) {
				show_datestring("--max-age=", arg);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--after=", &arg)) {
				show_datestring("--max-age=", arg);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--before=", &arg)) {
				show_datestring("--min-age=", arg);
				కొనసాగు;
			}
			ఒకవేళ (skip_ఉపసర్గ(arg, "--until=", &arg)) {
				show_datestring("--min-age=", arg);
				కొనసాగు;
			}
			ఒకవేళ (opt_with_విలువ(arg, "--show-object-format", &arg)) {
				స్థిరరాశి అక్షరం *val = arg ? arg : "storage";

				ఒకవేళ (strcmp(val, "storage") &&
				    strcmp(val, "compat") &&
				    strcmp(val, "input") &&
				    strcmp(val, "output"))
					die(_("unknown mode for --show-object-format: %s"),
					    arg);

				ఒకవేళ (!strcmp(val, "compat")) {
					ఒకవేళ (the_repository->compat_hash_algo)
						puts(the_repository->compat_hash_algo->name);
					లేకపోతే
						అక్షరం_రాయి('\n');
				} లేకపోతే {
					puts(the_hash_algo->name);
				}
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--show-ref-format") || !strcmp(arg, "--show-ref-storage-format")) {
				puts(ref_storage_format_గమ్యం_name(the_repository->ref_storage_format));
				కొనసాగు;
			}
			ఒకవేళ (!strcmp(arg, "--end-of-options")) {
				చూసిన_చివర_యొక్క_options = 1;
				ఒకవేళ (filter & (DO_FLAGS | DO_REVS))
					show_file(arg, 0);
				కొనసాగు;
			}
			ఒకవేళ (show_flag(arg) && verify)
				die_no_single_rev(quiet);
			కొనసాగు;
		}

		/* Not a flag argument */
		ఒకవేళ (try_difference(arg))
			కొనసాగు;
		ఒకవేళ (try_మాతృ_shorthands(arg))
			కొనసాగు;
		name = arg;
		type = సాధారణ;
		ఒకవేళ (*arg == '^') {
			name++;
			type = REVERSED;
		}
		ఒకవేళ (!repo_get_oid_with_flags(the_repository, name, &oid,
					     flags)) {
			ఒకవేళ (output_algo)
				repo_oid_గమ్యం_algop(the_repository, &oid,
						  output_algo, &oid);
			ఒకవేళ (verify)
				revs_లెక్క++;
			లేకపోతే
				show_rev(type, &oid, name);
			కొనసాగు;
		}
		ఒకవేళ (verify)
			die_no_single_rev(quiet);
		ఒకవేళ (has_dashdash)
			die(_("bad revision '%s'"), arg);
		as_is = 1;
		ఒకవేళ (!show_file(arg, output_ఉపసర్గ))
			కొనసాగు;
		verify_filename(the_repository, ఉపసర్గ, arg, 1);
	}
	strbuf_release(&buf);
	ఒకవేళ (verify) {
		ఒకవేళ (revs_లెక్క == 1) {
			show_rev(type, &oid, name);
			తిరిగి 0;
		} లేకపోతే ఒకవేళ (revs_లెక్క == 0 && show_default())
			తిరిగి 0;
		die_no_single_rev(quiet);
	} లేకపోతే
		show_default();
	తిరిగి 0;
}
