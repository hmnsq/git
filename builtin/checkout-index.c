/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#تعریف_کن DISABLE_SIGN_COMPARE_WARNINGS

#ضمن "builtin.h"
#ضمن "config.h"
#ضمن "environment.h"
#ضمن "gettext.h"
#ضمن "lockfile.h"
#ضمن "quote.h"
#ضمن "cache-tree.h"
#ضمن "parse-options.h"
#ضمن "entry.h"
#ضمن "parallel-checkout.h"
#ضمن "read-cache-ll.h"
#ضمن "setup.h"
#ضمن "sparse-index.h"

#تعریف_کن CHECKOUT_ALL 4
ساكن صحيح nul_جمله_سطر;
ساكن صحيح checkout_stage; /* default to checkout stage0 */
ساكن صحيح ignore_skip_worktree; /* default to 0 */
ساكن صحيح مقصد_tempfile = -1;
ساكن حرف topath[4][TEMPORARY_FILENAME_درازی + 1];

ساكن بنية checkout state = CHECKOUT_INIT;

ساكن فراغ write_tempfile_رکورد(ثابت حرف *name, ثابت حرف *پیشوند)
{
	صحيح i;
	صحيح have_tempname = 0;

	إذا (CHECKOUT_ALL == checkout_stage) {
		لكل (i = 1; i < 4; i++)
			إذا (topath[i][0]) {
				have_tempname = 1;
				اكسر;
			}

		إذا (have_tempname) {
			لكل (i = 1; i < 4; i++) {
				إذا (i > 1)
					حرف_چاپ_کن(' ');
				إذا (topath[i][0])
					fputs(topath[i], stdout);
				وإلا
					حرف_چاپ_کن('.');
			}
		}
	} وإلا إذا (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	إذا (have_tempname) {
		حرف_چاپ_کن('\t');
		write_name_quoted_relative(name, پیشوند, stdout,
					   nul_جمله_سطر ? '\0' : '\n');
	}

	لكل (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ساكن صحيح checkout_file(بنية شاخص_state *شاخص, ثابت حرف *name, ثابت حرف *پیشوند)
{
	صحيح namelen = طول_رشته(name);
	صحيح pos = شاخص_name_pos(شاخص, name, namelen);
	صحيح has_same_name = 0;
	صحيح is_file = 0;
	صحيح is_skipped = 1;
	صحيح did_checkout = 0;
	صحيح errs = 0;

	إذا (pos < 0)
		pos = -pos - 1;

	طالما (pos < شاخص->cache_nr) {
		بنية cache_مدخل *ce = شاخص->cache[pos];
		إذا (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			اكسر;
		has_same_name = 1;
		pos++;
		إذا (S_ISSPARSEDIR(ce->ce_نما))
			اكسر;
		is_file = 1;
		إذا (!ignore_skip_worktree && ce_skip_worktree(ce))
			اكسر;
		is_skipped = 0;
		إذا (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			استمر;
		did_checkout = 1;
		إذا (checkout_مدخل(ce, &state,
				   مقصد_tempfile ? topath[ce_stage(ce)] : عدم,
				   عدم) < 0)
			errs++;
	}

	إذا (did_checkout) {
		إذا (مقصد_tempfile)
			write_tempfile_رکورد(name, پیشوند);
		أرجع errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	إذا (has_same_name && checkout_stage == CHECKOUT_ALL)
		أرجع 0;

	إذا (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		إذا (!has_same_name)
			fprintf(stderr, "is not in the cache");
		وإلا إذا (!is_file)
			fprintf(stderr, "is a sparse directory");
		وإلا إذا (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		وإلا إذا (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		وإلا
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	أرجع -1;
}

ساكن صحيح checkout_all(بنية شاخص_state *شاخص, ثابت حرف *پیشوند, صحيح پیشوند_درازی)
{
	صحيح i, errs = 0;
	بنية cache_مدخل *آخرین_ce = عدم;

	لكل (i = 0; i < شاخص->cache_nr ; i++) {
		بنية cache_مدخل *ce = شاخص->cache[i];

		إذا (S_ISSPARSEDIR(ce->ce_نما)) {
			إذا (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			إذا (ignore_skip_worktree) {
				ensure_پر_شاخص(شاخص);
				ce = شاخص->cache[i];
			}
		}

		إذا (!ignore_skip_worktree && ce_skip_worktree(ce))
			استمر;
		إذا (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			استمر;
		إذا (پیشوند && *پیشوند &&
		    (ce_namelen(ce) <= پیشوند_درازی ||
		     memcmp(پیشوند, ce->name, پیشوند_درازی)))
			استمر;
		إذا (آخرین_ce && مقصد_tempfile) {
			إذا (ce_namelen(آخرین_ce) != ce_namelen(ce)
			    || memcmp(آخرین_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_رکورد(آخرین_ce->name, پیشوند);
		}
		إذا (checkout_مدخل(ce, &state,
				   مقصد_tempfile ? topath[ce_stage(ce)] : عدم,
				   عدم) < 0)
			errs++;
		آخرین_ce = ce;
	}
	إذا (آخرین_ce && مقصد_tempfile)
		write_tempfile_رکورد(آخرین_ce->name, پیشوند);
	أرجع !!errs;
}

ساكن ثابت حرف * ثابت builtin_checkout_شاخص_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	عدم
};

ساكن صحيح option_parse_stage(ثابت بنية option *opt,
			      ثابت حرف *arg, صحيح unset)
{
	صحيح *stage = opt->مقدار;

	BUG_ON_OPT_NEG(unset);

	إذا (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} وإلا {
		صحيح ch = arg[0];
		إذا ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		وإلا
			die(_("stage should be between 1 and 3 or all"));
	}
	أرجع 0;
}

صحيح cmd_checkout_شاخص(صحيح argc,
		       ثابت حرف **argv,
		       ثابت حرف *پیشوند,
		       بنية repository *repo)
{
	صحيح i;
	بنية قفل_file قفل_file = قفل_INIT;
	صحيح all = 0;
	صحيح read_from_stdin = 0;
	صحيح پیشوند_درازی;
	صحيح force = 0, quiet = 0, not_new = 0;
	صحيح شاخص_opt = 0;
	صحيح err = 0;
	صحيح pc_workers, pc_آستانه;
	بنية option builtin_checkout_شاخص_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &شاخص_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', عدم, &nul_جمله_سطر,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &مقصد_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.پایه_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_انتها()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_شاخص_usage,
					 builtin_checkout_شاخص_options);
	repo_config(repo, git_default_config, عدم);
	پیشوند_درازی = پیشوند ? طول_رشته(پیشوند) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_پر_شاخص = 0;

	إذا (repo_read_شاخص(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, پیشوند, builtin_checkout_شاخص_options,
			builtin_checkout_شاخص_usage, 0);
	state.istate = repo->شاخص;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	إذا (!state.پایه_dir)
		state.پایه_dir = "";
	state.پایه_dir_len = طول_رشته(state.پایه_dir);

	إذا (مقصد_tempfile < 0)
		مقصد_tempfile = (checkout_stage == CHECKOUT_ALL);
	إذا (!مقصد_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	إذا (شاخص_opt && !state.پایه_dir_len && !مقصد_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->شاخص;
		repo_hold_locked_شاخص(repo, &قفل_file,
				       قفل_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_آستانه);
	إذا (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	لكل (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		حرف *p;

		إذا (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		إذا (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = پیشوند_مسیر(repo, پیشوند, پیشوند_درازی, arg);
		err |= checkout_file(repo->شاخص, p, پیشوند);
		حرر(p);
	}

	إذا (read_from_stdin) {
		بنية strbuf buf = STRBUF_INIT;
		بنية strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		إذا (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_جمله_سطر ? strbuf_getline_nul : strbuf_getline_lf;
		طالما (getline_fn(&buf, stdin) != EOF) {
			حرف *p;
			إذا (!nul_جمله_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				إذا (unquote_c_style(&unquoted, buf.buf, عدم))
					die("line is badly quoted");
				strbuf_جابجا(&buf, &unquoted);
			}
			p = پیشوند_مسیر(repo, پیشوند, پیشوند_درازی, buf.buf);
			err |= checkout_file(repo->شاخص, p, پیشوند);
			حرر(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	إذا (all)
		err |= checkout_all(repo->شاخص, پیشوند, پیشوند_درازی);

	إذا (pc_workers > 1)
		err |= پیاپی_parallel_checkout(&state, pc_workers, pc_آستانه,
					     عدم, عدم);

	إذا (err)
		أرجع 1;

	إذا (is_قفل_file_locked(&قفل_file) &&
	    write_locked_شاخص(repo->شاخص, &قفل_file, COMMIT_قفل))
		die("Unable to write new index file");
	أرجع 0;
}
