#घोषणा USE_THE_REPOSITORY_VARIABLE
#समावेश "builtin.h"
#समावेश "config.h"
#समावेश "dir.h"
#समावेश "environment.h"
#समावेश "gettext.h"
#समावेश "quote.h"
#समावेश "pathspec.h"
#समावेश "parse-options.h"
#समावेश "submodule.h"
#समावेश "write-or-die.h"

अटल पूर्णांक quiet, verbose, stdin_paths, show_non_matching, no_सूचकांक;
अटल स्थिर अक्षर * स्थिर check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
शून्य
};

अटल पूर्णांक nul_term_पंक्ति;

अटल स्थिर काष्ठा option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', शून्य, &nul_term_पंक्ति,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_सूचकांक,
		 N_("ignore index when checking")),
	OPT_END()
};

अटल व्योम output_pattern(स्थिर अक्षर *path, काष्ठा path_pattern *pattern)
{
	स्थिर अक्षर *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	स्थिर अक्षर *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	अगर (!nul_term_पंक्ति) {
		अगर (!verbose) {
			write_name_quoted(path, मानक_निकास, '\n');
		} अन्यथा {
			अगर (pattern) {
				quote_c_style(pattern->pl->src, शून्य, मानक_निकास, 0);
				म_लिखो(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			अन्यथा {
				म_लिखो("::\t");
			}
			quote_c_style(path, शून्य, मानक_निकास, 0);
			ख_अक्षर_दो('\n', मानक_निकास);
		}
	} अन्यथा {
		अगर (!verbose) {
			म_लिखो("%s%c", path, '\0');
		} अन्यथा {
			अगर (pattern)
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

अटल पूर्णांक check_ignore(काष्ठा dir_struct *dir,
			स्थिर अक्षर *prefix, पूर्णांक argc, स्थिर अक्षर **argv)
{
	स्थिर अक्षर *full_path;
	अक्षर *seen;
	पूर्णांक अंक_ignored = 0, i;
	काष्ठा path_pattern *pattern;
	काष्ठा pathspec pathspec;

	अगर (!argc) {
		अगर (!quiet)
			ख_लिखो(मानक_त्रुटि, "no pathspec given.\n");
		वापस 0;
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

	die_path_inside_submodule(the_repository->सूचकांक, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	seen = find_pathspecs_matching_against_सूचकांक(&pathspec, the_repository->सूचकांक,
						     PS_HEED_SKIP_WORKTREE);
	क्रम (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.वस्तुएँ[i].match;
		pattern = शून्य;
		अगर (!seen[i]) {
			पूर्णांक dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->सूचकांक,
							full_path, &dtype);
			अगर (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = शून्य;
		}
		अगर (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.वस्तुएँ[i].original, pattern);
		अगर (pattern)
			अंक_ignored++;
	}
	मुक्त(seen);
	clear_pathspec(&pathspec);

	वापस अंक_ignored;
}

अटल पूर्णांक check_ignore_stdin_paths(काष्ठा dir_struct *dir, स्थिर अक्षर *prefix)
{
	काष्ठा strbuf buf = STRBUF_INIT;
	काष्ठा strbuf unquoted = STRBUF_INIT;
	अक्षर *pathspec[2] = { शून्य, शून्य };
	strbuf_getline_fn getline_fn;
	पूर्णांक अंक_ignored = 0;

	getline_fn = nul_term_पंक्ति ? strbuf_getline_nul : strbuf_getline_lf;
	जबतक (getline_fn(&buf, मानक_निवेश) != खातापूर्ण) {
		अगर (!nul_term_पंक्ति && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			अगर (unquote_c_style(&unquoted, buf.buf, शून्य))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		अंक_ignored += check_ignore(dir, prefix,
					    1, (स्थिर अक्षर **)pathspec);
		maybe_flush_or_die(मानक_निकास, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	वापस अंक_ignored;
}

पूर्णांक cmd_check_ignore(पूर्णांक argc,
		     स्थिर अक्षर **argv,
		     स्थिर अक्षर *prefix,
		     काष्ठा repository *repo UNUSED)
{
	पूर्णांक अंक_ignored;
	काष्ठा dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, शून्य);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	अगर (stdin_paths) {
		अगर (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} अन्यथा {
		अगर (nul_term_पंक्ति)
			die(_("-z only makes sense with --stdin"));
		अगर (argc == 0)
			die(_("no path specified"));
	}
	अगर (quiet) {
		अगर (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		अगर (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	अगर (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	अगर (!no_सूचकांक && repo_read_सूचकांक(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	अगर (stdin_paths) {
		अंक_ignored = check_ignore_stdin_paths(&dir, prefix);
	} अन्यथा {
		अंक_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(मानक_निकास, "ignore to stdout");
	}

	dir_clear(&dir);

	वापस !अंक_ignored;
}
