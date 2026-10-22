#define USE_THE_REPOSITORY_VARIABLE
#অন্তর্ভাব "builtin.h"
#অন্তর্ভাব "config.h"
#অন্তর্ভাব "dir.h"
#অন্তর্ভাব "environment.h"
#অন্তর্ভাব "gettext.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "pathspec.h"
#অন্তর্ভাব "parse-options.h"
#অন্তর্ভাব "submodule.h"
#অন্তর্ভাব "write-or-die.h"

স্থির পূর্ণসংখ্যা quiet, verbose, stdin_paths, show_non_matching, no_সূচক;
স্থির ধ্রুবক অক্ষর * ধ্রুবক check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ফাঁকা
};

স্থির পূর্ণসংখ্যা nul_term_সারি;

স্থির ধ্রুবক কাঠামো option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ফাঁকা, &nul_term_সারি,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_সূচক,
		 N_("ignore index when checking")),
	OPT_END()
};

স্থির শূন্য output_pattern(ধ্রুবক অক্ষর *path, কাঠামো path_pattern *pattern)
{
	ধ্রুবক অক্ষর *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	ধ্রুবক অক্ষর *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	যদি (!nul_term_সারি) {
		যদি (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} অন্যথা {
			যদি (pattern) {
				quote_c_style(pattern->pl->src, ফাঁকা, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			অন্যথা {
				printf("::\t");
			}
			quote_c_style(path, ফাঁকা, stdout, 0);
			fputc('\n', stdout);
		}
	} অন্যথা {
		যদি (!verbose) {
			printf("%s%c", path, '\0');
		} অন্যথা {
			যদি (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			অন্যথা
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

স্থির পূর্ণসংখ্যা check_ignore(কাঠামো dir_struct *dir,
			ধ্রুবক অক্ষর *prefix, পূর্ণসংখ্যা argc, ধ্রুবক অক্ষর **argv)
{
	ধ্রুবক অক্ষর *full_path;
	অক্ষর *seen;
	পূর্ণসংখ্যা অঙ্ক_ignored = 0, i;
	কাঠামো path_pattern *pattern;
	কাঠামো pathspec pathspec;

	যদি (!argc) {
		যদি (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		ফেরত 0;
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

	die_path_inside_submodule(the_repository->সূচক, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	seen = find_pathspecs_matching_against_সূচক(&pathspec, the_repository->সূচক,
						     PS_HEED_SKIP_WORKTREE);
	জন্য (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.বস্তুসমূহ[i].match;
		pattern = ফাঁকা;
		যদি (!seen[i]) {
			পূর্ণসংখ্যা dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->সূচক,
							full_path, &dtype);
			যদি (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = ফাঁকা;
		}
		যদি (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.বস্তুসমূহ[i].original, pattern);
		যদি (pattern)
			অঙ্ক_ignored++;
	}
	মুক্ত(seen);
	clear_pathspec(&pathspec);

	ফেরত অঙ্ক_ignored;
}

স্থির পূর্ণসংখ্যা check_ignore_stdin_paths(কাঠামো dir_struct *dir, ধ্রুবক অক্ষর *prefix)
{
	কাঠামো strbuf buf = STRBUF_INIT;
	কাঠামো strbuf unquoted = STRBUF_INIT;
	অক্ষর *pathspec[2] = { ফাঁকা, ফাঁকা };
	strbuf_getline_fn getline_fn;
	পূর্ণসংখ্যা অঙ্ক_ignored = 0;

	getline_fn = nul_term_সারি ? strbuf_getline_nul : strbuf_getline_lf;
	যখন (getline_fn(&buf, stdin) != EOF) {
		যদি (!nul_term_সারি && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			যদি (unquote_c_style(&unquoted, buf.buf, ফাঁকা))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		অঙ্ক_ignored += check_ignore(dir, prefix,
					    1, (ধ্রুবক অক্ষর **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	ফেরত অঙ্ক_ignored;
}

পূর্ণসংখ্যা cmd_check_ignore(পূর্ণসংখ্যা argc,
		     ধ্রুবক অক্ষর **argv,
		     ধ্রুবক অক্ষর *prefix,
		     কাঠামো repository *repo UNUSED)
{
	পূর্ণসংখ্যা অঙ্ক_ignored;
	কাঠামো dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ফাঁকা);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	যদি (stdin_paths) {
		যদি (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} অন্যথা {
		যদি (nul_term_সারি)
			die(_("-z only makes sense with --stdin"));
		যদি (argc == 0)
			die(_("no path specified"));
	}
	যদি (quiet) {
		যদি (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		যদি (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	যদি (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	যদি (!no_সূচক && repo_read_সূচক(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	যদি (stdin_paths) {
		অঙ্ক_ignored = check_ignore_stdin_paths(&dir, prefix);
	} অন্যথা {
		অঙ্ক_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	ফেরত !অঙ্ক_ignored;
}
