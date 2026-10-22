/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISABLE_SIGN_COMPARE_WARNINGS

#ਅਨ੍ਤਰ੍ਭਾਵ "builtin.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "environment.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gettext.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "lockfile.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cache-tree.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "entry.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parallel-checkout.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-cache-ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "setup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sparse-index.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECKOUT_ALL 4
ਸਥਿਰ ਪੂਰਨ nul_ਪਦ_ਸਤਰ;
ਸਥਿਰ ਪੂਰਨ checkout_stage; /* default to checkout stage0 */
ਸਥਿਰ ਪੂਰਨ ignore_skip_worktree; /* default to 0 */
ਸਥਿਰ ਪੂਰਨ ਮੰਜ਼ਿਲ_tempfile = -1;
ਸਥਿਰ ਅੱਖਰ topath[4][TEMPORARY_FILENAME_ਲੰਮਾਈ + 1];

ਸਥਿਰ ਬਣਤਰ checkout state = CHECKOUT_INIT;

ਸਥਿਰ ਖਾਲੀ write_tempfile_ਰਿਕਾਰਡ(ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਪੂਰਨ i;
	ਪੂਰਨ have_tempname = 0;

	ਜੇ (CHECKOUT_ALL == checkout_stage) {
		ਲਈ (i = 1; i < 4; i++)
			ਜੇ (topath[i][0]) {
				have_tempname = 1;
				ਰੋਕੋ;
			}

		ਜੇ (have_tempname) {
			ਲਈ (i = 1; i < 4; i++) {
				ਜੇ (i > 1)
					ਅੱਖਰ_ਲਿਖੋ(' ');
				ਜੇ (topath[i][0])
					fputs(topath[i], stdout);
				ਨਹੀਂਤਾਂ
					ਅੱਖਰ_ਲਿਖੋ('.');
			}
		}
	} ਨਹੀਂਤਾਂ ਜੇ (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	ਜੇ (have_tempname) {
		ਅੱਖਰ_ਲਿਖੋ('\t');
		write_name_quoted_relative(name, ਅਗੇਤਰ, stdout,
					   nul_ਪਦ_ਸਤਰ ? '\0' : '\n');
	}

	ਲਈ (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ਸਥਿਰ ਪੂਰਨ checkout_file(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਪੂਰਨ namelen = ਲੜੀ_ਲੰਬਾਈ(name);
	ਪੂਰਨ pos = ਸੂਚਕਾਂਕ_name_pos(ਸੂਚਕਾਂਕ, name, namelen);
	ਪੂਰਨ has_same_name = 0;
	ਪੂਰਨ is_file = 0;
	ਪੂਰਨ is_skipped = 1;
	ਪੂਰਨ did_checkout = 0;
	ਪੂਰਨ errs = 0;

	ਜੇ (pos < 0)
		pos = -pos - 1;

	ਜਦੋਂ (pos < ਸੂਚਕਾਂਕ->cache_nr) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = ਸੂਚਕਾਂਕ->cache[pos];
		ਜੇ (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			ਰੋਕੋ;
		has_same_name = 1;
		pos++;
		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ))
			ਰੋਕੋ;
		is_file = 1;
		ਜੇ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ਰੋਕੋ;
		is_skipped = 0;
		ਜੇ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ਜਾਰੀ;
		did_checkout = 1;
		ਜੇ (checkout_ਇੰਦਰਾਜ(ce, &state,
				   ਮੰਜ਼ਿਲ_tempfile ? topath[ce_stage(ce)] : ਨਿਰਾ,
				   ਨਿਰਾ) < 0)
			errs++;
	}

	ਜੇ (did_checkout) {
		ਜੇ (ਮੰਜ਼ਿਲ_tempfile)
			write_tempfile_ਰਿਕਾਰਡ(name, ਅਗੇਤਰ);
		ਵਾਪਸ errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	ਜੇ (has_same_name && checkout_stage == CHECKOUT_ALL)
		ਵਾਪਸ 0;

	ਜੇ (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		ਜੇ (!has_same_name)
			fprintf(stderr, "is not in the cache");
		ਨਹੀਂਤਾਂ ਜੇ (!is_file)
			fprintf(stderr, "is a sparse directory");
		ਨਹੀਂਤਾਂ ਜੇ (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		ਨਹੀਂਤਾਂ ਜੇ (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		ਨਹੀਂਤਾਂ
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਪੂਰਨ checkout_all(ਬਣਤਰ ਸੂਚਕਾਂਕ_state *ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਪੂਰਨ ਅਗੇਤਰ_ਲੰਮਾਈ)
{
	ਪੂਰਨ i, errs = 0;
	ਬਣਤਰ cache_ਇੰਦਰਾਜ *ਆਖਰੀ_ce = ਨਿਰਾ;

	ਲਈ (i = 0; i < ਸੂਚਕਾਂਕ->cache_nr ; i++) {
		ਬਣਤਰ cache_ਇੰਦਰਾਜ *ce = ਸੂਚਕਾਂਕ->cache[i];

		ਜੇ (S_ISSPARSEDIR(ce->ce_ਬਹੁਲਕ)) {
			ਜੇ (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			ਜੇ (ignore_skip_worktree) {
				ensure_ਭਰਿਆ_ਸੂਚਕਾਂਕ(ਸੂਚਕਾਂਕ);
				ce = ਸੂਚਕਾਂਕ->cache[i];
			}
		}

		ਜੇ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ਜਾਰੀ;
		ਜੇ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ਜਾਰੀ;
		ਜੇ (ਅਗੇਤਰ && *ਅਗੇਤਰ &&
		    (ce_namelen(ce) <= ਅਗੇਤਰ_ਲੰਮਾਈ ||
		     memcmp(ਅਗੇਤਰ, ce->name, ਅਗੇਤਰ_ਲੰਮਾਈ)))
			ਜਾਰੀ;
		ਜੇ (ਆਖਰੀ_ce && ਮੰਜ਼ਿਲ_tempfile) {
			ਜੇ (ce_namelen(ਆਖਰੀ_ce) != ce_namelen(ce)
			    || memcmp(ਆਖਰੀ_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_ਰਿਕਾਰਡ(ਆਖਰੀ_ce->name, ਅਗੇਤਰ);
		}
		ਜੇ (checkout_ਇੰਦਰਾਜ(ce, &state,
				   ਮੰਜ਼ਿਲ_tempfile ? topath[ce_stage(ce)] : ਨਿਰਾ,
				   ਨਿਰਾ) < 0)
			errs++;
		ਆਖਰੀ_ce = ce;
	}
	ਜੇ (ਆਖਰੀ_ce && ਮੰਜ਼ਿਲ_tempfile)
		write_tempfile_ਰਿਕਾਰਡ(ਆਖਰੀ_ce->name, ਅਗੇਤਰ);
	ਵਾਪਸ !!errs;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ builtin_checkout_ਸੂਚਕਾਂਕ_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ਨਿਰਾ
};

ਸਥਿਰ ਪੂਰਨ option_parse_stage(ਅਚਲ ਬਣਤਰ option *opt,
			      ਅਚਲ ਅੱਖਰ *arg, ਪੂਰਨ unset)
{
	ਪੂਰਨ *stage = opt->ਮੁੱਲ;

	BUG_ON_OPT_NEG(unset);

	ਜੇ (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} ਨਹੀਂਤਾਂ {
		ਪੂਰਨ ch = arg[0];
		ਜੇ ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		ਨਹੀਂਤਾਂ
			die(_("stage should be between 1 and 3 or all"));
	}
	ਵਾਪਸ 0;
}

ਪੂਰਨ cmd_checkout_ਸੂਚਕਾਂਕ(ਪੂਰਨ argc,
		       ਅਚਲ ਅੱਖਰ **argv,
		       ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
		       ਬਣਤਰ repository *repo)
{
	ਪੂਰਨ i;
	ਬਣਤਰ ਤਾਲਾ_file ਤਾਲਾ_file = ਤਾਲਾ_INIT;
	ਪੂਰਨ all = 0;
	ਪੂਰਨ read_from_stdin = 0;
	ਪੂਰਨ ਅਗੇਤਰ_ਲੰਮਾਈ;
	ਪੂਰਨ force = 0, quiet = 0, not_new = 0;
	ਪੂਰਨ ਸੂਚਕਾਂਕ_opt = 0;
	ਪੂਰਨ err = 0;
	ਪੂਰਨ pc_workers, pc_ਹੱਦ;
	ਬਣਤਰ option builtin_checkout_ਸੂਚਕਾਂਕ_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &ਸੂਚਕਾਂਕ_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', ਨਿਰਾ, &nul_ਪਦ_ਸਤਰ,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &ਮੰਜ਼ਿਲ_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.ਅਧਾਰ_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_ਸਿਰਾ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_ਸੂਚਕਾਂਕ_usage,
					 builtin_checkout_ਸੂਚਕਾਂਕ_options);
	repo_config(repo, git_default_config, ਨਿਰਾ);
	ਅਗੇਤਰ_ਲੰਮਾਈ = ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_ਭਰਿਆ_ਸੂਚਕਾਂਕ = 0;

	ਜੇ (repo_read_ਸੂਚਕਾਂਕ(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, ਅਗੇਤਰ, builtin_checkout_ਸੂਚਕਾਂਕ_options,
			builtin_checkout_ਸੂਚਕਾਂਕ_usage, 0);
	state.istate = repo->ਸੂਚਕਾਂਕ;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	ਜੇ (!state.ਅਧਾਰ_dir)
		state.ਅਧਾਰ_dir = "";
	state.ਅਧਾਰ_dir_len = ਲੜੀ_ਲੰਬਾਈ(state.ਅਧਾਰ_dir);

	ਜੇ (ਮੰਜ਼ਿਲ_tempfile < 0)
		ਮੰਜ਼ਿਲ_tempfile = (checkout_stage == CHECKOUT_ALL);
	ਜੇ (!ਮੰਜ਼ਿਲ_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	ਜੇ (ਸੂਚਕਾਂਕ_opt && !state.ਅਧਾਰ_dir_len && !ਮੰਜ਼ਿਲ_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->ਸੂਚਕਾਂਕ;
		repo_hold_locked_ਸੂਚਕਾਂਕ(repo, &ਤਾਲਾ_file,
				       ਤਾਲਾ_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_ਹੱਦ);
	ਜੇ (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	ਲਈ (i = 0; i < argc; i++) {
		ਅਚਲ ਅੱਖਰ *arg = argv[i];
		ਅੱਖਰ *p;

		ਜੇ (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		ਜੇ (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = ਅਗੇਤਰ_ਰਸਤਾ(repo, ਅਗੇਤਰ, ਅਗੇਤਰ_ਲੰਮਾਈ, arg);
		err |= checkout_file(repo->ਸੂਚਕਾਂਕ, p, ਅਗੇਤਰ);
		ਮੁਕਤ(p);
	}

	ਜੇ (read_from_stdin) {
		ਬਣਤਰ strbuf buf = STRBUF_INIT;
		ਬਣਤਰ strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		ਜੇ (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_ਪਦ_ਸਤਰ ? strbuf_getline_nul : strbuf_getline_lf;
		ਜਦੋਂ (getline_fn(&buf, stdin) != EOF) {
			ਅੱਖਰ *p;
			ਜੇ (!nul_ਪਦ_ਸਤਰ && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ਜੇ (unquote_c_style(&unquoted, buf.buf, ਨਿਰਾ))
					die("line is badly quoted");
				strbuf_ਅਦਲਾ_ਬਦਲੀ(&buf, &unquoted);
			}
			p = ਅਗੇਤਰ_ਰਸਤਾ(repo, ਅਗੇਤਰ, ਅਗੇਤਰ_ਲੰਮਾਈ, buf.buf);
			err |= checkout_file(repo->ਸੂਚਕਾਂਕ, p, ਅਗੇਤਰ);
			ਮੁਕਤ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	ਜੇ (all)
		err |= checkout_all(repo->ਸੂਚਕਾਂਕ, ਅਗੇਤਰ, ਅਗੇਤਰ_ਲੰਮਾਈ);

	ਜੇ (pc_workers > 1)
		err |= ਲੜੀਵਾਰ_parallel_checkout(&state, pc_workers, pc_ਹੱਦ,
					     ਨਿਰਾ, ਨਿਰਾ);

	ਜੇ (err)
		ਵਾਪਸ 1;

	ਜੇ (is_ਤਾਲਾ_file_locked(&ਤਾਲਾ_file) &&
	    write_locked_ਸੂਚਕਾਂਕ(repo->ਸੂਚਕਾਂਕ, &ਤਾਲਾ_file, COMMIT_ਤਾਲਾ))
		die("Unable to write new index file");
	ਵਾਪਸ 0;
}
