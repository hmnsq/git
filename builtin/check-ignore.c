#ವ್ಯಾಖ್ಯಾನಿಸು USE_THE_REPOSITORY_VARIABLE
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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ quiet, verbose, stdin_paths, show_non_matching, no_ಸೂಚ್ಯಂಕ;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nul_ಪದ_ಸಾಲು;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_ಗುಂಪು(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', ಖಾಲಿ, &nul_ಪದ_ಸಾಲು,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_ಸೂಚ್ಯಂಕ,
		 N_("ignore index when checking")),
	OPT_ಕೊನೆ()
};

ಸ್ಥಿರ ಶೂನ್ಯ output_ನಮೂನೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ರಚನೆ ಮಾರ್ಗ_ನಮೂನೆ *ನಮೂನೆ)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bang  = (ನಮೂನೆ && ನಮೂನೆ->flags & ನಮೂನೆ_FLAG_NEGATIVE)  ? "!" : "";
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *slash = (ನಮೂನೆ && ನಮೂನೆ->flags & ನಮೂನೆ_FLAG_MUSTBEDIR) ? "/" : "";
	ಆದರೆ (!nul_ಪದ_ಸಾಲು) {
		ಆದರೆ (!verbose) {
			write_name_quoted(ಮಾರ್ಗ, stdout, '\n');
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (ನಮೂನೆ) {
				quote_c_style(ನಮೂನೆ->pl->src, ಖಾಲಿ, stdout, 0);
				ಬರೆ(":%d:%s%s%s\t",
				       ನಮೂನೆ->srcpos,
				       bang, ನಮೂನೆ->ನಮೂನೆ, slash);
			}
			ಇಲ್ಲದಿದ್ದರೆ {
				ಬರೆ("::\t");
			}
			quote_c_style(ಮಾರ್ಗ, ಖಾಲಿ, stdout, 0);
			fputc('\n', stdout);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!verbose) {
			ಬರೆ("%s%c", ಮಾರ್ಗ, '\0');
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (ನಮೂನೆ)
				ಬರೆ("%s%c%d%c%s%s%s%c%s%c",
				       ನಮೂನೆ->pl->src, '\0',
				       ನಮೂನೆ->srcpos, '\0',
				       bang, ನಮೂನೆ->ನಮೂನೆ, slash, '\0',
				       ಮಾರ್ಗ, '\0');
			ಇಲ್ಲದಿದ್ದರೆ
				ಬರೆ("%c%c%c%s%c", '\0', '\0', '\0', ಮಾರ್ಗ, '\0');
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_ignore(ರಚನೆ dir_struct *dir,
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ತುಂಬಿದ_ಮಾರ್ಗ;
	ಅಕ್ಷರ *ನೋಡಿದ;
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ignored = 0, i;
	ರಚನೆ ಮಾರ್ಗ_ನಮೂನೆ *ನಮೂನೆ;
	ರಚನೆ pathspec pathspec;

	ಆದರೆ (!argc) {
		ಆದರೆ (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_ಮಾರ್ಗ |
		       PATHSPEC_KEEP_ಕ್ರಮ,
		       ಪೂರ್ವಪ್ರತ್ಯಯ, argv);

	die_ಮಾರ್ಗ_inside_submodule(the_repository->ಸೂಚ್ಯಂಕ, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	ನೋಡಿದ = ಕಂಡುಹಿಡಿ_pathspecs_matching_against_ಸೂಚ್ಯಂಕ(&pathspec, the_repository->ಸೂಚ್ಯಂಕ,
						     PS_HEED_SKIP_WORKTREE);
	ಗಾಗಿ (i = 0; i < pathspec.nr; i++) {
		ತುಂಬಿದ_ಮಾರ್ಗ = pathspec.ವಸ್ತುಗಳು[i].ಹೊಂದಿಕೆ;
		ನಮೂನೆ = ಖಾಲಿ;
		ಆದರೆ (!ನೋಡಿದ[i]) {
			ಪೂರ್ಣಾಂಕ dtype = DT_UNKNOWN;
			ನಮೂನೆ = ಕೊನೆಯ_matching_ನಮೂನೆ(dir, the_repository->ಸೂಚ್ಯಂಕ,
							ತುಂಬಿದ_ಮಾರ್ಗ, &dtype);
			ಆದರೆ (!verbose && ನಮೂನೆ &&
			    ನಮೂನೆ->flags & ನಮೂನೆ_FLAG_NEGATIVE)
				ನಮೂನೆ = ಖಾಲಿ;
		}
		ಆದರೆ (!quiet && (ನಮೂನೆ || show_non_matching))
			output_ನಮೂನೆ(pathspec.ವಸ್ತುಗಳು[i].original, ನಮೂನೆ);
		ಆದರೆ (ನಮೂನೆ)
			ಅಂಕೆ_ignored++;
	}
	ಬಿಡುಗಡೆ(ನೋಡಿದ);
	clear_pathspec(&pathspec);

	ಹಿಂತಿರುಗಿ ಅಂಕೆ_ignored;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_ignore_stdin_paths(ರಚನೆ dir_struct *dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ರಚನೆ strbuf buf = STRBUF_INIT;
	ರಚನೆ strbuf unquoted = STRBUF_INIT;
	ಅಕ್ಷರ *pathspec[2] = { ಖಾಲಿ, ಖಾಲಿ };
	strbuf_getline_fn getline_fn;
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ignored = 0;

	getline_fn = nul_ಪದ_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
	ಆಗ (getline_fn(&buf, stdin) != EOF) {
		ಆದರೆ (!nul_ಪದ_ಸಾಲು && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			ಆದರೆ (unquote_c_style(&unquoted, buf.buf, ಖಾಲಿ))
				die("line is badly quoted");
			strbuf_ಅದಲುಬದಲು(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		ಅಂಕೆ_ignored += check_ignore(dir, ಪೂರ್ವಪ್ರತ್ಯಯ,
					    1, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	ಹಿಂತಿರುಗಿ ಅಂಕೆ_ignored;
}

ಪೂರ್ಣಾಂಕ cmd_check_ignore(ಪೂರ್ಣಾಂಕ argc,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		     ರಚನೆ repository *repo ಬಳಸದ)
{
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ignored;
	ರಚನೆ dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, ಖಾಲಿ);

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, check_ignore_options,
			     check_ignore_usage, 0);

	ಆದರೆ (stdin_paths) {
		ಆದರೆ (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (nul_ಪದ_ಸಾಲು)
			die(_("-z only makes sense with --stdin"));
		ಆದರೆ (argc == 0)
			die(_("no path specified"));
	}
	ಆದರೆ (quiet) {
		ಆದರೆ (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		ಆದರೆ (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	ಆದರೆ (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	ಆದರೆ (!no_ಸೂಚ್ಯಂಕ && repo_read_ಸೂಚ್ಯಂಕ(the_repository) < 0)
		die(_("index file corrupt"));

	ಸಿದ್ಧತೆ_standard_excludes(&dir);

	ಆದರೆ (stdin_paths) {
		ಅಂಕೆ_ignored = check_ignore_stdin_paths(&dir, ಪೂರ್ವಪ್ರತ್ಯಯ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಅಂಕೆ_ignored = check_ignore(&dir, ಪೂರ್ವಪ್ರತ್ಯಯ, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	ಹಿಂತಿರುಗಿ !ಅಂಕೆ_ignored;
}
