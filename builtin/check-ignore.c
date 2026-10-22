#define USE_THE_REPOSITORY_VARIABLE
#अन्तर्भाव "builtin.h"
#अन्तर्भाव "config.h"
#अन्तर्भाव "dir.h"
#अन्तर्भाव "environment.h"
#अन्तर्भाव "gettext.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "pathspec.h"
#अन्तर्भाव "parse-options.h"
#अन्तर्भाव "submodule.h"
#अन्तर्भाव "write-or-die.h"

स्थिर पूर्णाङ्क quiet, verbose, stdin_paths, show_non_matching, no_index;
स्थिर ध्रुव वर्ण * ध्रुव check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
रिक्त
};

स्थिर पूर्णाङ्क nul_term_ओळ;

स्थिर ध्रुव संरचना option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', रिक्त, &nul_term_ओळ,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

स्थिर शून्य output_pattern(ध्रुव वर्ण *path, संरचना path_pattern *pattern)
{
	ध्रुव वर्ण *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	ध्रुव वर्ण *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	यदि (!nul_term_ओळ) {
		यदि (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} अन्यथा {
			यदि (pattern) {
				quote_c_style(pattern->pl->src, रिक्त, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			अन्यथा {
				printf("::\t");
			}
			quote_c_style(path, रिक्त, stdout, 0);
			fputc('\n', stdout);
		}
	} अन्यथा {
		यदि (!verbose) {
			printf("%s%c", path, '\0');
		} अन्यथा {
			यदि (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			अन्यथा
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

स्थिर पूर्णाङ्क check_ignore(संरचना dir_struct *dir,
			ध्रुव वर्ण *prefix, पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
	ध्रुव वर्ण *full_path;
	वर्ण *seen;
	पूर्णाङ्क अंक_ignored = 0, i;
	संरचना path_pattern *pattern;
	संरचना pathspec pathspec;

	यदि (!argc) {
		यदि (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		प्रत्यागम 0;
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
	प्रति (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.वस्तुसंच[i].match;
		pattern = रिक्त;
		यदि (!seen[i]) {
			पूर्णाङ्क dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			यदि (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = रिक्त;
		}
		यदि (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.वस्तुसंच[i].original, pattern);
		यदि (pattern)
			अंक_ignored++;
	}
	मुक्त(seen);
	clear_pathspec(&pathspec);

	प्रत्यागम अंक_ignored;
}

स्थिर पूर्णाङ्क check_ignore_stdin_paths(संरचना dir_struct *dir, ध्रुव वर्ण *prefix)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf unquoted = STRBUF_INIT;
	वर्ण *pathspec[2] = { रिक्त, रिक्त };
	strbuf_getline_fn getline_fn;
	पूर्णाङ्क अंक_ignored = 0;

	getline_fn = nul_term_ओळ ? strbuf_getline_nul : strbuf_getline_lf;
	यावत् (getline_fn(&buf, stdin) != EOF) {
		यदि (!nul_term_ओळ && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			यदि (unquote_c_style(&unquoted, buf.buf, रिक्त))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		अंक_ignored += check_ignore(dir, prefix,
					    1, (ध्रुव वर्ण **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	प्रत्यागम अंक_ignored;
}

पूर्णाङ्क cmd_check_ignore(पूर्णाङ्क argc,
		     ध्रुव वर्ण **argv,
		     ध्रुव वर्ण *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णाङ्क अंक_ignored;
	संरचना dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	यदि (stdin_paths) {
		यदि (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} अन्यथा {
		यदि (nul_term_ओळ)
			die(_("-z only makes sense with --stdin"));
		यदि (argc == 0)
			die(_("no path specified"));
	}
	यदि (quiet) {
		यदि (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		यदि (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	यदि (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	यदि (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	यदि (stdin_paths) {
		अंक_ignored = check_ignore_stdin_paths(&dir, prefix);
	} अन्यथा {
		अंक_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	प्रत्यागम !अंक_ignored;
}
