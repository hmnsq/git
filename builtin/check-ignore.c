#define USE_THE_REPOSITORY_VARIABLE
#شامل "builtin.h"
#شامل "config.h"
#شامل "dir.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "quote.h"
#شامل "pathspec.h"
#شامل "parse-options.h"
#شامل "submodule.h"
#شامل "write-or-die.h"

ایستا صحیح quiet, verbose, stdin_paths, show_non_matching, no_index;
ایستا ثابت نویسه * ثابت check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
پوچ
};

ایستا صحیح nul_term_سطر;

ایستا ثابت ساختار option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', پوچ, &nul_term_سطر,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

ایستا تهی output_pattern(ثابت نویسه *path, ساختار path_pattern *pattern)
{
	ثابت نویسه *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	ثابت نویسه *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	اگر (!nul_term_سطر) {
		اگر (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} وگرنه {
			اگر (pattern) {
				quote_c_style(pattern->pl->src, پوچ, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			وگرنه {
				printf("::\t");
			}
			quote_c_style(path, پوچ, stdout, 0);
			fputc('\n', stdout);
		}
	} وگرنه {
		اگر (!verbose) {
			printf("%s%c", path, '\0');
		} وگرنه {
			اگر (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			وگرنه
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

ایستا صحیح check_ignore(ساختار dir_struct *dir,
			ثابت نویسه *prefix, صحیح argc, ثابت نویسه **argv)
{
	ثابت نویسه *full_path;
	نویسه *seen;
	صحیح رقم_ignored = 0, i;
	ساختار path_pattern *pattern;
	ساختار pathspec pathspec;

	اگر (!argc) {
		اگر (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		بازگشت 0;
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
	برای (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.اقلام[i].match;
		pattern = پوچ;
		اگر (!seen[i]) {
			صحیح dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			اگر (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = پوچ;
		}
		اگر (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.اقلام[i].original, pattern);
		اگر (pattern)
			رقم_ignored++;
	}
	آزاد(seen);
	clear_pathspec(&pathspec);

	بازگشت رقم_ignored;
}

ایستا صحیح check_ignore_stdin_paths(ساختار dir_struct *dir, ثابت نویسه *prefix)
{
	ساختار strbuf buf = STRBUF_INIT;
	ساختار strbuf unquoted = STRBUF_INIT;
	نویسه *pathspec[2] = { پوچ, پوچ };
	strbuf_getline_fn getline_fn;
	صحیح رقم_ignored = 0;

	getline_fn = nul_term_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	وقتی (getline_fn(&buf, stdin) != EOF) {
		اگر (!nul_term_سطر && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			اگر (unquote_c_style(&unquoted, buf.buf, پوچ))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		رقم_ignored += check_ignore(dir, prefix,
					    1, (ثابت نویسه **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	بازگشت رقم_ignored;
}

صحیح cmd_check_ignore(صحیح argc,
		     ثابت نویسه **argv,
		     ثابت نویسه *prefix,
		     ساختار repository *repo UNUSED)
{
	صحیح رقم_ignored;
	ساختار dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, پوچ);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	اگر (stdin_paths) {
		اگر (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} وگرنه {
		اگر (nul_term_سطر)
			die(_("-z only makes sense with --stdin"));
		اگر (argc == 0)
			die(_("no path specified"));
	}
	اگر (quiet) {
		اگر (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		اگر (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	اگر (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	اگر (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	اگر (stdin_paths) {
		رقم_ignored = check_ignore_stdin_paths(&dir, prefix);
	} وگرنه {
		رقم_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	بازگشت !رقم_ignored;
}
