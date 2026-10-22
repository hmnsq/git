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

स्थिर पूर्णांक quiet, verbose, stdin_paths, show_non_matching, no_index;
स्थिर स्थिरांक अक्षर * स्थिरांक check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
रिक्त
};

स्थिर पूर्णांक nul_term_ओळ;

स्थिर स्थिरांक संरचना option check_ignore_options[] = {
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

स्थिर शून्य output_pattern(स्थिरांक अक्षर *path, संरचना path_pattern *pattern)
{
	स्थिरांक अक्षर *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	स्थिरांक अक्षर *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	जर (!nul_term_ओळ) {
		जर (!verbose) {
			write_name_quoted(path, मानक_निकास, '\n');
		} अन्यथा {
			जर (pattern) {
				quote_c_style(pattern->pl->src, रिक्त, मानक_निकास, 0);
				म_लिखो(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			अन्यथा {
				म_लिखो("::\t");
			}
			quote_c_style(path, रिक्त, मानक_निकास, 0);
			ख_अक्षर_दो('\n', मानक_निकास);
		}
	} अन्यथा {
		जर (!verbose) {
			म_लिखो("%s%c", path, '\0');
		} अन्यथा {
			जर (pattern)
				म_लिखो("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			अन्यथा
				म_लिखो("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

स्थिर पूर्णांक check_ignore(संरचना dir_struct *dir,
			स्थिरांक अक्षर *prefix, पूर्णांक argc, स्थिरांक अक्षर **argv)
{
	स्थिरांक अक्षर *full_path;
	अक्षर *seen;
	पूर्णांक अंक_ignored = 0, i;
	संरचना path_pattern *pattern;
	संरचना pathspec pathspec;

	जर (!argc) {
		जर (!quiet)
			ख_लिखो(मानक_त्रुटि, "no pathspec given.\n");
		परत 0;
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
	साठी (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.वस्तुसंच[i].match;
		pattern = रिक्त;
		जर (!seen[i]) {
			पूर्णांक dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			जर (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = रिक्त;
		}
		जर (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.वस्तुसंच[i].original, pattern);
		जर (pattern)
			अंक_ignored++;
	}
	मुक्त(seen);
	clear_pathspec(&pathspec);

	परत अंक_ignored;
}

स्थिर पूर्णांक check_ignore_stdin_paths(संरचना dir_struct *dir, स्थिरांक अक्षर *prefix)
{
	संरचना strbuf buf = STRBUF_INIT;
	संरचना strbuf unquoted = STRBUF_INIT;
	अक्षर *pathspec[2] = { रिक्त, रिक्त };
	strbuf_getline_fn getline_fn;
	पूर्णांक अंक_ignored = 0;

	getline_fn = nul_term_ओळ ? strbuf_getline_nul : strbuf_getline_lf;
	पर्यंत (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
		जर (!nul_term_ओळ && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			जर (unquote_c_style(&unquoted, buf.buf, रिक्त))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		अंक_ignored += check_ignore(dir, prefix,
					    1, (स्थिरांक अक्षर **)pathspec);
		maybe_flush_or_die(मानक_निकास, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	परत अंक_ignored;
}

पूर्णांक cmd_check_ignore(पूर्णांक argc,
		     स्थिरांक अक्षर **argv,
		     स्थिरांक अक्षर *prefix,
		     संरचना repository *repo UNUSED)
{
	पूर्णांक अंक_ignored;
	संरचना dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, रिक्त);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	जर (stdin_paths) {
		जर (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} अन्यथा {
		जर (nul_term_ओळ)
			die(_("-z only makes sense with --stdin"));
		जर (argc == 0)
			die(_("no path specified"));
	}
	जर (quiet) {
		जर (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		जर (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	जर (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	जर (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	जर (stdin_paths) {
		अंक_ignored = check_ignore_stdin_paths(&dir, prefix);
	} अन्यथा {
		अंक_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(मानक_निकास, "ignore to stdout");
	}

	dir_clear(&dir);

	परत !अंक_ignored;
}
