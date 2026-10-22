#define USE_THE_REPOSITORY_VARIABLE
#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "dir.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "pathspec.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "submodule.h"
#ಅನ್ತರ್ಭಾವ "write-or-die.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ quiet, verbose, stdin_paths, show_non_matching, no_index;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ nul_term_ಸಾಲು;

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ರಿಕ್ತ, &nul_term_ಸಾಲು,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

ಸ್ಥಿರ ಶೂನ್ಯ output_pattern(ಧ್ರುವ ವರ್ಣ *path, ಸಂರಚನಾ path_pattern *pattern)
{
	ಧ್ರುವ ವರ್ಣ *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	ಧ್ರುವ ವರ್ಣ *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	ಯದಿ (!nul_term_ಸಾಲು) {
		ಯದಿ (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} ಅನ್ಯಥಾ {
			ಯದಿ (pattern) {
				quote_c_style(pattern->pl->src, ರಿಕ್ತ, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			ಅನ್ಯಥಾ {
				printf("::\t");
			}
			quote_c_style(path, ರಿಕ್ತ, stdout, 0);
			fputc('\n', stdout);
		}
	} ಅನ್ಯಥಾ {
		ಯದಿ (!verbose) {
			printf("%s%c", path, '\0');
		} ಅನ್ಯಥಾ {
			ಯದಿ (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			ಅನ್ಯಥಾ
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_ignore(ಸಂರಚನಾ dir_struct *dir,
			ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
	ಧ್ರುವ ವರ್ಣ *full_path;
	ವರ್ಣ *seen;
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ignored = 0, i;
	ಸಂರಚನಾ path_pattern *pattern;
	ಸಂರಚನಾ pathspec pathspec;

	ಯದಿ (!argc) {
		ಯದಿ (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		ಪ್ರತ್ಯಾಗಮ 0;
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
	ಪ್ರತಿ (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.ವಸ್ತುಗಳು[i].match;
		pattern = ರಿಕ್ತ;
		ಯದಿ (!seen[i]) {
			ಪೂರ್ಣಾಙ್ಕ dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			ಯದಿ (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = ರಿಕ್ತ;
		}
		ಯದಿ (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.ವಸ್ತುಗಳು[i].original, pattern);
		ಯದಿ (pattern)
			ಅಂಕೆ_ignored++;
	}
	ಮುಕ್ತ(seen);
	clear_pathspec(&pathspec);

	ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_ignored;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_ignore_stdin_paths(ಸಂರಚನಾ dir_struct *dir, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
	ಸಂರಚನಾ strbuf unquoted = STRBUF_INIT;
	ವರ್ಣ *pathspec[2] = { ರಿಕ್ತ, ರಿಕ್ತ };
	strbuf_getline_fn getline_fn;
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ignored = 0;

	getline_fn = nul_term_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಯಾವತ್ (getline_fn(&buf, stdin) != EOF) {
		ಯದಿ (!nul_term_ಸಾಲು && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ಯದಿ (unquote_c_style(&unquoted, buf.buf, ರಿಕ್ತ))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		ಅಂಕೆ_ignored += check_ignore(dir, prefix,
					    1, (ಧ್ರುವ ವರ್ಣ **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_ignored;
}

ಪೂರ್ಣಾಙ್ಕ cmd_check_ignore(ಪೂರ್ಣಾಙ್ಕ argc,
		     ಧ್ರುವ ವರ್ಣ **argv,
		     ಧ್ರುವ ವರ್ಣ *prefix,
		     ಸಂರಚನಾ repository *repo UNUSED)
{
	ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_ignored;
	ಸಂರಚನಾ dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ರಿಕ್ತ);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	ಯದಿ (stdin_paths) {
		ಯದಿ (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} ಅನ್ಯಥಾ {
		ಯದಿ (nul_term_ಸಾಲು)
			die(_("-z only makes sense with --stdin"));
		ಯದಿ (argc == 0)
			die(_("no path specified"));
	}
	ಯದಿ (quiet) {
		ಯದಿ (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		ಯದಿ (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	ಯದಿ (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	ಯದಿ (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	ಯದಿ (stdin_paths) {
		ಅಂಕೆ_ignored = check_ignore_stdin_paths(&dir, prefix);
	} ಅನ್ಯಥಾ {
		ಅಂಕೆ_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	ಪ್ರತ್ಯಾಗಮ !ಅಂಕೆ_ignored;
}
