#define USE_THE_REPOSITORY_VARIABLE
#അന്തര്ഭാവ "builtin.h"
#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "dir.h"
#അന്തര്ഭാവ "environment.h"
#അന്തര്ഭാവ "gettext.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "pathspec.h"
#അന്തര്ഭാവ "parse-options.h"
#അന്തര്ഭാവ "submodule.h"
#അന്തര്ഭാവ "write-or-die.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ quiet, verbose, stdin_paths, show_non_matching, no_index;
സ്ഥിതം സ്ഥിരം അക്ഷരം * സ്ഥിരം check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ശൂന്യ
};

സ്ഥിതം പൂർണ്ണസംഖ്യ nul_term_വരി;

സ്ഥിതം സ്ഥിരം ഘടന option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ശൂന്യ, &nul_term_വരി,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

സ്ഥിതം ശൂന്യം output_pattern(സ്ഥിരം അക്ഷരം *path, ഘടന path_pattern *pattern)
{
	സ്ഥിരം അക്ഷരം *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	സ്ഥിരം അക്ഷരം *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	എങ്കിൽ (!nul_term_വരി) {
		എങ്കിൽ (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (pattern) {
				quote_c_style(pattern->pl->src, ശൂന്യ, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			അല്ലെങ്കിൽ {
				printf("::\t");
			}
			quote_c_style(path, ശൂന്യ, stdout, 0);
			fputc('\n', stdout);
		}
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!verbose) {
			printf("%s%c", path, '\0');
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			അല്ലെങ്കിൽ
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_ignore(ഘടന dir_struct *dir,
			സ്ഥിരം അക്ഷരം *prefix, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
	സ്ഥിരം അക്ഷരം *full_path;
	അക്ഷരം *seen;
	പൂർണ്ണസംഖ്യ അക്കം_ignored = 0, i;
	ഘടന path_pattern *pattern;
	ഘടന pathspec pathspec;

	എങ്കിൽ (!argc) {
		എങ്കിൽ (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		തിരികെ 0;
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
	വേണ്ടി (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.ഇനങ്ങൾ[i].match;
		pattern = ശൂന്യ;
		എങ്കിൽ (!seen[i]) {
			പൂർണ്ണസംഖ്യ dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			എങ്കിൽ (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = ശൂന്യ;
		}
		എങ്കിൽ (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.ഇനങ്ങൾ[i].original, pattern);
		എങ്കിൽ (pattern)
			അക്കം_ignored++;
	}
	സ്വതന്ത്രം(seen);
	clear_pathspec(&pathspec);

	തിരികെ അക്കം_ignored;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ check_ignore_stdin_paths(ഘടന dir_struct *dir, സ്ഥിരം അക്ഷരം *prefix)
{
	ഘടന strbuf buf = STRBUF_INIT;
	ഘടന strbuf unquoted = STRBUF_INIT;
	അക്ഷരം *pathspec[2] = { ശൂന്യ, ശൂന്യ };
	strbuf_getline_fn getline_fn;
	പൂർണ്ണസംഖ്യ അക്കം_ignored = 0;

	getline_fn = nul_term_വരി ? strbuf_getline_nul : strbuf_getline_lf;
	ആകുമ്പോൾ (getline_fn(&buf, stdin) != EOF) {
		എങ്കിൽ (!nul_term_വരി && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			എങ്കിൽ (unquote_c_style(&unquoted, buf.buf, ശൂന്യ))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		അക്കം_ignored += check_ignore(dir, prefix,
					    1, (സ്ഥിരം അക്ഷരം **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	തിരികെ അക്കം_ignored;
}

പൂർണ്ണസംഖ്യ cmd_check_ignore(പൂർണ്ണസംഖ്യ argc,
		     സ്ഥിരം അക്ഷരം **argv,
		     സ്ഥിരം അക്ഷരം *prefix,
		     ഘടന repository *repo UNUSED)
{
	പൂർണ്ണസംഖ്യ അക്കം_ignored;
	ഘടന dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ശൂന്യ);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	എങ്കിൽ (stdin_paths) {
		എങ്കിൽ (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (nul_term_വരി)
			die(_("-z only makes sense with --stdin"));
		എങ്കിൽ (argc == 0)
			die(_("no path specified"));
	}
	എങ്കിൽ (quiet) {
		എങ്കിൽ (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		എങ്കിൽ (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	എങ്കിൽ (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	എങ്കിൽ (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	എങ്കിൽ (stdin_paths) {
		അക്കം_ignored = check_ignore_stdin_paths(&dir, prefix);
	} അല്ലെങ്കിൽ {
		അക്കം_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	തിരികെ !അക്കം_ignored;
}
