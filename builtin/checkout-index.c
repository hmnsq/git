/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#ವ್ಯಾಖ್ಯಾನಿಸು DISABLE_SIGN_COMPARE_WARNINGS

#ಅನ್ತರ್ಭಾವ "builtin.h"
#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "environment.h"
#ಅನ್ತರ್ಭಾವ "gettext.h"
#ಅನ್ತರ್ಭಾವ "lockfile.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "cache-tree.h"
#ಅನ್ತರ್ಭಾವ "parse-options.h"
#ಅನ್ತರ್ಭಾವ "entry.h"
#ಅನ್ತರ್ಭಾವ "parallel-checkout.h"
#ಅನ್ತರ್ಭಾವ "read-cache-ll.h"
#ಅನ್ತರ್ಭಾವ "setup.h"
#ಅನ್ತರ್ಭಾವ "sparse-index.h"

#ವ್ಯಾಖ್ಯಾನಿಸು CHECKOUT_ALL 4
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ nul_ಪದ_ಸಾಲು;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_stage; /* default to checkout stage0 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ignore_skip_worktree; /* default to 0 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಮ್ಯ_tempfile = -1;
ಸ್ಥಿರ ಅಕ್ಷರ topath[4][TEMPORARY_FILENAME_ಉದ್ದಳತೆ + 1];

ಸ್ಥಿರ ರಚನೆ checkout state = CHECKOUT_INIT;

ಸ್ಥಿರ ಶೂನ್ಯ write_tempfile_ದಾಖಲೆ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ have_tempname = 0;

	ಆದರೆ (CHECKOUT_ALL == checkout_stage) {
		ಗಾಗಿ (i = 1; i < 4; i++)
			ಆದರೆ (topath[i][0]) {
				have_tempname = 1;
				ನಿಲ್ಲಿಸು;
			}

		ಆದರೆ (have_tempname) {
			ಗಾಗಿ (i = 1; i < 4; i++) {
				ಆದರೆ (i > 1)
					ಅಕ್ಷರ_ಬರೆ(' ');
				ಆದರೆ (topath[i][0])
					fputs(topath[i], stdout);
				ಇಲ್ಲದಿದ್ದರೆ
					ಅಕ್ಷರ_ಬರೆ('.');
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	ಆದರೆ (have_tempname) {
		ಅಕ್ಷರ_ಬರೆ('\t');
		write_name_quoted_relative(name, ಪೂರ್ವಪ್ರತ್ಯಯ, stdout,
					   nul_ಪದ_ಸಾಲು ? '\0' : '\n');
	}

	ಗಾಗಿ (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_file(ರಚನೆ ಸೂಚ್ಯಂಕ_state *ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ಪೂರ್ಣಾಂಕ namelen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name);
	ಪೂರ್ಣಾಂಕ pos = ಸೂಚ್ಯಂಕ_name_pos(ಸೂಚ್ಯಂಕ, name, namelen);
	ಪೂರ್ಣಾಂಕ has_same_name = 0;
	ಪೂರ್ಣಾಂಕ is_file = 0;
	ಪೂರ್ಣಾಂಕ is_skipped = 1;
	ಪೂರ್ಣಾಂಕ did_checkout = 0;
	ಪೂರ್ಣಾಂಕ errs = 0;

	ಆದರೆ (pos < 0)
		pos = -pos - 1;

	ಆಗ (pos < ಸೂಚ್ಯಂಕ->cache_nr) {
		ರಚನೆ cache_ನಮೂದು *ce = ಸೂಚ್ಯಂಕ->cache[pos];
		ಆದರೆ (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			ನಿಲ್ಲಿಸು;
		has_same_name = 1;
		pos++;
		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ))
			ನಿಲ್ಲಿಸು;
		is_file = 1;
		ಆದರೆ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ನಿಲ್ಲಿಸು;
		is_skipped = 0;
		ಆದರೆ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ಮುಂದುವರಿ;
		did_checkout = 1;
		ಆದರೆ (checkout_ನಮೂದು(ce, &state,
				   ಗಮ್ಯ_tempfile ? topath[ce_stage(ce)] : ಖಾಲಿ,
				   ಖಾಲಿ) < 0)
			errs++;
	}

	ಆದರೆ (did_checkout) {
		ಆದರೆ (ಗಮ್ಯ_tempfile)
			write_tempfile_ದಾಖಲೆ(name, ಪೂರ್ವಪ್ರತ್ಯಯ);
		ಹಿಂತಿರುಗಿ errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	ಆದರೆ (has_same_name && checkout_stage == CHECKOUT_ALL)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		ಆದರೆ (!has_same_name)
			fprintf(stderr, "is not in the cache");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!is_file)
			fprintf(stderr, "is a sparse directory");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		ಇಲ್ಲದಿದ್ದರೆ
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ checkout_all(ರಚನೆ ಸೂಚ್ಯಂಕ_state *ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ)
{
	ಪೂರ್ಣಾಂಕ i, errs = 0;
	ರಚನೆ cache_ನಮೂದು *ಕೊನೆಯ_ce = ಖಾಲಿ;

	ಗಾಗಿ (i = 0; i < ಸೂಚ್ಯಂಕ->cache_nr ; i++) {
		ರಚನೆ cache_ನಮೂದು *ce = ಸೂಚ್ಯಂಕ->cache[i];

		ಆದರೆ (S_ISSPARSEDIR(ce->ce_ಬಹುಲಕ)) {
			ಆದರೆ (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			ಆದರೆ (ignore_skip_worktree) {
				ensure_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ(ಸೂಚ್ಯಂಕ);
				ce = ಸೂಚ್ಯಂಕ->cache[i];
			}
		}

		ಆದರೆ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ಮುಂದುವರಿ;
		ಆದರೆ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ಮುಂದುವರಿ;
		ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ && *ಪೂರ್ವಪ್ರತ್ಯಯ &&
		    (ce_namelen(ce) <= ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ ||
		     memcmp(ಪೂರ್ವಪ್ರತ್ಯಯ, ce->name, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ)))
			ಮುಂದುವರಿ;
		ಆದರೆ (ಕೊನೆಯ_ce && ಗಮ್ಯ_tempfile) {
			ಆದರೆ (ce_namelen(ಕೊನೆಯ_ce) != ce_namelen(ce)
			    || memcmp(ಕೊನೆಯ_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_ದಾಖಲೆ(ಕೊನೆಯ_ce->name, ಪೂರ್ವಪ್ರತ್ಯಯ);
		}
		ಆದರೆ (checkout_ನಮೂದು(ce, &state,
				   ಗಮ್ಯ_tempfile ? topath[ce_stage(ce)] : ಖಾಲಿ,
				   ಖಾಲಿ) < 0)
			errs++;
		ಕೊನೆಯ_ce = ce;
	}
	ಆದರೆ (ಕೊನೆಯ_ce && ಗಮ್ಯ_tempfile)
		write_tempfile_ದಾಖಲೆ(ಕೊನೆಯ_ce->name, ಪೂರ್ವಪ್ರತ್ಯಯ);
	ಹಿಂತಿರುಗಿ !!errs;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ builtin_checkout_ಸೂಚ್ಯಂಕ_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ option_parse_stage(ಸ್ಥಿರಾಂಕ ರಚನೆ option *opt,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg, ಪೂರ್ಣಾಂಕ unset)
{
	ಪೂರ್ಣಾಂಕ *stage = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಆದರೆ (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ ch = arg[0];
		ಆದರೆ ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		ಇಲ್ಲದಿದ್ದರೆ
			die(_("stage should be between 1 and 3 or all"));
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ cmd_checkout_ಸೂಚ್ಯಂಕ(ಪೂರ್ಣಾಂಕ argc,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
		       ರಚನೆ repository *repo)
{
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ ಬೀಗ_file ಬೀಗ_file = ಬೀಗ_INIT;
	ಪೂರ್ಣಾಂಕ all = 0;
	ಪೂರ್ಣಾಂಕ read_from_stdin = 0;
	ಪೂರ್ಣಾಂಕ ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ;
	ಪೂರ್ಣಾಂಕ force = 0, quiet = 0, not_new = 0;
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ_opt = 0;
	ಪೂರ್ಣಾಂಕ err = 0;
	ಪೂರ್ಣಾಂಕ pc_workers, pc_ಮಿತಿ;
	ರಚನೆ option builtin_checkout_ಸೂಚ್ಯಂಕ_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &ಸೂಚ್ಯಂಕ_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', ಖಾಲಿ, &nul_ಪದ_ಸಾಲು,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &ಗಮ್ಯ_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.ತಳ_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_ಕೊನೆ()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_ಸೂಚ್ಯಂಕ_usage,
					 builtin_checkout_ಸೂಚ್ಯಂಕ_options);
	repo_config(repo, git_default_config, ಖಾಲಿ);
	ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ = ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_ತುಂಬಿದ_ಸೂಚ್ಯಂಕ = 0;

	ಆದರೆ (repo_read_ಸೂಚ್ಯಂಕ(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, ಪೂರ್ವಪ್ರತ್ಯಯ, builtin_checkout_ಸೂಚ್ಯಂಕ_options,
			builtin_checkout_ಸೂಚ್ಯಂಕ_usage, 0);
	state.istate = repo->ಸೂಚ್ಯಂಕ;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	ಆದರೆ (!state.ತಳ_dir)
		state.ತಳ_dir = "";
	state.ತಳ_dir_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(state.ತಳ_dir);

	ಆದರೆ (ಗಮ್ಯ_tempfile < 0)
		ಗಮ್ಯ_tempfile = (checkout_stage == CHECKOUT_ALL);
	ಆದರೆ (!ಗಮ್ಯ_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	ಆದರೆ (ಸೂಚ್ಯಂಕ_opt && !state.ತಳ_dir_len && !ಗಮ್ಯ_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->ಸೂಚ್ಯಂಕ;
		repo_hold_locked_ಸೂಚ್ಯಂಕ(repo, &ಬೀಗ_file,
				       ಬೀಗ_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_ಮಿತಿ);
	ಆದರೆ (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	ಗಾಗಿ (i = 0; i < argc; i++) {
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = argv[i];
		ಅಕ್ಷರ *p;

		ಆದರೆ (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		ಆದರೆ (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(repo, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ, arg);
		err |= checkout_file(repo->ಸೂಚ್ಯಂಕ, p, ಪೂರ್ವಪ್ರತ್ಯಯ);
		ಬಿಡುಗಡೆ(p);
	}

	ಆದರೆ (read_from_stdin) {
		ರಚನೆ strbuf buf = STRBUF_INIT;
		ರಚನೆ strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		ಆದರೆ (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_ಪದ_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
		ಆಗ (getline_fn(&buf, stdin) != EOF) {
			ಅಕ್ಷರ *p;
			ಆದರೆ (!nul_ಪದ_ಸಾಲು && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ಆದರೆ (unquote_c_style(&unquoted, buf.buf, ಖಾಲಿ))
					die("line is badly quoted");
				strbuf_ಅದಲುಬದಲು(&buf, &unquoted);
			}
			p = ಪೂರ್ವಪ್ರತ್ಯಯ_ಮಾರ್ಗ(repo, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ, buf.buf);
			err |= checkout_file(repo->ಸೂಚ್ಯಂಕ, p, ಪೂರ್ವಪ್ರತ್ಯಯ);
			ಬಿಡುಗಡೆ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	ಆದರೆ (all)
		err |= checkout_all(repo->ಸೂಚ್ಯಂಕ, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ);

	ಆದರೆ (pc_workers > 1)
		err |= ಸತತ_parallel_checkout(&state, pc_workers, pc_ಮಿತಿ,
					     ಖಾಲಿ, ಖಾಲಿ);

	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (is_ಬೀಗ_file_locked(&ಬೀಗ_file) &&
	    write_locked_ಸೂಚ್ಯಂಕ(repo->ಸೂಚ್ಯಂಕ, &ಬೀಗ_file, COMMIT_ಬೀಗ))
		die("Unable to write new index file");
	ಹಿಂತಿರುಗಿ 0;
}
