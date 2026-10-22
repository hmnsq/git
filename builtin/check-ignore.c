#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_THE_REPOSITORY_VARIABLE
#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dir.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pathspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "submodule.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "write-or-die.h"

ਸਥਿਰ ਪੂਰਨ quiet, verbose, stdin_paths, show_non_matching, no_ਸੂਚਕਾਂਕ;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ nul_ਪਦ_ਸਤਰ;

ਸਥਿਰ ਅਚਲ ਬਣਤਰ option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_ਸਮੂਹ(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ਨਿਰਾ, &nul_ਪਦ_ਸਤਰ,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_ਸੂਚਕਾਂਕ,
		 N_("ignore index when checking")),
	OPT_ਸਿਰਾ()
};

ਸਥਿਰ ਖਾਲੀ output_ਨਮੂਨਾ(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਣਤਰ ਰਸਤਾ_ਨਮੂਨਾ *ਨਮੂਨਾ)
{
	ਅਚਲ ਅੱਖਰ *bang  = (ਨਮੂਨਾ && ਨਮੂਨਾ->flags & ਨਮੂਨਾ_FLAG_NEGATIVE)  ? "!" : "";
	ਅਚਲ ਅੱਖਰ *slash = (ਨਮੂਨਾ && ਨਮੂਨਾ->flags & ਨਮੂਨਾ_FLAG_MUSTBEDIR) ? "/" : "";
	ਜੇ (!nul_ਪਦ_ਸਤਰ) {
		ਜੇ (!verbose) {
			write_name_quoted(ਰਸਤਾ, stdout, '\n');
		} ਨਹੀਂਤਾਂ {
			ਜੇ (ਨਮੂਨਾ) {
				quote_c_style(ਨਮੂਨਾ->pl->src, ਨਿਰਾ, stdout, 0);
				ਲਿਖੋ(":%d:%s%s%s\t",
				       ਨਮੂਨਾ->srcpos,
				       bang, ਨਮੂਨਾ->ਨਮੂਨਾ, slash);
			}
			ਨਹੀਂਤਾਂ {
				ਲਿਖੋ("::\t");
			}
			quote_c_style(ਰਸਤਾ, ਨਿਰਾ, stdout, 0);
			fputc('\n', stdout);
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!verbose) {
			ਲਿਖੋ("%s%c", ਰਸਤਾ, '\0');
		} ਨਹੀਂਤਾਂ {
			ਜੇ (ਨਮੂਨਾ)
				ਲਿਖੋ("%s%c%d%c%s%s%s%c%s%c",
				       ਨਮੂਨਾ->pl->src, '\0',
				       ਨਮੂਨਾ->srcpos, '\0',
				       bang, ਨਮੂਨਾ->ਨਮੂਨਾ, slash, '\0',
				       ਰਸਤਾ, '\0');
			ਨਹੀਂਤਾਂ
				ਲਿਖੋ("%c%c%c%s%c", '\0', '\0', '\0', ਰਸਤਾ, '\0');
		}
	}
}

ਸਥਿਰ ਪੂਰਨ check_ignore(ਬਣਤਰ dir_struct *dir,
			ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
	ਅਚਲ ਅੱਖਰ *ਭਰਿਆ_ਰਸਤਾ;
	ਅੱਖਰ *ਵੇਖਿਆ;
	ਪੂਰਨ ਅੰਕ_ignored = 0, i;
	ਬਣਤਰ ਰਸਤਾ_ਨਮੂਨਾ *ਨਮੂਨਾ;
	ਬਣਤਰ pathspec pathspec;

	ਜੇ (!argc) {
		ਜੇ (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		ਵਾਪਸ 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_ਰਸਤਾ |
		       PATHSPEC_KEEP_ਕ੍ਰਮ,
		       ਅਗੇਤਰ, argv);

	die_ਰਸਤਾ_inside_submodule(the_repository->ਸੂਚਕਾਂਕ, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	ਵੇਖਿਆ = ਲੱਭੋ_pathspecs_matching_against_ਸੂਚਕਾਂਕ(&pathspec, the_repository->ਸੂਚਕਾਂਕ,
						     PS_HEED_SKIP_WORKTREE);
	ਲਈ (i = 0; i < pathspec.nr; i++) {
		ਭਰਿਆ_ਰਸਤਾ = pathspec.ਚੀਜ਼ਾਂ[i].ਮੇਲ;
		ਨਮੂਨਾ = ਨਿਰਾ;
		ਜੇ (!ਵੇਖਿਆ[i]) {
			ਪੂਰਨ dtype = DT_UNKNOWN;
			ਨਮੂਨਾ = ਆਖਰੀ_matching_ਨਮੂਨਾ(dir, the_repository->ਸੂਚਕਾਂਕ,
							ਭਰਿਆ_ਰਸਤਾ, &dtype);
			ਜੇ (!verbose && ਨਮੂਨਾ &&
			    ਨਮੂਨਾ->flags & ਨਮੂਨਾ_FLAG_NEGATIVE)
				ਨਮੂਨਾ = ਨਿਰਾ;
		}
		ਜੇ (!quiet && (ਨਮੂਨਾ || show_non_matching))
			output_ਨਮੂਨਾ(pathspec.ਚੀਜ਼ਾਂ[i].original, ਨਮੂਨਾ);
		ਜੇ (ਨਮੂਨਾ)
			ਅੰਕ_ignored++;
	}
	ਮੁਕਤ(ਵੇਖਿਆ);
	clear_pathspec(&pathspec);

	ਵਾਪਸ ਅੰਕ_ignored;
}

ਸਥਿਰ ਪੂਰਨ check_ignore_stdin_paths(ਬਣਤਰ dir_struct *dir, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਬਣਤਰ strbuf buf = STRBUF_INIT;
	ਬਣਤਰ strbuf unquoted = STRBUF_INIT;
	ਅੱਖਰ *pathspec[2] = { ਨਿਰਾ, ਨਿਰਾ };
	strbuf_getline_fn getline_fn;
	ਪੂਰਨ ਅੰਕ_ignored = 0;

	getline_fn = nul_ਪਦ_ਸਤਰ ? strbuf_getline_nul : strbuf_getline_lf;
	ਜਦੋਂ (getline_fn(&buf, stdin) != EOF) {
		ਜੇ (!nul_ਪਦ_ਸਤਰ && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ਜੇ (unquote_c_style(&unquoted, buf.buf, ਨਿਰਾ))
				die("line is badly quoted");
			strbuf_ਅਦਲਾ_ਬਦਲੀ(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		ਅੰਕ_ignored += check_ignore(dir, ਅਗੇਤਰ,
					    1, (ਅਚਲ ਅੱਖਰ **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	ਵਾਪਸ ਅੰਕ_ignored;
}

ਪੂਰਨ cmd_check_ignore(ਪੂਰਨ argc,
		     ਅਚਲ ਅੱਖਰ **argv,
		     ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		     ਬਣਤਰ repository *repo ਅਣਵਰਤਿਆ)
{
	ਪੂਰਨ ਅੰਕ_ignored;
	ਬਣਤਰ dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ਨਿਰਾ);

	argc = parse_options(argc, argv, ਅਗੇਤਰ, check_ignore_options,
			     check_ignore_usage, 0);

	ਜੇ (stdin_paths) {
		ਜੇ (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} ਨਹੀਂਤਾਂ {
		ਜੇ (nul_ਪਦ_ਸਤਰ)
			die(_("-z only makes sense with --stdin"));
		ਜੇ (argc == 0)
			die(_("no path specified"));
	}
	ਜੇ (quiet) {
		ਜੇ (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		ਜੇ (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	ਜੇ (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	ਜੇ (!no_ਸੂਚਕਾਂਕ && repo_read_ਸੂਚਕਾਂਕ(the_repository) < 0)
		die(_("index file corrupt"));

	ਤਿਆਰੀ_standard_excludes(&dir);

	ਜੇ (stdin_paths) {
		ਅੰਕ_ignored = check_ignore_stdin_paths(&dir, ਅਗੇਤਰ);
	} ਨਹੀਂਤਾਂ {
		ਅੰਕ_ignored = check_ignore(&dir, ਅਗੇਤਰ, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	ਵਾਪਸ !ਅੰਕ_ignored;
}
