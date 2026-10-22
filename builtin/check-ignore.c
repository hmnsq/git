#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساكن صحيح quiet, verbose, stdin_paths, show_non_matching, no_اشاریہ;
ساكن ثابت حرف * ثابت check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
عدم
};

ساكن صحيح nul_رکن_سطر;

ساكن ثابت بنية option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروہ(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', عدم, &nul_رکن_سطر,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_اشاریہ,
		 N_("ignore index when checking")),
	OPT_سرا()
};

ساكن فراغ output_نمونہ(ثابت حرف *رستہ, بنية رستہ_نمونہ *نمونہ)
{
	ثابت حرف *bang  = (نمونہ && نمونہ->flags & نمونہ_FLAG_NEGATIVE)  ? "!" : "";
	ثابت حرف *slash = (نمونہ && نمونہ->flags & نمونہ_FLAG_MUSTBEDIR) ? "/" : "";
	إذا (!nul_رکن_سطر) {
		إذا (!verbose) {
			write_name_quoted(رستہ, stdout, '\n');
		} وإلا {
			إذا (نمونہ) {
				quote_c_style(نمونہ->pl->src, عدم, stdout, 0);
				لکھو(":%d:%s%s%s\t",
				       نمونہ->srcpos,
				       bang, نمونہ->نمونہ, slash);
			}
			وإلا {
				لکھو("::\t");
			}
			quote_c_style(رستہ, عدم, stdout, 0);
			fputc('\n', stdout);
		}
	} وإلا {
		إذا (!verbose) {
			لکھو("%s%c", رستہ, '\0');
		} وإلا {
			إذا (نمونہ)
				لکھو("%s%c%d%c%s%s%s%c%s%c",
				       نمونہ->pl->src, '\0',
				       نمونہ->srcpos, '\0',
				       bang, نمونہ->نمونہ, slash, '\0',
				       رستہ, '\0');
			وإلا
				لکھو("%c%c%c%s%c", '\0', '\0', '\0', رستہ, '\0');
		}
	}
}

ساكن صحيح check_ignore(بنية dir_struct *dir,
			ثابت حرف *اگیتر, صحيح argc, ثابت حرف **argv)
{
	ثابت حرف *بھریا_رستہ;
	حرف *تکیا;
	صحيح عدد_ignored = 0, i;
	بنية رستہ_نمونہ *نمونہ;
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
		       PATHSPEC_SYMLINK_LEADING_رستہ |
		       PATHSPEC_KEEP_ترتیب,
		       اگیتر, argv);

	die_رستہ_inside_submodule(the_repository->اشاریہ, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	تکیا = لبھو_pathspecs_matching_against_اشاریہ(&pathspec, the_repository->اشاریہ,
						     PS_HEED_SKIP_WORKTREE);
	لكل (i = 0; i < pathspec.nr; i++) {
		بھریا_رستہ = pathspec.اشیاء[i].میل;
		نمونہ = عدم;
		إذا (!تکیا[i]) {
			صحيح dtype = DT_UNKNOWN;
			نمونہ = آخری_matching_نمونہ(dir, the_repository->اشاریہ,
							بھریا_رستہ, &dtype);
			إذا (!verbose && نمونہ &&
			    نمونہ->flags & نمونہ_FLAG_NEGATIVE)
				نمونہ = عدم;
		}
		إذا (!quiet && (نمونہ || show_non_matching))
			output_نمونہ(pathspec.اشیاء[i].original, نمونہ);
		إذا (نمونہ)
			عدد_ignored++;
	}
	حرر(تکیا);
	clear_pathspec(&pathspec);

	أرجع عدد_ignored;
}

ساكن صحيح check_ignore_stdin_paths(بنية dir_struct *dir, ثابت حرف *اگیتر)
{
	بنية strbuf buf = STRBUF_INIT;
	بنية strbuf unquoted = STRBUF_INIT;
	حرف *pathspec[2] = { عدم, عدم };
	strbuf_getline_fn getline_fn;
	صحيح عدد_ignored = 0;

	getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	طالما (getline_fn(&buf, stdin) != EOF) {
		إذا (!nul_رکن_سطر && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			إذا (unquote_c_style(&unquoted, buf.buf, عدم))
				die("line is badly quoted");
			strbuf_وٹاندرا(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		عدد_ignored += check_ignore(dir, اگیتر,
					    1, (ثابت حرف **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	أرجع عدد_ignored;
}

صحيح cmd_check_ignore(صحيح argc,
		     ثابت حرف **argv,
		     ثابت حرف *اگیتر,
		     بنية repository *repo بن_ورتیا)
{
	صحيح عدد_ignored;
	بنية dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, عدم);

	argc = parse_options(argc, argv, اگیتر, check_ignore_options,
			     check_ignore_usage, 0);

	إذا (stdin_paths) {
		إذا (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} وإلا {
		إذا (nul_رکن_سطر)
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
	إذا (!no_اشاریہ && repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	تیاری_standard_excludes(&dir);

	إذا (stdin_paths) {
		عدد_ignored = check_ignore_stdin_paths(&dir, اگیتر);
	} وإلا {
		عدد_ignored = check_ignore(&dir, اگیتر, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	أرجع !عدد_ignored;
}
