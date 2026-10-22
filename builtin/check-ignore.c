#تعریف_کرو USE_THE_REPOSITORY_VARIABLE
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

ساکن صحیح quiet, verbose, stdin_paths, show_non_matching, no_اشاریہ;
ساکن ثابت حرف * ثابت check_ignore_usage[] = {
"git check-ignore [<options>] <pathname>...",
"git check-ignore [<options>] --stdin",
تہی
};

ساکن صحیح nul_رکن_سطر;

ساکن ثابت ساخت option check_ignore_options[] = {
	OPT__QUIET(&quiet, N_("suppress progress reporting")),
	OPT__VERBOSE(&verbose, N_("be verbose")),
	OPT_گروہ(""),
	OPT_BOOL(0, "stdin", &stdin_paths,
		 N_("read file names from stdin")),
	OPT_BOOL('z', تہی, &nul_رکن_سطر,
		 N_("terminate input and output records by a NUL character")),
	OPT_BOOL('n', "non-matching", &show_non_matching,
		 N_("show non-matching input paths")),
	OPT_BOOL(0, "no-index", &no_اشاریہ,
		 N_("ignore index when checking")),
	OPT_انتہا()
};

ساکن خالی output_نمونہ(ثابت حرف *راستہ, ساخت راستہ_نمونہ *نمونہ)
{
	ثابت حرف *bang  = (نمونہ && نمونہ->flags & نمونہ_FLAG_NEGATIVE)  ? "!" : "";
	ثابت حرف *slash = (نمونہ && نمونہ->flags & نمونہ_FLAG_MUSTBEDIR) ? "/" : "";
	اگر (!nul_رکن_سطر) {
		اگر (!verbose) {
			write_name_quoted(راستہ, stdout, '\n');
		} ورنہ {
			اگر (نمونہ) {
				quote_c_style(نمونہ->pl->src, تہی, stdout, 0);
				لکھو(":%d:%s%s%s\t",
				       نمونہ->srcpos,
				       bang, نمونہ->نمونہ, slash);
			}
			ورنہ {
				لکھو("::\t");
			}
			quote_c_style(راستہ, تہی, stdout, 0);
			fputc('\n', stdout);
		}
	} ورنہ {
		اگر (!verbose) {
			لکھو("%s%c", راستہ, '\0');
		} ورنہ {
			اگر (نمونہ)
				لکھو("%s%c%d%c%s%s%s%c%s%c",
				       نمونہ->pl->src, '\0',
				       نمونہ->srcpos, '\0',
				       bang, نمونہ->نمونہ, slash, '\0',
				       راستہ, '\0');
			ورنہ
				لکھو("%c%c%c%s%c", '\0', '\0', '\0', راستہ, '\0');
		}
	}
}

ساکن صحیح check_ignore(ساخت dir_struct *dir,
			ثابت حرف *سابقہ, صحیح argc, ثابت حرف **argv)
{
	ثابت حرف *بھرا_راستہ;
	حرف *نظر_آیا;
	صحیح عدد_ignored = 0, i;
	ساخت راستہ_نمونہ *نمونہ;
	ساخت pathspec pathspec;

	اگر (!argc) {
		اگر (!quiet)
			fprintf(stderr, "no pathspec given.\n");
		واپس 0;
	}

	/*
	 * check-ignore just needs paths. Magic beyond :/ is really
	 * irrelevant.
	 */
	parse_pathspec(&pathspec,
		       PATHSPEC_ALL_MAGIC & ~PATHSPEC_FROMTOP,
		       PATHSPEC_SYMLINK_LEADING_راستہ |
		       PATHSPEC_KEEP_ترتیب,
		       سابقہ, argv);

	die_راستہ_inside_submodule(the_repository->اشاریہ, &pathspec);

	/*
	 * look for pathspecs matching entries in the index, since these
	 * should not be ignored, in order to be consistent with
	 * 'git status', 'git add' etc.
	 */
	نظر_آیا = ڈھونڈو_pathspecs_matching_against_اشاریہ(&pathspec, the_repository->اشاریہ,
						     PS_HEED_SKIP_WORKTREE);
	برائے (i = 0; i < pathspec.nr; i++) {
		بھرا_راستہ = pathspec.اشیاء[i].مطابقت;
		نمونہ = تہی;
		اگر (!نظر_آیا[i]) {
			صحیح dtype = DT_UNKNOWN;
			نمونہ = آخری_matching_نمونہ(dir, the_repository->اشاریہ,
							بھرا_راستہ, &dtype);
			اگر (!verbose && نمونہ &&
			    نمونہ->flags & نمونہ_FLAG_NEGATIVE)
				نمونہ = تہی;
		}
		اگر (!quiet && (نمونہ || show_non_matching))
			output_نمونہ(pathspec.اشیاء[i].original, نمونہ);
		اگر (نمونہ)
			عدد_ignored++;
	}
	آزاد(نظر_آیا);
	clear_pathspec(&pathspec);

	واپس عدد_ignored;
}

ساکن صحیح check_ignore_stdin_paths(ساخت dir_struct *dir, ثابت حرف *سابقہ)
{
	ساخت strbuf buf = STRBUF_INIT;
	ساخت strbuf unquoted = STRBUF_INIT;
	حرف *pathspec[2] = { تہی, تہی };
	strbuf_getline_fn getline_fn;
	صحیح عدد_ignored = 0;

	getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
	جب (getline_fn(&buf, stdin) != EOF) {
		اگر (!nul_رکن_سطر && buf.buf[0] == '"') {
			strbuf_reset(&unquoted);
			اگر (unquote_c_style(&unquoted, buf.buf, تہی))
				die("line is badly quoted");
			strbuf_تبادلہ(&buf, &unquoted);
		}
		pathspec[0] = buf.buf;
		عدد_ignored += check_ignore(dir, سابقہ,
					    1, (ثابت حرف **)pathspec);
		maybe_flush_or_die(stdout, "check-ignore to stdout");
	}
	strbuf_release(&buf);
	strbuf_release(&unquoted);
	واپس عدد_ignored;
}

صحیح cmd_check_ignore(صحیح argc,
		     ثابت حرف **argv,
		     ثابت حرف *سابقہ,
		     ساخت repository *repo غیر_مستعمل)
{
	صحیح عدد_ignored;
	ساخت dir_struct dir = DIR_INIT;

	repo_config(the_repository, git_default_config, تہی);

	argc = parse_options(argc, argv, سابقہ, check_ignore_options,
			     check_ignore_usage, 0);

	اگر (stdin_paths) {
		اگر (argc > 0)
			die(_("cannot specify pathnames with --stdin"));
	} ورنہ {
		اگر (nul_رکن_سطر)
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
	اگر (!no_اشاریہ && repo_read_اشاریہ(the_repository) < 0)
		die(_("index file corrupt"));

	تیاری_standard_excludes(&dir);

	اگر (stdin_paths) {
		عدد_ignored = check_ignore_stdin_paths(&dir, سابقہ);
	} ورنہ {
		عدد_ignored = check_ignore(&dir, سابقہ, argc, argv);
		maybe_flush_or_die(stdout, "ignore to stdout");
	}

	dir_clear(&dir);

	واپس !عدد_ignored;
}
