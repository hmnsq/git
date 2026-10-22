#وصف_ڪريو USE_THE_REPOSITORY_VARIABLE
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

ساكن صحيح quiet, verbose, stdin_paths, show_non_matching, no_اشاريو;
ساكن ثابت حرف * ثابت check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
عدم
};

ساكن صحيح nul_رڪن_سٽ;

ساكن ثابت بنية option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروهه(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', عدم, &nul_رڪن_سٽ,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_اشاريو,
		 N_("ignore index when checking")),
	OPT_پڇاڙي()
};

ساكن فراغ output_نمونو(ثابت حرف *رستو, بنية رستو_نمونو *نمونو)
{
	ثابت حرف *bang  = (نمونو && نمونو->flags & نمونو_FLAG_NEGATIVE)  ? "!" : "";
	ثابت حرف *slash = (نمونو && نمونو->flags & نمونو_FLAG_MUSTBEDIR) ? "/" : "";
	إذا (!nul_رڪن_سٽ) {
		إذا (!verbose) {
			write_name_quoted(رستو, stdout, '\n');
		} وإلا {
			إذا (نمونو) {
				quote_c_style(نمونو->pl->src, عدم, stdout, 0);
				لکو(":%d:%s%s%s\t",
				       نمونو->srcpos,
				       bang, نمونو->نمونو, slash);
			}
			وإلا {
				لکو("::\t");
			}
			quote_c_style(رستو, عدم, stdout, 0);
			fputc('\n', stdout);
		}
	} وإلا {
		إذا (!verbose) {
			لکو("%s%c", رستو, '\0');
		} وإلا {
			إذا (نمونو)
				لکو("%s%c%d%c%s%s%s%c%s%c",
				       نمونو->pl->src, '\0',
				       نمونو->srcpos, '\0',
				       bang, نمونو->نمونو, slash, '\0',
				       رستو, '\0');
			وإلا
				لکو("%c%c%c%s%c", '\0', '\0', '\0', رستو, '\0');
		}
	}
}

ساكن صحيح check_ignore(بنية dir_struct *dir,
			ثابت حرف *اڳياڙي, صحيح argc, ثابت حرف **argv)
{
	ثابت حرف *ڀريل_رستو;
	حرف *ڏٺل;
	صحيح انگ_ignored = 0, i;
	بنية رستو_نمونو *نمونو;
	بنية pathspec pathspec;

	إذا (!argc) {
		إذا (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		أرجع 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_رستو |
		       PATHSPEC_KEEP_ترتيب,
		       اڳياڙي, argv);

	die_رستو_inside_submodule(the_repository->اشاريو, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	ڏٺل = ڳوليو_pathspecs_matching_against_اشاريو(&pathspec, the_repository->اشاريو,
						     PS_HEED_SKIP_WORKTREE);
	لكل (i = 0; i < pathspec.nr; i++) {
		ڀريل_رستو = pathspec.شيون[i].ميلاپ;
		نمونو = عدم;
		إذا (!ڏٺل[i]) {
			صحيح dtype = DT_UNKNOWN;
			نمونو = آخري_matching_نمونو(dir, the_repository->اشاريو,
							ڀريل_رستو, &dtype);
			إذا (!verbose && نمونو &&
			    نمونو->flags & نمونو_FLAG_NEGATIVE)
				نمونو = عدم;
		}
		إذا (!quiet && (نمونو || show_non_matching))
			output_نمونو(pathspec.شيون[i].original, نمونو);
		إذا (نمونو)
			انگ_ignored++;
	}
	حرر(ڏٺل);
	clear_pathspec(&pathspec);

	أرجع انگ_ignored;
}

ساكن صحيح check_ignore_stdin_paths(بنية dir_struct *dir, ثابت حرف *اڳياڙي)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *pathspec[2] = { عدم, عدم };
	strbuf_getline_fn getline_fn;
	صحيح انگ_ignored = 0;

	getline_fn = nul_رڪن_سٽ ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		إذا (!nul_رڪن_سٽ && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, buf.buf, عدم))
				die("line is badly quoted");
			strbuf_مٽا_سٽا(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		انگ_ignored += check_ignore(dir, اڳياڙي,
					    1, (ثابت حرف **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	أرجع انگ_ignored;
}

صحيح cmd_check_ignore(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اڳياڙي,
		     بنية repository *repo اڻ_استعمال_ٿيل)
{
	صحيح انگ_ignored;
	بنية dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اڳياڙي, check_ignore_options,
			     check_ignore_usage, 0);

	إذا (stdin_paths) {
		إذا (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} وإلا {
		إذا (nul_رڪن_سٽ)
			die(_("-z only makes sense with --stdin"));
		إذا (argc == 0)
			die(_("no path specified"));
	}
	إذا (quiet) {
		إذا (argc > 1)
			die(_("--quiet is only valid with a single pathname"));
		إذا (verbose)
			die(_("cannot have both --quiet and --verbose"));
	}
	إذا (show_non_matching && !verbose)
		die(_("--non-matching is only valid with --verbose"));

	/* read_cache() is only necessary so we can watch out for submodules. */
	إذا (!no_اشاريو && repo_read_اشاريو(the_repository) < 0)
		die(_("index file corrupt"));

	تياري_standard_excludes(&dir);

	إذا (stdin_paths) {
		انگ_ignored = check_ignore_stdin_paths(&dir, اڳياڙي);
	} وإلا {
		انگ_ignored = check_ignore(&dir, اڳياڙي, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	أرجع !انگ_ignored;
}
