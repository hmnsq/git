#నిర్వచించు USE_THE_REPOSITORY_VARIABLE
#అన్తర్భావ "builtin.h"
#అన్తర్భావ "config.h"
#అన్తర్భావ "dir.h"
#అన్తర్భావ "environment.h"
#అన్తర్భావ "gettext.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "pathspec.h"
#అన్తర్భావ "parse-options.h"
#అన్తర్భావ "submodule.h"
#అన్తర్భావ "write-or-die.h"

స్థిర పూర్ణాంకం quiet, verbose, stdin_paths, show_non_matching, no_సూచీ;
స్థిర స్థిరరాశి అక్షరం * స్థిరరాశి check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ఖాళీ
};

స్థిర పూర్ణాంకం nul_పదం_పంక్తి;

స్థిర స్థిరరాశి నిర్మాణం option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_సమూహం(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ఖాళీ, &nul_పదం_పంక్తి,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_సూచీ,
		 N_("ignore index when checking")),
	OPT_చివర()
};

స్థిర శూన్య output_నమూనా(స్థిరరాశి అక్షరం *మార్గం, నిర్మాణం మార్గం_నమూనా *నమూనా)
{
	స్థిరరాశి అక్షరం *bang  = (నమూనా && నమూనా->flags & నమూనా_FLAG_NEGATIVE)  ? "!" : "";
	స్థిరరాశి అక్షరం *slash = (నమూనా && నమూనా->flags & నమూనా_FLAG_MUSTBEDIR) ? "/" : "";
	ఒకవేళ (!nul_పదం_పంక్తి) {
		ఒకవేళ (!verbose) {
			write_name_quoted(మార్గం, stdout, '\n');
		} లేకపోతే {
			ఒకవేళ (నమూనా) {
				quote_c_style(నమూనా->pl->src, ఖాళీ, stdout, 0);
				రాయి(":%d:%s%s%s\t",
				       నమూనా->srcpos,
				       bang, నమూనా->నమూనా, slash);
			}
			లేకపోతే {
				రాయి("::\t");
			}
			quote_c_style(మార్గం, ఖాళీ, stdout, 0);
			fputc('\n', stdout);
		}
	} లేకపోతే {
		ఒకవేళ (!verbose) {
			రాయి("%s%c", మార్గం, '\0');
		} లేకపోతే {
			ఒకవేళ (నమూనా)
				రాయి("%s%c%d%c%s%s%s%c%s%c",
				       నమూనా->pl->src, '\0',
				       నమూనా->srcpos, '\0',
				       bang, నమూనా->నమూనా, slash, '\0',
				       మార్గం, '\0');
			లేకపోతే
				రాయి("%c%c%c%s%c", '\0', '\0', '\0', మార్గం, '\0');
		}
	}
}

స్థిర పూర్ణాంకం check_ignore(నిర్మాణం dir_struct *dir,
			స్థిరరాశి అక్షరం *ఉపసర్గ, పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
	స్థిరరాశి అక్షరం *నిండిన_మార్గం;
	అక్షరం *చూసిన;
	పూర్ణాంకం అంకె_ignored = 0, i;
	నిర్మాణం మార్గం_నమూనా *నమూనా;
	నిర్మాణం pathspec pathspec;

	ఒకవేళ (!argc) {
		ఒకవేళ (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		తిరిగి 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_మార్గం |
		       PATHSPEC_KEEP_క్రమం,
		       ఉపసర్గ, argv);

	die_మార్గం_inside_submodule(the_repository->సూచీ, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	చూసిన = కనుగొను_pathspecs_matching_against_సూచీ(&pathspec, the_repository->సూచీ,
						     PS_HEED_SKIP_WORKTREE);
	కొరకు (i = 0; i < pathspec.nr; i++) {
		నిండిన_మార్గం = pathspec.వస్తువులు[i].సరిపోలిక;
		నమూనా = ఖాళీ;
		ఒకవేళ (!చూసిన[i]) {
			పూర్ణాంకం dtype = DT_UNKNOWN;
			నమూనా = చివరి_matching_నమూనా(dir, the_repository->సూచీ,
							నిండిన_మార్గం, &dtype);
			ఒకవేళ (!verbose && నమూనా &&
			    నమూనా->flags & నమూనా_FLAG_NEGATIVE)
				నమూనా = ఖాళీ;
		}
		ఒకవేళ (!quiet && (నమూనా || show_non_matching))
			output_నమూనా(pathspec.వస్తువులు[i].original, నమూనా);
		ఒకవేళ (నమూనా)
			అంకె_ignored++;
	}
	విడుదల(చూసిన);
	clear_pathspec(&pathspec);

	తిరిగి అంకె_ignored;
}

స్థిర పూర్ణాంకం check_ignore_stdin_paths(నిర్మాణం dir_struct *dir, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
	నిర్మాణం strbuf buf = STRBUF_INIT;
	నిర్మాణం strbuf unquoted = STRBUF_INIT;
	అక్షరం *pathspec[2] = { ఖాళీ, ఖాళీ };
	strbuf_getline_fn getline_fn;
	పూర్ణాంకం అంకె_ignored = 0;

	getline_fn = nul_పదం_పంక్తి ? strbuf_getline_nul : strbuf_getline_lf;
	అయితే (getline_fn(&buf, stdin) != EOF) {
		ఒకవేళ (!nul_పదం_పంక్తి && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ఒకవేళ (unquote_c_style(&unquoted, buf.buf, ఖాళీ))
				die("line is badly quoted");
			strbuf_మార్పిడి(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		అంకె_ignored += check_ignore(dir, ఉపసర్గ,
					    1, (స్థిరరాశి అక్షరం **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	తిరిగి అంకె_ignored;
}

పూర్ణాంకం cmd_check_ignore(పూర్ణాంకం argc,
		     స్థిరరాశి అక్షరం **argv,
		     స్థిరరాశి అక్షరం *ఉపసర్గ,
		     నిర్మాణం repository *repo వాడని)
{
	పూర్ణాంకం అంకె_ignored;
	నిర్మాణం dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ఖాళీ);

	argc = parse_options(argc, argv, ఉపసర్గ, check_ignore_options,
			     check_ignore_usage, 0);

	ఒకవేళ (stdin_paths) {
		ఒకవేళ (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} లేకపోతే {
		ఒకవేళ (nul_పదం_పంక్తి)
			die(_("-z only makes sense with --stdin"));
		ఒకవేళ (argc == 0)
			die(_("no path specified"));
	}
	ఒకవేళ (quiet) {
		ఒకవేళ (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		ఒకవేళ (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	ఒకవేళ (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	ఒకవేళ (!no_సూచీ && repo_read_సూచీ(the_repository) < 0)
		die(_("index file corrupt"));

	అమరిక_standard_excludes(&dir);

	ఒకవేళ (stdin_paths) {
		అంకె_ignored = check_ignore_stdin_paths(&dir, ఉపసర్గ);
	} లేకపోతే {
		అంకె_ignored = check_ignore(&dir, ఉపసర్గ, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	తిరిగి !అంకె_ignored;
}
