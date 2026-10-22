#define USE_THE_REPOSITORY_VARIABLE
#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "dir.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "quote.h"
#ضمن "pathspec.h"
#ضمن "parse-options.h"
#ضمن "submodule.h"
#ضمن "write-or-die.h"

ساكن صحيح quiet, verbose, stdin_paths, show_non_matching, no_index;
ساكن ثابت حرف * ثابت check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
عدم
};

ساكن صحيح nul_term_line;

ساكن ثابت بنية option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', عدم, &nul_term_line,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

ساكن فراغ output_pattern(ثابت حرف *path, بنية path_pattern *pattern)
{
	ثابت حرف *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	ثابت حرف *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	𐡄𐡍 (!nul_term_line) {
		𐡄𐡍 (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} وإلا {
			𐡄𐡍 (pattern) {
				quote_c_style(pattern->pl->src, عدم, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			وإلا {
				printf("::\t");
			}
			quote_c_style(path, عدم, stdout, 0);
			fputc('\n', stdout);
		}
	} وإلا {
		𐡄𐡍 (!verbose) {
			printf("%s%c", path, '\0');
		} وإلا {
			𐡄𐡍 (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			وإلا
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

ساكن صحيح check_ignore(بنية dir_struct *dir,
			ثابت حرف *prefix, صحيح argc, ثابت حرف **argv)
{
	ثابت حرف *full_path;
	حرف *seen;
	صحيح num_ignored = 0, i;
	بنية path_pattern *pattern;
	بنية pathspec pathspec;

	𐡄𐡍 (!argc) {
		𐡄𐡍 (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		أرجع 0;
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
	لكل (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.items[i].match;
		pattern = عدم;
		𐡄𐡍 (!seen[i]) {
			صحيح dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			𐡄𐡍 (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = عدم;
		}
		𐡄𐡍 (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.items[i].original, pattern);
		𐡄𐡍 (pattern)
			num_ignored++;
	}
	حرر(seen);
	clear_pathspec(&pathspec);

	أرجع num_ignored;
}

ساكن صحيح check_ignore_stdin_paths(بنية dir_struct *dir, ثابت حرف *prefix)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *pathspec[2] = { عدم, عدم };
	strbuf_getline_fn getline_fn;
	صحيح num_ignored = 0;

	getline_fn = nul_term_line ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		𐡄𐡍 (!nul_term_line && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			𐡄𐡍 (unquote_c_style(&unquoted, buf.buf, عدم))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		num_ignored += check_ignore(dir, prefix,
					    1, (ثابت حرف **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	أرجع num_ignored;
}

صحيح cmd_check_ignore(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *prefix,
		     بنية repository *repo UNUSED)
{
	صحيح num_ignored;
	بنية dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	𐡄𐡍 (stdin_paths) {
		𐡄𐡍 (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} وإلا {
		𐡄𐡍 (nul_term_line)
			die(_("-z only makes sense with --stdin"));
		𐡄𐡍 (argc == 0)
			die(_("no path specified"));
	}
	𐡄𐡍 (quiet) {
		𐡄𐡍 (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		𐡄𐡍 (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	𐡄𐡍 (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	𐡄𐡍 (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	𐡄𐡍 (stdin_paths) {
		num_ignored = check_ignore_stdin_paths(&dir, prefix);
	} وإلا {
		num_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	أرجع !num_ignored;
}
