/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#تعریف_کرو DISABLE_SIGN_COMPARE_WARNINGS

#شامل "builtin.h"
#شامل "config.h"
#شامل "environment.h"
#شامل "gettext.h"
#شامل "lockfile.h"
#شامل "quote.h"
#شامل "cache-tree.h"
#شامل "parse-options.h"
#شامل "entry.h"
#شامل "parallel-checkout.h"
#شامل "read-cache-ll.h"
#شامل "setup.h"
#شامل "sparse-index.h"

#تعریف_کرو CHECKOUT_ALL 4
ساکن صحیح nul_رکن_سطر;
ساکن صحیح checkout_stage; /* default to checkout stage0 */
ساکن صحیح ignore_skip_worktree; /* default to 0 */
ساکن صحیح مقصد_tempfile = -1;
ساکن حرف topath[4][TEMPORARY_FILENAME_لمبائی + 1];

ساکن ساخت checkout state = CHECKOUT_INIT;

ساکن خالی write_tempfile_ریکارڈ(ثابت حرف *name, ثابت حرف *سابقہ)
{
	صحیح i;
	صحیح have_tempname = 0;

	اگر (CHECKOUT_ALL == checkout_stage) {
		برائے (i = 1; i < 4; i++)
			اگر (topath[i][0]) {
				have_tempname = 1;
				توڑو;
			}

		اگر (have_tempname) {
			برائے (i = 1; i < 4; i++) {
				اگر (i > 1)
					حرف_لکھو(' ');
				اگر (topath[i][0])
					fputs(topath[i], stdout);
				ورنہ
					حرف_لکھو('.');
			}
		}
	} ورنہ اگر (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	اگر (have_tempname) {
		حرف_لکھو('\t');
		write_name_quoted_relative(name, سابقہ, stdout,
					   nul_رکن_سطر ? '\0' : '\n');
	}

	برائے (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ساکن صحیح checkout_file(ساخت اشاریہ_state *اشاریہ, ثابت حرف *name, ثابت حرف *سابقہ)
{
	صحیح namelen = لڑی_لمبائی(name);
	صحیح pos = اشاریہ_name_pos(اشاریہ, name, namelen);
	صحیح has_same_name = 0;
	صحیح is_file = 0;
	صحیح is_skipped = 1;
	صحیح did_checkout = 0;
	صحیح errs = 0;

	اگر (pos < 0)
		pos = -pos - 1;

	جب (pos < اشاریہ->cache_nr) {
		ساخت cache_اندراج *ce = اشاریہ->cache[pos];
		اگر (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			توڑو;
		has_same_name = 1;
		pos++;
		اگر (S_ISSPARSEDIR(ce->ce_منوال))
			توڑو;
		is_file = 1;
		اگر (!ignore_skip_worktree && ce_skip_worktree(ce))
			توڑو;
		is_skipped = 0;
		اگر (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			جاری;
		did_checkout = 1;
		اگر (checkout_اندراج(ce, &state,
				   مقصد_tempfile ? topath[ce_stage(ce)] : تہی,
				   تہی) < 0)
			errs++;
	}

	اگر (did_checkout) {
		اگر (مقصد_tempfile)
			write_tempfile_ریکارڈ(name, سابقہ);
		واپس errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	اگر (has_same_name && checkout_stage == CHECKOUT_ALL)
		واپس 0;

	اگر (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		اگر (!has_same_name)
			fprintf(stderr, "is not in the cache");
		ورنہ اگر (!is_file)
			fprintf(stderr, "is a sparse directory");
		ورنہ اگر (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		ورنہ اگر (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		ورنہ
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	واپس -1;
}

ساکن صحیح checkout_all(ساخت اشاریہ_state *اشاریہ, ثابت حرف *سابقہ, صحیح سابقہ_لمبائی)
{
	صحیح i, errs = 0;
	ساخت cache_اندراج *آخری_ce = تہی;

	برائے (i = 0; i < اشاریہ->cache_nr ; i++) {
		ساخت cache_اندراج *ce = اشاریہ->cache[i];

		اگر (S_ISSPARSEDIR(ce->ce_منوال)) {
			اگر (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			اگر (ignore_skip_worktree) {
				ensure_بھرا_اشاریہ(اشاریہ);
				ce = اشاریہ->cache[i];
			}
		}

		اگر (!ignore_skip_worktree && ce_skip_worktree(ce))
			جاری;
		اگر (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			جاری;
		اگر (سابقہ && *سابقہ &&
		    (ce_namelen(ce) <= سابقہ_لمبائی ||
		     memcmp(سابقہ, ce->name, سابقہ_لمبائی)))
			جاری;
		اگر (آخری_ce && مقصد_tempfile) {
			اگر (ce_namelen(آخری_ce) != ce_namelen(ce)
			    || memcmp(آخری_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_ریکارڈ(آخری_ce->name, سابقہ);
		}
		اگر (checkout_اندراج(ce, &state,
				   مقصد_tempfile ? topath[ce_stage(ce)] : تہی,
				   تہی) < 0)
			errs++;
		آخری_ce = ce;
	}
	اگر (آخری_ce && مقصد_tempfile)
		write_tempfile_ریکارڈ(آخری_ce->name, سابقہ);
	واپس !!errs;
}

ساکن ثابت حرف * ثابت builtin_checkout_اشاریہ_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	تہی
};

ساکن صحیح option_parse_stage(ثابت ساخت option *opt,
			      ثابت حرف *arg, صحیح unset)
{
	صحیح *stage = opt->قدر;

	BUG_ON_OPT_NEG(unset);

	اگر (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} ورنہ {
		صحیح ch = arg[0];
		اگر ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		ورنہ
			die(_("stage should be between 1 and 3 or all"));
	}
	واپس 0;
}

صحیح cmd_checkout_اشاریہ(صحیح argc,
		       ثابت حرف **argv,
		       ثابت حرف *سابقہ,
		       ساخت repository *repo)
{
	صحیح i;
	ساخت تالا_file تالا_file = تالا_INIT;
	صحیح all = 0;
	صحیح read_from_stdin = 0;
	صحیح سابقہ_لمبائی;
	صحیح force = 0, quiet = 0, not_new = 0;
	صحیح اشاریہ_opt = 0;
	صحیح err = 0;
	صحیح pc_workers, pc_حد;
	ساخت option builtin_checkout_اشاریہ_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &اشاریہ_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', تہی, &nul_رکن_سطر,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &مقصد_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.بنیاد_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_انتہا()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_اشاریہ_usage,
					 builtin_checkout_اشاریہ_options);
	repo_config(repo, git_default_config, تہی);
	سابقہ_لمبائی = سابقہ ? لڑی_لمبائی(سابقہ) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_بھرا_اشاریہ = 0;

	اگر (repo_read_اشاریہ(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, سابقہ, builtin_checkout_اشاریہ_options,
			builtin_checkout_اشاریہ_usage, 0);
	state.istate = repo->اشاریہ;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	اگر (!state.بنیاد_dir)
		state.بنیاد_dir = "";
	state.بنیاد_dir_len = لڑی_لمبائی(state.بنیاد_dir);

	اگر (مقصد_tempfile < 0)
		مقصد_tempfile = (checkout_stage == CHECKOUT_ALL);
	اگر (!مقصد_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	اگر (اشاریہ_opt && !state.بنیاد_dir_len && !مقصد_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->اشاریہ;
		repo_hold_locked_اشاریہ(repo, &تالا_file,
				       تالا_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_حد);
	اگر (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	برائے (i = 0; i < argc; i++) {
		ثابت حرف *arg = argv[i];
		حرف *p;

		اگر (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		اگر (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = سابقہ_راستہ(repo, سابقہ, سابقہ_لمبائی, arg);
		err |= checkout_file(repo->اشاریہ, p, سابقہ);
		آزاد(p);
	}

	اگر (read_from_stdin) {
		ساخت strbuf buf = STRBUF_INIT;
		ساخت strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		اگر (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_رکن_سطر ? strbuf_getline_nul : strbuf_getline_lf;
		جب (getline_fn(&buf, stdin) != EOF) {
			حرف *p;
			اگر (!nul_رکن_سطر && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				اگر (unquote_c_style(&unquoted, buf.buf, تہی))
					die("line is badly quoted");
				strbuf_تبادلہ(&buf, &unquoted);
			}
			p = سابقہ_راستہ(repo, سابقہ, سابقہ_لمبائی, buf.buf);
			err |= checkout_file(repo->اشاریہ, p, سابقہ);
			آزاد(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	اگر (all)
		err |= checkout_all(repo->اشاریہ, سابقہ, سابقہ_لمبائی);

	اگر (pc_workers > 1)
		err |= تسلسل_parallel_checkout(&state, pc_workers, pc_حد,
					     تہی, تہی);

	اگر (err)
		واپس 1;

	اگر (is_تالا_file_locked(&تالا_file) &&
	    write_locked_اشاریہ(repo->اشاریہ, &تالا_file, COMMIT_تالا))
		die("Unable to write new index file");
	واپس 0;
}
