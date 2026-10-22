#تعریف_کن USE_THE_REPOSITORY_VARIABLE
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

ساكن صحيح quiet, verbose, stdin_paths, show_non_matching, no_شاخص;
ساكن ثابت حرف * ثابت check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
عدم
};

ساكن صحيح nul_جمله_سطر;

ساكن ثابت بنية option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروه(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', عدم, &nul_جمله_سطر,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_شاخص,
		 N_("ignore index when checking")),
	OPT_انتها()
};

ساكن فراغ output_الگو(ثابت حرف *مسیر, بنية مسیر_الگو *الگو)
{
	ثابت حرف *bang  = (الگو && الگو->flags & الگو_FLAG_NEGATIVE)  ? "!" : "";
	ثابت حرف *slash = (الگو && الگو->flags & الگو_FLAG_MUSTBEDIR) ? "/" : "";
	إذا (!nul_جمله_سطر) {
		إذا (!verbose) {
			write_name_quoted(مسیر, stdout, '\n');
		} وإلا {
			إذا (الگو) {
				quote_c_style(الگو->pl->src, عدم, stdout, 0);
				چاپ_کن(":%d:%s%s%s\t",
				       الگو->srcpos,
				       bang, الگو->الگو, slash);
			}
			وإلا {
				چاپ_کن("::\t");
			}
			quote_c_style(مسیر, عدم, stdout, 0);
			fputc('\n', stdout);
		}
	} وإلا {
		إذا (!verbose) {
			چاپ_کن("%s%c", مسیر, '\0');
		} وإلا {
			إذا (الگو)
				چاپ_کن("%s%c%d%c%s%s%s%c%s%c",
				       الگو->pl->src, '\0',
				       الگو->srcpos, '\0',
				       bang, الگو->الگو, slash, '\0',
				       مسیر, '\0');
			وإلا
				چاپ_کن("%c%c%c%s%c", '\0', '\0', '\0', مسیر, '\0');
		}
	}
}

ساكن صحيح check_ignore(بنية dir_struct *dir,
			ثابت حرف *پیشوند, صحيح argc, ثابت حرف **argv)
{
	ثابت حرف *پر_مسیر;
	حرف *دیده;
	صحيح رقم_ignored = 0, i;
	بنية مسیر_الگو *الگو;
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
		       PATHSPEC_SYMLINK_LEADING_مسیر |
		       PATHSPEC_KEEP_ترتیب,
		       پیشوند, argv);

	die_مسیر_inside_submodule(the_repository->شاخص, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	دیده = پیدا_کن_pathspecs_matching_against_شاخص(&pathspec, the_repository->شاخص,
						     PS_HEED_SKIP_WORKTREE);
	لكل (i = 0; i < pathspec.nr; i++) {
		پر_مسیر = pathspec.اقلام[i].تطابق;
		الگو = عدم;
		إذا (!دیده[i]) {
			صحيح dtype = DT_UNKNOWN;
			الگو = آخرین_matching_الگو(dir, the_repository->شاخص,
							پر_مسیر, &dtype);
			إذا (!verbose && الگو &&
			    الگو->flags & الگو_FLAG_NEGATIVE)
				الگو = عدم;
		}
		إذا (!quiet && (الگو || show_non_matching))
			output_الگو(pathspec.اقلام[i].original, الگو);
		إذا (الگو)
			رقم_ignored++;
	}
	حرر(دیده);
	clear_pathspec(&pathspec);

	أرجع رقم_ignored;
}

ساكن صحيح check_ignore_stdin_paths(بنية dir_struct *dir, ثابت حرف *پیشوند)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *pathspec[2] = { عدم, عدم };
	strbuf_getline_fn getline_fn;
	صحيح رقم_ignored = 0;

	getline_fn = nul_جمله_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		إذا (!nul_جمله_سطر && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, buf.buf, عدم))
				die("line is badly quoted");
			strbuf_جابجا(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		رقم_ignored += check_ignore(dir, پیشوند,
					    1, (ثابت حرف **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	أرجع رقم_ignored;
}

صحيح cmd_check_ignore(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *پیشوند,
		     بنية repository *repo استفادهنشده)
{
	صحيح رقم_ignored;
	بنية dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, پیشوند, check_ignore_options,
			     check_ignore_usage, 0);

	إذا (stdin_paths) {
		إذا (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} وإلا {
		إذا (nul_جمله_سطر)
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
	إذا (!no_شاخص && repo_read_شاخص(the_repository) < 0)
		die(_("index file corrupt"));

	راهاندازی_standard_excludes(&dir);

	إذا (stdin_paths) {
		رقم_ignored = check_ignore_stdin_paths(&dir, پیشوند);
	} وإلا {
		رقم_ignored = check_ignore(&dir, پیشوند, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	أرجع !رقم_ignored;
}
