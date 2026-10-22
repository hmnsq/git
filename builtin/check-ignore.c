#define USE_THE_REPOSITORY_VARIABLE
#ଅନ୍ତର୍ଭାଵ "builtin.h"
#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "dir.h"
#ଅନ୍ତର୍ଭାଵ "environment.h"
#ଅନ୍ତର୍ଭାଵ "gettext.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "pathspec.h"
#ଅନ୍ତର୍ଭାଵ "parse-options.h"
#ଅନ୍ତର୍ଭାଵ "submodule.h"
#ଅନ୍ତର୍ଭାଵ "write-or-die.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା quiet, verbose, stdin_paths, show_non_matching, no_index;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର * ଅଚଳ check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ଖାଲି
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nul_term_ଧାଡ଼ି;

ସ୍ଥିର ଅଚଳ ସଂରଚନା option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ଖାଲି, &nul_term_ଧାଡ଼ି,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

ସ୍ଥିର ଶୂନ୍ୟ output_pattern(ଅଚଳ ଅକ୍ଷର *path, ସଂରଚନା path_pattern *pattern)
{
	ଅଚଳ ଅକ୍ଷର *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	ଅଚଳ ଅକ୍ଷର *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	ଯଦି (!nul_term_ଧାଡ଼ି) {
		ଯଦି (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} ଅନ୍ୟଥା {
			ଯଦି (pattern) {
				quote_c_style(pattern->pl->src, ଖାଲି, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			ଅନ୍ୟଥା {
				printf("::\t");
			}
			quote_c_style(path, ଖାଲି, stdout, 0);
			fputc('\n', stdout);
		}
	} ଅନ୍ୟଥା {
		ଯଦି (!verbose) {
			printf("%s%c", path, '\0');
		} ଅନ୍ୟଥା {
			ଯଦି (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			ଅନ୍ୟଥା
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_ignore(ସଂରଚନା dir_struct *dir,
			ଅଚଳ ଅକ୍ଷର *prefix, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
	ଅଚଳ ଅକ୍ଷର *full_path;
	ଅକ୍ଷର *seen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ignored = 0, i;
	ସଂରଚନା path_pattern *pattern;
	ସଂରଚନା pathspec pathspec;

	ଯଦି (!argc) {
		ଯଦି (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		ଫେରନ୍ତୁ 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_PATH |
		       PATHSPEC_KEEP_ORDER,
		       prefix, argv);

	die_path_inside_submodule(the_repository->index, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	seen = find_pathspecs_matching_against_index(&pathspec, the_repository->index,
						     PS_HEED_SKIP_WORKTREE);
	ପାଇଁ (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].match;
		pattern = ଖାଲି;
		ଯଦି (!seen[i]) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			ଯଦି (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = ଖାଲି;
		}
		ଯଦି (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.ବସ୍ତୁଗୁଡ଼ିକ[i].original, pattern);
		ଯଦି (pattern)
			ଅଙ୍କ_ignored++;
	}
	ମୁକ୍ତ(seen);
	clear_pathspec(&pathspec);

	ଫେରନ୍ତୁ ଅଙ୍କ_ignored;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_ignore_stdin_paths(ସଂରଚନା dir_struct *dir, ଅଚଳ ଅକ୍ଷର *prefix)
{
	ସଂରଚନା strbuf buf = STRBUF_INIT;
	ସଂରଚନା strbuf unquoted = STRBUF_INIT;
	ଅକ୍ଷର *pathspec[2] = { ଖାଲି, ଖାଲି };
	strbuf_getline_fn getline_fn;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ignored = 0;

	getline_fn = nul_term_ଧାଡ଼ି ? strbuf_getline_nul : strbuf_getline_lf;
	ଯେତେବେଳେ (getline_fn(&buf, stdin) != EOF) {
		ଯଦି (!nul_term_ଧାଡ଼ି && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ଯଦି (unquote_c_style(&unquoted, buf.buf, ଖାଲି))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		ଅଙ୍କ_ignored += check_ignore(dir, prefix,
					    1, (ଅଚଳ ଅକ୍ଷର **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	ଫେରନ୍ତୁ ଅଙ୍କ_ignored;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmd_check_ignore(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc,
		     ଅଚଳ ଅକ୍ଷର **argv,
		     ଅଚଳ ଅକ୍ଷର *prefix,
		     ସଂରଚନା repository *repo UNUSED)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_ignored;
	ସଂରଚନା dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ଖାଲି);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	ଯଦି (stdin_paths) {
		ଯଦି (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} ଅନ୍ୟଥା {
		ଯଦି (nul_term_ଧାଡ଼ି)
			die(_("-z only makes sense with --stdin"));
		ଯଦି (argc == 0)
			die(_("no path specified"));
	}
	ଯଦି (quiet) {
		ଯଦି (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		ଯଦି (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	ଯଦି (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	ଯଦି (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	ଯଦି (stdin_paths) {
		ଅଙ୍କ_ignored = check_ignore_stdin_paths(&dir, prefix);
	} ଅନ୍ୟଥା {
		ଅଙ୍କ_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	ଫେରନ୍ତୁ !ଅଙ୍କ_ignored;
}
