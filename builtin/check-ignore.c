#define USE_THE_REPOSITORY_VARIABLE
#அந்தர்பாவ "builtin.h"
#அந்தர்பாவ "config.h"
#அந்தர்பாவ "dir.h"
#அந்தர்பாவ "environment.h"
#அந்தர்பாவ "gettext.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "pathspec.h"
#அந்தர்பாவ "parse-options.h"
#அந்தர்பாவ "submodule.h"
#அந்தர்பாவ "write-or-die.h"

நிலையான முழு quiet, verbose, stdin_paths, show_non_matching, no_index;
நிலையான மாறிலி எழுத்து * மாறிலி check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
வெற்று
};

நிலையான முழு nul_term_வரி;

நிலையான மாறிலி கட்டமைப்பு option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_GROUP(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', வெற்று, &nul_term_வரி,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_index,
		 N_("ignore index when checking")),
	OPT_END()
};

நிலையான வெறுமை output_pattern(மாறிலி எழுத்து *path, கட்டமைப்பு path_pattern *pattern)
{
	மாறிலி எழுத்து *bang  = (pattern && pattern->flags & PATTERN_FLAG_NEGATIVE)  ? "!" : "";
	மாறிலி எழுத்து *slash = (pattern && pattern->flags & PATTERN_FLAG_MUSTBEDIR) ? "/" : "";
	எனில் (!nul_term_வரி) {
		எனில் (!verbose) {
			write_name_quoted(path, stdout, '\n');
		} இல்லையெனில் {
			எனில் (pattern) {
				quote_c_style(pattern->pl->src, வெற்று, stdout, 0);
				printf(":%d:%s%s%s\t",
				       pattern->srcpos,
				       bang, pattern->pattern, slash);
			}
			இல்லையெனில் {
				printf("::\t");
			}
			quote_c_style(path, வெற்று, stdout, 0);
			fputc('\n', stdout);
		}
	} இல்லையெனில் {
		எனில் (!verbose) {
			printf("%s%c", path, '\0');
		} இல்லையெனில் {
			எனில் (pattern)
				printf("%s%c%d%c%s%s%s%c%s%c",
				       pattern->pl->src, '\0',
				       pattern->srcpos, '\0',
				       bang, pattern->pattern, slash, '\0',
				       path, '\0');
			இல்லையெனில்
				printf("%c%c%c%s%c", '\0', '\0', '\0', path, '\0');
		}
	}
}

நிலையான முழு check_ignore(கட்டமைப்பு dir_struct *dir,
			மாறிலி எழுத்து *prefix, முழு argc, மாறிலி எழுத்து **argv)
{
	மாறிலி எழுத்து *full_path;
	எழுத்து *seen;
	முழு இலக்கம்_ignored = 0, i;
	கட்டமைப்பு path_pattern *pattern;
	கட்டமைப்பு pathspec pathspec;

	எனில் (!argc) {
		எனில் (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		திரும்பு 0;
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
	க்கு (i = 0; i < pathspec.nr; i++) {
		full_path = pathspec.பொருட்கள்[i].match;
		pattern = வெற்று;
		எனில் (!seen[i]) {
			முழு dtype = DT_UNKNOWN;
			pattern = last_matching_pattern(dir, the_repository->index,
							full_path, &dtype);
			எனில் (!verbose && pattern &&
			    pattern->flags & PATTERN_FLAG_NEGATIVE)
				pattern = வெற்று;
		}
		எனில் (!quiet && (pattern || show_non_matching))
			output_pattern(pathspec.பொருட்கள்[i].original, pattern);
		எனில் (pattern)
			இலக்கம்_ignored++;
	}
	விடுவி(seen);
	clear_pathspec(&pathspec);

	திரும்பு இலக்கம்_ignored;
}

நிலையான முழு check_ignore_stdin_paths(கட்டமைப்பு dir_struct *dir, மாறிலி எழுத்து *prefix)
{
	கட்டமைப்பு strbuf buf = STRBUF_INIT;
	கட்டமைப்பு strbuf unquoted = STRBUF_INIT;
	எழுத்து *pathspec[2] = { வெற்று, வெற்று };
	strbuf_getline_fn getline_fn;
	முழு இலக்கம்_ignored = 0;

	getline_fn = nul_term_வரி ? strbuf_getline_nul : strbuf_getline_lf;
	போது (getline_fn(&buf, stdin) != EOF) {
		எனில் (!nul_term_வரி && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			எனில் (unquote_c_style(&unquoted, buf.buf, வெற்று))
				die("line is badly quoted");
			strbuf_swap(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		இலக்கம்_ignored += check_ignore(dir, prefix,
					    1, (மாறிலி எழுத்து **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	திரும்பு இலக்கம்_ignored;
}

முழு cmd_check_ignore(முழு argc,
		     மாறிலி எழுத்து **argv,
		     மாறிலி எழுத்து *prefix,
		     கட்டமைப்பு repository *repo UNUSED)
{
	முழு இலக்கம்_ignored;
	கட்டமைப்பு dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, வெற்று);

	argc = parse_options(argc, argv, prefix, check_ignore_options,
			     check_ignore_usage, 0);

	எனில் (stdin_paths) {
		எனில் (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} இல்லையெனில் {
		எனில் (nul_term_வரி)
			die(_("-z only makes sense with --stdin"));
		எனில் (argc == 0)
			die(_("no path specified"));
	}
	எனில் (quiet) {
		எனில் (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		எனில் (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	எனில் (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	எனில் (!no_index && repo_read_index(the_repository) < 0)
		die(_("index file corrupt"));

	setup_standard_excludes(&dir);

	எனில் (stdin_paths) {
		இலக்கம்_ignored = check_ignore_stdin_paths(&dir, prefix);
	} இல்லையெனில் {
		இலக்கம்_ignored = check_ignore(&dir, prefix, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	திரும்பு !இலக்கம்_ignored;
}
