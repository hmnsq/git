/*
 * Check-out files from the "current cache directory"
 *
 * Copyright (C) 2005 Linus Torvalds
 *
 */

#define DISABLE_SIGN_COMPARE_WARNINGS

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

#define CHECKOUT_ALL 4
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ nul_term_ಸಾಲು;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout_stage; /* default to checkout stage0 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ignore_skip_worktree; /* default to 0 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ to_tempfile = -1;
ಸ್ಥಿರ ವರ್ಣ topath[4][TEMPORARY_FILENAME_LENGTH + 1];

ಸ್ಥಿರ ಸಂರಚನಾ checkout state = CHECKOUT_INIT;

ಸ್ಥಿರ ಶೂನ್ಯ write_tempfile_record(ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ have_tempname = 0;

	ಯದಿ (CHECKOUT_ALL == checkout_stage) {
		ಪ್ರತಿ (i = 1; i < 4; i++)
			ಯದಿ (topath[i][0]) {
				have_tempname = 1;
				ಭಙ್ಗ;
			}

		ಯದಿ (have_tempname) {
			ಪ್ರತಿ (i = 1; i < 4; i++) {
				ಯದಿ (i > 1)
					putchar(' ');
				ಯದಿ (topath[i][0])
					fputs(topath[i], stdout);
				ಅನ್ಯಥಾ
					putchar('.');
			}
		}
	} ಅನ್ಯಥಾ ಯದಿ (topath[checkout_stage][0]) {
		have_tempname = 1;
		fputs(topath[checkout_stage], stdout);
	}

	ಯದಿ (have_tempname) {
		putchar('\t');
		write_name_quoted_relative(name, prefix, stdout,
					   nul_term_ಸಾಲು ? '\0' : '\n');
	}

	ಪ್ರತಿ (i = 0; i < 4; i++) {
		topath[i][0] = 0;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout_file(ಸಂರಚನಾ index_state *index, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *prefix)
{
	ಪೂರ್ಣಾಙ್ಕ namelen = strlen(name);
	ಪೂರ್ಣಾಙ್ಕ pos = index_name_pos(index, name, namelen);
	ಪೂರ್ಣಾಙ್ಕ has_same_name = 0;
	ಪೂರ್ಣಾಙ್ಕ is_file = 0;
	ಪೂರ್ಣಾಙ್ಕ is_skipped = 1;
	ಪೂರ್ಣಾಙ್ಕ did_checkout = 0;
	ಪೂರ್ಣಾಙ್ಕ errs = 0;

	ಯದಿ (pos < 0)
		pos = -pos - 1;

	ಯಾವತ್ (pos < index->cache_nr) {
		ಸಂರಚನಾ cache_entry *ce = index->cache[pos];
		ಯದಿ (ce_namelen(ce) != namelen ||
		    memcmp(ce->name, name, namelen))
			ಭಙ್ಗ;
		has_same_name = 1;
		pos++;
		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode))
			ಭಙ್ಗ;
		is_file = 1;
		ಯದಿ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ಭಙ್ಗ;
		is_skipped = 0;
		ಯದಿ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ಅನುವರ್ತ;
		did_checkout = 1;
		ಯದಿ (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ರಿಕ್ತ,
				   ರಿಕ್ತ) < 0)
			errs++;
	}

	ಯದಿ (did_checkout) {
		ಯದಿ (to_tempfile)
			write_tempfile_record(name, prefix);
		ಪ್ರತ್ಯಾಗಮ errs > 0 ? -1 : 0;
	}

	/*
	 * At this point we know we didn't try to check anything out. If it was
	 * because we did find an entry but it was stage 0, that's not an
	 * error.
	 */
	ಯದಿ (has_same_name && checkout_stage == CHECKOUT_ALL)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!state.quiet) {
		fprintf(stderr, "git checkout-index: %s ", name);
		ಯದಿ (!has_same_name)
			fprintf(stderr, "is not in the cache");
		ಅನ್ಯಥಾ ಯದಿ (!is_file)
			fprintf(stderr, "is a sparse directory");
		ಅನ್ಯಥಾ ಯದಿ (is_skipped)
			fprintf(stderr, "has skip-worktree enabled; "
					"use '--ignore-skip-worktree-bits' to checkout");
		ಅನ್ಯಥಾ ಯದಿ (checkout_stage)
			fprintf(stderr, "does not exist at stage %d",
				checkout_stage);
		ಅನ್ಯಥಾ
			fprintf(stderr, "is unmerged");
		fputc('\n', stderr);
	}
	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ checkout_all(ಸಂರಚನಾ index_state *index, ಧ್ರುವ ವರ್ಣ *prefix, ಪೂರ್ಣಾಙ್ಕ prefix_length)
{
	ಪೂರ್ಣಾಙ್ಕ i, errs = 0;
	ಸಂರಚನಾ cache_entry *last_ce = ರಿಕ್ತ;

	ಪ್ರತಿ (i = 0; i < index->cache_nr ; i++) {
		ಸಂರಚನಾ cache_entry *ce = index->cache[i];

		ಯದಿ (S_ISSPARSEDIR(ce->ce_mode)) {
			ಯದಿ (!ce_skip_worktree(ce))
				BUG("sparse directory '%s' does not have skip-worktree set", ce->name);

			/*
			 * If the current entry is a sparse directory and skip-worktree
			 * entries are being checked out, expand the index and continue
			 * the loop on the current index position (now pointing to the
			 * first entry inside the expanded sparse directory).
			 */
			ಯದಿ (ignore_skip_worktree) {
				ensure_full_index(index);
				ce = index->cache[i];
			}
		}

		ಯದಿ (!ignore_skip_worktree && ce_skip_worktree(ce))
			ಅನುವರ್ತ;
		ಯದಿ (ce_stage(ce) != checkout_stage
		    && (CHECKOUT_ALL != checkout_stage || !ce_stage(ce)))
			ಅನುವರ್ತ;
		ಯದಿ (prefix && *prefix &&
		    (ce_namelen(ce) <= prefix_length ||
		     memcmp(prefix, ce->name, prefix_length)))
			ಅನುವರ್ತ;
		ಯದಿ (last_ce && to_tempfile) {
			ಯದಿ (ce_namelen(last_ce) != ce_namelen(ce)
			    || memcmp(last_ce->name, ce->name, ce_namelen(ce)))
				write_tempfile_record(last_ce->name, prefix);
		}
		ಯದಿ (checkout_entry(ce, &state,
				   to_tempfile ? topath[ce_stage(ce)] : ರಿಕ್ತ,
				   ರಿಕ್ತ) < 0)
			errs++;
		last_ce = ce;
	}
	ಯದಿ (last_ce && to_tempfile)
		write_tempfile_record(last_ce->name, prefix);
	ಪ್ರತ್ಯಾಗಮ !!errs;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ builtin_checkout_index_usage[] = {
	N_("git checkout-index [<options>] [--] [<file>...]"),
	ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ option_parse_stage(ಧ್ರುವ ಸಂರಚನಾ option *opt,
			      ಧ್ರುವ ವರ್ಣ *arg, ಪೂರ್ಣಾಙ್ಕ unset)
{
	ಪೂರ್ಣಾಙ್ಕ *stage = opt->ಮೌಲ್ಯ;

	BUG_ON_OPT_NEG(unset);

	ಯದಿ (!strcmp(arg, "all")) {
		*stage = CHECKOUT_ALL;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ ch = arg[0];
		ಯದಿ ('1' <= ch && ch <= '3')
			*stage = arg[0] - '0';
		ಅನ್ಯಥಾ
			die(_("stage should be between 1 and 3 or all"));
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ cmd_checkout_index(ಪೂರ್ಣಾಙ್ಕ argc,
		       ಧ್ರುವ ವರ್ಣ **argv,
		       ಧ್ರುವ ವರ್ಣ *prefix,
		       ಸಂರಚನಾ repository *repo)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	ಸಂರಚನಾ lock_file lock_file = LOCK_INIT;
	ಪೂರ್ಣಾಙ್ಕ all = 0;
	ಪೂರ್ಣಾಙ್ಕ read_from_stdin = 0;
	ಪೂರ್ಣಾಙ್ಕ prefix_length;
	ಪೂರ್ಣಾಙ್ಕ force = 0, quiet = 0, not_new = 0;
	ಪೂರ್ಣಾಙ್ಕ index_opt = 0;
	ಪೂರ್ಣಾಙ್ಕ err = 0;
	ಪೂರ್ಣಾಙ್ಕ pc_workers, pc_threshold;
	ಸಂರಚನಾ option builtin_checkout_index_options[] = {
		OPT_BOOL('a', "all", &all,
			N_("check out all files in the index")),
		OPT_BOOL(0, "ignore-skip-worktree-bits", &ignore_skip_worktree,
			N_("do not skip files with skip-worktree set")),
		OPT__FORCE(&force, N_("force overwrite of existing files"), 0),
		OPT__QUIET(&quiet,
			N_("no warning for existing files and files not in index")),
		OPT_BOOL('n', "no-create", &not_new,
			N_("don't checkout new files")),
		OPT_BOOL('u', "index", &index_opt,
			 N_("update stat information in the index file")),
		OPT_BOOL('z', ರಿಕ್ತ, &nul_term_ಸಾಲು,
			N_("paths are separated with NUL character")),
		OPT_BOOL(0, "stdin", &read_from_stdin,
			N_("read list of paths from the standard input")),
		OPT_BOOL(0, "temp", &to_tempfile,
			N_("write the content to temporary files")),
		OPT_STRING(0, "prefix", &state.base_dir, N_("string"),
			N_("when creating files, prepend <string>")),
		OPT_CALLBACK_F(0, "stage", &checkout_stage, "(1|2|3|all)",
			N_("copy out the files from named stage"),
			PARSE_OPT_NONEG, option_parse_stage),
		OPT_END()
	};

	show_usage_with_options_if_asked(argc, argv,
					 builtin_checkout_index_usage,
					 builtin_checkout_index_options);
	repo_config(repo, git_default_config, ರಿಕ್ತ);
	prefix_length = prefix ? strlen(prefix) : 0;

	prepare_repo_settings(repo);
	repo->settings.command_requires_full_index = 0;

	ಯದಿ (repo_read_index(repo) < 0) {
		die("invalid cache");
	}

	argc = parse_options(argc, argv, prefix, builtin_checkout_index_options,
			builtin_checkout_index_usage, 0);
	state.istate = repo->index;
	state.force = force;
	state.quiet = quiet;
	state.not_new = not_new;

	ಯದಿ (!state.base_dir)
		state.base_dir = "";
	state.base_dir_len = strlen(state.base_dir);

	ಯದಿ (to_tempfile < 0)
		to_tempfile = (checkout_stage == CHECKOUT_ALL);
	ಯದಿ (!to_tempfile && checkout_stage == CHECKOUT_ALL)
		die(_("options '%s' and '%s' cannot be used together"),
		    "--stage=all", "--no-temp");

	/*
	 * when --prefix is specified we do not want to update cache.
	 */
	ಯದಿ (index_opt && !state.base_dir_len && !to_tempfile) {
		state.refresh_cache = 1;
		state.istate = repo->index;
		repo_hold_locked_index(repo, &lock_file,
				       LOCK_DIE_ON_ERROR);
	}

	get_parallel_checkout_configs(&pc_workers, &pc_threshold);
	ಯದಿ (pc_workers > 1)
		init_parallel_checkout();

	/* Check out named files first */
	ಪ್ರತಿ (i = 0; i < argc; i++) {
		ಧ್ರುವ ವರ್ಣ *arg = argv[i];
		ವರ್ಣ *p;

		ಯದಿ (all)
			die("git checkout-index: don't mix '--all' and explicit filenames");
		ಯದಿ (read_from_stdin)
			die("git checkout-index: don't mix '--stdin' and explicit filenames");
		p = prefix_path(repo, prefix, prefix_length, arg);
		err |= checkout_file(repo->index, p, prefix);
		ಮುಕ್ತ(p);
	}

	ಯದಿ (read_from_stdin) {
		ಸಂರಚನಾ strbuf buf = STRBUF_INIT;
		ಸಂರಚನಾ strbuf unquoted = STRBUF_INIT;
		strbuf_getline_fn getline_fn;

		ಯದಿ (all)
			die("git checkout-index: don't mix '--all' and '--stdin'");

		getline_fn = nul_term_ಸಾಲು ? strbuf_getline_nul : strbuf_getline_lf;
		ಯಾವತ್ (getline_fn(&buf, stdin) != EOF) {
			ವರ್ಣ *p;
			ಯದಿ (!nul_term_ಸಾಲು && buf.buf[0] == '"') {
				strbuf_reset(&unquoted);
				ಯದಿ (unquote_c_style(&unquoted, buf.buf, ರಿಕ್ತ))
					die("line is badly quoted");
				strbuf_swap(&buf, &unquoted);
			}
			p = prefix_path(repo, prefix, prefix_length, buf.buf);
			err |= checkout_file(repo->index, p, prefix);
			ಮುಕ್ತ(p);
		}
		strbuf_release(&unquoted);
		strbuf_release(&buf);
	}

	ಯದಿ (all)
		err |= checkout_all(repo->index, prefix, prefix_length);

	ಯದಿ (pc_workers > 1)
		err |= run_parallel_checkout(&state, pc_workers, pc_threshold,
					     ರಿಕ್ತ, ರಿಕ್ತ);

	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಯದಿ (is_lock_file_locked(&lock_file) &&
	    write_locked_index(repo->index, &lock_file, COMMIT_LOCK))
		die("Unable to write new index file");
	ಪ್ರತ್ಯಾಗಮ 0;
}
