#تعریف_کړه USE_THE_REPOSITORY_VARIABLE
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

ساكن صحيح nul_غړی_کرښه;

ساكن ثابت بنية option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_ډله(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', عدم, &nul_غړی_کرښه,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_شاخص,
		 N_("ignore index when checking")),
	OPT_پای_ټکی()
};

ساكن فراغ output_نمونه(ثابت حرف *لار, بنية لار_نمونه *نمونه)
{
	ثابت حرف *bang  = (نمونه && نمونه->flags & نمونه_FLAG_NEGATIVE)  ? "!" : "";
	ثابت حرف *slash = (نمونه && نمونه->flags & نمونه_FLAG_MUSTBEDIR) ? "/" : "";
	إذا (!nul_غړی_کرښه) {
		إذا (!verbose) {
			write_name_quoted(لار, stdout, '\n');
		} وإلا {
			إذا (نمونه) {
				quote_c_style(نمونه->pl->src, عدم, stdout, 0);
				چاپ_کړه(":%d:%s%s%s\t",
				       نمونه->srcpos,
				       bang, نمونه->نمونه, slash);
			}
			وإلا {
				چاپ_کړه("::\t");
			}
			quote_c_style(لار, عدم, stdout, 0);
			fputc('\n', stdout);
		}
	} وإلا {
		إذا (!verbose) {
			چاپ_کړه("%s%c", لار, '\0');
		} وإلا {
			إذا (نمونه)
				چاپ_کړه("%s%c%d%c%s%s%s%c%s%c",
				       نمونه->pl->src, '\0',
				       نمونه->srcpos, '\0',
				       bang, نمونه->نمونه, slash, '\0',
				       لار, '\0');
			وإلا
				چاپ_کړه("%c%c%c%s%c", '\0', '\0', '\0', لار, '\0');
		}
	}
}

ساكن صحيح check_ignore(بنية dir_struct *dir,
			ثابت حرف *مختاړی, صحيح argc, ثابت حرف **argv)
{
	ثابت حرف *ډک_لار;
	حرف *کتل_شوی;
	صحيح عدد_ignored = 0, i;
	بنية لار_نمونه *نمونه;
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
		       PATHSPEC_SYMLINK_LEADING_لار |
		       PATHSPEC_KEEP_ترتیب,
		       مختاړی, argv);

	die_لار_inside_submodule(the_repository->شاخص, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	کتل_شوی = پیدا_کړه_pathspecs_matching_against_شاخص(&pathspec, the_repository->شاخص,
						     PS_HEED_SKIP_WORKTREE);
	لكل (i = 0; i < pathspec.nr; i++) {
		ډک_لار = pathspec.توکي[i].سمون;
		نمونه = عدم;
		إذا (!کتل_شوی[i]) {
			صحيح dtype = DT_UNKNOWN;
			نمونه = وروستی_matching_نمونه(dir, the_repository->شاخص,
							ډک_لار, &dtype);
			إذا (!verbose && نمونه &&
			    نمونه->flags & نمونه_FLAG_NEGATIVE)
				نمونه = عدم;
		}
		إذا (!quiet && (نمونه || show_non_matching))
			output_نمونه(pathspec.توکي[i].original, نمونه);
		إذا (نمونه)
			عدد_ignored++;
	}
	حرر(کتل_شوی);
	clear_pathspec(&pathspec);

	أرجع عدد_ignored;
}

ساكن صحيح check_ignore_stdin_paths(بنية dir_struct *dir, ثابت حرف *مختاړی)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *pathspec[2] = { عدم, عدم };
	strbuf_getline_fn getline_fn;
	صحيح عدد_ignored = 0;

	getline_fn = nul_غړی_کرښه ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		إذا (!nul_غړی_کرښه && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, buf.buf, عدم))
				die("line is badly quoted");
			strbuf_بدلون(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		عدد_ignored += check_ignore(dir, مختاړی,
					    1, (ثابت حرف **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	أرجع عدد_ignored;
}

صحيح cmd_check_ignore(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *مختاړی,
		     بنية repository *repo ناکارول_شوی)
{
	صحيح عدد_ignored;
	بنية dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, مختاړی, check_ignore_options,
			     check_ignore_usage, 0);

	إذا (stdin_paths) {
		إذا (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} وإلا {
		إذا (nul_غړی_کرښه)
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

	تیاری_standard_excludes(&dir);

	إذا (stdin_paths) {
		عدد_ignored = check_ignore_stdin_paths(&dir, مختاړی);
	} وإلا {
		عدد_ignored = check_ignore(&dir, مختاړی, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	أرجع !عدد_ignored;
}
